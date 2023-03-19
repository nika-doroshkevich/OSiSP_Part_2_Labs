#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <tchar.h>

using namespace std;

HANDLE GetProcessByName(PCWSTR name);
void InjectLib(HANDLE hProc, LPCVOID param, DWORD paramSize, const char* libName, const char* functionName);
void LoadLibToThread(HANDLE hProc, const char* loadingLibName);

typedef struct
{
    char srcString[20];
    char resString[20];

} libInfo;

int main(){

    HANDLE hProc = GetProcessByName(L"TestInjection.exe"); //к какому процессу подключить длл
    if (hProc == NULL) {
        return -1;
    }

    LoadLibToThread(hProc, "LineReplacementLibrary.dll");
    return 0;
}

HANDLE GetProcessByName(PCWSTR name)
{
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    // Walkthrough all processes.
    if (Process32First(snapshot, &process))
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
            // trim .exe if necessary, etc.
            if (std::wstring(process.szExeFile) == std::wstring(name))
            {
                pid = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0)
    {
        return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    // Not found


    return NULL;
}

void LoadLibToThread(HANDLE hProc, const char* loadingLibName){
    int paramLength = strlen(loadingLibName) + 1;
    int paramSize = paramLength * sizeof(CHAR);
   
    InjectLib(hProc, loadingLibName, paramSize, "Kernel32.dll", "LoadLibraryA");
}

void InjectLib(HANDLE hProc, LPCVOID param, DWORD paramSize, const char* libName, const char* functionName){
    HANDLE hThread;
    PCSTR virtPath;

    virtPath = (PCSTR)VirtualAllocEx(hProc, NULL, strlen((const char *) functionName) + 1, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    if (virtPath == NULL)
    {
        CloseHandle(hProc);
        return;
    }

    if (!WriteProcessMemory(hProc, (LPVOID)virtPath, param, paramSize, NULL)) //если не получилось записать в процесс
    {
        VirtualFreeEx(hProc, (LPVOID)virtPath, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return;
    }

    PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE) //создаем поток
        GetProcAddress(GetModuleHandleA(libName), functionName);

    if (pfnThreadRtn == NULL)
    {
        VirtualFreeEx(hProc, (LPVOID)virtPath, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return;
    };

    hThread = CreateRemoteThread(hProc, NULL, 0, (PTHREAD_START_ROUTINE)pfnThreadRtn, (LPVOID)virtPath, 0, NULL); //создаем дистанционный поток
    if (hThread == NULL)
    {
        cout << GetLastError();
        VirtualFreeEx(hProc, (LPVOID)virtPath, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return;
    }

    WaitForSingleObject(hThread, INFINITE); //ждет сигнала от потока

    if (virtPath != NULL)
        VirtualFreeEx(hProc, (LPVOID)virtPath, 0, MEM_RELEASE);
    if (hThread != NULL)
        CloseHandle(hThread);
    if (hProc != NULL)
        CloseHandle(hProc);
}
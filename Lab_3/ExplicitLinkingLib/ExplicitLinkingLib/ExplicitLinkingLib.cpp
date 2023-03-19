#include <iostream>
#include <Windows.h>

typedef INT __cdecl ReplaceFunction(DWORD pid, const char* srcStr, const char* repStr);
void ReplaceString(DWORD pid, const char* srcString, const char* resString);

using namespace std;

int main() {
	const DWORD processId = GetCurrentProcessId(); //получение дескриптора процесса
	const char srcStr[] = "Old String";
	const char repStr[] = "New String";
	ReplaceString(processId, srcStr, repStr);

	cout << srcStr << endl;
}

void ReplaceString(DWORD pid, const char* srcString, const char* resString) {

	HMODULE hDll = LoadLibrary(L"LineReplacementLibrary");

	if (hDll == NULL) {
		cout << "Error load dll: " << GetLastError() << endl;
	}
	else {

		ReplaceFunction* replaceFunction = (ReplaceFunction*)GetProcAddress(hDll, "ReplaceString");
		if (replaceFunction != NULL)
		{
			replaceFunction(pid, srcString, resString);
		}
		else
		{
			std::cout << "Error load function: " << GetLastError() << endl;
		}

		FreeLibrary(hDll);
	}
}
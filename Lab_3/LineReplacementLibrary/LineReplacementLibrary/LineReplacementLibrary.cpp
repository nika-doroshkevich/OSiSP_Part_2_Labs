#include "pch.h"
#include "LineReplacementLibrary.h"


extern "C" __declspec(dllexport) INT __cdecl ReplaceString(DWORD processId, const char* srcStr, const char* repStr) {

	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processId);

	if (hProcess == NULL) {
		return 1;
	}

	// get maximum address range from system info 
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// get region size
	char* currAddr = 0;
	MEMORY_BASIC_INFORMATION mbi;
	std::vector<char> buffer;

	// process addresses cycle
	while (currAddr < si.lpMaximumApplicationAddress) {

		// get virtual memory pages and fill mbi
		if (VirtualQueryEx(hProcess, currAddr, &mbi, sizeof(mbi)) == 0) {
			break;	
		}

		/* indicate pages for which physical storage has been allocated
			and have read-only or read/write access */
		if (mbi.State == MEM_COMMIT && mbi.AllocationProtect == PAGE_READWRITE) {

			size_t srcLength = strlen(srcStr);
			size_t repLength = strlen(repStr);

			currAddr = (char*)mbi.BaseAddress;
			buffer.resize(mbi.RegionSize);
			SIZE_T nReadBytes;

			try {

				// read region
				if (ReadProcessMemory(hProcess, currAddr, &buffer[0], mbi.RegionSize, &nReadBytes)) {
					
					// replace str
					for (size_t i = 0; i < (nReadBytes - srcLength); i++)
					{

						if (memcmp(srcStr, &buffer[i], srcLength) == 0)
						{
							char* ch = (char*)currAddr + i;
							for (int j = 0; j < repLength; j++)
							{
								ch[j] = repStr[j];
							}

							ch[repLength] = 0;
						}
					}
				}

			}
			catch (std::bad_alloc& e) {

			}
		}

		currAddr += mbi.RegionSize;
	}

	CloseHandle(hProcess);
	return 0;
}

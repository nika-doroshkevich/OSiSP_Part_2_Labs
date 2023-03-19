#include <iostream>
#include <Windows.h>
#include "..\..\LineReplacementLibrary\LineReplacementLibrary\LineReplacementLibrary.h"

int main()
{
	using namespace std;

	const DWORD processId = GetCurrentProcessId();
	const char srcStr[] = "Smth Old";
	const char repStr[] = "Smth New";
	ReplaceString(processId, srcStr, repStr);

	cout << srcStr << endl;
}


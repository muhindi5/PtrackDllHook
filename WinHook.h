#pragma once

#ifdef _cplusplus
extern "C" {
	//only need to export C interface if used by 
	// C++ code

#endif


#include <Windows.h>

_declspec(dllexport) LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);

#ifdef _cplusplus
}
#endif
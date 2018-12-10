#ifndef __MAIN_H__
#define __MAIN_H__

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <tchar.h>

// C RunTime Header Files
#include "platform/CCStdC.h"

#endif    // __MAIN_H__
#endif 

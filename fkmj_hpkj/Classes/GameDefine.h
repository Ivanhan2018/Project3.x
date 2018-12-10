#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"

typedef byte			BYTE;
typedef longlong		LONGLONG;
typedef tchar			TCHAR;
typedef int				INT;
typedef double			DOUBLE;
typedef int				QYLONG;
typedef unsigned short	WORD;
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
typedef unsigned int	DWORD;
#endif

#define MAX_PATH 260

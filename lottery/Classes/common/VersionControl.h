#ifndef VERSION_DEFINE_FILE
#define VERSION_DEFINE_FILE
#pragma once

#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;

//app版本 a ver_9yi | b ver_qianbo | c ver_369
//#define VER_9YI
//#define VER_QIANBO
#define VER_369

//游戏相关
#define USE_ZJH
#define USE_DZPK
#define USE_DDZ
#define USE_BJL
#define USE_FISHBEAT
#define USE_PPC		//	碰碰车
#define USE_BRNN	//	百人牛牛
#define USE_SGLY	//	水果乐园

//资源热更新
//#define USE_HOT_UPDATE
//支持选房间
#define USE_ROOM_LIST

//版本控制
#ifdef VER_QIANBO

#define BALL_RADDIUS 84
#define HEAD_IMG "beark2.png"
#define HE_IMG "gamble_00.png"

#endif

#ifdef VER_369

#define BALL_RADDIUS 84
#define HEAD_IMG "beark2.png"
#define HE_IMG "gamble_00.png"

#endif

#ifdef VER_9YI

#define BALL_RADDIUS 78
#define HEAD_IMG "room_01.png"
#define HE_IMG "gamble_00.png"

#endif

#endif //VERSION_DEFINE_FILE
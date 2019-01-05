/*
 *  platform.h
 *  test
 *
 *  Created by Aden on 14-11-20.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#pragma pack(1)

#define	BUILD_VER	0	// 授权版本
#define PRODUCT_VER 6	// 产品版本

// 借口版本
#define INTERFACE_VERSION(cbMainVer,cbSubVer)			\
		(((BYTE)(PRODUCT_VER))<<24)+					\
		(((BYTE)(cbMainVer))<<16)+						\
		((BYTE)(cbSubVer)<<8)+							\
		(BYTE)(BUILD_VER))			

// 模块版本
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)  \
		(DWORD)(										\
		(((BYTE)(PRODUCT_VER))<<24)+					\
		(((BYTE)(cbMainVer))<<16)+						\
		 ((BYTE)(cbSubVer)<<8)+							\
		  (BYTE)(cbBuildVer))
		 
// 程序版本
#define VERSION_FRAME			PROCESS_VERSION(6,0,3)				// 框架版本
#define VERSION_PLAZA			PROCESS_VERSION(10,0,3)				// 大厅版本

//非凡
//#define VERSION_MOBILE			PROCESS_VERSION(29,0,6)				// 手机版本
//#define VERSION_ROOM			PROCESS_VERSION(29,0,6)				// 手机版本
//九亿
#define VERSION_MOBILE			PROCESS_VERSION(29,0,16)				// 手机版本
#define VERSION_ROOM			PROCESS_VERSION(29,0,6)				// 手机版本
//七星
//#define VERSION_MOBILE			PROCESS_VERSION(21,0,6)				// 手机版本
//#define VERSION_ROOM			PROCESS_VERSION(21,0,6)				// 手机版本

// 版本定义
#define VERSION_EFFICACY		0									// 效验版本
#define VERSION_FRAME_SDK		INTERFACE_VERSION(6,3)				// 框架版本



#pragma pack()
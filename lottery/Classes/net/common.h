/*
 *  common.h
 *  stream
 *
 *  Created by WJ on 14-11-25.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#ifndef COMMON_LIB_H
#define COMMON_LIB_H

#include"Define.h"
//隐射数据
bool Common_MappedBuffer(void* data, int nDataSize);

//隐射数据
bool Common_unMappedBuffer(void* data, int nDataSize);

//网络验证
bool Common_Compilation(TCHART *data);


#endif
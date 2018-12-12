
/*
 *  ValueSpace.h
 *  test
 *
 *  Created by Aden on 11-4-1.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "common.h"
#include "GlobalDef.h"
#include "IDataArchive.h"
#include "Define.h"
#include "CMD_LogonServer.h"
#include "packet.h"
#include "cocos2d.h"
#include "MyNSString.h"
using namespace cocos2d;

//加密数据
bool Common_MappedBuffer(void* data, int nDataSize)
{
	//变量定义
	BYTE *buffer	 = (BYTE*)data;
	//效验码与字节映射
	BYTE cbCheckCode = 0;

	for (WORD i = sizeof(CMD_Info); i < nDataSize; i++)
	{
		cbCheckCode += buffer[i];

		buffer[i] = g_SendByteMap[buffer[i]];
	}
	
	//设置数据
	CMD_Info *pInfo	 = (CMD_Info*)data;
	pInfo->cbCheckCode = ~cbCheckCode+1;
	pInfo->wPacketSize = nDataSize;
	pInfo->cbVersion |= DK_MAPPED;

	return true;
}

//解密数据
bool Common_unMappedBuffer(void* data, int nDataSize)
{
	//变量定义
	BYTE* buffer=(BYTE*)data;
	CMD_Info* pInfo=(CMD_Info*)data;
	
	//映射
	if( (pInfo->cbVersion & DK_MAPPED) !=0)
	{
		BYTE cbCheckCode = pInfo->cbCheckCode;
		
		for(WORD i=sizeof(CMD_Info);i<nDataSize;i++)
		{
			cbCheckCode += g_RecvByteMap[buffer[i]];
			buffer[i] = g_RecvByteMap[buffer[i]];
		}
		//效验
		if(cbCheckCode!=0)
			return false;
	}
	return true;
}

// important
char szCompilatio[] = "345E2FFA-891E-4021-A57E-80ECF3466896";

bool Common_Compilation(TCHART *data)
{	
	MyNSString::mbs2wc(szCompilatio, sizeof(szCompilatio), data);
	return true;
}

































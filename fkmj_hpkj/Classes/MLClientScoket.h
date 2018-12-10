/****************************************************************************
Copyright (c) 2014 Lijunlin - Jason lee

Created by Lijunlin - Jason lee on 2014

jason.lee.c@foxmail.com
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCNET_SOCKET_H__
#define __CCNET_SOCKET_H__

#include "cocos2d.h"
#include "MLNetDefine.h"
#include "MLClientInetAddress.h"
#include <string>

NS_CC_BEGIN

/**
 * calss  : CCSocket
 * author : Jason lee
 * email  : jason.lee.c@foxmail.com
 * descpt : socket manager object
 */
class MLClientScoket
{
public:
	MLClientScoket();
	virtual ~MLClientScoket();
	bool ccInit();
	int  ccRead(char* _buff, int _len);
	int  ccWrite(char* _buff, int _len);
	int  ccIsConnected();
	bool ccIsSocketVaild(bool bRecive);
	void ccClose();
	bool ccConnect();
	void ccDisconnect();
	void setInetAddress(const MLClientInetAddress& oInetAddress);

public:
	SOCKET m_uSocket;
	MLClientInetAddress m_oInetAddress;
};


NS_CC_END

#endif //__CCNET_SOCKET_H__
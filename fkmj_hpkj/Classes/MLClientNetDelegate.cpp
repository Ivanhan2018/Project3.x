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
#include "MLClientNetDelegate.h"

NS_CC_BEGIN

MLClientNetDelegate::MLClientNetDelegate()
: m_fSoTimeout(SOCKET_SOTIMEOUT)
, m_eStatus(eSocketIoClosed)
, m_fConnectingDuration(0.0f)
, m_iSendCout(0)
{
	m_pSendBuffer = new char[SOCKET_READ_BUFFER_SIZE];
	m_pReadBuffer = new char[SOCKET_READ_BUFFER_SIZE];

	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(MLClientNetDelegate::runSchedule),this,0.0f,false);
}

MLClientNetDelegate::~MLClientNetDelegate()
{
	delete[] m_pReadBuffer;
	delete[] m_pSendBuffer;
	m_oSocket.ccClose();
	cocos2d::Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(MLClientNetDelegate::runSchedule),this);
}

void MLClientNetDelegate::setInetAddress(const MLClientInetAddress& oInetAddress)
{
	m_oInetAddress = oInetAddress;
}

const MLClientInetAddress& MLClientNetDelegate::getInetAddress() const
{
	return m_oInetAddress;
}

void MLClientNetDelegate::setSoTimeout(float fSoTimeout)
{
	m_fSoTimeout = fSoTimeout;
}

float MLClientNetDelegate::getSoTimeout() const
{
	return m_fSoTimeout;
}

void MLClientNetDelegate::send(const char* pData,int iSize)
{
	if( iSize == 0 || !isConnected() )
		return;

	memcpy(&(m_pSendBuffer[m_iSendCout]), pData, iSize);
	m_iSendCout+=iSize;
}

bool MLClientNetDelegate::isConnected()
{
	return m_eStatus == eSocketConnected;
}

bool MLClientNetDelegate::connect()
{
	m_iSendCout = 0;
	if( m_eStatus != eSocketConnected && m_eStatus != eSocketConnecting )
	{
		m_oSocket.setInetAddress(m_oInetAddress);
		if( m_oSocket.ccConnect() )
		{
			m_eStatus = eSocketConnecting;
			return true;
		}
		else
		{
			onExceptionCaught(eSocketConnectFailed);
			return false;
		}
	}
	return false;
}

void MLClientNetDelegate::disconnect()
{
	if( m_eStatus == eSocketConnected )
	{
		m_oSocket.ccDisconnect();
		m_eStatus = eSocketDisconnected;
		onDisconnected();
	}
}

void MLClientNetDelegate::close()
{
	if( m_eStatus == eSocketConnected )
	{
		m_oSocket.ccClose();
		m_eStatus = eSocketIoClosed;
		onDisconnected();
	}
}

void MLClientNetDelegate::runSchedule(float dt)
{
	switch( m_eStatus )
	{
	case eSocketConnecting:
		{
			switch( m_oSocket.ccIsConnected() )
			{
			case eSocketConnected:
				{
					m_eStatus = eSocketConnected;
					onConnected();
				}
				break;
			case eSocketConnectFailed:
				{
                    m_oSocket.ccClose();
					m_eStatus = eSocketConnectFailed;
					onExceptionCaught(eSocketConnectFailed);
				}
				break;
			case eSocketConnecting:
				{
					if( m_fConnectingDuration > m_fSoTimeout )
					{
						m_oSocket.ccDisconnect();
						m_eStatus = eSocketDisconnected;
						onConnectTimeout();
						m_fConnectingDuration = 0.0f;
					}
					else
					{
						m_fConnectingDuration += dt;
					}
					
				}
				break;
			default:
				break;
			}
		}
		break;
	case eSocketConnected:
		{
#if HANDLE_ON_SINGLE_FRAME
			while( m_oSocket.ccIsSocketVaild(true) )
#else
			if( m_oSocket.ccIsSocketVaild(true) )
#endif
			{
				if( this->runRead() ) return;
			}

#if HANDLE_ON_SINGLE_FRAME
			while( m_oSocket.ccIsSocketVaild(false) && m_iSendCout )
#else
			if( m_oSocket.ccIsSocketVaild(false) && !m_pSendBuffer.empty() )
#endif
			{
				if( this->runWrite() ) return;
			}
		}
		break;
	default:
		break;
	}	
}

bool MLClientNetDelegate::runRead()
{
	memset(m_pReadBuffer,0,SOCKET_READ_BUFFER_SIZE);
	int nRet = m_oSocket.ccRead(m_pReadBuffer, SOCKET_READ_BUFFER_SIZE);
	if( nRet == eSocketIoError || nRet == eSocketIoClosed )
	{
		m_oSocket.ccClose();
		m_eStatus = eSocketIoClosed;
		onExceptionCaught(eSocketIoError);
		return true;
	}
	else
	{
#if DEBUG_SOCKET_DATA_INFO
		CCLOG("CCSOCKET READ %d", nRet);
#endif
		onMessageReceived(m_pReadBuffer,nRet);
	}
	return false;
}

bool MLClientNetDelegate::runWrite()
{
	int nRet = m_oSocket.ccWrite(&m_pSendBuffer[0], m_iSendCout);
#if DEBUG_SOCKET_DATA_INFO
	CCLOG("CCSOCKET WRITE %d", nRet);
#endif
	if( nRet == eSocketIoError )
	{
		m_oSocket.ccClose();
		m_eStatus = eSocketIoClosed;
		onExceptionCaught(eSocketIoClosed);
		return true;
	}
	else if( nRet >= m_iSendCout)
	{
		m_iSendCout = 0;
	}
	else
	{
		m_iSendCout -= nRet;
	}
	return false;
}

NS_CC_END
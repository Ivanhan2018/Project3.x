//
//  TcpNetwork.h
//  SDH
//
//  Created by zhouwei on 13-6-6.
//
//

#ifndef __SDH__TcpNetwork__
#define __SDH__TcpNetwork__

#define SOCKET_TCP_VER 0x01


#include <iostream>
#include "Define.h"
#include "BaseObject.h"
#include "GBEvent.h"
#include "GC_Socket.h"

//class MsgDispatch;
class ClientSock : public BaseObject
{
public:

	ClientSock();
	~ClientSock();

	void update();

	//判断释放连接服务器
	bool isServerConected();

	//连接服务器
	bool connectServer(const char* ip, unsigned short port);

	void sendData(WORD wMainCmdID,WORD wSubCmdID);

	void sendData(void* pData,WORD wDataSize,WORD wMainCmdID,WORD wSubCmdID,WORD wRountID);

	void sendData(WORD wMainCmdID,WORD wSubCmdID,void* pData,DWORD wDataSize);

	void recvData(GBEventArg& arg);

	DWORD SendDataBuffer(void * pBuffer, WORD wSendSize);

	WORD encryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize);

	WORD crevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize);

	//随机映射
	WORD SeedRandMap(WORD wSeed);

	//映射发送数据
	BYTE MapSendByte(BYTE const cbData);

	//映射接收数据
	BYTE MapRecvByte(BYTE const cbData);

	//重置
	void reset();

	//关闭套接字
	void closeSocket();

public:
	//void setDispatch(MsgDispatch* p) { m_pMsgDispatch = p; }
private:
	static IGC_SocketStream* m_pISocketStrem;

	//增加自动重连功能；
	char	m_szIpaddr[20];
	short	m_nPort;

private:
	BYTE  m_cbSendRound;
	BYTE  m_cbRecvRound;
	DWORD m_dwSendXorKey;
	DWORD m_dwRecvXorKey;
	DWORD m_dwSendPacketCount;
	DWORD m_dwRecvPacketCount;
	
	short m_netstate;
};


#endif /* defined(__SDH__TcpNetwork__) */

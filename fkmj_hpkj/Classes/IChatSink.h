#ifndef _IChatSink_H_
#define _IChatSink_H_

//////////////////////////////////////////////////////////////////////////
// 聊天钩子
//////////////////////////////////////////////////////////////////////////
class IChatSink
{
public:
	virtual ~IChatSink(){};

	//表情消息
public:
	//用户表情
	virtual bool InsertExpression(const char* pszSendUser, unsigned int index)=0;
	//用户表情
	virtual bool InsertExpression(const char* pszSendUser, unsigned int index, bool bMyselfString)=0;
	//用户表情
	virtual bool InsertExpression(const char* pszSendUser, const char* pszRecvUser, unsigned int index)=0;

	//聊天消息
public:
	//用户聊天
	virtual bool InsertUserChat(const char* pszSendUser, const char* pszString, unsigned int crColor)=0;
	//用户聊天
	virtual bool InsertUserChat(const char* pszSendUser, const char* pszRecvUser, const char* pszString, unsigned int crColor)=0;
	//用户私聊
	virtual bool InsertWhisperChat(const char* pszSendUser, const char* pszString, unsigned int crColor, bool bMyselfString)=0;
	//用户喇叭
	virtual bool InsertUserTrumpet(const char* pszSendUser,const char* pszString,unsigned int crColor)=0;
	//用户喇叭
	virtual bool InsertUserTyphon(const char* pszSendUser,const char* pszString,unsigned int crColor)=0;

	//系统消息
public:
	//系统消息
	virtual bool InsertSystemChat(const char* pszString)=0;

};

#endif // _IChatSink_H_
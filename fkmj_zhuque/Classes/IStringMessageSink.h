#ifndef _IStringMessageSink_H_
#define _IStringMessageSink_H_

#include "PlatformHeader.h"

//信息接口
class IStringMessageSink
{
public:
	virtual ~IStringMessageSink(){}

	//事件消息
public:
	//进入事件
	virtual bool InsertUserEnter(const char* pszUserName)=0;
	//离开事件
	virtual bool InsertUserLeave(const char* pszUserName)=0;
	//断线事件
	virtual bool InsertUserOffLine(const char* pszUserName)=0;

	//字符消息
public:
	//普通消息(窗口输出)
	virtual bool InsertNormalString(const char* pszString)=0;
	//系统消息(窗口输出)
	virtual bool InsertSystemString(const char* pszString)=0;
	//系统消息(窗口输出)
	virtual bool InsertSystemStringScript(const char* pszString)=0;
	//提示消息(对话框方式??)0:确认 1:确认,取消
	virtual int InsertPromptString(const char* pszString, int iButtonType)=0;

	//定制消息
public:
	//喜报消息
	virtual bool InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum)=0;
	
//	//定制消息
//	virtual bool InsertCustomString(const char* pszString, dword crColor)=0;
//	//定制消息
//	virtual bool InsertCustomString(const char* pszString, dword crColor, dword crBackColor)=0;
};

#endif // _IStringMessageSink_H_

#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"

class NoticeMsg
	:public FvSingleton<NoticeMsg> 
	,public cocos2d::Node
{
public:
	NoticeMsg();
	~NoticeMsg();
	
	 bool init();
public:
	void ShowNoticeMsg(std::string kText);
	void ShowNoticeMsgByScript(std::string kText);
	void ShowNoticeAction(cocos2d::Node* pNode);
	void ShowTopMsg(std::string kText);
	void ShowTopMsgByScript(std::string kText);
	void ShowTopMsgByScript(std::string kText,std::string kValue1);
	void ShowTopMsgByScript(std::string kText,std::string kValue1,std::string kValue2);
	void showWait(float fShow);
};
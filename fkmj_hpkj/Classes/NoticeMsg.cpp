#include "NoticeMsg.h"
#include "GameLib.h"

USING_NS_CC;

FV_SINGLETON_STORAGE(NoticeMsg);


NoticeMsg::NoticeMsg()
{
	init();
	WidgetScenceXMLparse xml("Script/NoticeMsg.xml",this);
}
NoticeMsg::~NoticeMsg()
{

}
bool NoticeMsg::init()
{
	if (!Node::init())
	{
		return false;
	};
	return true;
}



void NoticeMsg::ShowNoticeMsg(std::string kText )
{
	WidgetFun::setText(this,"NoticeMsg1",kText);
	WidgetFun::setText(this,"NoticeMsg2",kText);
	ShowNoticeAction(WidgetFun::getChildWidget(this,"NoticeMsgBack"));
	ShowNoticeAction(WidgetFun::getChildWidget(this,"NoticeMsg1"));
	ShowNoticeAction(WidgetFun::getChildWidget(this,"NoticeMsg2"));
}
void NoticeMsg::ShowNoticeMsgByScript(std::string kText)
{
	ScriptData<std::string> kTemp(kText);
	ShowNoticeMsg(kTemp.Value());
}

void NoticeMsg::ShowNoticeAction(cocos2d::Node* pNode)
{
	pNode->setVisible(true);

	ActionInterval*  action1 = CCFadeIn::create(0.5f);
	ActionInterval*  action2 = CCFadeOut::create(0.5f);

	Action * seq1 = Sequence::create( 
		action1, 
		DelayTime::create(3.0f), 
		action2,
		cocos2d::CCVisibleAction::create(0,false),
		NULL);
	pNode->stopAllActions();
	pNode->runAction( seq1 );
}
void NoticeMsg::ShowTopMsg(std::string kText)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"TopNoticeMsgBack");
	pNode->setVisible(true);
	float fHeight = pNode->getContentSize().height;
	WidgetFun::setText(this,"TopMsg",kText);
	ActionInterval*  action1 = MoveTo::create(0.5f,Point(0,-fHeight));
	ActionInterval*  action2 = MoveTo::create(0.5f,Point(0,0));

	Action * seq1 = Sequence::create( 
		action1, 
		DelayTime::create(3.0f), 
		action2,
		cocos2d::CCVisibleAction::create(0,false),
		NULL);
	pNode->stopAllActions();
	pNode->runAction( seq1 );
}

void NoticeMsg::ShowTopMsgByScript(std::string kText)
{
	ScriptData<std::string> kTemp(kText);
	ShowTopMsg(kTemp.Value());
}
void NoticeMsg::ShowTopMsgByScript(std::string kText,std::string kValue1)
{
	kText = utility::getScriptReplaceValue(kText,kValue1);
	ShowTopMsg(kText);
}
void NoticeMsg::ShowTopMsgByScript(std::string kText,std::string kValue1,std::string kValue2)
{
	kText = utility::getScriptReplaceValue(kText,kValue1,kValue2);
	ShowTopMsg(kText);
}
void NoticeMsg::showWait(float fShowTime)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"WaitePlane");
	pNode->setVisible(true);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::CCVisibleAction::create(fShowTime,false));
}
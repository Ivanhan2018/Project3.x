#include "HNHomeScence.h"
#include "HNScenceManager.h"
#include "GameLib.h"


//完成通知
void HNHomeScence::OnGameItemFinish()
{
}
//完成通知
void HNHomeScence::OnGameKindFinish(uint16 wKindID)
{

}
//更新通知
void HNHomeScence::OnGameItemUpdateFinish()
{

}
//插入通知
void HNHomeScence::OnGameItemInsert(CGameListItem * pGameListItem)
{
}
//更新通知
void HNHomeScence::OnGameItemUpdate(CGameListItem * pGameListItem)
{

}
//删除通知
void HNHomeScence::OnGameItemDelete(CGameListItem * pGameListItem)
{

}
void HNHomeScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}
void HNHomeScence::onGPIndividualSuccess(int type, const char* szDescription)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}
void HNHomeScence::onGPIndividualFailure(int type, const char* szDescription)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(szDescription));
}
void HNHomeScence::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	if (kKey == "NC_SHOP_TXT")
	{
		WidgetFun::setText(this,"shopTxt",utility::a_u8(szDescription));
	}

	if (kKey == "HN_SC_NOTICE")
	{
		std::string kStr = utility::a_u8(szDescription);
		m_kMsgList = utility::split(kStr,"|");
		NextSpeaker();
	}
}
void HNHomeScence::NextSpeaker()
{
	if (!m_kMsgList.size())
	{
		return;
	}
	std::string kStr = *m_kMsgList.begin();
	m_kMsgList.erase(m_kMsgList.begin());
	m_kMsgList.push_back(kStr);
	showSysSpeakerTxt(kStr,"255 255 255");
}
void HNHomeScence::showSysSpeakerTxt(std::string kTxt,std::string kColor)
{
	cocos2d::Node* pTxt = WidgetFun::getChildWidget(this,"SysSpeakerTxt");
	cocos2d::Node* pLayoutNode = WidgetFun::getChildWidget(this,"LayoutNode");
	WidgetFun::setText(pTxt,kTxt);
	WidgetFun::setTextColor(pTxt,kColor);
	float fMaxHeight = pLayoutNode->getContentSize().height;
	float fMaxWidth = pLayoutNode->getContentSize().width;
	float fTxtWidth = pTxt->getContentSize().width;
	pTxt->setPosition(0,-fMaxHeight);
	pTxt->stopAllActions();
	pTxt->setVisible(true);
	if (fMaxWidth > fTxtWidth)
	{
		cocos2d::CCAction * seq1 = cocos2d::CCSequence::create(
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,0)),
			cocos2d::CCDelayTime::create(6.0f), 
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,fMaxHeight)),
			cocos2d::CCVisibleAction::create(0,false),
			cocos2d::CCCallFunc::create(this,callfunc_selector(HNHomeScence::NextSpeaker)),
			NULL);
		pTxt->runAction(seq1);
	}
	else
	{
		float fPosX = fMaxWidth-fTxtWidth-10;
		cocos2d::CCAction * seq1 = cocos2d::CCSequence::create( 
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,0)),
			cocos2d::CCDelayTime::create(3.0f), 
			cocos2d::MoveTo::create(2.0f,cocos2d::Point(fPosX,0)),
			cocos2d::CCDelayTime::create(3.0f), 
			cocos2d::MoveTo::create(0.5f,cocos2d::Point(fPosX,fMaxHeight)),
			cocos2d::CCVisibleAction::create(0,false),
			cocos2d::CCCallFunc::create(this,callfunc_selector(HNHomeScence::NextSpeaker)),
			NULL);
		pTxt->runAction(seq1);
	}
}
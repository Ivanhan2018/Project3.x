#ifndef __GameUserMessage_H__
#define __GameUserMessage_H__

#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
#include "MyNSString.h"
#include "Define.h"
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;
enum GameUserMessageEnum
{
	Enum_GameUserMessageBack = 95,
};
class GameUserMessage : public cocos2d::Layer
{
public:
	ui::Widget*         m_uiWidget;
	ui::Button*         m_MessageBack;
	ui::ImageView*      m_MessageFace;
	ui::Text*			m_MessageNick;
	ui::Text*			m_MessageUid;
	ui::Text*			m_MessageVip;
	ui::Text*			m_MessageCoin;
	ui::ImageView* m_vipImg;
	static GameUserMessage* m_gameUserMsg;
public:
	static GameUserMessage* getInstance(WORD nChairID);
	static void destoryInstance(void);
	GameUserMessage(void);
	~GameUserMessage(void);
	virtual bool init();
	CREATE_FUNC(GameUserMessage);
	void onCallback(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void onClose(Ref* pSender, TouchEventType type);
	void setRecordValue(WORD faceID,std::string strName, DWORD userID, BYTE VipLv, LONGLONG userCoin);
};


#endif 

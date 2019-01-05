#ifndef _MENU_LAYER_H_
#define _MENU_LAYER_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "AwardLayer.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

class ByMenuLayer : public Layer
{
public:
	CREATE_FUNC(ByMenuLayer);
	bool init();
	void setTicketNumber(std::int32_t dwTicket);
	ByMenuLayer();
	~ByMenuLayer();
	static ByMenuLayer* GetInstance();
	void setExitMenuStatue(bool isEnabled);
	//CC_SYNTHESIZE(ByRechargeLayer*, m_rechargelayer, ChargeLayer);
	AwardLayer* m_awardLayer;
private:
	MenuItemImage* menu_menu;
	MenuItemImage* menu_Award; 
	MenuItemImage* menu_AutoShoot;
	MenuItemImage* menu_AddGun;
	MenuItemImage* menu_SubGun;
	MenuItemImage* menu_Setting;
	MenuItemImage* menu_Music; 
	MenuItemImage* menu_Effect; 
	MenuItemImage* menu_Instro; 
	MenuItemImage* menu_Exit; 
	MenuItemImage* menu_Charge; 
	Menu* m_menu;
	bool isShow;
	LabelTTF* m_labelTicketScore;
	std::int32_t	 m_dwTicketNumber;

	static ByMenuLayer*	m_pMenuLayer;	
public:
	bool m_isExchange;
	void onCharge(Ref* pSender);
	void onAward(Ref* pSender);
	void showAward(CMD_S_OnLineAward* onLineAward);
	void onAlms(bool bAlms);
	void onQuitGame(Ref* sender);
	//ByRechargeLayer* getRechargeLayer();
private:
	void onAutoShoot(Ref* sender);
	void onControlMusic(Ref* sender);
	void onControlEffect(Ref* sender);
	void onInstroduce(Ref* sender);
	void btnCallback(Ref* pSender);
	void enabledCallback(Ref* pSender); 
	void menuCloseCallback(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	ActionInterval* spawnOut(Vec2 point, float duration);
	ActionInterval* spawnIn(float duration);
	void autoHideMenu(float dt);
};


#endif // _MENU_LAYER_H_




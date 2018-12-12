//
//  GetCharge.h
//  Game
//
//  Created by 罗敏 on 13-8-6.
//
//

#ifndef __Game__ChongCoinsLayer__
#define __Game__ChongCoinsLayer__

#include "cocos2d.h"
#include "CocosUnits.h"
#include "cocos-ext.h"
USING_NS_CC;

class ChongCoinsLayer : public Layer
{
public:
    static ChongCoinsLayer * ChongCoinsLayerWith(Vec2 pos);
    bool initChongCoinsLayer(Vec2 pos);
    ChongCoinsLayer();
    ~ChongCoinsLayer();
	void closeChongCoinsLayer(Object* obj);
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
public:
    
    int currentSelct;                    //点击领取时当前领取的类型
private:


private:
    MenuItemSprite* m_pZJDCardItemSprite;
    MenuItemSprite* m_pYidongMSMItemSprite;
    MenuItemSprite* m_pLiantongMSMItemSprite;
    MenuItemSprite* m_pDianxingMSMItemSprite;
	MenuItemSprite* m_pPhoneCardItemSprite;
	MenuItemSprite* m_pAlipayMSMItemSprite;
	Node* m_pZJDcARD;
	Node* m_pYidongMSM;
	Node* m_pLiantongMSM;
	Node* m_pDianxingMSM;
	Node* m_pPhoneCard;
	Node* m_pAlipayMSM;
private:
	void showZJDCard(Object* obj);
	void showYidongSMS(Object* obj);
	void showLiantongSMS(Object* obj);
	void showDianxingSMS(Object* obj);
	void showPhoneCard(Object* obj);
	void showAliPay(Object* obj);

};

#endif /* defined(__Game__GetCharge__) */

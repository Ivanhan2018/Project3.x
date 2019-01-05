#ifndef _EXCHANGELAYER_H_
#define _EXCHANGELAYER_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

class ExchangeLayer : public cocos2d::Layer
{
private:
	const char*                            m_strAccounts;
	const char*                            m_strPassword;
	const char*                            m_strNickname;
	cocos2d::ui::Widget*				   m_uiWidget;

	cocos2d::ui::Button*	               First_[6];
	cocos2d::ui::Button*	               Second_[4];	
	cocos2d::ui::Layout*				   m_layoutA;
	cocos2d::ui::Layout*				   m_layoutB;
	cocos2d::ui::Layout*				   m_layoutC;
	cocos2d::ui::Button*					m_ExOkA;
	cocos2d::ui::Button*					m_ExOkB;
	cocos2d::ui::Button*					m_close;
	cocos2d::ui::ImageView*					m_PicImgA;
	cocos2d::ui::ImageView*					m_PicImgB;
	cocos2d::ui::TextField*                 SjA;
	cocos2d::ui::TextField*                 SjB;
	cocos2d::ui::TextField*                 Dz;
	cocos2d::ui::TextField*                 Xm;
	cocos2d::ui::Text*						m_TextCoin;
	cocos2d::ui::Text*						m_TextTicket;
	cocos2d::ui::LoadingBar*				m_ProgressA;
	cocos2d::ui::LoadingBar*				m_ProgressB;
	cocos2d::ui::Text*						m_TextNum[8];

	int										m_nFeeNeedTicket[3];	//话费所需奖券
	int										m_nFlowNeedTicket[3];	//流量所需奖券
	

public:
	ExchangeLayer(void);
	~ExchangeLayer(void);
	static ExchangeLayer *create();
	virtual bool init();
 	void OnTextFieldCallBack(cocos2d::Ref* sender, cocos2d::ui::TextFiledEventType type);
 	void ExOk(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
 	void OnRechargeCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
 	void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
 	void SetBtnEnabled(bool enabled);
	void updateView();
	//更新实物数量
	void updateEntity(int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount, int nIpadNeed, int nMp3Need, int nWhiteNeed, int nZippoNeed);

	//更新实物
	void updateEntity();
};

#endif // !_EXCHANGELAYER_H_

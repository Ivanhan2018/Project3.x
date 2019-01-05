#ifndef _SystemLayer_H_
#define _SystemLayer_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"


USING_NS_CC;
using namespace ui;
USING_NS_CC_EXT;

class SystemLayer : public cocos2d::LayerColor
{
private:
	Widget*	      m_widgetUI;
	Layout*          Panel_SystemLayer;
	Button*	      set_close;
				
	CheckBox*	      CheckBox_chat;
	CheckBox*	      CheckBox_cartoon;
	CheckBox*		CheckBox_Music;
	CheckBox*		CheckBox_Effect;
	Sprite*			m_spSwitch;
	ImageView*		musicImage;	
	ImageView*		effectImage;	
	ImageView*		shakeImage;		
	ImageView*		gameEffectImage;
	ControlSlider*  m_sliderMusic;
	EventListenerTouchOneByOne* m_TouchListener;

	cocos2d::Ref* m_touchtarget;
	cocos2d::ui::SEL_TouchEvent m_touchevent;

	cocos2d::Ref* m_Switchtarget;//切换帐号
	cocos2d::ui::SEL_TouchEvent m_SwitchAccountEvent;

	CC_SYNTHESIZE(Button*, m_btnSwitchAccount, BtnSwitchAccount);
	void sliderMusicChange(Ref* psender, Control::EventType event);
	void sliderEffectChange(Ref* psender, Control::EventType event);
	void OnSliderCallBack(cocos2d::Ref* sender,cocos2d::ui::SliderEventType type);

	void MessageBox(const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag1, int tag2);

	void onSwitchAccount(Ref* obj,TouchEventType type);


	const char* mpszFilePath;
public:
	SystemLayer(void);
	~SystemLayer(void);
	virtual bool init(float fScale,const char* pszFilePath);
	static SystemLayer *create(float fScale,const char* pszFilePath);
	//震动效果
	void shakeBtnCall(cocos2d::Ref* pSender);
	//游戏特效
	void gameEffect(cocos2d::Ref* pSender);
	//设置CHECK BOX回调函数
	void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void SetLayerTouchEnable(bool var);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	void SetSwitchAccountCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void HideSwitchAccount();
};

#endif // !_SystemLayer_H_

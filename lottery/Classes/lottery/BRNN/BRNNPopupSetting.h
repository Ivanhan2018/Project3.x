#ifndef __BRNN_POPUP_SETTING_H__
#define __BRNN_POPUP_SETTING_H__

#include "cocos2d.h"
#include "BaseDialog.h"
#include "cocos-ext.h"

USING_NS_CC;

/*
**	@brief	百人牛牛——设置界面
*/
class BRNNPopupSetting: public Layer
{
public:
	BRNNPopupSetting();
	~BRNNPopupSetting();
	CREATE_FUNC(BRNNPopupSetting);

	//	设置配置界面是否显示
	void setPopupSettingVisible(bool bIsVisible);
private:
	bool init();
	void initUI();

	//	音乐开关按钮事件
	void onControlMusicChange(Ref* sender, extension::Control::EventType controlEvent);

	//	音效开关按钮事件
	void onControlSoundEffectChange(Ref* sender, extension::Control::EventType controlEvent);

	//	单点触摸事件——开始
	bool onTouchBegan(Touch *touch, Event *unused_event);

	//	单点触摸事件——移动
	void onTouchMoved(Touch *touch, Event *unused_event);

	//	单点触摸事件——取消
	void onTouchCancelled(Touch *touch, Event *unused_event);

	//	单点触摸事件——结束
	void onTouchEnded(Touch *touch, Event *unused_event);

	//	背景
	Sprite* m_pBG;

	//	开关按钮——音乐
	extension::ControlSwitch* m_pControlMusic;

	//	开关按钮——音效
	extension::ControlSwitch* m_pControlSoundEffect;
};


#endif  // __BRNN_POPUP_SETTING_H__
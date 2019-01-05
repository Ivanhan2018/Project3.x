#ifndef __SGLY_POPUP_SETTING_H__
#define __SGLY_POPUP_SETTING_H__

#include "cocos2d.h"
#include "BaseDialog.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace std;

/*
**	@brief	水果乐园——设置界面
*/
class SGLYPopupSetting: public BaseDialog
{
public:
	SGLYPopupSetting();
	~SGLYPopupSetting();
	CREATE_FUNC(SGLYPopupSetting);

private:
	bool init();
	void initUI();

	//	点击音乐复选框事件
	void onCBMusicEvent(Ref* pSender,CheckBox::EventType type);

	//	点击音效复选框事件
	void onCBSoundEffectEvent(Ref* pSender,CheckBox::EventType type);

	//	点击确定按钮事件
	void onClickBtnOKEvent();

	//	点击取消按钮事件
	void onClickBtnNOEvent();

	//	背景
	Sprite* m_pBG;

	//	文本——设置
	Sprite* m_pTextSetting;

	//	文本——音乐
	Sprite* m_pTextMusic;

	//	文本——音效
	Sprite* m_pTextSoundEffect;

	//	复选框——音乐
	CheckBox* m_pCheckBoxMusic;

	//	复选框——音效
	CheckBox* m_pCheckBoxSoundEffect;

	//	按钮——确定
	Button* m_pBtnOK;

	//	按钮——取消
	Button* m_pBtnNO;

	//	是否开背景音乐
	bool m_bOnMusic;

	//	是否开音效
	bool m_bOnSoundEffect;

};


#endif  // __SGLY_POPUP_SETTING_H__
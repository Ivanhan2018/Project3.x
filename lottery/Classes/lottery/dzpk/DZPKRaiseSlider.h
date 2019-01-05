#ifndef __DZPK_RAISE_SLIDER_H__
#define __DZPK_RAISE_SLIDER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Define.h"//by hxh

USING_NS_CC;

//	德州扑克——加注滑动条
class DZPKRaiseSlider: public Node
{
public:
	DZPKRaiseSlider();
	~DZPKRaiseSlider();
	CREATE_FUNC(DZPKRaiseSlider);
	bool init();
	void initUI();

	//	重置加注滑动条
	void resetRaiseSlider();

	//	显示Allin
	void showTextAllin();

	//	设置滑动条数值
	void setLabNum(LONGLONG llValue);

	//	获取加注值
	LONGLONG getRaiseValue() const;

	//	最小加注值
	CC_SYNTHESIZE(LONGLONG, m_llMinRaiseValue, MinRaiseValue);

private:

	//	改变滑动条时处理事件
	void onChangeSlider(Ref* sender, ui::Slider::EventType eEventTyp);
	
	//	滑动条背景
	Sprite* m_pSliderBG;

	//	滑动条文本——ALL-IN
	Sprite* m_pTextAllin;

	//	滑动条数值
	Label* m_pLabNum;

	//	滑动条
	ui::Slider* m_pSlider;

	//	实际加注数值
	LONGLONG m_llRaiseValue;
};



#endif	//	__DZPK_RAISE_SLIDER_H__
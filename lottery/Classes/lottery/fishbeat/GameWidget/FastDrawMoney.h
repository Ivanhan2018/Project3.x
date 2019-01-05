#ifndef _FAST_DRAWMONEY_H_
#define _FAST_DRAWMONEY_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"


class FastDrawMoney : public cocos2d::Layer
{
	static FastDrawMoney*			m_drawMoney;
private:
	cocos2d::ui::Widget*			UI;
	cocos2d::ui::Text*              textMoney_YH;
	cocos2d::ui::Text*           	textMoney_BB;
	const char*						m_szCinText[2];
public:
	FastDrawMoney(void);
	~FastDrawMoney(void);
	virtual bool init();
	static FastDrawMoney *create();
	static FastDrawMoney* GetInstance() { return m_drawMoney; }
	void OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void OnTextFieldCallBack(cocos2d::Ref* sender, cocos2d::ui::TextFiledEventType type);
	char* GetNumber(const char* src,char* buf);
	void onUpdateView();
};

#endif // !_FAST_DRAWMONEY_H_

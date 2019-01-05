#ifndef _AwardLayer_H_
#define _AwardLayer_H_

#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
#include "CMD_Fish.h"

USING_NS_CC;
using namespace cocostudio;
using namespace NS_lrbyFish;

class AwardLayer : public Layer
{
public:
	CREATE_FUNC(AwardLayer);
	bool init(CMD_S_OnLineAward* onLineAward);
	bool init();
	AwardLayer();
	~AwardLayer();
	void setStatus(int nAllSetp, int nCurStep, int nTotlaTime);
	void setCurStatus(int nCurStep);

private:
	cocos2d::ui::Widget*		uiWidget;
	ui::Button*					m_btnQuit;
	ui::Button*					m_btnConfirm;
	void buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void buttonConfirm(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	cocos2d::ui::ImageView*		m_Award_Show[6];
	cocos2d::ui::ImageView*		m_Award_Show_Min[6];
	cocos2d::ui::ImageView*		m_Award_NotGet[6];
	int							m_nCurStep;
	int							m_nAllStep;
	int							m_nTotalTime;
	int							m_nCanGetCount;
	int							m_nCanMaxCount;
	cocos2d::ui::Text*			m_Label_Hour;
	cocos2d::ui::Text*			m_Label_Min;
	cocos2d::ui::Text*			m_Label_Sec;

	void						onTime(float dt);
	int							m_nSecond[6]; 
};


#endif // _AwardLayer_H_


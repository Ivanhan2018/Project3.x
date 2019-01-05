#ifndef _INTRODUCELAYER_H_
#define _INTRODUCELAYER_H_

#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;


class IntroduceItem : public cocos2d::ui::Layout
{
	cocos2d::ui::ImageView*			m_rankingbg;
	cocos2d::ui::ImageView*			m_fishbg1;
	cocos2d::ui::ImageView*			m_fishbg2;
	cocos2d::ui::ImageView*			m_fishbg3;
	cocos2d::ui::Text*				m_txtSub1;
	cocos2d::ui::Text*				m_txtSub2;
	cocos2d::ui::Text*				m_txtSub3;

public:
	IntroduceItem();
	~IntroduceItem();
	virtual bool init(int rankingID);
	static IntroduceItem *create(int rankingID);
	void updateView(void* pRankingView);
};

class IntroduceLayer : public Layer
{
public:
	CREATE_FUNC(IntroduceLayer);
	bool init();
	IntroduceLayer();
	~IntroduceLayer();

private:
	cocos2d::ui::Widget*		uiWidget;
	ui::Button*					m_btnQuit;
	cocos2d::ui::ListView*		 m_ListView;

	void buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
};


#endif // _INTRODUCELAYER_H_


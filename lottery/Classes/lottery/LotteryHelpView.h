#ifndef __Game__LotteryHelpView__
#define __Game__LotteryHelpView__
#include <iostream>
#include "cocos2d.h"
#include "comm.h"
using namespace cocos2d;

class LotteryHelpView : public Layer
{

public:
	LotteryHelpView();
	~LotteryHelpView();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(LotteryHelpView);
public:
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
	void back(Object *obj);
	void setHelpId(CaiZhong id);
	void refreshContent(const char* src, const int num,const char* firstSrc);
private:
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	Node *m_node;
	float m_start;
	float m_end;
	Vec2 m_endPos;
	CaiZhong helpId;
	LabelTTF *title;
	Sprite *background;
};
#endif  //__Game__HelpScrollView__
#ifndef __Game__HelpScrollView__
#define __Game__HelpScrollView__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class HelpScrollView : public Layer
{

public:
	HelpScrollView();
	~HelpScrollView();
	virtual bool init();
	static Scene* scene();
	Rect addString(Vec2 pt,const char* buff,cocos2d::Size size);

	CREATE_FUNC(HelpScrollView);
public:
	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
private:
	Node *m_node;
	float m_start;
	float m_end;
	Vec2 m_endPos;

};
#endif  //__Game__HelpScrollView__
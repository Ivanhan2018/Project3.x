#ifndef __Game__AgreementScrollView__
#define __Game__AgreementScrollView__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
class AgreementScrollView : public Layer
{

public:
	AgreementScrollView();
	~AgreementScrollView();
	virtual bool init();
	static Scene* scene();
	//virtual void visit();
	CREATE_FUNC(AgreementScrollView);
public:
	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
private:
	LabelTTF *str1;
	float m_start;
	float m_end;

};
#endif  //__Game__AgreementScrollView__
//  [4/10/2014 xieyi]
#ifndef __SHAKE_H__
#define __SHAKE_H__
#include "cocos2d.h"
USING_NS_CC;

//¶¶¶¯¶¯»­
class CCShake : public cocos2d::CCActionInterval
{
public:
	CCShake();
	static CCShake * create(float d,float strength);
	static CCShake * createWithStrength(float d,float strength_x,float strength_y);
	bool initWithDuration(float d,float stength_x,float strength_y);

protected:

	void startWithTarget(cocos2d::Node * pTarget);
	void update(float time);
	void stop(void);
	Vec2 m_startPosition;
	float m_strength_x;
	float m_strength_y;

};
//ÎÄ×ÖÃè±ß
class CCStrokeLabel : public cocos2d::Node {
public:
	static CCStrokeLabel* create(cocos2d::LabelTTF* labelTTF,cocos2d::Color3B fullColor,cocos2d::Color3B StrokeColor,float strokeSize);
	virtual bool init();
	float getStrokeSize();
	void setStrokeSize(float strokeSize);
private:
	cocos2d::Sprite* m_sprite;
	cocos2d::LabelTTF* m_label;
	cocos2d::Color3B m_fullColor;
	cocos2d::Color3B m_StrokeColor;
	float m_strokeSize;
};

#endif
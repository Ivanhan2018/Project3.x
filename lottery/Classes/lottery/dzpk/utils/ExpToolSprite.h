#ifndef __Game_ExpToolSprite_h
#define __Game_ExpToolSprite_h

#include "Inc.h"

class ExpToolSprite : public Layer
{

public:
	ExpToolSprite();
	virtual ~ExpToolSprite();
	CREATE_FUNC(ExpToolSprite);

public:
	virtual bool init();
	static ExpToolSprite* createWith(const char* expToolBg,const char* expTool);
	static ExpToolSprite* createWithFrame(CCSpriteFrame* expBg,CCSpriteFrame* exp);
	void updatePercent(float percent);
	void updatePercent(float percent,UNS_INT total,UNS_INT sub);

private:
	void initUI();

private:
	CCSprite* expSprite;
	CCSprite* expBgSprite;
	float percent;

};

#endif
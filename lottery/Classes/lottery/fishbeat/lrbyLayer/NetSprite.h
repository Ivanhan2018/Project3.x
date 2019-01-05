#ifndef __GAME_NET_SPRITE_H__
#define __GAME_NET_SPRITE_H__
#include "cocos2d.h"


USING_NS_CC;

enum enNetStatue
{
	NET_1_NORMAL=1,
	NET_2_NORMAL,
	NET_3_NORMAL,
	NET_4_NORMAL
};
class NetSprite : public Sprite
{
public:
	NetSprite();
	virtual ~NetSprite();
	static NetSprite *create(int nNetStatue);

	bool init(int nNetStatue);
	void run(Vec2 pt, int nNetStatue);

	Sprite* m_spNet;
	Sequence*  m_pSeqAction;
	Vec2 m_ptPos;

	CC_SYNTHESIZE(bool, m_live, Live);

public:
	void removeMySelf();
public:
	int m_NetStatue;
};
#endif 

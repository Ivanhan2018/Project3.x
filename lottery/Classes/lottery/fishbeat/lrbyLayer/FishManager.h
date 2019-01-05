#ifndef __GAME_FISH_MANAGER_H__
#define __GAME_FISH_MANAGER_H__
#include "cocos2d.h"
#include <iostream>
#include "FishSprite.h"

using namespace cocos2d;
using namespace std;

class FishManager : public Node
{
public:
	virtual ~FishManager();

	virtual bool init();
	

	FishSprite* createFish(int nFish, int nFishKind, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	void getFishAnimationCount(int nFishId, int& nNormalCount, int& nDieCount);
	void addFish(FishSprite* fish);
	void removeFish(FishSprite* fish);

	int lockFish(FishKind* lock_fish_kind = NULL, int last_lock_fish_id = 0, FishKind last_fish_kind = FISH_KIND_COUNT);

	void clearAllFish();
	static FishManager* GetInstance();

	void destroy();

	FishSprite* getFishById(int fishId);

	static bool insideScreen(Vec2 pos);
private:
	CREATE_FUNC(FishManager);
	FishManager();
	static FishManager*					m_pFishManager;					// 对象指针
public:
	cocos2d::Vector<FishSprite*> m_fishList;   // 鱼列表
};


#endif // __FISH_MANAGER_H__
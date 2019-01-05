#ifndef __GAME_FISH_SPRITE_H__
#define __GAME_FISH_SPRITE_H__

#include <iostream>
#include "cocos2d.h"
#include "CMD_Fish.h"

using namespace cocos2d;
using namespace std;
using namespace NS_lrbyFish;

enum Direction
{
	VERTICAL = 0,
	HORIZONTAL
};

enum FishStatue
{
	FISH_NULL=0,
	FISH_SWIM,
	FISH_HITED,
	FISH_DIE
};

/*
(x1,y1)------(x2,y2)
   |            |
   |            |
(x4,y4)------(x3,y3)
*/

class BoundingBox {
 public:
  BoundingBox();
  BoundingBox(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

  void CreateBoundingBox(float box_width, float box_height, float center_x, float center_y, float angle);
  bool ComputeCollision(float x, float y);

  float x1_, y1_, x2_,  y2_, x3_, y3_, x4_, y4_;
};

class FishSprite : public Sprite
{
public:
	FishSprite();
	virtual ~FishSprite();

	static FishSprite *createCommonFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	static FishSprite* createBombFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	static FishSprite* createSuperFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	static FishSprite* createKingFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	
	virtual bool initCommonFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	virtual bool initBombFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	virtual bool initSuperFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);
	virtual bool initKingFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count);

	void funCommomFish();
	void funBombFish();
	void funSuperFish();
	void funKingFish();

	bool updateFish(float dt);	

	inline void setRotate(float angle) { if(m_node != nullptr) m_node->setAnchorPoint(ccp(0.5, 0.5));	m_node->setRotation(angle); }
	inline void setPosition(Vec2 pt) {if(m_node != nullptr) m_node->setPosition(pt); }
	
	Vec2 getFishNextPosition(int nCount);
	Vec2 getFishPosition();
	void playBombSwimAnimation();
	void playBombDieAnimation();
	void playBombHitedAnimation(bool bSelf);

	void playNormalSwimAnimation();
	void playNormalDieAnimation();
	void playNormalHitedAnimation(bool bSelf);

	void playSuperSwimAnimation();
	void playSuperDieAnimation();
	void playSuperHitedAnimation(bool bSelf);

	void playKingSwimAnimation();
	void playKingDieAnimation();
	void playKingHitedAnimation(bool bSelf);

	void playSwimAnimation();
	void playDieAnimation();
	void playHitedAnimation(bool bSelf);

	void setLKMultiple(int nMul);
	int getFishStatue() { return m_fishStatue; }
	int getFishId() { return m_fish_id; }
	int getFishKind() { return m_nFishKind; }
	void setTraceType(TraceType Ttrace);
	void SetFishStop(int nStopIndex, int nStopCount);
	int getFishMutiple();
	void removeFish();

	//计算轨迹
	void CalcFishSwimPath(int nFishid, TraceType nTraceType, float init_x_pos[], float init_y_pos[], int init_count, float fSpeed);

	//bool BulletHitTest(const Vec2 &pt);

	void getFishBoundingBox(BoundingBox &box);

public:
	std::vector<FPointAngle> m_vecFishPath;
	int m_nPathCount;		//鱼游动到的当前路径值
	bool m_bClacFinish;
	int m_nFishKind;
	int m_nNorCount;
	int m_nDieCount;
	
private:
	LabelAtlas* m_labelLKNum;		//李逵倍数显示
	Node* m_node;				//添加鱼的节点
	float m_speed;				//鱼游动的速度
	Sprite* m_fish[4];			//大四喜需要同时使用四个精灵
	Sprite* m_wheel[4];			//旋转的齿轮，同上

	Animation *m_SwimAnimation;
	Animation *m_DieAnimation;

private:
	
	int m_nFishSuperKing;
	int m_nFishCount;		//只针对大三元，大四喜的变量

private:
	int stop_index_;
	int stop_count_;
	int current_stop_count_;

private:
	int m_fish_id;
	int m_fish_multiple;
	float m_bounding_box_width;
	float m_bounding_box_height;
	int m_fishStatue;

	TraceType m_trace_type;
};


#endif // __FISH_H__

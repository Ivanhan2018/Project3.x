
#ifndef LOCK_FISH_MANAGER_H_
#define LOCK_FISH_MANAGER_H_

#include "CMD_Fish.h"
#include "Define.h"
#include "cocos2d.h"
#include "LockLineLayer.h"

USING_NS_CC;
using namespace NS_lrbyFish;

class LockFishManager : public cocos2d::Layer {
public:
	LockFishManager();
	virtual ~LockFishManager();

	CREATE_FUNC(LockFishManager);

	virtual bool init();

	bool LoadGameResource();

	bool OnFrame(float delta_time);
	bool OnRender(float offset_x, float offset_y, float hscale, float vscale);

	void SetLockFishID(WORD chair_id, int lock_fish_id) { lock_fish_id_[chair_id] = lock_fish_id; }
	int GetLockFishID(WORD chair_id) const { return lock_fish_id_[chair_id]; }
	void SetLockFishKind(WORD chair_id, FishKind lock_fish_kind) { lock_fish_kind_[chair_id] = lock_fish_kind; }
	FishKind GetLockFishKind(WORD chair_id) const { return lock_fish_kind_[chair_id];}

	//鱼进入展示界面开始绘制锁定界面
	void SetEnterFishStatue(WORD chair_id, bool statue) { lock_fish_is_enter[chair_id] = statue; }
	bool GetEnterFishStatue(WORD chair_id) { return lock_fish_is_enter[chair_id]; }

	void UpdateLockTrace(WORD chair_id, float fish_pos_x, float fish_pos_y);
	void ClearLockTrace(WORD chair_id);
	Vec2 LockPos(WORD chair_id);

	void setLockLineVisible(int chair_id, bool visible);

public:

	void createLockLine(int chairId, Vec2 pStart, Vec2 pEnd);

private:
	
  //hgeSprite* spr_lock_flag_[BY_GAME_PLAYER];
  //hgeSprite* spr_lock_line_;
  //hgeAnimation* ani_lock_flag_;

	LockLineLayer* m_LineLayer[BY_GAME_PLAYER];
	Sprite* m_LockFlag[BY_GAME_PLAYER];

	float rotate_angle_;

	int lock_fish_id_[BY_GAME_PLAYER];
	FishKind lock_fish_kind_[BY_GAME_PLAYER];
	//typedef std::vector<Vec2> FPointVector;

	Vec2 lock_line_trace_[BY_GAME_PLAYER];

	Vec2 lock_line_start[BY_GAME_PLAYER];

	//由于鱼开始出现时，可能没有出现在手机端界面，（初始position超出， 1280*720 ，这个范围）
	bool lock_fish_is_enter[BY_GAME_PLAYER];
};

#endif // LOCK_FISH_MANAGER_H_

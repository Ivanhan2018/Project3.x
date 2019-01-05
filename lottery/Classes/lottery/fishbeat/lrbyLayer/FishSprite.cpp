#include "FishSprite.h"
#include "FishManager.h"
#include "LrbyGameConfig.h"

#include "math_aide.h"

using namespace NS_lrbyFish;

extern CLrbyGameConfig g_LrbyGameConfig;

BoundingBox::BoundingBox() {
	x1_ = x2_ = x3_ = x4_ = y1_ = y2_ = y3_ = y4_ = 0.f;
}

BoundingBox::BoundingBox(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	: x1_(x1), y1_(y1), x2_(x2), y2_(y2), x3_(x3), y3_(y3), x4_(x4), y4_(y4) {}

void BoundingBox::CreateBoundingBox(float box_width, float box_height, float center_x, float center_y, float angle) {
	box_width = box_width / 2;
	box_height = box_height / 2;
	float sint = sinf(angle);
	float cost = cosf(angle);
	float hot_x1 = -box_width;
	float hot_y1 = -box_height;
	float hot_x2 = box_width;
	float hot_y2 = box_height;
	x1_ = hot_x1 * cost - hot_y1 * sint + center_x;
	y1_ = hot_x1 * sint + hot_y1 * cost + center_y;
	x2_ = hot_x2 * cost - hot_y1 * sint + center_x;
	y2_ = hot_x2 * sint + hot_y1 * cost + center_y;
	x3_ = hot_x2 * cost - hot_y2 * sint + center_x;
	y3_ = hot_x2 * sint + hot_y2 * cost + center_y;
	x4_ = hot_x1 * cost - hot_y2 * sint + center_x;
	y4_ = hot_x1 * sint + hot_y2 * cost + center_y;
}

bool BoundingBox::ComputeCollision(float x, float y) {
	FPoint point[4] = { {x1_, y1_}, {x2_, y2_}, {x3_, y3_}, {x4_, y4_} };
	int i, j;
	bool inside = false;
	int count1 = 0, count2 = 0;

	for (i = 0, j = 4 - 1; i < 4; j = i++) {
		float value = (x - point[j].x) * (point[i].y - point[j].y) - (y - point[j].y) * (point[i].x - point[j].x);
		if (value > 0) {
			++count1;
		} else if (value < 0) {
			++count2;
		}
	}

	if (0 == count1 || 0 == count2) inside = true;

	return inside;
}

FishSprite::FishSprite()
	:m_SwimAnimation(nullptr)
	,m_DieAnimation(nullptr)
	,m_labelLKNum(nullptr)
	,m_fishStatue(FISH_NULL)
	,m_bClacFinish(false)
{

}

FishSprite::~FishSprite()
{
//	m_SwimAnimation->release();
//	m_DieAnimation->release();
	CC_SAFE_RELEASE_NULL(m_SwimAnimation);
	CC_SAFE_RELEASE_NULL(m_DieAnimation);
}
//创建普通鱼群
FishSprite* FishSprite::createCommonFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	FishSprite *fish = new FishSprite();
	if (fish && fish->initCommonFish(id, normalCount, dieCount, nFishid, nTraceType, nInit_pos, nInit_count))
	{
		//fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}
//创建炸弹
FishSprite* FishSprite::createBombFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	FishSprite *fish = new FishSprite();
	if (fish && fish->initBombFish(id, normalCount, dieCount, nFishid, nTraceType, nInit_pos, nInit_count))
	{
		//fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}
//创建超级鱼群，如大三元，大四喜
FishSprite* FishSprite::createSuperFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	FishSprite *fish = new FishSprite();
	if (fish && fish->initSuperFish(id, normalCount, dieCount, nFishid, nTraceType, nInit_pos, nInit_count))
	{
		//fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}
//创建鱼王
FishSprite* FishSprite::createKingFishByID(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	FishSprite *fish = new FishSprite();
	if (fish && fish->initKingFish(id, normalCount, dieCount, nFishid, nTraceType, nInit_pos, nInit_count))
	{
		//fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

//初始化变通鱼
bool FishSprite::initCommonFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	//return init(id, normalCount, dieCount, nFishid, nTraceType, nInit_pos, nInit_count);

	if(!Sprite::init())
	{
		return false;
	}

	m_nFishKind = id;

	//解析鱼类所需要的数据
	FishTraceInfo fish_trace_info = { 0 };
	fish_trace_info.init_count = nInit_count;

	for (int i = 0; i < nInit_count; ++ i)
	{
		fish_trace_info.init_x_pos[i] = nInit_pos[i].x;
		fish_trace_info.init_y_pos[i] = kResolutionHeight - nInit_pos[i].y;
	}
	
	m_fish_multiple = g_LrbyGameConfig.m_gameConfig.fish_multiple[m_nFishKind];	
	m_bounding_box_width = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_width[m_nFishKind];
	m_bounding_box_height = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_height[m_nFishKind];

	m_nNorCount = normalCount;
	m_nDieCount = dieCount;

	float fish_speed = g_LrbyGameConfig.m_gameConfig.fish_speed[m_nFishKind];
	CalcFishSwimPath(nFishid, nTraceType, fish_trace_info.init_x_pos, fish_trace_info.init_y_pos, fish_trace_info.init_count, fish_speed);

	m_bClacFinish = true;

	return true;
}
//炸弹初始化函数
bool FishSprite::initBombFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	if (!Sprite::init())
	{
		return false;
	}
	m_nFishKind = id;

	//解析鱼类所需要的数据
	FishTraceInfo fish_trace_info = { 0 };
	fish_trace_info.init_count = nInit_count;
	for (int i = 0; i < fish_trace_info.init_count; ++ i)
	{
		fish_trace_info.init_x_pos[i] = nInit_pos[i].x;
		fish_trace_info.init_y_pos[i] = kResolutionHeight - nInit_pos[i].y;
	}
	

	m_fish_multiple = g_LrbyGameConfig.m_gameConfig.fish_multiple[m_nFishKind];
	m_bounding_box_width = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_width[m_nFishKind];
	m_bounding_box_height = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_height[m_nFishKind];
	
	float fish_speed = g_LrbyGameConfig.m_gameConfig.fish_speed[m_nFishKind];
	CalcFishSwimPath(nFishid, nTraceType, fish_trace_info.init_x_pos, fish_trace_info.init_y_pos, fish_trace_info.init_count, fish_speed);
	
	m_bClacFinish = true;

	return true;
}
//特殊鱼群初始化函数
bool FishSprite::initSuperFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	if (!Sprite::init())
	{
		return false;
	}

	if(id == FISH_KIND_25)
	{
		m_nFishSuperKing = FISH_KIND_4;
		m_nFishCount = 3;
	}
	else if(id == FISH_KIND_26)
	{
		m_nFishSuperKing = FISH_KIND_5;
		m_nFishCount = 3;
	}
	else if(id == FISH_KIND_27)
	{
		m_nFishSuperKing = FISH_KIND_6;
		m_nFishCount = 3;
	}
	else if(id == FISH_KIND_28)
	{
		m_nFishSuperKing = FISH_KIND_7;
		m_nFishCount = 4;
	}
	else if(id == FISH_KIND_29)
	{
		m_nFishSuperKing = FISH_KIND_8;
		m_nFishCount = 4;
	}
	else if(id == FISH_KIND_30)
	{
		m_nFishSuperKing = FISH_KIND_10;
		m_nFishCount = 4;
	}
	m_nFishKind = id;


	//解析鱼类所需要的数据
	FishTraceInfo fish_trace_info = { 0 };
	fish_trace_info.init_count = nInit_count;
	for (int i = 0; i < fish_trace_info.init_count; ++ i)
	{
		fish_trace_info.init_x_pos[i] = nInit_pos[i].x;
		fish_trace_info.init_y_pos[i] = kResolutionHeight - nInit_pos[i].y;
	}
	
	m_fish_multiple = g_LrbyGameConfig.m_gameConfig.fish_multiple[m_nFishKind];
	m_bounding_box_width = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_width[m_nFishKind];
	m_bounding_box_height = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_height[m_nFishKind];
	

	m_nNorCount = normalCount;
	m_nDieCount = dieCount;

	float fish_speed = g_LrbyGameConfig.m_gameConfig.fish_speed[m_nFishKind];
	CalcFishSwimPath(nFishid, nTraceType, fish_trace_info.init_x_pos, fish_trace_info.init_y_pos, fish_trace_info.init_count, fish_speed);

	m_bClacFinish = true;

	return true;
}
//鱼王鱼群初始化函数
bool FishSprite::initKingFish(int id, int normalCount, int dieCount, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	if (!Sprite::init())
	{
		return false;
	}

	if(id == FISH_KIND_31)
	{
		m_nFishSuperKing = FISH_KIND_1;
	}
	else if(id == FISH_KIND_32)
	{
		m_nFishSuperKing = FISH_KIND_2;
	}
	else if(id == FISH_KIND_33)
	{
		m_nFishSuperKing = FISH_KIND_3;
	}
	else if(id == FISH_KIND_34)
	{
		m_nFishSuperKing = FISH_KIND_4;
	}
	else if(id == FISH_KIND_35)
	{
		m_nFishSuperKing = FISH_KIND_5;
	}
	else if(id == FISH_KIND_36)
	{
		m_nFishSuperKing = FISH_KIND_6;
	}
	else if(id == FISH_KIND_37)
	{
		m_nFishSuperKing = FISH_KIND_7;
	}
	else if(id == FISH_KIND_38)
	{
		m_nFishSuperKing = FISH_KIND_8;
	}
	else if(id == FISH_KIND_39)
	{
		m_nFishSuperKing = FISH_KIND_9;
	}
	else if(id == FISH_KIND_40)
	{
		m_nFishSuperKing = FISH_KIND_10;
	}

	m_nFishKind = id;
	//m_nFishNum = nFish;
	//解析鱼类所需要的数据
	FishTraceInfo fish_trace_info = { 0 };
	fish_trace_info.init_count = nInit_count;
	for (int i = 0; i < fish_trace_info.init_count; ++ i)
	{
		fish_trace_info.init_x_pos[i] = nInit_pos[i].x;
		fish_trace_info.init_y_pos[i] = kResolutionHeight - nInit_pos[i].y;
	}

	m_fish_multiple = g_LrbyGameConfig.m_gameConfig.fish_multiple[m_nFishKind];
	m_bounding_box_width = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_width[m_nFishKind];
	m_bounding_box_height = g_LrbyGameConfig.m_gameConfig.fish_bounding_box_height[m_nFishKind];

	m_nNorCount = normalCount;
	m_nDieCount = dieCount;

	float fish_speed = g_LrbyGameConfig.m_gameConfig.fish_speed[m_nFishKind];
	CalcFishSwimPath(nFishid, nTraceType, fish_trace_info.init_x_pos, fish_trace_info.init_y_pos, fish_trace_info.init_count, fish_speed);

	m_bClacFinish = true;

	return true;
}

//普通鱼群游动动画
void FishSprite::playNormalSwimAnimation()
{
	if(m_fish != NULL)
	{
		m_fishStatue = FISH_SWIM;
		(m_fish[0])->stopAllActions();
		Animate *swim = Animate::create(m_SwimAnimation);
		RepeatForever *rf = RepeatForever::create(swim);
		(m_fish[0])->runAction(rf);
		(m_fish[0])->setColor(ccc3(255, 255, 255));
		
	}
}
//普通鱼群死亡动画
void FishSprite::playNormalDieAnimation()
{
	if((m_fish[0]) != NULL)
	{
		m_fishStatue = FISH_DIE;
		(m_fish[0])->stopAllActions();
		FiniteTimeAction *ft = Sequence::create(Animate::create(m_DieAnimation),CallFunc::create(this,callfunc_selector(FishSprite::removeFish)),NULL);
		(m_fish[0])->runAction(ft);
		(m_fish[0])->setColor(ccc3(255, 255, 255));
		
	}
}
//普通鱼群击中动画
void FishSprite::playNormalHitedAnimation(bool bSelf)
{
	if((m_fish[0]) != NULL)
	{
		m_fishStatue = FISH_HITED;
		(m_fish[0])->stopAllActions();
		FiniteTimeAction *rf = Sequence::create(Animate::create(m_SwimAnimation),CallFunc::create(this,callfunc_selector(FishSprite::playSwimAnimation)),NULL);
		if (bSelf)
		{
			(m_fish[0])->setColor(ccc3(255, 0, 0));
		}
		else
		{
			(m_fish[0])->setColor(ccc3(255, 255, 255));
		}
		
		(m_fish[0])->runAction(rf);
		
	}

}

//炸弹鱼游动动画
void FishSprite::playBombSwimAnimation()
{
	if(m_fish[0] != NULL)
	{
		m_fishStatue = FISH_SWIM;
		(m_fish[0])->stopAllActions();
		RotateBy* rotate = RotateBy::create(2.0f, 360.0f); 
		RepeatForever *rf = RepeatForever::create(rotate);
		(m_fish[0])->runAction(rf);
		(m_fish[0])->setColor(ccc3(255, 255, 255));
		
	}
}
//炸弹鱼死亡动画
void FishSprite::playBombDieAnimation()
{
	if((m_fish[0]) != NULL)
	{
		m_fishStatue = FISH_DIE;
		Vec2 pt = (m_fish[0])->getPosition();
		(m_fish[0])->removeFromParentAndCleanup(true);
		m_fish[0] = Sprite::createWithSpriteFrameName("fish24/fish24_d_002.png");
		this->addChild(m_fish[0]);
		this->setPosition(pt);

		FiniteTimeAction *ft = Sequence::create(RotateBy::create(2.0f, 360.0f),CallFunc::create(this,callfunc_selector(FishSprite::removeFish)),NULL);
		(m_fish[0])->runAction(ft);
	}
}
//炸弹鱼击中动画
void FishSprite::playBombHitedAnimation(bool bSelf)
{
	if((m_fish[0]) != NULL)
	{
		m_fishStatue = FISH_HITED;
		(m_fish[0])->stopAllActions();
		RotateBy* rotate = RotateBy::create(2.0f, 360); 
		FiniteTimeAction *rf = Sequence::create(rotate,CallFunc::create(this,callfunc_selector(FishSprite::playSwimAnimation)),NULL);
		if (bSelf)
		{
			(m_fish[0])->setColor(ccc3(255, 0, 0));
		}
		else
		{
			(m_fish[0])->setColor(ccc3(255, 255, 255));
		}
		(m_fish[0])->runAction(rf);
		
	}
}

//特殊鱼群游动动画
void FishSprite::playSuperSwimAnimation()
{
	m_fishStatue = FISH_SWIM;
	for(int i = 0; i < m_nFishCount; i ++)
	{
		if(m_fish[i] != nullptr)
		{
			(m_fish[i])->stopAllActions();
			Animate *swim = Animate::create(m_SwimAnimation);
			RepeatForever *rf = RepeatForever::create(swim);
			(m_fish[i])->runAction(rf);
			(m_fish[i])->setColor(ccc3(255, 255, 255));
		}
		if(m_wheel[i] != nullptr)
		{
			(m_wheel[i])->stopAllActions();
			RotateBy* rotate = RotateBy::create(2.0f, 360.0f); 
			RepeatForever *rf = RepeatForever::create(rotate);
			(m_wheel[i])->runAction(rf);
			(m_wheel[i])->setColor(ccc3(255, 255, 255));
		}
	}
	
}
//特殊鱼群死亡动画
void FishSprite::playSuperDieAnimation()
{
	m_fishStatue = FISH_DIE;
	for(int i = 0; i < m_nFishCount; i ++)
	{
		if(m_fish[i] != nullptr)
		{
			(m_fish[i])->stopAllActions();
			FiniteTimeAction *ft = Sequence::create(Animate::create(m_DieAnimation),CallFunc::create(this,callfunc_selector(FishSprite::removeFish)),NULL);
			(m_fish[i])->runAction(ft);
			(m_fish[i])->setColor(ccc3(255, 255, 255));
		}
		if(m_wheel[i] != nullptr)
		{
			(m_wheel[i])->stopAllActions();
			RotateBy* rotate = RotateBy::create(1.0f, 720.0f); 
			RepeatForever *rf = RepeatForever::create(rotate);
			(m_wheel[i])->runAction(rf);
			(m_wheel[i])->setColor(ccc3(255, 255, 255));
		}
	}
	
}
//特殊鱼群击中动画
void FishSprite::playSuperHitedAnimation(bool bSelf)
{
	m_fishStatue = FISH_HITED;
	for(int i = 0; i < m_nFishCount; i ++)
	{
		if(m_fish[i] != nullptr)
		{
			(m_fish[i])->stopAllActions();
			FiniteTimeAction *rf = Sequence::create(Animate::create(m_SwimAnimation),CallFunc::create(this,callfunc_selector(FishSprite::playSwimAnimation)),NULL);
			if (bSelf)
			{
				(m_fish[i])->setColor(ccc3(255, 0, 0));
			}
			else
			{
				(m_fish[i])->setColor(ccc3(255, 255, 255));
			}
			(m_fish[i])->runAction(rf);
		}
		if(m_wheel[i] != nullptr)
		{
			(m_wheel[i])->stopAllActions();
			RotateBy* rotate = RotateBy::create(2.0f, 360.0f); 
			FiniteTimeAction *rf = Sequence::create(rotate,CallFunc::create(this,callfunc_selector(FishSprite::playSwimAnimation)),NULL);
			(m_wheel[i])->runAction(rf);
			if (bSelf)
			{
				(m_wheel[i])->setColor(ccc3(255, 0, 0));
			}
			else
			{
				(m_wheel[i])->setColor(ccc3(255, 255, 255));
			}
		}
	}
	
}

//鱼王游动动画
void FishSprite::playKingSwimAnimation()
{
	m_fishStatue = FISH_SWIM;

		if(m_fish[0] != nullptr)
		{
			(m_fish[0])->stopAllActions();
			Animate *swim = Animate::create(m_SwimAnimation);
			RepeatForever *rf = RepeatForever::create(swim);
			(m_fish[0])->runAction(rf);
			(m_fish[0])->setColor(ccc3(255, 255, 255));
		}
		if(m_wheel[0] != nullptr)
		{
			(m_wheel[0])->stopAllActions();
			RotateBy* rotate = RotateBy::create(2.0f, 360.0f); 
			RepeatForever *rf = RepeatForever::create(rotate);
			(m_wheel[0])->runAction(rf);
			(m_wheel[0])->setColor(ccc3(255, 255, 255));
		}
		
}
//鱼王死亡动画
void FishSprite::playKingDieAnimation()
{
	m_fishStatue = FISH_DIE;
		if(m_fish[0] != nullptr)
		{
			(m_fish[0])->stopAllActions();
			FiniteTimeAction *ft = Sequence::create(Animate::create(m_DieAnimation),CallFunc::create(this,callfunc_selector(FishSprite::removeFish)),NULL);
			(m_fish[0])->runAction(ft);
			(m_fish[0])->setColor(ccc3(255, 255, 255));
		}
		if(m_wheel[0] != nullptr)
		{
			(m_wheel[0])->stopAllActions();
			RotateBy* rotate = RotateBy::create(1.0f, 720.0f); 
			RepeatForever *rf = RepeatForever::create(rotate);
			(m_wheel[0])->runAction(rf);
			(m_wheel[0])->setColor(ccc3(255, 255, 255));
		}
		
}
//鱼王击中动画
void FishSprite::playKingHitedAnimation(bool bSelf)
{

	m_fishStatue = FISH_HITED;
		if(m_fish[0] != NULL)
		{
			(m_fish[0])->stopAllActions();
			FiniteTimeAction *rf = Sequence::create(Animate::create(m_SwimAnimation),CallFunc::create(this,callfunc_selector(FishSprite::playSwimAnimation)),NULL);

			if (bSelf)
			{
				(m_fish[0])->setColor(ccc3(255, 0, 0));
			}
			else
			{
				(m_fish[0])->setColor(ccc3(255, 255, 255));
			}
			(m_fish[0])->runAction(rf);
		}
		if(m_wheel[0] != nullptr)
		{
			(m_wheel[0])->stopAllActions();
			RotateBy* rotate = RotateBy::create(2.0f, 360.0f); 
			FiniteTimeAction *rf = Sequence::create(rotate,CallFunc::create(this,callfunc_selector(FishSprite::playSwimAnimation)),NULL);
			if (bSelf)
			{
				(m_wheel[0])->setColor(ccc3(255, 0, 0));
			}
			else
			{
				(m_wheel[0])->setColor(ccc3(255, 255, 255));
			}
			(m_wheel[0])->runAction(rf);
		}
		
}

int FishSprite::getFishMutiple()
{
	return m_fish_multiple;
}

void FishSprite::removeFish()
{
	m_fishStatue = FISH_DIE;
	FishManager* fm = (FishManager*)this->getParent();
	if(fm != NULL)
	{
		fm->removeFish(this);
	}
}

void FishSprite::CalcFishSwimPath(int nFishid, TraceType nTraceType, float init_x_pos[], float init_y_pos[], int init_count, float fSpeed)
{
	m_fish_id = nFishid;
	m_trace_type = nTraceType;
	if(init_count < 2)
		return;
	if(m_trace_type == TRACE_LINEAR)
	{
		MathAide::BuildLinear(init_x_pos, init_y_pos, init_count, m_vecFishPath, fSpeed);
	}
	else
	{
		MathAide::BuildBezier(init_x_pos, init_y_pos, init_count,m_vecFishPath, fSpeed);
	}
}

void FishSprite::getFishBoundingBox(BoundingBox &box)
{
	FPointAngle curPoint = m_vecFishPath.at(m_nPathCount);
	box.CreateBoundingBox(m_bounding_box_width, m_bounding_box_height, curPoint.x, curPoint.y, curPoint.angle);
	return ;
}

void FishSprite::setTraceType(TraceType Ttrace)
{
	m_trace_type = Ttrace;
}

void FishSprite::SetFishStop(int nStopIndex, int nStopCount)
{
	stop_index_ = nStopIndex;
	stop_count_ = nStopCount;
	current_stop_count_ = 0;
}

void FishSprite::playSwimAnimation()
{
	if(m_fishStatue == FISH_DIE)
		return;
	if(m_fishStatue == FISH_SWIM)
		return;
	if((m_nFishKind >= FISH_KIND_1) && (m_nFishKind <= FISH_KIND_23))
	{
		playNormalSwimAnimation();
	}
	else if(m_nFishKind == FISH_KIND_24)		//旋转的超级炸弹
	{
		playBombSwimAnimation();
	}
	else if((m_nFishKind >= FISH_KIND_25) && (m_nFishKind <= FISH_KIND_30))
	{
		playSuperSwimAnimation();
	}
	else if((m_nFishKind >= FISH_KIND_31) && (m_nFishKind <= FISH_KIND_40))
	{
		playKingSwimAnimation();
	}
}
void FishSprite::playDieAnimation()
{
	if(m_fishStatue == FISH_DIE)
		return;
	if((m_nFishKind >= FISH_KIND_1) && (m_nFishKind <= FISH_KIND_23))
	{
		playNormalDieAnimation();
	}
	else if(m_nFishKind == FISH_KIND_24)		//旋转的超级炸弹
	{
		playBombDieAnimation();
	}
	else if((m_nFishKind >= FISH_KIND_25) && (m_nFishKind <= FISH_KIND_30))
	{
		playSuperDieAnimation();
	}
	else if((m_nFishKind >= FISH_KIND_31) && (m_nFishKind <= FISH_KIND_40))
	{
		playKingDieAnimation();
	}
	//else if (m_nFishKind >= FISH_KIND_41)
	//{
	//	playNormalDieAnimation();
	//}
}
void FishSprite::playHitedAnimation(bool bSelf)
{
	if(m_fishStatue == FISH_DIE)
		return;
	if(m_fishStatue == FISH_HITED)
		return;
	if((m_nFishKind >= FISH_KIND_1) && (m_nFishKind <= FISH_KIND_23))
	{
		playNormalHitedAnimation(bSelf);
	}
	else if(m_nFishKind == FISH_KIND_24)		//旋转的超级炸弹
	{
		playBombHitedAnimation(bSelf);
	}
	else if((m_nFishKind >= FISH_KIND_25) && (m_nFishKind <= FISH_KIND_30))
	{
		playSuperHitedAnimation(bSelf);
	}
	else if((m_nFishKind >= FISH_KIND_31) && (m_nFishKind <= FISH_KIND_40))
	{
		playKingHitedAnimation(bSelf);
	}
}

void FishSprite::setLKMultiple(int nMul)
{
	if(nMul <= 0)
		nMul = m_fish_multiple;
	if(m_labelLKNum != nullptr)
	{
		m_labelLKNum->setString(String::createWithFormat("%d", nMul)->getCString());
	}
}

void FishSprite::funCommomFish()
{
	Vector<SpriteFrame*> normalFrames(m_nNorCount);
	//死亡动画
	Vector<SpriteFrame*> dieFrames;
	if ((m_nFishKind==FISH_KIND_22)||(m_nFishKind==FISH_KIND_23))
	{
		for(int i = 1; i <= m_nNorCount; i ++)
		{
			char str[64] = {0};
			sprintf(str, "fish%d/fish%d_%03d.png", m_nFishKind + 1, m_nFishKind + 1, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			normalFrames.pushBack(frame);
			dieFrames.pushBack(frame);
		}
	}
	else
	{
		for(int i = 1; i <= m_nNorCount; i ++)
		{
			char str[64] = {0};
			sprintf(str, "fish%d/fish%d_%03d.png", m_nFishKind + 1, m_nFishKind + 1, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			normalFrames.pushBack(frame);
		}

		for(int i = 1; i <= m_nDieCount; i ++)
		{
			char str1[64] = {0};
			sprintf(str1, "fish%d_d/fish%d_d_%03d.png", m_nFishKind + 1, m_nFishKind + 1, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
			dieFrames.pushBack(frame);
		}
	}

	m_SwimAnimation = Animation::createWithSpriteFrames(normalFrames, 0.1f);
	m_SwimAnimation->retain();	

	m_DieAnimation = Animation::createWithSpriteFrames(dieFrames, 0.15f);
	m_DieAnimation->retain();

	m_node = Node::create();
	m_node->setAnchorPoint(Vec2(0.0, 0.0));
	this->addChild(m_node);
	m_fish[0] = Sprite::createWithSpriteFrameName(String::createWithFormat("fish%d/fish%d_000.png", m_nFishKind + 1, m_nFishKind + 1)->getCString());
	CCASSERT(m_fish,"fish is not exist!");
	m_node->addChild(m_fish[0]);

	if(m_nFishKind == FISH_KIND_21)
	{
		m_labelLKNum = LabelAtlas::create(String::createWithFormat("%d", m_fish_multiple)->getCString(), "709/score_num.png", 33, 38, '0');
		m_labelLKNum->setPosition(ccp(0, 110));
		m_node->addChild(m_labelLKNum, 2);
	}
	
	playSwimAnimation();

	this->setPosition(ccp(-100, -300));
	this->setAnchorPoint(ccp(0.5, 0.5));

	m_nPathCount = 1;
	stop_index_ = 0;
	stop_count_ = 0;
	current_stop_count_ = 0;
}

void FishSprite::funBombFish()
{
	m_node = Node::create();
	m_node->setAnchorPoint(Vec2(0.0, 0.0));
	this->addChild(m_node);
	m_fish[0] = Sprite::createWithSpriteFrameName("fish24/fish24_d_001.png");
	CCASSERT(m_fish[0],"fish is not exist!");
	m_node->addChild(m_fish[0]);

	playSwimAnimation();

	this->setPosition(ccp(-100, -300));

	m_nPathCount = 1;
	stop_index_ = 0;
	stop_count_ = 0;
	current_stop_count_ = 0;
}

void FishSprite::funSuperFish()
{
	Vector<SpriteFrame*> normalFrames(m_nNorCount);
	for(int i = 1; i <= m_nNorCount; i ++)
	{
		char str[64] = {0};
		sprintf(str, "fish%d/fish%d_%03d.png", m_nFishSuperKing + 1, m_nFishSuperKing + 1, i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		normalFrames.pushBack(frame);
	}
	m_SwimAnimation = Animation::createWithSpriteFrames(normalFrames, 0.1f);
	m_SwimAnimation->retain();

	//死亡动画
	Vector<SpriteFrame*> dieFrames(m_nDieCount);
	for(int i = 1; i <= m_nDieCount; i ++)
	{
		char str1[64] = {0};
		sprintf(str1, "fish%d_d/fish%d_d_%03d.png", m_nFishSuperKing + 1, m_nFishSuperKing + 1, i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
		dieFrames.pushBack(frame);
	}
	m_DieAnimation = Animation::createWithSpriteFrames(dieFrames, 0.15f);
	m_DieAnimation->retain();

	m_node = Node::create();
	m_node->setAnchorPoint(Vec2(0.0, 0.0));
	this->addChild(m_node);
	float ptPos = -(m_nFishCount-1) * 120.0f/2.0f;
	for(int i = 0; i < m_nFishCount; i ++)
	{
		m_fish[i] = Sprite::createWithSpriteFrameName(String::createWithFormat("fish%d/fish%d_000.png", m_nFishSuperKing + 1, m_nFishSuperKing + 1)->getCString());
		CCASSERT(m_fish[i],"fish is not exist!");
		m_node->addChild(m_fish[i], 2);
		(m_fish[i])->setPosition(ccp(0 + ptPos, 0.0f));

		m_wheel[i] = Sprite::createWithSpriteFrameName("dish.png");
		CCASSERT(m_wheel[i],"dish is not exist!");
		m_node->addChild(m_wheel[i], 1);
		(m_wheel[i])->setPosition(ccp(0 + ptPos, 0.0f));
		ptPos+=120.0f;
	}

	playSwimAnimation();

	this->setPosition(ccp(-100, -300));

	m_nPathCount = 1;
	stop_index_ = 0;
	stop_count_ = 0;
	current_stop_count_ = 0;
}

void FishSprite::funKingFish()
{
	//正常游动
	Vector<SpriteFrame*> normalFrames(m_nNorCount);
	for(int i = 1; i <= m_nNorCount; i ++)
	{
		char str[64] = {0};
		sprintf(str, "fish%d/fish%d_%03d.png", m_nFishSuperKing + 1, m_nFishSuperKing + 1, i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		normalFrames.pushBack(frame);
	}
	m_SwimAnimation = Animation::createWithSpriteFrames(normalFrames, 0.1f);
	m_SwimAnimation->retain();
	//死亡动画
	Vector<SpriteFrame*> dieFrames(m_nDieCount);
	for(int i = 1; i <= m_nDieCount; i ++)
	{
		char str1[64] = {0};
		sprintf(str1, "fish%d_d/fish%d_d_%03d.png", m_nFishSuperKing + 1, m_nFishSuperKing + 1, i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
		dieFrames.pushBack(frame);
	}
	m_DieAnimation = Animation::createWithSpriteFrames(dieFrames, 0.15f);
	m_DieAnimation->retain();
	m_node = Node::create();
	m_node->setAnchorPoint(Vec2(0.0, 0.0));
	this->addChild(m_node);

	m_fish[0] = Sprite::createWithSpriteFrameName(String::createWithFormat("fish%d/fish%d_000.png", m_nFishSuperKing + 1, m_nFishSuperKing + 1)->getCString());
	CCASSERT(m_fish[0],"fish is not exist!");
	m_node->addChild(m_fish[0], 2);
	(m_fish[0])->setPosition(ccp(0, 0));

	m_wheel[0] = Sprite::createWithSpriteFrameName("halo.png");
	CCASSERT(m_wheel[0],"halo is not exist!");
	m_node->addChild(m_wheel[0], 1);
	(m_wheel[0])->setPosition(ccp(0, 0));

	playSwimAnimation();

	this->setPosition(ccp(-100, -300));
	m_nPathCount = 1;
	stop_index_ = 0;
	stop_count_ = 0;
	current_stop_count_ = 0;
}

cocos2d::Vec2 FishSprite::getFishNextPosition(int nCount)
{
	Vec2 ptFish = ccp(0.0f, 0.0f);
	int nPathCount = m_nPathCount+nCount;
	if(nPathCount >= m_vecFishPath.size())
	{
		nPathCount = m_vecFishPath.size()-1;
	}

	FPointAngle curPoint = m_vecFishPath.at(nPathCount);
	ptFish.x = curPoint.x;
	ptFish.y = curPoint.y;
	return ptFish;
}

cocos2d::Vec2 FishSprite::getFishPosition()
{
	Vec2 ptFish = ccp(0.0f, 0.0f);
	FPointAngle curPoint = m_vecFishPath.at(m_nPathCount);
	ptFish.x = curPoint.x;
	ptFish.y = curPoint.y;
	return ptFish;
}

// 返回false表示鱼删除了
bool FishSprite::updateFish(float dt)
{
	if (m_fishStatue == FISH_DIE)
	{
		return true;
	}
	if (stop_count_ > 0 && m_nPathCount == stop_index_ && current_stop_count_ < stop_count_)
	{
		++current_stop_count_;
		if (current_stop_count_ >= stop_count_)
		{
			SetFishStop(0, 0);
		}
		return true;
	}
	else
	{
		if((m_nPathCount+1) >= m_vecFishPath.size())
		{
			this->removeFish();
			return false;
		}
		++m_nPathCount;
	}

	FPointAngle curPoint = m_vecFishPath.at(m_nPathCount);
	this->setPosition(ccp(curPoint.x, curPoint.y));
	this->setRotate(curPoint.angle);
	return true;
}
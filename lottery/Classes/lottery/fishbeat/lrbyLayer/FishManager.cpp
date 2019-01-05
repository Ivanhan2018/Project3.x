#include "FishManager.h"
#include "CMD_Fish.h"

//静态变量
FishManager * FishManager::m_pFishManager = nullptr;					//对象指针

FishManager::FishManager()
{
	m_fishList.clear();
	m_pFishManager = this;
}

FishManager::~FishManager()
{

}

bool FishManager::init()
{
	if(!Node::init())
		return false;

	return true;
}
void FishManager::addFish(FishSprite* fish)
{
	if(fish == nullptr) return;
	m_fishList.pushBack(fish);	

	int nFishType = fish->m_nFishKind;
	if (fish->m_bClacFinish)
	{
		if((nFishType >= FISH_KIND_1) && (nFishType <= FISH_KIND_23))			//普通鱼
		{
			fish->funCommomFish();
		}
		else if(nFishType == FISH_KIND_24)										//炸弹鱼
		{
			fish->funBombFish();
		}
		else if((nFishType >= FISH_KIND_25) && (nFishType <= FISH_KIND_30))		//超级鱼
		{
			fish->funSuperFish();
		}
		else if((nFishType >= FISH_KIND_31) && (nFishType <= FISH_KIND_40))		//鱼王
		{
			fish->funKingFish();
		}
		//else if (nFishType == FISH_KIND_41)
		//{
		//	fish->funCommomFish();
		//}
	}
	this->addChild(fish);
}

void FishManager::removeFish(FishSprite* fish)
{
	if(fish)
	{
		fish->removeFromParentAndCleanup(true);
		m_fishList.eraseObject(fish, true);
	}
}

void FishManager::clearAllFish()
{
	FishSprite* sp = nullptr;
	for(int i = 0; i < m_fishList.size(); i ++)
	{
		sp = m_fishList.at(i);
		sp->removeFromParentAndCleanup(true);
	}
	m_fishList.clear();
}

FishManager* FishManager::GetInstance()
{
	if(m_pFishManager == nullptr)
	{
		//m_pFishManager = new(std::nothrow) FishManager();
		m_pFishManager = FishManager::create();
	}
	return m_pFishManager;
}

FishSprite* FishManager::createFish(int nFish, int nFishKind, int nFishid, TraceType nTraceType, const FPoint nInit_pos[], int nInit_count)
{
	int nIndex = nFishKind;
	int nNormalCount = 0, nDieCount = 0;
	
	getFishAnimationCount(nIndex, nNormalCount, nDieCount);
	
	FishSprite *fish = nullptr;
	if((nIndex >= FISH_KIND_1) && (nIndex <= FISH_KIND_23))
	{
		fish = FishSprite::createCommonFishByID(nIndex, nNormalCount, nDieCount, nFishid, nTraceType, nInit_pos, nInit_count);
		fish->retain();
	}
	else if(nIndex == FISH_KIND_24)		//旋转的超级炸弹
	{
		fish = FishSprite::createBombFishByID(nIndex, nNormalCount, nDieCount, nFishid, nTraceType, nInit_pos, nInit_count);
		fish->retain();
	}
	else if((nIndex >= FISH_KIND_25) && (nIndex <= FISH_KIND_30))
	{
		fish = FishSprite::createSuperFishByID(nIndex, nNormalCount, nDieCount, nFishid, nTraceType, nInit_pos, nInit_count);
		fish->retain();
	}
	else if((nIndex >= FISH_KIND_31) && (nIndex <= FISH_KIND_40))
	{
		fish = FishSprite::createKingFishByID(nIndex, nNormalCount, nDieCount, nFishid, nTraceType, nInit_pos, nInit_count);
		fish->retain();
	}
	//else if (nIndex == FISH_KIND_41)
	//{
	//	fish = FishSprite::createCommonFishByID(nIndex, nNormalCount, nDieCount, nFishid, nTraceType, nInit_pos, nInit_count);
	//	fish->retain();
	//}

	return fish;
}

void FishManager::getFishAnimationCount(int nFishId, int& nNormalCount, int& nDieCount)
{
	switch (nFishId)
	{

	case FISH_KIND_1:	//0:
		{
			nNormalCount = 11;
			nDieCount = 5;
		}
		break;
	case FISH_KIND_2:
		{
			nNormalCount = 15;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_3:	
		{
			nNormalCount = 23;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_4:	
		{
			nNormalCount = 23;
			nDieCount = 9;
		}
		break;
	case FISH_KIND_5:		
		{
			nNormalCount = 23;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_6:	
		{
			nNormalCount = 24;
			nDieCount = 5;
		}
		break;
	case FISH_KIND_7:	
		{
			nNormalCount = 59;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_8:	
		{
			nNormalCount = 19;
			nDieCount = 5;
		}
		break;
	case FISH_KIND_9:	
		{
			nNormalCount = 23;
			nDieCount = 4;
		}
		break;
	case FISH_KIND_10:	
		{
			nNormalCount = 15;
			nDieCount = 7;
		}
		break;
	case FISH_KIND_11:	
		{
			nNormalCount = 23;
			nDieCount = 4;
		}
		break;
	case FISH_KIND_12:	
		{
			nNormalCount = 11;
			nDieCount = 4;
		}
		break;
	case FISH_KIND_13:	
		{
			nNormalCount = 23;
			nDieCount = 4;
		}
		break;
	case FISH_KIND_14:	
		{
			nNormalCount = 19;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_15:	
		{
			nNormalCount = 23;
			nDieCount = 6;
		}
		break;
	case FISH_KIND_16:	
		{
			nNormalCount = 23;
			nDieCount = 6;
		}
		break;
	case FISH_KIND_17:	
		{
			nNormalCount = 23;
			nDieCount = 4;
		}
		break;
	case FISH_KIND_18:	
		{
			nNormalCount = 8;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_19:
		{
			nNormalCount = 8;
			nDieCount = 4;
		}
		break;
	case FISH_KIND_20:
		{
			nNormalCount = 19;
			nDieCount = 19;
		}
		break;
	case FISH_KIND_21:
		{
			nNormalCount = 14;
			nDieCount = 8;
		}
		break;
	case FISH_KIND_22:
		{
			nNormalCount = 14;
			nDieCount = 14;
		}
		break;
	case FISH_KIND_23: //23
		{
			nNormalCount = 7;
			nDieCount = 7;
		}
		break;
	case FISH_KIND_24: //炸弹旋转
		{
			nNormalCount = 0;
			nDieCount = 0;
		}
		break;
	case FISH_KIND_25: //大三元1 FISH_KIND_4
		{
			nNormalCount = 23;
			nDieCount = 9;
		}
		break;
	case FISH_KIND_26: //大三元2
		{
			nNormalCount = 23;	//59;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_27: //大三元3
		{
			nNormalCount = 23;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_28: //大四喜1
		{
			nNormalCount = 59;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_29: //大四喜2
		{
			nNormalCount = 19;
			nDieCount = 5;
		}
		break;
	case FISH_KIND_30: //大四喜3
		{
			nNormalCount = 15;
			nDieCount = 4;
		}
		break;

	case FISH_KIND_31:	//鱼王1 FISH_KIND_1
		{
			nNormalCount = 11;
			nDieCount = 5;
		}
		break;
	case FISH_KIND_32:	//鱼王2
		{
			nNormalCount = 15;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_33:	//鱼王3
		{
			nNormalCount = 23;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_34:	//鱼王4
		{
			nNormalCount = 23;
			nDieCount = 9;
		}
		break;
	case FISH_KIND_35:	//鱼王5
		{
			nNormalCount = 23;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_36:	//鱼王6
		{
			nNormalCount = 24;
			nDieCount = 5;
		}
		break;
	case FISH_KIND_37:	//鱼王7
		{
			nNormalCount = 59;
			nDieCount = 3;
		}
		break;
	case FISH_KIND_38:	//鱼王8
		{
			nNormalCount = 19;
			nDieCount = 5;
		}
		break;
	case FISH_KIND_39:	//鱼王9
		{
			nNormalCount = 23;
			nDieCount = 4;
		}
		break;
	case FISH_KIND_40:	//鱼王10
		{
			nNormalCount = 15;
			nDieCount = 7;
		}
		break;
	//case FISH_KIND_41:  //奖券鱼
	//	{
	//		nNormalCount = 11;
	//		nDieCount = 0;
	//	}
		break;
	default:
		{
			nNormalCount = 0;
			nDieCount = 0;
			log("the special fish,does not code!!!!!!!!!!!!!!!!!!!");
		}
		break;
	}
}

void FishManager::destroy()
{
	this->m_pFishManager = nullptr;
}

int FishManager::lockFish(FishKind* lock_fish_kind, int last_lock_fish_id, FishKind last_fish_kind) 
{
	FishSprite* fish = NULL;
	std::vector<FishSprite*>::iterator iter;
	FishKind next_fish_kind = last_fish_kind;
	if (next_fish_kind != FISH_KIND_COUNT) 
	{
		bool exist_fish_kind[FISH_KIND_COUNT];
		memset(exist_fish_kind, 0, sizeof(exist_fish_kind));
		for (int i = 0; i < m_fishList.size(); i++)
		{
			fish =  m_fishList.at(i);
			if (fish->getFishStatue() == FISH_DIE) continue;
			if (fish->getFishId() == last_lock_fish_id) continue;
			if (fish->getFishKind() < FISH_KIND_16) continue;
			if (fish->m_nPathCount + 1 >= fish->m_vecFishPath.size()) continue;
			Vec2 position = fish->getFishPosition();
			if (!insideScreen(position)) continue;
			exist_fish_kind[fish->getFishKind()] = true;
		}
		
		for (int i = FISH_KIND_16; i < FISH_KIND_COUNT; ++i) 
		{
			next_fish_kind = static_cast<FishKind>((next_fish_kind + 1) % FISH_KIND_COUNT);
			if (next_fish_kind < FISH_KIND_16) next_fish_kind = FISH_KIND_16;
			if (exist_fish_kind[next_fish_kind]) break;
		}
	}

	for (int i = 0; i < m_fishList.size(); i++)
	{
		fish =  m_fishList.at(i);
		if (fish->getFishStatue() == FISH_DIE) continue;
		if (fish->getFishId() == last_lock_fish_id) continue;
		if (fish->getFishKind() < FISH_KIND_16) continue;
		if (fish->m_nPathCount + 1 >= fish->m_vecFishPath.size()) continue;
		Vec2 position = fish->getFishPosition();
		if (!insideScreen(position)) continue;
		if (next_fish_kind == FISH_KIND_COUNT || next_fish_kind == fish->getFishKind()) {
			if (lock_fish_kind != NULL) *lock_fish_kind = static_cast<FishKind>(fish->getFishKind());
			return fish->getFishId();
		}
	}

	if (last_lock_fish_id > 0) {
		if (lock_fish_kind != NULL) *lock_fish_kind = last_fish_kind;
		return last_lock_fish_id;
	} else {
		if (lock_fish_kind != NULL) *lock_fish_kind = FISH_KIND_COUNT;
		return 0;
	}
}

bool FishManager::insideScreen(Vec2 pos)
{
	if (pos.x >= 0.0f && pos.x <= kResolutionWidth && pos.y >= 0.0f && pos.y <= kResolutionHeight) return true;
	return false;
}

FishSprite* FishManager::getFishById(int fishId)
{
	if (fishId <= 0)
	{
		return NULL;
	}
	FishSprite* tempFish = NULL;
	if (m_fishList.size() > 0)
	{
		Vector<FishSprite *>::iterator iter = m_fishList.begin();
		while( iter != m_fishList.end())  
		{  
			if ((*iter)->getFishId() == fishId)
			{
				tempFish = *iter;
				break;
			}
			else
			{
				iter ++ ;  
			}
		}
		
		if (tempFish == NULL || tempFish->getFishStatue() == FISH_DIE || !insideScreen(tempFish->getFishPosition()))
		{
			return NULL;
		}
		
	}
	return tempFish;
}

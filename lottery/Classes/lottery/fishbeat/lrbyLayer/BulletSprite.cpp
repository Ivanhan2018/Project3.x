#include "BulletSprite.h"
#include "NetSprite.h"

#include "math_aide.h"
#include "LrbyGameConfig.h"
#include "LrbyClientView.h"

using namespace std;
using namespace NS_lrbyFish;

extern  int ptPositionX[BY_GAME_PLAYER];// = {(int)(kResolutionWidth * 0.2), (int)(kResolutionWidth * 0.55),(int)(kResolutionWidth * 0.9), (int)(kResolutionWidth * 0.80), (int)(kResolutionWidth * 0.45), (int)(kResolutionWidth * 0.1)};
extern  int ptPositionY[2] ;//= {(int)(kResolutionHeight * 0.95), (int)(kResolutionHeight * 0.05)};

USING_NS_CC;

extern CLrbyGameConfig g_LrbyGameConfig;

BulletSprite::BulletSprite():
	m_nBulletOfChair(-1),
	m_bulletBg(nullptr),
	m_pLockFish(nullptr),
	m_bulletMulriple(1),
	m_nPathCount(0),
	m_android_chairid(-1),
	m_LockFishId(0),
	m_fireChairId(0)
{
	m_ptSrc.x=0;
	m_ptSrc.y=0;
}

BulletSprite::~BulletSprite()
{
}

bool BulletSprite::init()
{
	if ( !Node::init() )
	{
		return false;
	}
	m_nBulletId = 0;
	m_bulletStatue = BULLET_KIND_1_NORMAL;
	m_bulletMulriple = 1;

	return true;
}

//切换子弹，更改子弹的图片
void BulletSprite::setBulletStatue(int nStatue, int bulletMulriple, Vec2 pt, float fAngle, int nChairID)
{
	if (nStatue<BULLET_KIND_1_NORMAL || nStatue>=BULLET_KIND_COUNT)
	{
		nStatue = BULLET_KIND_1_NORMAL;
	}
	m_bulletStatue = nStatue;
	m_bulletMulriple = bulletMulriple; 
	m_android_chairid = -1;
	m_startPos = pt;

	if((nStatue <= BULLET_KIND_4_NORMAL) && (nStatue >= BULLET_KIND_1_NORMAL))
	{
		char strBullet[64] = {0};
		//这里只有6个 nchairid只到5 0-2 3 4-6 7

		sprintf(strBullet, "bullet/bullet%d/bullet%d_norm%d_01.png", m_bulletStatue + 1, m_bulletStatue + 1, BulletSprite::ExchangeChairID(nChairID));
		m_bulletBg = Sprite::createWithSpriteFrameName(strBullet);
		this->addChild(m_bulletBg, 100);
		m_bulletBg->setPosition(0, 0);
		this->setPosition(Vec2(pt.x, pt.y));
		
		//开火动画
		Vector<SpriteFrame*> fireFrames(2);
		for(int i = 1; i <= 2; i ++)
		{
			char str[64] = {0};
			sprintf(str, "bullet/bullet%d/bullet%d_norm%d_%02d.png", m_bulletStatue + 1, m_bulletStatue + 1, BulletSprite::ExchangeChairID(nChairID), i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			fireFrames.pushBack(frame);
		}
		Animation *fireAnimation = Animation::createWithSpriteFrames(fireFrames, 0.2f);
		//fireAnimation->retain();
		m_bulletBg->runAction(RepeatForever::create(Animate::create(fireAnimation)));
	}
	else
	{
		this->setPosition(Vec2(pt.x, pt.y));

		char strBullet[64] = {0};
		sprintf(strBullet, "bullet/bullet%d/bullet%d_ion_01.png", m_bulletStatue - BULLET_KIND_4_NORMAL, m_bulletStatue - BULLET_KIND_4_NORMAL);		//"bullet_%d_1.png", m_bulletStatue);
		m_bulletBg = Sprite::createWithSpriteFrameName(strBullet);
		this->addChild(m_bulletBg, 300);
		m_bulletBg->setPosition(0, 0);
		
		//开火动画
		Vector<SpriteFrame*> fireFrames(2);
		for(int i = 1; i <= 2; i ++)
		{
			char str[64] = {0};
			sprintf(str, "bullet/bullet%d/bullet%d_ion_%02d.png", m_bulletStatue - BULLET_KIND_4_NORMAL, m_bulletStatue - BULLET_KIND_4_NORMAL, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			fireFrames.pushBack(frame);
		}
		Animation *fireAnimation = Animation::createWithSpriteFrames(fireFrames, 0.2f);
		//fireAnimation->retain();
		m_bulletBg->runAction(RepeatForever::create(Animate::create(fireAnimation)));
	}

	m_ptSrc = pt;
	m_nPathCount = 5;
	setBulletAngle(fAngle);
	setbulletSpeed(g_LrbyGameConfig.m_gameConfig.bullet_speed[nStatue]);

	//if (m_pLockFish != NULL)
	//{
	//	Vec2 pEnd = m_pLockFish->getFishPosition();
	//	float dis = MathAide::CalcDistance(pt.x, pt.y, pEnd.x, pEnd.y);
	//	runBezier(pt, pt, m_pLockFish->getFishPosition(), dis);
	//}
}

void GetBulletTargetPoint(float screen_width, float screen_height, float src_x_pos, float src_y_pos, float &angle, float& target_x_pos, float& target_y_pos)
{
	// 转换为0到2*M_PI
	while (angle <= -M_PI * 2) {
		angle += M_PI * 2;
	}
	if (angle < 0.f) angle += M_PI * 2;
	while (angle >= M_PI * 2) {
		angle -= M_PI * 2;
	}

	if (angle < M_PI_2)
	{
		target_x_pos = screen_width;
		target_y_pos = src_y_pos + (screen_width - src_x_pos) * tanf(angle);		
		if ((target_y_pos>screen_height) && fabsf(target_y_pos-src_y_pos)>0.000001f)
		{
			target_y_pos = screen_height;
			target_x_pos = src_x_pos + (screen_height - src_y_pos)/tanf(angle);
			angle = 2*M_PI-angle;
		}
		else
		{
			angle = M_PI-angle;
		}
	} 
	else if (angle >= M_PI_2 && angle < M_PI) 
	{
		target_x_pos = 0;
		target_y_pos = src_y_pos - (src_x_pos) * tanf(angle);
		if ((target_y_pos>screen_height) && fabsf(target_y_pos-src_y_pos)>0.000001f)
		{
			target_y_pos = screen_height;
			target_x_pos = src_x_pos + (screen_height - src_y_pos)/tanf(angle);
			angle = 2*M_PI-angle;
		}
		else
		{
			angle = M_PI-angle;
		}

	} 
	else if (angle >= M_PI && angle < (M_PI+M_PI_2))
	{
		target_x_pos = 0;
		target_y_pos = src_y_pos - (src_x_pos ) * tanf(angle);
		if (target_y_pos<0 && fabsf(target_y_pos-src_y_pos)>0.000001f)
		{
			target_y_pos = 0;
			target_x_pos = src_x_pos + (0 - src_y_pos)/tanf(angle);

			angle = 2*M_PI-angle;
		}
		else
		{
			angle = 3*M_PI-angle;
		}

	}
	else
	{
		target_x_pos = screen_width ;
		target_y_pos = src_y_pos + (screen_width - src_x_pos ) * tanf(angle);
		if (target_y_pos<0 && fabsf(target_y_pos-src_y_pos)>0.000001f)
		{
			target_y_pos = 0;
			target_x_pos = src_x_pos + (0 - src_y_pos)/tanf(angle);
			angle = 2*M_PI-angle;
		}
		else
		{
			angle = 3*M_PI-angle;
		}
	}  
}


void BulletSprite::updateBullet(float dt)
{
	// Point OldPos = getPosition();

	Vec2 curPos(0,0);
	cocos2d::Rect rect(0,0,kResolutionWidth, kResolutionHeight);
	if(m_pLockFish != nullptr)
	{
		if (m_pLockFish->getFishStatue() != FISH_DIE && rect.containsPoint(m_pLockFish->getFishPosition()))
		{
			Vec2 pStart = m_startPos;
			Vec2 pEnd = m_pLockFish->getFishPosition();
			float currAngle = MathAide::CalcAngle(pStart.x, pStart.y, pEnd.x, pEnd.y);
			setBulletAngle(currAngle - kChairDefaultAngle[m_fireChairId]);
		}
		else
		{
			//当失去锁定鱼时，不立刻将锁定子弹设置为普通子弹
			/*setLockFishId(0);
			setLockFish(NULL);*/
		}
	}

	curPos.x= cos(m_bulletAngle)*(m_bulletSpeed*m_nPathCount) + m_ptSrc.x;
	curPos.y= sin(m_bulletAngle)*(m_bulletSpeed*m_nPathCount) + m_ptSrc.y;
	++m_nPathCount;
	
	if (rect.containsPoint(curPos))
	{
		this->setPosition(curPos);
		return ;
	}
	//当子弹到达屏幕边缘，锁定子弹，变为普通子弹
	setLockFishId(0);
	setLockFish(NULL);

	// 按4个分区处理
	GetBulletTargetPoint(kResolutionWidth, kResolutionHeight, m_ptSrc.x, m_ptSrc.y, m_bulletAngle, curPos.x, curPos.y);
	this->setPosition(curPos);
	m_ptSrc = curPos;
	setBulletAngle(m_bulletAngle);
	m_nPathCount = 1;
}

void BulletSprite::setBulletAngle(float angle)
{
	m_bulletAngle = angle;
	float degrees = 90-CC_RADIANS_TO_DEGREES(angle);  // 图片旋转的角度
	m_bulletBg->setRotation(degrees);
}

void BulletSprite::setbulletSpeed(float speed)
{
	m_bulletSpeed = speed;
}

void BulletSprite::removeBullet()
{
	m_bulletMulriple = 0;
	this->removeFromParentAndCleanup(true);
}

Vec2 BulletSprite::getBulletPosition()
{
	return getPosition();
}

void BulletSprite::setBulletId(int nId)
{
	m_nBulletId = nId;
}
int BulletSprite::getBulletId()
{
	return m_nBulletId;
}


void BulletSprite::runBezier(Vec2 ptCannon, Vec2 ptStartPos, Vec2 ptEndPos, float fDistance)
{
	//初始化
	float init_x_pos[3];
	float init_y_pos[3];
	int init_count = 0;

	init_x_pos[0] = ptCannon.x;
	init_y_pos[0] = ptCannon.y;

	init_x_pos[1] = ptStartPos.x;
	init_y_pos[1] = ptStartPos.y;

	init_x_pos[2] = ptEndPos.x;
	init_y_pos[2] = ptEndPos.y;
	m_nPathCount = 0;
	//调用函数
	MathAide::BuildBezierFast(init_x_pos, init_y_pos, 3, m_vecTraceBullet, fDistance);
}

int BulletSprite::ExchangeChairID(const int iChairID)
{
	if (iChairID == 0 || iChairID == 1 || iChairID == 2) return 0;
	if (iChairID == 3) return 2;
	if (iChairID == 4 || iChairID == 5 || iChairID == 6) return 3;
	if (iChairID == 7) return 5;
}

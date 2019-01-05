#include "CannonSprite.h"
#include "CMD_Fish.h"
#include "../../cocos/base/ccUTF8.h"
#include "ByMenuLayer.h"
#include "MyConfig.h"//by hxh

using namespace std;
using namespace NS_lrbyFish;

#define WINSIZE cocos2d::Size(kResolutionWidth,kResolutionHeight)
#define SCALE 


USING_NS_CC;

CannonNode::CannonNode(int nChairID):m_nChairID(nChairID),
	m_spCannon(nullptr),
	m_fireAnimate(nullptr),
	m_cannonStatue(-1),
	m_spVIPLevel(nullptr),
	m_nScoreNumber(0)
{
}

CannonNode::~CannonNode()
{
}

CannonNode* CannonNode::create(int nChairID)
{
	CannonNode *ret = new CannonNode(nChairID);
	if (ret && ret->init(nChairID))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}
bool CannonNode::init(int nChairID)
{
	if ( !Node::init() )
	{
		return false;
	}
	MyConfig& myConfig=MyConfig::Instance();
	cocos2d::Size winSize = cocos2d::Size(kResolutionWidth,kResolutionHeight);	

	auto m_spCannonDi = Sprite::create("709/fish_cannon/gun_bottom.png");
	m_spCannonDi->setPosition(ccp(-winSize.width * 0.025 + 35, winSize.height * 0.005 - 15));
	this->addChild(m_spCannonDi);

	m_spCannon = Sprite::createWithSpriteFrameName("cannon/cannon1_norm_01.png");
	m_spCannon->setPosition(ccp(winSize.width * 0.577, winSize.height * 0.09 - 2));
	this->addChild(m_spCannon, 1);
	
	//炮台倍率
	m_labelNum = LabelAtlas::create("0", "709/fish_cannon/num_user_fish_score.png", 15, 19, '0');
	//m_labelNum->setPosition(ccp(-winSize.width * 0.025 + 35, winSize.height * 0.005 - 26));
	m_labelNum->setPosition(ccp(myConfig.m_CannonNodePos[0][0],myConfig.m_CannonNodePos[0][1]));
	m_labelNum->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(m_labelNum, 2);
	
	//玩家分数
	m_labelFishScore = LabelAtlas::create("100", "709/fish_cannon/num_user_gold.png", 12, 17, '0');
	m_labelFishScore->setAnchorPoint(Vec2(0,0.5));
	//m_labelFishScore->setPosition(Vec2(m_spCannonDi->getContentSize().width / 4 * 3 - 6, 14));
	m_labelFishScore->setPosition(ccp(myConfig.m_CannonNodePos[1][0],myConfig.m_CannonNodePos[1][1]));
	m_spCannonDi->addChild(m_labelFishScore, 2);

	//玩家昵称
	m_labelNickName = LabelTTF::create("", "SimHei", 18);
	m_labelNickName->setColor(ccc3(232, 166, 26));
	m_labelNickName->setAnchorPoint(Vec2(0.5, 0.5));
	//m_labelNickName->setPosition(Vec2(m_spCannonDi->getContentSize().width / 4 - 32, 15));
	m_labelNickName->setPosition(ccp(myConfig.m_CannonNodePos[2][0],myConfig.m_CannonNodePos[2][1]));
	m_spCannonDi->addChild(m_labelNickName, 2);

	m_fireStatue = CANNON_NORMAL;
	m_nScoreNumber = 0;
	m_nCannonNumber = 0;

	return true;
}

void CannonNode::setCannonAngle(float fAngle)
{
	if(m_spCannon)
	{
		float fDegrees = CC_RADIANS_TO_DEGREES(M_PI_2-fAngle);  // 这里需要换算为图片旋转的角度
		//if (m_nChairID == 0)
		//{
		//	fDegrees+=180.0f;    //
		//}
		//if (m_nChairID == 1 || m_nChairID == 2 || m_nChairID == 3)
		//{
		//	fDegrees -= 90.0f;
		//}
		m_spCannon->setRotation(fDegrees);
		m_fCannonAngle = fAngle;
	}
}

float CannonNode::getCannonAngle()
{
	return m_fCannonAngle;
}
void CannonNode::setCannonNumber(int nNum)
{
	String str = StringUtils::toString(nNum);
	if(m_labelNum)
	{
		m_labelNum->setString(StringUtils::toString(nNum));
		m_nCannonNumber = nNum;
	}
}

void CannonNode::setScoreNumber(LONGLONG nNum)
{	
	if (nNum <=0)
	{
		nNum = 0;
	}
	
	String str = StringUtils::toString(nNum);

	if(m_labelFishScore)
	{		
		m_labelFishScore->setString(str.getCString());
		m_nScoreNumber = nNum;
	}
}

//切换炮台时更改炮台动画，包括开火中，和空闲中。
void CannonNode::setCannonStatue(int nStatue)
{
	if(nStatue == m_cannonStatue)
		return;
	if (nStatue<BULLET_KIND_1_NORMAL || nStatue>=BULLET_KIND_COUNT)
	{
		nStatue = BULLET_KIND_1_NORMAL;
	}
	cocos2d::Size sz = cocos2d::Size(kResolutionWidth,kResolutionHeight);
	m_cannonStatue = nStatue;
	if(m_spCannon != nullptr)
	{
		m_spCannon->removeFromParentAndCleanup(true);
		m_spCannon = nullptr;
	}
	if(m_fireAnimate != nullptr)
	{
		m_fireAnimate->release();
		m_fireAnimate= nullptr;
	}
	
	if(m_cannonStatue <= BULLET_KIND_4_NORMAL)
	{
		m_spCannon = Sprite::createWithSpriteFrameName(String::createWithFormat("cannon/cannon%d_norm_01.png", m_cannonStatue + 1)->getCString());		
		m_spCannon->setPosition(Vec2(sz.width * 0.0017, sz.height * 0.05 - 32));
		m_spCannon->setAnchorPoint(Vec2(0.5, 0.2));
		this->addChild(m_spCannon, 1);

		//开火动画
		Vector<SpriteFrame*> fireFrames(6);
		for(int i = 2; i <= 7; i ++)
		{
			char str[64] = {0};
			sprintf(str, "cannon/cannon%d_norm_%02d.png", m_cannonStatue + 1, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			fireFrames.pushBack(frame);
		}
		auto fireAnimation = Animation::createWithSpriteFrames(fireFrames, 0.1f);
		m_fireAnimate = Animate::create(fireAnimation);
		m_fireAnimate->retain();
	}
	else
	{
		m_spCannon = Sprite::createWithSpriteFrameName(String::createWithFormat("cannon/cannon%d_ion_01.png", m_cannonStatue - BULLET_KIND_4_NORMAL)->getCString());
 		m_spCannon->setPosition(Vec2(sz.width * 0.0017, sz.height * 0.05 - 32));
		m_spCannon->setAnchorPoint(Vec2(0.5, 0.2));
		this->addChild(m_spCannon, 1);

		//开火动画
		Vector<SpriteFrame*> fireFrames(6);
		for(int i = 2; i <= 7; i ++)
		{
			char str[64] = {0};
			sprintf(str, "cannon/cannon%d_ion_%02d.png", m_cannonStatue - BULLET_KIND_4_NORMAL, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			fireFrames.pushBack(frame);
		}
		auto fireAnimation = Animation::createWithSpriteFrames(fireFrames, 0.1f);
		m_fireAnimate = Animate::create(fireAnimation);
		m_fireAnimate->retain();
	}
}

int CannonNode::getCannonStatue()
{
	return m_cannonStatue;
}

int CannonNode::getCannonFireStatue()
{
	return m_fireStatue;
}
void CannonNode::cannonFire()
{
	if(m_spCannon != nullptr)
	{
		m_spCannon->stopAllActions();
		m_spCannon->runAction(m_fireAnimate);
		m_fireStatue = CANNON_FIRE;
	}
}

void CannonNode::setNickName(std::string strNickName)
{
	m_labelNickName->setString(strNickName);
}

void CannonNode::setVIPLevel(int nVIPLevel)
{
	////vip
	//if (m_spVIPLevel == nullptr)
	//{
	//	m_spVIPLevel = Sprite::create(StringUtils::format("gameCannon/Game_vip%d.png", nVIPLevel));
	//	m_spVIPLevel->setPosition(Vec2(WINSIZE.width * 0.080 - 345, WINSIZE.height * 0.005 - 20));
	//	this->addChild(m_spVIPLevel, 2);
	//}	
}

void CannonNode::setFace(int nFace)
{		
 //	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("face/face.plist","face/face.png");
	//cocos2d::Size winSize = cocos2d::Size(kResolutionWidth,kResolutionHeight);
	//m_spFace = Sprite::createWithSpriteFrameName(StringUtils::format("face_%d.png", nFace % 9));
	//m_spFace->setPosition(Vec2(WINSIZE.width * 0.080 - 660, WINSIZE.height * 0.005));
	//m_spFace->setScale(1.2f);
	//this->addChild(m_spFace, 2);
 }

void CannonNode::setBoard(int nChairID, DWORD dwTicket)
{

	//log("lance:m_nChairID = %d, nChairID = %d", m_nChairID, nChairID);

	if (m_nChairID == nChairID)
	{
		ByMenuLayer::GetInstance()->setTicketNumber(dwTicket);
	}
	else
	{
		//m_spCannonBg->setRotation(180);

		m_labelFishScore->setRotation(180);
		m_labelFishScore->setAnchorPoint(Vec2(1, 0.5));
		m_labelFishScore->setPosition(Vec2(345, -13));

		m_spVIPLevel->setRotation(180);
		m_spVIPLevel->setPosition(Vec2(-235, -13));

		//玩家昵称
		m_labelNickName->setRotation(180);
		m_labelNickName->setAnchorPoint(Vec2(1, 0.5));
		m_labelNickName->setPosition(Vec2(-480, -13));

		//头像
		m_spFace->setFlipX(180);
		m_spFace->setFlipY(180);
	}
	//m_spCannonBg->setScale(0.5);
	//m_spCannonBg->setPosition(ccp(0, 0));
}
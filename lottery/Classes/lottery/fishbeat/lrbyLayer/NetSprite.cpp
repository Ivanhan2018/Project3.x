#include "NetSprite.h"
#include "CMD_Fish.h"
#include "../../cocos/base/ccUTF8.h"
#include "BulletSprite.h"
#include "CocosUnits.h"

using namespace std;

USING_NS_CC;

NetSprite::NetSprite()
{
}

NetSprite::~NetSprite()
{
	if (m_pSeqAction!= nullptr)
	{
		m_pSeqAction->release();
	}
}

NetSprite * NetSprite::create(int nNetStatue)
{
	NetSprite *net = new NetSprite();
	if (net && net->init(nNetStatue))
	{
		net->autorelease();
		return net;
	}
	CC_SAFE_DELETE(net);
	return nullptr;
}

bool NetSprite::init(int nNetStatue)
{
	if (!Sprite::init())
	{
		return false;
	}

	m_spNet = nullptr;
	m_pSeqAction = nullptr;
	m_live = false;

	if((nNetStatue < NET_2_NORMAL) || (nNetStatue > NET_4_NORMAL))
	{
		nNetStatue = NET_2_NORMAL;
	}

	m_NetStatue = nNetStatue;

	if (m_spNet == nullptr)
	{
		std::string str = String::createWithFormat("net/net%d_000.png",m_NetStatue)->getCString();
		m_spNet = Sprite::createWithSpriteFrameName(str);		
		m_spNet->setScale(0.9f);
		this->addChild(m_spNet, 200);		
	}
	if (m_pSeqAction == nullptr)
	{
		Vector<SpriteFrame*> netFrames(7);
		for(int i = 0; i < 7; i ++)
		{
			char buff[64] = {0};
			sprintf(buff, "net/net%d_%03d.png", m_NetStatue, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buff);
			netFrames.pushBack(frame);
		}
		auto netAnimation = Animation::createWithSpriteFrames(netFrames, 0.1f);
		m_pSeqAction= Sequence::create(Animate::create(netAnimation), CallFunc::create(this, callfunc_selector(NetSprite::removeMySelf)), nullptr);
		m_pSeqAction->retain();
	}

	return true;
}

void NetSprite::removeMySelf()
{	
	m_spNet->stopAllActions();
	this->removeFromParent();
	m_live = false;
}

void NetSprite::run( Vec2 pt, int nNetStatue )
{
	if((nNetStatue < NET_2_NORMAL) || (nNetStatue > NET_4_NORMAL))
	{
		nNetStatue = NET_2_NORMAL;
	}

	m_spNet->setPosition(pt);
	m_spNet->runAction(m_pSeqAction);
}

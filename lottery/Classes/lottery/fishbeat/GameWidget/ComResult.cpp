#include "ComResult.h"
#include "Sound.h"

USING_NS_CC;
using namespace std;
using namespace ui;


ComResult::ComResult(void)
{
	
}


ComResult::~ComResult(void)
{
	_eventDispatcher->removeEventListener(_touchListener);
}


bool ComResult::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ComResult::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(ComResult::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;

	return true;
}


bool ComResult::onTouchBegan(Touch *touch, Event *unused_event)
{

	return true;
}


void ComResult::onTouchEnded(Touch *touch, Event *unused_event)
{
	//this->removeFromParentAndCleanup(true);
}


void ComResult::setResult(bool bwin,int repeatTimes, int delayTime)
{
	auto winSize = Director::getInstance()->getWinSize();
	std::string szName(bwin ? "ComLayer/ComResult/pass_kill_1.png" : "ComLayer/ComResult/pass_lost_1.png");
	auto sprite = Sprite::create(szName);
	sprite->setPosition(winSize / 2);
	this->addChild(sprite);

	auto animation = Animation::create();
	for (int j = 0; j < 2; j++)
	{
		char tmp[50] = {0};
		sprintf(tmp, bwin ? "ComLayer/ComResult/pass_kill_%d.png" : "ComLayer/ComResult/pass_lost_%d.png", j+1);
		animation->addSpriteFrameWithFile(tmp);
	}
	animation->setDelayPerUnit(0.5f / 2.0f);
	auto action = Animate::create(animation);
	if (delayTime == -1)
	{
		//Sound::getInstance()->playEffect(bwin ? EFFECT_BLACK_JACK_TYPE::EFFECT_BJ_TYPE_ALL_WIN : EFFECT_BLACK_JACK_TYPE::EFFECT_BJ_TYPE_ALL_LOSE);
		sprite->runAction(Repeat::create(action,repeatTimes));
	}
	else
	{
		sprite->runAction(Sequence::create(DelayTime::create(delayTime),
			CallFunc::create([=]{sprite->setVisible(true);
		//Sound::getInstance()->playEffect(bwin ? EFFECT_BLACK_JACK_TYPE::EFFECT_BJ_TYPE_ALL_WIN : EFFECT_BLACK_JACK_TYPE::EFFECT_BJ_TYPE_ALL_LOSE);
		}), Repeat::create(action,repeatTimes), nullptr));
	}
}

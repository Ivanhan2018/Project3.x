#include "EffectScence.h"

#include "GameLib.h"

USING_NS_CC;

FV_SINGLETON_STORAGE(EffectScence);


EffectScence::EffectScence()
{
	init();
	WidgetScenceXMLparse xml("Script/GoldEffect.xml",this);
}
EffectScence::~EffectScence()
{

}
bool EffectScence::init()
{
	if (!Node::init())
	{
		return false;
	};
	return true;
}

void EffectScence::ShowGoldEffect()
{
	WidgetManager::Instance().createWidget("GoldEffect",this);
	//SoundFun::playEffect("Animation_CoinAudio.mp3");
}

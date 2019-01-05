#include "Sound.h"
#include "cocos2d.h"


USING_NS_CC;
using namespace CocosDenshion;

Sound * Sound::m_sound = NULL;
Sound * Sound::getInstance()
{
	if(m_sound == NULL)
	{
		m_sound = new Sound();
	}
	return m_sound;
}
void Sound::freeInstance()
{
	if(m_sound != NULL)
	{
		delete m_sound;
		m_sound = NULL;
	}
}

Sound::Sound()
{

	////预先加载声音，参数是声音文件
	//auto audio = SimpleAudioEngine::getInstance();
	////预先加载音乐的函数在win32下是空的实现 播放的效果是滞后的 在android上没有问题
	//audio->preloadBackgroundMusic(getMusic(MUSIC_TYPE_BACKGROUND1));
	//audio->preloadBackgroundMusic(getMusic(MUSIC_TYPE_BACKGROUND2));
	//audio->preloadBackgroundMusic(getMusic(MUSIC_LANDTYPE_BACKGROUND));
	////加载音效 在win32下加载音效没有什么效果，播放出来是滞后的，但是在android上没有滞后的问题
	//for (int i = EFFECT_TYPE_BINGO; i < EFFECT_TYPE_Unknow; ++ i)
	//{
	//	audio->preloadEffect(getEffect((EFFECT_TYPE)(i)));
	//}
	//
	//for (int i = EFFECT_LANDTYPE_BEGIN; i < EFFECT_LANDTYPE_Unknow; i++)
	//{
	//	audio->preloadEffect(getEffect((EFFECT_LANDTYPE)(i)));
	//}
	//// 设置声音的大小0.0-1.0 在win32下也是空的实现
	//float fVolume = UserDefault::getInstance()->getFloatForKey("BgMusicVol", 0.9f);
	//audio->setBackgroundMusicVolume(fVolume);
	//fVolume = UserDefault::getInstance()->getFloatForKey("EffectsVol", 0.9f);
	//audio->setEffectsVolume(fVolume);
}

//获取音效和音乐的文件
const char * Sound::getMusic(MUSIC_TYPE type)
{
	switch(type)
	{
	case MUSIC_TYPE_SceneBg:
		return "sounds/SceneBg.mp3";
		break;
	case MUSIC_TYPE_BACKGROUND1:
		return "sounds/bgm/bgm1.mp3";
		break;
	case MUSIC_TYPE_BACKGROUND2:
		return "sounds/bgm/bgm2.mp3";
		break;
		case MUSIC_TYPE_BACKGROUND3:
			return "sounds/bgm/bgm3.mp3";
			break;
		case MUSIC_TYPE_BACKGROUND4:
			return "sounds/bgm/bgm4.mp3";
			break;
	default:
		break;
	}
	return NULL;
}

void Sound::playBgMusic(MUSIC_TYPE type)
{
 	bool bMusic = getMusicStatue();
	if(bMusic)
	{
		auto audio = SimpleAudioEngine::getInstance();
		if(!audio->isBackgroundMusicPlaying())
		{
			audio->playBackgroundMusic(this->getMusic(type),true);
		}
		else
		{
			audio->resumeBackgroundMusic();
		}
	}
}

const char * Sound::getEffect(EFFECT_FOUR_FISH_TYPE type)
{
	switch (type)
	{
	case EFFECT_TYPE_BINGO: return "sounds/effect/bingo.mp3";
	case EFFECT_TYPE_CANNON_SWITCH: return "sounds/effect/changecannon.mp3";
	case EFFECT_TYPE_SILVER: return "sounds/effect/silver.mp3";
	case EFFECT_TYPE_CATCH: return "sounds/effect/catch.mp3";
	case EFFECT_TYPE_FIRE: return "sounds/effect/fire.mp3";
	case EFFECT_TYPE_FISH10_1: return "sounds/effect/fish10_1.mp3";
	case EFFECT_TYPE_FISH10_2: return "sounds/effect/fish10_2.mp3";
	case EFFECT_TYPE_FISH11_1: return "sounds/effect/fish11_1.mp3";
	case EFFECT_TYPE_FISH11_2: return "sounds/effect/fish11_2.mp3";
	case EFFECT_TYPE_FISH12_1: return "sounds/effect/fish12_1.mp3";
	case EFFECT_TYPE_FISH12_2: return "sounds/effect/fish12_2.mp3";
	case EFFECT_TYPE_FISH13_1: return "sounds/effect/fish13_1.mp3";
	case EFFECT_TYPE_FISH13_2: return "sounds/effect/fish13_2.mp3";
	case EFFECT_TYPE_FISH14_1: return "sounds/effect/fish14_1.mp3";
	case EFFECT_TYPE_FISH14_2: return "sounds/effect/fish14_2.mp3";
	case EFFECT_TYPE_FISH15_1: return "sounds/effect/fish15_1.mp3";
	case EFFECT_TYPE_FISH15_2: return "sounds/effect/fish15_2.mp3";
	case EFFECT_TYPE_FISH16_1: return "sounds/effect/fish16_1.mp3";
	case EFFECT_TYPE_FISH16_2: return "sounds/effect/fish16_2.mp3";
	case EFFECT_TYPE_FISH17_1: return "sounds/effect/fish17_1.mp3";
	case EFFECT_TYPE_FISH17_2: return "sounds/effect/fish17_2.mp3";
	case EFFECT_TYPE_GOLD: return "sounds/effect/gold.mp3";
	case EFFECT_TYPE_INSERT: return "sounds/effect/insert.mp3";
	case EFFECT_TYPE_ION_CATCH: return "sounds/effect/ion_catch.mp3";
	case EFFECT_TYPE_ION_FIRE: return "sounds/effect/ion_fire.mp3";
	case EFFECT_TYPE_LOCK: return "sounds/effect/lock.mp3";
	case EFFECT_TYPE_WAVE: return "sounds/effect/wave.mp3";
	case EFFECT_TYPE_coinsky: return "sounds/effect/coinsHuahua.mp3";
	case EFFECT_TYPE_Unknow:
		break;
	default:
		break;
	}
	return nullptr;
}

void Sound::playEffect(EFFECT_FOUR_FISH_TYPE type)
{
	bool bEffect = getEffectStatue();
	if(bEffect)
	{
		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect(this->getEffect(type));
	}
}


void Sound::setBgMusicVolume(float volume) //设置背景音乐音量
{
	UserDefault::getInstance()->setFloatForKey("BgMusicVol", volume);
	auto audio = SimpleAudioEngine::getInstance();
	audio->setBackgroundMusicVolume(volume);
}

float Sound::getBgMusicVolume()             //获取背景音乐音量
{
	float fVolume = UserDefault::getInstance()->getFloatForKey("BgMusicVol", 0.9f);
	return fVolume;
}

void Sound::setEffectsVolume(float volume) //设置音效音量
{
	UserDefault::getInstance()->setFloatForKey("EffectsVol", volume);
	auto audio = SimpleAudioEngine::getInstance();
	audio->setEffectsVolume(volume);
}

float  Sound::getEffectsVolume() //获取音效音量
{
	float fVolume = UserDefault::getInstance()->getFloatForKey("EffectsVol", 0.9f);
	return fVolume;
}

bool Sound::getEffectStatue()
{
	bool bEffect = UserDefault::getInstance()->getBoolForKey("MusicEffect", true);
	return bEffect;
}

bool Sound::getMusicStatue()
{
	bool bMusic = UserDefault::getInstance()->getBoolForKey("Music", true);
	return bMusic;
}

void Sound::StopEffect()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

void Sound::StopMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

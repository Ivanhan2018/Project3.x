#include "DDZSoundControl.h"
static DDZSoundControl * s_SharedDDZSoundControl = NULL;

DDZSoundControl::DDZSoundControl()
{
	m_musicVol = UserDefault::getInstance()->getFloatForKey("ZJD_SOUNDNUM",0.5f);
	m_soundEffVol = UserDefault::getInstance()->getFloatForKey("SOUNDEFFVOL",0.5f);
	m_voiceIsOpen = UserDefault::getInstance()->getBoolForKey("ZJD_SOUNDSTATE",false);
}

DDZSoundControl::~DDZSoundControl()
{
}

DDZSoundControl * DDZSoundControl::sharedDDZSoundControl()
{
	if (!s_SharedDDZSoundControl)
	{
		s_SharedDDZSoundControl = new DDZSoundControl();
	}
	return s_SharedDDZSoundControl;
}

void DDZSoundControl::playBackGroundMusic(const char * soundName,bool loop)
{
	if (m_voiceIsOpen)
	{
		return;
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic(soundName,loop);
}

void DDZSoundControl::playSoundEffect(const char * soundName)
{
	if (m_voiceIsOpen)
	{
		return;
	}
	SimpleAudioEngine::getInstance()->playEffect(soundName);
}

void DDZSoundControl::setVoiceIsOpen(bool var)
{
	m_voiceIsOpen = var;
	UserDefault::getInstance()->setBoolForKey("ZJD_SOUNDSTATE",m_voiceIsOpen);
	if (!m_voiceIsOpen)
	{
		playBackGroundMusic(DDZsound_name::s_bgMusic,true);
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}
}

void DDZSoundControl::setSoundEffVol(float vol)
{
	m_soundEffVol = UserDefault::getInstance()->getFloatForKey("SOUNDEFFVOL",0.5f);
	if (vol!=-1)
	{
		m_soundEffVol = vol;
	}
	SimpleAudioEngine::getInstance()->setEffectsVolume(m_soundEffVol);
 	UserDefault::getInstance()->setFloatForKey("SOUNDEFFVOL",m_soundEffVol);
 	UserDefault::getInstance()->flush();
}

void DDZSoundControl::setMusicVol(float vol)
{
	m_musicVol = UserDefault::getInstance()->getFloatForKey("ZJD_SOUNDNUM",0.5f);
	if (vol!=-1)
	{
		m_musicVol = vol;
	}
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_musicVol);
	UserDefault::getInstance()->setFloatForKey("ZJD_SOUNDNUM",m_musicVol);
	UserDefault::getInstance()->flush();
}

void DDZSoundControl::coloseBackGroundMusic()
{
	if(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
}
static DDZSoundEffect * s_sharedSoundEffect = NULL;

DDZSoundEffect::DDZSoundEffect()
{
	m_leftUser.userId = 0;
	m_leftUser.gender = 1;
	m_self.userId = 0;
	m_self.gender = 1;
	m_rightUser.userId = 0;
	m_rightUser.gender = 1;
}

DDZSoundEffect::~DDZSoundEffect()
{

}

DDZSoundEffect * DDZSoundEffect::sharedSoundEffect()
{
	if (!s_sharedSoundEffect)
	{
		s_sharedSoundEffect = new DDZSoundEffect();
	}
	return s_sharedSoundEffect;
}

void DDZSoundEffect::initLeftTag(DDZTag left)
{
	m_leftUser = left;

}

void DDZSoundEffect::initRightTag(DDZTag right)
{
	m_rightUser = right;
}

void DDZSoundEffect::initSelfTag(DDZTag self)
{
	m_self = self;
}

void DDZSoundEffect::playEffect(const char * soundName,int userId)
{
	int gender = 0;
	switch (userId)
	{
	case 0://上家
		gender = m_leftUser.gender;
		break;
	case 1://自己
		gender = m_self.gender;
		break;
	case 2://下家
		gender = m_rightUser.gender;
		break;
	}
	CCString str;
	if (gender==1)//1表示男
	{
		str.initWithFormat("%s",soundName);
	}else
	{
		str.initWithFormat("wm_%s",soundName);
	}
	DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(str.getCString());

}

void DDZSoundEffect::playPassEffect(int userId)
{
	int i=rand()%3;
	switch(i)
	{
	case 0:
		playEffect(DDZsound_name::s_buyao,userId);
		break;
	case 1:
		playEffect(DDZsound_name::s_guo,userId);
		break;
	case 2:
		playEffect(DDZsound_name::s_yaobuqi,userId);
		break;
	}
}

void DDZSoundEffect::playDaPaiEffect(int userId)
{
	switch(rand()%2)
	{
	case 0:
		playEffect(DDZsound_name::s_yasi,userId);
		break;
	case 1:
		playEffect(DDZsound_name::s_dani,userId);
		break;
	}
}
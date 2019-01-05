#include "SoundControl.h"
#ifdef DZZ
#include "DDZSoundControl.h"
#endif // DZZ

static SoundControl * s_SharedSoundControl = NULL;

SoundControl::SoundControl()
{
	m_musicVol = UserDefault::getInstance()->getFloatForKey("ZJD_SOUNDNUM",0.5f);
	m_soundEffVol = UserDefault::getInstance()->getFloatForKey("SOUNDEFFVOL",0.5f);
	m_voiceIsOpen = UserDefault::getInstance()->getBoolForKey("ZJD_SOUNDSTATE",false);
}

SoundControl::~SoundControl()
{
}

SoundControl * SoundControl::sharedSoundControl()
{
	if (!s_SharedSoundControl)
	{
		s_SharedSoundControl = new SoundControl();
	}
	return s_SharedSoundControl;
}

void SoundControl::playBackGroundMusic(const char * soundName,bool loop)
{
	if (m_voiceIsOpen)
	{
		return;
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic(soundName,loop);
}

void SoundControl::playSoundEffect(const char * soundName)
{
	if (m_voiceIsOpen)
	{
		return;
	}
	SimpleAudioEngine::getInstance()->playEffect(soundName);
}

void SoundControl::setVoiceIsOpen(bool var)
{
	m_voiceIsOpen = var;
	UserDefault::getInstance()->setBoolForKey("ZJD_SOUNDSTATE",m_voiceIsOpen);
	if (!m_voiceIsOpen)
	{
		playBackGroundMusic(sound_name::s_bgMusic,true);
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}
}

void SoundControl::setSoundEffVol(float vol)
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

void SoundControl::setMusicVol(float vol)
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

void SoundControl::coloseBackGroundMusic()
{
	if(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
}
static SoundEffect * s_sharedSoundEffect = NULL;

SoundEffect::SoundEffect()
{
	m_leftUser.userId = 0;
	m_leftUser.gender = 1;
	m_self.userId = 0;
	m_self.gender = 1;
	m_rightUser.userId = 0;
	m_rightUser.gender = 1;
}

SoundEffect::~SoundEffect()
{

}

SoundEffect * SoundEffect::sharedSoundEffect()
{
	if (!s_sharedSoundEffect)
	{
		s_sharedSoundEffect = new SoundEffect();
	}
	return s_sharedSoundEffect;
}

void SoundEffect::initLeftTag(Tag left)
{
	m_leftUser = left;

}

void SoundEffect::initRightTag(Tag right)
{
	m_rightUser = right;
}

void SoundEffect::initSelfTag(Tag self)
{
	m_self = self;
}

void SoundEffect::playEffect(const char * soundName,int userId)
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
	String str;
	if (gender==1)//1表示男
	{
		str.initWithFormat("%s",soundName);
	}else
	{
		str.initWithFormat("wm_%s",soundName);
	}
	SoundControl::sharedSoundControl()->playSoundEffect(str.getCString());

}

void SoundEffect::playPassEffect(int userId)
{
#ifdef DZZ
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
#endif
}

void SoundEffect::playDaPaiEffect(int userId)
{
#ifdef DZZ
	switch(rand()%2)
	{
	case 0:
		playEffect(DDZsound_name::s_yasi,userId);
		break;
	case 1:
		playEffect(DDZsound_name::s_dani,userId);
		break;
	}
#endif
}
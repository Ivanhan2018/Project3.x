
#ifndef __SoundFun_FUN_H__
#define __SoundFun_FUN_H__

#include "cocos2d.h"
#include "TimeNode.h"
#include "FvSingleton.h"

class SoundFun
	:public TimeNode
	,public FvSingleton<SoundFun>
{
public:
	SoundFun();

	void playBackMusic(std::string kName);
	void stopBackMusic();
	void playEffect(std::string kName,bool bPreload = false);
	void playEffectDirect(std::string kName,bool bPreload = false);
	void playCardByGender(int iGender,std::string kName,bool bPreload = false);
	void PreloadEffectByGender(std::string kName);
	void upDatePlayerEffect();
	void SetSoundBackground(float fValue);
	void SetSoundEffect(float fValue);
	void setMute(bool bMute);
	void playCustomEffect(std::string kName);

	void PaseBackMusic();
	void OnResumeBackMusic();
	void ResumeBackMusic(float fTime = 0.f);

	void PaseEffectMusic();
	void OnResumeEffectMusic();
	void ResumeEffectMusic(float fTime = 0.f);

	void setPlayButton(bool bNeed);
	void playButtonEffect();
private:
	std::string s_soundPath;
	std::string m_kBackSound;
	bool s_bMute;
	float s_fSound1;
	float s_fSound2;

	bool m_bPlayButton;
};

#endif // 

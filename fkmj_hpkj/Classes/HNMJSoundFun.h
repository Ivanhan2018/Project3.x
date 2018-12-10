#pragma once

#include "cocos2d.h"

namespace HNMJSoundFun
{
	const static int EFFECT_NUM     = 3;

	const static int AN_GANG_NUM_W    = 2;

	const static std::string RAW_KIND= ".mp3";

	const static std::string EFFECT_HU		 = "hu";
	const static std::string EFFECT_PENG	 = "peng";
	const static std::string EFFECT_GANG	 = "gang";
	const static std::string EFFECT_BU		= "buzhang";
	const static std::string EFFECT_CHI		 = "chi";
	const static std::string EFFECT_ZIMO     = "zimo";
	

	void setPuTong(bool bPuTong);
	void playBackMusic(std::string kName);
	void stopBackMusic();
	void playEffect(std::string kName,bool bPreload = false);
	std::string getDefineSound(int iGender,std::string kStr);
	void playCardByGender(int iGender,int nCard);
	void playEffectByKind(int iGender,std::string kName);
	void playEffectEX(std::string kName);
	std::string getPath();
} 



#ifndef __XZDDSoundFun_FUN_H__
#define __XZDDSoundFun_FUN_H__

#include "cocos2d.h"

namespace XZDDSoundFun
{
	const static int EFFECT_NUM     = 3;

	const static int AN_GANG_NUM_W    = 2;

	const static std::string RAW_KIND= ".mp3";

	const static std::string EFFECT_HU		 = "hu";
	const static std::string EFFECT_PENG	 = "peng";
	const static std::string EFFECT_GANG	 = "gang";
	const static std::string EFFECT_AN_GANG  = "anGang";
	const static std::string EFFECT_ZIMO     = "zimo_other";
	


	const static std::string EX_CHI		 = "EX_chi.mp3";
	const static std::string EX_DAHU	     = "EX_dahu.mp3";
	const static std::string EX_FANGPAO	     = "EX_fangpao.mp3";
	const static std::string EX_GANG        = "EX_gang.mp3";
	const static std::string EX_LIUJU		 = "EX_liuju.mp3";
	const static std::string EX_PENG	     = "EX_peng.mp3";
	const static std::string EX_ZIMO	     = "EX_zimo.mp3";

	const static std::string EX_BACKGRUAND = "backMusic.mp3";

	void setPuTong(bool bPuTong);
	void playBackMusic(std::string kName);
	void stopBackMusic();
	void playEffect(std::string kName,bool bPreload = false);
	void playCardByGender(int iGender,int nCard);
	void playEffectByGender(int iGender,std::string kName);
	void playEffectEX(std::string kName);
	std::string getDefineSound(int iGender,std::string kStr);
} 

#endif // 

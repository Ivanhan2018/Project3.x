#include "HNMJSoundFun.h"
#include "SimpleAudioEngine.h"
#include "utility.h"
#include "SoundFun.h"
#include "HNMJGameLogic.h"
namespace HNMJSoundFun
{

	static std::string LG_PATH_HN = "GameHNMJ/sound/";
	static std::string LG_FLAG_HN = "putong/";

	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	void setPuTong(bool bPuTong)
	{
		if (bPuTong)
		{
			LG_FLAG_HN = "putong/";
		}
		else
		{
			LG_FLAG_HN = "changsha/";
		}
	}
	void playBackMusic(std::string kName)
	{
		kName = utility::toString(LG_PATH_HN,kName);
		SoundFun::Instance().playBackMusic(kName);
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::toString(LG_PATH_HN,LG_FLAG_HN,kName,RAW_KIND);

		SoundFun::Instance().playEffect(kName,bPreload);
	}

	std::string getDefineSound(int iGender,std::string kStr)
	{
		std::string strValue = utility::toString("fix_msg_",kStr);
		if (iGender)
		{
			strValue = utility::toString("man/",strValue);
		}
		else
		{
			strValue =utility::toString("woman/",strValue);
		}
		return utility::toString(LG_PATH_HN,LG_FLAG_HN,strValue,RAW_KIND);
	}

	void playCardByGender(int iGender,int nCard)
	{
		int nColor = HNMJ::CGameLogic::Instance().GetCardColor(nCard);
		int nValue = HNMJ::CGameLogic::Instance().GetCardValue(nCard);
		std::string strValue = utility::toString(nValue);
		std::string strColor;
		if (nColor == CARD_COLOR_WAN)
		{
			strColor="mjt1_";
		}
		else if (nColor == CARD_COLOR_TONG)
		{
			strColor="mjt2_";
		}
		else if (nColor == CARD_COLOR_TIAO)
		{
			strColor="mjt3_";
		}
		else
		{
			strColor = "mjt4_";
		}
		std::string kName = utility::toString(strColor,strValue);
		if (iGender)
		{
			kName = utility::toString("man/",kName);
		}
		else
		{
			kName =utility::toString("woman/",kName);
		}
		playEffect(kName);
	}

	void playEffectByKind(int iGender,std::string kName)
	{
		if (kName == EFFECT_HU)
		{
			kName = utility::toString("hu",rand()%3+1);
		}
		if (kName == EFFECT_GANG)
		{
			kName = utility::toString("gang",rand()%3+1);
		}
		if (kName == EFFECT_PENG)
		{
			kName = utility::toString("peng",rand()%4+1);
		}
		if (kName == EFFECT_CHI)
		{
			kName = utility::toString("chi",rand()%4+1);
		}
		if (kName == EFFECT_ZIMO)
		{
			kName = utility::toString("zimo",rand()%2+1);
		}
		if (iGender)
		{
			kName =utility::toString("man/",kName);
		}
		else
		{
			kName =utility::toString("woman/",kName);
		}

		playEffect(kName);
	}

	void playEffectEX(std::string kName)
	{
		kName = utility::toString(LG_PATH_HN,kName);
		SoundFun::Instance().playEffect(kName);
	}
	std::string getPath()
	{
		return LG_PATH_HN;
	}

} 

#include "NCMJSoundFun.h"
#include "SimpleAudioEngine.h"
#include "utility.h"
#include "SoundFun.h"
#include "NCMJGameLogic.h"
namespace NCMJSoundFun
{

	static const std::string LG_FLAG_NC = "GameNCMJ/sound/putong/";

	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	
	void playBackMusic(std::string kName)
	{
		SoundFun::Instance().playBackMusic((kName));
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::a_u8(LG_FLAG_NC + kName + RAW_KIND);

		SoundFun::Instance().playEffect(kName,bPreload);
		cocos2d::log("NCMJSoundFun:%s",kName.c_str());
	}

	void playCardByGender(int iGender,int nCard)
	{
		int nColor = NCMJ::CGameLogic::Instance().GetCardColor(nCard);
		int nValue = NCMJ::CGameLogic::Instance().GetCardValue(nCard);

		std::string strValue = utility::toString(nValue);
		std::string strColor;
		if (nColor == CARD_COLOR_WAN)
		{
			strColor="wan";
		}
		else if (nColor == CARD_COLOR_TONG)
		{
			strColor="tong";
		}
		else if (nColor == CARD_COLOR_TIAO)
		{
			strColor="tiao";
		}
		else if (nColor == CARD_COLOR_ZIPAI)
		{
			strColor="zipai";
		}
		else
		{
			strValue = "";
			strColor = "zhong";
		}
		std::string kName = utility::toString(strValue,strColor);
		if (iGender)
		{

			kName = utility::toString("g_",kName);
		}
		playEffect(kName);
	}

	void playEffectByGender(int iGender,std::string kName)
	{
		if (iGender)
		{
			kName =utility::toString("g_",kName);
		}
		else
		{
			kName =utility::toString("",kName);
		}

		playEffect(kName);
	}

	void playEffectEX(std::string kName)
	{
		SoundFun::Instance().playEffect(kName);
	}

	std::string getDefineSound(int iGender,std::string kStr)
	{
		std::string strValue = utility::toString("fix_msg_",kStr);
		strValue = utility::toString("man/",strValue);
		return utility::toString(LG_FLAG_NC,strValue,RAW_KIND);
	}
} 

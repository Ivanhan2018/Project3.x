#include "NNSoundFun.h"
#include "SimpleAudioEngine.h"
#include "utility.h"
#include "SoundFun.h"
#include "NNGameLogic.h"
namespace NNSoundFun
{
	static std::string s_kSoundPath = "GameNN/";
	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	
	void playBackMusic(std::string kName)
	{
		SoundFun::Instance().playBackMusic(utility::toString(s_kSoundPath,kName));
		cocos2d::log("NCMJSoundFun playBackMusic :%s",utility::a_u8(kName).c_str());
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::toString(s_kSoundPath,kName);

		SoundFun::Instance().playEffect(kName,bPreload);
	}

	void playCardByGender(int iGender,int nCard)
	{
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

} 

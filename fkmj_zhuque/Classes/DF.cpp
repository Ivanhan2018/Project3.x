#include "DF.h"
#include "MD5.h"
#include "cocos2d.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////

DF* DF::shared() 
{
	static DF sInstance;
	return &sInstance;
}

const char* DF::MD5Encrypt(const char* pszSourceData)
{
	static std::string sPassword;
	sPassword = md5(pszSourceData);
	return sPassword.c_str();
}
//////////////////////////////////////////////////////////////////////////

DF::DF()
	:mClientVersion(0)
	,mKindID(0)
	,mGamePlayers(0)
{
}

DF::~DF()
{
}

void DF::init(int iKindID, int iPlayers, int iClientVersion, const char* sGameName)
{
	mKindID = iKindID;
	mGamePlayers = iPlayers;
	mClientVersion = iClientVersion;
	mGameName = sGameName;
}

byte DF::GetDeviceType()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return DEVICE_TYPE_IPHONE;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return DEVICE_TYPE_ANDROID;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return DEVICE_TYPE_PC;
#endif
	
}

const char* DF::GetMachineID()
{
	static bool bFirst = true;
	static char szMachineID[LEN_MACHINE_ID]={0};


	return szMachineID;
}

const char* DF::GetMobilePhone()
{
	static bool bFirst = true;
	static char szPhone[LEN_MACHINE_ID]={0};

	if (bFirst)
	{
		bFirst=false;

		switch (GetDeviceType())
		{
		case DEVICE_TYPE_PC:
			{
				break;
			}
		case DEVICE_TYPE_ANDROID:
			{
				break;
			}
		}
	}

	return szPhone;
}



uint32 DF::GetPlazaVersion()
{

	return PROCESS_VERSION(10,0,3);//VERSION_PLAZA;
}

uint32 DF::GetFrameVersion()
{
	return PROCESS_VERSION(6,0,3);//VERSION_FRAME;;
}

uint16 DF::GetPlazzKindID()
{
	return GetGameKindID();
}


uint32 DF::GetGameVersion()
{
	return mClientVersion;
}

uint16 DF::GetGameKindID()
{
	return mKindID;
}
uint32 DF::GetGamePlayer()
{
	return mGamePlayers;
}

const char* DF::GetGameName(char szGameName[LEN_KIND])
{
	const char* sGameName = mGameName.c_str();
	strncpy(szGameName, sGameName, LEN_KIND);
	return sGameName;
}
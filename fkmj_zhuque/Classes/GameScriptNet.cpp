#include "GameScriptNet.h"
#include "GameLib.h"
#include "JniFun.h"

FV_SINGLETON_STORAGEBASE(GameScriptNet);

GameScriptNet::GameScriptNet()
	:m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	m_kPublicNoticMission.setMissionSink(this);
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(GameScriptNet::LogonSucess));
}
GameScriptNet::~GameScriptNet()
{

}
void GameScriptNet::LogonSucess()
{
	m_kPublicNoticMission.PublicNotice("GameScriptNotic");

}
void GameScriptNet::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	if (kKey == "GameScriptNotic" && iError == 0)
	{
		std::string kNetStr = utility::a_u8(szDescription);
		std::vector<std::string> kKeyValue = utility::split(kNetStr,"&");
		for (int i = 0;i<(int)kKeyValue.size();i++)
		{
			std::vector<std::string> kTempValue = utility::split(kKeyValue[i],"=");
			if (kTempValue.size() == 2)
			{
				ScriptDataManager::setValue(kTempValue[0],kTempValue[1]);
			}
		}
		OnGetConfig();
	}
}
void GameScriptNet::OnGetConfig()
{
	std::string kClientVersion = utility::getScriptString("ClientVersion",false);
	std::string kServerVersion = utility::getScriptString("ServerVersion",false);
	if (kServerVersion != "" && kClientVersion != kServerVersion)
	{
		std::string kDownUrl = utility::getScriptString("DownUrl",false);
		JniFun::showWebView(kDownUrl.c_str());
	}
}
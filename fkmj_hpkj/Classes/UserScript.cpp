#include "UserScript.h"
#include "GameLib.h"

FV_SINGLETON_STORAGE(UserScript);

UserScript::UserScript()
	:m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	m_kPublicNoticMission.setMissionSink(this);
}
UserScript::~UserScript()
{

}
void UserScript::getUserDefine(std::string kKey)
{
	m_kPublicNoticMission.PublicNotice(kKey);
}
void UserScript::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	if (iError)
	{
		utility::log(utility::toString("UserScript::onGPPublicNoticeResult:",kKey).c_str());
	}
	else
	{
		ScriptDataManager::setValue(kKey,std::string(szDescription));
	}
}
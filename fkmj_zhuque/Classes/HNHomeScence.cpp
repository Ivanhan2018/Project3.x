#include "HNHomeScence.h"
#include "HNScenceManager.h"
#include "GameLib.h"
#include "JniFun.h"

FV_SINGLETON_STORAGE(HNHomeScence);

HNHomeScence::HNHomeScence()
	:m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_kFaceMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_kIndividualMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_iGameLevel(0)
{
	init();
	CServerListData::shared()->SetServerListDataSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(HNHomeScence::upPlayerInfo));
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(HNHomeScence::LogonSucess));
	m_kPublicNoticMission.setMissionSink(this);
	m_kIndividualMission.setMissionSink(this);

}
HNHomeScence::~HNHomeScence()
{

}
bool HNHomeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNHomeScence.xml",this);

	init_Button();

	return true;
}
void HNHomeScence::EnterScence()
{
	WidgetFun::setVisible(this,"HomeRootPlane",true);
	WidgetFun::setVisible(this,"HomePublicPlane",false);
	upPlayerInfo();
	SoundFun::Instance().playBackMusic("raw/backMusic.mp3");
}
void HNHomeScence::upPlayerInfo()
{
	utility::log("SCHomeScence::upPlayerInfo()");
	WidgetFun::setText(this,"HomeID",utility::paseInt(UserInfo::Instance().getUserID(),6));
	WidgetFun::setText(this,"HomeName",UserInfo::Instance().getUserNicName());
	WidgetFun::setText(this,"HomeGold",UserInfo::Instance().getUserScore());
	WidgetFun::setText(this,"HomeZuanShi",UserInfo::Instance().getUserInsure());
	ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(this,"HeadImagic"),
		UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());

}
void HNHomeScence::LogonSucess()
{
	m_kIndividualMission.query(UserInfo::Instance().getUserID());

	m_kPublicNoticMission.PublicNotice("HN_SC_NOTICE");
	m_kPublicNoticMission.PublicNotice("NC_SHOP_TXT");
}
int HNHomeScence::getGameLevel()
{
	return m_iGameLevel;
}
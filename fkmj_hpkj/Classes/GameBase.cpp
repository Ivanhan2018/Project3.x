#include "GameBase.h"

#include "GameLib.h"
//#include "ArmFun.h"

#include "ScenceManagerBase.h"
#include "GameManagerBase.h"

GameBase::GameBase(unsigned int iType,unsigned int iOption)
	:m_pSelfPlayer(NULL)
	,m_kReqPlayerInfo(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	,m_iGameType(iType)
	,m_iGameOption(iOption)
{
	m_kReqPlayerInfo.setMissionSink(this);
}
GameBase::~GameBase()
{

}
bool GameBase::init()
{
	if (!Node::init())
	{
		return false;
	};
	return true;
}
void GameBase::setOptionRule(unsigned int iRule)
{
	m_iGameOption = iRule;
}
bool GameBase::HaveOptionRule(int iRule)
{
	return FvMask::HasAny(m_iGameOption,_MASK_(iRule));
}
void GameBase::Button_ExitGameBase(cocos2d::Ref*,WidgetUserInfo*)
{
	if (IServerItem::get())
	{
		IServerItem::get()->PerformStandUpAction();
	}
	else
	{
		ExitGame();
	}
}
void GameBase::clearInfo()
{
	while (m_kPlayers.size())
	{
		GamePlayer* pTempPlayer = m_kPlayers[0];
		removeGamePlayerToList(pTempPlayer);
		DeletePlayer(pTempPlayer);
	}
}
void GameBase::ExitGame()
{
	clearInfo();
	ScenceManagerBase::InstanceBase().GameBackScence();
	UserInfo::Instance().reqAccountInfo();
	GameManagerBase::InstanceBase().disconnectServer();
}

void GameBase::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{

}
void GameBase::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{

}
void GameBase::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{

}
bool GameBase::BackKey()
{
	if (!IsInGame())
	{
		return false;
	}
	PopScence::Instance().showAccessPlane(utility::getScriptString("GameExitKeyBack"),this,
		button_selector(GameBase::Button_CB_OnExit),NULL);
	return true;
}
void GameBase::Button_CB_OnExit(cocos2d::Ref*,WidgetUserInfo*)
{
	ExitGame();
}
void GameBase::Button_CB_SaveAndExit(cocos2d::Ref*,WidgetUserInfo*)
{
	UserInfo::Instance().setPrivateInGameServerID(-1);
	IServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_EXIT_SAVE,NULL,NULL);
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(GameBase::onWeiXinShareFriend,this),0.5f);
}
void GameBase::sendTalkFile(int iChair,std::string kFile)
{
	ssize_t iSize = 0;
	std::string kDestFile = kFile;
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
	kDestFile = cocos2d::CCFileUtils::getInstance()->getWritablePath()+utility::toString(time(NULL),".arm");
	//ArmFun::WavToArm(kFile.c_str(),kDestFile.c_str());
#endif
	CMD_GR_C_TableTalk kNetInfo;
	kNetInfo.cbChairID = iChair;
	kNetInfo.cbType = CMD_GR_C_TableTalk::TYPE_FILE;
	unsigned char* pData = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(kDestFile,"rb",&iSize);
	if (!pData)
	{
		NoticeMsg::Instance().ShowTopMsg("Record Error");
		return;
	}
	kNetInfo.cbChairID = iChair;
	memcpy(kNetInfo.strTalkData,pData,iSize);
	free(pData);
	kNetInfo.strTalkSize = iSize;
	int iNetSize = sizeof(kNetInfo) - sizeof(kNetInfo.strTalkData);
	IServerItem::get()->SendSocketData(MDM_GF_FRAME,SUB_GR_TABLE_TALK,&kNetInfo,iNetSize+iSize);
}
void GameBase::sendTalkString(int iChair,std::string strString)
{
	CMD_GR_C_TableTalk kNetInfo;
	kNetInfo.cbChairID = iChair;
    kNetInfo.cbType = CMD_GR_C_TableTalk::TYPE_WORD;
	utility::StringToChar(strString,kNetInfo.strString,128);
	int iNetSize = sizeof(kNetInfo) - sizeof(kNetInfo.strTalkData);
	IServerItem::get()->SendSocketData(MDM_GF_FRAME,SUB_GR_TABLE_TALK,&kNetInfo,iNetSize);
}
void GameBase::sendTalkDefine(int iChair,std::string strSoundPath)
{
	CMD_GR_C_TableTalk kNetInfo;
	kNetInfo.cbChairID = iChair;
	kNetInfo.cbType = CMD_GR_C_TableTalk::TYPE_DEFINE;
	utility::StringToChar(strSoundPath,kNetInfo.strString,128);
	int iNetSize = sizeof(kNetInfo) - sizeof(kNetInfo.strTalkData);
	IServerItem::get()->SendSocketData(MDM_GF_FRAME,SUB_GR_TABLE_TALK,&kNetInfo,iNetSize);
}
void GameBase::sendTalkBiaoQing(int iChair,std::string strFilePath)
{
	CMD_GR_C_TableTalk kNetInfo;
	kNetInfo.cbChairID = iChair;
	kNetInfo.cbType = CMD_GR_C_TableTalk::TYPE_BIAOQING;
	utility::StringToChar(strFilePath,kNetInfo.strString,128);
	int iNetSize = sizeof(kNetInfo) - sizeof(kNetInfo.strTalkData);
	IServerItem::get()->SendSocketData(MDM_GF_FRAME,SUB_GR_TABLE_TALK,&kNetInfo,iNetSize);
}
bool GameBase::RevTalk(void* data,int dataSize)
{
	if(!IsInGame())
	{
		return true;
	}
	CMD_GR_C_TableTalk* pNetInfo = (CMD_GR_C_TableTalk*)data;
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
	{
		RevTalk_File(pNetInfo);
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
	{
		RevTalk_String(pNetInfo);
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		RevTalk_TalkIdex(pNetInfo);
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
	{
		RevTalk_BiaoQing(pNetInfo);
	}
	return true;
}
bool GameBase::RevTalk_File(CMD_GR_C_TableTalk* pNetInfo)
{
	if (pNetInfo->strTalkSize == 0)
	{
		return true;
	}
	static int iIdex = 0;
	iIdex ++;
	std::string kFile = utility::toString(cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath(),"TableTalk",iIdex,".arm");
	FILE *fp = fopen(kFile.c_str(), "wb");

	fseek(fp,0,SEEK_END);
	fseek(fp,0,SEEK_SET);
	fwrite(&pNetInfo->strTalkData,sizeof(unsigned char), pNetInfo->strTalkSize,fp);
	fclose(fp);
	int iAddTime = pNetInfo->strTalkSize/1200+2.0f;
	if (iAddTime > 10)
	{
		iAddTime = 10;
	}
	std::string kDestFile = kFile;
	utility::StringReplace(kDestFile,"arm","wav");
	//ArmFun::ArmToWav(kFile.c_str(),kDestFile.c_str());
	SoundFun::Instance().PaseBackMusic();
	SoundFun::Instance().ResumeBackMusic(iAddTime);
	SoundFun::Instance().PaseEffectMusic();
	SoundFun::Instance().ResumeEffectMusic(iAddTime);

	SoundFun::Instance().playEffectDirect(kDestFile);

	GamePlayer* pPlayer = getBasePlayerByChairID(pNetInfo->cbChairID);
	if (pPlayer)
	{
		pPlayer->showTalkState(pNetInfo);
	}
	return true;
}
bool GameBase::RevTalk_String(CMD_GR_C_TableTalk* pNetInfo)
{
	GamePlayer* pPlayer = getBasePlayerByChairID(pNetInfo->cbChairID);
	if (pPlayer)
	{
		pPlayer->showTalkState(pNetInfo);
	}
	return true;
}
bool GameBase::RevTalk_TalkIdex(CMD_GR_C_TableTalk* pNetInfo)
{
	GamePlayer* pPlayer = getBasePlayerByChairID(pNetInfo->cbChairID);
	SoundFun::Instance().PaseBackMusic();
	SoundFun::Instance().ResumeBackMusic(5.0f);
	if (pPlayer)
	{
		pPlayer->showTalkState(pNetInfo);
	}
	return true;
}

bool GameBase::RevTalk_BiaoQing(CMD_GR_C_TableTalk* pNetInfo)
{
	GamePlayer* pPlayer = getBasePlayerByChairID(pNetInfo->cbChairID);
	if (pPlayer)
	{
		pPlayer->showTalkState(pNetInfo);
	}
	return true;
}
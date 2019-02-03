
#ifndef JNI_HELPER_H
#define JNI_HELPER_H

#include "cocos2d.h"
#include "EntityMgr.h"
#include "AnsString.h"
#include "Define.h"
#include "../proj.android/jni/hellocpp/Jni_system.h"
#include "GlobalUnits.h"
#include "TaskInfo.h"
#include "ConfigMgr.h"

using namespace cocos2d;

inline void showAndroidRechageLayer(const char* userId,const char* userLevel,const char* dialogID)
{
	//低等级 = 1 中等级 = 2 高等级 = 3 有X = 1 没X = 2
	showRechageLayer(userId,userLevel,dialogID);
}

inline void showAndroidFeedBackLayer()
{
	showFeedBackLayer();
}

inline void checkVersionToUpdate()
{
	checkUpdate();
}

inline void toPhoneSever()
{
	onLineServer();
}

inline void toAndridUserProtion(const char* str)
{
	toUserProtion(str);
}

inline void toSendAndroidOrder(int orderID)
{
	sendOrderID(orderID);
}

inline void toUpdateGame(std::string id,std::string str,const char* url)
{
	updateAndriodGame(id.c_str(),str.c_str(),url);
}

inline int getNetType()
{
	return getNetworkType();
}

inline void toLandscape()
{

}

inline void toPortrait()
{

}

inline void IMEI(const char* address)
{
	CCLOG("AAAAAAAAAAAmacaddress = %s",address);
	EntityMgr::instance()->setMacAddress(String::createWithFormat("%s%s","M",address)->getCString());
	CCLOG("cccccccccccccccccccccccc%s",address);
}

inline void reConnect()
{
	EntityMgr::instance()->getDispatch()->SendReConnect();	
}

inline void downloadGame(const char* gameID)
{
	downloadAndriodGame(gameID);
}

inline void RechargeableCard(const char* type, const char* dwCardFee, const char* cardnum, const char* cardpassword)
{
	CCLOG("type:%s,dwCardFee:%s,cardnum:%s,cardpassword:%s,",type,dwCardFee,cardnum,cardpassword);
	if(EntityMgr::instance()->getDispatch()->connectLoginServer())
	{
		EntityMgr::instance()->getDispatch()->phoneCardRechange(type,dwCardFee,cardnum,cardpassword);
	}
}

inline void PurpleGoldIslandCard(const char* cardnum, const char* cardpassword)
{
	CCLOG("cardnum:%s,cardpassword:%s,",cardnum,cardpassword);
	if(EntityMgr::instance()->getDispatch()->connectLoginServer())
	{
		EntityMgr::instance()->getDispatch()->isLnadCardRecarge(cardnum,cardpassword);
	}
}

inline void getOrder(const char* payName,const char* chargeNum)
{
	if (EntityMgr::instance()->getDispatch()->getLoginState() == enLogin_Server)
	{
		if(!EntityMgr::instance()->getDispatch()->connectLoginServer())
		{
			networkAnomaly();
			return;
		}
	}
	
	EntityMgr::instance()->getDispatch()->sendOrderInfo(payName,ans::AnsString::string2Int(chargeNum));
	CCLOG("payName = %s,string2Int(chargeNum) = %d",payName,ans::AnsString::string2Int(chargeNum));
	CCLOG("payName = %s,atoi(chargeNum) = %d",payName,atoi(chargeNum));
	
}

inline void orderOver(const char* coinsNum/*,const char* goldEggNum*/)
{
	g_GlobalUnits.getGolbalUserData()->lScore = ans::AnsString::string2Int(coinsNum);
	CCLOG("coinsNum in jnihelper  = %d",ans::AnsString::string2Int(coinsNum));
	//g_GlobalUnits.getGolbalUserData()->lGoldEggs = (int)goldEggNum;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATESCOREANDGOLDEGG,NULL);
}

inline void cannelRechanger()
{
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_CANNELRECHANGER,NULL);
}

inline void getChannl(const char* str)
{
	CCLOG("*****************************channelstr = %s",str);
	
	ConfigMgr::instance()->m_Config.m_channelStr = str;
}

inline void getVersion(const char* str)
{
	CCLOG("11111111111111111111version = %s",str);
	//分解版本 得到版本号

	int versionFix[3];
	versionFix[0] = 0;
	versionFix[1] = 0;
	versionFix[2] = 0;
	/* 校验x_sVer */
	bool ret = true;
	if(str==NULL || strlen(str)<5)
	{
		ret = false;
		CCLOG("version error size");
	}

	size_t iLen = strlen(str);
	int iCot = 0;

	for(size_t i=0; i<iLen; i++)
	{
		if(str[i]==0x2E)
		{
			iCot++;
		}
		else if(str[i]<0x30 || str[i]>0x39)
		{
			ret = false;
			CCLOG("have error char");
		}
	}

	if(iCot!=2)
	{
		ret = false;
		CCLOG("less point");
	}

	/* 获取 */
	if(ret)
	{
		sscanf(str, "%d.%d.%d", versionFix+0, versionFix+1, versionFix+2);
	}
	ConfigMgr::instance()->m_VersionInfo.m_nHighVersion = versionFix[0];
	ConfigMgr::instance()->m_VersionInfo.m_nMiddleVersion = versionFix[1];
	ConfigMgr::instance()->m_VersionInfo.m_nLowVersion = versionFix[2];
}

inline void downloadPesent(const char* pesent,const char* gameID)
{
	int _pesent = atoi(pesent);
	int _gameID = atoi(gameID);
	CCLOG("downloadgame gameID = %d,pesent = %d",_gameID,_pesent);

	if (EntityMgr::instance()->m_nDownLoadPe == _pesent)
	{
		return;
	}
	if(_pesent%2==0)
	{
		Integer* inter = Integer::create(_pesent);
		//EntityMgr::instance()->m_nDownLoadPe = _pesent;
#if 0
		switch (_gameID)
		{
		case GameId_DDZ:
			{
				break;
			}
		case GameId_PHZ:
			{
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_RUNBREAD,inter);
				if (_pesent >= 100)
				{
					//EntityMgr::instance()->getDispatch()->connectLoginServer();
					//EntityMgr::instance()->getDispatch()->getDownLoadAward(SeverID_PHZ,1);
					EntityMgr::instance()->m_nDownLoadPe = 1;
				}
				break;
			}
		case GameId_CSMJ:
			{
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_CHANGSHAMAJIADOWNLOAD,inter);
				if (_pesent >= 100)
				{
					//EntityMgr::instance()->getDispatch()->connectLoginServer();
					//EntityMgr::instance()->getDispatch()->getDownLoadAward(SeverID_CSMJ,1);
					EntityMgr::instance()->m_nDownLoadPe = 1;
				}
				break;
			}
		case GameId_ZJH:
			{
				break;
			}
		case GameId_SDK:
			{
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_THREETOONE,inter);
				if (_pesent >= 100)
				{
					//EntityMgr::instance()->getDispatch()->connectLoginServer();
					//EntityMgr::instance()->getDispatch()->getDownLoadAward(SeverID_SDH,1);
					EntityMgr::instance()->m_nDownLoadPe = 1;
				}
				break;
			}
		case GameId_PDK:
			{
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_RUNFASTDOWNLOAD,inter);
				if (_pesent >= 100)
				{
					//EntityMgr::instance()->getDispatch()->connectLoginServer();
					//EntityMgr::instance()->getDispatch()->getDownLoadAward(SeverID_PDK,1);
					EntityMgr::instance()->m_nDownLoadPe = 1;
				}
				break;
			}

		default:
			{
				assert(false);
				return;
			}
		}
#endif
	}
}

inline int isGameExit(int gameID)
{
	CCLOG("GAMEID = %d,exist = %d",gameID,getGameExist(gameID));
	return getGameExist(gameID);
}

inline void openAndriodGame(const char* gameID)
{
	openGame(gameID);
}

inline void outInout(const char* str)
{
	CCLOG("%s  is null ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`",str);
}

inline void cannelGameDownload()
{
	CCLOG("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	EntityMgr::instance()->m_nDownLoadPe = 1;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_CANNELDOWNLOADGAME,NULL);
	CCLOG("cccccccccccccccccccccccccccccccccccccccccccccc");
}

inline void newUMMessage()
{
	TaskInfo* info = TaskInfo::ShareTaskInfo();
	info->m_nUMMessage++;
}

//void setPackageName(const char *packageName)
//{
//	CCLOG("packageName: %s", packageName); 
//}
//
//void setLogin(const char* str)
//{
//
//	//Director::getInstance()->end();
//	EntityMgr::instance()->setLogin(str);
//}
//
//void setUserInfo(const char* str)
//{
//	EntityMgr::instance()->setUserInfo(str);
//}
//
//void setUserName(const char* str)
//{
//	EntityMgr::instance()->setUserName(str);
//}
//
//void setLevelStandart(const char* str)
//{
//	EntityMgr::instance()->setLevelStandart(str);
//}
//
//void setDangweiStandart(const char* str)
//{
//	EntityMgr::instance()->setDangweiStandart(str);
//}
//
//void setMusic(const char* str)
//{
//	EntityMgr::instance()->setMusic(str);
//}
//
//void exitView()
//{
//	CCLOG("exitView1");
//	tagSigOutObj tag;
//	tag.bSigOut = true;
//	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_SIGOUT,&tag);
//	CCLOG("exitView2"); 
//}

#endif //JNI_HELPER_H

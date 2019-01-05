//
//  Login.cpp
//  Game
//
//  Created by zhouwei on 13-6-17.
//
//

#include "Login.h"
#include "EntityMgr.h"
#include "CMD_Game.h"
#include "CMD_Plaza.h"
#include "GlobalDef.h"
#include "GlobalUnits.h"
#include "cocos2d.h"
#include "utf-8.h"
#include "GlobalField.h"
#include "GlobalFrame.h"
#include "TaskInfo.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

Login::Login()
{
    GBEVENT_ARG_REGISTER(GBEVENT_NETWORK_LOGIN, Login::onSocketLogin);
    
}
Login::~Login()
{
    GBEVENT_ARG_UNREGISTER(GBEVENT_NETWORK_LOGIN, Login::onSocketLogin);
}

//用户登陆
void Login::setUserLogin(string account,string password)
{
    EntityMgr::instance()->getDispatch()->setUserLogin(account, password);
    m_szAccount = account;
    m_szPassword = password;
}

//用户注册
void Login::setUserRegister(const char* szAccount,const char* szPassword,int nfaceId,int nGender, int visiter)
{
    CCLOG("%d",nfaceId);
   EntityMgr::instance()->getDispatch()->setUserRegister(szAccount, szPassword,nfaceId,nGender,visiter);
	//EntityMgr::instance()->getDispatch()->setUserLogin(szAccount, szPassword);
    m_szAccount = szAccount;
    m_szPassword = szPassword;
}
void Login::onSocketLogin(GBEventConstArg& arg)
{
    tagCmdBuffer& tag = (tagCmdBuffer&)arg;
    CMD_Command* pCommand = &tag.command;
    
    enLoginState state = EntityMgr::instance()->getDispatch()->getLoginState();
    if (state == enLogin_Server)
    {
        onEventLoginRead(pCommand,tag.dateBuffer,tag.nDataSize);
        return;
    }
    //onEventGameRead(pCommand,tag.dateBuffer,tag.nDataSize);
}

bool Login::onEventLoginRead(CMD_Command* pCommand,void* pBuffer,WORD wDataSize)
{
    switch (pCommand->wSubCmdID)
    {
        case SUB_GP_GET_PZINFO:     //获取服务器配置信息
        {
            //效验参数
            if (wDataSize<sizeof(CMD_GP_LoginConfigResult))
            {
                return false;
            }
            
            CMD_GP_LoginConfigResult* result = (CMD_GP_LoginConfigResult*)pBuffer;
            g_GlobalUnits.m_nOnlineCount = 0;

			char szVer[24] = {0};	// 版本号
			snprintf(szVer, sizeof(szVer)-1, "%d.%d.%d", result->nMajorVer, result->nMinorVer, result->wRevisionVer);
			
			char szSize[48] = {0};
			snprintf(szSize, sizeof(szSize)-1, "%.2f", result->dwVerSize / 1024.f);
			string strSize = szSize;
			if(result->nUpdateType == 0)
			{
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_VERSIONNEW,NULL);
			}
			else
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				toUpdateGame(ans::AnsString::int2String(result->nUpdateType),szSize,result->szDownUrl);
#endif
			}


            return true;
        }
		case SUB_GP_LOGON_VERSION:     //获取版本信息
			{
				//效验参数
				if (wDataSize<sizeof(DWORD))
				{
					return false;
				}

				DWORD result = *(DWORD*)pBuffer;
				Integer* inter = Integer::create(result);
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_VERSIONERROR,inter);

				
				return true;
			}
        case SUB_GF_PHONE_CONFIG:
        {
            //效验参数
            if (wDataSize<sizeof(CMD_GP_IOSConfigResult))
            {
                return false;
            }
            
            CMD_GP_IOSConfigResult* result = (CMD_GP_IOSConfigResult*)pBuffer;
            
            if (result->lGameKindid == 0)
            {
                EntityMgr::instance()->getDispatch()->closeLoginSocket();
               
                return  true;
            }
            //判断是否是斗地主
            if (result->lGameKindid == 300)
            {
                tagConfigObj configObj;
                memset(&configObj, 0, sizeof(configObj));
                configObj.nHighVerID    = result->lHighVerID;
                configObj.nLowVerID     = result->lLowVerID;
                configObj.bForcedUpdate = result->isForcedUpdate;
                
				//char buf[50];
				//memset(buf, 0, 50);
				//utf::utf_gb_convert("gb2312", "utf-8", configObj.szGame,128, (char*)result->szGameGg, 128);
				//CCLOG("s = %s",configObj.szGame);

				//memcpy( g_GlobalUnits.m_szTextTips, configObj.szGame,128);
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFINISH,&configObj);
            }
            break;
        }
        case SUB_GR_LOGON_SUCCESS: //登陆成功
        {
            //效验参数
            if (wDataSize<sizeof(CMD_GP_LogonSuccess))
            {
                return false;
            }
            //保存信息
            tagGlobalUserData* pUserData = new tagGlobalUserData;
            CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pBuffer;
            
   //         pUserData->wFaceID=pLogonSuccess->wFaceID;
   //         pUserData->cbGender=pLogonSuccess->cbGender;
   //         pUserData->cbMember=pLogonSuccess->cbMember;
   //         pUserData->lFleeCount=pLogonSuccess->lFleeCount;
   //         pUserData->lGoldEggs=pLogonSuccess->lGoldEggs;
   //         pUserData->lScore=pLogonSuccess->lScore;
   //         pUserData->lWeekMingci=pLogonSuccess->lWeekMingci;
   //         pUserData->lWeekWinCount=pLogonSuccess->lWeekWinCount;
   //         pUserData->lWeiWang=pLogonSuccess->lWeiWang;
   //         pUserData->lWinCount=pLogonSuccess->lWinCount;
			//pUserData->lLostCount = pLogonSuccess->lLostCount;
   //         pUserData->sdKindID=pLogonSuccess->sdKindID;
   //         pUserData->sdServerID=pLogonSuccess->sdServerID;
   //         pUserData->dwUserID=pLogonSuccess->dwUserID;
   //         pUserData->dwGameID=pLogonSuccess->dwGameID;
         
            
            //unsigned char srcBuf[255];
            //unsigned int ntemp1 = 255;
            //unsigned int* srcLen = &ntemp1;
            
            unsigned char desBuf[255];
            //unsigned int ntemp2 = 255;
            //unsigned int* desLen = &ntemp2;
        
            //读取扩展信息
			void * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
			/*while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
					memset(desBuf,0,sizeof(desBuf));

                    case DTP_USER_ACCOUNTS:		//用户账户
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(pUserData->szAccounts));
						if (DataDescribe.wDataSize<=sizeof(pUserData->szAccounts))
						{
                            
                            memcpy(desBuf,pDataBuffer,DataDescribe.wDataSize);
                            
                            //CCLOG("szAccounts:%s",pUserData->szAccounts);
                           
                            //utf::msf_xchr_gb_to_utf8(srcBuf, srcLen, desBuf, desLen);
                            memcpy(pUserData->szAccounts,pDataBuffer,DataDescribe.wDataSize);

                            CCLOG("szAccounts:%s",pUserData->szAccounts);
                            m_szAccount = pUserData->szAccounts;

						}
						break;
					}
                    case DTP_USER_PASS:			//用户密码
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(pUserData->szPassWord));
						if (DataDescribe.wDataSize<=sizeof(pUserData->szPassWord))
						{
                            memcpy(desBuf,pDataBuffer,DataDescribe.wDataSize);
                            //utf::msf_xchr_gb_to_utf8(srcBuf, srcLen, desBuf, desLen);
                            memcpy(pUserData->szPassWord,desBuf,sizeof(pUserData->szPassWord));

							//memcpy(pUserData->szPassWord,desBuf,sizeof(pUserData->szPassWord));
						}
						break;
					}
                        
                    case DTP_USER_GROUP_NAME:	//社团名字
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(pUserData->szGroupName));
						if (DataDescribe.wDataSize<=sizeof(pUserData->szGroupName))
						{                            
                            memcpy(desBuf,pDataBuffer,DataDescribe.wDataSize);
                            //utf::msf_xchr_gb_to_utf8(srcBuf, srcLen, desBuf, desLen);
                            memcpy(pUserData->szGroupName,desBuf,sizeof(pUserData->szGroupName));
						}
						break;
					}
                    case DTP_NICK_NAME:		//用户昵称
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(pUserData->szNickName));
						if (DataDescribe.wDataSize<=sizeof(pUserData->szNickName))
						{
							memcpy(desBuf,pDataBuffer,DataDescribe.wDataSize);
							//utf::msf_xchr_gb_to_utf8(srcBuf, srcLen, desBuf, desLen);
							memcpy(pUserData->szNickName,desBuf,sizeof(pUserData->szNickName));
						}
						break;
					}
                    case DTP_STATION_PAGE:		//游戏主站
					{
//						ASSERT(pDataBuffer!=NULL);
//						if (pDataBuffer!=NULL)
//						{
//							g_GlobalUnits.SetStationPage((LPCTSTR)pDataBuffer);
//							//m_pHtmlBrower->Navigate(g_GlobalUnits.GetStationPage());
//						}
						break;
					}
                    case DTP_USER_ZIP_CODE:	//社团名字
					{
//						ASSERT(pDataBuffer!=NULL);
//						ASSERT(DataDescribe.wDataSize>0);
//						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szZipCode));
//						if (DataDescribe.wDataSize<=sizeof(UserData.szZipCode))
//						{
//							memcpy(UserData.szZipCode,pDataBuffer,DataDescribe.wDataSize);
//							UserData.szZipCode[CountArray(UserData.szZipCode)-1]=0;
//						}
						break;
					}
                    case DTP_USER_CITY:	//城市信息
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(pUserData->szDwellingPlace));
						if (DataDescribe.wDataSize<=sizeof(pUserData->szDwellingPlace))
						{                            
                            memcpy(desBuf,pDataBuffer,DataDescribe.wDataSize);
                            //utf::msf_xchr_gb_to_utf8(srcBuf, srcLen, desBuf, desLen);
                            memcpy(pUserData->szDwellingPlace,desBuf,sizeof(pUserData->szDwellingPlace));
						}
						break;
					}
                    case DTP_USER_DESCRIBE:
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(pUserData->szDescribeString));
						if (DataDescribe.wDataSize<=sizeof(pUserData->szDescribeString))
						{
                            memcpy(desBuf,pDataBuffer,DataDescribe.wDataSize);
                            //utf::msf_xchr_gb_to_utf8(srcBuf, srcLen, desBuf, desLen);
                            memcpy(pUserData->szDescribeString,desBuf,sizeof(pUserData->szDescribeString));
                            //CCLOG((const char*)desBuf);
						}
						break;
					}
					case DTP_USER_VISITOR:
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(pUserData->cbVisitor));
						if(DataDescribe.wDataSize <= sizeof(pUserData->cbVisitor))
						{
							memcpy(desBuf,pDataBuffer,DataDescribe.wDataSize);
							//utf::msf_xchr_gb_to_utf8(srcBuf, srcLen, desBuf, desLen);
							pUserData->cbVisitor = *(BYTE *)desBuf;
							CCLOG((const char*)desBuf);
						}
						break;

					}
					case DTP_USER_Keepdays:
					{
						assert(pDataBuffer!=NULL);
						assert(DataDescribe.wDataSize>0);
						assert(DataDescribe.wDataSize<=sizeof(UKeepLogonDays));
						if (DataDescribe.wDataSize <= sizeof(UKeepLogonDays))
						{
							UKeepLogonDays *pKeep = (UKeepLogonDays *)pDataBuffer;
							pUserData->nTodayScore = pKeep->dwTodayScore;
							pUserData->nTomorrowScore = pKeep->dwTomorrowScore;
							pUserData->nKeepDays = pKeep->days;
							pUserData->nCanGet = pKeep->canGet;
						}
						
						break;
					}
					case DTP_USER_PlayCount:
						{
							assert(pDataBuffer!=NULL);
							assert(DataDescribe.wDataSize>0);
							assert(DataDescribe.wDataSize<=sizeof(int));
							if (DataDescribe.wDataSize <= sizeof(int))
							{
								int playCount = *(int *)pDataBuffer;
								TaskInfo::ShareTaskInfo()->m_nGameCont = playCount;

							}

							break;
						}
					case DTP_USER_GetPoint:
						{
							assert(pDataBuffer!=NULL);
							assert(DataDescribe.wDataSize>0);
							assert(DataDescribe.wDataSize<=sizeof(int));
							if (DataDescribe.wDataSize <= sizeof(int))
							{

							}

							break;
						}
					case DTP_USER_PrizeInfo:
						{
							assert(pDataBuffer!=NULL);
							assert(DataDescribe.wDataSize>0);
							assert(DataDescribe.wDataSize<=sizeof(DWORD));
							if (DataDescribe.wDataSize <= sizeof(DWORD))
							{
								int prizeInfo = *(DWORD *)pDataBuffer;
								TaskInfo::ShareTaskInfo()->initData(prizeInfo);
							}

							break;
						}
					case DTP_USER_TotalCount:
						{
							assert(pDataBuffer!=NULL);
							assert(DataDescribe.wDataSize>0);
							assert(DataDescribe.wDataSize<=sizeof(DWORD));
							if (DataDescribe.wDataSize <= sizeof(DWORD))
							{
								pUserData->lTotalCount = *(DWORD *)pDataBuffer;
							}
							break;
						}
					case DTP_USER_RelieveCount:
						{
							assert(pDataBuffer!=NULL);
							assert(DataDescribe.wDataSize==sizeof(char));
							if (DataDescribe.wDataSize == sizeof(char))
							{
								pUserData->nRelieveCount = *(char *)pDataBuffer;
							}
						break;
						}
                    default: { assert(false); }
				}
			}
            */
            g_GlobalUnits.setGolbalUserData(pUserData);
            CCLOG("wFaceID:%d",pUserData->wFaceID);
            
            CCLOG("login success2! file(%s) line(%d)",__FILE__, __LINE__);

            return true;
            }
            case SUB_GR_LOGON_ERROR:   //登陆失败
            {
                EntityMgr::instance()->getDispatch()->closeLoginSocket();
             
                //效验参数
                CMD_GP_LogonError* pLogonError = (CMD_GP_LogonError *)pBuffer;
                assert(wDataSize>=(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)));
                if (wDataSize<(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)))
                {
                    return false;
                }
                
                if ( !EntityMgr::instance()->getDispatch()->isRegLogin())
                {
                    CCLOG("login failed! file(%s) line(%d)",__FILE__, __LINE__);
                    //GBEvent::instance()->fire(GBEVENT_UI_LOGINFAILE);
                    
                    NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFAIL);
                    return true;
                }
        
                //显示消息
                WORD wDescribeSize = wDataSize-(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe));
                if (wDescribeSize>0)
                {
                    pLogonError->szErrorDescribe[wDescribeSize-1] = 0;
                    tagRegBuffer tag;
                    //tag.lErrorCode = pLogonError->lErrorCode;
                    unsigned int ntemp = 128;
                    unsigned int* srclen = &ntemp;
                    
                    utf::msf_xchr_gb_to_utf8((unsigned char*)pLogonError->szErrorDescribe, srclen, tag.szErrorDescribe, srclen);
                    EntityMgr::instance()->getDispatch()->setRegLogin(false);
					String* lerror = String::createWithFormat("%s",tag.szErrorDescribe);
                    NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REGNFAIL,lerror);
                    return true;
                }
                

                return true;
            }

            case SUB_GR_LOGON_FINISH:  //登陆完成
            {
         
                EntityMgr::instance()->getDispatch()->closeLoginSocket();
                
                UserDefault::getInstance()->setStringForKey("NEWZJD_ACCOUNT", m_szAccount);
                UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", m_szPassword);
                
                
                
                //为注册登陆
                if ( EntityMgr::instance()->getDispatch()->isRegLogin())
                {
                    CCLOG("register success! file(%s) line(%d)",__FILE__, __LINE__);
                    
                    int count = UserDefault::getInstance()->getIntegerForKey("KEY_ACCOUNT_COUNT") + 1;
                    UserDefault::getInstance()->setIntegerForKey("KEY_ACCOUNT_COUNT", count);
                    
                    EntityMgr::instance()->getDispatch()->setRegLogin(false);
                    //GBEvent::instance()->fire(GBEVENT_UI_REGISTERSUCC);

					NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REGSUCC);
                    return true;
                }
                
                CCLOG("login success! file(%s) line(%d)",__FILE__, __LINE__);
                //GBEvent::instance()->fire(GBEVENT_UI_LOGINFINISH);
                NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFINISH);
               
                return true;
            }
        case SUB_GP_GET_PASSWORD_RESULT:  //找回密码
        {
            EntityMgr::instance()->getDispatch()->closeLoginSocket();
            CCLOG("find password! file(%s) line(%d)",__FILE__, __LINE__);
            
            //获取密码结果
//            struct CMD_GP_GetPasswordResult
//            {
//                LONG			lErrCode;	//消息标志 0成功
//                char			szPassword[33];	//密码
//                char			szErrorDescribe[128];	//描述信息
//            };
            
            CMD_GP_GetPasswordResult* pGetPassword = (CMD_GP_GetPasswordResult*)pBuffer;
            
            tagGetPassObj obj;
            unsigned int ntemp = 128;
            unsigned int* srclen = &ntemp;
            
            obj.lErrCode = pGetPassword->lErrCode;
            memcpy(obj.szPassword, pGetPassword->szPassword, sizeof(obj.szPassword));
            
            utf::msf_xchr_gb_to_utf8((unsigned char*)pGetPassword->szErrorDescribe, srclen, obj.szErrorDescribe, srclen);
                
            NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_FINDPASS,&obj);

            return true;
        }
        case SUB_GP_GET_PASSWORD_GETALIST_RESULT:       //获取用户列表
        {
            EntityMgr::instance()->getDispatch()->closeLoginSocket();
            
            CMD_GP_GetAccountListResult* pList = (CMD_GP_GetAccountListResult*)pBuffer;
            
            tagUserListObj obj;
            obj.lErrCode = pList->lErrCode;
            memcpy(obj.szUserNameList, pList->szUserNameList, sizeof(obj.szUserNameList));
            
            CCLOG("get username list！%s",pList->szUserNameList);
//            string str = pList->szUserNameList;
//            
//            for (int i =0; str.length(); i++)
//            {
//                int offset = str.find('}');
//                string temp =str.substr(0,offset);
//                str = str.substr(offset + 1);
//                
//                if (offset == -1) break;
//            }

            NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_USERLIST,&obj);
            
            return true;
        }
            default:
                break;
    }
    return  true;
}

//
//  Login.cpp
//  Game
//
//  Created by zhouwei on 13-6-17.
//
//

#include "Login.h"
#include "EntityMgr.h"
#include "CMD_GameServer.h"
#include "CMD_LogonServer.h"
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
    pMsgDispatch->setUserLogin(account, password);
    m_szAccount = account;
    m_szPassword = password;
}

//用户注册
void Login::setUserRegister(const char* szAccount,const char* szPassword,int nfaceId,int nGender, int visiter)
{
    pMsgDispatch->setUserRegister(szAccount, szPassword,nfaceId,nGender,visiter);
    m_szAccount = szAccount;
    m_szPassword = szPassword;
}

void Login::onSocketLogin(GBEventConstArg& arg)
{
    tagCmdBuffer& tag = (tagCmdBuffer&)arg;
    CMD_Command* pCommand = &tag.command;
    
    enLoginState state = pMsgDispatch->getLoginState();
    if (state == enLogin_Server)
    {
        onEventLoginRead(pCommand,tag.dateBuffer,tag.nDataSize);
        return;
    }
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
                pMsgDispatch->closeLoginSocket();
               
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
             
            unsigned char desBuf[255];
        
            //读取扩展信息
			void * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));

            g_GlobalUnits.setGolbalUserData(pUserData);
            CCLOG("wFaceID:%d",pUserData->wFaceID);
            
            CCLOG("login success2! file(%s) line(%d)",__FILE__, __LINE__);

            return true;
            }
            case SUB_GR_LOGON_ERROR:   //登陆失败
            {
                pMsgDispatch->closeLoginSocket();
             
                //效验参数
                CMD_GP_LogonError* pLogonError = (CMD_GP_LogonError *)pBuffer;
                assert(wDataSize>=(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)));
                if (wDataSize<(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)))
                {
                    return false;
                }
                
                if ( !pMsgDispatch->isRegLogin())
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
                    unsigned int ntemp = 128;
                    unsigned int* srclen = &ntemp;
                    
                    utf::msf_xchr_gb_to_utf8((unsigned char*)pLogonError->szErrorDescribe, srclen, tag.szErrorDescribe, srclen);
                    pMsgDispatch->setRegLogin(false);
					String* lerror = String::createWithFormat("%s",tag.szErrorDescribe);
                    NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REGNFAIL,lerror);
                    return true;
                }
                

                return true;
            }

            case SUB_GR_LOGON_FINISH:  //登陆完成
            {
         
                pMsgDispatch->closeLoginSocket();
                
                UserDefault::getInstance()->setStringForKey("NEWZJD_ACCOUNT", m_szAccount);
                UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", m_szPassword);
                
                
                
                //为注册登陆
                if ( pMsgDispatch->isRegLogin())
                {
                    CCLOG("register success! file(%s) line(%d)",__FILE__, __LINE__);
                    
                    int count = UserDefault::getInstance()->getIntegerForKey("KEY_ACCOUNT_COUNT") + 1;
                    UserDefault::getInstance()->setIntegerForKey("KEY_ACCOUNT_COUNT", count);
                    
                    pMsgDispatch->setRegLogin(false);
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
            pMsgDispatch->closeLoginSocket();
            CCLOG("find password! file(%s) line(%d)",__FILE__, __LINE__);
            
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
            pMsgDispatch->closeLoginSocket();
            
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

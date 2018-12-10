#include "MissionWeiXin.h"
#include "ScriptData.h"
#include "cocos/base/CCUserDefault.h"
#include "JniFun.h"
#include "GameLib.h"

FV_SINGLETON_STORAGE(MissionWeiXin)

MissionWeiXin::MissionWeiXin()
	:m_pJinMissionSink(NULL)
{
}

MissionWeiXin::~MissionWeiXin()
{

}

void MissionWeiXin::logonWeiXin()
{
	std::string access_token = cocos2d::UserDefault::getInstance()->getStringForKey("access_token");
	std::string refresh_token = cocos2d::UserDefault::getInstance()->getStringForKey("refresh_token");
	std::string openid = cocos2d::UserDefault::getInstance()->getStringForKey("openid","");
	if (access_token != "" && openid != "")
	{
		Req_UserInfo(access_token,openid);
	}
	else
	{
		JniFun::longinWX("","");
	}
}
void MissionWeiXin::restWeiXin()
{
	cocos2d::UserDefault::getInstance()->setStringForKey("access_token","");
	cocos2d::UserDefault::getInstance()->setStringForKey("refresh_token","");
}
void MissionWeiXin::setMissionSink( IWeiXinMissionSink* pJniMissionSink )
{
	m_pJinMissionSink = pJniMissionSink;
}

IWeiXinMissionSink* MissionWeiXin::getMissionSink()
{
	return m_pJinMissionSink;
}

void MissionWeiXin::Req_Fail_token(std::string param)
{
	if (m_pJinMissionSink)
	{
		m_pJinMissionSink->onWxLoginFail(param);
	}
}
void MissionWeiXin::Req_Access_token(std::string param)
{
	std::vector<std::string> kVector = utility::split(param,":");
	if (kVector.size() != 4)
	{
		utility::log("if (kVector.size() != 4)");
		return ;
	}
	std::string kUrl = "https://api.weixin.qq.com/sns/oauth2/access_token?";

	m_kWxAppId = kVector[0];
	m_kWxAppSecret = kVector[1];

	MCWebReq::pushValue(kUrl,"appid",kVector[0]);
	MCWebReq::pushValue(kUrl,"secret",kVector[1]);
	MCWebReq::pushValue(kUrl,"code",kVector[2]);
	MCWebReq::pushValue(kUrl,"grant_type",kVector[3]);

	//	kUrl="https://api.weixin.qq.com/sns/oauth2/access_token?appid=wx79ad4c77ea07f41e&secret=66ad79231c6954ba68811afb14fad5e2&code=031RcTgJ17gEpa028PfJ1a9VgJ1RcTgO&grant_type=authorization_code";
	MCWebReq::instance().sendRequestDocumentUrl(kUrl,CC_CALLBACK_1(MissionWeiXin::Rsp_Access_token,this),nullptr);
}

void MissionWeiXin::Rsp_Access_token(rapidjson::Document* pDoc)
{
	std::string access_token = MCWebReq::getDataValueStr(pDoc,"access_token");
	int expires_in = MCWebReq::getDataValueInt(pDoc,"expires_in");
	std::string refresh_token = MCWebReq::getDataValueStr(pDoc,"refresh_token");
	std::string openid = MCWebReq::getDataValueStr(pDoc,"openid");
	std::string scope = MCWebReq::getDataValueStr(pDoc,"scope");
	std::string unionid = MCWebReq::getDataValueStr(pDoc,"unionid");

	cocos2d::log(" JNIMission access_token:%s",access_token.c_str());
	cocos2d::log(" JNIMission expires_in:%d",expires_in);
	cocos2d::log(" JNIMission refresh_token:%s",refresh_token.c_str());
	cocos2d::log(" JNIMission openid:%s",openid.c_str());
	cocos2d::log(" JNIMission scope:%s",scope.c_str());
	cocos2d::log(" JNIMission unionid:%s",unionid.c_str());

	cocos2d::UserDefault::getInstance()->setStringForKey("access_token",access_token);
	cocos2d::UserDefault::getInstance()->setStringForKey("refresh_token",refresh_token);
	cocos2d::UserDefault::getInstance()->setStringForKey("openid",openid);
	cocos2d::UserDefault::getInstance()->setStringForKey("unionid",unionid);
	cocos2d::UserDefault::getInstance()->flush();

	Req_UserInfo(access_token,openid);
}

void MissionWeiXin::Req_UserInfo(const std::string& access_token,const std::string& open_id)
{
	std::string kUrl = "https://api.weixin.qq.com/sns/userinfo?";
	MCWebReq::pushValue(kUrl,"access_token",access_token);
	MCWebReq::pushValue(kUrl,"openid",open_id);
	MCWebReq::instance().sendRequestDocumentUrl(kUrl,CC_CALLBACK_1(MissionWeiXin::Rsp_UserInfo,this),nullptr);
}
void MissionWeiXin::Rsp_UserInfo(rapidjson::Document* pDoc)
{
	int errcode = MCWebReq::getDataValueInt(pDoc,"errcode");
	if (errcode!=0)
	{
		JniFun::longinWX("","");
		return;
	}

	WxUserInfo kWxUserInfo;
	kWxUserInfo.openid = MCWebReq::getDataValueStr(pDoc,"openid");
	kWxUserInfo.nickname = MCWebReq::getDataValueStr(pDoc,"nickname");
	kWxUserInfo.sex = MCWebReq::getDataValueInt(pDoc,"sex");
	kWxUserInfo.province = MCWebReq::getDataValueStr(pDoc,"province");
	kWxUserInfo.city = MCWebReq::getDataValueStr(pDoc,"city");
	kWxUserInfo.country = MCWebReq::getDataValueStr(pDoc,"country");
	kWxUserInfo.headimgurl = MCWebReq::getDataValueStr(pDoc,"headimgurl");
	kWxUserInfo.unionid = MCWebReq::getDataValueStr(pDoc,"unionid");

	if (kWxUserInfo.sex == 2)
	{
		kWxUserInfo.sex = 0;
	}
	else
	{
		kWxUserInfo.sex = 1;
	}

	cocos2d::UserDefault::getInstance()->setStringForKey("openid",kWxUserInfo.openid);
	cocos2d::UserDefault::getInstance()->setStringForKey("unionid",kWxUserInfo.unionid);
	cocos2d::UserDefault::getInstance()->flush();

	if (m_pJinMissionSink)
	{
		m_pJinMissionSink->onWxLoginSuccess(kWxUserInfo);
	}
}


void MissionWeiXin::Req_RefreshToken()
{
	std::string kUrl = "https://api.weixin.qq.com/sns/oauth2/refresh_token?";
	std::string access_token = cocos2d::UserDefault::getInstance()->getStringForKey("access_token");
	std::string refresh_token = cocos2d::UserDefault::getInstance()->getStringForKey("refresh_token");

	MCWebReq::pushValue(kUrl,"appid",access_token);
	MCWebReq::pushValue(kUrl,"grant_type","refresh_token");
	MCWebReq::pushValue(kUrl,"refresh_token",refresh_token);
	MCWebReq::instance().sendRequestDocumentUrl(kUrl,CC_CALLBACK_1(MissionWeiXin::Rsp_RefreshToken,this),nullptr);
}
void MissionWeiXin::Rsp_RefreshToken(rapidjson::Document* pDoc)
{
	Rsp_Access_token(pDoc);
}

void MissionWeiXin::shareScreenWeiXin(std::string strImagic,int nShareType /*= SHARE_SESSION*/)
{
	captureScreen(true,strImagic,nShareType);
}
void MissionWeiXin::captureScreen(bool succeed, const std::string& outputFile,int nShareType)
{
	if (succeed)
	{
		cocos2d::log("MissionWeiXin captureScreen:%s",outputFile.c_str());
		JniFun::shareImageWX(outputFile.c_str(),nShareType);
	}
	else
	{
		cocos2d::log("MissionWeiXin captureScreen failed!");
	}
}
void MissionWeiXin::shareTextWeiXin(std::string kText,int nShareType /*= SHARE_SESSION*/)
{
	JniFun::shareTextWX(kText.c_str(),nShareType);
}
void MissionWeiXin::shareUrlWeiXin(std::string kUrl,std::string kTitle,std::string kDesc,int nShareType)
{
	JniFun::shareUrlWX(kUrl.c_str(),kTitle.c_str(),kDesc.c_str(),nShareType);
}

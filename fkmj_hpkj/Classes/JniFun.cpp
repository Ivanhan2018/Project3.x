#include "JniFun.h"
#include "cocos2d.h"
#include "Convert.h"
#include "utility.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <Jni.h>
#include "platform/android/jni/JniHelper.h"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "IosHelper.h"
#endif

#define JAVA_CLASSNAME  "org/cocos2dx/cpp/Native"
using namespace cocos2d;
namespace JniFun
{

	void longinWX(const char* APP_ID,const char* AppSecret)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME,"LoginWX","(Ljava/lang/String;Ljava/lang/String;)V");
		if (isHave)
		{
			jstring jAPP_ID = minfo.env->NewStringUTF(APP_ID);
			jstring jAppSecret = minfo.env->NewStringUTF(AppSecret);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jAPP_ID,jAppSecret);

			minfo.env->DeleteLocalRef(jAPP_ID);
			minfo.env->DeleteLocalRef(jAppSecret);
			minfo.env->DeleteLocalRef(minfo.classID); 
			cocos2d::log("JniFun call LoginWX over!");
		}
		else
		{
			cocos2d::log("JniFun call LoginWX error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		IosHelper::sendAuthRequest();
#endif
	}

	void shareImageWX(const char* ImgPath,int nType)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME,"ShareImageWX","(Ljava/lang/String;I)V");
		if (isHave)
		{
			jstring jImgPath = minfo.env->NewStringUTF(ImgPath);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jImgPath,nType);
			cocos2d::log("JniFun call ShareImageWX over!");

			minfo.env->DeleteLocalRef(jImgPath);
			minfo.env->DeleteLocalRef(minfo.classID); 
		}
		else
		{
			cocos2d::log("JniFun call ShareImageWX error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		if (nType == 0)
		{
			IosHelper::shareWithWeixinFriendImg("",ImgPath);
		}
		else
		{
			IosHelper::shareWithWeixinCircleImg("",ImgPath);
		}
#endif
	}

	void shareTextWX(const char* kText,int nType)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			JniMethodInfo minfo;
			bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME,"ShareTextWX","(Ljava/lang/String;I)V");
			if (isHave)
			{
				jstring jkText = minfo.env->NewStringUTF(kText);
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jkText,nType);
				cocos2d::log("JniFun call ShareTextWX over!");

				minfo.env->DeleteLocalRef(jkText);
				minfo.env->DeleteLocalRef(minfo.classID); 
			}
			else
			{
				cocos2d::log("JniFun call ShareTextWX error!");
			}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			if (nType == 0)
			{
				IosHelper::shareWithWeixinFriendTxt(kText,kText,"www.baidu.com");
			}
			else
			{
				IosHelper::shareWithWeixinCircleTxt(kText,"www.baidu.com");
			}
#endif
	}

	void shareUrlWX(const char* kUrl,const char* kTitle,const char* kDesc,int nType)
	{

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME,"ShareUrlWX","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
		if (isHave)
		{
			jstring jkUrlt = minfo.env->NewStringUTF(kUrl);
			jstring jkTitle = minfo.env->NewStringUTF(kTitle);
			jstring jkDesc = minfo.env->NewStringUTF(kDesc);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jkUrlt,jkTitle,jkDesc,nType);
			cocos2d::log("JniFun call ShareUrlWX over!");

			minfo.env->DeleteLocalRef(jkUrlt);
			minfo.env->DeleteLocalRef(jkTitle);
			minfo.env->DeleteLocalRef(jkDesc);
			minfo.env->DeleteLocalRef(minfo.classID); 
		}
		else
		{
			cocos2d::log("JniFun call ShareUrlWX error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		if (nType == 0)
		{
			IosHelper::shareWithWeixinFriendTxt(kTitle,kDesc,kUrl);
		}
		else
		{
			IosHelper::shareWithWeixinCircleTxt(kTitle,kUrl);
		}
#endif
	}

	void showWebView(const char* url )
	{

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME,"showWebView","(Ljava/lang/String;)V");
		if (isHave)
		{
			jstring jurl = minfo.env->NewStringUTF(url);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jurl);
			cocos2d::log("JniFun call showWebView over!");

			minfo.env->DeleteLocalRef(jurl);
			minfo.env->DeleteLocalRef(minfo.classID); 
			cocos2d::log("JniFun call showWebView over!111");
		}
		else
		{
			cocos2d::log("JniFun call showWebView error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		IosHelper::startBrowserJni(url);
#endif
	}

	// °æ±¾¸üÐÂ
	void versionUpdate(const char* url ,const char* desc, const int filesize, const int isUpdate)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;  
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME, "versionUpdate", "(Ljava/lang/String;Ljava/lang/String;II)V");
		if (isHave)  
		{  
			jstring jFileName = minfo.env->NewStringUTF(url);
			jstring jDes = minfo.env->NewStringUTF(desc);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jFileName,jDes, filesize, isUpdate); 

			cocos2d::log("JniFun call versionUpdate over!");

			minfo.env->DeleteLocalRef(jFileName);
			minfo.env->DeleteLocalRef(jDes);
			minfo.env->DeleteLocalRef(minfo.classID);  
		}  
		else
		{
			cocos2d::log("JniFun call versionUpdate error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		IosHelper::startBrowserJni(url);
#endif
	}
	std::string s_kRecordFileName;
	void startSoundRecord()
	{
		std::string kFileName = utility::toString(time(NULL),".wav");
		s_kRecordFileName = cocos2d::FileUtils::getInstance()->getWritablePath()+kFileName;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;  
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME, "startSoundRecord", "(Ljava/lang/String;)V");
		if (isHave)  
		{  
			jstring jurl = minfo.env->NewStringUTF(kFileName.c_str());
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jurl); 
			cocos2d::log("JniFun call startSoundRecord over!");

			minfo.env->DeleteLocalRef(minfo.classID);  
		}  
		else
		{
			cocos2d::log("JniFun call startSoundRecord error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		IosHelper::beginRecord(s_kRecordFileName.c_str());
#endif
	}
	const char* stopSoundRecord()
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		std::string str;
		JniMethodInfo minfo;  
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME, "stopSoundRecord", "()Ljava/lang/String;");
		if (isHave)  
		{  
			jstring jFileName = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID); 
			const char *newStr = minfo.env->GetStringUTFChars(jFileName, 0);
			str = newStr;
			cocos2d::log("JniFun call stopSoundRecord over :");
			cocos2d::log("%s",str.c_str());
			minfo.env->ReleaseStringUTFChars(jFileName, newStr);
			minfo.env->DeleteLocalRef(minfo.classID); 
		}  
		else
		{
			cocos2d::log("JniFun call stopSoundRecord error!");
		}
		return str.c_str();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		IosHelper::endRecord();
		return s_kRecordFileName.c_str();
#endif
		return "";
	}
	void startWeiXinPay(std::string kStr)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;  
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME, "startWeiXinPay", "(Ljava/lang/String;)V");
		if (isHave)  
		{  
			jstring jurl = minfo.env->NewStringUTF(kStr.c_str());
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jurl); 
			cocos2d::log("JniFun call startWeiXinPay over!");

			minfo.env->DeleteLocalRef(minfo.classID);  
		}  
		else
		{
			cocos2d::log("JniFun call startSoundRecord error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		IosHelper::WeiXinPay(kStr.c_str());
#endif
	}
	void startHuiPay(std::string kStr)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		JniMethodInfo minfo;  
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME, "startHuiPay", "(Ljava/lang/String;)V");
		if (isHave)  
		{  
			jstring jurl = minfo.env->NewStringUTF(kStr.c_str());
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jurl); 
			cocos2d::log("JniFun call startWeiXinPay over!");

			minfo.env->DeleteLocalRef(minfo.classID);  
		}  
		else
		{
			cocos2d::log("JniFun call startSoundRecord error!");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        IosHelper::HuiPay(kStr.c_str());
#endif
    }

	void startGPS()
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		std::string str;
		JniMethodInfo minfo;  
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME, "startGPS", "()Ljava/lang/String;");
		if (isHave)  
		{  
			jstring jFileName = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID); 
			const char *newStr = minfo.env->GetStringUTFChars(jFileName, 0);
			str = newStr;
			minfo.env->ReleaseStringUTFChars(jFileName, newStr);
			minfo.env->DeleteLocalRef(minfo.classID); 
		}  
		else
		{
			cocos2d::log("JniFun call startGPS error!");
		}
#endif
	}

	std::string getGPSLocation()
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		std::string str;
		JniMethodInfo minfo;  
		bool isHave = JniHelper::getStaticMethodInfo(minfo,JAVA_CLASSNAME, "getGPSLocation", "()Ljava/lang/String;");
		if (isHave)  
		{  
			jstring jFileName = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID); 
			const char *newStr = minfo.env->GetStringUTFChars(jFileName, 0);
			str = newStr;
			minfo.env->ReleaseStringUTFChars(jFileName, newStr);
			minfo.env->DeleteLocalRef(minfo.classID);
			return str;
		}  
		else
		{
			cocos2d::log("JniFun call getGPSLocation error!");
		}
#endif
		return "Error";
	}
} 

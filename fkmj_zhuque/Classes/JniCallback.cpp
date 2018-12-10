#include "JniCallback.h"
#include "cocos2d.h"
#include "GameLib.h"
#include "MissionWeiXin.h"
#include "PayMananger.h"



#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <Jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

using namespace cocos2d;

namespace JniFun
{
	extern "C" 
	{

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		 
		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_WxLoginGetAccessToken (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			MissionWeiXin::Instance().Req_Access_token(data);
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_WxLoginGetFailToken (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			MissionWeiXin::Instance().Req_Fail_token(data);
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_WxPayResp (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			PayMananger::Instance().jni_WeiXinResoult(data);
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_HuiPayResp (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			PayMananger::Instance().jni_HuiResoult(data);
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_GPSGetPos (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			UserInfo::Instance().modeGPS(data);
			env->ReleaseStringUTFChars(param, data);
		}
#endif
	}

} 
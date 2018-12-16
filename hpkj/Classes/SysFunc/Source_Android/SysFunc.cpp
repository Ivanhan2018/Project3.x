#include "SysFunc.h"
#include "Encrypt.h"

void SysFunc::ToPortrait()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	GLView* eglView = Director::getInstance()->getOpenGLView();	
	eglView->setViewName("QiXing");
	eglView->setFrameSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	eglView->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT, kResolutionExactFit);
#endif
	//ÇÐ»»ÊúÆÁ´úÂë 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	cocos2d::Size frameSize = pEGLView->getFrameSize();
	JniMethodInfo minfo;
	if( JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","changedActivityOrientation","(I)V") )
	{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,2);
	}
	pEGLView->setFrameSize(frameSize.height,frameSize.width);
	pEGLView->setDesignResolutionSize(SCREEN_WIDTH, SCREEN_HEIGHT,kResolutionExactFit);
#endif
}

void SysFunc::ToLandscape()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	//MyConfig::Instance().LoadData("xml/MyConfig.xml");
	GLView* eglView = Director::getInstance()->getOpenGLView();
	eglView->setViewName("Baccarat");
	eglView->setFrameSize(SCREEN_HEIGHT,SCREEN_WIDTH);
	eglView->setDesignResolutionSize(SCREEN_HEIGHT,SCREEN_WIDTH,kResolutionExactFit);
#endif
	//ÇÐ»»ºáÆÁ´úÂë
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	cocos2d::Size frameSize = pEGLView->getFrameSize(); 
	JniMethodInfo minfo;
	if( JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","changedActivityOrientation","(I)V") )
	{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,1);
	}
	pEGLView->setFrameSize(frameSize.height,frameSize.width);
	pEGLView->setDesignResolutionSize(SCREEN_HEIGHT,SCREEN_WIDTH,kResolutionExactFit);
#endif
}

//mac address
std::string SysFunc::WHGetMacAddress()
{
#ifdef WIN32
    {
		std::string ret("a123456");
		ret = CMD5Encrypt::MD5Encrypt(ret);
		return ret;
    }
#endif   
    //°²×¿Éè±¸
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    std::string ret("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getMachineID", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }
    ret = CMD5Encrypt::MD5Encrypt(ret);
    return ret;
#endif
}

LONGLONG SysFunc::getsystemtomillisecond()
{
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    
    LONGLONG time = tv.tv_sec*1000 + tv.tv_usec/1000;
    return time;
}

time_t SysFunc::getTime()
{
	time_t tp;
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	time(&tp);
	return tp;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	timeval now;
	gettimeofday(&now, NULL);
	tp = now.tv_sec;
	return tp;
#endif
}

std::string SysFunc::getTimeStr(time_t timesec,int flag)
{
    struct tm* ptime = localtime(&timesec);
	int year = (int)ptime->tm_year+1900;
	int mon = (int)ptime->tm_mon+1;
	int day = (int)ptime->tm_mday;
	int min = (int)ptime->tm_min;
	int hour = (int)ptime->tm_hour;
	int sec = (int)ptime->tm_sec;
	if(flag==0)
	{
		String str;
		str.initWithFormat("%d/%02d/%02d",year,mon,day);
		return str.getCString();	
	}
	else if(flag==1)
	{
		String str;
		str.initWithFormat("%02d:%02d:%02d",hour,min,sec);
		return str.getCString();	
	}
	else
	{
		std::string timestr =cocos2d::__String::createWithFormat("%d%02d%02d%02d%02d%02d", ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday, ptime->tm_hour, ptime->tm_min, ptime->tm_sec)->getCString();
		return timestr;	
	}
}

std::string SysFunc::getDateNow()
{
	time_t timesec = getTime();
	string str = getTimeStr(timesec,2);
	return str;
}
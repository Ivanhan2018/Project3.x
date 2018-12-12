#include "SysFunc.h"
#include "Define.h"

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
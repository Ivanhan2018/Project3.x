#include "DDZJniControl.h"
#include "DDZDDZRes.h"
#include "DDZRes.h"//<by hxh>
#include "DDZSceneControl.h"
#include "GlobalUnits.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JniHelper.h"
#endif

DDZJniControl::DDZJniControl(void)
{
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZJniControl::JniC_MsgToAndroid),MESSAGE_JNICONTROL,NULL);
}


DDZJniControl::~DDZJniControl(void)
{
	NotificationCenter::getInstance()->removeObserver(this, MESSAGE_JNICONTROL);
}

DDZJniControl* DDZJniControl::sharedDDZJniControl()
{
	static DDZJniControl* s_GniControl=NULL;
	if (!s_GniControl)
	{
		s_GniControl = new DDZJniControl();
	}
	return s_GniControl;
}

void DDZJniControl::JniC_MsgToAndroid( Object* obj )
{
	Integer* _int=(Integer*)obj;
	//CCAssert(_int, "JniC_MsgToAndroid obj=null");

	switch(_int->getValue())
	{
	case MSG_JNI_Share:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
	//
#endif
		break;

	case MSG_JNI_Task:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
	//
#endif
		break;
	case MSG_JNI_Start:
		break;
	case MSG_JNI_Recharge:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//低等级 = 1 中等级 = 2 高等级 = 3 有X = 1 没X = 2
		if(g_GlobalUnits.getGolbalUserData()->lScore < ConfigMgr::instance()->m_Config.nServerEnter[(((1-1) % 3 ) << 1) + 1])
		{
			showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","5");
		}
		else if(g_GlobalUnits.getGolbalUserData()->lScore < ConfigMgr::instance()->m_Config.nServerEnter[(((2-1) % 3 ) << 1) + 1])
		{
			showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"2","5");
		}
		else
		{
			showAndroidRechageLayer(CCString::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"3","5");
		}
#else
		//NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATESCOREANDGOLDEGG,NULL);  刷新钱和花费点
#endif
		break;
	}
}
DDZJniControl* pDDZJni=DDZJniControl::sharedDDZJniControl();
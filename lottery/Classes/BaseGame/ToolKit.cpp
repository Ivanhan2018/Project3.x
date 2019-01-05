#include "ToolKit.h"
#include "GlobalDef.h"
#include "MyConfig.h"
#include "MyNSString.h"
#include "ui/CocosGUI.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AppController.h"
#endif

using namespace ui;

ToolKit::ToolKit()
{
}

ToolKit::~ToolKit()
{
}

void ToolKit::toPortrait()
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
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//ÊúÆÁ
	[AppController changeRootViewControllerV];
	//ÖØÐÂÊÊÅä
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	//»ñÈ¡ÆÁÄ»·Ö±æÂÊ
	CGRect tempRect = [UIScreen mainScreen].bounds;
	CGFloat scale_screen = [UIScreen mainScreen].scale;
	pEGLView->setFrameSize(tempRect.size.width * scale_screen,tempRect.size.height * scale_screen);
	pEGLView->setDesignResolutionSize(SCREEN_WIDTH,SCREEN_HEIGHT,kResolutionExactFit);
#endif
}

void ToolKit::toLandscape()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	MyConfig::Instance().LoadData("MyConfig.xml");
	GLView* eglView = Director::getInstance()->getOpenGLView();
	eglView->setViewName("Baccarat");
	eglView->setFrameSize(SCREEN_HEIGHT*0.8,SCREEN_WIDTH*0.8);
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
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//ºáÆÁ
	[AppController changeRootViewControllerH];
	//ÖØÐÂÊÊÅä
	GLView *pEGLView = Director::getInstance()->getOpenGLView();
	//»ñÈ¡ÆÁÄ»·Ö±æÂÊ
	CGRect tempRect = [UIScreen mainScreen].bounds;
	CGFloat scale_screen = [UIScreen mainScreen].scale;
	pEGLView->setFrameSize(tempRect.size.width * scale_screen,tempRect.size.height * scale_screen);
	pEGLView->setDesignResolutionSize(SCREEN_HEIGHT,SCREEN_WIDTH,kResolutionExactFit);
#endif
}

//	Ëæ»úÉú³ÉÒ»¸öêÇ³Æ
string ToolKit::generateNickname()
{

	char cFront[128] = {};
	int i = 0;
	do 
	{
		int nNum = random(97, 122);
		sprintf(&cFront[i], "%c", nNum);
		if (i == 4)
		{
			break;
		}
		i++;
	} while (1);

	int nLast = random(100, 999);
	char cLastBuf[5] = {};
	sprintf(cLastBuf, "%d", nLast);
	string sResult = cFront;
	sResult.append(cLastBuf);

	return sResult;

/*
	int nNum = random(0, 9);
	string sNickNameBuf[10] = 
	{
		"adffd123",
		"man001",
		"hellooo",
		"Lion00",
		"liii11",
		"123321hhh",
		"luckyboy",
		"wuliao122",
		"nicheng123",
		"hdhdhd"
	};
	return sNickNameBuf[nNum];
*/
}
void ToolKit::showTips(string sTips, float fDuration)
{
	Scene* pCurrScene = Director::getInstance()->getRunningScene();
	if (!pCurrScene)
	{
		return;
	}
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	LayerColor* pLayer = LayerColor::create(Color4B::GRAY);
	pCurrScene->addChild(pLayer);
	pLayer->setPosition(Vec2(sVSize / 2));
	Label* pTips = Label::create();
	pLayer->addChild(pTips);
	pTips->setString(sTips);
	pTips->setSystemFontSize(35);
	pTips->setColor(Color3B::GREEN);
	pLayer->setContentSize(pTips->getContentSize());
	pTips->setPosition(Vec2(pLayer->getContentSize() / 2));

	auto pDelay = DelayTime::create(fDuration);
	auto pRemove = RemoveSelf::create();
	auto pSeq = Sequence::create(pDelay, pRemove, nullptr);
	pLayer->runAction(pSeq);
}

void ToolKit::studioUISingle(Node* pParent, Node* pChild)
{

	auto pDelay = DelayTime::create(2);
	auto pCall = CallFunc::create([=]
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		listener->onTouchBegan = [=](Touch* touch, Event* event){
			if(!pChild){
				return false;
			}
			Vec2 pTPos = pParent->convertToNodeSpace(touch->getLocation());
// 			Vec2 pTPos = touch->getLocation();
			if (pChild->boundingBox().containsPoint(pTPos))
			{
				return true;
			}
			return false;
		};
		listener->onTouchMoved = [=](Touch* touch, Event* event){
			pChild->setPosition(pChild->getPosition() + touch->getDelta());
			Vec2 vPos = pChild->getPosition();
			cocos2d::Size sParentSize = pParent->getContentSize();
			float fPercentX = vPos.x / sParentSize.width;
			float fPercentY = vPos.y / sParentSize.height;
			log("pos  Vec2(%f,%f);", vPos.x, vPos.y);
			log("percent	(X = %f, Y = %f)", fPercentX, fPercentY);
		};
		listener->onTouchEnded = [=](Touch* touch, Event* event){
			pChild->setPosition(pChild->getPosition() + touch->getDelta());
			Vec2 vPos = pChild->getPosition();
			cocos2d::Size sParentSize = pParent->getContentSize();
			float fPercentX = vPos.x / sParentSize.width;
			float fPercentY = vPos.y / sParentSize.height;
			log("pos  Vec2(%f,%f);", vPos.x, vPos.y);
			log("percent	(X = %f, Y = %f)", fPercentX, fPercentY);
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pParent);

	});
	pParent->runAction(Sequence::create(pDelay, pCall, nullptr));

}

void ToolKit::studioUIMulti(Node* pParent, std::vector<Node*> vChildren)
{
	static Node* pCurrChild = nullptr;
	pCurrChild = nullptr;

	auto pDelay = DelayTime::create(2);
	auto pCall = CallFunc::create([=]
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);

		//	´¥Ãþ¿ªÊ¼
		listener->onTouchBegan = [=](Touch* touch, Event* event)
		{
			for (auto k : vChildren)
			{
				Node* pChild = dynamic_cast<Node*>(k);
				if (!pChild)
				{
					continue;
				}
// 				Vec2 pTPos = touch->getLocation();
				Vec2 pTPos = pParent->convertToNodeSpace(touch->getLocation());
				if (pChild->getBoundingBox().containsPoint(pTPos))
				{
					pCurrChild = pChild;
					return true;
				}
			}
			return false;
		};

		//	´¥ÃþÒÆ¶¯
		listener->onTouchMoved = [=](Touch* touch, Event* event)
		{
			pCurrChild->setPosition(pCurrChild->getPosition() + touch->getDelta());

			cocos2d::Size sParentSize = pParent->getContentSize();
			Vec2 vNodePos = pCurrChild->getPosition();
			Vec2 vWorldPos = pCurrChild->convertToNodeSpace(vNodePos);
			float fPercentX = vNodePos.x / sParentSize.width;
			float fPercentY = vNodePos.y / sParentSize.height;
			log("NodePos	Vec2(%f,%f);", vNodePos.x, vNodePos.y);
			log("WorldPos	Vec2(%f,%f);", vWorldPos.x, vWorldPos.y);
			log("PercentPos	(X = %f, Y = %f)", fPercentX, fPercentY);
		};

		//	´¥Ãþ½áÊø
		listener->onTouchEnded = [=](Touch* touch, Event* event)
		{
			pCurrChild->setPosition(pCurrChild->getPosition() + touch->getDelta());

			cocos2d::Size sParentSize = pParent->getContentSize();
			Vec2 vNodePos = pCurrChild->getPosition();
			Vec2 vWorldPos = pCurrChild->convertToNodeSpace(vNodePos);
			float fPercentX = vNodePos.x / sParentSize.width;
			float fPercentY = vNodePos.y / sParentSize.height;
			log("NodePos	Vec2(%f,%f);", vNodePos.x, vNodePos.y);
			log("WorldPos	Vec2(%f,%f);", vWorldPos.x, vWorldPos.y);
			log("PercentPos	(X = %f, Y = %f)", fPercentX, fPercentY);
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pParent);

	});

	pParent->runAction(Sequence::create(pDelay, pCall, nullptr));

}

string ToolKit::dealStringOfPrivacy(const string &sString)
{
	string str;
	char16_t x[]={'*','*','*','*',0};
	std::u16string wstrUTF16;
	std::u16string wstr1=x;
	bool bRet1=StringUtils::UTF8ToUTF16(sString,wstrUTF16);
	int nLength = wstrUTF16.length();
	int nChangeLen = 4;
	if (nLength <= nChangeLen)
	{
		nChangeLen = nLength;
	}
	wstrUTF16=wstrUTF16.substr(0,nChangeLen)+wstr1;

	bool bRet2=StringUtils::UTF16ToUTF8(wstrUTF16,str);

	return str;
}

void ToolKit::toShowInfo()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[AppController changeRootViewShowInfo];
#endif

	//ÏÔÊ¾
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	if( JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","changedActivityOrientation","(I)V") )
	{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,4);
	}
#endif
}

void ToolKit::toDontShowInfo()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[AppController changeRootViewDontShowInfo];
#endif

	//²»ÏÔÊ¾×´Ì¬À¸
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	if( JniHelper::getStaticMethodInfo(minfo,"org.cocos2dx.cpp.AppActivity","changedActivityOrientation","(I)V") )
	{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,3);
	}	
#endif
}
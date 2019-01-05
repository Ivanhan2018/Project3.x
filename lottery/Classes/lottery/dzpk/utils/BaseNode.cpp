#include "BaseNode.h"

//#include "popupTip/PopupTipLayer.h"
//#include "GlobalSchedule/GlobalSchedule.h"

BaseNode::BaseNode():
pStartBgTexture(NULL)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	this->visibleW = visibleSize.width;
	this->visibleH = visibleSize.height;
	this->originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
    
    pStartBgTexture = CCTextureCache::sharedTextureCache()->addImage("background/background.png");
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("start/start_scene_ui.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("common/common_res.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hall/hall_res.plist");
}

BaseNode::~BaseNode()
{
    
}

bool BaseNode::init()
{
	bool bRec = false;

	do 
	{
		CC_BREAK_IF(!Layer::create());
		
		this->initUI();

		bRec = true;

	} while (0);

	return bRec;
}

void BaseNode::initUI()
{
	this->setKeypadEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    this->setContentSize(CCSizeMake(this->visibleW, this->visibleH));
}

void BaseNode::setBackground(CCTexture2D* pStartBgTexture)
{
	CCSprite* pBgSprite = CCSprite::createWithTexture(pStartBgTexture);
    
    CCSize bgSpriteSize = pBgSprite->getContentSize();
    pBgSprite->setScaleY((float)this->visibleH/(float)bgSpriteSize.height);
    
	pBgSprite->setAnchorPoint(ccp(0,0));
	pBgSprite->setPosition(this->originPoint);
	this->addChild(pBgSprite);
}

//Loading* BaseNode::createLoading()
//{
//    pLoading = Loading::create();
//    //CC_SAFE_RETAIN(pLoading);
//    pLoading->setAnchorPoint(ccp(0,0));
//    pLoading->setPosition(this->originPoint);
//    this->addChild(pLoading,200);
//    return pLoading;
//}

void BaseNode::stopLoading()
{
    //CC_SAFE_RELEASE_NULL(pLoading);
    //pLoading->removeFromParentAndCleanup(true);
}

/*const char* BaseNode::getStrFromXml(const char *key)
{
    CCDictionary* pDictionary = CCDictionary::createWithContentsOfFile("language/strings_zhCN.xml");
    return ((CCString*)pDictionary->objectForKey(key))->m_sString.c_str();
}*/

//void BaseNode::keyBackClicked()
//{
//	
//}
//
//void BaseNode::keyMenuClicked()
//{
//	
//}

void BaseNode::exitGame()
{
	CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("start/start_scene_ui.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("common/common_res.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("hall/hall_res.plist");
    
	CCDirector::sharedDirector()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
    
    //关闭全局的定时器
    //GlobalSchedule::stop();
}

void BaseNode::createPopupTipLayer(const char* title,const char* content,CCVerticalTextAlignment vTextAlignment,CCTextAlignment hTextAlignment,TIP_TAG currTipTag)
{
    this->currTipTag = currTipTag;
    
 //   PopupTipLayer* popupTipLayer = PopupTipLayer::create();
 //   
 //   popupTipLayer->setTitle(title);
 //   popupTipLayer->setContent(content,vTextAlignment,hTextAlignment);
	//popupTipLayer->setCallbackFunc(this, callfuncN_selector(BaseNode::popupTipLayerCallback));
 //   popupTipLayer->addMenu("common/pop_btn_right_1.png", "common/pop_btn_right_1.png","确 定",CCPointMake(popupTipLayer->contentSize.width-168,11),POPUP_CTRLBTN_OK_TAG);
 //   popupTipLayer->addMenu("common/pop_btn_left_1.png", "common/pop_btn_left_1.png","取 消",CCPointMake(10,11),POPUP_CTRLBTN_CANCEL_TAG);
	//popupTipLayer->setAnchorPoint(ccp(0,0));
	//popupTipLayer->setPosition(this->originPoint);
 //   
	//this->addChild(popupTipLayer,100);
}

void BaseNode::createPopupTipLayer(const char* title,const char* content,TIP_TAG currTipTag)
{
    this->currTipTag = currTipTag;
    
 //   PopupTipLayer* popupTipLayer = PopupTipLayer::create();
 //   
 //   popupTipLayer->setTitle(title);
 //   popupTipLayer->setContent(content);
	//popupTipLayer->setCallbackFunc(this, callfuncN_selector(BaseNode::popupTipLayerCallback));
 //   popupTipLayer->addMenu("common/pop_btn_right_1.png", "common/pop_btn_right_1.png","确 定",CCPointMake(popupTipLayer->contentSize.width-168,11),POPUP_CTRLBTN_OK_TAG);
 //   popupTipLayer->addMenu("common/pop_btn_left_1.png", "common/pop_btn_left_1.png","取 消",CCPointMake(10,11),POPUP_CTRLBTN_CANCEL_TAG);
	//popupTipLayer->setAnchorPoint(ccp(0,0));
	//popupTipLayer->setPosition(this->originPoint);
 //   
	//this->addChild(popupTipLayer,100);
}

void BaseNode::createToastLayer(const char* msg,float delay)
{
    CCLabelTTF* tip = CCLabelTTF::create(msg, "Arial", 15);
    CCSize tipSize = tip->getContentSize();
    
    CCSize toastSize = CCSizeMake(tipSize.width+20, 30);
    CCLayerColor* pToastLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), toastSize.width, toastSize.height);
    pToastLayer->setAnchorPoint(ccp(0,0));
    pToastLayer->setPosition(ccp(this->originPoint.x+this->visibleW/2-toastSize.width/2,this->originPoint.y+this->visibleH/2-toastSize.height/2));
    
    tip->setAnchorPoint(ccp(0,0));
    tip->setPosition(ccp(toastSize.width/2-tipSize.width/2,toastSize.height/2-tipSize.height/2));
    pToastLayer->addChild(tip);
    
    this->addChild(pToastLayer,50);
    
    pToastLayer->runAction(CCSequence::create(CCFadeIn::create(0.5f),CCDelayTime::create(delay-1),CCFadeOut::create(0.5f),CCCallFuncN::create(this, callfuncN_selector(BaseNode::showToastCallback)),NULL));
}

void BaseNode::createToastLayer(const char* msg,float delay,CCPoint point)
{
    CCLabelTTF* tip = CCLabelTTF::create(msg, "Arial", 15);
    CCSize tipSize = tip->getContentSize(); 
    
    CCSize toastSize = CCSizeMake(tipSize.width+20, 30);
    CCLayerColor* pToastLayer = CCLayerColor::create(ccc4(0, 0, 0, 255), toastSize.width, toastSize.height);
    pToastLayer->setAnchorPoint(ccp(0,0));
    pToastLayer->setPosition(point);
    
    tip->setAnchorPoint(ccp(0,0));
    tip->setPosition(ccp(toastSize.width/2-tipSize.width/2,toastSize.height/2-tipSize.height/2));
    pToastLayer->addChild(tip);
    
    this->addChild(pToastLayer,50);
    
    pToastLayer->runAction(CCSequence::create(CCFadeIn::create(0.5f),CCDelayTime::create(delay-1),CCFadeOut::create(0.5f),CCCallFuncN::create(this, callfuncN_selector(BaseNode::showToastCallback)),NULL));
}

void BaseNode::showToastCallback(CCNode* pSender)
{
    pSender->removeFromParentAndCleanup(true);
}













#include "PopupTipLayer.h"

PopupTipLayer::PopupTipLayer()
{
    
}

PopupTipLayer::~PopupTipLayer()
{
    
}

bool PopupTipLayer::init()
{
    bool bRef = false;
    
    do{
        CC_BREAK_IF(!PopupNode::init());
        
        this->initUI();
        
        bRef = true;
    }while(0);
    
    return bRef;
}

void PopupTipLayer::initUI()
{
	CCSprite* bgSprite = CCSprite::create("common/comm_pop_frame.png");

    this->contentSize = bgSprite->getContentSize();
    this->pContentNode->setContentSize(this->contentSize);
    this->pContentNode->setAnchorPoint(ccp(0.5f, 0.5f));
	this->pContentNode->setPosition(ccp(this->visibleW/2,this->visibleH/2));
    
    bgSprite->setAnchorPoint(Vec2::ZERO);
    bgSprite->setPosition(Vec2::ZERO);
    this->pContentNode->addChild(bgSprite);
}

void PopupTipLayer::setTitle(const char* title)
{
    CCLabelTTF* pLabelTitle = CCLabelTTF::create(title,"Arial-BoldMT",25);
	pLabelTitle->setAnchorPoint(Vec2::ZERO);
	CCSize size = pLabelTitle->getContentSize();
	pLabelTitle->setPosition(ccp(contentSize.width/2-size.width/2,contentSize.height-63));
	this->pContentNode->addChild(pLabelTitle);
}

void PopupTipLayer::setContent(const char* content)
{
    CCSize ttfLayerSize = CCSizeMake(this->contentSize.width-80, this->contentSize.height-130);
    CCLayerColor* ttfLayer = CCLayerColor::create(ccc4(255, 0, 0, 0), ttfLayerSize.width, ttfLayerSize.height);
    ttfLayer->setAnchorPoint(Vec2::ZERO);
    ttfLayer->setPosition(ccp(this->contentSize.width/2-ttfLayerSize.width/2, this->contentSize.height/2-ttfLayerSize.height/2));
    this->pContentNode->addChild(ttfLayer);
    
    CCLabelTTF* pLabelContent = CCLabelTTF::create(content,"Arial",20);
    pLabelContent->setDimensions(ttfLayerSize);
    pLabelContent->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    pLabelContent->setHorizontalAlignment(kCCTextAlignmentCenter);
	pLabelContent->setAnchorPoint(ccp(0.5,0.5));
	pLabelContent->setPosition(ccp(ttfLayerSize.width/2,ttfLayerSize.height/2));
	ttfLayer->addChild(pLabelContent);
}

void PopupTipLayer::setContent(const char* content,CCVerticalTextAlignment vTextAlignment,CCTextAlignment hTextAlignment)
{
    CCSize ttfLayerSize = CCSizeMake(this->contentSize.width-60, this->contentSize.height-130);
    CCLayerColor* ttfLayer = CCLayerColor::create(ccc4(255, 0, 0, 0), ttfLayerSize.width, ttfLayerSize.height);
    ttfLayer->setAnchorPoint(Vec2::ZERO);
    ttfLayer->setPosition(ccp(this->contentSize.width/2-ttfLayerSize.width/2, this->contentSize.height/2-ttfLayerSize.height/2));
    this->pContentNode->addChild(ttfLayer);
    
    CCLabelTTF* pLabelContent = CCLabelTTF::create(content,"Arial",20);
    pLabelContent->setDimensions(ttfLayerSize);
    pLabelContent->setVerticalAlignment(vTextAlignment);
    pLabelContent->setHorizontalAlignment(hTextAlignment);
	pLabelContent->setAnchorPoint(ccp(0.5,0.5));
	pLabelContent->setPosition(ccp(ttfLayerSize.width/2,ttfLayerSize.height/2));
	ttfLayer->addChild(pLabelContent);
}

void PopupTipLayer::menuCallback(cocos2d::CCObject *pSender)
{
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    //DLog::showLog("touch tag: %d", node->getTag());
    
    if (m_callback && m_callbackListener)
    {
        (m_callbackListener->*m_callback)(node);
    }
    
    Sound::getInstance()->playEffect("sound/sound_guanbi.ogg");
    
    CCActionInterval * easeBounceOut = CCEaseElasticOut::create(CCScaleTo::create(0.2f,0.0f,0.0f));
    CallFunc* call = CallFunc::create(CC_CALLBACK_0(PopupTipLayer::closeCallback,this));
	pContentNode->runAction(CCSequence::create(easeBounceOut,call,NULL));

}

void PopupTipLayer::closeCallback(/*CCObject* pSender*/)
{
    this->removeFromParentAndCleanup(true);
}

void PopupTipLayer::onEnter()
{
    Layer::onEnter();
    
    Sound::getInstance()->playEffect("sound/sound_chuangkou.ogg");
    
    // 弹出效果
	pContentNode->setScale(0.0f);
	CCActionInterval * easeBounceOut = CCEaseElasticOut::create(CCScaleTo::create(0.2f,1.0f,1.0f));
	pContentNode->runAction(easeBounceOut);
}

void PopupTipLayer::onExit()
{
    Layer::onExit();
}
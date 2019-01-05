#include "PopupNode.h"

PopupNode::PopupNode():
pMenu(NULL),
m_callbackListener(NULL),
m_callback(NULL),
pContentNode(NULL),
bgColorLayer(NULL)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	this->visibleW = visibleSize.width;
	this->visibleH = visibleSize.height;
	this->originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
}

PopupNode::~PopupNode()
{
	CC_SAFE_RELEASE_NULL(pContentNode);
	CC_SAFE_RELEASE_NULL(bgColorLayer);
    CC_SAFE_RELEASE_NULL(pMenu);
}

//void PopupNode::registerWithTouchDispatcher()
//{
//    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//    pDispatcher->addTargetedDelegate(this, POPUP_LAYER_TOUCH_PRIORITY, true);
//}

//bool PopupNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//    return true;
//}

bool PopupNode::init()
{
	bool bRef = false;

	do{
		CC_BREAK_IF(!Layer::init());

		this->setTouchEnabled(true);

		this->initUI();

		bRef = true;
	}while(0);

	return bRef;
}

void PopupNode::initUI()
{
	this->setContentSize(CCSizeMake(this->visibleW, this->visibleH));
    
	bgColorLayer = CCLayerColor::create(ccc4(0, 0, 0, 200), this->visibleW, this->visibleH);
    CC_SAFE_RETAIN(bgColorLayer);
	bgColorLayer->setAnchorPoint(Vec2::ZERO);
	bgColorLayer->setPosition(Vec2::ZERO);
	this->addChild(bgColorLayer);
    
    //使下一层的触摸无效
    ControlButton* ctrlBgBtn = CtrlManager::createCtrlBtnWithFile("popup/transparent_bg.png","",CCSizeMake(this->visibleW, this->visibleH),CTRL_BTN_TOUCH_PRIORITY);
    //ctrlBgBtn->setScaleX(this->visibleW);
    //ctrlBgBtn->setScaleY(this->visibleH);
    //ctrlBgBtn->setContentSize(CCSizeMake(this->visibleW, this->visibleH));
    ctrlBgBtn->setAnchorPoint(Vec2::ZERO);
    ctrlBgBtn->setPosition(Vec2::ZERO);
    ctrlBgBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(PopupNode::ctrlBtnCallbck), Control::EventType::TOUCH_UP_INSIDE);
    ctrlBgBtn->setTag(POPUP_ALLWAY_TAG);
    bgColorLayer->addChild(ctrlBgBtn,0);
    
    pContentNode = CCNode::create();
    CC_SAFE_RETAIN(pContentNode);
    this->addChild(pContentNode,1);

	// 初始化需要的 Menu
	this->pMenu = CCMenu::create();
    CC_SAFE_RETAIN(pMenu);
	this->pMenu->setPosition(Vec2::ZERO);
	this->pMenu->setPosition(Vec2::ZERO);
	pContentNode->addChild(this->pMenu,2);
    
}

void PopupNode::setBgColorOpacity(UNS_INT opacity)
{
    bgColorLayer->setOpacity(opacity);
}

void PopupNode::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun)
{
    m_callbackListener = target;
    m_callback = callfun;
}

bool PopupNode::addMenu(CCSprite* normalSprite, CCSprite* selectedSprite,CCPoint& point,int tag)
{
	selectedSprite->setColor(ccc3(131,131,131));
    CCSize size = CCSizeMake(normalSprite->getContentSize().width*1.5f,normalSprite->getContentSize().height*1.5f);
    normalSprite->setContentSize(size);
    selectedSprite->setContentSize(CCSizeMake(normalSprite->getContentSize().width*1.5f,normalSprite->getContentSize().height*1.5f));
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(PopupNode::menuCallback,this));
    
	menuItem->setTag(tag);
	menuItem->setAnchorPoint(Vec2::ZERO);
	menuItem->setPosition(point);
	this->pMenu->addChild(menuItem);
    
	return true;
}

bool PopupNode::addMenu(const char *normalImage, const char *selectedImage,CCPoint& point,int tag)
{
	CCSprite* nSprite = CCSprite::create(normalImage);
	CCSprite* sSprite = CCSprite::create(selectedImage);
	return this->addMenu(nSprite, sSprite,point,tag);
}

bool PopupNode::addMenu(const char *normalImage, const char *selectedImage,const char* title,CCPoint point,int tag,int fontSize)
{
    CCSprite* normalSprite = CCSprite::create(normalImage);
	CCSprite* selectedSprite = CCSprite::create(selectedImage);
    
    selectedSprite->setColor(ccc3(131,131,131));
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(PopupNode::menuCallback,this));
    
	menuItem->setTag(tag);
	menuItem->setAnchorPoint(Vec2::ZERO);
	CCSize menuSize = normalSprite->getContentSize();
	menuItem->setPosition(point);
	this->pMenu->addChild(menuItem);
    
    CCLabelTTF* titleTTF = CCLabelTTF::create(title, "Arial", fontSize);
    titleTTF->setAnchorPoint(ccp(0.5f,0.5f));
    CCSize titleSize = titleTTF->getContentSize();
    titleTTF->setPosition(ccp(menuSize.width/2,menuSize.height/2));
    menuItem->addChild(titleTTF,10);
    
	return true;
}

bool PopupNode::addMenu(const char *normalImage, const char *selectedImage,const char* title,CCPoint point,int tag,CCPoint textPoint,int fontSize)
{
    CCSprite* normalSprite = CCSprite::create(normalImage);
	CCSprite* selectedSprite = CCSprite::create(selectedImage);
    
    selectedSprite->setColor(ccc3(131,131,131));
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(PopupNode::menuCallback,this));
    
	menuItem->setTag(tag);
	menuItem->setAnchorPoint(Vec2::ZERO);
	CCSize menuSize = normalSprite->getContentSize();
	menuItem->setPosition(point);
	this->pMenu->addChild(menuItem);
    
    CCLabelTTF* titleTTF = CCLabelTTF::create(title, "Arial", fontSize);
    titleTTF->setAnchorPoint(Vec2::ZERO);
    titleTTF->setPosition(textPoint);
    menuItem->addChild(titleTTF,10);
    
	return true;
}

bool PopupNode::addMenu(const char *normalImage, const char *selectedImage,int tag)
{
	CCSprite* nSprite = CCSprite::create(normalImage);
	CCSprite* sSprite = CCSprite::create(selectedImage);
	return this->addMenu(nSprite, sSprite,tag);
}

bool PopupNode::addMenu(CCSprite* normalSprite, CCSprite* selectedSprite,int tag)
{
	selectedSprite->setColor(ccc3(131,131,131));
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(PopupNode::menuCallback,this));
    
	menuItem->setTag(tag);
	menuItem->setAnchorPoint(ccp(1.0f,1.0f));
	CCSize menuSize = normalSprite->getContentSize();
	menuItem->setPosition(ccp(this->contentSize.width+menuSize.width/4,this->contentSize.height+menuSize.height/4));
	this->pMenu->addChild(menuItem);
    
	return true;
}

void PopupNode::onEnter()
{
	Layer::onEnter();
}

void PopupNode::onExit()
{
	Layer::onExit();
}
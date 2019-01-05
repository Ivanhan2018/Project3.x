#include "PopupLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace std;
using namespace cocostudio;
using namespace ui;
//静态变量
PopupLayer * PopupLayer::popLayer=nullptr;					//对象指针
// 构造函数中变量设初值
PopupLayer::PopupLayer()
{
	popLayer = this;
	m_contentPadding = 0;
	m_contentPaddingTop = 0;
	m_pMenu = NULL;
	m_callback = nullptr;
	m_sfBackGround = NULL;
	//m_s9BackGround = NULL;
	m_ltContentText = NULL;
	m_ltTitle = NULL;
}
//释放
PopupLayer::~PopupLayer()
{
	CC_SAFE_RELEASE(m_pMenu);
	CC_SAFE_RELEASE(m_sfBackGround);
	//CC_SAFE_RELEASE(m_s9BackGround);
	CC_SAFE_RELEASE(m_ltContentText);
	CC_SAFE_RELEASE(m_ltTitle);
	popLayer = nullptr;
}
//初始化
bool PopupLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	this->setContentSize(cocos2d::Size(0, 0));
	//初始化需要的Menu
	ui::Button* menu = ui::Button::create();
	menu->setPosition(Vec2(0,0));
	setMenuButton(menu);  //set()方法
	setTouchEnabled(true);  //开启触摸响应

	//屏蔽底层触摸
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	return true;
}

//重写触摸注册函数，重新给定触摸级别
//void PopupLayer::registerWithTouchDispatcher(){
//
//	// 这里的触摸优先级设置为-128，与CCMenu同级，保证了屏蔽下方的触摸
//
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
//}

//触摸函数ccTouchBegan，返回true
bool PopupLayer::TouchBegan( Touch *pTouch, Event *pEvent )
{
	return true;
}
//创建一个弹出层，给背景精灵变量赋值
PopupLayer* PopupLayer::create( const char* backgroundImage )
{
	PopupLayer* popup = PopupLayer::create();
	auto tt = Sprite::create(backgroundImage);
	popup->setSpriteBackGround(tt);

	//cocos2d::Size x = tt->getContentSize();

	//popup->setSprite9BackGround(Scale9Sprite::create(backgroundImage));
	return popup;
}
//给标题变量赋值
void PopupLayer::setTitle( const char* title, int fontsize )
{
	CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "Arial", fontsize);
	ltfTitle->setColor(ccc3(250, 250, 250));
	setLabelTitle(ltfTitle);
}
//给文本变量赋值
void PopupLayer::setContentText( const char* text, int fontsize, int padding, int paddingTop )
{
	ui::Text* content = ui::Text::create(text, "Arial", fontsize);
	content->setColor(ccc3(250, 250, 250));
	setLabelContentText(content);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}
//给下层层变量和回调函数变量赋值
void PopupLayer::setCallBackFunc(const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack)
{
	m_callback = callBack;
}
//给menu菜单变量添加Item
bool PopupLayer::addButton( const char* normalImage, const char* selectedImage, int tag )
{
	cocos2d::Size winSize = CCDirector::sharedDirector()->getWinSize();
	Vec2 center = ccp(winSize.width/2, winSize.height/2);
	// 创建图片菜单按钮
	ui::Button* menuImage = ui::Button::create();
	menuImage->loadTextures(normalImage,selectedImage);//,"",CC_CALLBACK_2(PopupLayer::buttonCallBack,this));
	menuImage->setTag(tag);
	menuImage->setPosition(center);
	menuImage->addTouchEventListener(this,SEL_TouchEvent(&PopupLayer::buttonCallBack));
	getMenuButton()->addChild(menuImage);
	return true;
}
//销毁弹出框，传递参数node给下层
void PopupLayer::buttonCallBack(CCObject* pSender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	CCNode* node = dynamic_cast<Node*>(pSender);
	log("touch tag: %d", node->getTag());
	if (m_callback)
	{
		//执行HelloWorld层的回调函数，传递node参数
		m_callback(node,type);
	}
	this->removeFromParentAndCleanup(true);
}
//全部参数都设定好后，在运行时动态加载
void PopupLayer::onEnter()
{
	Layer::onEnter();
	cocos2d::Size winSize = Director::sharedDirector()->getWinSize();
	Vec2 center = Vec2(winSize.width/2, winSize.height/2);
	cocos2d::Size contentSize;
	// 设定好参数，在运行时加载
	if (getContentSize().equals(cocos2d::Size::ZERO))
	{
		getSpriteBackGround()->setPosition(center);
		this->addChild(getSpriteBackGround(),0,0);
		contentSize = getSpriteBackGround()->getContentSize();
	}
	else
	{
		ui::Scale9Sprite* background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(center);
		this->addChild(background, 0);
		contentSize = getContentSize();
	}
	//添加按钮，并根据Item的个数设置其位置
	this->addChild(getMenuButton());
	float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount()+1);
	Vector<Node*>array = getMenuButton()->getChildren();
	CCObject* pObj = NULL;
	int i = 0;
	for (Node* pObj : array)
	{
		CCNode* node = dynamic_cast<CCNode*>(pObj);
//		int w = winSize.width/2-(getMenuButton()->getChildrenCount()*node->getContentSize().width)/2+node->getContentSize().width*(i+0.5);//确定左
		int w = winSize.width/2+(getMenuButton()->getChildrenCount()*node->getContentSize().width)/2-node->getContentSize().width*(i+0.5);
		int h = winSize.height/2 - contentSize.height/3+20;
		node->setPosition(ccp( w, h));
		/*node->setPosition(ccp(winSize.width/2 - contentSize.width/2 + btnWidth*(i+1), 
			winSize.height/2 - contentSize.height/3));*/
		i++;
	}
	// 显示对话框标题
	if (getLabelTitle())
	{
		getLabelTitle()->setPosition(ccpAdd(center, ccp(0, contentSize.height/2 - 25.0f)));
		this->addChild(getLabelTitle());
	}
	//显示文本内容
	if (getLabelContentText())
	{
		//ui::Text* ffff = ui::Text::create()
		ui::Text* ltf = getLabelContentText();
		ltf->ignoreContentAdaptWithSize(false);
		ltf->setTextAreaSize(cocos2d::Size(contentSize.width - m_contentPadding*2,150));
		ltf->setPosition(ccpAdd(center, ccp(0,32)));
		//ltf->setDimensions(CCSizeMake(contentSize.width - m_contentPadding*2, contentSize.height - m_contentPaddingTop));
		ltf->setTextHorizontalAlignment(TextHAlignment::CENTER);
		ltf->setTextVerticalAlignment(TextVAlignment::CENTER);
		this->addChild(ltf);
	}
#if 1
	//弹出效果
	/*CCSequence *popupActions = CCSequence::create(
	CCScaleTo::create(0.0, 0.0), 
	CCScaleTo::create(0.06, 1.05),
	CCScaleTo::create(0.08, 0.95),
	CCScaleTo::create(0.08, 1.0), NULL);*/
	MoveTo* mt = MoveTo::create(0.5, Vec2(0, center.y/10));
	Sequence *popupActions = Sequence::create(
		//CCScaleTo::create(0.15, 1.25), 
		//CCScaleTo::create(0.1, 1)
		//CCScaleTo::create(0.08, 1.0),
		EaseExponentialInOut::create(mt),
		JumpTo::create(0.5, Vec2(0, center.y/10), center.y/10+20, 2),
		NULL);
	this->runAction(popupActions);
#else


	this->setPositionY(center.y/10);
// 	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
// 	this->setScale(0.1);
// 	this->setOpacity(0x0);

this->setScale(0.5);
	Spawn* sp = Spawn::createWithTwoActions(FadeIn::create(0.5), EaseExponentialInOut::create(ScaleTo::create(0.5, 1)));

// 	this->runAction(sp);
#endif
}
//退出
void PopupLayer::onExit()
{
	CCLayer::onExit();
}

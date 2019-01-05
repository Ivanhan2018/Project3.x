#include "DDZDialogLayer.h"
#include "DDZSceneData.h"
#include "DDZSceneControl.h"
#include "DDZRes.h"//<by hxh>
#include "SceneView.h"//<by hxh>
#include "DDZSceneLogic.h"

using namespace ui;

DDZDialogLayer::DDZDialogLayer(DDZdialog_type type)
{
	m_type = type;
}

DDZDialogLayer::~DDZDialogLayer()
{
}

DDZDialogLayer * DDZDialogLayer::create(DDZdialog_type type)
{
	DDZDialogLayer * pRet = new DDZDialogLayer(type);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool DDZDialogLayer::init()
{
	//setTouchEnabled(true);
	//setTouchMode(kCCTouchesAllAtOnce);	
	if(!Layer::init())
	{
		return false;
	}

	Dictionary * strings = Dictionary::createWithContentsOfFile("ddz_strings.xml");
	string sText;
	switch(m_type)
	{
	case DDZdialog_hints:
		sText = ((CCString*)strings->objectForKey("socketofboxstring"))->getCString();
		initTable(sText, 1);
		break;
	case DDZdialog_back:
		sText = ((CCString*)strings->objectForKey("backtodt"))->getCString();
		initTable(sText, 2);
		break;
	case DDZdialog_poChan:
		sText = ((CCString*)strings->objectForKey("pochan"))->getCString();
		initTable(sText, 1);
		break;
	case DDZdialog_poChanSongCoin:
		sText = ((CCString*)strings->objectForKey("pochansongcoin"))->getCString();
		initTable(sText, 1);
		break;
	case DDZdialog_dismissTable:
		sText = ((CCString*)strings->objectForKey("dismisstable"))->getCString();
		initTable(sText, 1);
		break;
	case DDZdialog_roomLimitMax:
		sText = ((CCString*)strings->objectForKey("roomLimitMax"))->getCString();
		initTable(sText, 1);
		break;
	case DDZdialog_roomLimitMin:
		sText = ((CCString*)strings->objectForKey("roomLimitMin"))->getCString();
		initTable(sText, 2);
		break;
	case DDZdialog_Other_Login:
	case DDZdialog_ChangeTableFailed:
		initTable("", 1);
		break;
	case DDZdialog_Reject:
		sText = ((CCString*)strings->objectForKey("dialog_reject"))->getCString();
		initTable(sText, 1);
		break;
	default:
		break;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(DDZDialogLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(DDZDialogLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(DDZDialogLayer::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(DDZDialogLayer::onTouchCancelled, this);
	listener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool DDZDialogLayer::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZDialogLayer::onTouchBegan");
	return true;
}

void DDZDialogLayer::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZDialogLayer::onTouchMoved");
}

void DDZDialogLayer::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZDialogLayer::onTouchEnded");
}

void DDZDialogLayer::onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZDialogLayer::onTouchCancelled");
}

void DDZDialogLayer::setTipsString(const std::string sText)
{
	m_pLabTips->setString(sText);
}

void DDZDialogLayer::onClickBtnOK(Ref* pSender)
{
	switch(m_type)
	{
	case DDZdialog_back:
	case DDZdialog_poChan:
	case DDZdialog_hints:
	case DDZdialog_dismissTable:
	case DDZdialog_ChangeTableFailed:	//	换桌失败
	case DDZdialog_Reject:
		{
			//EntityMgr::instance()->roomFrame()->setGameIsStart(false);
			// add by hxh 20160818
			SceneView::ToPortrait();
			Director::getInstance()->replaceScene(DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_start,false));
		}
		break;
	case DDZdialog_roomLimitMax:
		{
			DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_endAgain,false);					//--直接排队
		}
		break;
	case DDZdialog_roomLimitMin:
		{
			NotificationCenter::getInstance()->postNotification(MESSAGE_JNICONTROL,Integer::create(MSG_JNI_Recharge));//充值
			DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_endAgain,false);					//--直接排队
		}
		break;
	case DDZdialog_poChanSongCoin:
		{
			Director::getInstance()->replaceScene(DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_start,false));		//--返回大厅
			//DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_endAgain,false);
		}
		break;
	case DDZdialog_Other_Login:
		{
			//Director::getInstance()->end();
			//exit(0);
			SceneView::ToPortrait();
			Director::getInstance()->replaceScene(DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_start,false));
		}
		break;
	default:
		break;
	}
	getParent()->removeChild(this);
}

void DDZDialogLayer::onClickBtnCancel(Ref* pSender)
{
	switch(m_type)
	{
	case DDZdialog_back:			//--不做处理
		break;
	case DDZdialog_roomLimitMin:
		{
			DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_endAgain,false);					//--直接排队
		}
		break;
	}
	removeFromParent();
}

void DDZDialogLayer::onEnter()
{
	Layer::onEnter();
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min)()+1,true);
}

void DDZDialogLayer::onExit()
{
	Layer::onExit();
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void DDZDialogLayer::initTable(const string sName,int type)
{
	cocos2d::Size size = Director::sharedDirector()->getWinSize();
	LayerColor * colorLayer = LayerColor::create(ccc4(0,0,0,120));
	this->addChild(colorLayer);

	//	弹窗背景
	Sprite * pBG = Sprite::createWithSpriteFrameName(DDZtexture_name::s_popups_bg);
	colorLayer->addChild(pBG);
	pBG->setPosition(ccp(size.width/2, size.height/2));
	cocos2d::Size sBGSize = pBG->getContentSize();

	//	提示文
/*
	Dictionary * strings = Dictionary::createWithContentsOfFile("xml/ddz_strings.xml");
	m_pLabTips = LabelTTF::create(((CCString*)strings->objectForKey(Name))->getCString(),
		"Helvetica-Bold",
		30,
		cocos2d::Size(480,90),
		kCCTextAlignmentCenter,
		kCCVerticalTextAlignmentTop);
*/
	m_pLabTips = LabelTTF::create(sName,
		"Helvetica-Bold",
		30,
		cocos2d::Size(480,90),
		kCCTextAlignmentCenter,
		kCCVerticalTextAlignmentTop);

	pBG->addChild(m_pLabTips);
	m_pLabTips->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.6));
	m_pLabTips->setColor(ccc3(118,78,0));

	if (type==1)
	{
		m_pBtnOK = Button::create(DDZtexture_name::s_btnOKNormal, DDZtexture_name::s_btnOKPressed, "", TextureResType::PLIST);
		pBG->addChild(m_pBtnOK);
		m_pBtnOK->addClickEventListener(CC_CALLBACK_1(DDZDialogLayer::onClickBtnOK, this));
		m_pBtnOK->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.25));
	}
	else if(type==2)
	{
		m_pBtnOK = Button::create(DDZtexture_name::s_btnOKNormal, DDZtexture_name::s_btnOKPressed, "", TextureResType::PLIST);
		pBG->addChild(m_pBtnOK);
		m_pBtnOK->addClickEventListener(CC_CALLBACK_1(DDZDialogLayer::onClickBtnOK, this));
		m_pBtnOK->setPosition(Vec2(sBGSize.width * 0.26, sBGSize.height * 0.25));

		m_pBtnCancel = Button::create(DDZtexture_name::s_btnCancelNormal, DDZtexture_name::s_btnCancelPressed, "", TextureResType::PLIST);
		pBG->addChild(m_pBtnCancel);
		m_pBtnCancel->addClickEventListener(CC_CALLBACK_1(DDZDialogLayer::onClickBtnCancel, this));
		m_pBtnCancel->setPosition(Vec2(sBGSize.width * 0.74, sBGSize.height * 0.25));
	}
}

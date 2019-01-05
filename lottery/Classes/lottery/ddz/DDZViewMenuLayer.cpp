#include "DDZViewMenuLayer.h"
#include "DDZSceneLogic.h"
#include "DDZDialogLayer.h"
#include "DDZSetUpLayer.h"

DDZViewMenuLayer::DDZViewMenuLayer()
{
	m_bIsShowMenu = true;
}

DDZViewMenuLayer::~DDZViewMenuLayer()
{
}

bool DDZViewMenuLayer::init()
{
	bool ret = false;
	do 
	{
		if(!Layer::init())
		{
			break;
		}

		cocos2d::Size size = Director::getInstance()->getWinSize();
		m_isVisibleStart = false;
		m_menuPositionY = size.height * 0.46;		//按钮坐标Y位置
		setKeypadEnabled(true);
		SEL_MenuHandler selector = menu_selector(DDZViewMenuLayer::menuCallback);

		//计时器
		m_timer =DDZTimer::create();
		this->addChild(m_timer);
		m_timer->setPosition(ccp(size.width/2,170.0f));
		m_timer->setVisible(false);

		//	菜单栏背景
		m_pMenuBG = Sprite::createWithSpriteFrameName(DDZtexture_name::s_menuBarBackground);
		this->addChild(m_pMenuBG);
		cocos2d::Size sMenuBGSize = m_pMenuBG->getContentSize();
		m_pMenuBG->setPosition(Vec2(size.width / 2, size.height - sMenuBGSize.height / 2));

		//菜单栏拉环
		m_pPullTab = Button::create(DDZtexture_name::s_menuPullTabBackNormal, "", "", TextureResType::PLIST);
		m_pMenuBG->addChild(m_pPullTab);
		m_pPullTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		m_pPullTab->setPosition(Vec2(sMenuBGSize.width * 0.85, sMenuBGSize.height * 0.06));
		m_pPullTab->addClickEventListener(CC_CALLBACK_1(DDZViewMenuLayer::onClickPullTab, this));

		//菜单栏
		m_menuBar = Menu::create();
		m_pMenuBG->addChild(m_menuBar);

		//返回按钮
		m_pMenuItem[DDZmenu_menuBarBackItem] = addMenuBarItem(
			DDZtexture_name::s_back,
			"",
			"",
			this,
			menu_selector(DDZViewMenuLayer::onClickBack),
			ccp(140.0f,-26.0f),
			Integer::create(DDZmenu_menuBarBackItem),
			DDZmenu_menuBarBackItem,
			true);

		//机器人按钮
		m_pMenuItem[DDZmenu_menuBarRobotItem] = addMenuBarItem(
			DDZtexture_name::s_robot,
			"",
			"",
			this,
			selector,
			ccp(-70.0f,-26.0f)
			,Integer::create(DDZmenu_menuBarRobotItem)
			,DDZmenu_menuBarRobotItem,
			true);

		//设置按钮
		m_pMenuItem[DDZmenu_menuBarOptionsItem] = addMenuBarItem(
			DDZtexture_name::s_options,
			"",
			"",
			this,
			menu_selector(DDZViewMenuLayer::onClickSetting),
			ccp(70.0f,-26.0f),
			Integer::create(DDZmenu_menuBarOptionsItem),
			DDZmenu_menuBarOptionsItem,
			true);
/*
		//聊天按钮
		addMenuBarItem(DDZtexture_name::s_talk,"","",this,selector,ccp(-140.0f,-26.0f),Integer::create(DDZmenu_menuBarTalkItem),DDZmenu_menuBarTalkItem,false);
		//任务按钮
		addMenuBarItem(DDZtexture_name::s_task,"","",this,selector,ccp(0,-26.0f),Integer::create(DDZmenu_menuBarTaskItem),DDZmenu_menuBarTaskItem,false);
*/

		m_menuBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_menuBar->setPosition(Vec2(m_pMenuBG->getContentSize() / 2));
		m_menuBar->alignItemsHorizontallyWithPadding(130);

		//菜单
		m_menu = Menu::create();
		this->addChild(m_menu);
		m_menu->setPosition(Vec2::ZERO);

		cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
		this->setContentSize(sVSize);

		//	换桌
 		m_pMenuItem[DDZmenu_changeTable] = addMenuItem(DDZtexture_name::s_changeTableNormal, DDZtexture_name::s_changeTablePressed, "", this, selector,
 			ccp(size.width * 0.3, m_menuPositionY),Integer::create(DDZmenu_changeTable),DDZmenu_changeTable,false);
		//明牌开始
 		m_pMenuItem[DDZmenu_visibleStartMenuItem] = addMenuItem(DDZtexture_name::s_openStartNormal, DDZtexture_name::s_openStartPressed, "", this, selector,
 			ccp(size.width * 0.3,m_menuPositionY),Integer::create(DDZmenu_visibleStartMenuItem),DDZmenu_visibleStartMenuItem,false);
		//普通开始
		m_pMenuItem[DDZmenu_startMenuItem] = addMenuItem(DDZtexture_name::s_startNormal, DDZtexture_name::s_startPressed, "", this, selector,
			ccp(size.width * 0.7, m_menuPositionY),Integer::create(DDZmenu_startMenuItem),DDZmenu_startMenuItem,false);

		//发牌过程中的明牌开始
		m_pMenuItem[DDZmenu_visibleCardStartItem] = addMenuItem(DDZtexture_name::s_openStartNormal, DDZtexture_name::s_openStartPressed, "", this, selector,
			ccp(size.width/2,m_menuPositionY),Integer::create(DDZmenu_visibleCardStartItem),DDZmenu_visibleCardStartItem,false);
		//发牌过程中的明牌开始倍率
		m_pMenuItem[DDZmenu_number] = addMenuItem(DDZtexture_name::s_one,"","",this,selector,
			ccp(size.width/2-90.0f,m_menuPositionY+5.0f),Integer::create(DDZmenu_number),DDZmenu_number,false);

		//不出
		m_pMenuItem[DDZmenu_passOutCardMenuItem] = addMenuItem(DDZtexture_name::s_passNormal, DDZtexture_name::s_passPressed, "", this, selector,
			ccp(size.width * 0.3, m_menuPositionY),Integer::create(DDZmenu_passOutCardMenuItem),DDZmenu_passOutCardMenuItem,false);
		//提示
		m_pMenuItem[DDZmenu_hintMenuItem] = addMenuItem(DDZtexture_name::s_tipsNormal, DDZtexture_name::s_tipsPressed, "", this, selector,
			ccp(size.width * 0.5, m_menuPositionY),Integer::create(DDZmenu_hintMenuItem),DDZmenu_hintMenuItem,false);
		//出牌按钮
		m_pMenuItem[DDZmenu_outCardMenuItem] = addMenuItem(DDZtexture_name::s_outCardNormal, DDZtexture_name::s_outCardPressed, "", this, selector,
			ccp(size.width * 0.7, m_menuPositionY),Integer::create(DDZmenu_outCardMenuItem),DDZmenu_outCardMenuItem,false);

		//明牌X2倍
		m_pMenuItem[DDZmenu_visibleCardItem] = addMenuItem(DDZtexture_name::s_openCardNormal, DDZtexture_name::s_openCardPressed, "", this, selector,
			ccp(size.width/2-200,m_menuPositionY),Integer::create(DDZmenu_visibleCardItem),DDZmenu_visibleCardItem,false);

		//叫地主
		m_pMenuItem[DDZmenu_jiaoDiZhuItem] = addMenuItem(DDZtexture_name::s_jiaoDiZhuNormal, DDZtexture_name::s_jiaoDiZhuPressed, "", this, selector,
			ccp(size.width * 0.3,m_menuPositionY),Integer::create(DDZmenu_jiaoDiZhuItem),DDZmenu_jiaoDiZhuItem,false);
		//不叫
		m_pMenuItem[DDZmenu_buJiaoItem] = addMenuItem(DDZtexture_name::s_buJiaoNormal, DDZtexture_name::s_buJiaoPressed, "", this, selector,
			ccp(size.width * 0.7,m_menuPositionY),Integer::create(DDZmenu_buJiaoItem),DDZmenu_buJiaoItem,false);

		//抢地主
		m_pMenuItem[DDZmenu_qiangDiZhuItem] = addMenuItem(DDZtexture_name::s_qiangDiZhuNormal,DDZtexture_name::s_qiangDiZhuPressed, "", this, selector,
			ccp(size.width * 0.3,m_menuPositionY),Integer::create(DDZmenu_qiangDiZhuItem),DDZmenu_qiangDiZhuItem,false);
		//不抢地主
		m_pMenuItem[DDZmenu_passQiangDiZhuItem] = addMenuItem(DDZtexture_name::s_buqiangNormal, DDZtexture_name::s_buqiangPressed, "", this, selector,
			ccp(size.width * 0.7,m_menuPositionY),Integer::create(DDZmenu_passQiangDiZhuItem),DDZmenu_passQiangDiZhuItem,false);

		//加倍
		m_pMenuItem[DDZmenu_doubleItem] = addMenuItem(DDZtexture_name::s_redoubleNormal, DDZtexture_name::s_redoublePressed, "", this, selector,
			ccp(size.width * 0.3,m_menuPositionY),Integer::create(DDZmenu_doubleItem),DDZmenu_doubleItem,false);
		//不加倍
		m_pMenuItem[DDZmenu_unDoubleItem] = addMenuItem(DDZtexture_name::s_notRedoubleNormal, DDZtexture_name::s_notRedoublePressed, "", this, selector,
			ccp(size.width * 0.7,m_menuPositionY),Integer::create(DDZmenu_unDoubleItem),DDZmenu_unDoubleItem,false);

		//结算详情
		m_pMenuItem[DDZmenu_settlementDetial] = addMenuItem(DDZtexture_name::s_detailNormal, DDZtexture_name::s_detailPressed, "", this, selector,
			ccp(250.0f,100.0f),Integer::create(DDZmenu_settlementDetial),DDZmenu_settlementDetial,false);
		//再来一句
		m_pMenuItem[DDZmenu_again] = addMenuItem(DDZtexture_name::s_againNormal, DDZtexture_name::s_againPressed, "", this, selector,
			ccp(size.width-250.0f,100.0f),Integer::create(DDZmenu_again),DDZmenu_again,false);

		ret = true;
	} while (0);

	return ret;
}

void DDZViewMenuLayer::menuCallback(Object * pSender)
{
	MenuItemSprite * itemSprite = (MenuItemSprite*)pSender;
	if (itemSprite->getTag()==DDZmenu_visibleCardStartItem)
	{
		DDZCCviewOpenCardNum * _openCard = (DDZCCviewOpenCardNum*)(itemSprite->getUserObject());
		NotificationCenter::getInstance()->postNotification(MESSAGE_MENUCLICKED,_openCard);
	}
	else
	{
		Integer * msg = Integer::create(itemSprite->getTag());
		NotificationCenter::getInstance()->postNotification(MESSAGE_MENUCLICKED,msg);
	}
}

void DDZViewMenuLayer::enterGameView()
{
	finishClick();
	cocos2d::Size size = Director::getInstance()->getWinSize();
	MenuItemSprite * changeTableItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_changeTable);
	changeTableItem->setPosition(ccp(size.width * 0.5, m_menuPositionY));
	changeTableItem->setVisible(true);
}

void DDZViewMenuLayer::startGame()
{
	finishClick();
	cocos2d::Size size = Director::getInstance()->getWinSize();
	MenuItemSprite * visibleStartItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_visibleStartMenuItem);
	//visibleStartItem->setVisible(true);
	MenuItemSprite * changeTableItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_changeTable);
	//ccp(size.width * 0.3, m_menuPositionY)
	changeTableItem->setPosition(ccp(size.width * 0.3, m_menuPositionY));
	changeTableItem->setVisible(true);
	MenuItemSprite * startMenuItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_startMenuItem);
	startMenuItem->setVisible(true);
	MenuItemSprite * visibleStartNumberItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_number);
	visibleStartNumberItem->setNormalImage(Sprite::createWithSpriteFrameName(DDZtexture_name::s_five));
	visibleStartNumberItem->setPositionX(310.0f);
	//visibleStartNumberItem->setVisible(true);
}

void DDZViewMenuLayer::visibleCard()
{
	return;
	finishClick();
	MenuItemSprite * visibleCardItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_visibleCardItem);
	visibleCardItem->setVisible(true);
}

void DDZViewMenuLayer::jiaoDiZhu()
{
	finishClick();
	cocos2d::Size size = Director::getInstance()->getWinSize();
	MenuItemSprite * jiaoDiZhuItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_jiaoDiZhuItem);
	jiaoDiZhuItem->setVisible(true);
	MenuItemSprite * buJiaoItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_buJiaoItem);
	buJiaoItem->setVisible(true);
}

void DDZViewMenuLayer::qiangDiZhu()
{
	finishClick();
	cocos2d::Size size = Director::getInstance()->getWinSize();
	MenuItemSprite * qiangDiZhuItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_qiangDiZhuItem);
	qiangDiZhuItem->setVisible(true);
	MenuItemSprite * buQiangItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_passQiangDiZhuItem);
	buQiangItem->setVisible(true);
}

void DDZViewMenuLayer::outCard(int state)
{
	finishClick();
	cocos2d::Size size = Director::getInstance()->getWinSize();
	MenuItemSprite * outCardItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_outCardMenuItem);//出牌按钮
	MenuItemSprite * passOutCardItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_passOutCardMenuItem);//不出按钮
	MenuItemSprite * hintItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_hintMenuItem);//提示按钮
	MenuItemSprite * visibleCardItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_visibleCardItem);//明牌按钮
	MenuItemSprite * menuDoubleItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_doubleItem);//加倍按钮
// 	menuDoubleItem->setPosition(ccp(size.width/2+200,m_menuPositionY));
	switch(state)
	{
	case 1://地主首轮出牌且不能明牌
		outCardItem->setPosition(size.width/2,m_menuPositionY);
		break;
	case 0://地主首轮出牌能明牌
		//visibleCardItem->setVisible(true);
		outCardItem->setPosition(size.width/2,m_menuPositionY);
		break;
	case 2://上轮最大方，现在第一轮出牌
		outCardItem->setPosition(size.width/2,m_menuPositionY);
// 		m_timer->setPosition(Vec2(size.width * 0.5, size.height * 0.4));
		break;
	default:
		outCardItem->setPosition(size.width * 0.7,m_menuPositionY);
		passOutCardItem->setVisible(true);
		hintItem->setVisible(true);
// 		hintItem->setPosition(ccp(size.width/2,m_menuPositionY));
// 		passOutCardItem->setPosition(ccp(hintItem->getPositionX()-10-passOutCardItem->getNormalImage()->getContentSize().width,
// 			m_menuPositionY));
// 		m_timer->setPosition(Vec2(size.width * 0.5, size.height * 0.4));
// 		outCardItem->setPosition(ccp(m_timer->getPositionX()+outCardItem->getNormalImage()->getContentSize().width/2+40,
// 			m_menuPositionY));
		break;
	}
	outCardItem->setVisible(true);
}

void DDZViewMenuLayer::gameDDZSettlement()
{
	finishClick();
	/*m_pMenuItem[DDZmenu_settlementDetial]->setVisible(true);
	m_pMenuItem[DDZmenu_again]->setVisible(true);*/
}

void DDZViewMenuLayer::jiaBei(bool isShow)
{
	//return;
	//DDZmenu_unDoubleItem
	cocos2d::Size size = Director::getInstance()->getWinSize();
	MenuItemSprite * menuDoubleItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_doubleItem);
	
	//MenuItemSprite * outCardMenuItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_outCardMenuItem);
	//if (!outCardMenuItem->isVisible())
	//{
	//	menuDoubleItem->setPositionX(size.width/2);
	//}
	
	MenuItemSprite * menuUnDoubleItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_unDoubleItem);
	if (!isShow)
	{
		menuDoubleItem->setVisible(false);
		menuUnDoubleItem->setVisible(false);
		return;
	}
	menuDoubleItem->setVisible(true);
	menuUnDoubleItem->setVisible(true);
	menuDoubleItem->setPositionX(size.width * 0.7);
	menuUnDoubleItem->setPositionX(size.width * 0.3);
}

bool DDZViewMenuLayer::isShowBtns()
{
	if (m_pMenuItem[DDZmenu_visibleStartMenuItem]->isVisible()
		||m_pMenuItem[DDZmenu_changeTable]->isVisible()
		||m_pMenuItem[DDZmenu_outCardMenuItem]->isVisible()
		||m_pMenuItem[DDZmenu_jiaoDiZhuItem]->isVisible()
		||m_pMenuItem[DDZmenu_qiangDiZhuItem]->isVisible()
		||m_pMenuItem[DDZmenu_doubleItem]->isVisible()
		||m_pMenuItem[DDZmenu_startMenuItem]->isVisible()
		||m_pMenuItem[DDZmenu_passOutCardMenuItem]->isVisible()
		||m_pMenuItem[DDZmenu_buJiaoItem]->isVisible()
		||m_pMenuItem[DDZmenu_passQiangDiZhuItem]->isVisible()
		||m_pMenuItem[DDZmenu_visibleCardItem]->isVisible()
		||m_pMenuItem[DDZmenu_settlementDetial]->isVisible()
		||m_pMenuItem[DDZmenu_again]->isVisible()
		||m_pMenuItem[DDZmenu_hintMenuItem]->isVisible()
		||m_pMenuItem[DDZmenu_number]->isVisible()
		||m_pMenuItem[DDZmenu_visibleCardStartItem]->isVisible()
		)
	{
		return true;
	}
	return false;
}

void DDZViewMenuLayer::finishClick()
{
	m_pMenuItem[DDZmenu_visibleStartMenuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_changeTable]->setVisible(false);
	m_pMenuItem[DDZmenu_outCardMenuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_jiaoDiZhuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_qiangDiZhuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_doubleItem]->setVisible(false);
	m_pMenuItem[DDZmenu_startMenuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_passOutCardMenuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_buJiaoItem]->setVisible(false);
	m_pMenuItem[DDZmenu_passQiangDiZhuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_visibleCardItem]->setVisible(false);
	m_pMenuItem[DDZmenu_settlementDetial]->setVisible(false);
	m_pMenuItem[DDZmenu_again]->setVisible(false);
	m_pMenuItem[DDZmenu_hintMenuItem]->setVisible(false);
	m_pMenuItem[DDZmenu_number]->setVisible(false);
	m_pMenuItem[DDZmenu_visibleCardStartItem]->setVisible(false);
	m_pMenuItem[DDZmenu_unDoubleItem]->setVisible(false);

/*
	MenuItemSprite * visibleStartItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_visibleStartMenuItem);
	visibleStartItem->setVisible(false);
	MenuItemSprite * changeTableItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_changeTable);
	changeTableItem->setVisible(false);
	MenuItemSprite * outCardItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_outCardMenuItem);
	outCardItem->setVisible(false);
	MenuItemSprite * jiaoDiZhuItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_jiaoDiZhuItem);
	jiaoDiZhuItem->setVisible(false);
	MenuItemSprite * qiangDiZhuItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_qiangDiZhuItem);
	qiangDiZhuItem->setVisible(false);
	MenuItemSprite * doubleItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_doubleItem);
	doubleItem->setVisible(false);
	MenuItemSprite * startMenuItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_startMenuItem);
	startMenuItem->setVisible(false);
	MenuItemSprite * passOutCardItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_passOutCardMenuItem);
	passOutCardItem->setVisible(false);
	MenuItemSprite * buJiaoItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_buJiaoItem);
	buJiaoItem->setVisible(false);
	MenuItemSprite * buQiangItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_passQiangDiZhuItem);
	buQiangItem->setVisible(false);
// 	MenuItemSprite * unDoubleItem = (MenuItemSprite*)m_menu->getChildByTag(menu_unDoubleItem);
// 	unDoubleItem->setVisible(false);
	MenuItemSprite * visibleCardItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_visibleCardItem);
	visibleCardItem->setVisible(false);
	MenuItemSprite * settlmentDetialItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_settlementDetial);
	settlmentDetialItem->setVisible(false);
	MenuItemSprite * againItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_again);
	againItem->setVisible(false);
	MenuItemSprite * hintItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_hintMenuItem);
	hintItem->setVisible(false);
	MenuItemSprite * visibleStartNumberItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_number);
	visibleStartNumberItem->setVisible(false);
	MenuItemSprite * visibleCardStartItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_visibleCardStartItem);
	visibleCardStartItem->setVisible(false);
*/
}

MenuItemSprite* DDZViewMenuLayer::addMenuItem(const char * normal,const char * selected,const char * disable, Object * context,SEL_MenuHandler selector,Vec2 point, Object * pUserObject, int tag,bool visible)
{

	SpriteFrame * pFrameNormal = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	SpriteFrame * pFrameSelected = SpriteFrameCache::getInstance()->spriteFrameByName(selected);
	SpriteFrame * pFrameDisable = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	Sprite * normalSprite = Sprite::createWithSpriteFrame(pFrameNormal);
 	Sprite * selectedSprite = Sprite::createWithSpriteFrame(pFrameSelected);
 	Sprite * disableSprite = Sprite::createWithSpriteFrame(pFrameDisable);
	disableSprite->setColor(ccc3(100,100,100));
	MenuItemSprite * menuItemSprite = MenuItemSprite::create(normalSprite,selectedSprite,disableSprite,context,selector);
	menuItemSprite->setPosition(point);
//	menuItemSprite->setUserObject(pUserObject);
	menuItemSprite->setScale(0.9f);
	menuItemSprite->setTag(tag);
	menuItemSprite->setVisible(visible);
	menuItemSprite->setAnchorPoint(ccp(0.5f,0.5f));
	m_menu->addChild(menuItemSprite);
	
	return menuItemSprite;
}

MenuItemSprite* DDZViewMenuLayer::addMenuBarItem(const char * normal,const char * selected,const char * disable, Object * context,SEL_MenuHandler selector,Vec2 point, Object * pUserObject, int tag,bool visible)
{
	SpriteFrame * pFrameNormal = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	SpriteFrame * pFrameSelected = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	SpriteFrame * pFrameDisable = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	Sprite * normalSprite = Sprite::createWithSpriteFrame(pFrameNormal);
	Sprite * selectedSprite = Sprite::createWithSpriteFrame(pFrameSelected);
	selectedSprite->setColor(ccc3(100,100,100));
	Sprite * disableSprite = Sprite::createWithSpriteFrame(pFrameDisable);
	MenuItemSprite * menuItemSprite = MenuItemSprite::create(normalSprite,selectedSprite,disableSprite,context,selector);
// 	menuItemSprite->setPosition(point);
	menuItemSprite->setPosition(Vec2::ZERO);
	menuItemSprite->setUserObject(pUserObject);
	menuItemSprite->setTag(tag);
	menuItemSprite->setVisible(visible);
	m_menuBar->addChild(menuItemSprite);

	return menuItemSprite;
}

void DDZViewMenuLayer::setVisibleStartNumber(int num)
{
	return;
	finishClick();
	cocos2d::Size size = Director::getInstance()->getWinSize();
	MenuItemSprite * visibleStartNumberItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_number);
	visibleStartNumberItem->setPositionX(size.width/2+65);
	//visibleStartNumberItem->setVisible(true);
	MenuItemSprite * visibleCardStartItem = (MenuItemSprite*)m_menu->getChildByTag(DDZmenu_visibleCardStartItem);
	//visibleCardStartItem->setVisible(true);
	if (m_isVisibleStart)
	{
		visibleStartNumberItem->setVisible(false);
		visibleCardStartItem->setVisible(false);
		return ;
	}
	if (num<=1)
	{
		visibleStartNumberItem->setVisible(false);
		visibleCardStartItem->setVisible(false);
	}
	int _int=0;
	switch (num)
	{
	case 5:
		_int = 5;
		visibleStartNumberItem->setNormalImage(Sprite::createWithSpriteFrameName(DDZtexture_name::s_five));
		break;
	case 4:
		_int = 4;
		visibleStartNumberItem->setNormalImage(Sprite::createWithSpriteFrameName(DDZtexture_name::s_four));
		break;
	case 3:
		_int = 3;
		visibleStartNumberItem->setNormalImage(Sprite::createWithSpriteFrameName(DDZtexture_name::s_three));
		break;
	case 2:
		_int = 2;
		visibleStartNumberItem->setNormalImage(Sprite::createWithSpriteFrameName(DDZtexture_name::s_two));
		break;
	}
	DDZCCviewOpenCardNum * obj = DDZCCviewOpenCardNum::create(DDZmenu_visibleCardStartItem);
	obj->m_num = _int;
	visibleCardStartItem->setUserObject(obj);
}

void DDZViewMenuLayer::setTimer(int userId,DDZGame_State gameState,int t,bool visible,int key)
{
	if(!visible)
	{
		m_timer->setVisible(false);
		return;
	}
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_timer->unscheduleAllSelectors();
	if (!m_timer->isVisible())
	{
		m_timer->setVisible(true);
	}
	m_timer->start(userId,t,gameState,key);
	
}

void DDZViewMenuLayer::closeTimer()
{
	m_timer->unscheduleAllSelectors();
	m_timer->setVisible(false);
}

void DDZViewMenuLayer::hintVisibleCardItem()
{
	m_menu->getChildByTag(DDZmenu_visibleCardItem)->setVisible(false);
}

void DDZViewMenuLayer::setRobotEnable(bool enable)
{
	MenuItemSprite * itemSprite = (MenuItemSprite*)m_menuBar->getChildByTag(DDZmenu_menuBarRobotItem);
	itemSprite->setEnabled(enable);
	if (enable)
	{
		itemSprite->setColor(ccc3(255,255,255));
	}
	else
	{
		itemSprite->setColor(ccc3(120,120,120));
	}
}

void DDZViewMenuLayer::setTalkEnable(bool enable)
{
/*
	//	去除聊天功能
	return;
	MenuItemSprite * itemSprite = (MenuItemSprite*)m_menuBar->getChildByTag(DDZmenu_menuBarTalkItem);
	itemSprite->setEnabled(enable);
	if (enable)
	{
		itemSprite->setColor(ccc3(255,255,255));
	}
	else
	{
		itemSprite->setColor(ccc3(120,120,120));
	}
*/
}

void DDZViewMenuLayer::onClickPullTab(Ref* sender)
{
	log("DDZViewMenuLayer::onClickPullTab");
	cocos2d::Size sMenuBGSize = m_pMenuBG->getContentSize();
	MoveBy* pMove;
	CallFunc* pCallEnd;
	if (m_bIsShowMenu)
	{
		pMove = MoveBy::create(0.5f, Vec2(0, sMenuBGSize.height));
		pCallEnd = CallFunc::create([=](){
			m_pPullTab->loadTextureNormal(DDZtexture_name::s_menuPullTabFrontNormal, TextureResType::PLIST);
			m_pPullTab->loadTexturePressed("", TextureResType::PLIST);
			m_pPullTab->loadTextureDisabled("", TextureResType::PLIST);
		});
	}
	else
	{
		pMove = MoveBy::create(0.5f, Vec2(0, -sMenuBGSize.height));
		pCallEnd = CallFunc::create([=](){
			m_pPullTab->loadTextureNormal(DDZtexture_name::s_menuPullTabBackNormal, TextureResType::PLIST);
			m_pPullTab->loadTexturePressed("", TextureResType::PLIST);
			m_pPullTab->loadTextureDisabled("", TextureResType::PLIST);
		});
	}
	m_bIsShowMenu = !m_bIsShowMenu;
	m_pMenuBG->runAction(Sequence::create(pMove, pCallEnd, nullptr));
}

void DDZViewMenuLayer::onClickBack(Ref* sender)
{
	auto scene = Director::getInstance()->getRunningScene();
	if (scene->getChildByTag(1128))
	{
		return;
	}
	DDZDialogLayer* pDDZDialogLayer = DDZDialogLayer::create(DDZdialog_back);
	pDDZDialogLayer->setTag(1128);
	
	scene->addChild(pDDZDialogLayer);
}

void DDZViewMenuLayer::onClickSetting(Ref* sender)
{
	DDZSetUpLayer* pSetUpLayer = DDZSetUpLayer::create();
	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(pSetUpLayer);
}

void DDZViewMenuLayer::setMenuItemVisible(DDZMenuItemType type, bool isVisible)
{
	m_pMenuItem[type]->setVisible(isVisible);
}

void DDZViewMenuLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		onClickBack(NULL);
		event->stopPropagation();
	}
}

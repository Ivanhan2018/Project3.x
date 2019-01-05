#include "RoomLayer.h"
#include "BJK8Kind.h"
#include "LotteryGambleLayer.h"
#include "LotteryBetData.h"
#include "VersionControl.h"

using namespace cocos2d;

BJK8Kind::BJK8Kind()
{

}
BJK8Kind::~BJK8Kind()
{
	m_vecPot.clear();
}
Scene* BJK8Kind::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		BJK8Kind *layer = BJK8Kind::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}
bool BJK8Kind::init()
{
	bool bRet = false;
	do 
	{
		if (!Layer::init())
		{
			return false;
		}
		selectedSpriteTag = ITEM_TAG;
		this->initData();
		this->initView();
		//this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void BJK8Kind::initData()
{
	//界面数据
	lyWanFa = nullptr;
	m_node = nullptr;

	m_isNowDel = false;
	
	//保存数据
	//MyBetData::getInstance()->reloadData();
}

void BJK8Kind::initView()
{

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(BJK8Kind::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(BJK8Kind::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(BJK8Kind::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(BJK8Kind::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//变量初始化
	totalItemSpriteCount = 0;
	int kindItemTag = ITEM_TAG;
	float xPos1 = 30;
	float xPos2 = SCREEN_WIDTH / 5.0f + SCREEN_WIDTH  / 10.0f;
	float xPos3 = SCREEN_WIDTH * 2.0f / 5.0f + SCREEN_WIDTH  / 5.0f;
	float xPos4 = SCREEN_WIDTH * 3.0f / 5.0f + SCREEN_WIDTH  /10.0f;
	float xPos5 = SCREEN_WIDTH * 4.0f / 5.0f + SCREEN_WIDTH  / 10.0f;
	float gapY = 70;//80;
	float fontSize = 24;//27;
	float startPosY = SCREEN_HEIGHT - 50; //50改成110

	//背景图
	auto frBackground = spriteFrame("beark.png");
#ifdef VER_QIANBO
	frBackground = spriteFrame("bj_a1.png");
#endif
	auto sptBackground = Sprite::createWithTexture(frBackground->getTexture(),cocos2d::Rect(0,0,MENU_WIDTH,SCREEN_HEIGHT));
	sptBackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	sptBackground->setPosition(Vec2(0,SCREEN_HEIGHT));
	this->addChild(sptBackground);

	//我的收藏玩法
	lyWanFa = LayerColor::create(ccc4(255,255,255,0),MENU_WIDTH,MENU_HEIGT);
	lyWanFa->setPosition(Vec2(0,SCREEN_HEIGHT - lyWanFa->getContentSize().height));
	this->addChild(lyWanFa, 3);

	//玩法背景图
	auto sptBack = Sprite::createWithTexture(frBackground->getTexture(),cocos2d::Rect(0,SCREEN_HEIGHT - lyWanFa->getContentSize().height,lyWanFa->getContentSize().width,lyWanFa->getContentSize().height),false);
	sptBack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	sptBack->setPosition(Vec2(0,lyWanFa->getContentSize().height));
	lyWanFa->addChild(sptBack);

	auto ltfMyWanFa = LabelTTF::create();
	ltfMyWanFa->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfMyWanFa->setPosition(Vec2(xPos1, lyWanFa->getContentSize().height - 100));
	ltfMyWanFa->setFontName("");
	ltfMyWanFa->setFontSize(fontSize + 5);
	ltfMyWanFa->setFontFillColor(Color3B::WHITE);
	ltfMyWanFa->setString(ConfigMgr::instance()->text("display_DuangDong.xml", "t1630"));
	lyWanFa->addChild(ltfMyWanFa, 1);	

	auto btDel = ui::Button::create("my_lottery_29.png","my_lottery_29_2.png","my_lottery_29_2.png");
	btDel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btDel->setPosition(Vec2(lyWanFa->getContentSize().width - 100,ltfMyWanFa->getPositionY() + 5));
	lyWanFa->addChild(btDel);

	auto btDone = ui::Button::create("my_lottery_29_3.png","my_lottery_29_4.png","my_lottery_29_4.png");
	btDone->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btDone->setPosition(Vec2(lyWanFa->getContentSize().width - 100,ltfMyWanFa->getPositionY() + 5));
	lyWanFa->addChild(btDone);

	btDone->setVisible(false);

	//分割线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	gap1->setPosition(Vec2(0, btDel->getPositionY() - btDel->getContentSize().height / 2 - 8));
	gap1->setScaleX(lyWanFa->getContentSize().width / gap1->getContentSize().width);	
	lyWanFa->addChild(gap1);

	//玩法列表
	for(int i = 0;i < LIST_MAX_SIZE;i ++)
	{
		float pos_now_x = 30;
		float pos_now_y = gap1->getPositionY() * (3 - i / 3) / 4.0f + (1 - i / 3) * 10;

		int pos_flag = i % 3;
		if(pos_flag == 1)
		{
			pos_now_x = 30 + 200;
		}else
			if(pos_flag == 2)
			{
				pos_now_x = 30 + 400;
			}

			auto itemBack = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_34.png")); 
			itemBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			itemBack->setPosition(Vec2(pos_now_x,pos_now_y));
			itemBack->setTag(8888 + i);						
			lyWanFa->addChild(itemBack,4);
			itemBack->setVisible(false);
			//文字
			LabelTTF *itemWord = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",1)->getCString()), "", fontSize);
			itemWord->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);	
			itemWord->setTag(6666 + i);
			itemWord->setPosition(Vec2(pos_now_x,pos_now_y));
			itemWord->setFontFillColor(ccc3(255,96,102));
			lyWanFa->addChild(itemWord,5);
	}

	//分割线
	Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	gap2->setPosition(Vec2(0, 0));
	gap2->setScaleX(lyWanFa->getContentSize().width / gap2->getContentSize().width);	
	lyWanFa->addChild(gap2);

	startPosY = SCREEN_HEIGHT - lyWanFa->getContentSize().height - gapY / 2;

	//菜单列表
	m_node = Node::create();
	m_node->setPosition(Vec2(0,0));
	this->addChild(m_node,2);
	Vec2 pt = Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT);

	winSize = Director::getInstance()->getWinSize();
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 0),lyWanFa->getContentSize().width, SCREEN_HEIGHT);
	layer->setAnchorPoint(Vec2::ZERO);
	layer->setPosition(Vec2::ZERO);
	this->addChild(layer,0);	

	int chongQingKindCount = atoi(ConfigMgr::instance()->text("display_kuaile8.xml", "t1000"));
	int iViewHeight = 0;

	//
	for(int i = 0 ; i < chongQingKindCount ; i++)
	{
		//位置
		int iPosTitle = atoi(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", 1000+i + 1)->getCString()));
		//分类名称 文字
		LabelTTF *kindTitleLabel = LabelTTF::create(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", iPosTitle)->getCString()), "", fontSize);
		kindTitleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		kindTitleLabel->setPosition(Vec2(xPos1, startPosY));		
		kindTitleLabel->setFontFillColor(Color3B::WHITE);
		m_node->addChild(kindTitleLabel, 2);

#ifdef VER_QIANBO
		kindTitleLabel->setFontFillColor(ccColor3B::WHITE);
#endif

		startPosY-=gapY;
		//分割线
		Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gap1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		gap1->setPosition(Vec2(0, startPosY + gapY / 2));
		gap1->setScaleX(lyWanFa->getContentSize().width / gap1->getContentSize().width);		
		m_node->addChild(gap1);

		iViewHeight ++;

		//大分类的个数
		int cataCount = atoi(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", iPosTitle + 1)->getCString()));

		for(int cata_i = 0;cata_i < cataCount;cata_i ++)
		{
			int iPosCata = atoi(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", iPosTitle + cata_i + 2)->getCString()));

			//显示前面 分类名称
			std::string strTitle = ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", iPosCata)->getCString());

			//为0 时不显示
			if(strTitle.size() == 1) strTitle.clear();

			LabelTTF *ltfTitle = LabelTTF::create(strTitle, "", fontSize);
			ltfTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			ltfTitle->setPosition(xPos1, startPosY);
			ltfTitle->setColor(Color3B::WHITE);
			m_node->addChild(ltfTitle, 2);		

			//菜单叶子 位置
			int itemPos = atoi(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", iPosCata + 1)->getCString()));

			//叶子个数
			int itemCount = atoi(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", itemPos)->getCString()));

			for(int j = 0 ; j < itemCount ; j++){

				String* kindStr = String::create(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d", itemPos + j + 1)->getCString()));

				//CCLOG("-=-=-=-=-=-==-== %s", kindStr->getCString());

				Sprite* sptKindItem = Sprite::createWithSpriteFrame(spriteFrame("gamble_05_3.png"));
				sptKindItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

				LabelTTF *kindItem = LabelTTF::create(kindStr->getCString(), "", fontSize);
				kindItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				kindItem->setColor(Color3B::WHITE);
				kindItem->setTag(9527);
				kindItem->setPosition(sptKindItem->getContentSize() / 2);			
				sptKindItem->addChild(kindItem);

				//位置
				int xPos = j % 2;
				if(xPos == 0){
					if(j!=0){
						startPosY-=gapY;
						//分割线
						Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
						gap1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
						gap1->setPosition(Vec2(0, startPosY + gapY / 2));
						gap1->setScaleX(lyWanFa->getContentSize().width / gap1->getContentSize().width);						
						m_node->addChild(gap1);

						iViewHeight ++;
					}
					sptKindItem->setPosition(Vec2(xPos2, startPosY));
				}else if(xPos == 1){
					sptKindItem->setPosition(Vec2(xPos3, startPosY));
				}else if(xPos == 2){
					sptKindItem->setPosition(Vec2(xPos4, startPosY));
				}else if(xPos == 3){
					sptKindItem->setPosition(Vec2(xPos5, startPosY));
				}

				sptKindItem->setTag(kindItemTag);
				m_node->addChild(sptKindItem, 2);

				if(kindItemTag == selectedSpriteTag){
					selectedSprite = Sprite::createWithSpriteFrame(spriteFrame("gamble_05_4.png")); 
					selectedSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
					selectedSprite->setPosition(sptKindItem->getPosition());
					m_node->addChild(selectedSprite, 3);
					selectedSprite->setVisible(false);
					//文字
					LabelTTF *kindItem = LabelTTF::create(ConfigMgr::instance()->text("display_kuaile8.xml",kindStr->getCString()), "", fontSize);
					kindItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);	
					kindItem->setTag(9527);
					kindItem->setPosition(sptKindItem->getContentSize() / 2);					
					kindItem->setFontFillColor(Color3B::WHITE);
					selectedSprite->addChild(kindItem);
				}
				kindItemTag++;
				totalItemSpriteCount++;
			}

			startPosY-=gapY; 
			//分割线
			Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
			gap2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			gap2->setPosition(Vec2(0, startPosY + gapY / 2));
			gap2->setScaleX(lyWanFa->getContentSize().width / gap2->getContentSize().width);			
			m_node->addChild(gap2);

			iViewHeight ++;
		}
	}

	//事件处理
	btDel->addClickEventListener([=](Ref *){
		btDone->setVisible(true);
		btDel->setVisible(false);
		m_isNowDel = true;
		refreshContent();
	});

	btDone->addClickEventListener([=](Ref *){
		btDone->setVisible(false);
		btDel->setVisible(true);
		m_isNowDel = false;

		MyBetData::getInstance()->saveData();
		refreshContent();
	});

	m_endPos=Vec2(pt.x,gapY * (iViewHeight - 13));	//调整高度

	refreshContent();
	refreshMenu();
}

void BJK8Kind::pressKindButton(int tag)
{
	playButtonSound();
	LotteryGambleLayer *layer = LotteryGambleLayer::create();

	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}


void BJK8Kind::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);	
}

void BJK8Kind::onExit()
{
	//
	Layer::onExit();
}

bool BJK8Kind::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (pTouch->getLocation().x > MENU_WIDTH) return false;

	//清空数组
	m_vecPot.clear();
	m_bDrag = false;

	pEvent->stopPropagation();
	return true;
}

void BJK8Kind::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = pTouch->getLocation();	
	//重写加速滑动
	m_bDrag = true; 

	m_end = m_node->getPosition().y + pTouch->getDelta().y;

	if(m_end<0){
		m_node->setPositionY(0);
		m_end = 0;
	} else
		if (m_end>m_endPos.y)
		{
			m_node->setPositionY(m_endPos.y);
			m_end = m_endPos.y;
		} else
		{
			m_node->setPositionY(m_end);	
			//加入点	
			m_vecPot.push_back(pTouch->getLocation()); 			
		} 	
}

void BJK8Kind::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	selectedSprite->setVisible(false);

	if(m_bDrag && m_vecPot.size() > 5)
	{
		m_bDrag = false;
		int iVecSize = m_vecPot.size(); 
		if(iVecSize < 1) return;
		float fYspeed = m_vecPot[iVecSize - 1].y;

		if(iVecSize < 2) return;

		fYspeed = fYspeed - m_vecPot[iVecSize - 2].y; 	

		while (abs(fYspeed) > 0.1)  
		{  
			Vec2 potNew = m_node->getPosition() + Vec2(0, fYspeed);  
			auto moveBy = MoveBy::create(0.5f, Vec2(0, fYspeed * 20)); 

			//检测滚动的位置
			m_node->runAction(Sequence::create(moveBy,CallFunc::create([=](){
				if(m_node->getPositionY() < 0)
				{
					m_node->setPositionY(0);					
				} else
					if (m_node->getPositionY() > m_endPos.y)
					{
						m_node->setPositionY(m_endPos.y);						
					}

			}),nullptr));				

			fYspeed *= 0.2;  //设置垂直摩擦系数
		}  

		return;
	}

	Vec2 touchPoint = pTouch->getLocation();

	if(touchPoint.y > SCREEN_HEIGHT - MENU_HEIGT)
	{
		if(lyWanFa == nullptr)
		{
			pEvent->stopPropagation();
			return;
		}

		auto temp = lyWanFa->convertToNodeSpace(touchPoint);

		for(int i = 0 ; i < LIST_MAX_SIZE ; i++)
		{
			auto sptBack = (Sprite *)lyWanFa->getChildByTag(8888 + i);
			if(sptBack == nullptr || i >= MyBetData::getInstance()->getWanfaSize()) continue;
			cocos2d::Rect rect = sptBack->getBoundingBox();	
			if(rect.containsPoint(temp))
			{
				if(m_isNowDel)
				{
					if(MyBetData::getInstance()->delWanfaFromList(i))
					{
						refreshContent();
						refreshMenu();
						pEvent->stopPropagation();
						return;
					}
				} else
				{
					//跳转
					NotificationCenter::getInstance()->postNotification("CQSSCChangeKind", Integer::create(MyBetData::getInstance()->getWanfaAt(i)));
					pEvent->stopPropagation();
					return;
				}				
			}
		}

	} else
	{
		auto temp = m_node->convertToNodeSpace(touchPoint);

		for(int i = 1 ; i <= totalItemSpriteCount ; i++)
		{
			Sprite *kindSprite = (Sprite *)m_node->getChildByTag(i);
			if (kindSprite == nullptr) continue;
			cocos2d::Rect rect = kindSprite->boundingBox();
			if(rect.containsPoint(temp)){			

				//文字copy
				LabelTTF* curTTF = (LabelTTF *)selectedSprite->getChildByTag(9527);
				LabelTTF* bottomTTF = (LabelTTF *)kindSprite->getChildByTag(9527);
				if(curTTF != nullptr && bottomTTF != nullptr) curTTF->setString(bottomTTF->getString());

				selectedSprite->setPosition(kindSprite->getPosition());		
				selectedSprite->setVisible(true);			

				selectedSpriteTag = i;
				if(MyBetData::getInstance()->addWanfaToList(selectedSpriteTag))
				{					
					refreshContent();
					refreshMenu();
				}

			}
		}
	}
}

void BJK8Kind::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

void BJK8Kind::refreshContent()
{
	if(lyWanFa == nullptr) return;

	for(int i = 0;i < LIST_MAX_SIZE;i ++)
	{
		//一定要遍历完
		bool isOutList = i >= MyBetData::getInstance()->getWanfaSize();
		//文字
		auto ltfWord = (LabelTTF *)lyWanFa->getChildByTag(6666 + i);
		if(ltfWord == nullptr) continue;
		ltfWord->setVisible(!isOutList);
		//按钮
		auto sptBack = (Sprite *)lyWanFa->getChildByTag(8888 + i);
		if(sptBack == nullptr) continue;
		sptBack->setVisible(false);

		//值
		if(!isOutList)
		{
			ltfWord->setString(ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d",20 + tagKuai8[MyBetData::getInstance()->getWanfaAt(i)])->getCString()));
			if(m_isNowDel)
			{
				sptBack->setVisible(true);				
			}
		}
	}

}
 
void BJK8Kind::refreshMenu()
{
	if(m_node == nullptr) return;

	for(int i = 1;i < m_node->getChildrenCount();i ++)
	{
		auto sptItem = (Sprite *)m_node->getChildByTag(i);
		if(sptItem == nullptr) continue;
		auto ltfName = (LabelTTF *)sptItem->getChildByTag(9527);
		if(ltfName == nullptr) continue;
		ltfName->setFontFillColor(Color3B::WHITE);
		for(int j = 0;j < MyBetData::getInstance()->getWanfaSize();j ++)
		{
			if(i == MyBetData::getInstance()->getWanfaAt(j))
			{
				ltfName->setFontFillColor(ccc3(255,96,102));
				break;
			}
		}
	}	
}
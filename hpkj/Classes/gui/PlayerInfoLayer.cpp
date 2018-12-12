#include "PlayerInfoLayer.h"
#include "RoomLayer.h"
#include "ChoosePlayerPhotoLayer.h"
#include "GroupSprite.h"
#include "EntityMgr.h"
#include "MovingLabelLayer.h"
#include "utf-8.h"
#include "PromptBox.h"
#include "FreeLayer.h"
#include "ModifyPassWord.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AppController.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

int article_num[20] = {0};

PlayerInfoLayer::PlayerInfoLayer()
{
	photoTable = NULL;
	medalTable = NULL;
	goodsTable = NULL;
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(PlayerInfoLayer::onModifyNickNameResult), MSG_UI_ANS_NICKNAMEUPDATE,NULL);
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(PlayerInfoLayer::onModifyTouxiangResult), MSG_UI_ANS_TOUXIANGUPDATE,NULL);
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(PlayerInfoLayer::setTouxiangModifyResult), MSG_UI_ANS_UPDATETOUXIANGNUM,NULL);
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(PlayerInfoLayer::removeTouxiangLayer), MSG_UI_ANS_UPDATETOUXIANGLAYER,NULL);
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(PlayerInfoLayer::onSureModifyNickName), MSG_UI_ANS_SUREMODYFINICKNAME,NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(PlayerInfoLayer::updateArticle), "UpdateUserPacket", NULL);
}

PlayerInfoLayer::~PlayerInfoLayer()
{
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_NICKNAMEUPDATE);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_TOUXIANGUPDATE);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_UPDATETOUXIANGNUM);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_UPDATETOUXIANGLAYER);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_SUREMODYFINICKNAME);
	NotificationCenter::getInstance()->removeObserver(this,"UpdateUserPacket");
	 this->unschedule(schedule_selector(PlayerInfoLayer::updateUserPacket));
	if(photoTable != NULL)
	{
		photoTable->release();
		photoTable = NULL;
	}
	if(medalTable != NULL)
	{
		medalTable->release();
		medalTable = NULL;
	}
	if(goodsTable != NULL)
	{
		goodsTable->release();
		goodsTable = NULL;
	}
	helper1->release();
}

Scene* PlayerInfoLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		PlayerInfoLayer *layer = PlayerInfoLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

/**���漴����ʾ*/
void PlayerInfoLayer::onEnter()
{
	Layer::onEnter();
	//Director::getInstance()->getTouchDispatcher()->removeDelegate(medalTable);
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(medalTable, kMenuHandlerPriority, false);
}

PlayerInfoLayer *PlayerInfoLayer::create()
{
	Array *normalName = Array::create();
	Array *selectedName = Array::create();
	normalName->addObject(String::create("Sprite_PlayerInfo1.png"));
	selectedName->addObject(String::create("Sprite_PlayerInfo2.png"));
	normalName->addObject(String::create("Sprite_PlayerInfo5.png"));
	selectedName->addObject(String::create("Sprite_PlayerInfo6.png"));
	normalName->addObject(String::create("Sprite_PlayerInfo3.png"));
	selectedName->addObject(String::create("Sprite_PlayerInfo4.png"));
	return PlayerInfoLayer::createWithTabItems(normalName, selectedName);
}

PlayerInfoLayer *PlayerInfoLayer::createWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	PlayerInfoLayer *tabLayer = new PlayerInfoLayer();
	if (tabLayer && tabLayer->initWithTabItems(itemNormalName, itemSelectedName))
	{
		tabLayer->init();
		tabLayer->autorelease();
		return tabLayer;
	}
	else
	{
		CC_SAFE_DELETE(tabLayer);
		return NULL;
	}
}

bool PlayerInfoLayer::init()
{
	bool bRet = false;
	do 
	{
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		photoNum = 2;


		m_bInputNickName = NULL;


		/**��ӵ�ͼ����**/
		photoBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_PlayerInfo13.png"));
		photoBk->setPosition(Vec2(135.0f, 200.5f));
		this->addChild(photoBk, 1);
		//�û���Ƭ
		const char* pSZPhonoName;
		if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
		{
			if(g_GlobalUnits.getGolbalUserData()->wFaceID > 1)
			{
				g_GlobalUnits.getGolbalUserData()->wFaceID = 0;
			}
			pSZPhonoName = String::createWithFormat("dt_player_%d.png",100+g_GlobalUnits.getGolbalUserData()->wFaceID)->getCString();
		}
		else
		{
			if(g_GlobalUnits.getGolbalUserData()->wFaceID > 1)
			{
				g_GlobalUnits.getGolbalUserData()->wFaceID = 0;
			}
			pSZPhonoName = String::createWithFormat("dt_player_%d.png",g_GlobalUnits.getGolbalUserData()->wFaceID)->getCString();
		}
char name[1024];
            string path = UserDefault::getInstance()->getStringForKey("ImageSavedPath", "");
        if (path!="") {
                m_pPlayerPhoto = Sprite::create(path.c_str());
                if (m_pPlayerPhoto != NULL) {
                    float width = m_pPlayerPhoto->boundingBox().size.width;
                    float height = m_pPlayerPhoto->boundingBox().size.height;
                    m_pPlayerPhoto->setScaleY(248.0/height);
                    m_pPlayerPhoto->setScaleX(155.0/width);
                }else{
                    m_pPlayerPhoto = Sprite::createWithSpriteFrame(spriteFrame(pSZPhonoName));
                }
            }else{
                m_pPlayerPhoto = Sprite::createWithSpriteFrame(spriteFrame(pSZPhonoName));
            }
		
m_pPlayerPhoto->setPosition(Vec2(135.0f,206.0f));
		this->addChild(m_pPlayerPhoto,1);

		float startX = 286;
		float startY = 350.0f;
		float gapY = 50;
		ccColor3B textColor = ccc3(255, 192, 0);

		//�û�����
		string pTextUserName;
		if(strlen(g_GlobalUnits.getGolbalUserData()->szNickName)>0)
		{
			CCLOG("have nick name");
			pTextUserName = g_GlobalUnits.getGolbalUserData()->szNickName;
		}
		else
		{
			CCLOG("not have nick name");
			pTextUserName = EntityMgr::instance()->login()->getAccout();
		}

		if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 0)
		{
			m_pUserName = LabelTTF::create(String::createWithFormat("%s%s",pConfigMgr->text("t82"),pTextUserName.c_str())->getCString(),"",22);
		}
		else
		{
			m_pUserName = LabelTTF::create(String::createWithFormat("%s%s",pConfigMgr->text("t82"),pTextUserName.c_str())->getCString(),"",22);
		}
		m_pUserName->setColor(textColor);
		m_pUserName->setAnchorPoint(Vec2(0,0));
		m_pUserName->setPosition(Vec2(startX,startY));
		this->addChild(m_pUserName,1);

		//�û��Ա�
		//Sprite *pButtonBJ = Sprite::create("dt_warning_8.png");
		const char *pTextUserSex;
		if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
		{
			pTextUserSex = String::createWithFormat
				("%s%s",pConfigMgr->text("t83"),pConfigMgr->text("t150"))->getCString();
		}
		else
		{
			pTextUserSex = String::createWithFormat
				("%s%s",pConfigMgr->text("t83"),pConfigMgr->text("t151"))->getCString();
		}

		m_pUserSex = LabelTTF::create(pTextUserSex,"HelveticaNeue-Bold", 22);
		m_pUserSex->setColor(textColor);
		m_pUserSex->setAnchorPoint(Vec2(0,0));
		m_pUserSex->setPosition(Vec2(startX,startY-gapY));
		this->addChild(m_pUserSex,1);

		//�û����
		const char *pTextUserGold = String::createWithFormat
			("%s%d",pConfigMgr->text("t85"),g_GlobalUnits.getGolbalUserData()->lScore)->getCString();
		pUserGold = LabelTTF::create(pTextUserGold,"HelveticaNeue-Bold", 22);
		pUserGold->setColor(textColor);
		pUserGold->setAnchorPoint(Vec2(0,0));
		pUserGold->setPosition(Vec2(startX,startY-2*gapY));
		this->addChild(pUserGold,1);

		//�û�ս��
		const char *pTextUserRecord = String::createWithFormat
			("%s%d%s/%d%s",pConfigMgr->text("t88"),g_GlobalUnits.getGolbalUserData()->lWinCount,pConfigMgr->text("t165"),g_GlobalUnits.getGolbalUserData()->lLostCount,pConfigMgr->text("t166"))->getCString();
		pUserRecord = LabelTTF::create(pTextUserRecord,"HelveticaNeue-Bold", 22);
		pUserRecord->setColor(textColor);
		pUserRecord->setAnchorPoint(Vec2(0,0));
		pUserRecord->setPosition(Vec2(startX,startY-3*gapY));
		this->addChild(pUserRecord,1);

		//�û�ʤ��
		int lTotalCount = g_GlobalUnits.getGolbalUserData()->lWinCount+
			g_GlobalUnits.getGolbalUserData()->lLostCount+
			g_GlobalUnits.getGolbalUserData()->lFleeCount;
		const char *pTextUserWinning;
		if(lTotalCount == 0)
		{
			pTextUserWinning = String::createWithFormat
				("%s%d%%",pConfigMgr->text("t89"),0)->getCString();
		}
		else
		{
			pTextUserWinning = String::createWithFormat
				("%s%.2f%%",pConfigMgr->text("t89"),((float)g_GlobalUnits.getGolbalUserData()->lWinCount/(float)g_GlobalUnits.getGolbalUserData()->lTotalCount)*100)->getCString();
		}

		pUserWinning = LabelTTF::create(pTextUserWinning,"HelveticaNeue-Bold", 22);
		pUserWinning->setColor(textColor);
		pUserWinning->setAnchorPoint(Vec2(0,0));
		pUserWinning->setPosition(Vec2(startX,startY-4*gapY));
		this->addChild(pUserWinning,1);
        
        Sprite * changePassNormal = Sprite::create("Button_ModifyPass1.png");
        Sprite * changePassSelect = Sprite::create("Button_ModifyPass2.png");
//        Sprite * changePassNormal = Sprite::createWithSpriteFrame(spriteFrame("Button_ModifyPass1.png"));
//        Sprite * changePassSelect = Sprite::createWithSpriteFrame(spriteFrame("Button_ModifyPass2.png"));
        MenuItemSprite* change = MenuItemSprite::create(changePassNormal,changePassSelect,CC_CALLBACK_1(PlayerInfoLayer::changePassWord,this));
        changePassMenu = Menu::create(change,NULL);
        changePassMenu->setPosition(Vec2(600,150));
       this->addChild(changePassMenu, 1);


		Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_PlayerInfo9.png"));
		tableViewCellSize = tempSprite->getContentSize();
		//��Ƭ
		cocos2d::Size tableViewSize;
		tableViewSize.width = 560;
		tableViewSize.height = tempSprite->getContentSize().height;
		photoTable = TableView::create(this, tableViewSize);
		photoTable->setDelegate(this);
		photoTable->setAnchorPoint(Vec2(0.5, 0.5));
		photoTable->setPosition(Vec2(256, 32));
		photoTable->setDirection(TableView::Direction::HORIZONTAL);
		photoTable->retain();
		this->addChild(photoTable, 1);
		photoTable->reloadData();

		//ѫ��
		MedalTableHelper *helper = new MedalTableHelper(this, 0);
		tableViewSize.width = SCREEN_HEIGHT;
		tableViewSize.height = 379;
		medalTable = TableView::create(helper, tableViewSize);
		medalTable->setContentSize(tableViewSize);
		medalTable->setDelegate(helper);
		medalTable->setAnchorPoint(Vec2(0.5, 0.5));
		medalTable->setPosition(Vec2(0, 10));
		medalTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		medalTable->setDirection(ScrollView::Direction::VERTICAL);
		medalTable->retain();
		this->addChild(medalTable, 1);
		medalTable->reloadData();
		medalTable->setVisible(false);

		labelMedalInfo = LabelTTF::create("","HelveticaNeue-Bold", 30, cocos2d::Size(500,0),kCCTextAlignmentCenter);
		labelMedalInfo->setAnchorPoint(Vec2(0.5f,0.5f));
		labelMedalInfo->setColor(textColor);
		labelMedalInfo->setPosition(Vec2(0,0));
		this->addChild(labelMedalInfo,2);
		labelMedalInfo->setVisible(false);

		//����
		goodsBk = Sprite::create("Bk_Article.png");
		goodsBk->setAnchorPoint(Vec2(0.5,0));
		goodsBk->setPosition(Vec2(winSize.width/2, 0));
		this->addChild(goodsBk, 1);
		goodsBk->setVisible(false);

		helper1 = new MedalTableHelper(this, 1);
		helper1->retain();
		tableViewSize.width = SCREEN_HEIGHT;
		tableViewSize.height = 360;
		goodsTable = TableView::create(helper1, tableViewSize);
		goodsTable->setContentSize(tableViewSize);
		goodsTable->setDelegate(helper1);
		goodsTable->setAnchorPoint(Vec2(0.5, 0.5));
		goodsTable->setPosition(Vec2(0, 10));
		goodsTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		goodsTable->setDirection(ScrollView::Direction::VERTICAL);
		goodsTable->retain();
		this->addChild(goodsTable, 2);
		goodsTable->reloadData();
		goodsTable->setVisible(false);

		this->schedule(schedule_selector(PlayerInfoLayer::updateUserPacket), 1, true, 0);	
		bRet = true;
		this->setKeypadEnabled(true);
	} while (0);

	return bRet;

}

void PlayerInfoLayer::updateUserPacket(float dt)
{
    pMsgDispatch->SendPacketWithGetUserPacket();
}

void PlayerInfoLayer::changePassWord(Object* obj)
{
    playButtonSound();
    Director::getInstance()->replaceScene(ModifyPassWord::scene());
}


void PlayerInfoLayer::doSomething()
{
	CCLOG("PlayerInfoLayer::doSomething");
	if(currentItem == TabLayerBaseTag){
		m_pPlayerPhoto->setVisible(true);						//�û�ͷ��
		photoBk->setVisible(true);								//ͷ�񱳾�
		m_pUserName->setVisible(true);						//�û�����
		m_pUserSex->setVisible(true);							//�û��Ա�
		pUserGold->setVisible(true);							//�û����
		pUserRecord->setVisible(true);						//�û�ս��
		pUserWinning->setVisible(true);						//�û�ʤ��
		photoTable->setVisible(true);
        changePassMenu->setVisible(true);

		medalTable->setVisible(false);
		labelMedalInfo->setVisible(false);

		goodsTable->setVisible(false);
		goodsBk->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+2){
		m_pPlayerPhoto->setVisible(false);						//�û�ͷ��
		photoBk->setVisible(false);								//ͷ�񱳾�
		m_pUserName->setVisible(false);						//�û�����
		m_pUserSex->setVisible(false);							//�û��Ա�
		pUserGold->setVisible(false);							//�û����
		pUserRecord->setVisible(false);						//�û�ս��
		pUserWinning->setVisible(false);						//�û�ʤ��
		photoTable->setVisible(false);
        changePassMenu->setVisible(false);

		medalTable->setVisible(false);
		labelMedalInfo->setVisible(false);

		goodsTable->setVisible(true);
		goodsBk->setVisible(true);
	}else if(currentItem == TabLayerBaseTag+1){
		m_pPlayerPhoto->setVisible(false);						//�û�ͷ��
		photoBk->setVisible(false);								//ͷ�񱳾�
		m_pUserName->setVisible(false);						//�û�����
		m_pUserSex->setVisible(false);							//�û��Ա�
		pUserGold->setVisible(false);							//�û����
		pUserRecord->setVisible(false);						//�û�ս��
		pUserWinning->setVisible(false);						//�û�ʤ��
		photoTable->setVisible(false);
        changePassMenu->setVisible(false);

		medalTable->setVisible(true);
		labelMedalInfo->setVisible(false);

		goodsTable->setVisible(false);
		goodsBk->setVisible(false);
	}
}
/**��Ԫ���С
������table��Ԫ������ı��
*/
cocos2d::Size PlayerInfoLayer::cellSizeForTable(TableView *table)
{
	cocos2d::Size size = tableViewCellSize;
	size.width+=30;
	return size;
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t PlayerInfoLayer::numberOfCellsInTableView(TableView *table)
{
	int size;
	size = 3;
	return size;
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *PlayerInfoLayer::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell(); 
	cell->autorelease(); 
    char name[1024] = {0};
    if (index == 2) {
        string path = UserDefault::getInstance()->getStringForKey("ImageSavedPath", "");
        if (path!="") {
            std::sprintf(name, "%s", path.c_str());
            Sprite *tempSprite = Sprite::create(name);
            if (tempSprite == NULL) {
                memset(name, 0, sizeof(name));
                std::sprintf(name, "Sprite_PlayerInfo%d.png", index+9);
            }
        }else{
            std::sprintf(name, "Sprite_PlayerInfo%d.png", index+9);
        }
    }else{
	std::sprintf(name, "Sprite_PlayerInfo%d.png", index+9);
    }
	Vec2 pos = cell->getPosition();
	Sprite *normalSprite= Sprite::createWithSpriteFrame(spriteFrame(name));
//	Sprite *selectSprite = Sprite::createWithSpriteFrame(spriteFrame(name));
    float spriteWidth = normalSprite->boundingBox().size.width;
    float spriteHeight = normalSprite->boundingBox().size.height;
	normalSprite->setScaleX(76.0/spriteWidth);
	normalSprite->setScaleY(76.0/spriteHeight);
//    normalSprite->setContentSize(cocos2d::Size(76.0, 76.0));
//    selectSprite->setContentSize(cocos2d::Size(76.0*0.9, 76.0*0.9));
//	selectSprite->setScaleX(76.0/spriteWidth*0.9f);
//	selectSprite->setScaleY(76.0/spriteHeight*0.9f);
//	MenuItemSprite *pButton = MenuItemSprite::create
//    (normalSprite,selectSprite,CC_CALLBACK_1(PlayerInfoLayer::pressChangePhoto));
//	Menu *menu = Menu::createWithItem(pButton);
//	menu->setAnchorPoint(Vec2(0.5f,0.5f));
	normalSprite->setPosition(Vec2(2.5f+tableViewCellSize.width/2,tableViewCellSize.height/2));
	cell->addChild(normalSprite);
    cell->setTag(index);
	if(index>=photoNum)
	{
//		menu->setEnabled(false);
	}
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void PlayerInfoLayer::tableCellTouched(TableView *table, TableViewCell *cell)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
        [[AppController sharedAppController] turnToImageView];
#endif
}

void PlayerInfoLayer::scrollViewDidScroll(ScrollView* view)
{

}
void PlayerInfoLayer::scrollViewDidZoom(ScrollView* view)
{

}

void PlayerInfoLayer::pressChangePhoto(Object *pObject)
{

}

void PlayerInfoLayer::dismiss(Object *obj){
	playButtonSound();
	this->removeFromParentAndCleanup(true);
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void PlayerInfoLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		dismiss(NULL);
	}
}

void PlayerInfoLayer::changeUserName(Object *obj)
{
	playButtonSound();
	if(!m_bInputNickName)
	{	
		m_pNickNameInput->setTouchEnabled(true);
		m_pNickNameInput->setVisible(true);
		m_bInputNickName = true;
		return;
	}

	m_szModifyNickName = const_cast<char*>( m_pNickNameInput->getText());

	if(!isLegal(m_szModifyNickName))
	{
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(427,240),mPromptNickNameError);
		this->addChild(box,10);
		return;
	}

	if(strlen(m_szModifyNickName) > 12 || strlen(m_szModifyNickName) < 4)
	{
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(427,240),mPromptTypeErrorNickName);
		this->addChild(box,10);
	}
	else
	{
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(427,240),mPromptTypeMakeSureModifyNickName);
		this->addChild(box,10);
	}
}

void PlayerInfoLayer::onSureModifyNickName(Object* obj)
{
	if(!pMsgDispatch->connectLoginServer())
	{
		return;
	}
	/*char* szModifyNickName = const_cast<char*>( m_pNickNameInput->getText());*/
	//char buf[50];
	//memset(buf, 0, 50);
	////utf::utf_gb_convert("utf-8", "gb2312", buf,50, szModifyNickName, strlen(szModifyNickName));
	//utf::utf_gb_convert("utf-8", "gb2312", buf,50, m_szModifyNickName, strlen(m_szModifyNickName));
	pMsgDispatch->modifyNickName(m_szModifyNickName);
}

void PlayerInfoLayer::setTouxiangModifyResult(Object* obj)
{
	Integer* inter = (Integer*)obj;
	m_nGenterResult = inter->getValue()/10;
	m_nHeadResult = inter->getValue()%10;
}

void PlayerInfoLayer::onModifyTouxiangResult(Object* obj)
{
	Integer* pNickNameResult = (Integer*)obj;
	int i = pNickNameResult->getValue();
	if(i == 0)
	{
		//�����Ա�
		g_GlobalUnits.getGolbalUserData()->cbGender = m_nGenterResult;
		const char *pTextUserSex;
		if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
		{
			pTextUserSex = String::createWithFormat
				("%s%s",pConfigMgr->text("t83"),pConfigMgr->text("t150"))->getCString();
		}
		else
		{
			pTextUserSex = String::createWithFormat
				("%s%s",pConfigMgr->text("t83"),pConfigMgr->text("t151"))->getCString();
		}

		m_pUserSex->setString(pTextUserSex);

		//����ͷ��
		g_GlobalUnits.getGolbalUserData()->wFaceID = m_nHeadResult;
		const char* pSZPhonoName;
		if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
		{
			if(g_GlobalUnits.getGolbalUserData()->wFaceID > 1)
			{
				g_GlobalUnits.getGolbalUserData()->wFaceID = 0;
			}
			pSZPhonoName = String::createWithFormat("dt_player_%d.png",100+g_GlobalUnits.getGolbalUserData()->wFaceID)->getCString();
		}
		else
		{
			if(g_GlobalUnits.getGolbalUserData()->wFaceID > 1)
			{
				g_GlobalUnits.getGolbalUserData()->wFaceID = 0;
			}
			pSZPhonoName = String::createWithFormat("dt_player_%d.png",g_GlobalUnits.getGolbalUserData()->wFaceID)->getCString();
		}
		m_pPlayerPhoto->setSpriteFrame(spriteFrame(pSZPhonoName));
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t408"),Vec2(427,240));
		this->addChild(layer,20);
		//�ɹ�
	}
	else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t162"),Vec2(427,240));
		this->addChild(layer);
		//ʧ��
	}
}

void PlayerInfoLayer::onModifyNickNameResult(Object* obj)
{
	Integer* pNickNameResult = (Integer*)obj;
	int i = pNickNameResult->getValue();
	if(i == 0)
	{
		if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 1)
		{
			m_pUserName->setString(String::createWithFormat("%s%s",pConfigMgr->text("t82"),m_szModifyNickName)->getCString());
		}
		else
		{
			m_pUserName->setString(String::createWithFormat("%s%s",pConfigMgr->text("t82"),m_szModifyNickName)->getCString());
		}
		strcpy(g_GlobalUnits.getGolbalUserData()->szNickName, m_szModifyNickName);
		pMenu1->setVisible(false);
		pMenu1->removeFromParentAndCleanup(true);
		pMenu1 = NULL;
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t409"),Vec2(427,240));
		this->addChild(layer,20);
		//�ɹ�
	}
	else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t162"),Vec2(427,240));
		this->addChild(layer);
		//ʧ��
	}
	if(m_bInputNickName)
	{
		m_pNickNameInput->removeFromParentAndCleanup(true);
		m_pNickNameInput = NULL;
		m_bInputNickName = false;
	}
}

void PlayerInfoLayer::rechange(Object* obj)
{
	playButtonSound();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//�͵ȼ� = 1 �еȼ� = 2 �ߵȼ� = 3 ��X = 1 ûX = 2
	if(g_GlobalUnits.getGolbalUserData()->lScore < pConfigMgr->m_Config.nServerEnter[(((1-1) % 3 ) << 1) + 1])
	{
		showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"1","5");
	}
	else if(g_GlobalUnits.getGolbalUserData()->lScore < pConfigMgr->m_Config.nServerEnter[(((2-1) % 3 ) << 1) + 1])
	{
		showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"2","5");
	}
	else
	{
		showAndroidRechageLayer(String::createWithFormat("%d",g_GlobalUnits.getGolbalUserData()->dwUserID)->getCString(),"3","5");
	}
#endif
}
//void PlayerInfoLayer::saveChange(Object *obj){

//}
void PlayerInfoLayer::changeUserPhoto(Object *obj){
	playButtonSound();
	this->forbidButtom();
	if(m_bInputNickName){
		m_pNickNameInput->setVisible(false);
		m_pNickNameInput->setTouchEnabled(false);
		m_bInputNickName = false;
	}
	ChoosePlayerPhotoLayer *pChoosePlayerPhoto = ChoosePlayerPhotoLayer::create();
	this->addChild(pChoosePlayerPhoto,5,100);

}

void PlayerInfoLayer::removeTouxiangLayer(Object* obj)
{
	ChoosePlayerPhotoLayer *pChoosePlayerPhoto = (ChoosePlayerPhotoLayer*)this->getChildByTag(100);
	pChoosePlayerPhoto->removeFromParentAndCleanup(true);
	this->permitButtom();
}
/**��ֹ�ý�������а�ť����**/
void PlayerInfoLayer::forbidButtom(){
	pCloseButton->setEnabled(false);
	pMenuPlayer->setEnabled(false);
	mianfeiMenu->setEnabled(false);
	if(pMenu1)
	{
		pMenu1->setEnabled(false);
	}
	//pMenu2->setEnabled(false);
}
/**�򿪸ý�������а�ť����**/
void PlayerInfoLayer::permitButtom(){
	pCloseButton->setEnabled(true);
	pMenuPlayer->setEnabled(true);
	mianfeiMenu->setEnabled(true);
	if(pMenu1)
	{
		pMenu1->setEnabled(true);
	}
	//pMenu2->setEnabled(true);
}
void PlayerInfoLayer::toFreeLayer(Object* obj)
{
	playButtonSound();
	Director::getInstance()->replaceScene(FreeLayer::scene());
}

bool PlayerInfoLayer::isLegal(string str)
{
	//const char* s = str.c_str();
	char s[128] = {0};
	memcpy(s, str.c_str(), min(sizeof(s)-1, str.length()));
	CCLOG("get user accout str = %s, strlen(str)=%d",str.c_str(), strlen(str.c_str()));
	CCLOG("get user accout char = %s strlen(s)=%d", s, strlen(s));
	int i = 0;
	while (i++ < str.length() - 1)
	{
		if (s[i] < 0 || s[i] > 127)
		{
			//("����");
			CCLOG("registLayer chinese");
		}
		else if (s[i]>='0' && s[i]<='9')
		{
			//("����");
			CCLOG("registLayer number");
		}
		else if (s[i]>='A' && s[i]<='Z' || s[i]>='a' && s[i]<='z')
		{
			CCLOG("registLayer a b c");
		}
		else
		{
			//("����");
			CCLOG("RegistLayer::isLegal false s[%d]:%d %s", i, s[i], &s[i]);
			return  false;
		}
	}
	return  true;
}

void PlayerInfoLayer::updateArticle(Object *obj)
{
	Array *data = (Array *)obj;
	for(int i = 0 ; i < data->count(); i+=2)
	{
		Integer *number1 = (Integer *)data->objectAtIndex(i);
		Integer *number2 = (Integer *)data->objectAtIndex(i+1);
		int key = number1->getValue();
		int value = number2->getValue();
		article_num[key-2] = value;
	}
	this->reloadArticleData();
}

void PlayerInfoLayer::reloadArticleData()
{
	if(goodsTable!=NULL){
		goodsTable->reloadData();
	}
}

MedalTableHelper::MedalTableHelper(Object *fath, int tag)
{
	father = fath;
	tableTag = tag;
	winSize = Director::getInstance()->getWinSize();
	if(tag == 0){
		Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat("%s0.png", SPRITE_MEDAL_L)->getCString()));
		tableItemSize = tempSprite->getContentSize();
		tableGapX = (winSize.width-tableItemSize.width*MEDAL_TABLE_COLUMN)/(MEDAL_TABLE_COLUMN+1);
		tableGapY = 10;
	}else{
		Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("article1.png"));
		tableItemSize = tempSprite->getContentSize();
		tableGapX = (winSize.width-tableItemSize.width*ARTICLE_TABLE_COLUMN)/(ARTICLE_TABLE_COLUMN+1);
		tableGapY = 50;
		articleTableItemsNum = ARTICLE_NUM;
	}
}

MedalTableHelper::~MedalTableHelper()
{

}

/**��Ԫ���С
������table��Ԫ������ı��
*/
cocos2d::Size MedalTableHelper::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(winSize.width, tableItemSize.height+tableGapY);
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t MedalTableHelper::numberOfCellsInTableView(TableView *table)
{
	int size;
	if(tableTag == 0){
		size = MEDAL_NUM/MEDAL_TABLE_COLUMN;
		if(MEDAL_NUM%MEDAL_TABLE_COLUMN!=0)
		{
			size++;
		}
	}else{
		size = ARTICLE_NUM/ARTICLE_TABLE_COLUMN;
		if(ARTICLE_NUM%ARTICLE_TABLE_COLUMN!=0)
		{
			size++;
		}
	}
	return size;
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *MedalTableHelper::tableCellAtIndex(TableView *table,ssize_t index)
{
	if(tableTag == 0){
		static const char * cellIdentifier = "cell-identifier";
		TableViewCell *cell = NULL;
		//cell = table->dequeueCell();
		if(cell == NULL)
		{
			cell = new TableViewCell();  
			cell->autorelease(); 
			char name[32];
			float startX = tableItemSize.width/2+tableGapX;
			float startY = tableItemSize.height/2+tableGapY;
			for(int i = 0 ; i < MEDAL_TABLE_COLUMN&&(((int)index*MEDAL_TABLE_COLUMN+i)<MEDAL_NUM) ; i++)
			{
            srand(time(NULL));
            if(rand()%2==0){
                std::sprintf(name, "%s%d.png", SPRITE_MEDAL_A, (int)index*MEDAL_TABLE_COLUMN+i);
            }else{
                std::sprintf(name, "%s%d.png", SPRITE_MEDAL_L, (int)index*MEDAL_TABLE_COLUMN+i);
            }
				Scale9Sprite *medalSprite = Scale9Sprite::createWithSpriteFrame(spriteFrame(name));
				ControlButton *button = ControlButton::create(medalSprite);
				button->setPreferredSize(medalSprite->getPreferredSize());
				button->setTag(((int)index*MEDAL_TABLE_COLUMN+i));
				button->setPosition(Vec2(startX, startY));
				button->addTargetWithActionForControlEvents(father, cccontrol_selector(PlayerInfoLayer::showMedalInfo), Control::EventType::TOUCH_DOWN);
				cell->addChild(button);
				startX+=tableItemSize.width+tableGapX;
			}
		}
		return cell;  
	}else{
		static const char * cellIdentifier = "cell-identifier";
		TableViewCell *cell = new TableViewCell();  
		cell->autorelease(); 

		Sprite *cellBk = Sprite::createWithSpriteFrame(spriteFrame("article_gap.png"));
		cellBk->setAnchorPoint(Vec2(0.5,0));
		cellBk->setPosition(Vec2(400, 0));
		cell->addChild(cellBk);

		char name[32];
		float startX = tableItemSize.width/2+tableGapX;
		float startY = tableItemSize.height/2+tableGapY;
		for(int i = 1 ; i <= ARTICLE_TABLE_COLUMN&&(((int)index*ARTICLE_TABLE_COLUMN+i)<=ARTICLE_NUM) ; i++)
		{
			std::sprintf(name, "article%d.png", (int)index*ARTICLE_TABLE_COLUMN+i);
			Sprite *sprite = Sprite::createWithSpriteFrame(spriteFrame(name));
			sprite->setPosition(Vec2(startX, startY));
			cell->addChild(sprite);
			memset(name, 0, sizeof(name));	

			String *str = String::createWithFormat("%s %d %s", 
				pConfigMgr->text(String::createWithFormat("t%d",2010+(int)index*ARTICLE_TABLE_COLUMN+i)->getCString())
				, article_num[(int)index*ARTICLE_TABLE_COLUMN+i],
				pConfigMgr->text(String::createWithFormat("t%d",2020+(int)index*ARTICLE_TABLE_COLUMN+i)->getCString()));

			LabelTTF *label = LabelTTF::create(str->getCString(), "", 20);
			label->setPosition(Vec2(startX, startY-70));
			cell->addChild(label);

			//Sprite * buyNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyArticle1.png"));
			//Sprite * buySelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyArticle2.png"));
			//MenuItemSprite *buyMenuItem = MenuItemSprite::create(buyNormalSprite, 
			//	buySelectSprite, father, CC_CALLBACK_1(ShopSystemLayer::buyButton,this));
			//buyMenuItem->setTag(BuyButton_Article+index);
			//Menu *buyMenu = Menu::create(buyMenuItem,NULL);
			//buyMenu->setPosition(Vec2(startX+tableItemSize.width/2-buyNormalSprite->getContentSize().width/2-8,
			//	startY-tableItemSize.height/2+buyNormalSprite->getContentSize().height/2+8));
			//cell->addChild(buyMenu);
			startX+=tableItemSize.width+tableGapX;
		}
		return cell;  
	}
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void MedalTableHelper::tableCellTouched(TableView *table, TableViewCell *cell)
{

}

void MedalTableHelper::scrollViewDidScroll(ScrollView* view)
{

}
void MedalTableHelper::scrollViewDidZoom(ScrollView* view)
{

}

void PlayerInfoLayer::showMedalInfo(Object *obj, Control::EventType event)
{
	Node *target = ((Node *)obj);
	Node *parentNode = target->getParent();
	int tag = target->getTag();
	if(event == Control::EventType::TOUCH_DOWN)
	{
		ccColor3B textColor = ccc3(0,255,0);
		labelMedalInfo->setString(pConfigMgr->text(String::createWithFormat("t%d", 720+tag)->getCString()));
		labelMedalInfo->setColor(textColor);
		labelMedalInfo->setPosition(ccpAdd(target->convertToWorldSpace(target->getPosition()), Vec2(0,0)));
		labelMedalInfo->setPositionX(target->getPositionX());
		CCTintTo *tintTo = CCTintTo::create(1.0f,0,50,0);
		//FiniteTimeAction *sequence = Sequence::create(CCShow::create(), tintTo, CCHide::create(), NULL);
		FiniteTimeAction *sequence = Sequence::create(CCShow::create(), DelayTime::create(1.5f), CCHide::create(), NULL);
		labelMedalInfo->stopAllActions();
		labelMedalInfo->runAction(sequence);
	}
}



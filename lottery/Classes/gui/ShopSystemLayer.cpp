#include "ShopSystemLayer.h"
#include "EntityMgr.h"
#include "DBHandler.h"

ShopSystemLayer::ShopSystemLayer()
{
	friendStatusData = NULL;
	myFriendList = NULL;
	coinsTable = NULL;
	articleTable = NULL;
	promptBox = NULL;
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShopSystemLayer::refreshView), "UserInsureInfo", NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ShopSystemLayer::buyGoodsInfoRet), "BuyGoodsInfoRet", NULL);
}

ShopSystemLayer::~ShopSystemLayer()
{
	if(friendStatusData!=NULL)
		friendStatusData->release();
	if(myFriendList!=NULL)
		myFriendList->release();
	if(coinsTable!=NULL)
		coinsTable->release();
	if(articleTable!=NULL)
		articleTable->release();
    NotificationCenter::getInstance()->removeObserver(this, "UserInsureInfo");
    NotificationCenter::getInstance()->removeObserver(this, "BuyGoodsInfoRet");
    this->unschedule(schedule_selector(ShopSystemLayer::updateMoney));
}

ShopSystemLayer *ShopSystemLayer::create()
{
	Array *normalName = Array::create();
	Array *selectedName = Array::create();
	normalName->addObject(String::create("Tab_Bank1.png"));
    selectedName->addObject(String::create("Tab_Bank2.png"));
    normalName->addObject(String::create("Tab_Article3.png"));
    selectedName->addObject(String::create("Tab_Article4.png"));
    //	normalName->addObject(String::create("Tab_Coins1.png"));
    //	selectedName->addObject(String::create("Tab_Coins2.png"));
	return ShopSystemLayer::createWithTabItems(normalName, selectedName);
}

ShopSystemLayer *ShopSystemLayer::createWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	ShopSystemLayer *tabLayer = new ShopSystemLayer();
	if (tabLayer && tabLayer->initWithTabItems(itemNormalName, itemSelectedName))
	{
		tabLayer->initView();
		tabLayer->autorelease();
		return tabLayer;
	}
	else
	{
		CC_SAFE_DELETE(tabLayer);
		return NULL;
	}
}

void ShopSystemLayer::initView()
{
	score = 0;
	bankScore = 0;
	winSize = Director::getInstance()->getWinSize();
	Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopCoinTableBk1.png"));
	tableItemSize = tempSprite->getContentSize();
	tableGapX = (winSize.width-tableItemSize.width*3)/4;
	tableGapY = 4;
	coinsTableItemsNum = 7;
	this->setTabMenuPositionY(winSize.height*0.945f);
    Sprite *sprite = Sprite::create("Bk_Shop.png");
	pBJ->setSpriteFrame(sprite->displayFrame());
	maskSprite->setSpriteFrame(spriteFrame("Sprite_ShopMask1.png"));
	maskSprite2 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopMask2.png"));
	maskSprite2->setPosition(Vec2(winSize.width/2,maskSprite2->getContentSize().height/2));
	this->addChild(maskSprite2,2);
	maskSprite3 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopMask3.png"));
	maskSprite3->setPosition(Vec2(winSize.width/2,maskSprite2->getContentSize().height/2));
	this->addChild(maskSprite3,2);

	//���
	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = 310;
	coinsTable = TableView::create(this, tableViewSize);
	coinsTable->setDelegate(this);
	coinsTable->setAnchorPoint(Vec2(0.5, 0.5));
	coinsTable->setPosition(Vec2(0, 60));
	coinsTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	coinsTable->setDirection(ScrollView::Direction::VERTICAL);
	coinsTable->retain();
	coinsTable->setTag(ShopTableTag_Coins);
	this->addChild(coinsTable, 1);
	//����
	tableViewSize.width = winSize.width;
	tableViewSize.height = 310;
	articleTable = TableView::create(new ArticleTableHelper(this), tableViewSize);
	articleTable->setDelegate(this);
	articleTable->setAnchorPoint(Vec2(0.5, 0.5));
	articleTable->setPosition(Vec2(0, 60));
	articleTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	articleTable->setDirection(ScrollView::Direction::VERTICAL);
	articleTable->retain();
	articleTable->setTag(ShopTableTag_Article);
	this->addChild(articleTable, 1);
	articleTable->setVisible(false);

	//���չ�
	bankBk = Sprite::createWithSpriteFrame(spriteFrame("Bk_ShopBank.png"));
	bankBk->setAnchorPoint(Vec2(0.5,0));
	bankBk->setPosition(Vec2(winSize.width/2, 40));
	this->addChild(bankBk,1);	
	float textWidth = 500;
	char info[100]={0};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	sprintf(info, "%I64d", score);
#else
	sprintf(info, "%lld", score);
#endif
	scoreLabel = LabelTTF::create(info,"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	scoreLabel->setColor(ccc3(255,255,255));
	scoreLabel->setAnchorPoint(Vec2(0,0.5f));
	scoreLabel->setPosition(Vec2(465,228));
	this->addChild(scoreLabel,1);

	memset(info, 0, sizeof(info));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	sprintf(info, "%I64d", bankScore);
#else
	sprintf(info, "%lld", bankScore);
#endif
	bankScoreLabel = LabelTTF::create(info,"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	bankScoreLabel->setColor(ccc3(255,255,255));
	bankScoreLabel->setAnchorPoint(Vec2(0,0.5f));
	bankScoreLabel->setPosition(Vec2(465,178));
	this->addChild(bankScoreLabel,1);
	//��Ӱ�ť
	Sprite * depositNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_ShopBankDeposit1.png"));
	Sprite * depositSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_ShopBankDeposit2.png"));
	Sprite * withdrawNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_ShopBankWithdraw1.png"));
	Sprite * withdrawSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_ShopBankWithdraw2.png"));
	Sprite * sendNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_ShopBankSend1.png"));
	Sprite * sendSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_ShopBankSend2.png"));
	MenuItemSprite *depositMenuItem = MenuItemSprite::create(depositNormalSprite, 
		depositSelectSprite,  CC_CALLBACK_1(ShopSystemLayer::depositButton,this));
	MenuItemSprite *withdrawMenuItem = MenuItemSprite::create(withdrawNormalSprite, 
		withdrawSelectSprite,  CC_CALLBACK_1(ShopSystemLayer::withdrawButton,this));
	MenuItemSprite *sendMenuItem = MenuItemSprite::create(sendNormalSprite, 
		sendSelectSprite,  CC_CALLBACK_1(ShopSystemLayer::sendButton,this));
	bankMenu = Menu::create(depositMenuItem,withdrawMenuItem,sendMenuItem,NULL);
	bankMenu->setPosition(Vec2(winSize.width * 0.5, 101));	
	bankMenu->alignItemsHorizontallyWithPadding(0.5);//�������з�ʽ��������20
	this->addChild(bankMenu,1);
	bankBk->setVisible(false);
	scoreLabel->setVisible(false);
	bankScoreLabel->setVisible(false);
	bankMenu->setVisible(false);
	/************����**********/
	buyBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyBk.png"));
	buyBk->setAnchorPoint(Vec2(0.5f,0));
	buyBk->setPosition(Vec2(winSize.width/2,88));
	this->addChild(buyBk,2);

	buyText = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyLabel.png"));
	buyText->setPosition(Vec2(winSize.width/2,340));
	this->addChild(buyText,2);


	textWidth = 100;
	//���ͱ�ǩ
	buyArticleType1 = LabelTTF::create(ConfigMgr::instance()->text("t558"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	buyArticleType1->setColor(ccc3(255,255,255));
	buyArticleType1->setPosition(Vec2(241,261));
	this->addChild(buyArticleType1,2);
	//��Ʒ���͵׿�
	buyArticleTypeBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png"));
	buyArticleTypeBk->setAnchorPoint(Vec2(0,0.5f));
	buyArticleTypeBk->setPosition(Vec2(284,261));
	buyArticleTypeBk->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(buyArticleTypeBk,2);
	//��Ʒ����
	buyArticleType2 = LabelTTF::create(ConfigMgr::instance()->text("t558"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	buyArticleType2->setColor(ccc3(138,99,64));
	buyArticleType2->setPosition(Vec2(winSize.width/2+30,261));
	this->addChild(buyArticleType2,2);

	//������ǩ
	buyArticleNumLabel = LabelTTF::create(ConfigMgr::instance()->text("t559"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	buyArticleNumLabel->setColor(ccc3(255,255,255));
	buyArticleNumLabel->setPosition(Vec2(241,202));
	this->addChild(buyArticleNumLabel,2);

	//�������������
	buyArticleNum = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	buyArticleNum->setPosition(Vec2(284+buyArticleTypeBk->getContentSize().width/2, 202));
	buyArticleNum->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	buyArticleNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	buyArticleNum->setAnchorPoint(Vec2(0.5f,0.5f));
	buyArticleNum->setFontColor(ccc3(0,0,0));
	buyArticleNum->setFont("",30);
	this->addChild(buyArticleNum,2);

	//����ȷ��ȡ���˵�
	Sprite * okNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk1.png"));
	Sprite * okSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk2.png"));
	Sprite * cancelNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel1.png"));
	Sprite * cancelSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel2.png"));
	MenuItemSprite *okMenuItem = MenuItemSprite::create(okNormalSprite, 
		okSelectSprite,  CC_CALLBACK_1(ShopSystemLayer::pressOk,this));
	MenuItemSprite *canceldrawMenuItem = MenuItemSprite::create(cancelNormalSprite, 
		cancelSelectSprite,  CC_CALLBACK_1(ShopSystemLayer::pressCancel,this));
	buyMenu = Menu::create(okMenuItem,canceldrawMenuItem,NULL);
	buyMenu->setPosition(Vec2(winSize.width * 0.5, 136));	
	buyMenu->alignItemsHorizontallyWithPadding(20);
	this->addChild(buyMenu,2);
	this->setBuyViewVisible(false);
	/************ȡ��**********/
	withdrawBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyBk.png"));
	withdrawBk->setAnchorPoint(Vec2(0.5f,0));
	withdrawBk->setPosition(Vec2(winSize.width/2,88));
	this->addChild(withdrawBk,1);

	withdrawText = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopWithdrawLabel.png"));
	withdrawText->setPosition(Vec2(winSize.width/2,340));
	this->addChild(withdrawText,1);

	textWidth = 120;

	//������ǩ
	withdrawNumLabel = LabelTTF::create(ConfigMgr::instance()->text("t569"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	withdrawNumLabel->setColor(ccc3(255,255,255));
	withdrawNumLabel->setPosition(Vec2(241,261));
	this->addChild(withdrawNumLabel,1);

	//�������������
	withdrawNum = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	withdrawNum->setPosition(Vec2(309, 261));
	withdrawNum->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	withdrawNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	withdrawNum->setAnchorPoint(Vec2(0,0.5f));
	withdrawNum->setFontColor(ccc3(0,0,0));
	withdrawNum->setFont("",30);
	this->addChild(withdrawNum,1);

	//�����ǩ
	withdrawPasswordLabel = LabelTTF::create(ConfigMgr::instance()->text("t570"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	withdrawPasswordLabel->setColor(ccc3(255,255,255));
	withdrawPasswordLabel->setPosition(Vec2(241,202));
	this->addChild(withdrawPasswordLabel,1);

	//���������
	withdrawPassword = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	withdrawPassword->setPosition(Vec2(309, 202));
	withdrawPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	withdrawPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	withdrawPassword->setAnchorPoint(Vec2(0,0.5f));
	withdrawPassword->setFontColor(ccc3(0,0,0));
	withdrawPassword->setFont("",30);
	this->addChild(withdrawPassword,1);

	//����ȷ��ȡ���˵�
	Sprite * okNormalSprite2 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk1.png"));
	Sprite * okSelectSprite2 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk2.png"));
	Sprite * cancelNormalSprite2 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel1.png"));
	Sprite * cancelSelectSprite2 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel2.png"));
	MenuItemSprite *okMenuItem2 = MenuItemSprite::create(okNormalSprite2, 
		okSelectSprite2,  CC_CALLBACK_1(ShopSystemLayer::pressOk,this));
	MenuItemSprite *canceldrawMenuItem2 = MenuItemSprite::create(cancelNormalSprite2, 
		cancelSelectSprite2,  CC_CALLBACK_1(ShopSystemLayer::pressCancel,this));
	withdrawMenu = Menu::create(okMenuItem2,canceldrawMenuItem2,NULL);
	withdrawMenu->setPosition(Vec2(winSize.width * 0.5, 136));	
	withdrawMenu->alignItemsHorizontallyWithPadding(20);
	this->addChild(withdrawMenu,1);
	this->setWithdrawViewVisible(false);

	/************���**********/
	depositBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyBk.png"));
	depositBk->setAnchorPoint(Vec2(0.5f,0));
	depositBk->setPosition(Vec2(winSize.width/2,88));
	this->addChild(depositBk,1);

	depositText = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopDepositLabel.png"));
	depositText->setPosition(Vec2(winSize.width/2,340));
	this->addChild(depositText,1);

	textWidth = 120;

	//������ǩ
	depositNumLabel = LabelTTF::create(ConfigMgr::instance()->text("t571"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	depositNumLabel->setColor(ccc3(255,255,255));
	depositNumLabel->setPosition(Vec2(241,261));
	this->addChild(depositNumLabel,1);

	//�������������
	depositNum = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	depositNum->setPosition(Vec2(309, 261));
	depositNum->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	depositNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	depositNum->setAnchorPoint(Vec2(0,0.5f));
	depositNum->setFontColor(ccc3(0,0,0));
	depositNum->setFont("",30);
	this->addChild(depositNum,1);

	//�����ǩ
	depositPasswordLabel = LabelTTF::create(ConfigMgr::instance()->text("t570"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	depositPasswordLabel->setColor(ccc3(255,255,255));
	depositPasswordLabel->setPosition(Vec2(241,202));
	this->addChild(depositPasswordLabel,1);

	//���������
	depositPassword = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	depositPassword->setPosition(Vec2(309, 202));
	depositPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	depositPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	depositPassword->setAnchorPoint(Vec2(0,0.5f));
	depositPassword->setFontColor(ccc3(0,0,0));
	depositPassword->setFont("",30);
	this->addChild(depositPassword,1);

	//����ȷ��ȡ���˵�
	Sprite * okNormalSprite1 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk1.png"));
	Sprite * okSelectSprite1 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk2.png"));
	Sprite * cancelNormalSprite1 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel1.png"));
	Sprite * cancelSelectSprite1 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel2.png"));
	MenuItemSprite *okMenuItem1 = MenuItemSprite::create(okNormalSprite1, 
		okSelectSprite1,  CC_CALLBACK_1(ShopSystemLayer::pressOk,this));
	MenuItemSprite *canceldrawMenuItem1 = MenuItemSprite::create(cancelNormalSprite1, 
		cancelSelectSprite1,  CC_CALLBACK_1(ShopSystemLayer::pressCancel,this));
	depositMenu = Menu::create(okMenuItem1,canceldrawMenuItem1,NULL);
	depositMenu->setPosition(Vec2(winSize.width * 0.5, 136));	
	depositMenu->alignItemsHorizontallyWithPadding(20);
	this->addChild(depositMenu,1);
	this->setDepositViewVisible(false);

	/************ת��**********/
	transferBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyBk.png"));
	transferBk->setAnchorPoint(Vec2(0.5f,0));
	transferBk->setPosition(Vec2(winSize.width/2,88));
	this->addChild(transferBk,1);

	transferText = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopSendLabel.png"));
	transferText->setPosition(Vec2(winSize.width/2,340));
	this->addChild(transferText,1);

	textWidth = 120;

	//ת���ǳƱ�ǩ
	transferNickname = LabelTTF::create(ConfigMgr::instance()->text("t573"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	transferNickname->setColor(ccc3(255,255,255));
	transferNickname->setPosition(Vec2(241,286));
	this->addChild(transferNickname,1);

	//�ǳ������
	nickname = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	nickname->setPosition(Vec2(309, 286));
	nickname->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	nickname->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	nickname->setAnchorPoint(Vec2(0,0.5f));
	nickname->setFontColor(ccc3(0,0,0));
	nickname->setFont("",30);
	this->addChild(nickname,1);

	//������ǩ
	transferNumLabel = LabelTTF::create(ConfigMgr::instance()->text("t574"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	transferNumLabel->setColor(ccc3(255,255,255));
	transferNumLabel->setPosition(Vec2(241,231));
	this->addChild(transferNumLabel,1);

	//�������������
	transferNum = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	transferNum->setPosition(Vec2(309, 231));
	transferNum->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	transferNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	transferNum->setAnchorPoint(Vec2(0,0.5f));
	transferNum->setFontColor(ccc3(0,0,0));
	transferNum->setFont("",30);
	this->addChild(transferNum,1);

	//�����ǩ
	transferPasswordLabel = LabelTTF::create(ConfigMgr::instance()->text("t570"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	transferPasswordLabel->setColor(ccc3(255,255,255));
	transferPasswordLabel->setPosition(Vec2(241,176));
	this->addChild(transferPasswordLabel,1);

	//���������
	transferPassword = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	transferPassword->setPosition(Vec2(309, 176));
	transferPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	transferPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	transferPassword->setAnchorPoint(Vec2(0,0.5f));
	transferPassword->setFontColor(ccc3(0,0,0));
	transferPassword->setFont("",30);
	this->addChild(transferPassword,1);

	//����ȷ��ȡ���˵�
	Sprite * okNormalSprite3 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk1.png"));
	Sprite * okSelectSprite3 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk2.png"));
	Sprite * cancelNormalSprite3 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel1.png"));
	Sprite * cancelSelectSprite3 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel2.png"));
	MenuItemSprite *okMenuItem3 = MenuItemSprite::create(okNormalSprite3, 
		okSelectSprite3,  CC_CALLBACK_1(ShopSystemLayer::pressOk,this));
	MenuItemSprite *canceldrawMenuItem3 = MenuItemSprite::create(cancelNormalSprite3, 
		cancelSelectSprite3,  CC_CALLBACK_1(ShopSystemLayer::pressCancel,this));
	transferMenu = Menu::create(okMenuItem3,canceldrawMenuItem3,NULL);
	transferMenu->setPosition(Vec2(winSize.width * 0.5, 125));	
	transferMenu->alignItemsHorizontallyWithPadding(20);
	this->addChild(transferMenu,1);
	this->setSendViewVisible(false);
    
    this->schedule(schedule_selector(ShopSystemLayer::updateMoney), 1, true, 0);
    this->queryBankInfo();
    currentItem = TabLayerBaseTag;
    this->doSomething();
    this->setKeypadEnabled(true);
    //this->setTouchEnabled(true);
}

void ShopSystemLayer::updateMoney(float dt)
{
    //this->queryBankInfo();
}

void ShopSystemLayer::doSomething()
{
	if(buyViewVisible == true)
	{
		return;
	}
	CCLOG("ShopSystemLayer::doSomething");
	if(currentItem == TabLayerBaseTag){
		maskSprite2->setVisible(false);
		maskSprite3->setVisible(false);
		coinsTable->setVisible(false);
		articleTable->setVisible(false);
        
		bankBk->setVisible(true);
		scoreLabel->setVisible(true);
		bankScoreLabel->setVisible(true);
		bankMenu->setVisible(true);
        
	}else if(currentItem == TabLayerBaseTag+2){
		maskSprite2->setVisible(true);
		maskSprite3->setVisible(true);
		coinsTable->setVisible(true);
		articleTable->setVisible(false);
        
		bankBk->setVisible(false);
		scoreLabel->setVisible(false);
		bankScoreLabel->setVisible(false);
		bankMenu->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+1){
		maskSprite2->setVisible(true);
		maskSprite3->setVisible(true);
		coinsTable->setVisible(false);
		articleTable->setVisible(true);

		bankBk->setVisible(false);
		scoreLabel->setVisible(false);
		bankScoreLabel->setVisible(false);
		bankMenu->setVisible(false);
	}
}
void ShopSystemLayer::refreshView(Object *pObject)
{
    score = EntityMgr::instance()->getDispatch()->m_dwlUserScore;
    bankScore = EntityMgr::instance()->getDispatch()->m_dwlUserInsure;
    char str[100];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sprintf(str, "%I64d", score);
#else
    sprintf(str, "%lld", score);
#endif
    scoreLabel->setString(str);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sprintf(str, "%I64d", bankScore);
#else
    sprintf(str, "%lld", bankScore);
#endif
    bankScoreLabel->setString(str);
    if(promptBox!=NULL)
    {
        promptBox->setPromptText(EntityMgr::instance()->getDispatch()->szDescribeString);
        
        char message[1024];
        time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ��cuo
        tm* t= localtime(&tt);
        sprintf(message, "insert into PersonalMsg(msg) values(\'%s     %d-%02d-%02d %02d:%02d:%02d\')", EntityMgr::instance()->getDispatch()->szDescribeString,
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        DBHandler::sharedDBHandler()->execute(message);
    }
}

void ShopSystemLayer::buyGoodsInfoRet(Object *pObject)
{
	if(promptBox!=NULL)
	{
		promptBox->setPromptText(EntityMgr::instance()->getDispatch()->szDescribeString);

		char message[1024];
		time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ��cuo
		tm* t= localtime(&tt);
		sprintf(message, "insert into PersonalMsg(msg) values(\'%s     %d-%02d-%02d %02d:%02d:%02d\')", EntityMgr::instance()->getDispatch()->szDescribeString,
			t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		DBHandler::sharedDBHandler()->execute(message);
	}
}

void ShopSystemLayer::queryBankInfo()
{
	EntityMgr::instance()->getDispatch()->SendPacketWithQueryBankInfo();
}

void ShopSystemLayer::setBuyViewVisible(bool flag)
{
	buyViewVisible = flag;
	buyBk->setVisible(flag);
	buyText->setVisible(flag);
	buyArticleType1->setVisible(flag);
	buyArticleTypeBk->setVisible(flag);
	buyArticleType2->setVisible(flag);
	buyArticleNumLabel->setVisible(flag);
	buyArticleNum->setVisible(flag);
	buyMenu->setVisible(flag);
    if (flag == false) {
        buyArticleNum->setPositionY(buyArticleNum->getPositionY()+1000);
    }else{
        buyArticleNum->setPositionY(buyArticleNum->getPositionY()-1000);
    }
}

void ShopSystemLayer::setDepositViewVisible(bool flag)
{
	depositViewVisible = flag;
	depositBk->setVisible(flag);
	depositText->setVisible(flag);
	depositNumLabel->setVisible(flag);
	depositNum->setVisible(flag);
	depositPasswordLabel->setVisible(flag);
	depositPassword->setVisible(flag);
	depositMenu->setVisible(flag);
}

void ShopSystemLayer::setWithdrawViewVisible(bool flag)
{
	withdrawViewVisible = flag;
	withdrawBk->setVisible(flag);
	withdrawText->setVisible(flag);
	withdrawNumLabel->setVisible(flag);
	withdrawNum->setVisible(flag);
	withdrawPasswordLabel->setVisible(flag);
	withdrawPassword->setVisible(flag);
	withdrawMenu->setVisible(flag);
}

void ShopSystemLayer::setSendViewVisible(bool flag)
{
	sendViewVisible = flag;
	transferBk->setVisible(flag);
	transferText->setVisible(flag);
	transferNickname->setVisible(flag);
	nickname->setVisible(flag);
	transferNumLabel->setVisible(flag);
	transferNum->setVisible(flag);
	transferPasswordLabel->setVisible(flag);
	transferPassword->setVisible(flag);
	transferMenu->setVisible(flag);
}

void ShopSystemLayer::buyButton(Object *obj)
{
	if(buyViewVisible == true)
	{
		return;
	}
	objTag = ((Node *)obj)->getTag();
	if(objTag == BuyButton_Coins)
	{

	}
	if(objTag >= BuyButton_Article+1 && objTag <= BuyButton_Article + 9)
	{
        String *str = String::createWithFormat("t%d", 2010+objTag-BuyButton_Article);
        buyArticleType2->setString(ConfigMgr::instance()->text(str->getCString()));
	}
	this->setBuyViewVisible(true);
}

void ShopSystemLayer::depositButton(Object *obj)
{
	if(depositViewVisible == false)
	{
		setDepositViewVisible(true);
	}
}

void ShopSystemLayer::withdrawButton(Object *obj)
{
	if(withdrawViewVisible == false)
	{
		setWithdrawViewVisible(true);
	}
}

void ShopSystemLayer::sendButton(Object *obj)
{
	if(sendViewVisible == false)
	{
		setSendViewVisible(true);
	}
}

void ShopSystemLayer::pressOk(Object *obj)
{
	if(buyViewVisible == true){
		this->setBuyViewVisible(false);
	}else if(depositViewVisible == true){
		int inputScore = atol(depositNum->getText());
        if(inputScore<=score||true)
		{
			EntityMgr::instance()->getDispatch()->SendPacketWithDeposit(inputScore);
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(ConfigMgr::instance()->text("t556"));
			this->addChild(promptBox,2);
		}else{
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(ConfigMgr::instance()->text("t577"));
			this->addChild(promptBox,2);
		}
		this->setDepositViewVisible(false);
	}else if(withdrawViewVisible == true){
		int inputScore = atol(withdrawNum->getText());
		if(inputScore<=bankScore||true)
		{
			string passStr = withdrawPassword->getText();
			EntityMgr::instance()->getDispatch()->SendPacketWithWithdraw(inputScore, passStr);
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(ConfigMgr::instance()->text("t556"));
			this->addChild(promptBox,2);
		}else{
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(ConfigMgr::instance()->text("t577"));
			this->addChild(promptBox,2);
		}
		this->setWithdrawViewVisible(false);
	}else if(sendViewVisible == true){
		int inputScore = atol(transferNum->getText());
		if(inputScore<=bankScore||true)
		{
			string nicknameStr = nickname->getText();
			string passStr = transferPassword->getText();
			EntityMgr::instance()->getDispatch()->SendPacketWithSendScore(nicknameStr, inputScore, passStr, 0);
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(ConfigMgr::instance()->text("t556"));
			this->addChild(promptBox,2);
		}else{
			promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(ConfigMgr::instance()->text("t577"));
			this->addChild(promptBox,2);
		}
		this->setSendViewVisible(false);
    }
    
    if(currentItem == TabLayerBaseTag+1)
    {
        DWORD num = atol(buyArticleNum->getText());
        if (num<=0) {
            promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
            promptBox->setPromptText(ConfigMgr::instance()->text("t2030"));
           this->addChild(promptBox,2);
        }else{
            EntityMgr::instance()->getDispatch()->SendPacketWithBuyGoods(objTag-200+2, num);
            promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
            promptBox->setPromptText(ConfigMgr::instance()->text("t556"));
           this->addChild(promptBox,2);
        }
    }
}

void ShopSystemLayer::pressCancel(Object *obj)
{
	if(buyViewVisible == true){
		this->setBuyViewVisible(false);
	}else if(depositViewVisible == true){
		this->setDepositViewVisible(false);
	}else if(withdrawViewVisible == true){
		this->setWithdrawViewVisible(false);
	}else if(sendViewVisible == true){
		this->setSendViewVisible(false);
	}
}

/**��Ԫ���С
������table��Ԫ������ı��
*/
cocos2d::Size ShopSystemLayer::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(winSize.width, tableItemSize.height+tableGapY);
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t ShopSystemLayer::numberOfCellsInTableView(TableView *table)
{
	int size;
	size = coinsTableItemsNum/3;
	if(coinsTableItemsNum%3!=0)
	{
		size++;
	}
	return size;
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *ShopSystemLayer::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	char name[32];
	float startX = tableItemSize.width/2+tableGapX;
	float startY = tableItemSize.height/2+tableGapY;
	for(int i = 1 ; i < 4&&(((int)index*3+i)<=coinsTableItemsNum) ; i++)
	{
		sprintf(name, "%s%d.png", ShopCoinTableFileName, (int)index*3+i);
		Sprite *cellBk = Sprite::createWithSpriteFrame(spriteFrame(name));
		cellBk->setPosition(Vec2(startX, startY));
		cell->addChild(cellBk);

		memset(name, 0, sizeof(name));
		sprintf(name, "%s%d_1.png", BuyCoinButtonFileName, (int)index*3+i);
		Sprite * buyNormalSprite = Sprite::createWithSpriteFrame(spriteFrame(name));
		memset(name, 0, sizeof(name));
		sprintf(name, "%s%d_2.png", BuyCoinButtonFileName, (int)index*3+i);
		Sprite * buySelectSprite = Sprite::createWithSpriteFrame(spriteFrame(name));
		MenuItemSprite *buyMenuItem = MenuItemSprite::create(buyNormalSprite, 
			buySelectSprite,  CC_CALLBACK_1(ShopSystemLayer::buyButton,this));
		buyMenuItem->setTag(BuyButton_Coins+(int)index*3+i);
		Menu *buyMenu = Menu::create(buyMenuItem,NULL);
		buyMenu->setPosition(Vec2(startX+tableItemSize.width/2-buyNormalSprite->getContentSize().width/2-8,
			startY-tableItemSize.height/2+buyNormalSprite->getContentSize().height/2+8));
		cell->addChild(buyMenu);

		startX+=tableItemSize.width+tableGapX;
	}
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void ShopSystemLayer::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//LabelTTF *nameLabel = (LabelTTF *)cell->getChildByTag(nameLabelTag);
	//const char *name = nameLabel->getString();
	//textField->setString(name);
	//this->removeChild(_tableView, true);
	//flag = 0;
}

void ShopSystemLayer::scrollViewDidScroll(ScrollView* view)
{

}
void ShopSystemLayer::scrollViewDidZoom(ScrollView* view)
{

}

/**��Ԫ���С
	������table��Ԫ������ı��
	*/
ArticleTableHelper::ArticleTableHelper(Object *fath)
{
	father = fath;
	winSize = Director::getInstance()->getWinSize();
	Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopCoinTableBk1.png"));
	tableItemSize = tempSprite->getContentSize();
	tableGapX = (winSize.width-tableItemSize.width*3)/4;
	tableGapY = 4;
	articleTableItemsNum = 9;
}

ArticleTableHelper::~ArticleTableHelper()
{

}


void ArticleTableHelper::buyButton(Object *obj)
{

}

cocos2d::Size ArticleTableHelper::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(winSize.width, tableItemSize.height+tableGapY);
}

/**����е�Ԫ��ĸ���
	������table��Ԫ������ı��
	*/
ssize_t ArticleTableHelper::numberOfCellsInTableView(TableView *table)
{
	int size;
	size = articleTableItemsNum/3;
	if(articleTableItemsNum%3!=0)
	{
		size++;
	}
	return size;
}

/**��Ԫ���ʼ��
	������table��Ԫ������ı��
	*/
TableViewCell *ArticleTableHelper::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	char name[32];
	float startX = tableItemSize.width/2+tableGapX;
	float startY = tableItemSize.height/2+tableGapY;
	for(int i = 1 ; i < 4&&(((int)index*3+i)<=articleTableItemsNum) ; i++)
	{
		sprintf(name, "%s%d.png", ShopArticleTableFileName, (int)index*3+i);
		Sprite *cellBk = Sprite::createWithSpriteFrame(spriteFrame(name));
		cellBk->setPosition(Vec2(startX, startY));
		cell->addChild(cellBk);

		memset(name, 0, sizeof(name));	
		Sprite * buyNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyArticle1.png"));
		Sprite * buySelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyArticle2.png"));
		MenuItemSprite *buyMenuItem = MenuItemSprite::create(buyNormalSprite,buySelectSprite, NULL);
		buyMenuItem->setTag(BuyButton_Article+(int)index*3+i);
		Menu *buyMenu = Menu::create(buyMenuItem,NULL);
		buyMenu->setPosition(Vec2(startX+tableItemSize.width/2-buyNormalSprite->getContentSize().width/2-8,
			startY-tableItemSize.height/2+buyNormalSprite->getContentSize().height/2+8));
		cell->addChild(buyMenu);
		startX+=tableItemSize.width+tableGapX;
	}
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void ArticleTableHelper::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//LabelTTF *nameLabel = (LabelTTF *)cell->getChildByTag(nameLabelTag);
	//const char *name = nameLabel->getString();
	//textField->setString(name);
	//this->removeChild(_tableView, true);
	//flag = 0;
}

void ArticleTableHelper::scrollViewDidScroll(ScrollView* view)
{

}
void ArticleTableHelper::scrollViewDidZoom(ScrollView* view)
{

}
#include "RoomLayer.h"
#include "CocosUnits.h"
#include "GuessCoinSide.h"

GuessCoinSide::GuessCoinSide()
{
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GuessCoinSide::refreshView), "UserInsureInfo", NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GuessCoinSide::guessCoinRet), "GuessCoinRet", NULL);
}
GuessCoinSide::~GuessCoinSide()
{
    NotificationCenter::getInstance()->removeObserver(this, "UserInsureInfo");
    NotificationCenter::getInstance()->removeObserver(this, "GuessCoinRet");
    this->unschedule(schedule_selector(GuessCoinSide::updateMoney));
}
Scene* GuessCoinSide::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GuessCoinSide *layer = GuessCoinSide::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool GuessCoinSide::init()
{
	bool bRet = false;
	do 
	{
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		/**ªÊ÷∆∞ÎÕ∏√˜√…∞Â**/
		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
		//layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);
		canRotate = true;
		this->initView();
		this->initLabel();
		bRet = true;
        this->schedule(schedule_selector(GuessCoinSide::updateMoney), 1, true, 0);
		this->setKeypadEnabled(true);
	} while (0);

	return bRet;

}

void GuessCoinSide::updateMoney(float dt)
{
    CCLOG("GuessCoinSide::updateMoney");
    this->queryBankInfo();
}

void GuessCoinSide::initView()
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	/**ªÊ÷∆±≥æ∞Õº∆¨**/
	Sprite * pBJ = Sprite::create("Bk_LoadView.png");
	pBJ->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(pBJ,0);

	float gap = 10;

	Sprite * pBJ1 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideBk1.png"));
	pBJ1->setPosition(Vec2(pBJ1->getContentSize().width/2+gap,pBJ1->getContentSize().height/2+gap));
	this->addChild(pBJ1,0);

	pBJ2 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideBk2.png"));
	pBJ2->setPosition(Vec2(winSize.width-pBJ2->getContentSize().width/2-gap,pBJ2->getContentSize().height/2+gap));
	this->addChild(pBJ2,0);

	coinSprite = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideHead.png"));
	coinSprite->setPosition(Vec2(pBJ1->getPositionX(),pBJ1->getPositionY()+pBJ1->getContentSize().height/2+coinSprite->getContentSize().height/2));
	this->addChild(coinSprite,0);

	float deltaX = 5;
	float deltaY = 5;
	float chipWidth = 65;
	float chipHeight = 79;
	float startPosX = pBJ2->getPositionX()-(chipWidth+deltaX)*3/2;
	float startPosY = pBJ2->getPositionY()+(chipHeight+deltaX)/2;
	//Array *menuItems = Array::create();
	Vector<MenuItem* > menuItems;
	for(int i = 1 ; i <= 7 ; i++){
		if(i == 5){
			startPosX = pBJ2->getPositionX()-(chipWidth+deltaX)*3/2;
			startPosY = startPosY-deltaY-chipHeight;
		}
		Sprite *normalSprite1 = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat("GessCoinSideChip%d_1.png", i)->getCString()));
		Sprite *selectSprite1 = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat("GessCoinSideChip%d_2.png", i)->getCString()));
		MenuItemSprite *menuSprite = MenuItemSprite::create
			(normalSprite1,selectSprite1,CC_CALLBACK_1(GuessCoinSide::pressChip,this));
		menuSprite->setTag(i);
		menuSprite->setPosition(Vec2(startPosX, startPosY));
		//menuItems->addObject(menuSprite);
		menuItems.pushBack(menuSprite);
		startPosX+=deltaX+chipWidth;
	}
	Menu *chipMenu = Menu::createWithArray(menuItems);
	chipMenu->setPosition(Vec2(0,0));
	this->addChild(chipMenu);

	//»°œ˚∞¥≈•
	Sprite *normalSprite = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideCancel1.png"));
	Sprite *selectSprite = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideCancel2.png"));
	MenuItemSprite *menuSprite = MenuItemSprite::create
		(normalSprite,selectSprite,CC_CALLBACK_1(GuessCoinSide::pressCancel,this));

	Menu *cancelMenu= Menu::create(menuSprite,NULL);
	cancelMenu->setPosition(Vec2(pBJ2->getPositionX()+105, pBJ2->getPositionY()+175)); 
	this->addChild(cancelMenu);

	//—π’˝°¢—π∑¥°¢»∑»œœ¬◊¢
	Sprite *normalSprite1 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideHead1.png"));
	Sprite *selectSprite1 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideHead2.png"));
	Sprite *normalSprite2 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideTail1.png"));
	Sprite *selectSprite2 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideTail2.png"));
	Sprite *normalSprite3 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideConfirm1.png"));
	Sprite *selectSprite3 = Sprite::createWithSpriteFrame(spriteFrame("GessCoinSideConfirm2.png"));
	MenuItemSprite *menuSprite1 = MenuItemSprite::create
		(normalSprite1,selectSprite1,CC_CALLBACK_1(GuessCoinSide::betHead,this));
	MenuItemSprite *menuSprite2 = MenuItemSprite::create
		(normalSprite2,selectSprite2,CC_CALLBACK_1(GuessCoinSide::betTail,this));
	MenuItemSprite *menuSprite3 = MenuItemSprite::create
		(normalSprite3,selectSprite3,CC_CALLBACK_1(GuessCoinSide::pressConfirm,this));

	Menu *menu1= Menu::create(menuSprite1, menuSprite2, menuSprite3, NULL);
	menu1->setPosition(Vec2(pBJ2->getPositionX(), pBJ2->getPositionY()-144)); 
	menu1->alignItemsHorizontallyWithPadding(15);
	this->addChild(menu1);

	//πÿ±’∞¥≈•
	Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	MenuItemSprite *pButtonClose = MenuItemSprite::create
		(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(GuessCoinSide::dismiss,this));
	pButtonClose->setAnchorPoint(Vec2(0.5,0.5));
	pbuttonSelectClose->setScale(0.9f);
	Menu *pButton0 = Menu::create(pButtonClose,NULL);
	pButton0->setPosition(Vec2(winSize.width-pbuttonNormalClose->getContentSize().width/2,
		winSize.height-pbuttonNormalClose->getContentSize().height/2));  
	this->addChild(pButton0);
}

void GuessCoinSide::initLabel()
{
	betSide = 0;
	score = 0;
	betScore = 0;
	float fontSize = 30;
	//LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("t2001"), "", fontSize);
	//title->setPosition(Vec2(211,118));
	//this->addChild(title);

	fontSize = 27;
	scoreLabel = LabelTTF::create(String::createWithFormat("%d", score)->getCString(), "", fontSize);
	scoreLabel->setPosition(Vec2(pBJ2->getPositionX()-128-8,pBJ2->getPositionY()+174));
	scoreLabel->setAnchorPoint(Vec2(0,0.5));
	this->addChild(scoreLabel);

	fontSize = 30;
	betScoreLabel = LabelTTF::create(String::createWithFormat("%d", betScore)->getCString(), "", fontSize);
	betScoreLabel->setPosition(Vec2(85,58));
	betScoreLabel->setAnchorPoint(Vec2(0,0.5));
	this->addChild(betScoreLabel);
	
	fontSize = 25;
	betInfoLabel = LabelTTF::create(ConfigMgr::instance()->text("t2006"), "", fontSize);
	betInfoLabel->setPosition(Vec2(385,betScoreLabel->getPositionY()));
	betInfoLabel->setAnchorPoint(Vec2(1,0.5));
	this->addChild(betInfoLabel);

	fontSize = 30;
	LabelTTF *hintLabel = LabelTTF::create(ConfigMgr::instance()->text("t2002"), "", fontSize);
	hintLabel->setPosition(Vec2(pBJ2->getPositionX(),pBJ2->getPositionY()+115));
	this->addChild(hintLabel);
}

void GuessCoinSide::queryBankInfo()
{
	EntityMgr::instance()->getDispatch()->SendPacketWithQueryBankInfo();
}

void GuessCoinSide::refreshView(Object *pObject)
{
	score = EntityMgr::instance()->getDispatch()->m_dwlUserScore;
    this->refreshScoreLabel();
}

void GuessCoinSide::refreshScoreLabel()
{
    char str[100];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sprintf(str, "%I64d", score);
#else
    sprintf(str, "%lld", score);
#endif
    scoreLabel->setString(str);
}

void GuessCoinSide::guessCoinRet(Object *pObject)
{
    Array *data = (Array *)pObject;
    //判断正反
    coinSideRet = ((Bool *)(data->objectAtIndex(0)))->getValue();
    //下注结果
    scoreEarned = ((Integer *)(data->objectAtIndex(1)))->getValue();
    
    SCORE tempScore = scoreEarned>0?scoreEarned/2:-scoreEarned/2;
    score-=betScore;
    this->refreshScoreLabel();
    this->coinRotate(coinSideRet?0:1);
}

void GuessCoinSide::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		RoomLayer* layer = (RoomLayer*)this->getParent();
		layer->permitButtonClick();
		this->removeFromParentAndCleanup(true);
	}
}

void GuessCoinSide::dismiss(Object *obj){
	//playButtonSound();
	//RoomLayer* layer = (RoomLayer*)this->getParent();
	//layer->permitButtonClick();
	//this->removeFromParentAndCleanup(true);

	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void GuessCoinSide::pressCancel(Object *obj)
{
	betInfoLabel->setString(ConfigMgr::instance()->text("t2006"));
	betScore = 0;
	betSide = 0;
	betScoreLabel->setString("0");
}

void GuessCoinSide::pressChip(Object *obj)
{
    if(canRotate == false)
    {
        return;
    }
    
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	int chipValue[8] = {0,1,5,10,50,100,500,1000};
	int tag = ((MenuItemSprite *)obj)->getTag();
	if(betScore+chipValue[tag]*10000>score){
		PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(ConfigMgr::instance()->text("t2007"));
		this->addChild(promptBox,1);
		return;
	}
    if (betScore+chipValue[tag]*10000>10000000) {
        
        PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
        promptBox->setPromptText(ConfigMgr::instance()->text("t2008"));
       this->addChild(promptBox,1);
        return;
    }
	betScore+=chipValue[tag]*10000;
	betScoreLabel->setString(String::createWithFormat("%d", betScore)->getCString());
	string betInfo = ConfigMgr::instance()->text("t2005");
	if(betSide == 1||betSide == 2){
		betInfo = ConfigMgr::instance()->text(String::createWithFormat("t%d", betSide+2002)->getCString())+betInfo;
	}
	betInfoLabel->setString(betInfo.c_str());
}

void GuessCoinSide::betHead(Object* obj)
{
    if(canRotate == false)
    {
        return;
    }
    
	betSide = 1;
	string betInfo = ConfigMgr::instance()->text("t2003");
	if(betScore!=0)
	{
		betInfo+=ConfigMgr::instance()->text("t2005");
	}else{
		betInfo+=ConfigMgr::instance()->text("t2006");
	}
	betInfoLabel->setString(betInfo.c_str());
}

void GuessCoinSide::betTail(Object* obj)
{
    if(canRotate == false)
    {
        return;
    }
    
	betSide = 2;
	string betInfo = ConfigMgr::instance()->text("t2004");
	if(betScore!=0)
	{
		betInfo+=ConfigMgr::instance()->text("t2005");
	}else{
		betInfo+=ConfigMgr::instance()->text("t2006");
	}
	betInfoLabel->setString(betInfo.c_str());
}

void GuessCoinSide::pressConfirm( Object* obj)
{
	if(canRotate == false)
	{
		return;
	}
	coinSprite->setFlipY(0);
    canRotate = false;
    EntityMgr::instance()->getDispatch()->SendPacketWithGuessCoin(betSide==1?true:false, betScore);
//	this->addRandSeed();
//	int num = rand()%2;
}

void GuessCoinSide::coinRotate(int tag)
{
	int circle = 4;
	OrbitCamera *flipY1 = OrbitCamera::create(0.5f, 1, 0, 0, 90, 0, 0);
	OrbitCamera *flipY2 = OrbitCamera::create(0.5f, 1, 0, 90, 90, 0, 0);
	OrbitCamera *flipY3 = OrbitCamera::create(0.5f, 1, 0, 180, 90, 0, 0);
	OrbitCamera *flipY4 = OrbitCamera::create(0.5f, 1, 0, 270, 90, 0, 0);
	Sequence *sequence = Sequence::create(flipY1, CallFunc::create(this, callfunc_selector(GuessCoinSide::showTailFrame)),
		flipY2, flipY3, CallFunc::create(this, callfunc_selector(GuessCoinSide::showHeadFrame)), flipY4, CallFunc::create(this, callfunc_selector(GuessCoinSide::toZero)), NULL);
	Repeat *repeat = Repeat::create(sequence, circle);
	EaseExponentialOut *easeAction; 
	if(tag == 1){
		Sequence *finalSequence = Sequence::create(repeat, flipY1, CallFunc::create(this, callfunc_selector(GuessCoinSide::showTailFrame)),
			flipY2, NULL);
		easeAction= EaseExponentialOut::create(dynamic_cast<CCActionInterval *>(finalSequence));
	}else{
		easeAction= EaseExponentialOut::create(dynamic_cast<CCActionInterval *>(repeat));
	}
	Sequence *seq = Sequence::createWithTwoActions(easeAction, CallFuncN::create(this, callfuncN_selector(GuessCoinSide::coinRotateEnd)));
	coinSprite->runAction(seq);

}

void GuessCoinSide::showHeadFrame()
{
	coinSprite->setSpriteFrame(spriteFrame("GessCoinSideHead.png"));
}

void GuessCoinSide::showTailFrame()
{
	coinSprite->setSpriteFrame(spriteFrame("GessCoinSideTail.png"));
}

void GuessCoinSide::toZero()
{
	//CCCamera *camera = this->getCamera();
	//camera->setEyeXYZ(0,0,0);
}

void GuessCoinSide::coinRotateEnd(Node *obj)
{
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
	canRotate = true;
    score+=scoreEarned;
    this->refreshScoreLabel();
    String *str;
    SCORE tempScore = scoreEarned-betScore;
    if (tempScore>0) {
        str = String::createWithFormat("%s %lld %s", ConfigMgr::instance()->text("t2045"), tempScore, ConfigMgr::instance()->text("t2046"));
    }else{
        str = String::createWithFormat("%s %lld %s", ConfigMgr::instance()->text("t2047"), tempScore, ConfigMgr::instance()->text("t2048"));
    }
    PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
    promptBox->setPromptText(str->getCString());
   this->addChild(promptBox,3);
}

void GuessCoinSide::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);	
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec/1000;
	srand(rand_seed);
}
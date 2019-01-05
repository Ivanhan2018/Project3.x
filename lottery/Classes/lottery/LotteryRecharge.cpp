#include "LotteryRecharge.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "MyNSString.h"
#include "LotteryRechargeDetail.h"
#include "LotteryMyLot.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecharge::scene()
{
	Scene *scene = Scene::create();
	LotteryRecharge *layer = LotteryRecharge::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecharge::LotteryRecharge()
{
	bankInfo = Array::create();
	bankInfo->retain();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecharge::getChongzhiTypeRet), "GetChongzhiTypeRet", NULL);	
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecharge::getNeedRefresh), "NeedRefreshData", NULL);	
	selectBankSprite = NULL;
}

LotteryRecharge::~LotteryRecharge()
{
	clear();	
}

void LotteryRecharge::getChongzhiTypeRet(Object *obj)
{
	CCLOG(" LotteryRecharge::getChongzhiTypeRet");
	bankInfo->removeAllObjects();
	bankInfo->addObjectsFromArray((Array *)obj);
	
	if(bankInfo->count() == 0){
		bankTable->setBounceable(false);
		
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t953"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		return;
	}
	bankTable->reloadData();
}

void LotteryRecharge::getNeedRefresh(Object *obj)
{
	EntityMgr::instance()->getDispatch()->SendPacketWithGetChongzhiType(0);
}

bool LotteryRecharge::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	EntityMgr::instance()->getDispatch()->SendPacketWithGetChongzhiType(0);
	this->setKeypadEnabled(true);
	return true;
}

void LotteryRecharge::initData()
{
	winSize = Director::getInstance()->getWinSize();
	bankNum = 3;
	tableGap = 15;
	tableHeight = 60+2*tableGap;
	currentBank = 0;
}

void LotteryRecharge::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(225, 225, 225, 225), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	layer->setColor(ccc3(242,243,245));
	layer->setOpacity(255);
	this->addChild(layer);

	//
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryRecharge::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//快速充值
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t590"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 30;
	//选择
	LabelTTF *selectabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t591"), "", fontSize);
	selectabel->setAnchorPoint(Vec2(0,0.5f));
	selectabel->setPosition(Vec2(10, SCREEN_HEIGHT-162 - 40));
	selectabel->setColor(ccc3(50,50,50));
	this->addChild(selectabel, 1);

	//分割线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, selectabel->getPositionY()-40));
	this->addChild(gap1, 1);

	//列表
	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = tableHeight*6;			//默认6个row长度
	bankTable = TableView::create(this, tableViewSize);
	bankTable->setDelegate(this);
	bankTable->setAnchorPoint(Vec2(0.5, 0.5));
	bankTable->setPosition(Vec2(0, gap1->getPositionY()-tableViewSize.height));
	bankTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	bankTable->setDirection(ScrollView::Direction::VERTICAL);
	bankTable->retain();
	this->addChild(bankTable, 1);

	//fontSize = 30;
	//Sprite *rechargeBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_00.png"));
	//rechargeBk->setAnchorPoint(Vec2(0,0.5f));
	//rechargeBk->setPosition(Vec2(0, bankTable->getPositionY()-tableHeight-30));
	//this->addChild(rechargeBk, 1);

	//LabelTTF *rechargeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t591"), "", fontSize);
	//rechargeLabel->setAnchorPoint(Vec2(0,0.5f));
	//rechargeLabel->setPosition(Vec2(selectabel->getPositionX(), rechargeBk->getPositionY()));
	//rechargeLabel->setColor(blackColor);
	//this->addChild(rechargeLabel, 1);

	fontSize = 30;
	//float rechargeInputPosX = 100;
	//rechargeInput = EditBox::create(cocos2d::Size(winSize.width-rechargeInputPosX, rechargeBk->getContentSize().height/2), "blank.png");
	//rechargeInput->setPosition(Vec2(rechargeInputPosX, rechargeBk->getPositionY()));
	//rechargeInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	//rechargeInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	//rechargeInput->setInputMode(ui::EditBox::InputMode::DECIMAL);
	//rechargeInput->setAnchorPoint(Vec2(0,0.5f));
	//rechargeInput->setFontColor(ccc3(0,0,0));
	//rechargeInput->setFont("",fontSize);
	//rechargeInput->setColor(ccc3(112,112,112));
	//rechargeInput->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t593"));
	//rechargeInput->setPlaceholderFontSize(25);
	//rechargeInput->setMaxLength(10);
	//this->addChild(rechargeInput, 1);


	//文字说明
	LabelTTF *hintLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t594"), "", fontSize);
	hintLabel->setAnchorPoint(Vec2(0.5f,0.5f));
	hintLabel->setPosition(Vec2(SCREEN_WIDTH / 2, bankTable->getPositionY()-tableHeight-30));
	hintLabel->setColor(blackColor);
	this->addChild(hintLabel, 1);
	//隐藏
	hintLabel->setVisible(false);

	//确认充值
	Sprite *normalButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_07.png"));
	Sprite *selectButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_08.png"));
	MenuItemSprite *itemSprite = MenuItemSprite::create
		(normalButton,selectButton,CC_CALLBACK_1(LotteryRecharge::recharge,this));
	Menu *rechargeButton = Menu::create(itemSprite,NULL);
//	rechargeButton->setPosition(Vec2(winSize.width / 2, (hintLabel->getPositionY() - hintLabel->getContentSize().height / 2) / 2 -30));
	rechargeButton->setPosition(Vec2(SCREEN_WIDTH / 2, bankTable->getPositionY()-tableHeight-20));
	this->addChild(rechargeButton,1);
}

void LotteryRecharge::back(Object *obj)
{
	playButtonSound();
	//Scene *scene = LotteryMyLot::scene();
	//Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	Director::getInstance()->popScene();
}

void LotteryRecharge::recharge(Object *obj)
{
	playButtonSound();

	//没有获取到数据则返回提示
	if(bankInfo->count() == 0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t953"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		return;
	}
	Dictionary * objData = (Dictionary *)bankInfo->objectAtIndex(currentBank);
	//打开充值链接
	openUrl(objData);
	return;



	Scene *scene = Scene::create();
	LotteryRechargeDetail *layer = LotteryRechargeDetail::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));	

	layer->setBank(currentBank);
	if(bankInfo == NULL || bankInfo->count() == 0){
		return;
	}
	//旧代码：传递到详细页面去展示充值参数，2018.10.22注释
	//	layer->setInfoData((Dictionary *)bankInfo->objectAtIndex(currentBank));
}


//打开充值链接
void LotteryRecharge::openUrl(Dictionary *obj)
{
	playButtonSound();

	if(obj == NULL) return;

	String *url=NULL;	
	Integer *ntypeid = (Integer*)obj->objectForKey("n_t_typeid");

	//增加了参数：userid, ntypeid
	url = String::createWithFormat("%s?userid=%ld&tid=%ld", 
		((String *)obj->objectForKey("s_t_web"))->getCString(),
		EntityMgr::instance()->getDispatch()->m_dwUserID,
		(ntypeid==NULL)? 0 : ntypeid->getValue());
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	TCHART w_url[255] = {0};
	MyNSString::mbs2wc(url->getCString(), strlen(url->getCString()), w_url);
	ShellExecute(NULL, L"open", w_url, NULL, NULL, SW_SHOWNORMAL);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Application::getInstance()->openURL(url->getCString());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Application::getInstance()->openURL(url->getCString());
#endif
}

void LotteryRecharge::onEnter()
{
	Layer::onEnter();
}

void LotteryRecharge::onExit()
{
	Layer::onExit();
}

void LotteryRecharge::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

cocos2d::Size LotteryRecharge::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = SCREEN_WIDTH;
	size.height = tableHeight;
	return size;
}

ssize_t LotteryRecharge::numberOfCellsInTableView(TableView *table)
{
	int num = bankInfo->count();
	if(num == 0)
	{
		num = bankNum;
	}
	//return 3;
	return num;
}
TableViewCell *LotteryRecharge::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cell->setTag(index);

	if(bankInfo->count() == 0){
		return cell;
	}

	//背景
	LayerColor *cellBk = LayerColor::create();//"recharge_00.png"));
	cellBk->setContentSize(cocos2d::Size(SCREEN_WIDTH, tableHeight - 2));
	cellBk->setAnchorPoint(Vec2(0,0));
	cellBk->setPosition(Vec2(0, 0));
	cellBk->setColor(ccc3(255,255,255));
	cellBk->setOpacity(255);
	cell->addChild(cellBk);

	//分割线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	cell->addChild(gap1, 3);

	char filename[255] = {0};
	//银行名称
	Dictionary *myDic = (Dictionary *)bankInfo->objectAtIndex(index);
	String* tempStr = (String* )myDic->objectForKey("s_t_yinhang");
	String* strDesc = (String*) myDic->objectForKey("s_t_desc");

	std::string myStr = tempStr->getCString();

	//初始值
	sprintf(filename, "recharge_03.png");

	const char* tempChar = ConfigMgr::instance()->text("display_DuangDong.xml","t17");
	if(myStr.find(tempChar,0) != std::string::npos)		//是微信充值
	{
		sprintf(filename, "recharge_01.png");
	}
	const char* tempChar1 = ConfigMgr::instance()->text("display_DuangDong.xml","t16");
	if(myStr.find(tempChar1,0)  != std::string::npos)	//是在线充值
	{
		sprintf(filename, "recharge_03.png");
	}	
	const char* tempChar2 = ConfigMgr::instance()->text("display_DuangDong.xml","t41");
	if(myStr.find(tempChar2,0)  != std::string::npos)	//是财付通充值
	{
		sprintf(filename, "recharge_002.png");
	}
	const char* tempChar3 = ConfigMgr::instance()->text("display_DuangDong.xml","t42");
	if(myStr.find(tempChar3,0)  != std::string::npos)	//是支付宝充值
	{
		sprintf(filename, "recharge_02.png");
	}
	const char* tempChar4 = ConfigMgr::instance()->text("display_DuangDong.xml","t43");
	if(myStr.find(tempChar4, 0) != std::string::npos)	//QQ SCAN
		sprintf(filename, "recharge_002.png");

	CCLOG("!!!!!!!!!!!! %s,%s,%s,%s,%s",myStr.c_str(),tempChar,tempChar1,tempChar2,tempChar3);

	//银行
	Sprite *sprite = Sprite::createWithSpriteFrame(spriteFrame(filename));
	sprite->setAnchorPoint(Vec2(0,0.5f));
	sprite->setPosition(Vec2(34, tableHeight / 2));
	cell->addChild(sprite,2);
	//选中的那一行
	if(currentBank == index){		
		//单选择
		selectBankSprite = Sprite::createWithSpriteFrame(spriteFrame("beting28.png"));
		selectBankSprite->setAnchorPoint(Vec2(0.5f,0.5f));
		selectBankSprite->setPosition(Vec2(SCREEN_WIDTH - 34, tableHeight / 2));		
		cell->addChild(selectBankSprite,2);

		//显示DESC信息
		ccColor3B	redColor = ccc3(241,91,90);

		//清理掉以前的文字
		for(int n=0; n<10; n++)
			this->removeChildByTag(DETAIL_INFO_LABEL_TAG + n);

		float fontSize = 38;
		LabelTTF *label = LabelTTF::create(strDesc->getCString(), "微软雅黑", fontSize);
		label->setAnchorPoint(Vec2(0,0.5f));
		label->setDimensions(CCSize(700,200));					//描述信息的长宽
		label->setPosition(Vec2(10, tableHeight/2 +130));		//描述信息的坐标位置
		label->setColor(redColor);
		label->setHorizontalAlignment(kCCTextAlignmentLeft);
		label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
		label->setTag(DETAIL_INFO_LABEL_TAG + index);
		this->addChild(label, 1);
	}else{
		//单选框			
		selectBankSprite = Sprite::createWithSpriteFrame(spriteFrame("beting27.png"));
		selectBankSprite->setAnchorPoint(Vec2(0.5f,0.5f));		
		selectBankSprite->setPosition(Vec2(SCREEN_WIDTH - 34, tableHeight / 2));
		cell->addChild(selectBankSprite,2);
	}	

	return cell;  
}

void LotteryRecharge::tableCellTouched(TableView *table, TableViewCell *cell)
{
	int cellTag = cell->getTag();
	if(cellTag>=bankInfo->count()){
		return;
	}

	bool isNeedRefresh = currentBank != cellTag;

	currentBank = cellTag;

	//刷新table
	if (isNeedRefresh) table->reloadData();
}

void LotteryRecharge::scrollViewDidScroll(ScrollView* view)
{

}
void LotteryRecharge::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecharge::clear()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetChongzhiTypeRet");
	NotificationCenter::getInstance()->removeObserver(this, "NeedRefreshData");
	bankTable->release();
	if(bankInfo)
	{
		bankInfo->removeAllObjects();
		bankInfo->release();
		bankInfo = nullptr;
	}
}
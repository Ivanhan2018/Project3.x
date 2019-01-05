#include "LotterySubExchange.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "LotteryMyLot.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "VersionControl.h"
#include "ValidateLayer.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* LotterySubExchange::scene(const char* username,const int value)
{
	Scene *scene = Scene::create();
	LotterySubExchange *layer = LotterySubExchange::create();
	layer->setTargetID(username, value);
	scene->addChild(layer);
	return scene;
}

LotterySubExchange::LotterySubExchange()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotterySubExchange::zhuanZhangRet), "ZhuanZhangRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotterySubExchange::getLastYueRet), "GetLastYueRet", NULL);  ///余额查询
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotterySubExchange::onValidated), "Validated", NULL); //验证
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotterySubExchange::getTransPhoneRet), "GetTransPhoneRet", NULL);//是否需要验证码
}

LotterySubExchange::~LotterySubExchange()
{
	if(fandianTable != nullptr)
	{
		fandianTable->release();
	}

	NotificationCenter::getInstance()->removeObserver(this, "ZhuanZhangRet");	
	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "Validated"); 
	NotificationCenter::getInstance()->removeObserver(this, "GetTransPhoneRet");
}

bool LotterySubExchange::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	//余额
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
	return true;
}

void LotterySubExchange::initData()
{
	winSize = Director::getInstance()->getWinSize();
	posX1 = 250;
	posX2 = 280;
	gapY = 60;
	tableCellHeight = 40;
	tableCellNum = 0;
	fandianStatus = 0;	

	m_targetID = 0;
	m_index = 0;

	m_isNeedValidate = false;
	m_phoneNum.clear();
}

void LotterySubExchange::initView()
{	
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//头
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotterySubExchange::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t60"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);
	
	fontSize = 32;
	//请认真填写一下信息
	LabelTTF *title1 = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t59"), "", fontSize);
	title1->setAnchorPoint(Vec2(0,0.5f));
	title1->setPosition(Vec2(10, SCREEN_HEIGHT - 200));
	title1->setColor(ccc3(171,71,71));
	title1->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(title1);	

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, title1->getPositionY()-40));
	this->addChild(gap1);

	startY = gap1->getPositionY()-60;

	fontSize = 27;
	int deltaUp = 5;
	for(int i = 0 ; i < 5 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml",
			String::createWithFormat("t%d", 61+i)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX1, startY-gapY*i + (i == 0 ? deltaUp : 0)));
		label->setColor(blackColor);
		this->addChild(label, 1);
		if(i == 0 || i == 2)
		{
			LabelTTF* ltfInfo = LabelTTF::create();
			ltfInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			ltfInfo->setPosition(Vec2(posX2, startY-gapY*i + (i == 0 ? deltaUp : 0)));
			ltfInfo->setFontName("");
			ltfInfo->setFontFillColor(blackColor);
			ltfInfo->setHorizontalAlignment(TextHAlignment::LEFT);
			ltfInfo->setFontSize(fontSize);
			ltfInfo->setString("0");
			ltfInfo->setTag(EDITBOX_TAG_EXCHANGE + i);
			this->addChild(ltfInfo, 1);			
		} else
		{		
			Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
			inputBk->setAnchorPoint(Vec2(0,0.5f));
			inputBk->setPosition(Vec2(posX2-5, startY-gapY*i + (i == 0 ? deltaUp : 0)));
			this->addChild(inputBk);

			EditBox *editBox = EditBox::create(cocos2d::Size(220, 30), "blank.png");
			editBox->setPosition(Vec2(posX2, startY-gapY*i + (i == 0 ? deltaUp : 0)));
			editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
			editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
			editBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
			editBox->setAnchorPoint(Vec2(0,0.5f));
			editBox->setFontColor(blackColor);
			editBox->setFontName("");editBox->setFontSize(fontSize);
			editBox->setColor(ccc3(112,112,112));
			editBox->setMaxLength(20);
			editBox->setTag(EDITBOX_TAG_EXCHANGE+i);
			this->addChild(editBox, 1);	

			if(i == 1)
			{
				editBox->setText(ConfigMgr::instance()->text("display_DuangDong.xml","t69"));
				editBox->setEnabled(false);
			}
			if(i == 4)
			{
				editBox->setInputFlag(EditBox::InputFlag::PASSWORD);
			}
		}
	}	
	
	//确定按钮
	MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotterySubExchange::pressConfirm,this));
	confirmItem->setPosition(Vec2(SCREEN_WIDTH/2, startY - gapY * 5.5f - 15));
	confirmItem->setScale(0.8f);
	Menu *confirmMenu = Menu::create(confirmItem, NULL);
	confirmMenu->setPosition(Vec2(0,0));
	this->addChild(confirmMenu);

	Sprite *gap3 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap3->setAnchorPoint(Vec2(0,0.5));
	gap3->setPosition(Vec2(0, startY-gapY*7));
	this->addChild(gap3);


	/*********************************/
	m_TableNode = Node::create();
	m_TableNode->setPosition(Vec2::ZERO);
	this->addChild(m_TableNode, 1);

	tableCellWidth = 300;
	fandianTableBk = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	fandianTableBk->setAnchorPoint(Vec2(0,0));
	fandianTableBk->setScaleX(tableCellWidth/fandianTableBk->getContentSize().width);
	fandianTableBk->setScaleY(160 / fandianTableBk->getContentSize().height);
	fandianTableBk->setPosition(Vec2::ZERO);
	m_TableNode->addChild(fandianTableBk,1);

	tableViewSize.width = tableCellWidth;
	tableViewSize.height = 160;

	//下拉菜单的弹出位置
	tableViewPos = Vec2(posX2, startY-gapY * 1.5f - 160);

	fandianTable = TableView::create(this, tableViewSize);
	fandianTable->setDelegate(this);
	fandianTable->setAnchorPoint(Vec2(0.5, 0.5));
	fandianTable->setPosition(Vec2::ZERO);
	fandianTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	fandianTable->setDirection(ScrollView::Direction::VERTICAL);
	fandianTable->retain();
	m_TableNode->addChild(fandianTable, 2);

	//下拉按钮
	Sprite *timeButtonNormal1 = Sprite::createWithSpriteFrame(spriteFrame("beting17.png"));
	Sprite *timeButtonSelect1 = Sprite::createWithSpriteFrame(spriteFrame("beting18.png"));
	timeButtonItem1 = MenuItemSprite::create(timeButtonNormal1,timeButtonSelect1,CC_CALLBACK_1(LotterySubExchange::pressSetFandian,this));
	timeButtonItem1->setScale(0.86);
	timeButtonItem1->setPosition(Vec2(posX2-5+300,startY-gapY));

	Menu *timeButton = Menu::create(timeButtonItem1,NULL);
	timeButton->setPosition(Vec2::ZERO);
	this->addChild(timeButton);

	hideTable();

	//发送消息，看是否需要验证
	EntityMgr::instance()->getDispatch()->SendPacketWithGetTransPhone();
}

void LotterySubExchange::hideTable()
{
	m_TableNode->setPosition(Vec2(1000,1000));

	for(int i = 0 ; i < 5 ; i++){
		if(i < 3) continue;			
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE+i);
		if(editBox)	editBox->setEnabled(true);		
	}
}

void LotterySubExchange::showTable()
{
	m_TableNode->setPosition(tableViewPos);
	
	for(int i = 0 ; i < 5 ; i++){
		if(i < 3) continue;
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE+i);
		if(editBox)	editBox->setEnabled(false);
	}
}

void LotterySubExchange::pressConfirm(Object *obj)
{
	playButtonSound();
	
	EditBox *moneyNum = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE+3);
	EditBox *password = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE+4);

	if(moneyNum == nullptr || password == nullptr) return;

	const char* szMoneyNum = moneyNum->getText();
	const char* szPassword = password->getText();

	//设置转帐类型
	if(m_index == 0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t75"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}	
	float money = atof(szMoneyNum);
	//金额不能小于0！
	if (money <= 0) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t76"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,255);
		return;
	}	
	
	//密码不能为空！
	if (strcmp(szPassword, "") == 0) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t77"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,255);
		return;
	}

	//开始判断是否需要弹出验证窗口
	if(m_isNeedValidate)
	{
		ValidateLayer::show(m_phoneNum.c_str(),3); //3为转账验证
	}else
	{
		EntityMgr::instance()->getDispatch()->SendPacketWithZhuanZhang(money,m_targetID,m_index,szPassword);	
	}	
}

void LotterySubExchange::zhuanZhangRet(Object *obj)
{
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();

	//转帐返回
	Dictionary* data = (Dictionary *)obj;
	String* s_t_Desc = (String *)data->objectForKey("s_t_Desc");

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(s_t_Desc->getCString(), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2));
	this->addChild(layer,255);

	//还原现场
	for(int i = 0 ; i < 5 ; i++){
		if(i < 3) continue;			
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE+i);
		if(editBox)	editBox->setText("");
	}	
}

void LotterySubExchange::pressSetFandian(Object *obj)
{
	playButtonSound();

	MenuItemSprite *timeButtonItem = (MenuItemSprite *)obj;
	if (fandianStatus != 0) {
		hideTable();
		fandianStatus = 0;
		timeButtonItem->unselected();
	} else
	{
		timeButtonItem->selected();
		fandianStatus = 1;
		showTable();
	}
	
}

void LotterySubExchange::getLastYueRet(Object *obj)
{
	LabelTTF* ltfMoney = (LabelTTF *)this->getChildByTag(EDITBOX_TAG_EXCHANGE + 2);
	if (ltfMoney == nullptr) return;

	//余额
	Dictionary *dic = (Dictionary *)obj;
	Double *f_t_yue = (Double *)dic->objectForKey("f_t_yue");

	if(f_t_yue == nullptr) return;
	ltfMoney->setString(String::createWithFormat("%.03f %s",f_t_yue->getValue(),ConfigMgr::instance()->text("display_DuangDong.xml", "t178"))->getCString());	
}

void LotterySubExchange::onValidated(Object* obj)
{	
	Integer* tempInt = (Integer *)obj;
	if(tempInt != nullptr && tempInt->getValue() == -1)
	{
		//点击的取消按钮
		return;
	}	
	//验证完毕
	EditBox *moneyNum = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE+3);
	EditBox *password = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE+4);

	if(moneyNum == nullptr || password == nullptr) return;

	const char* szMoneyNum = moneyNum->getText();
	const char* szPassword = password->getText();
	
	float money = atof(szMoneyNum);

	EntityMgr::instance()->getDispatch()->SendPacketWithZhuanZhang(money,m_targetID,m_index,szPassword);	
}	

void LotterySubExchange::getTransPhoneRet(Object* obj)
{
	m_phoneNum.clear();

	auto dic = (Dictionary *)obj;
	if(dic == nullptr) return;

	Integer* n_t_state = (Integer *)dic->objectForKey("n_t_state");
	String* sPhone = (String *)dic->objectForKey("sPhoneNum");

	m_isNeedValidate = n_t_state->getValue() == 1;//1表示有验证
	if(m_isNeedValidate && sPhone != nullptr)
	{
		m_phoneNum.append(sPhone->getCString());
	}
}

void LotterySubExchange::back(Object *obj)
{
	playButtonSound();	
	Director::getInstance()->popScene();
}

void LotterySubExchange::onEnter()
{
	BaseLayer::onEnter();
}

void LotterySubExchange::onExit()
{
	BaseLayer::onExit();
}

void LotterySubExchange::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotterySubExchange::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
	
	if (fandianStatus == 1) {
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG_EXCHANGE + 1);
		if(editBox == nullptr) return;
		editBox->setText(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",69 + cellTag)->getCString()));
		m_index = cellTag;
		fandianStatus = 0;
		timeButtonItem1->unselected();	
	}
	hideTable();
}

cocos2d::Size LotterySubExchange::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(tableCellWidth, tableCellHeight);
}

TableViewCell* LotterySubExchange::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();
	cell->autorelease();
	cell->setTag(idx);

	float fontSize = 27;
	ccColor3B blackColor = ccc3(76,76,76);
	LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",69 + idx)->getCString()), "", fontSize);
	label->setAnchorPoint(Vec2(0,0.5f));
	label->setPosition(Vec2(10, tableCellHeight/2));
	label->setColor(blackColor);
	cell->addChild(label, 1);

	return cell;
}

ssize_t LotterySubExchange::numberOfCellsInTableView(TableView *table)
{
	return 4;
}

void LotterySubExchange::scrollViewDidScroll(ScrollView* view)
{

}

void LotterySubExchange::scrollViewDidZoom(ScrollView* view)
{

}

void LotterySubExchange::refreshLayer()
{

}

void LotterySubExchange::setTargetID(const char* username,const int value)
{
	m_targetID = value;

	LabelTTF* ltfUser = (LabelTTF *)this->getChildByTag(EDITBOX_TAG_EXCHANGE);
	if (ltfUser == nullptr) return;

	ltfUser->setString(String::createWithFormat("%s[%d]", username, value)->getCString());
}
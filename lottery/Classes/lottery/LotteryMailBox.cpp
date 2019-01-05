#include "LotteryMailBox.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "RoomLayer.h"
#include "LotteryRecharge.h"
#include "LotteryWithdraw.h"

#include "LotteryBasicUserInfo.h"
#include "LotteryModifyPassWord.h"
#include "LotteryCardBinding.h"
#include "LotteryMyLot.h"

#include "LotteryRecordRecharge.h"
#include "LotteryRecordWithdraw.h"
#include "LotteryRecordProfit.h"
#include "LotteryRecordProfitStatics.h"
#include "LotteryRecordGame.h"

#include "LotteryAddSub.h"
#include "LotteryRecordSubGambleStatics.h"
#include "LotteryRecordSubGamble.h"
#include "LotteryMemberShip.h"

#include "LotteryRecordSubProfitStatics.h"
#include "LotteryRecordSubProfit.h"
#include "LotteryRecordSubRecharge.h"
#include "LotteryRecordSubWithdraw.h"

#include "LotteryRecordSubGameProfit.h"
#include "LotteryRecordSubGame.h"

#include "LotteryRecordSubMember.h"

//新加
#include "LotterySign.h"
#include "LotteryHappyHouse.h"
#include "LotteryRetBack.h"
#include "LotteryNewerBack.h"

#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

//所有代理功能
Scene* LotteryMailBox::scene()
{
	Scene *scene = Scene::create();
	LotteryMailBox *layer = LotteryMailBox::create();
	scene->addChild(layer);
	return scene;
}

LotteryMailBox::LotteryMailBox()
{
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryMailBox::getInboxMessageRet),"GetInboxMessageRet",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryMailBox::chkInboxMessageRet),"ChkInboxMessageRet",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryMailBox::getXJUserInfoRet),"GetXJUserInfoRet",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryMailBox::sendMessageRet),"SendMessageRet",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryMailBox::getMessageCountRet),"GetMessageCountRet",NULL);
	//删除
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryMailBox::deleteMessageRet),"DeleteMessageRet",NULL);
}

LotteryMailBox::~LotteryMailBox()
{
	NotificationCenter::getInstance()->removeObserver(this,"GetInboxMessageRet");
	NotificationCenter::getInstance()->removeObserver(this,"ChkInboxMessageRet");
	NotificationCenter::getInstance()->removeObserver(this,"GetXJUserInfoRet");
	NotificationCenter::getInstance()->removeObserver(this,"SendMessageRet");
	NotificationCenter::getInstance()->removeObserver(this,"GetMessageCountRet");
	//删除
	NotificationCenter::getInstance()->removeObserver(this,"DeleteMessageRet");

	if(m_arrContent)
	{
		m_arrContent->removeAllObjects();
		m_arrContent->release();
		m_arrContent = nullptr;
	}

	if(m_arrUserInfo)
	{
		m_arrUserInfo->removeAllObjects();
		m_arrUserInfo->release();
		m_arrUserInfo = nullptr;
	}

	if(userInfoTable)
	{
		userInfoTable->release();
	}
}

bool LotteryMailBox::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryMailBox::refreshLayer()
{
	//
}

void LotteryMailBox::initData()
{
	winSize = Director::getInstance()->getWinSize();
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	money = g_GlobalUnits.getGolbalUserData()->lScore;
	cellHeight = 80;
	cellNum = 11;

	row_count = 0;
	
	m_iBoxType = 1;
	m_iPage = 1;
	m_iCurRow = 0;
	m_iPageNum = 15;
	m_iPageCount = 1;

	m_iCurRevType = 1;

	m_isPosting = false;
	m_top_id = 0;

	m_user_page_cur = 1;
	m_user_page_count = 0;

	m_arrContent = Array::create();
	m_arrContent->retain();

	m_arrUserInfo = Array::create();
	m_arrUserInfo->retain();

	userInfoTable = nullptr;
}

void LotteryMailBox::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//背景图
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	//返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryMailBox::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setAnchorPoint(Vec2(0,1));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	Sprite *pGonggaoNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_2.png"));
	Sprite *pGonggaoSelected = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_1.png"));
	Sprite *pMailNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_4.png"));
	Sprite *pMailSelected = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_3.png"));
	MenuItemSprite *pGonggao = MenuItemSprite::create(pGonggaoNormal,pGonggaoSelected,CC_CALLBACK_1(LotteryMailBox::toNotice,this));
	MenuItemSprite *pMail = MenuItemSprite::create(pMailNormal,pMailSelected,CC_CALLBACK_1(LotteryMailBox::toMail,this));
	Menu *title = Menu::create(pGonggao,pMail,NULL);	
	title->setAnchorPoint(Vec2(0.5,0.5));
	title->alignItemsHorizontallyWithPadding(0);
	title->setPosition(Vec2(SCREEN_WIDTH / 2, pCloseButton->getPositionY()));
	this->addChild(title,1);

	//默认开启了邮件
	pMail->selected();

	Sprite *inboxNormal = Sprite::createWithSpriteFrame(spriteFrame("recharge_23_1.png"));
	Sprite *inboxSelected = Sprite::createWithSpriteFrame(spriteFrame("recharge_23_2.png"));
	Sprite *outboxNormal = Sprite::createWithSpriteFrame(spriteFrame("recharge_23_3.png"));
	Sprite *outboxSelected = Sprite::createWithSpriteFrame(spriteFrame("recharge_23_4.png"));
	Sprite *sendboxNormal = Sprite::createWithSpriteFrame(spriteFrame("recharge_23_5.png"));
	Sprite *sendboxSelected = Sprite::createWithSpriteFrame(spriteFrame("recharge_23_6.png"));
	MenuItemSprite *pinbox = MenuItemSprite::create(inboxNormal,inboxSelected,CC_CALLBACK_1(LotteryMailBox::toInbox,this));
	pinbox->setTag(100);
	MenuItemSprite *poutbox = MenuItemSprite::create(outboxNormal,outboxSelected,CC_CALLBACK_1(LotteryMailBox::toOutBox,this));
	poutbox->setTag(200);
	MenuItemSprite *psendbox = MenuItemSprite::create(sendboxNormal,sendboxSelected,CC_CALLBACK_1(LotteryMailBox::toSendBox,this));
	psendbox->setTag(300);
	Menu *title1 = Menu::create(pinbox,poutbox,psendbox,NULL);	
	title1->setAnchorPoint(Vec2(0.5,0.5));
	title1->alignItemsHorizontallyWithPadding(0);
	title1->setTag(111);
	title1->setPosition(Vec2(SCREEN_WIDTH / 2, pCloseButton->getPositionY() - inboxNormal->getContentSize().height));
	this->addChild(title1,1);

	pinbox->selected();

	fontSize = 30;
	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = title1->getPositionY() - 170;
	userInfoTable = TableView::create(this, tableViewSize);
	userInfoTable->setDelegate(this);
	userInfoTable->setAnchorPoint(Vec2(0.5, 0.5));
	userInfoTable->setPosition(Vec2(0, 100));
	userInfoTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	userInfoTable->setDirection(extension::ScrollView::Direction::VERTICAL);
	userInfoTable->retain();
	this->addChild(userInfoTable, 1);

	LayerColor* layer_content = LayerColor::create(ccc4(255,255,255,255));
	layer_content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	layer_content->setContentSize(cocos2d::Size(SCREEN_WIDTH,title->getPositionY() - 70 - inboxNormal->getContentSize().height));
	layer_content->setTag(9527);
	layer_content->setVisible(false);
	this->addChild(layer_content,2);

	//标题
	//auto ltf_notice_title = LabelTTF::create();
	//ltf_notice_title->setTag(9528);
	//ltf_notice_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	//ltf_notice_title->setFontName("");
	//ltf_notice_title->setFontSize(27);
	//ltf_notice_title->setHorizontalAlignment(TextHAlignment::CENTER);
	//ltf_notice_title->setVerticalAlignment(TextVAlignment::CENTER);
	//ltf_notice_title->setDimensions(cocos2d::Size(SCREEN_WIDTH - 60, 80));
	//ltf_notice_title->setFontFillColor(COLOR_BK_TITLE);
	//ltf_notice_title->setPosition(Vec2(SCREEN_WIDTH / 2,title->getPositionY() - 70 - inboxNormal->getContentSize().height));
	//layer_content->addChild(ltf_notice_title);

	//邮件内容
	auto ltf_notice_content = LabelTTF::create();
	ltf_notice_content->setTag(9529);
	ltf_notice_content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	ltf_notice_content->setFontName("");
	ltf_notice_content->setFontSize(24);
	ltf_notice_content->setHorizontalAlignment(TextHAlignment::LEFT);
	ltf_notice_content->setVerticalAlignment(TextVAlignment::TOP);
	ltf_notice_content->setDimensions(cocos2d::Size(SCREEN_WIDTH - 60, 0));
	ltf_notice_content->setFontFillColor(COLOR_BK_GRAY);
	//ltf_notice_content->setPosition(ltf_notice_title->getPosition() + Vec2(0,-80));
	//layer_content->addChild(ltf_notice_content);

	//改变为可滚动的
	auto scl_content = extension::ScrollView::create();
	scl_content->setViewSize(cocos2d::Size(SCREEN_WIDTH - 60, title->getPositionY() - 80 - inboxNormal->getContentSize().height));
	scl_content->setContainer(ltf_notice_content);
	scl_content->setTag(8878);
	scl_content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	scl_content->setPositionX(30);
	scl_content->setBounceable(true);
	scl_content->setContentOffset(Vec2(0,30));
	scl_content->setDirection(extension::ScrollView::Direction::VERTICAL);
	layer_content->addChild(scl_content);

	//内容页底部的回复删除按钮
	auto layer_notice_bottom = LayerColor::create(ccc4(255,255,255,255));
	layer_notice_bottom->setContentSize(cocos2d::Size(SCREEN_WIDTH, 120));
	layer_notice_bottom->setTag(527);
	layer_content->addChild(layer_notice_bottom);
	Sprite *replyNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_30.png"));
	Sprite *replySelect = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_30_2.png"));
	Sprite *delNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_29_2.png"));
	Sprite *delSelect = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_29.png"));
	MenuItemSprite *replyItem = MenuItemSprite::create(replyNormal,replySelect,CC_CALLBACK_1(LotteryMailBox::toReply,this));
	replyItem->setTag(526);
	MenuItemSprite *delItem = MenuItemSprite::create(delNormal,delSelect,CC_CALLBACK_1(LotteryMailBox::toDel,this));
	Menu *replydelMenu = Menu::create(replyItem,delItem,NULL);	
	replydelMenu->setTag(528);
	replydelMenu->setAnchorPoint(Vec2(0.5,0.5));
	replydelMenu->alignItemsHorizontallyWithPadding(30);
	replydelMenu->setPosition(Vec2(SCREEN_WIDTH * 3 / 4, layer_notice_bottom->getContentSize().height / 2));
	layer_notice_bottom->addChild(replydelMenu);

	//底部翻页
	auto layer_bottom = LayerColor::create(ccc4(255,255,255,255));
	layer_bottom->setContentSize(cocos2d::Size(SCREEN_WIDTH, 120));
	this->addChild(layer_bottom);
	Sprite *preNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_12.png"));
	Sprite *preSelected = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_13.png"));
	Sprite *nextNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_14.png"));
	Sprite *nextSelected = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_15.png"));
	MenuItemSprite *preItem = MenuItemSprite::create(preNormal,preSelected,CC_CALLBACK_1(LotteryMailBox::toPre,this));
	MenuItemSprite *nextItem = MenuItemSprite::create(nextNormal,nextSelected,CC_CALLBACK_1(LotteryMailBox::toNext,this));
	Menu *prenextMenu = Menu::create(preItem,nextItem,NULL);	
	prenextMenu->setAnchorPoint(Vec2(0.5,0.5));
	prenextMenu->alignItemsHorizontallyWithPadding(30);
	prenextMenu->setPosition(Vec2(SCREEN_WIDTH * 3 / 4, layer_bottom->getContentSize().height / 2));
	layer_bottom->addChild(prenextMenu);

	const int ltfLen = 120;
	const int marginLeft = 150;
	const int font_size = 26;

	//发件箱的层
	auto layer_send = LayerColor::create(ccc4(255,255,255,255));
	layer_send->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	layer_send->setContentSize(cocos2d::Size(SCREEN_WIDTH,title->getPositionY() - 70 - inboxNormal->getContentSize().height));
	layer_send->setTag(9580);
	layer_send->setVisible(false);
	this->addChild(layer_send,5);

	//类型
	auto ltfType = LabelTTF::create();
	ltfType->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	ltfType->setFontName("");
	ltfType->setFontSize(font_size);
	ltfType->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1525"));
	ltfType->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfType->setVerticalAlignment(TextVAlignment::CENTER);
	ltfType->setDimensions(cocos2d::Size(ltfLen, 60));
	ltfType->setFontFillColor(ccColor3B::BLACK);
	ltfType->setPosition(Vec2(marginLeft,title->getPositionY() - 180));
	layer_send->addChild(ltfType);

	auto ltfName = LabelTTF::create();
	ltfName->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	ltfName->setFontName("");
	ltfName->setFontSize(font_size);
	ltfName->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1522"));
	ltfName->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfName->setVerticalAlignment(TextVAlignment::CENTER);
	ltfName->setDimensions(cocos2d::Size(ltfLen, 60));
	ltfName->setFontFillColor(ccColor3B::BLACK);
	ltfName->setPosition(Vec2(marginLeft,ltfType->getPositionY() - 80));
	layer_send->addChild(ltfName);

	auto ltfTitle = LabelTTF::create();
	ltfTitle->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	ltfTitle->setFontName("");
	ltfTitle->setFontSize(font_size);
	ltfTitle->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1523"));
	ltfTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfTitle->setVerticalAlignment(TextVAlignment::CENTER);
	ltfTitle->setDimensions(cocos2d::Size(ltfLen, 60));
	ltfTitle->setFontFillColor(ccColor3B::BLACK);
	ltfTitle->setPosition(Vec2(marginLeft,ltfName->getPositionY() - 80));
	layer_send->addChild(ltfTitle);

	auto ltfMailContent = LabelTTF::create();
	ltfMailContent->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	ltfMailContent->setFontName("");
	ltfMailContent->setFontSize(font_size);
	ltfMailContent->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1524"));
	ltfMailContent->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfMailContent->setVerticalAlignment(TextVAlignment::CENTER);
	ltfMailContent->setDimensions(cocos2d::Size(ltfLen, 60));
	ltfMailContent->setFontFillColor(ccColor3B::BLACK);
	ltfMailContent->setPosition(Vec2(marginLeft,ltfTitle->getPositionY() - 80));
	layer_send->addChild(ltfMailContent);

	//ltfType
	auto cbBox1 = ui::Button::create("recharge_24_1.png", "recharge_24_2.png", "recharge_24_2.png");
	cbBox1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	cbBox1->setPosition(ltfType->getPosition() + Vec2(20,-10));
	cbBox1->setTag(9600);
	cbBox1->setBright(true);
	layer_send->addChild(cbBox1);

	auto ltfBox1 = LabelTTF::create();
	ltfBox1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	ltfBox1->setFontName("");
	ltfBox1->setFontSize(font_size - 5);
	ltfBox1->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1526"));
	ltfBox1->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfBox1->setVerticalAlignment(TextVAlignment::CENTER);
	ltfBox1->setDimensions(cocos2d::Size(60, 60));
	ltfBox1->setFontFillColor(ccColor3B::BLACK);
	ltfBox1->setPosition(cbBox1->getPosition() + Vec2(30,10));
	layer_send->addChild(ltfBox1);

	auto cbBox2 = ui::Button::create("recharge_24_1.png", "recharge_24_2.png", "recharge_24_2.png");
	cbBox2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	cbBox2->setPosition(ltfType->getPosition() + Vec2(140,-10));
	cbBox2->setTag(9601);
	cbBox2->setBright(true);
	layer_send->addChild(cbBox2);

	auto ltfBox2 = LabelTTF::create();
	ltfBox2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	ltfBox2->setFontName("");
	ltfBox2->setFontSize(font_size - 5);
	ltfBox2->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1527"));
	ltfBox2->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfBox2->setVerticalAlignment(TextVAlignment::CENTER);
	ltfBox2->setDimensions(cocos2d::Size(60, 60));
	ltfBox2->setFontFillColor(ccColor3B::BLACK);
	ltfBox2->setPosition(cbBox2->getPosition() + Vec2(30,10));
	layer_send->addChild(ltfBox2);

	auto cbBox3 = ui::Button::create("recharge_24_1.png", "recharge_24_2.png", "recharge_24_2.png");
	cbBox3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	cbBox3->setPosition(ltfType->getPosition() + Vec2(260,-10));
	cbBox3->setTag(9602);
	cbBox3->setBright(true);
	layer_send->addChild(cbBox3);

	auto ltfBox3 = LabelTTF::create();
	ltfBox3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	ltfBox3->setFontName("");
	ltfBox3->setFontSize(font_size - 5);
	ltfBox3->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1528"));
	ltfBox3->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfBox3->setVerticalAlignment(TextVAlignment::CENTER);
	ltfBox3->setDimensions(cocos2d::Size(60, 60));
	ltfBox3->setFontFillColor(ccColor3B::BLACK);
	ltfBox3->setPosition(cbBox3->getPosition() + Vec2(30,10));
	layer_send->addChild(ltfBox3);

	//ltfName
	auto dtName = EditBox::create(cocos2d::Size(SCREEN_WIDTH - 200,60),"regist_03.png");
	dtName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	dtName->setPosition(ltfName->getPosition() + Vec2(10, 0));
	dtName->setTag(9603);
	dtName->setFontName("");dtName->setFontSize(fontSize);
	dtName->setInputMode(EditBox::InputMode::SINGLE_LINE);
	dtName->setFontColor(ccColor3B::BLACK);
	dtName->setFontSize(font_size);	
	layer_send->addChild(dtName);
	dtName->setEnabled(false);

	//主题
	auto dtTitle = EditBox::create(cocos2d::Size(SCREEN_WIDTH - 200,60), "regist_03.png");
	dtTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	dtTitle->setPosition(ltfTitle->getPosition() + Vec2(10, 0));
	dtTitle->setTag(9604);
	dtTitle->setFontName("");dtTitle->setFontSize(fontSize);
	dtTitle->setFontColor(ccColor3B::BLACK);
	dtTitle->setInputFlag(EditBox::InputFlag::SENSITIVE);
	dtTitle->setInputMode(EditBox::InputMode::SINGLE_LINE);
	dtTitle->setReturnType(EditBox::KeyboardReturnType::DONE);
	layer_send->addChild(dtTitle);

	//内容
	auto dtContent = EditBox::create(cocos2d::Size(SCREEN_WIDTH - 200,400), "beting03.png");
	dtContent->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	dtContent->setPosition(ltfMailContent->getPosition() + Vec2(10, 0));
	dtContent->setTag(9605);
	dtContent->setFontName("");dtContent->setFontSize(fontSize);
	dtContent->setFontColor(ccColor3B::BLACK);
	dtContent->setInputFlag(EditBox::InputFlag::SENSITIVE);
	dtContent->setInputMode(EditBox::InputMode::SINGLE_LINE);
	dtContent->setReturnType(EditBox::KeyboardReturnType::DONE);
	layer_send->addChild(dtContent);

	auto btSend = ui::Button::create("my_lottery_31_2.png","my_lottery_31.png","my_lottery_31.png");
	btSend->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btSend->setPosition(Vec2(SCREEN_WIDTH / 2 - btSend->getContentSize().width + 30,dtContent->getPositionY() - 400 - 50));
	layer_send->addChild(btSend);

	auto btcacel = ui::Button::create("my_lottery_32.png","my_lottery_32_2.png","my_lottery_32_2.png");
	btcacel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btcacel->setPosition(Vec2(SCREEN_WIDTH / 2 + 60,btSend->getPositionY()));
	layer_send->addChild(btcacel);

	//事件处理
	btSend->addClickEventListener([=](Ref *){	
		auto box = (EditBox *)this->getChildByTag(9580)->getChildByTag(9603);
		if(box == nullptr) return;
		const char* strName = box->getText();
		box = (EditBox *)this->getChildByTag(9580)->getChildByTag(9604);
		if(box == nullptr) return;
		const char* strTitle = box->getText();
		box = (EditBox *)this->getChildByTag(9580)->getChildByTag(9605);
		if(box == nullptr) return;		
		const char* strContent = box->getText();

		int len = strlen(strName);
		if(len == 0)
		{
			//收件人
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1584"),
				Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
			this->addChild(layer, 255);
			return;
		}

		len = strlen(strTitle);
		if(len == 0)
		{
			//主题
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1585"),
				Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
			this->addChild(layer, 255);
			return;
		} else
		if(len > 100)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1589"),
				Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
			this->addChild(layer, 255);
			return;
		}
			
		len = strlen(strContent);
		if(len == 0)
		{
			//内容
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1586"),
				Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
			this->addChild(layer, 255);
			return;
		} else
			if(len > 500)
			{
				MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1588"),
					Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
				this->addChild(layer, 255);
				return;
			}



		m_i_sendtimes = vec_NameId.size();

		//过滤html
		std::string str_title = BetLimit::GetStrWithoutHtml(BetLimit::GetStrWithoutSpace(strTitle).c_str());
		std::string str_content = BetLimit::GetStrWithoutHtml(BetLimit::GetStrWithoutSpace(strContent).c_str());

		for(int i = 0;i < vec_NameId.size();i ++)
		{
			//选中的人的index
			//Integer* selId = vec_NameId.at(i);
			Dictionary* dic = vec_NameId.at(i);

			Integer* dic_id = (Integer *)dic->objectForKey("n_t_id");
			Integer* dic_type = (Integer *)dic->objectForKey("n_t_type");
			String* dic_name = (String *)dic->objectForKey("s_t_name");
			
			EntityMgr::instance()->getDispatch()->SendPacketWithSendMessage(dic_id->getValue(),9527,m_iCurRevType,str_content.c_str(),str_title.c_str());			
		}

		auto tempsbox = (EditBox *)this->getChildByTag(9580)->getChildByTag(9603);
		if(tempsbox == nullptr) return;
		tempsbox->setText("");
	});

	btcacel->addClickEventListener([=](Ref *){
		dtName->setText("");
		dtTitle->setText("");
		dtContent->setText("");

	});

	cbBox1->addClickEventListener([=](Ref *){
		//上级
		cbBox1->setBright(false);
		cbBox2->setBright(true);
		cbBox3->setBright(true);

		m_iCurRevType = 1;

		//给值
		vec_NameId.clear();
		
		Dictionary* dic_add = Dictionary::create();
		dic_add->setObject(Integer::create(m_top_id),"n_t_id");
		dic_add->setObject(Integer::create(2),"n_t_type");
		dic_add->setObject(String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1590")),"s_t_name");

		vec_NameId.pushBack(dic_add);
				

		showSendMember();
	});

	cbBox2->addClickEventListener([=](Ref *){
		cbBox1->setBright(true);
		cbBox2->setBright(false);
		cbBox3->setBright(true);

		//不是下级
		if(m_iCurRevType != 2)
		{
			//清掉
			vec_NameId.clear();
		}

		m_iCurRevType = 2;		

		//弹出窗
		showSubMember();	
		showSendMember();
	});

	cbBox3->addClickEventListener([=](Ref *){
		cbBox1->setBright(true);
		cbBox2->setBright(true);
		cbBox3->setBright(false);

		m_iCurRevType = 3;

		//给值
		vec_NameId.clear();		

		Dictionary* dic_add = Dictionary::create();
		dic_add->setObject(Integer::create(1),"n_t_id");
		dic_add->setObject(Integer::create(-1),"n_t_type");
		dic_add->setObject(String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1591")),"s_t_name");

		vec_NameId.pushBack(dic_add);			
		
		showSendMember();
	});	
	
	//需要的消息	
	queryMessageBox();
	queryMember();
}

void LotteryMailBox::onEnter()
{
	Layer::onEnter();
}

void LotteryMailBox::onExit()
{
	Layer::onExit();
}

void LotteryMailBox::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryMailBox::back(Object *obj)
{
	playButtonSound();

	auto layer_content = (LayerColor *)this->getChildByTag(9527);
	if(layer_content)
	{
		if(layer_content->isVisible())
		{
			layer_content->setVisible(false);
			userInfoTable->setTouchEnabled(true);
			return;
		}
	}
	//返回主界面
	Director::getInstance()->popScene();
	Director::getInstance()->popScene();
}

void LotteryMailBox::toNotice(Object *obj)
{
	MenuItemSprite* tempItem = (MenuItemSprite *)obj;
	if(obj == nullptr) return;

	if(!tempItem->isSelected())
	{
		Director::getInstance()->popScene();
	}
}

void LotteryMailBox::toMail(Object *obj)
{
	MenuItemSprite* tempItem = (MenuItemSprite *)obj;
	if(obj == nullptr) return;	

	tempItem->selected();
}

void LotteryMailBox::toInbox(Object *obj)
{
	auto layer_content = (LayerColor *)this->getChildByTag(9527);
	if(layer_content)
	{		
		layer_content->setVisible(false);
		userInfoTable->setTouchEnabled(true);		
	}
	//按钮状态
	auto temp1 = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(200);
	auto temp2 = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(300);
	auto objItem = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(100);

	if(temp1 == nullptr || temp2 == nullptr || objItem == nullptr) return;

	temp1->unselected();
	temp2->unselected();
	objItem->selected();

	//隐藏发送
	auto layer_send = (LayerColor *)this->getChildByTag(9580);
	if(layer_send)
	{		
		layer_send->setVisible(false);
		userInfoTable->setTouchEnabled(true);			
	}

	m_iBoxType = 1;
	m_iPage = 1;
	queryMessageBox();
}

void LotteryMailBox::toOutBox(Object *obj)
{
	//显示列表
	auto layer_content = (LayerColor *)this->getChildByTag(9527);
	if(layer_content)
	{		
		layer_content->setVisible(false);
		userInfoTable->setTouchEnabled(true);			
	}
	//按钮状态
	auto temp1 = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(100);
	auto temp2 = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(300);
	auto objItem = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(200);

	if(temp1 == nullptr || temp2 == nullptr || objItem == nullptr) return;

	temp1->unselected();
	temp2->unselected();
	objItem->selected();
	
	//隐藏发送
	auto layer_send = (LayerColor *)this->getChildByTag(9580);
	if(layer_send)
	{		
		layer_send->setVisible(false);
		userInfoTable->setTouchEnabled(true);			
	}

	m_iBoxType = 2;
	m_iPage = 1;
	queryMessageBox();
}

void LotteryMailBox::toSendBox(Object *obj)
{
	
	auto temp1 = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(200);
	auto temp2 = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(100);
	auto objItem = (MenuItemSprite *)this->getChildByTag(111)->getChildByTag(300);

	if(temp1 == nullptr || temp2 == nullptr || objItem == nullptr) return;
	temp1->unselected();
	temp2->unselected();
	objItem->selected();
	

	auto layer_send = (LayerColor *)this->getChildByTag(9580);
	if(layer_send)
	{		
		layer_send->setVisible(true);
		userInfoTable->setTouchEnabled(false);			
	}
	
}

void LotteryMailBox::toPre(Object *obj)
{
	m_iPage --;
	if(m_iPage < 1)
	{
		//前一页
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml","t722"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		this->addChild(layer, 255);
		m_iPage = 1;
		return;
	}

	queryMessageBox();
}

void LotteryMailBox::toNext(Object *obj)
{
	m_iPage ++;	
	if(m_iPage > m_iPageCount / m_iPageNum + 1)
	{
		//发送失败
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml","t721"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		this->addChild(layer, 255);
		m_iPage = m_iPageCount / m_iPageNum + 1;
		return;
	}
	queryMessageBox();
}

void LotteryMailBox::toDel(Object *obj)
{
	for(int i = 0;i < m_arrContent->count();i ++)
	{
		Dictionary* dic = (Dictionary *)m_arrContent->objectAtIndex(i);
		Integer* I_CurRow = (Integer *)dic->objectForKey("n_t_id");
		Integer* I_S_ID = (Integer *)dic->objectForKey("n_send_userid");

		if(I_CurRow->getValue() == m_iCurRow && I_S_ID->getValue() == 1)
		{		
			toInbox(nullptr);
			return;
		}
	}
	//这个和获取站内信的时候的boxtype不一样
	EntityMgr::instance()->getDispatch()->SendPacketWithDeleteMessageAt(3 - m_iBoxType,m_iCurRow);
}

void LotteryMailBox::toReply(Object *obj)
{
	toSendBox(nullptr);
	//添加到收件人
	vec_NameId.clear();

	for(int i = 0;i < m_arrContent->count();i ++)
	{
		Dictionary* dic = (Dictionary *)m_arrContent->objectAtIndex(i);
		Integer* I_CurRow = (Integer *)dic->objectForKey("n_t_id");
		Integer* I_S_ID = (Integer *)dic->objectForKey("n_send_userid");
		Integer* I_type = (Integer *)dic->objectForKey("n_t_ifSj");
		String* S_name = (String *)dic->objectForKey("s_t_username");
		
		if(I_CurRow->getValue() == m_iCurRow)
		{
			//客服退出
			if(I_S_ID->getValue() == 1) return;
			
			Dictionary* dic_add = Dictionary::create();
			dic_add->setObject(I_S_ID->clone(),"n_t_id");

			if(I_type->getValue() == 1)
			{
				dic_add->setObject(Integer::create(2),"n_t_type");
				dic_add->setObject(String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1590")),"s_t_name");
			}else
			{
				dic_add->setObject(Integer::create(0),"n_t_type");
				dic_add->setObject(String::create(S_name->getCString()),"s_t_name");
			}			

			vec_NameId.pushBack(dic_add);	
			
			break;
		}
	}	

	//显示
	showSendMember();	
	showSendTopic();
}

cocos2d::Size LotteryMailBox::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 100; //高度调整为100
	return size;
}

ssize_t LotteryMailBox::numberOfCellsInTableView(TableView *table)
{
	return row_count;
}
TableViewCell *LotteryMailBox::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 

	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(150,150,150);
	ccColor3B blackColor1 = ccc3(0,0,0);
	float fontSize = 27;
	float gap = 34;
	float textWidth = SCREEN_WIDTH - 200;

	Dictionary* dic = (Dictionary *)m_arrContent->objectAtIndex(index);
	if(dic == nullptr)
	{
		return cell;
	}

	Integer* I_Read = (Integer *)dic->objectForKey("c_t_ifread");

	String* tempStr = NULL;

	if(m_iBoxType == 2)
	{
		tempStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1587"));
	}else
	{
		if(I_Read->getValue() == 1)
		{
			tempStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1582"));
		}else
		{
			tempStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1583"));
		}
	}
	
	LabelTTF *ltfTitleLeft = LabelTTF::create(tempStr->getCString(),"",fontSize,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	ltfTitleLeft->setColor(blackColor1);
	ltfTitleLeft->setAnchorPoint(Vec2(0,0.5f));
	ltfTitleLeft->setVerticalAlignment(TextVAlignment::TOP);
	ltfTitleLeft->setPosition(Vec2(gap, 75));
	cell->addChild(ltfTitleLeft, 8);

	tempStr = (String *)dic->objectForKey("s_t_title");
	LabelTTF *ltfTitle = LabelTTF::create(tempStr->getCString(),"",fontSize,cocos2d::Size(textWidth,28),kCCTextAlignmentLeft);
	ltfTitle->setColor(blackColor1);
	ltfTitle->setAnchorPoint(Vec2(0,0.5f));
	ltfTitle->setVerticalAlignment(TextVAlignment::TOP);
	ltfTitle->setPosition(Vec2(gap + 200, 75));
	cell->addChild(ltfTitle, 8);

	tempStr = (String *)dic->objectForKey("n_t_time");
	LabelTTF *timeLabel = LabelTTF::create(tempStr->getCString(),"",fontSize,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	timeLabel->setColor(blackColor1);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setVerticalAlignment(TextVAlignment::TOP);
	timeLabel->setPosition(Vec2(gap, 25));
	cell->addChild(timeLabel, 8);

	//右箭头
	Sprite *more = Sprite::createWithSpriteFrame(spriteFrame("setting_01.png"));
	more->setAnchorPoint(Vec2(1,0.5f));
	more->setPosition(Vec2(SCREEN_WIDTH, 100 / 2));
	cell->addChild(more, 8);

	//分隔线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5f));
	gap1->setPosition(Vec2(0, 0));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);	
	cell->addChild(gap1,9);

	cell->setTag(index + 1);

	return cell;  
}

void LotteryMailBox::tableCellTouched(TableView *table, TableViewCell *cell)
{
	int cellTag = cell->getTag() - 1;

	displayIndex(cellTag);
}

void LotteryMailBox::scrollViewDidScroll(extension::ScrollView* view)
{

}

void LotteryMailBox::scrollViewDidZoom(extension::ScrollView* view)
{

}

const char* LotteryMailBox::getUtfString(const char* tempStr)
{
	std::string result = tempStr;

	for(int i = 0;i < result.size();i ++)
	{
		char tempChar = result.at(i);
		if(tempChar == '<')
		{
			result.replace(i,1," ");
		}
	}

	return result.c_str();
}

void LotteryMailBox::queryMessageBox()
{
	m_arrContent->removeAllObjects();
	row_count = 0;
	userInfoTable->reloadData();
	EntityMgr::instance()->getDispatch()->SendPacketWithGetInboxMessage(m_iPage,m_iPageNum,m_iBoxType);//1收件箱，2发件箱
}

void LotteryMailBox::showSubMember()
{
	auto layer = LayerColor::create(ccc4(0,0,0,100));//ccc3+透明度
	layer->setTag(6699);
	this->addChild(layer,100);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){ return true;};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layer);

	//禁掉返回键
	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){	

		if(keycode == EventKeyboard::KeyCode::KEY_BACK)
		{
			if(layer) layer->removeFromParent();
			event->stopPropagation();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, layer);

	auto sptBackground = Sprite::createWithSpriteFrame(spriteFrame("recharge_25.png"));
	sptBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptBackground->setTag(6688);
	sptBackground->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	layer->addChild(sptBackground);

	LabelTTF* ltfTitle = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1530"), "", 37);
	ltfTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfTitle->setPosition(Vec2(sptBackground->getContentSize().width / 2,sptBackground->getContentSize().height - 42));
	ltfTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfTitle->setFontFillColor(ccc3(255,255,255));
	sptBackground->addChild(ltfTitle);

	Sprite* sptEditBack = Sprite::createWithSpriteFrame(spriteFrame("recharge_25_2.png"));
	sptEditBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptEditBack->setPosition(Vec2(sptBackground->getContentSize().width / 3,ltfTitle->getPositionY() - 100));
	sptEditBack->setScaleX(260 / sptEditBack->getContentSize().width);
	sptBackground->addChild(sptEditBack);

	EditBox* dt_name_filter = EditBox::create(cocos2d::Size(250,50),Scale9Sprite::create("recharge_25_2.png"));
	dt_name_filter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	dt_name_filter->setPosition(Vec2(sptBackground->getContentSize().width / 3,ltfTitle->getPositionY() - 100));
	dt_name_filter->setInputFlag(EditBox::InputFlag::SENSITIVE);
	dt_name_filter->setReturnType(EditBox::KeyboardReturnType::DONE);
	dt_name_filter->setInputMode(EditBox::InputMode::SINGLE_LINE);
	dt_name_filter->setFontName("");dt_name_filter->setFontSize(27);
	dt_name_filter->setPlaceHolder(ConfigMgr::instance()->text("display_DuangDong.xml","t1600"));
	dt_name_filter->setFontColor(Color4B::BLACK);
	dt_name_filter->setPlaceholderFont("",27);
	sptBackground->addChild(dt_name_filter);

	ui::Button* btn_search = ui::Button::create("recharge_24_3.png","recharge_24_4.png","recharge_24_3.png",ui::Widget::TextureResType::LOCAL);
	btn_search->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btn_search->setPosition(Vec2(dt_name_filter->getPositionX() + 130,dt_name_filter->getPositionY()));
	sptBackground->addChild(btn_search);
	
	btn_search->addClickEventListener([=](Ref *){
		
		queryMember(dt_name_filter->getText());
	});

	auto lyScl = Layer::create(); 
	lyScl->setTag(6666);

	//添加
	refreshList(lyScl);

	//装按钮的框
	auto sclBox = extension::ScrollView::create();  //scrollview pageview listview都只是一个布局，不能直接显示控件
	//位置是由它决定的
	sclBox->setPosition(Vec2(50,120));
	sclBox->setDirection(extension::ScrollView::Direction::VERTICAL);
	//设置
	sclBox->setContainer(lyScl);
	sclBox->setContentSize(cocos2d::Size(sptBackground->getContentSize().width - 100,m_arrUserInfo->count() * 26));
	sclBox->setViewSize(cocos2d::Size(sptBackground->getContentSize().width - 100,sptBackground->getContentSize().height - 320));
	sclBox->setBounceable(true);
	sclBox->setTag(6677);
	sptBackground->addChild(sclBox);
	sclBox->setContentOffset(Vec2(5,5));

	//没有scrollto类的方法，直接设置相对位置
	lyScl->setPositionY(380 - m_arrUserInfo->count() * 26);	

	auto btnConfirm = ui::Button::create("beting22.png","beting23.png","beting22.png");
	btnConfirm->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnConfirm->setPosition(Vec2(sptBackground->getContentSize().width - 130,60));

	sptBackground->addChild(btnConfirm);

	// 全选
	auto btnAll = ui::Button::create("recharge_24_1.png", "recharge_24_2_1.png", "recharge_24_2_1.png");
	btnAll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnAll->setPosition(Vec2(80,60));
	sptBackground->addChild(btnAll);
	btnAll->setBright(true);

	//全选文字
	auto ltfAll = LabelTTF::create();
	ltfAll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfAll->setFontName("");
	ltfAll->setFontFillColor(Color3B::RED);
	ltfAll->setFontSize(27);
	ltfAll->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t1592"));
	ltfAll->setPosition(btnAll->getPosition() + Vec2(10,0));
	sptBackground->addChild(ltfAll);

	//上一页
	auto btnPre = ui::Button::create("recharge_24_5.png", "recharge_24_6.png", "recharge_24_5.png");
	btnPre->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnPre->setPosition(Vec2(160,60));
	sptBackground->addChild(btnPre);

	//下一页
	auto btnNext = ui::Button::create("recharge_24_7.png", "recharge_24_8.png", "recharge_24_7.png");
	btnNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnNext->setPosition(Vec2(320,60));
	sptBackground->addChild(btnNext);

	//显示页数
	String* tempStr = String::createWithFormat("%s %d/%d",ConfigMgr::instance()->text("display_DuangDong.xml","t1601"),m_user_page_cur,m_user_page_count);
	

	auto ltfPage = LabelTTF::create();
	ltfPage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfPage->setFontName("");
	ltfPage->setFontFillColor(Color3B::BLACK);
	ltfPage->setFontSize(24);
	ltfPage->setTag(2233);
	ltfPage->setString(tempStr->getCString());
	ltfPage->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfPage->setVerticalAlignment(TextVAlignment::CENTER);
	ltfPage->setPosition((btnPre->getPosition() + btnNext->getPosition()) / 2);
	ltfPage->setDimensions(cocos2d::Size(120,30));
	sptBackground->addChild(ltfPage);

	//事件处理
	btnConfirm->addClickEventListener([=](Ref *){

		//不清空列表，直接显示到收件人
		showSendMember();
		//释放
		if(layer) layer->removeFromParent();
	});

	//全选
	btnAll->addClickEventListener([=](Ref *)
	{
		//全选只全选当前页，并且已选择的不处理
		//为了实现查询，在这里必须刷新界面
		auto layer = (LayerColor *)this->getChildByTag(6699);
		if(layer == nullptr) return;
		auto background = (Sprite *)layer->getChildByTag(6688);
		if(background == nullptr) return;
		auto lyBox = (Layer *)background->getChildByTag(6677)->getChildByTag(6666);
		if(lyBox == nullptr) return;
		//这里为止已经找到添加按钮的地方

		bool isselect = btnAll->isBright();  //true为未选中

		int i_count = m_arrUserInfo->count();

		for(int i  = 0;i < i_count;i ++)
		{
			auto btnBox = (ui::Button* )lyBox->getChildByTag(i + 1000);
			if(btnBox == nullptr) continue;

			if(btnBox->isBright() == isselect)   //true为未选中
			{
				selectButton(btnBox);
			}			
		}	

		btnAll->setBright(!isselect);
	});

	//上一页
	btnPre->addClickEventListener([=](Ref *)
	{
		m_user_page_cur --;
		if(m_user_page_cur < 1)
		{
			//前一页
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml","t722"),
				Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
			this->addChild(layer, 255);
			m_user_page_cur = 1;
			return;
		}

		//显示页数
		String* tempStr = String::createWithFormat("%s %d/%d",ConfigMgr::instance()->text("display_DuangDong.xml","t1601"),m_user_page_cur,m_user_page_count);
		ltfPage->setString(tempStr->getCString());

		queryMember();
	});

	//下一页
	btnNext->addClickEventListener([=](Ref *)
	{
		m_user_page_cur ++;
		if(m_user_page_cur > m_user_page_count)
		{
			//前一页
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml","t721"),
				Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
			this->addChild(layer, 255);
			m_user_page_cur = m_user_page_count;
			return;
		}	
		//显示页数
		String* tempStr = String::createWithFormat("%s %d/%d",ConfigMgr::instance()->text("display_DuangDong.xml","t1601"),m_user_page_cur,m_user_page_count);
		ltfPage->setString(tempStr->getCString());

		queryMember();
	});
}

void LotteryMailBox::showSendMember()
{
	EditBox* tempBox = (EditBox *)this->getChildByTag(9580)->getChildByTag(9603);
	ui::Button* cbBox1 = (ui::Button *)this->getChildByTag(9580)->getChildByTag(9600);
	ui::Button* cbBox2 = (ui::Button *)this->getChildByTag(9580)->getChildByTag(9601);
	ui::Button* cbBox3 = (ui::Button *)this->getChildByTag(9580)->getChildByTag(9602);
	if(tempBox == nullptr || cbBox1 == nullptr || cbBox2 == nullptr || cbBox3 == nullptr) return;
	tempBox->setText("");

	String* strResult = String::create("");

	for(int i = 0;i < vec_NameId.size();i ++)
	{		
		Dictionary* dic = vec_NameId.at(i);

		Integer* dic_id = (Integer *)dic->objectForKey("n_t_id");
		Integer* dic_type = (Integer *)dic->objectForKey("n_t_type");
		String* dic_name = (String *)dic->objectForKey("s_t_name");		

		if(dic_type->getValue() == 2)//2为上级
		{
			m_iCurRevType = 1;cbBox1->setBright(false);cbBox2->setBright(true);cbBox3->setBright(true);
			strResult->append(String::createWithFormat("%s;",dic_name->getCString())->getCString());
		} else
		if(dic_type->getValue() == -1)
		{
			m_iCurRevType = 1;cbBox1->setBright(true);cbBox2->setBright(true);cbBox3->setBright(false);
			strResult->append(String::createWithFormat("%s;",dic_name->getCString())->getCString());
		} else
		{
			m_iCurRevType = 2;cbBox1->setBright(true);cbBox2->setBright(false);cbBox3->setBright(true);
			strResult->append(String::createWithFormat("%s(%d);",dic_name->getCString(),dic_id->getValue())->getCString());				
		}
		
	}
	
	tempBox->setText(strResult->getCString());	
}

void LotteryMailBox::showSendTopic()
{
	EditBox* tempBox = (EditBox *)this->getChildByTag(9580)->getChildByTag(9604);
	if(tempBox == nullptr) return;
	tempBox->setText("");

	std::string strResult = "";

	for(int i = 0;i < m_arrContent->count();i ++)
	{
		Dictionary* dic = (Dictionary *)m_arrContent->getObjectAtIndex(i);
		Integer* iIndex = (Integer *)dic->objectForKey("n_t_id");

		if(iIndex->getValue() == m_iCurRow)
		{
			String* strTopic = (String *)dic->objectForKey("s_t_title");
			strResult.append(strTopic->getCString());
			break;
		}
	}

	//检测头
	bool hasHead = strResult.substr(0, 3).compare("Re:") == 0;

	if(!hasHead)
	{
		strResult.insert(0,"Re:");
	}

	tempBox->setText(strResult.c_str());
}

void LotteryMailBox::getInboxMessageRet(Object *obj)
{
	//清理
	//m_arrContent->removeAllObjects();
	//row_count = 0;
	//userInfoTable->reloadData();

	//整个添加
	Array* myArray = (Array *)obj;
	if(myArray == nullptr) return;
	myArray->retain();
	m_arrContent->addObjectsFromArray(myArray);
	myArray->release();
	row_count = m_arrContent->count();	

	userInfoTable->reloadData();

	//总共多少条
	if(row_count == 0) return;
	Dictionary *dic = (Dictionary *)m_arrContent->getObjectAtIndex(0);
	Integer* pagecount = (Integer *)dic->objectForKey("n_t_count");
	m_iPageCount = pagecount->getValue();
}

void LotteryMailBox::chkInboxMessageRet(Object *obj)
{

}

void LotteryMailBox::getXJUserInfoRet(Object *obj)
{
	m_arrUserInfo->removeAllObjects();
	//用新的array
	m_arrUserInfo->addObjectsFromArray((Array *)obj);	

	//得到上级id
	if(m_top_id == 0)
	{
		for(int i = 0;i < m_arrUserInfo->count();i ++)
		{
			Dictionary* dic = (Dictionary*)m_arrUserInfo->objectAtIndex(i);
			Integer* dic_id = (Integer *)dic->objectForKey("n_t_userid");
			Integer* dic_type = (Integer *)dic->objectForKey("n_t_type");
			//先算count
			if(i == 0)
			{
				Integer* dic_count = (Integer *)dic->objectForKey("n_t_count");
				m_user_page_count = dic_count->getValue() / 30;
				if(dic_count->getValue() % 30 > 0)
				{
					m_user_page_count ++;
				}
			}
			//获取上级
			if(dic_type->getValue() == 2)
			{
				m_top_id = dic_id->getValue();
				break;
			}

			
		}
	}
	
	//为了实现查询，在这里必须刷新界面
	auto layer = (LayerColor *)this->getChildByTag(6699);
	if(layer == nullptr) return;
	auto background = (Sprite *)layer->getChildByTag(6688);
	if(background == nullptr) return;
	auto lyBox = (Layer *)background->getChildByTag(6677)->getChildByTag(6666);
	if(lyBox == nullptr) return;
	//这里为止已经找到添加按钮的地方
	lyBox->removeAllChildren();

	refreshList(lyBox);
}

void LotteryMailBox::sendMessageRet(Object *obj)
{
	m_i_sendtimes --;
	if(m_i_sendtimes > 0) return;

	//0失败其它都是成功
	Dictionary *dic = (Dictionary *)obj;
	Integer* n_t_retid = (Integer* )dic->objectForKey("n_t_retid");
	Integer *n_t_sign = (Integer *)dic->objectForKey("n_t_sign");

	if(n_t_retid->getValue() == 0)
	{
		//发送失败
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1532"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		this->addChild(layer, 255);
	} else
	{
		//发送成功
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1531"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		this->addChild(layer, 255);

		//清空数据
		//9580 9603
		vec_NameId.clear();
		for(int i = 0;i < 3;i ++)
		{
			auto box = (EditBox *)this->getChildByTag(9580)->getChildByTag(9603 + i);
			if(box == nullptr) continue;
			box->setText("");
		}
	}
}

void LotteryMailBox::getMessageCountRet(Object *obj)
{
	//dic->setObject(Integer::create(info->nResult), "nResult");
}

void LotteryMailBox::deleteMessageRet(Object *obj)
{
	Dictionary* dic = (Dictionary *)obj;
	if(dic == nullptr) return;
	Integer* result = (Integer *)dic->objectForKey("n_t_retid");

	if(result->getValue() == 1)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1580"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		Director::getInstance()->getRunningScene()->addChild(layer, 255);

		//刷新界面
		if(m_iBoxType == 1)
		{
			toInbox(nullptr);
		}else
		if(m_iBoxType == 2)
		{
			toOutBox(nullptr);
		}
	}else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1581"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		Director::getInstance()->getRunningScene()->addChild(layer, 255);
	}
}

void LotteryMailBox::displayIndex(const int index)
{
	userInfoTable->setTouchEnabled(false);

	if(index >= m_arrContent->count()) return;

	auto layer = (LayerColor *)this->getChildByTag(9527);
	if(layer == nullptr) return;
	//标题和内容
	//auto layer_title = (LabelTTF *)layer->getChildByTag(9528);
	//if(layer_title == nullptr) return;

	auto layer_content = (LabelTTF *)layer->getChildByTag(8878)->getChildByTag(9529);
	if(layer_content == nullptr) return;

	auto btReply = (MenuItemSprite *)layer->getChildByTag(527)->getChildByTag(528)->getChildByTag(526);
	if(btReply == nullptr) return;

	if(m_iBoxType == 2)
	{
		btReply->setVisible(false);
	}else
	{
		btReply->setVisible(true);
	}

	Dictionary* dic = (Dictionary *)m_arrContent->objectAtIndex(index);
	if(dic == nullptr) return;
	
	String* strTitle = (String *)dic->objectForKey("s_t_title");
	//layer_title->setString(strTitle->getCString());

	String* strName = (String *)dic->objectForKey("s_t_username");
	String* strContent = (String *)dic->objectForKey("s_t_content");
	String* strTime  = (String *)dic->objectForKey("n_t_time");
	Integer* iIndex = (Integer *)dic->objectForKey("n_t_id");
	
	//判断是不是上级
	Integer* I_type = (Integer *)dic->objectForKey("n_t_ifSj");
	if(I_type->getValue() == 1)
	{
		if(strName)
		{
			strName->release();
			strName= NULL;
		}
		strName = String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1590"));
	}else
	if(I_type->getValue() == 2)
	{
		if(strName)
		{
			strName->release();
			strName = NULL;
		}

		strName = String::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1591"));
	}

	//保存当前显示条数
	m_iCurRow = iIndex->getValue();
	//刷新状态
	dic->removeObjectForKey("c_t_ifread");
	dic->setObject(Integer::create(1),"c_t_ifread");
	userInfoTable->reloadData();
	//发送已查看
	EntityMgr::instance()->getDispatch()->SendPacketWithChkInboxMessage(m_iCurRow);

	//发件人还是收件人
	String* strNamePre = NULL;
	if(m_iBoxType == 2) //发件人
	{
		strNamePre = String::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1522"));
	}else
	{
		strNamePre = String::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1520"));
	}

	String* content = String::createWithFormat("%s%s\n\n%s%s\n%s%s\n\n%s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1523"),
		strTitle->getCString(),
		strNamePre->getCString(),
		strName->getCString(),
		ConfigMgr::instance()->text("display_DuangDong.xml", "t1521"),
		strTime->getCString(),
		strContent->getCString()
		);

	layer_content->setString(BetLimit::GetStrWithoutHtml(content->getCString()));

	//位置归0
	layer_content->setPositionY(SCREEN_HEIGHT - 310 - layer_content->getContentSize().height);

	layer->setVisible(true);
}

void LotteryMailBox::queryMember(const char* name /* = "" */)
{
	EntityMgr::instance()->getDispatch()->SendPacketWithGetXJUserInfo(name, 0, m_user_page_cur);
}

void LotteryMailBox::refreshList(Layer *lyScl)
{
	int i_indeed = 0;
	int i_count = m_arrUserInfo->count();
	for(int i = 0;i < i_count;i ++)
	{
		Dictionary *dic = (Dictionary *)m_arrUserInfo->getObjectAtIndex(i);
		Integer* iUserType = (Integer *)dic->objectForKey("n_t_type");
		String* strUserName = (String *)dic->objectForKey("s_t_name");
		Integer* iUserId = (Integer *)dic->objectForKey("n_t_userid");

		//为2的时候是上级 其它的都是下级
		if(iUserType->getValue() == 2) 
		{			
			continue;
		}

		auto btSel = ui::Button::create("recharge_24_1.png", "recharge_24_2_1.png", "recharge_24_2_1.png");
		btSel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		btSel->setPosition(Vec2(280 * (i_indeed % 2),((i_count - i_indeed) + (i_indeed % 2)) * 26 - 20));//此处20是离顶的位置，因为是居中对齐
		btSel->setBright(true);
		btSel->setTag(i + 1000);
		lyScl->addChild(btSel);

		for(int i_check = 0;i_check < vec_NameId.size();i_check ++)
		{
			Dictionary* check_dic = vec_NameId.at(i_check);
			Integer* I_user_id = (Integer *)check_dic->objectForKey("n_t_id");
			if(I_user_id->getValue() == iUserId->getValue())
			{
				btSel->setBright(false);				
			}
		}

		auto ltfName = LabelTTF::create(strUserName->getCString(),"", 26);
		ltfName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		ltfName->setPosition(btSel->getPosition() + Vec2(35,0));
		ltfName->setFontFillColor(ccColor3B::BLACK);
		ltfName->setDimensions(cocos2d::Size(200,30));
		ltfName->setHorizontalAlignment(TextHAlignment::LEFT);
		lyScl->addChild(ltfName);

		//发送
		btSel->addClickEventListener(CC_CALLBACK_1(LotteryMailBox::selectButton,this));


		i_indeed ++;
	}	

	//没有scrollto类的方法，直接设置相对位置
	lyScl->setPositionY(380 - m_arrUserInfo->count() * 26);	
}

void LotteryMailBox::selectButton(Object *obj)
{
	//按钮状态
	ui::Button* btn = (ui::Button *)obj;
	//在点击按钮的时候就选中
	bool isSelect = !btn->isBright();
	int btn_id = btn->getTag() - 1000;

	Dictionary* dic_arr = (Dictionary *)m_arrUserInfo->objectAtIndex(btn_id);
	Integer* temp_id = (Integer *)dic_arr->objectForKey("n_t_userid");
	Integer* temp_type = (Integer *)dic_arr->objectForKey("n_t_type");
	String* temp_name = (String *)dic_arr->objectForKey("s_t_name");

	//处理  
	if(isSelect)
	{		
		for(int i  = 0;i < vec_NameId.size();i ++)
		{
			Dictionary* dic_vec = (Dictionary *)vec_NameId.at(i);
			Integer* vec_id = (Integer *)dic_vec->objectForKey("n_t_id");

			if(temp_id->getValue() == vec_id->getValue())
			{				
				vec_NameId.swap(i,vec_NameId.size() - 1);
				vec_NameId.popBack();
				break;
			}
		}
	}else
	{
		Dictionary* dic_add = Dictionary::create();
		dic_add->setObject(temp_id->clone(),"n_t_id");
		dic_add->setObject(temp_type->clone(),"n_t_type");
		dic_add->setObject(String::create(temp_name->getCString()),"s_t_name");

		vec_NameId.pushBack(dic_add);
	}
	//
	btn->setBright(isSelect);
}
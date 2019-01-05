#include "LotteryMessageCenter.h"
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

#include "LotteryMailBox.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

//所有代理功能
Scene* LotteryMessageCenter::scene()
{
	Scene *scene = Scene::create();
	LotteryMessageCenter *layer = LotteryMessageCenter::create();
	scene->addChild(layer);
	return scene;
}

LotteryMessageCenter::LotteryMessageCenter()
{
	//userInfoTable = nullptr;
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryMessageCenter::getNoticeRet),"GetNoticeRet",NULL);
}

LotteryMessageCenter::~LotteryMessageCenter()
{
	//userInfoTable->release();
	//if(userInfoTable)
	//{
	//	userInfoTable->release();
	//}
	NotificationCenter::getInstance()->removeObserver(this,"GetNoticeRet");
}

bool LotteryMessageCenter::init()
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

void LotteryMessageCenter::refreshLayer()
{
	//
}

void LotteryMessageCenter::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	money = g_GlobalUnits.getGolbalUserData()->lScore;
	cellHeight = 80;
	cellNum = 11;

	row_count = 0;
}

void LotteryMessageCenter::initView()
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
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryMessageCenter::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	//pCloseButton->setPosition(Vec2(32, SCREEN_HEIGHT - 120));
	pCloseButton->setAnchorPoint(Vec2(0,1));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	Sprite *pGonggaoNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_2.png"));
	Sprite *pGonggaoSelected = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_1.png"));
	Sprite *pMailNormal = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_4.png"));
	Sprite *pMailSelected = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_22_3.png"));
	MenuItemSprite *pGonggao = MenuItemSprite::create(pGonggaoNormal,pGonggaoSelected,CC_CALLBACK_1(LotteryMessageCenter::toNotice,this));
	MenuItemSprite *pMail = MenuItemSprite::create(pMailNormal,pMailSelected,CC_CALLBACK_1(LotteryMessageCenter::toMail,this));
	Menu *title = Menu::create(pGonggao,pMail,NULL);	
	title->setAnchorPoint(Vec2(0.5,0.5));
	title->alignItemsHorizontallyWithPadding(0);
	title->setPosition(Vec2(SCREEN_WIDTH / 2, pCloseButton->getPositionY()));
	this->addChild(title,1);

	//默认开启公告
	pGonggao->selected();

	fontSize = 30;
	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = title->getPositionY() - 70;
	auto userInfoTable = TableView::create(this, tableViewSize);
	userInfoTable->setDelegate(this);
	userInfoTable->setAnchorPoint(Vec2(0.5, 0.5));
	userInfoTable->setPosition(Vec2(0, 0));
	userInfoTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	userInfoTable->setDirection(extension::ScrollView::Direction::VERTICAL);
	userInfoTable->setTag(9564);
	this->addChild(userInfoTable, 1);

	LayerColor* layer_content = LayerColor::create(ccc4(255,255,255,255));
	layer_content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	layer_content->setContentSize(cocos2d::Size(SCREEN_WIDTH,title->getPositionY() - 70));
	layer_content->setTag(9527);
	layer_content->setVisible(false);
	this->addChild(layer_content,2);

	//公告标题
	auto ltf_notice_title = LabelTTF::create();
	ltf_notice_title->setTag(9528);
	ltf_notice_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	ltf_notice_title->setFontName("");
	ltf_notice_title->setFontSize(40);
	ltf_notice_title->setFontFillColor(COLOR_BK_TITLE);
	ltf_notice_title->setHorizontalAlignment(TextHAlignment::CENTER);
	ltf_notice_title->setVerticalAlignment(TextVAlignment::CENTER);
	ltf_notice_title->setDimensions(cocos2d::Size(SCREEN_WIDTH - 60, 80));
	ltf_notice_title->setPosition(Vec2(SCREEN_WIDTH / 2,title->getPositionY() - 70));
	layer_content->addChild(ltf_notice_title);

	auto ltf_notice_time = LabelTTF::create();
	ltf_notice_time->setTag(9530);
	ltf_notice_time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	ltf_notice_time->setFontName("");
	ltf_notice_time->setFontSize(25);
	ltf_notice_time->setHorizontalAlignment(TextHAlignment::CENTER);
	ltf_notice_time->setVerticalAlignment(TextVAlignment::CENTER);
	ltf_notice_time->setFontFillColor(ccColor3B::BLACK);
	ltf_notice_time->setPosition(Vec2(SCREEN_WIDTH / 2,ltf_notice_title->getPositionY() - 70));
	layer_content->addChild(ltf_notice_time);

	//分隔线
	Sprite *gap = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap->setAnchorPoint(Vec2(0,0.5f));
	gap->setPosition(Vec2(0, ltf_notice_time->getPositionY() - 40));
	gap->setScaleX(SCREEN_WIDTH / gap->getContentSize().width);	
	layer_content->addChild(gap,9);

	//公告内容
	auto ltf_notice_content = LabelTTF::create();
	ltf_notice_content->setTag(9529);
	ltf_notice_content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	ltf_notice_content->setFontName("");
	ltf_notice_content->setFontSize(30);
	ltf_notice_content->setFontFillColor(COLOR_BK_GRAY);
	ltf_notice_content->setHorizontalAlignment(TextHAlignment::LEFT);
	ltf_notice_content->setVerticalAlignment(TextVAlignment::TOP);
	ltf_notice_content->setDimensions(cocos2d::Size(SCREEN_WIDTH - 60, 0));
	//ltf_notice_content->setPosition(gap->getPosition() + Vec2(SCREEN_WIDTH / 2,-30));
	//layer_content->addChild(ltf_notice_content);

	//改变为可滚动的
	auto scl_content = extension::ScrollView::create();
	scl_content->setViewSize(cocos2d::Size(SCREEN_WIDTH - 60, gap->getPositionY() - 15));
	scl_content->setContainer(ltf_notice_content);
	scl_content->setTag(8878);
	scl_content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	scl_content->setPositionX(30);
	scl_content->setPositionY(5);
	scl_content->setContentOffset(Vec2(5,5));
	scl_content->setBounceable(true);scl_content->setDirection(extension::ScrollView::Direction::VERTICAL);
	layer_content->addChild(scl_content);

	//发送消息
	EntityMgr::instance()->getDispatch()->SendPacketWithGetNews();
}

void LotteryMessageCenter::onEnter()
{
	Layer::onEnter();

	if(vec_content.size() <= 0)
	{
		EntityMgr::instance()->getDispatch()->SendPacketWithGetNews();
	}
}

void LotteryMessageCenter::onExit()
{
	Layer::onExit();
}

void LotteryMessageCenter::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryMessageCenter::back(Object *obj)
{
	playButtonSound();

	auto layer_content = (LayerColor *)this->getChildByTag(9527);
	if(layer_content)
	{
		if(layer_content->isVisible())
		{
			layer_content->setVisible(false);

			auto userInfoTable = (TableView *)this->getChildByTag(9564);
			if(userInfoTable != nullptr)
				userInfoTable->setTouchEnabled(true);
			return;
		}
	}
	//返回
	Director::getInstance()->popScene();
}

void LotteryMessageCenter::toNotice(Object *obj)
{
	MenuItemSprite* tempItem = (MenuItemSprite *)obj;
	if(obj == nullptr) return;	

	tempItem->selected();
}

void LotteryMessageCenter::toMail(Object *obj)
{
	MenuItemSprite* tempItem = (MenuItemSprite *)obj;
	if(obj == nullptr) return;

	if(!tempItem->isSelected())
	{
		auto scene = LotteryMailBox::scene();
		Director::getInstance()->pushScene(scene);

		NotificationCenter::getInstance()->removeObserver(this,"GetNoticeRet");
	}
}

//transaction menu
void LotteryMessageCenter::recharge(Object* obj)
{
	//if(showRecordInfo == true){
	//	return;
	//}
	Scene *scene = LotteryRecharge::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	NotificationCenter::getInstance()->removeObserver(this,"GetNoticeRet");
}

void LotteryMessageCenter::withdraw(Object *obj)
{
	Scene *scene = LotteryWithdraw::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	NotificationCenter::getInstance()->removeObserver(this,"GetNoticeRet");
}

cocos2d::Size LotteryMessageCenter::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 100; //高度调整为100
	return size;
}

ssize_t LotteryMessageCenter::numberOfCellsInTableView(TableView *table)
{
	return row_count;
}
TableViewCell *LotteryMessageCenter::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 


	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(150,150,150);
	ccColor3B blackColor1 = ccc3(0,0,0);
	float fontSize = 27;
	float gap = 34;
	float textWidth = SCREEN_WIDTH - 60;

	String* tempStr = vec_content.at(index * 3);

	LabelTTF *timeLabel = LabelTTF::create(tempStr->getCString(),"",fontSize,cocos2d::Size(textWidth,50),kCCTextAlignmentLeft);
	timeLabel->setColor(blackColor1);
	timeLabel->setVerticalAlignment(TextVAlignment::TOP);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setPosition(Vec2(gap, 100/2));
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

void LotteryMessageCenter::tableCellTouched(TableView *table, TableViewCell *cell)
{
	int cellTag = cell->getTag() - 1;

	displayIndex(cellTag);
}

void LotteryMessageCenter::scrollViewDidScroll(extension::ScrollView* view)
{

}
void LotteryMessageCenter::scrollViewDidZoom(extension::ScrollView* view)
{

}

const char* LotteryMessageCenter::getUtfString(const char* tempStr)
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

void LotteryMessageCenter::getNoticeRet(Object *obj)
{
	Array* data = (Array *)obj;
	row_count = data->count();
	for(int i = 0;i < row_count;i ++)
	{	
		Dictionary* temp = (Dictionary *)data->objectAtIndex(i);

		String* content = (String *)temp->objectForKey("s_t_news");	
		String* title = (String *)temp->objectForKey("s_t_title"); 	
		String* stime = (String* )temp->objectForKey("n_t_time");

		vec_content.pushBack (title);
		vec_content.pushBack(content);	
		vec_content.pushBack(stime);
	}
	auto userInfoTable = (TableView *)this->getChildByTag(9564);
	if(userInfoTable != nullptr)
		userInfoTable->reloadData();
}

void LotteryMessageCenter::displayIndex(const int index)
{
	auto userInfoTable = (TableView *)this->getChildByTag(9564);
	if(userInfoTable != nullptr)
		userInfoTable->setTouchEnabled(false);

	if(index >= vec_content.size()) return;

	auto layer = (LayerColor *)this->getChildByTag(9527);
	if(layer == nullptr) return;
	//标题和内容
	auto layer_title = (LabelTTF *)layer->getChildByTag(9528);
	auto layer_content = (LabelTTF *)layer->getChildByTag(8878)->getChildByTag(9529);
	auto layer_time = (LabelTTF *)layer->getChildByTag(9530);

	if(layer_content == nullptr || layer_title == nullptr || layer_time == nullptr) return;
	
	layer_title->setString(vec_content.at(index * 3)->getCString());
	layer_content->setString(BetLimit::GetStrWithoutHtml(vec_content.at(index * 3 + 1)->getCString()).c_str());
	layer_time->setString(String::createWithFormat("%s%s",ConfigMgr::instance()->text("display_DuangDong.xml","t1529"),vec_content.at(index * 3 + 2)->getCString())->getCString());

	//位置归0
	layer_content->setPositionY(SCREEN_HEIGHT - 350 - layer_content->getContentSize().height);

	layer->setVisible(true);
}
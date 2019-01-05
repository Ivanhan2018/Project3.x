#include "LotteryMemberShipManagement.h"
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
#include "VersionControl.h"

using namespace CocosDenshion;

//所有代理功能
Scene* LotteryMemberShipManagement::scene()
{
	Scene *scene = Scene::create();
	LotteryMemberShipManagement *layer = LotteryMemberShipManagement::create();
	scene->addChild(layer);
	return scene;
}

LotteryMemberShipManagement::LotteryMemberShipManagement()
{
   
}
LotteryMemberShipManagement::~LotteryMemberShipManagement()
{

}

bool LotteryMemberShipManagement::init()
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

void LotteryMemberShipManagement::refreshLayer()
{
	//
	NotificationCenter::getInstance()->postNotification("Refresh_Data_Recon");
}

void LotteryMemberShipManagement::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	money = g_GlobalUnits.getGolbalUserData()->lScore;
	cellHeight = 80;
	cellNum = 10;
}

void LotteryMemberShipManagement::initView()
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
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryMemberShipManagement::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	//pCloseButton->setPosition(Vec2(32, SCREEN_HEIGHT - 120));
	pCloseButton->setAnchorPoint(Vec2(0,1));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//文字 会员中心
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t887"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 30;
	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = title->getPositionY() - 70;
	//tableViewSize.height = 700;
	auto userInfoTable = TableView::create(this, tableViewSize);
	userInfoTable->setDelegate(this);
	userInfoTable->setAnchorPoint(Vec2(0.5, 0.5));
	userInfoTable->setPosition(Vec2(0, 0));
	userInfoTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	userInfoTable->setDirection(ScrollView::Direction::VERTICAL);
	//userInfoTable->retain();			//有addchild无需retain, 当然也就无需release了
	
	this->addChild(userInfoTable, 1);
}

void LotteryMemberShipManagement::onEnter()
{
	BaseLayer::onEnter();
}

void LotteryMemberShipManagement::onExit()
{
	BaseLayer::onExit();
}

void LotteryMemberShipManagement::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryMemberShipManagement::back(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	RoomLayer *layer = RoomLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

//transaction menu
void LotteryMemberShipManagement::recharge(Object* obj)
{
	//if(showRecordInfo == true){
	//	return;
	//}
	Scene *scene = LotteryRecharge::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LotteryMemberShipManagement::withdraw(Object *obj)
{
	Scene *scene = LotteryWithdraw::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

cocos2d::Size LotteryMemberShipManagement::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 100; //高度调整为100
	return size;
}

ssize_t LotteryMemberShipManagement::numberOfCellsInTableView(TableView *table)
{
	return cellNum;
}
TableViewCell *LotteryMemberShipManagement::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 


	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(150,150,150);
	ccColor3B blackColor1 = ccc3(0,0,0);
	float fontSize = 27;
	float gap = 120;
	float textWidth = 200;

	//处理列表的图标
	Texture2D *pTexture = TextureCache::getInstance()->addImage("logo_member.png");
	cocos2d::Rect logoRect =  Rect((index)* 59, 0, 59, 59);

#ifdef VER_369
	logoRect = Rect((index)* 64, 0, 64, 63);
#endif

	if (index <= 10 && pTexture)
	{
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(pTexture, logoRect));
		//ballSprite->setAnchorPoint(Vec2(0, 0.5f));
		ballSprite->setPosition(Vec2(30 + 34 + 5, 100 / 2));		
		cell->addChild(ballSprite, 8);
	}
	LabelTTF *timeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", 
		String::createWithFormat("t%d", 888+index)->getCString())
		,"",fontSize,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	timeLabel->setColor(blackColor1);
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

	cell->setTag(index-1);

	return cell;  
}

void LotteryMemberShipManagement::tableCellTouched(TableView *table, TableViewCell *cell)
{
	int cellTag = cell->getTag();
	switch(cellTag)
	{
	case -1: //会员列表
        {
            Scene *scene = LotteryRecordSubMember::scene();
            Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 0:	//添加下级
        {
            Scene *scene = LotteryAddSub::scene();
            Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 1:	//彩票记录
        {
			Scene *scene = LotteryRecordSubGamble::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 2:	//棋牌记录
        {
			Scene *scene = LotteryRecordSubGame::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 3: //团队数据
        {
            Scene *scene = LotteryMemberShip::scene();
            Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 4:	//团队报表
		{
			Scene *scene = LotteryRecordSubProfitStatics::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 5:	//下级投注统计
		{
			Scene *scene = LotteryRecordSubGambleStatics::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;			
		}
	case 6: //盈亏明细
        {
			Scene *scene = LotteryRecordSubProfit::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;           
		}
	case 7: //下级充值
		{ 
			Scene *scene = LotteryRecordSubRecharge::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;           
		}
	case 8: //下级提现
		{ 
			Scene *scene = LotteryRecordSubWithdraw::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;           
		}
	case 9:
        {
			//屏蔽棋牌盈亏
			Scene *scene = LotteryRecordSubGameProfit::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	default:
		break;
	}
}

void LotteryMemberShipManagement::scrollViewDidScroll(ScrollView* view)
{

}
void LotteryMemberShipManagement::scrollViewDidZoom(ScrollView* view)
{

}
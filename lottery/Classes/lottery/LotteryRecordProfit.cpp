#include "LotteryRecordProfit.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecordProfit::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordProfit *layer = LotteryRecordProfit::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordProfit::LotteryRecordProfit()
{

}

LotteryRecordProfit::~LotteryRecordProfit()
{

}

bool LotteryRecordProfit::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 5;
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	this->initView();
	this->setKeypadEnabled(true);

	//今天的时间
	time_t ct;
	ct = time(NULL);
	tm *st = localtime(&ct);
	const char* todayStr = String::createWithFormat("%04d-%02d-%02d",st->tm_year + 1900,st->tm_mon + 1,st->tm_mday)->getCString();
	s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);

	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");
	//传值
	EntityMgr::instance()->getDispatch()->SendPacketWithGetYingkuiLogCount(true,s_t_start.c_str(),s_t_end.c_str());
	return true;
}

void LotteryRecordProfit::initView()
{
	setTitle(1117);
}

void LotteryRecordProfit::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
}

cocos2d::Size LotteryRecordProfit::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = recordTableSize.width;
	size.height = tableCellItemHeight*tableCellItemNum;
	return size;
}

TableViewCell* LotteryRecordProfit::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cell->setTag(idx);

	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(0,0,0);
	float fontSize = 27;

#ifdef VER_369
	redColor = ccc3(50, 62, 123);
#endif
	
	if(recordData->count() == 0){
		fontSize = 30;
		LabelTTF *hintLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml","t720"), "", fontSize);
		hintLabel->setAnchorPoint(Vec2(0.5f,0.5f));
		hintLabel->setPosition(Vec2(SCREEN_WIDTH/2, 0));
		hintLabel->setColor(blackColor);
		cell->addChild(hintLabel, 1);
		return cell;
	}

	for(int i = 0 ; i < 5 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 730+i*2)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label->setColor(blackColor);
		cell->addChild(label, 1);

		if(i==4){
			continue;
		}
		String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 731+i*2)->getCString()));
		LabelTTF *label1 = LabelTTF::create(labelStr1->getCString(), "", fontSize);
		label1->setAnchorPoint(Vec2(1,0.5f));
		label1->setPosition(Vec2(tableCellPosX2, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label1->setColor(blackColor);
		cell->addChild(label1, 1);
	}

	Dictionary *items = (Dictionary *)recordData->objectAtIndex(idx);
	Double *f_t_chongzhi = (Double *)items->objectForKey("f_t_chongzhi");
	Double *f_t_qukuan = (Double *)items->objectForKey("f_t_qukuan");
	Double *f_t_touzhu = (Double *)items->objectForKey("f_t_touzhu");
	Double *f_t_fandian = (Double *)items->objectForKey("f_t_fandian");
	Double *f_t_zhongjiang = (Double *)items->objectForKey("f_t_zhongjiang");
	Double *f_t_huodong = (Double *)items->objectForKey("f_t_huodong");
	Double *f_t_chedan = (Double *)items->objectForKey("f_t_chedan");


	LabelTTF *usernameLabel = LabelTTF::create(username.c_str(),
		"", fontSize);
	usernameLabel->setAnchorPoint(Vec2(0,0.5f));
	usernameLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	usernameLabel->setColor(redColor);
	cell->addChild(usernameLabel, 1);

	LabelTTF *typeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml","t739"),"", fontSize);
	typeLabel->setAnchorPoint(Vec2(0,0.5f));
	typeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	typeLabel->setColor(redColor);
	cell->addChild(typeLabel, 1);

	LabelTTF *f_t_chongzhiLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_chongzhi->getValue())->getCString(),
		"", fontSize);
	f_t_chongzhiLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_chongzhiLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	f_t_chongzhiLabel->setColor(redColor);
	cell->addChild(f_t_chongzhiLabel, 1);

	LabelTTF *f_t_qukuanLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_qukuan->getValue())->getCString(),
		"", fontSize);
	f_t_qukuanLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_qukuanLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	f_t_qukuanLabel->setColor(redColor);
	cell->addChild(f_t_qukuanLabel, 1);

	LabelTTF *f_t_touzhuLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_touzhu->getValue())->getCString(),
		"", fontSize);
	f_t_touzhuLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_touzhuLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	f_t_touzhuLabel->setColor(redColor);
	cell->addChild(f_t_touzhuLabel, 1);

	LabelTTF *f_t_fandianLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_fandian->getValue())->getCString(),
		"", fontSize);
	f_t_fandianLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_fandianLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	f_t_fandianLabel->setColor(redColor);
	cell->addChild(f_t_fandianLabel, 1);

	LabelTTF *f_t_zhongjiangLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_zhongjiang->getValue())->getCString(),
		"", fontSize);
	f_t_zhongjiangLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_zhongjiangLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	f_t_zhongjiangLabel->setColor(redColor);
	cell->addChild(f_t_zhongjiangLabel, 1);

	LabelTTF *f_t_huodongLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_huodong->getValue())->getCString(),
		"", fontSize);
	f_t_huodongLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_huodongLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	f_t_huodongLabel->setColor(redColor);
	cell->addChild(f_t_huodongLabel, 1);

	LabelTTF *f_t_chedanLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_chedan->getValue())->getCString(),
		"", fontSize);
	f_t_chedanLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_chedanLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*4));
	f_t_chedanLabel->setColor(redColor);
	cell->addChild(f_t_chedanLabel, 1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	
	cell->addChild(gap1,1);
	return cell;
}

ssize_t LotteryRecordProfit::numberOfCellsInTableView(TableView *table)
{
	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordProfit::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordProfit::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordProfit::sendPacket()
{
    time_t t = time(0);
    char timeStr[64] = {0};
	char startTimeStr[64] = {0};
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S.999",localtime(&t) );
    strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d 00:00:00.000",localtime(&t) );
	EntityMgr::instance()->getDispatch()->SendPacketWithGetYingkuiLog(currentPage, pageSize, true, startTimeStr,timeStr);
}
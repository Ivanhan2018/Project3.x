#include "LotteryRecordProfitStatics.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecordProfitStatics::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordProfitStatics *layer = LotteryRecordProfitStatics::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordProfitStatics::LotteryRecordProfitStatics()
{

}

LotteryRecordProfitStatics::~LotteryRecordProfitStatics()
{

}

bool LotteryRecordProfitStatics::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 4;
	this->initView();
	this->setKeypadEnabled(true);
	EntityMgr::instance()->getDispatch()->SendPacketWithGetTiXianLogCount();
	return true;
}

void LotteryRecordProfitStatics::initView()
{
	setTitle(683);
}

void LotteryRecordProfitStatics::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
}

cocos2d::Size LotteryRecordProfitStatics::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = recordTableSize.width;
	size.height = tableCellItemHeight*tableCellItemNum;
	return size;
}

TableViewCell* LotteryRecordProfitStatics::tableCellAtIndex(TableView *table, ssize_t idx)
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

	for(int i = 0 ; i < 4 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 709+i*2)->getCString()));
		//if(i == 5){
		//	nameLabel = LabelTTF::create(labelStr->getCString(), "", fontSize);
		//	nameLabel->setAnchorPoint(Vec2(1,0.5f));
		//	nameLabel->setPosition(Vec2(posX1, startY-gapY*i));
		//	nameLabel->setColor(blackColor);
		//	m_node->addChild(nameLabel, 1);
		//	continue;
		//}
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label->setColor(blackColor);
		cell->addChild(label, 1);

		if(i==3){
			continue;
		}
		String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 710+i*2)->getCString()));
		LabelTTF *label1 = LabelTTF::create(labelStr1->getCString(), "", fontSize);
		label1->setAnchorPoint(Vec2(1,0.5f));
		label1->setPosition(Vec2(tableCellPosX2, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label1->setColor(blackColor);
		cell->addChild(label1, 1);
	}

	Dictionary *items = (Dictionary *)recordData->objectAtIndex(idx);
	String *time = (String *)items->objectForKey("time");
	Integer *n_t_id = (Integer *)items->objectForKey("n_t_id");
	String *n_t_yinhang = (String *)items->objectForKey("n_t_yinhang");
	Double *f_t_jine = (Double *)items->objectForKey("f_t_jine");
	String *s_t_kaihuren = (String *)items->objectForKey("s_t_kaihuren");
	Integer *n_t_state = (Integer *)items->objectForKey("n_t_state");
	String *s_t_note = (String *)items->objectForKey("s_t_note");


	LabelTTF *n_t_idLabel = LabelTTF::create(String::createWithFormat("%d",n_t_id->getValue())->getCString(),
		"", fontSize);
	n_t_idLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_idLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	n_t_idLabel->setColor(redColor);
	cell->addChild(n_t_idLabel, 1);

	LabelTTF *timeLabel = LabelTTF::create(time->getCString(),"", fontSize);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	timeLabel->setColor(redColor);
	cell->addChild(timeLabel, 1);

	LabelTTF *n_t_yinhangLabel = LabelTTF::create(n_t_yinhang->getCString(),
		"", fontSize);
	n_t_yinhangLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_yinhangLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	n_t_yinhangLabel->setColor(redColor);
	cell->addChild(n_t_yinhangLabel, 1);

	LabelTTF *f_t_jineLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_jine->getValue())->getCString(),
		"", fontSize);
	f_t_jineLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_jineLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	f_t_jineLabel->setColor(redColor);
	cell->addChild(f_t_jineLabel, 1);

	LabelTTF *s_t_kaihurenLabel = LabelTTF::create(s_t_kaihuren->getCString(),
		"", fontSize);
	s_t_kaihurenLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_kaihurenLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	s_t_kaihurenLabel->setColor(redColor);
	cell->addChild(s_t_kaihurenLabel, 1);

	String *n_t_stateStr = String::create(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 4350+n_t_state->getValue())->getCString()));
	LabelTTF *n_t_stateLabel = LabelTTF::create(n_t_stateStr->getCString(),
		"", fontSize);
	n_t_stateLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_stateLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	n_t_stateLabel->setColor(redColor);
	cell->addChild(n_t_stateLabel, 1);

	LabelTTF *s_t_noteLabel = LabelTTF::create(s_t_note->getCString(),
		"", fontSize);
	s_t_noteLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_noteLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	s_t_noteLabel->setColor(redColor);
	cell->addChild(s_t_noteLabel, 1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	
	cell->addChild(gap1,1);
	return cell;
}

ssize_t LotteryRecordProfitStatics::numberOfCellsInTableView(TableView *table)
{
	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordProfitStatics::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordProfitStatics::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordProfitStatics::sendPacket()
{
	EntityMgr::instance()->getDispatch()->SendPacketWithGetTiXianLog(currentPage, pageSize);
}
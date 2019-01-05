#include "LotteryRecordSubWithdraw.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "comm.h"
#include "BetLimit.h"
using namespace CocosDenshion;

Scene* LotteryRecordSubWithdraw::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordSubWithdraw *layer = LotteryRecordSubWithdraw::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordSubWithdraw::LotteryRecordSubWithdraw()
{
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubWithdraw::cHKXJTZHLogCountRet), "GetXjTxLogCountRet", NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubWithdraw::cHKXJTZHLog), "GetXJTxLogRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubWithdraw::refreshLayer), "Refresh_Data_Recon", NULL);
}

LotteryRecordSubWithdraw::~LotteryRecordSubWithdraw()
{
    NotificationCenter::getInstance()->removeObserver(this, "GetXjTxLogCountRet");
    NotificationCenter::getInstance()->removeObserver(this, "GetXJTxLogRet");
	NotificationCenter::getInstance()->removeObserver(this, "Refresh_Data_Recon");
}

void LotteryRecordSubWithdraw::cHKXJTZHLogCountRet(Object *obj)
{
    recordDataCount = ((Integer *)obj)->getValue();
    totalPage = recordDataCount%pageSize!=0?(recordDataCount/pageSize+1):recordDataCount/pageSize;
    if (totalPage>20) {
        totalPage = 20;
    }
    if(recordDataCount != 0){
		EditBox *startDate = (EditBox *)getChildByTag(EDITBOX_TAG + 2);
		EditBox *endDate = (EditBox *)getChildByTag(EDITBOX_TAG + 3);

		s_t_start = BetLimit::AjustDateStr(startDate->getText());
		s_t_end = BetLimit::AjustDateStr(endDate->getText());

		startDate->setText(s_t_start.c_str());
		endDate->setText(s_t_end.c_str());

		s_t_start.append(" 00:00:00.000");
		s_t_end.append(" 23:59:59.999");

		currentPage = 1;

		sendPacket();
    }
}

void LotteryRecordSubWithdraw::cHKXJTZHLog(Object *obj)
{
    Array *data = (Array *)obj;
    recordData->removeAllObjects();
    if(data->count()>pageSize*20){
        for (int i = 0 ; i < pageSize*20 ; i++) {
            recordData->addObject(data->objectAtIndex(i));
        }
    }else{
        recordData->addObjectsFromArray(data);
    }
    recordTable->reloadData();
    this->refreshLogLabel();
    
    isQuerying = false;
}

bool LotteryRecordSubWithdraw::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 5;
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryRecordSubWithdraw::initView()
{
    setTitle(897);
    
	float posX1 = 100;
	float posX2 = 150;
	float startY = SCREEN_HEIGHT - 220;
	float gapY = 60;
	float fontSize = 26;
	ccColor3B blackColor = ccc3(76, 76, 76);

	time_t ct;
	ct = time(NULL);
	tm *st = localtime(&ct);
	const char* todayStr = String::createWithFormat("%04d-%02d-%02d",st->tm_year + 1900,st->tm_mon + 1,st->tm_mday)->getCString();	

	for(int i = 0 ; i < 4 ; i++)
	{
		int deltaLen = (i % 2)* 220;

		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", (i < 2 ? 780 : 988)+i)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX1+ 40 + deltaLen , startY-gapY*(i / 2)));
		label->setColor(blackColor);
		this->addChild(label, 1);

		Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("beting26.png"));
		inputBk->setAnchorPoint(Vec2(0,0.5f));
		inputBk->setScale(1.2f);
		inputBk->setScaleX(1.4f);
		inputBk->setPosition(Vec2(posX2 + 10 + deltaLen, label->getPositionY()));
		this->addChild(inputBk);

		EditBox *editBox = EditBox::create(cocos2d::Size(inputBk->getContentSize().width * 1.4f, 35), "blank.png");
		editBox->setPosition(inputBk->getPosition());
		editBox->setInputFlag(EditBox::InputFlag::SENSITIVE);
		editBox->setReturnType(EditBox::KeyboardReturnType::DONE);
		editBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
		editBox->setAnchorPoint(Vec2(0,0.5f));
		editBox->setFontColor(ccc3(0,0,0));
		editBox->setFontName("");editBox->setFontSize(fontSize);
		editBox->setColor(ccc3(112,112,112));
		editBox->setMaxLength(20);
		if(i == 1)
		{
			editBox->setInputMode(EditBox::InputMode::NUMERIC);
		}else
		if(i > 1)
		{
			editBox->setInputMode(EditBox::InputMode::NUMERIC);
			editBox->setMaxLength(10);
			editBox->setText(todayStr);
		}
		editBox->setTag(EDITBOX_TAG+i);
		this->addChild(editBox, 1);
	}
    
    MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryRecordSubWithdraw::pressConfirm,this));
	confirmItem->setPosition(Vec2(620, startY - gapY * 1));
    confirmItem->setScale(0.8);
    Menu *confirmMenu = Menu::create(confirmItem, NULL);
    confirmMenu->setPosition(Vec2(0,0));
    this->addChild(confirmMenu,1);
    
    Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
    gap1->setAnchorPoint(Vec2(0,0.5));
    gap1->setPosition(Vec2(0, startY-gapY*2));
    gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
    this->addChild(gap1,1);
    
	recordTable->setViewSize(cocos2d::Size(SCREEN_WIDTH, gap1->getPositionY() - 80));
    recordTable->reloadData();
	s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);

	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");

	EntityMgr::instance()->getDispatch()->SendPacketWithGetXjTxLogCount(true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
}

void LotteryRecordSubWithdraw::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
}

cocos2d::Size LotteryRecordSubWithdraw::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = recordTableSize.width;
	size.height = tableCellItemHeight*tableCellItemNum;
	return size;
}

TableViewCell* LotteryRecordSubWithdraw::tableCellAtIndex(TableView *table, ssize_t idx)
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

	for(int i = 0 ; i < 5 ; i++)
	{
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 708+i*2)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label->setColor(blackColor);
		cell->addChild(label, 1);
		if (i == 4)
			break;
		String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 709+i*2)->getCString()));
		LabelTTF *label1 = LabelTTF::create(labelStr1->getCString(), "", fontSize);
		label1->setAnchorPoint(Vec2(1,0.5f));
		label1->setPosition(Vec2(tableCellPosX2, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label1->setColor(blackColor);
		cell->addChild(label1, 1);
    }
    
	Dictionary *items = (Dictionary *)recordData->objectAtIndex(idx);
    Integer *n_t_id = (Integer *)items->objectForKey("n_t_id");
    Integer *n_t_user_id = (Integer *)items->objectForKey("n_t_user_id");
    String *s_t_account = (String *)items->objectForKey("s_t_account");
    String *time = (String *)items->objectForKey("time");
    String *s_t_yinhang = (String *)items->objectForKey("s_t_yinhang");
    Double *f_t_jine = (Double *)items->objectForKey("f_t_jine");
    String *s_t_kaihuren = (String *)items->objectForKey("s_t_kaihuiren");
    Integer *n_t_state = (Integer *)items->objectForKey("n_t_state");
    String *s_t_note = (String *)items->objectForKey("s_t_note");
    
    
    LabelTTF *n_t_idLabel = LabelTTF::create(String::createWithFormat("%d",n_t_id->getValue())->getCString(),
                                                 "", fontSize);
    n_t_idLabel->setAnchorPoint(Vec2(0,0.5f));
    n_t_idLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
    n_t_idLabel->setColor(redColor);
    cell->addChild(n_t_idLabel, 1);
    
    LabelTTF *n_t_user_idLabel = LabelTTF::create(String::createWithFormat("%d",n_t_user_id->getValue())->getCString(),"", fontSize);
    n_t_user_idLabel->setAnchorPoint(Vec2(0,0.5f));
    n_t_user_idLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
    n_t_user_idLabel->setColor(redColor);
    cell->addChild(n_t_user_idLabel, 1);
    
    LabelTTF *s_t_accountLabel = LabelTTF::create(s_t_account->getCString(),"", fontSize);
    s_t_accountLabel->setAnchorPoint(Vec2(0,0.5f));
    s_t_accountLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
    s_t_accountLabel->setColor(redColor);
    cell->addChild(s_t_accountLabel, 1);
    
    LabelTTF *timeLabel = LabelTTF::create(time->getCString(),"", fontSize);
    timeLabel->setAnchorPoint(Vec2(0,0.5f));
    timeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
    timeLabel->setColor(redColor);
    cell->addChild(timeLabel, 1);
    
    LabelTTF *n_t_yinhangLabel = LabelTTF::create(s_t_yinhang->getCString(),"", fontSize);
    n_t_yinhangLabel->setAnchorPoint(Vec2(0,0.5f));
    n_t_yinhangLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
    n_t_yinhangLabel->setColor(redColor);
    cell->addChild(n_t_yinhangLabel, 1);
    
    LabelTTF *f_t_jineLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_jine->getValue())->getCString(),
                                                   "", fontSize);
    f_t_jineLabel->setAnchorPoint(Vec2(0,0.5f));
    f_t_jineLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
    f_t_jineLabel->setColor(redColor);
    cell->addChild(f_t_jineLabel, 1);
    
	//已改成状态
	LabelTTF *s_t_kaihurenLabel = LabelTTF::create(s_t_kaihuren->getCString(), "", fontSize);
    s_t_kaihurenLabel->setAnchorPoint(Vec2(0,0.5f));
    s_t_kaihurenLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
    s_t_kaihurenLabel->setColor(redColor);
    cell->addChild(s_t_kaihurenLabel, 1);
    
    String *n_t_stateStr = String::create(ConfigMgr::instance()->text("display_text.xml",
                                                                          String::createWithFormat("t%d", 903+n_t_state->getValue())->getCString()));
    LabelTTF *n_t_stateLabel = LabelTTF::create(n_t_stateStr->getCString(),
                                                    "", fontSize);
    n_t_stateLabel->setAnchorPoint(Vec2(0,0.5f));
    n_t_stateLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
    n_t_stateLabel->setColor(redColor);
    cell->addChild(n_t_stateLabel, 1);
    
    LabelTTF *s_t_noteLabel = LabelTTF::create(s_t_note->getCString(),
                                                   "", fontSize);
    s_t_noteLabel->setAnchorPoint(Vec2(0,0.5f));
    s_t_noteLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*4));
    s_t_noteLabel->setColor(redColor);
    cell->addChild(s_t_noteLabel, 1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	cell->addChild(gap1,3);
	return cell;
}

ssize_t LotteryRecordSubWithdraw::numberOfCellsInTableView(TableView *table)
{
	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordSubWithdraw::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordSubWithdraw::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordSubWithdraw::pressConfirm(Object *obj)
{
	playButtonSound();

	EditBox *accEditBox = (EditBox *)getChildByTag(EDITBOX_TAG);
	EditBox *idEditBox = (EditBox *)getChildByTag(EDITBOX_TAG+1);
	EditBox *startDate = (EditBox *)getChildByTag(EDITBOX_TAG + 2);
	EditBox *endDate = (EditBox *)getChildByTag(EDITBOX_TAG + 3);
	const char *accStr = accEditBox->getText();
	const char *idStr = idEditBox->getText();	
	//较正日期  995错误
	int lenstart = String::create(startDate->getText())->length();
	int lenend = String::create(endDate->getText())->length();
	if( lenstart < 6 || lenstart > 10 || lenend < 6 || lenend > 10 || !BetLimit::CheckDateStr(startDate->getText()) || !BetLimit::CheckDateStr(endDate->getText()))
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml","t995"),Vec2(SCREEN_WIDTH / 2,240));
		this->addChild(layer,255);	
		return;
	}
	recordData->removeAllObjects();
	recordTable->reloadData();
	checkIsQuerying(true);

	s_t_start = BetLimit::AjustDateStr(startDate->getText());
	s_t_end = BetLimit::AjustDateStr(endDate->getText());

	startDate->setText(s_t_start.c_str());
	endDate->setText(s_t_end.c_str());

	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");

	if (strcmp(accStr, "") != 0) {
		n_t_type = 2;
		s_t_account = BetLimit::GetStrWithoutSpace(accStr);
	}else{
		if(strcmp(idStr, "") != 0)
		{
			n_t_type = 1;
			userID = atoi(idStr);
		}else{
			resetInfo();
		}
	}

	currentPage = 1;

	EntityMgr::instance()->getDispatch()->SendPacketWithGetXjTxLogCount(true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
}

void LotteryRecordSubWithdraw::sendPacket()
{    
	if (checkIsQuerying()) return;
	LotteryRecord::sendPacket();
    EntityMgr::instance()->getDispatch()->SendPacketWithGetXJTxLog(currentPage, pageSize, true, s_t_start.c_str(),s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
}

//根据彩种ID和玩法ID，返回玩法名称
const char *LotteryRecordSubWithdraw::getGameKindTitle(int gameType, int gameKind)
{
return BetLimit::GetWanFaNameFromId(gameType, gameKind).c_str();
}

void LotteryRecordSubWithdraw::refreshLayer(Object *obj)
{
	sendPacket();
}
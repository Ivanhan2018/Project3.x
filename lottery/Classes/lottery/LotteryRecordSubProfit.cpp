#include "LotteryRecordSubProfit.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "comm.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecordSubProfit::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordSubProfit *layer = LotteryRecordSubProfit::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordSubProfit::LotteryRecordSubProfit()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubProfit::getXJYKLogCountRet), "GetXJYKLogCountRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubProfit::getXJYKLogRet), "GetXJYKLogRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubProfit::refreshLayer), "Refresh_Data_Recon", NULL);
}

LotteryRecordSubProfit::~LotteryRecordSubProfit()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetXJYKLogCountRet");
	NotificationCenter::getInstance()->removeObserver(this, "GetXJYKLogRet");
	NotificationCenter::getInstance()->removeObserver(this, "Refresh_Data_Recon");
}

void LotteryRecordSubProfit::getXJYKLogRet(Object *obj)
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

void LotteryRecordSubProfit::getXJYKLogCountRet(Object *obj)
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

bool LotteryRecordSubProfit::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 4;
	nowPosIndex = 0;
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryRecordSubProfit::initView()
{
	setTitle(895);

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

	//确定
	MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryRecordSubProfit::pressConfirm,this));
	confirmItem->setPosition(Vec2(620, startY - gapY * 1));
	confirmItem->setScale(0.8);
	Menu *confirmMenu = Menu::create(confirmItem, NULL);
	confirmMenu->setPosition(Vec2(0,0));
	confirmMenu->setTag(9531);
	this->addChild(confirmMenu,1);

	//前排标志 下拉框
	Sprite* selectBk = Sprite::createWithSpriteFrame(spriteFrame("gamble_06.png"));
	selectBk->setPosition(Vec2(610, startY)); 
	selectBk->setScaleY(46.5 / selectBk->getContentSize().height);
	selectBk->setScaleX(102 / selectBk->getContentSize().width);
	this->addChild(selectBk,2);
	Sprite *normalSprite = Sprite::createWithSpriteFrame(spriteFrame("gamble_107.png"));
	Sprite *selectedSprite = Sprite::createWithSpriteFrame(spriteFrame("gamble_108.png"));
	MenuItemSprite* moreItem = MenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(LotteryRecordSubProfit::toSelect,this));
	moreItem->setPosition(Vec2(SCREEN_WIDTH - normalSprite->getContentSize().width / 2, startY));
	moreItem->setScale(0.8f);
	moreItem->setTag(9528);
	Menu *selectMenu = Menu::create(moreItem,NULL);
	selectMenu->setPosition(Vec2::ZERO);
	selectMenu->setTag(9529);
	this->addChild(selectMenu,2);

	//筛选条件
	LabelTTF* ltfSort = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1010"), "", fontSize-4);
	ltfSort->setPosition(Vec2(610, startY));
	ltfSort->setTag(9527);
	ltfSort->setColor(ccc3(255,255,255));
	this->addChild(ltfSort, 2);

#ifdef VER_369
	ltfSort->setFontFillColor(ccc3(61,8,40));
#endif

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, startY-gapY*2));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	this->addChild(gap1,1);

	Node* myNode = Node::create();	
	myNode->setPosition(Vec2(HIDE_POS_X, startY - HEIGHT_LIST / 2 - 25));
	myNode->setTag(9530);
	this->addChild(myNode, 255);

	Sprite* sptNode = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	sptNode->setScaleX(140 / sptNode->getContentSize().width);
	sptNode->setScaleY(HEIGHT_LIST / sptNode->getContentSize().height);
	myNode->addChild(sptNode);
	//初始值
	cocos2d::Size tableViewSize;	
	tableViewSize.width = 140;
	tableViewSize.height = HEIGHT_LIST;
	TableView* tvList = TableView::create(this, tableViewSize);
	tvList->setPosition(Vec2(-140 / 2 + 10, -HEIGHT_LIST / 2));
	tvList->setTag(1111);
	tvList->setColor(ccc3(200,200,200));
	tvList->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tvList->setDirection(ScrollView::Direction::VERTICAL);
	tvList->setDelegate(this);
	myNode->addChild(tvList, 1);

	tvList->reloadData();

	recordTable->setViewSize(cocos2d::Size(SCREEN_WIDTH, gap1->getPositionY() - 80));
	recordTable->reloadData();
	
	s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);

	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");

	EntityMgr::instance()->getDispatch()->SendPacketWithGetXJYKLogCount(true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str(),nowPosIndex);
}

void LotteryRecordSubProfit::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		nowPosIndex = cell->getTag();
		table->getParent()->setPositionX(HIDE_POS_X);
		MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
		if(tempSpriteItem)	tempSpriteItem->unselected();

		recordTable->setTouchEnabled(true);
		//更新
		LabelTTF* ltfDisplay = (LabelTTF *)this->getChildByTag(9527);
		if(ltfDisplay)
		{
			ltfDisplay->setString(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",1010 + nowPosIndex)->getCString()));
		}
	}
}

cocos2d::Size LotteryRecordSubProfit::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;

	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		size.width = 140;
		size.height = HEIGHT_LIST / 12;
		return size;
	}
	//表格
	size.width = recordTableSize.width;
	size.height = tableCellItemHeight*tableCellItemNum;
	return size;
}

TableViewCell* LotteryRecordSubProfit::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cell->setTag(idx);

	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){	
		
		LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",1010 + idx)->getCString()), "", 22);
		label->setAnchorPoint(Vec2(0,0.5f));
		label->setPosition(Vec2(0, 25));
		label->setColor(ccColor3B::BLACK);
		cell->addChild(label, 0);
		return cell;  
	}	
	
	//表格
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
			String::createWithFormat("t%d", 850+i*2)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label->setColor(blackColor);
		cell->addChild(label, 1);

		if(i==3){
			continue;
		}
		String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 851+i*2)->getCString()));
		LabelTTF *label1 = LabelTTF::create(labelStr1->getCString(), "", fontSize);
		label1->setAnchorPoint(Vec2(1,0.5f));
		label1->setPosition(Vec2(tableCellPosX2, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label1->setColor(blackColor);
		cell->addChild(label1, 1);
	}

	Dictionary *items = (Dictionary *)recordData->objectAtIndex(idx);
	Integer *n_t_id = (Integer *)items->objectForKey("n_t_id");
	Integer *n_t_userid = (Integer *)items->objectForKey("n_t_userid");
	String *s_t_account = (String *)items->objectForKey("s_t_account");
	Double *f_t_yingkui = (Double *)items->objectForKey("f_t_yingkui");
	Double *f_t_at_yue = (Double *)items->objectForKey("f_t_at_yue");
	String *n_t_time = (String *)items->objectForKey("n_t_time");
	Integer *n_t_type = (Integer *)items->objectForKey("n_t_type");
	String *s_t_comment = (String *)items->objectForKey("s_t_comment");


	LabelTTF *n_t_idLabel = LabelTTF::create(String::createWithFormat("%d",n_t_id->getValue())->getCString(),
		"", fontSize);
	n_t_idLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_idLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	n_t_idLabel->setColor(redColor);
	cell->addChild(n_t_idLabel, 1);

	LabelTTF *s_t_accountLabel = LabelTTF::create(s_t_account->getCString(),"", fontSize);
	s_t_accountLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_accountLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	s_t_accountLabel->setColor(redColor);
	cell->addChild(s_t_accountLabel, 1);

	LabelTTF *f_t_yingkuiLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_yingkui->getValue())->getCString(),"", fontSize);
	f_t_yingkuiLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_yingkuiLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	f_t_yingkuiLabel->setColor(redColor);
	cell->addChild(f_t_yingkuiLabel, 1);

	LabelTTF *f_t_at_yueLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_at_yue->getValue())->getCString(),"", fontSize);
	f_t_at_yueLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_at_yueLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	f_t_at_yueLabel->setColor(redColor);
	cell->addChild(f_t_at_yueLabel, 1);

	LabelTTF *n_t_timeLabel = LabelTTF::create(n_t_time->getCString(),"", fontSize);
	n_t_timeLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_timeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	n_t_timeLabel->setColor(redColor);
	cell->addChild(n_t_timeLabel, 1);

	//余额转换
	String *n_t_typeStr = String::create(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 857+n_t_type->getValue())->getCString()));
	LabelTTF *n_t_typeLabel = LabelTTF::create(n_t_typeStr->getCString(),
		"", fontSize);
	n_t_typeLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_typeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	n_t_typeLabel->setColor(redColor);
	cell->addChild(n_t_typeLabel, 1);

	char comment[127] = {0};
	memcpy(comment, s_t_comment->getCString(), strlen(s_t_comment->getCString()));
	char *token = strtok(comment, "|");
	string qiShu = "2015082724";
	int gameType = 1;
	int gameKind = 1;
	int i = 0;
	while(token != NULL){
		if(i == 0){
			qiShu = string(token);
		}else if(i == 1){
			gameType = atoi(token);
		}else if(i == 2){
			gameKind = atoi(token);
		}
		i++;
		token = strtok( NULL, "|");
	}
	String *noteStr = NULL;
	if(n_t_type->getValue() < 4 || n_t_type->getValue() == 8) //投注
	{
		noteStr = String::createWithFormat("%s[%s%s]%s",
			ConfigMgr::instance()->text("display_config.xml",String::createWithFormat("t%d", 20+gameType)->getCString()),	qiShu.c_str(),ConfigMgr::instance()->text("display_text.xml","t71"),
			BetLimit::GetWanFaNameFromId(gameType,gameKind).c_str());
	}else
	if(n_t_type->getValue() == 12)
	{
		noteStr = String::create(ConfigMgr::instance()->text("display_text.xml","t880"));
	}else
	{
		noteStr = String::createWithFormat("%s",qiShu.c_str());
	}
	LabelTTF *s_t_noteLabel = LabelTTF::create(noteStr->getCString(),
		"", fontSize);
	s_t_noteLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_noteLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	s_t_noteLabel->setColor(redColor);
	cell->addChild(s_t_noteLabel, 1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	cell->addChild(gap1,1);
	return cell;
}

ssize_t LotteryRecordSubProfit::numberOfCellsInTableView(TableView *table)
{
	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		return 12;
	}
	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordSubProfit::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordSubProfit::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordSubProfit::pressConfirm(Object *obj)
{
	playButtonSound();

	TableView* tvTemp = (TableView *)this->getChildByTag(9530)->getChildByTag(1111);	
	if(tvTemp && tvTemp->getParent()->getPositionX() == NOW_POS_X) return;
	checkIsQuerying(true);
	recordData->removeAllObjects();
	recordTable->reloadData();

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

	EntityMgr::instance()->getDispatch()->SendPacketWithGetXJYKLogCount(true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str(),nowPosIndex);
}

void LotteryRecordSubProfit::toSelect(Object *obj)
{
	playButtonSound();

	TableView* tvTemp = (TableView *)this->getChildByTag(9530)->getChildByTag(1111);
	if(tvTemp){	
		if(tvTemp->getParent()->getPositionX() == NOW_POS_X)
		{
			tvTemp->getParent()->setPositionX(HIDE_POS_X);
			MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
			if(tempSpriteItem)	tempSpriteItem->unselected();

			recordTable->setTouchEnabled(true);
		} else
		{
			tvTemp->getParent()->setPositionX(NOW_POS_X);
			MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
			if(tempSpriteItem)	tempSpriteItem->selected();

			recordTable->setTouchEnabled(false);
		}
	}
}

void LotteryRecordSubProfit::sendPacket()
{
	if (checkIsQuerying()) return;LotteryRecord::sendPacket();
	EntityMgr::instance()->getDispatch()->SendPacketWithGetXJYKLog(currentPage, pageSize, true, s_t_start.c_str(),s_t_end.c_str(), n_t_type, userID, s_t_account.c_str(), nowPosIndex);
}

//根据彩种ID和玩法ID，返回玩法名称
const char *LotteryRecordSubProfit::getGameKindTitle(int gameType, int gameKind)
{

	return BetLimit::GetWanFaNameFromId(gameType, gameKind).c_str();
}

void LotteryRecordSubProfit::refreshLayer(Object *obj)
{
	isQuerying = false;
	sendPacket();
}
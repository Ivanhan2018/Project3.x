#include "LotteryRecordSubGamble.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "comm.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecordSubGamble::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordSubGamble *layer = LotteryRecordSubGamble::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordSubGamble::LotteryRecordSubGamble()
{
 //   NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubGamble::cHKXJTZHLogCountRet), "CHKXJTZHLogCountRet", NULL);
//    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubGamble::cHKXJTZHLog), "CHKXJTZHLog", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubGamble::refreshLayer), "Refresh_Data_Recon", NULL);

	caizhong.clear();
	//全部彩种个数  
	int count = atoi(ConfigMgr::instance()->text("display_config.xml","t18"));
	//初始化彩种列表
	for(int i = 0;i < count;i ++)
	{
		//没有使用，或者已经放弃的彩种
		if(i == 2 || i == 7 || i == 18 || i == 19 || i == 20 || i == 21 || i == 22 || i == 25 || i==29 || i==30 || i==31 || i==32)
			continue;
		//添加进列表
		caizhong.push_back(i);
	}
}

LotteryRecordSubGamble::~LotteryRecordSubGamble()
{
 //   NotificationCenter::getInstance()->removeObserver(this, "CHKXJTZHLogCountRet");
 //   NotificationCenter::getInstance()->removeObserver(this, "CHKXJTZHLog");
	NotificationCenter::getInstance()->removeObserver(this,"Refresh_Data_Recon");

	caizhong.clear();
}

void LotteryRecordSubGamble::cHKXJTZHLogCountRet(Object *obj)
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

		s_t_start.append(" 00:00:00:000");
		s_t_end.append(" 23:59:59:999");

		currentPage = 1;

		sendPacket();
    }
}
void LotteryRecordSubGamble::cHKXJTZHLog(Object *obj)
{
    Array *data = (Array *)obj;
    recordData->removeAllObjects();
	CCLOG("%d",pageSize);
	CCLOG("%d",data->count());
    if(data->count()>pageSize*20){
        for (int i = 0 ; i < pageSize*20 ; i++) {
            recordData->addObject(data->objectAtIndex(i));
        }
    }else{
        recordData->addObjectsFromArray(data);
    }
    recordTable->reloadData();
    this->refreshLogLabel();
}

bool LotteryRecordSubGamble::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 5;
	tableCellItemHeight = 50;
	nowPosIndex = 0;
	nowStatusIndex = 0;
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

//查看下级投注
void LotteryRecordSubGamble::initView()
{
    setTitle(890);
    
	float posX1 = 100;
	float posX2 = 150;
	float startY = SCREEN_HEIGHT - 200;
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
    
    MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryRecordSubGamble::pressConfirm,this));
	confirmItem->setPosition(Vec2(620, startY - gapY * 2));
    confirmItem->setScale(0.8);
    Menu *confirmMenu = Menu::create(confirmItem, NULL);
    confirmMenu->setPosition(Vec2(0,0));
    this->addChild(confirmMenu,1);

	//前排标志 下拉框
	Sprite* selectBk = Sprite::createWithSpriteFrame(spriteFrame("gamble_06.png"));
	selectBk->setPosition(Vec2(610, startY));
	selectBk->setScaleY(46.5 / selectBk->getContentSize().height);
	selectBk->setScaleX(102 / selectBk->getContentSize().width);
	this->addChild(selectBk,2);
	Sprite *normalSprite = Sprite::createWithSpriteFrame(spriteFrame("gamble_107.png"));
	Sprite *selectedSprite = Sprite::createWithSpriteFrame(spriteFrame("gamble_108.png"));
	MenuItemSprite* moreItem = MenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(LotteryRecordSubGamble::toSelect,this));
	moreItem->setPosition(Vec2(SCREEN_WIDTH - normalSprite->getContentSize().width / 2, startY));
	moreItem->setScale(0.8f);
	moreItem->setTag(9528);
	Menu *selectMenu = Menu::create(moreItem,NULL);
	selectMenu->setPosition(Vec2::ZERO);
	selectMenu->setTag(9529);
	this->addChild(selectMenu,2);

	//筛选条件
	LabelTTF* ltfSort = LabelTTF::create(ConfigMgr::instance()->text("display_config.xml", "t20"), "", fontSize-4);
	ltfSort->setPosition(Vec2(610, startY));
	ltfSort->setTag(9527);
	ltfSort->setDimensions(cocos2d::Size(100,22));
	ltfSort->setHorizontalAlignment(TextHAlignment::LEFT);
	ltfSort->setColor(ccc3(255,255,255));
	this->addChild(ltfSort, 2);

#ifdef VER_369
	ltfSort->setFontFillColor(ccc3(61,8,40));
#endif

	//增加状态
	//前排标志 下拉框
	Sprite* selectBk1 = Sprite::createWithSpriteFrame(spriteFrame("gamble_06.png"));
	selectBk1->setPosition(Vec2(610,  startY - gapY * 1));
	selectBk1->setScaleY(46.5 / selectBk->getContentSize().height);
	selectBk1->setScaleX(102 / selectBk->getContentSize().width);
	this->addChild(selectBk1,2);
	Sprite *normalSprite1 = Sprite::createWithSpriteFrame(spriteFrame("gamble_107.png"));
	Sprite *selectedSprite1 = Sprite::createWithSpriteFrame(spriteFrame("gamble_108.png"));
	MenuItemSprite* moreItem1 = MenuItemSprite::create(normalSprite1,selectedSprite1,CC_CALLBACK_1(LotteryRecordSubGamble::toSelectStatus,this));
	moreItem1->setPosition(Vec2(SCREEN_WIDTH - normalSprite1->getContentSize().width / 2,  startY - gapY * 1));
	moreItem1->setScale(0.8f);
	moreItem1->setTag(9531);
	Menu *selectMenu1 = Menu::create(moreItem1,NULL);
	selectMenu1->setPosition(Vec2::ZERO);
	selectMenu1->setTag(9532);
	this->addChild(selectMenu1,2);

	//筛选条件
	LabelTTF* ltfSort1 = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1510"), "", fontSize-4);
	ltfSort1->setPosition(Vec2(610,  startY - gapY * 1));
	ltfSort1->setTag(9533);
	ltfSort1->setDimensions(cocos2d::Size(100,22));
	ltfSort1->setHorizontalAlignment(TextHAlignment::LEFT);
	ltfSort1->setColor(ccc3(255,255,255));
	this->addChild(ltfSort1, 2);

#ifdef VER_369
	ltfSort1->setFontFillColor(ccc3(61,8,40));
#endif

	//    
    Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
    gap1->setAnchorPoint(Vec2(0,0.5));
    gap1->setPosition(Vec2(0, startY-gapY*3 + 20));
    gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
    this->addChild(gap1,1);

	Node* myNode = Node::create();	
	myNode->setPosition(Vec2(HIDE_POS_XX, startY - HEIGHT_LISTX / 2 - 25));
	myNode->setTag(9530);
	this->addChild(myNode, 255);

	Node* myNode1 = Node::create();	
	myNode1->setPosition(Vec2(HIDE_POS_XX, startY - HEIGHT_LIST_H / 2 - 25 - gapY));
	myNode1->setTag(9535);
	this->addChild(myNode1, 255);

	Sprite* sptNode = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	sptNode->setScaleX(WIDTH_LISTX / sptNode->getContentSize().width);
	sptNode->setScaleY(HEIGHT_LISTX / sptNode->getContentSize().height);
	myNode->addChild(sptNode);

	Sprite* sptNode1 = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	sptNode1->setScaleX(WIDTH_LISTX / sptNode1->getContentSize().width);
	sptNode1->setScaleY(HEIGHT_LIST_H / sptNode1->getContentSize().height);
	myNode1->addChild(sptNode1);
	//初始值
	cocos2d::Size tableViewSize;	
	tableViewSize.width = WIDTH_LISTX;
	tableViewSize.height = HEIGHT_LISTX;
	TableView* tvList = TableView::create(this, tableViewSize);
	tvList->setPosition(Vec2(-WIDTH_LISTX / 2 + 10, -HEIGHT_LISTX / 2));
	tvList->setTag(1111);
	tvList->setColor(ccc3(200,200,200));
	tvList->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tvList->setDirection(ScrollView::Direction::VERTICAL);
	tvList->setDelegate(this);
	myNode->addChild(tvList, 1);

	tvList->reloadData();

	//初始值
	cocos2d::Size tableViewSize1;	
	tableViewSize1.width = WIDTH_LISTX;
	tableViewSize1.height = HEIGHT_LIST_H - 4;
	TableView* tvList1 = TableView::create(this, tableViewSize1);
	tvList1->setPosition(Vec2(-WIDTH_LISTX / 2 + 10, -HEIGHT_LIST_H / 2));
	tvList1->setTag(2222);
	tvList1->setColor(ccc3(200,200,200));
	tvList1->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tvList1->setDirection(ScrollView::Direction::VERTICAL);
	tvList1->setDelegate(this);
	myNode1->addChild(tvList1, 1);

	tvList1->reloadData();

    
	recordTable->setViewSize(cocos2d::Size(SCREEN_WIDTH, gap1->getPositionY() - 80));
    recordTable->reloadData();

    s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);

	s_t_start.append(" 00:00:00:000");
	s_t_end.append(" 23:59:59:999");
//数组大小，没有判断哦
	if(nowPosIndex < caizhong.size())
		EntityMgr::instance()->getDispatch()->SendPacketWithCHKXJTZHLogCount(caizhong.at(nowPosIndex),status[nowStatusIndex],true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());

}

void LotteryRecordSubGamble::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		nowPosIndex = cell->getTag();
		table->getParent()->setPositionX(HIDE_POS_XX);
		MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
		if(tempSpriteItem)	tempSpriteItem->unselected();

		recordTable->setTouchEnabled(true);
		//更新
		LabelTTF* ltfDisplay = (LabelTTF *)this->getChildByTag(9527);
		if(ltfDisplay)
		{
			ltfDisplay->setString(ConfigMgr::instance()->text("display_config.xml",String::createWithFormat("t%d",20 + caizhong.at(nowPosIndex))->getCString()));
		}
	} else
	if(tag == 2222)
	{
		nowStatusIndex = cell->getTag();
		table->getParent()->setPositionX(HIDE_POS_XX);
		MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9532)->getChildByTag(9531);
		if(tempSpriteItem)	tempSpriteItem->unselected();

		recordTable->setTouchEnabled(true);
		//更新
		LabelTTF* ltfDisplay = (LabelTTF *)this->getChildByTag(9533);
		if(ltfDisplay)
		{
			ltfDisplay->setString(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",1510 + nowStatusIndex)->getCString()));
		}
	}
}

cocos2d::Size LotteryRecordSubGamble::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;

	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111 || tag == 2222){
		size.width = WIDTH_LISTX;
		size.height = 50;
		return size;
	}

	size.width = recordTableSize.width;
	size.height = tableCellItemHeight*tableCellItemNum;
	return size;
}

//下级投注处理
TableViewCell* LotteryRecordSubGamble::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cell->setTag(idx);

	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){	

		LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_config.xml",String::createWithFormat("t%d",20 + caizhong.at(idx))->getCString()), "", 22);
		label->setAnchorPoint(Vec2(0,0.5f));
		label->setPosition(Vec2(0, 25));
		label->setColor(ccColor3B::BLACK);
		cell->addChild(label, 0);
		return cell;  
	}else
		if(tag == 2222){	

			LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",1510 + idx)->getCString()), "", 22);
			label->setAnchorPoint(Vec2(0,0.5f));
			label->setPosition(Vec2(0, 25));
			label->setColor(ccColor3B::BLACK);
			cell->addChild(label, 0);
			return cell;  
		}

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
			String::createWithFormat("t%d", 830+i*2)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label->setColor(blackColor);
		cell->addChild(label, 1);

		if(i==4){
			continue;
		}
		String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 831+i*2)->getCString()));
		LabelTTF *label1 = LabelTTF::create(labelStr1->getCString(), "", fontSize);
		label1->setAnchorPoint(Vec2(1,0.5f));
		label1->setPosition(Vec2(tableCellPosX2, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label1->setColor(blackColor);
		cell->addChild(label1, 1);
    }
    
	Dictionary *items = (Dictionary *)recordData->objectAtIndex(idx);
    Integer *n_t_id = (Integer *)items->objectForKey("n_t_id");
    String *s_t_account = (String *)items->objectForKey("s_t_account");
    Integer *n_t_type = (Integer *)items->objectForKey("n_t_type");
    Integer *n_t_kind = (Integer *)items->objectForKey("n_t_kind");
    String *s_t_qishu = (String *)items->objectForKey("s_t_qishu");
    Integer *n_t_zhushu = (Integer *)items->objectForKey("n_t_zhushu");
    Integer *n_t_moshi = (Integer *)items->objectForKey("n_t_moshi");
    Integer *n_t_beishu = (Integer *)items->objectForKey("n_t_beishu");
    String *n_t_time = (String *)items->objectForKey("n_t_time");
    Integer *n_t_state = (Integer *)items->objectForKey("n_t_state");
    Integer *n_t_winzhushu = (Integer *)items->objectForKey("n_t_winzhushu");
    Double *f_t_yingkui = (Double *)items->objectForKey("f_t_yingkui");
    Integer *n_t_zhuihao = (Integer *)items->objectForKey("n_t_zhuihao");	

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
    
	//if(n_t_type->getValue() > 26) n_t_type = Integer::create(21);

    LabelTTF *gameTypeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_config.xml", String::createWithFormat("t%d", 20+n_t_type->getValue())->getCString()),
            "",fontSize);
    gameTypeLabel->setColor(redColor);
    gameTypeLabel->setAnchorPoint(Vec2(0,0.5f));
    gameTypeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
    cell->addChild(gameTypeLabel, 1);
    
    LabelTTF *gameKindLabel = LabelTTF::create(BetLimit::GetWanFaNameFromId(n_t_type->getValue(),n_t_kind->getValue()).c_str(),"",fontSize);
    gameKindLabel->setColor(redColor);
    gameKindLabel->setAnchorPoint(Vec2(0,0.5f));
    gameKindLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
    cell->addChild(gameKindLabel, 1);

	LabelTTF *s_t_qishuLabel = LabelTTF::create(s_t_qishu->getCString(),
		"", fontSize);
	s_t_qishuLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_qishuLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	s_t_qishuLabel->setColor(redColor);
	cell->addChild(s_t_qishuLabel, 1);

    float baseValue = 0;
	if (n_t_moshi->getValue() == 0){
        baseValue = 2;
    }else if(n_t_moshi->getValue() == 1){
        baseValue = 0.2;
    }else if(n_t_moshi->getValue() == 2)
    {
        baseValue = 0.02;
    }else if(n_t_moshi->getValue() == 3)
	{
		baseValue = 0.002;
	}
	//幸运28和六合彩分开处理
	if(n_t_type->getValue() == CZ_LUCKY_28 || n_t_type->getValue() == CZ_HK6HeCai)
	{
		baseValue = 1;
	}
	
	CCLOG("TOUZHUJINE == %.02ff",baseValue*n_t_zhushu->getValue()*n_t_beishu->getValue());
    String *touzhuJine = String::createWithFormat("%.02f%s", baseValue*n_t_zhushu->getValue()*n_t_beishu->getValue(), ConfigMgr::instance()->text("display_text.xml","t78"));
	LabelTTF *f_t_jineLabel = LabelTTF::create(touzhuJine->getCString(),"", fontSize);
	f_t_jineLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_jineLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	f_t_jineLabel->setColor(redColor);
	cell->addChild(f_t_jineLabel, 1);

	LabelTTF *n_t_timeLabel = LabelTTF::create(n_t_time->getCString(),
		"", fontSize);
	n_t_timeLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_timeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	n_t_timeLabel->setColor(redColor);
	cell->addChild(n_t_timeLabel, 1);

	ccColor3B statusColor = ccc3(255,255,255);

	//订单状态：未开，已开（未中，中奖），撤单
	String *n_t_stateStr = NULL;
	//中奖
	if (n_t_winzhushu->getValue() > 0)
	{
		n_t_stateStr = String::createWithFormat("%s%ld%s", ConfigMgr::instance()->text("display_DuangDong.xml",
			String::createWithFormat("t9")->getCString()), n_t_winzhushu->getValue(), ConfigMgr::instance()->text("display_DuangDong.xml", String::createWithFormat("t13")->getCString()));

		statusColor = ccc3(231,76,69);
	}
	else
		{
		n_t_stateStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 579 + n_t_state->getValue())->getCString()));

		if(n_t_state->getValue() == 1) 
			statusColor = ccc3(25,143,2);
		else
			statusColor = ccc3(6,111,199);
				
		if(n_t_state->getValue() == 2 && n_t_zhuihao->getValue() > 2)
		{
			n_t_stateStr = String::create(ConfigMgr::instance()->text("display_text.xml",
				String::createWithFormat("t817")->getCString()));			
		}

		if(n_t_state->getValue() > 2)
		{
			n_t_stateStr = String::create(ConfigMgr::instance()->text("display_text.xml",
				String::createWithFormat("t818")->getCString()));			
		}
	}

	LabelTTF *n_t_stateLabel = LabelTTF::create(n_t_stateStr->getCString(),
		"", fontSize);
	n_t_stateLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_stateLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	n_t_stateLabel->setColor(statusColor);
	cell->addChild(n_t_stateLabel, 1);
    
    String *touzhuYinkui = String::createWithFormat("%.3lf%s", f_t_yingkui->getValue(), ConfigMgr::instance()->text("display_text.xml","t78"));
    LabelTTF *f_t_yingkuiLabel = LabelTTF::create(touzhuYinkui->getCString(),"", fontSize);
    f_t_yingkuiLabel->setAnchorPoint(Vec2(0,0.5f));
    f_t_yingkuiLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*4));
    f_t_yingkuiLabel->setColor(redColor);
    cell->addChild(f_t_yingkuiLabel, 1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	cell->addChild(gap1,3);

	return cell;
}

ssize_t LotteryRecordSubGamble::numberOfCellsInTableView(TableView *table)
{
	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		return caizhong.size();
	}else
	if (tag == 2222)
	{
		return 7;
	}

	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordSubGamble::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordSubGamble::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordSubGamble::pressConfirm(Object *obj)
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

	checkIsQuerying(true);

	recordData->removeAllObjects();
	recordTable->reloadData();

	s_t_start = BetLimit::AjustDateStr(startDate->getText());
	s_t_end = BetLimit::AjustDateStr(endDate->getText());

	startDate->setText(s_t_start.c_str());
	endDate->setText(s_t_end.c_str());

	s_t_start.append(" 00:00:00:000");
	s_t_end.append(" 23:59:59:999");

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

	EntityMgr::instance()->getDispatch()->SendPacketWithCHKXJTZHLogCount(caizhong.at(nowPosIndex),status[nowStatusIndex],true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
	return; 
}

void LotteryRecordSubGamble::sendPacket()
{
 if (checkIsQuerying()) return;LotteryRecord::sendPacket();
    EntityMgr::instance()->getDispatch()->SendPacketWithCHKXJTZHLog(caizhong.at(nowPosIndex),status[nowStatusIndex],currentPage, pageSize, true, s_t_start.c_str(),s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
}

//根据彩种ID和玩法ID，返回玩法名称
const char *LotteryRecordSubGamble::getGameKindTitle(int gameType, int gameKind)
{
	return BetLimit::GetWanFaNameFromId(gameType, gameKind).c_str();
}

void LotteryRecordSubGamble::refreshLayer(Object *obj)
{	
	isQuerying = false;
	sendPacket();
}

void LotteryRecordSubGamble::toSelect(Object *obj)
{
	playButtonSound();

	TableView* tvTemp = (TableView *)this->getChildByTag(9530)->getChildByTag(1111);
	if(tvTemp){	
		if(tvTemp->getParent()->getPositionX() == NOW_POS_XX)
		{
			tvTemp->getParent()->setPositionX(HIDE_POS_XX);
			MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
			if(tempSpriteItem)	tempSpriteItem->unselected();

			recordTable->setTouchEnabled(true);
		} else
		{
			tvTemp->getParent()->setPositionX(NOW_POS_XX);
			MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
			if(tempSpriteItem)	tempSpriteItem->selected();

			recordTable->setTouchEnabled(false);
		}
	}

	TableView* tvTemp1 = (TableView *)this->getChildByTag(9535)->getChildByTag(2222);
	if(tvTemp1){	
		if(tvTemp1->getParent()->getPositionX() == NOW_POS_XX)
		{
			tvTemp1->getParent()->setPositionX(HIDE_POS_XX);
		}
	}
}

void LotteryRecordSubGamble::toSelectStatus(Object *obj)
{
	playButtonSound();

	TableView* tvTemp = (TableView *)this->getChildByTag(9535)->getChildByTag(2222);
	if(tvTemp){	
		if(tvTemp->getParent()->getPositionX() == NOW_POS_XX)
		{
			tvTemp->getParent()->setPositionX(HIDE_POS_XX);
			MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9532)->getChildByTag(9531);
			if(tempSpriteItem)	tempSpriteItem->unselected();

			recordTable->setTouchEnabled(true);
		} else
		{
			tvTemp->getParent()->setPositionX(NOW_POS_XX);
			MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9532)->getChildByTag(9531);
			if(tempSpriteItem)	tempSpriteItem->selected();

			recordTable->setTouchEnabled(false);
		}
	}

	TableView* tvTemp1 = (TableView *)this->getChildByTag(9530)->getChildByTag(1111);
	if(tvTemp1){	
		if(tvTemp1->getParent()->getPositionX() == NOW_POS_XX)
		{
			tvTemp1->getParent()->setPositionX(HIDE_POS_XX);	
		} 
	}
}
#include "LotteryRecordGame.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecordGame::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordGame *layer = LotteryRecordGame::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordGame::LotteryRecordGame()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordGame::refreshLayer), "Refresh_Data_Recon", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordGame::getGameKindRet), "GetGameKindRet", NULL);
}

LotteryRecordGame::~LotteryRecordGame()
{
	NotificationCenter::getInstance()->removeObserver(this, "Refresh_Data_Recon");
	NotificationCenter::getInstance()->removeObserver(this, "GetGameKindRet");

	if(m_arr_game)
	{
		m_arr_game->release();
	}
}

bool LotteryRecordGame::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 3;
	m_sort_index = 0;
	m_arr_game = Array::create();
	m_arr_game->retain();
	this->initView();
	this->setKeypadEnabled(true);
    
	return true;
}

void LotteryRecordGame::initView()
{
    setTitle(1118);

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

	for(int i = 0 ; i < 2 ; i++)
	{
		int deltaLen = (i % 2)* 220;

		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 990+i)->getCString()));
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
		
		editBox->setInputMode(EditBox::InputMode::NUMERIC);
		editBox->setMaxLength(10);
		editBox->setText(todayStr);
			
		editBox->setTag(EDITBOX_TAG+i);
		this->addChild(editBox, 1);
	}

	MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryRecordGame::pressConfirm,this));
	confirmItem->setPosition(Vec2(620, startY - gapY));
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
	MenuItemSprite* moreItem = MenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(LotteryRecordGame::toSelect,this));
	moreItem->setPosition(Vec2(SCREEN_WIDTH - normalSprite->getContentSize().width / 2, startY));
	moreItem->setScale(0.8f);
	moreItem->setTag(9528);
	Menu *selectMenu = Menu::create(moreItem,NULL);
	selectMenu->setPosition(Vec2::ZERO);
	selectMenu->setTag(9529);
	this->addChild(selectMenu,2);

	//筛选条件
	LabelTTF* ltfSort = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t2050"), "", fontSize-4);
	ltfSort->setPosition(Vec2(610, startY));
	ltfSort->setTag(9527);
	ltfSort->setDimensions(cocos2d::Size(100,22));
	ltfSort->setHorizontalAlignment(TextHAlignment::LEFT);
	ltfSort->setColor(ccc3(255,255,255));
	this->addChild(ltfSort, 2);	

#ifdef VER_369
	ltfSort->setFontFillColor(ccc3(61,8,40));
#endif

	Node* myNode = Node::create();	
	myNode->setPosition(Vec2(HIDE_POS_XX, startY - HEIGHT_LISTX / 2 - 25));
	myNode->setTag(9530);
	this->addChild(myNode, 255);	

	Sprite* sptNode = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	sptNode->setScaleX(WIDTH_LISTX / sptNode->getContentSize().width);
	sptNode->setScaleY(HEIGHT_LISTX / sptNode->getContentSize().height);
	myNode->addChild(sptNode);

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
	
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, startY-gapY*2));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	this->addChild(gap1,1);

	recordTable->setViewSize(cocos2d::Size(SCREEN_WIDTH, gap1->getPositionY() - 80));
	recordTable->reloadData();

	s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);

	s_t_start.append(" 00:00:00:000");
	s_t_end.append(" 23:59:59:999");

	//获取游戏列表
	EntityMgr::instance()->getDispatch()->SendPacketWithGetGameKind(4);
	resetInfo();
	n_t_type=2;		//by time, by userid
	EntityMgr::instance()->getDispatch()->SendPacketWithGetQiPaiYingkuiLogCount(n_t_type, 1,"",s_t_start.c_str(),s_t_end.c_str(),m_sort_index);
}

void LotteryRecordGame::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		m_sort_index = cell->getTag();
		table->getParent()->setPositionX(HIDE_POS_XX);
		MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
		if(tempSpriteItem)	tempSpriteItem->unselected();

		recordTable->setTouchEnabled(true);
		//更新
		LabelTTF* ltfDisplay = (LabelTTF *)this->getChildByTag(9527);
		if(ltfDisplay)
		{
			Dictionary* dic = (Dictionary*)m_arr_game->objectAtIndex(m_sort_index);
			String* tempStr = (String*)dic->objectForKey("s_t_Name");

			ltfDisplay->setString(tempStr->getCString());
		}
	}
}

cocos2d::Size LotteryRecordGame::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;

	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		size.width = WIDTH_LISTX;
		size.height = 50;
		return size;
	}

	size.width = recordTableSize.width;
	size.height = tableCellItemHeight*tableCellItemNum;
	return size;
}

TableViewCell* LotteryRecordGame::tableCellAtIndex(TableView *table, ssize_t idx)
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

	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111 && m_arr_game->count() > idx){	

		Dictionary* dic = (Dictionary*)m_arr_game->objectAtIndex(idx);
		String* tempStr = (String*)dic->objectForKey("s_t_Name");

		LabelTTF *label = LabelTTF::create(tempStr->getCString(), "", 22);
		label->setAnchorPoint(Vec2(0,0.5f));
		label->setPosition(Vec2(0, 25));
		label->setColor(ccColor3B::BLACK);
		cell->addChild(label, 0);
		return cell;  
	}    
    
    if(recordData->count() == 0){
        fontSize = 30;
        LabelTTF *hintLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml","t720"), "", fontSize);
        hintLabel->setAnchorPoint(Vec2(0.5f,0.5f));
		hintLabel->setPosition(Vec2(SCREEN_WIDTH/2, 0));
        hintLabel->setColor(blackColor);
        cell->addChild(hintLabel, 1);
        return cell;
    }
    
    for(int i = 0 ; i < 3 ; i++){
        String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
                                                                          String::createWithFormat("t%d", 750+i*2)->getCString()));
        LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
        label->setAnchorPoint(Vec2(1,0.5f));
        label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
        label->setColor(blackColor);
        cell->addChild(label, 1);
        
        String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml",
                                                                           String::createWithFormat("t%d", 751+i*2)->getCString()));
        LabelTTF *label1 = LabelTTF::create(labelStr1->getCString(), "", fontSize);
        label1->setAnchorPoint(Vec2(1,0.5f));
        label1->setPosition(Vec2(tableCellPosX2, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
        label1->setColor(blackColor);
        cell->addChild(label1, 1);
    }
    
    Dictionary *items = (Dictionary *)recordData->objectAtIndex(idx);
    Integer *n_t_userid = (Integer *)items->objectForKey("n_t_userid");
    Integer *n_t_drawid = (Integer *)items->objectForKey("n_t_drawid");
    Double *f_t_score = (Double *)items->objectForKey("f_t_score");
    Integer *n_t_playcount = (Integer *)items->objectForKey("n_t_playcount");
    Double *f_t_revenue = (Double *)items->objectForKey("f_t_revenue");
    String *time = (String *)items->objectForKey("time");
    String *s_t_servername = (String *)items->objectForKey("s_t_servername");
    String *s_t_accounts = (String *)items->objectForKey("s_t_accounts");
    
    
    LabelTTF *n_t_useridLabel = LabelTTF::create(String::createWithFormat("%d",n_t_userid->getValue())->getCString(),
                                                     "", fontSize);
    n_t_useridLabel->setAnchorPoint(Vec2(0,0.5f));
    n_t_useridLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
    n_t_useridLabel->setColor(redColor);
    cell->addChild(n_t_useridLabel, 1);
    
    LabelTTF *s_t_accountsLabel = LabelTTF::create(s_t_accounts->getCString(),
                                                       "", fontSize);
    s_t_accountsLabel->setAnchorPoint(Vec2(0,0.5f));
    s_t_accountsLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
    s_t_accountsLabel->setColor(redColor);
    cell->addChild(s_t_accountsLabel, 1);
    
    LabelTTF *s_t_servernameLabel = LabelTTF::create(s_t_servername->getCString(),
                                                         "", fontSize);
    s_t_servernameLabel->setAnchorPoint(Vec2(0,0.5f));
    s_t_servernameLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
    s_t_servernameLabel->setColor(redColor);
    cell->addChild(s_t_servernameLabel, 1);
    
    LabelTTF *n_t_drawidLabel = LabelTTF::create(String::createWithFormat("%d",n_t_drawid->getValue())->getCString(),
                                                     "", fontSize);
    n_t_drawidLabel->setAnchorPoint(Vec2(0,0.5f));
    n_t_drawidLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
    n_t_drawidLabel->setColor(redColor);
    cell->addChild(n_t_drawidLabel, 1);
    
    LabelTTF *timeLabel = LabelTTF::create(time->getCString(),
                                               "", fontSize);
    timeLabel->setAnchorPoint(Vec2(0,0.5f));
    timeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
    timeLabel->setColor(redColor);
    cell->addChild(timeLabel, 1);
    
    LabelTTF *f_t_scoreLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_score->getValue())->getCString(),
                                                    "", fontSize);
    f_t_scoreLabel->setAnchorPoint(Vec2(0,0.5f));
    f_t_scoreLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
    f_t_scoreLabel->setColor(redColor);
    cell->addChild(f_t_scoreLabel, 1);
    
    //LabelTTF *n_t_playcountLabel = LabelTTF::create(String::createWithFormat("%d",n_t_playcount->getValue())->getCString(),
    //                                                    "", fontSize);
    //n_t_playcountLabel->setAnchorPoint(Vec2(0,0.5f));
    //n_t_playcountLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
    //n_t_playcountLabel->setColor(redColor);
    //cell->addChild(n_t_playcountLabel, 1);
    //
    //LabelTTF *f_t_revenueLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_revenue->getValue())->getCString(),
    //                                                  "", fontSize);
    //f_t_revenueLabel->setAnchorPoint(Vec2(0,0.5f));
    //f_t_revenueLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
    //f_t_revenueLabel->setColor(redColor);
    //cell->addChild(f_t_revenueLabel, 1);
    
    Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
    gap1->setAnchorPoint(Vec2(0,0.5));
    gap1->setPosition(Vec2(0, 0));
    gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
    cell->addChild(gap1,1);
    return cell;
}

ssize_t LotteryRecordGame::numberOfCellsInTableView(TableView *table)
{

	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		return m_arr_game->count();
	}

	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordGame::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordGame::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordGame::pressConfirm(Object *obj)
{
	MenuItemSprite* tempSpriteItem = (MenuItemSprite *)this->getChildByTag(9529)->getChildByTag(9528);
	if(tempSpriteItem && tempSpriteItem->isSelected())
	{
		return;
	}

	playButtonSound();
	EditBox *startDate = (EditBox *)getChildByTag(EDITBOX_TAG);
	EditBox *endDate = (EditBox *)getChildByTag(EDITBOX_TAG+1);

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

	if(m_sort_index >= m_arr_game->count()) return;
	Dictionary* dic = (Dictionary* )m_arr_game->objectAtIndex(m_sort_index);
	Integer* nowInt = (Integer *)dic->objectForKey("n_t_kindid");

	currentPage = 1;

    EntityMgr::instance()->getDispatch()->SendPacketWithGetQiPaiYingkuiLogCount(n_t_type,1,"",s_t_start.c_str(),s_t_end.c_str(),nowInt->getValue());   
}

bool LotteryRecordGame::isValidUserID(const char *text)
{
    return true;
}

void LotteryRecordGame::sendPacket()
{	
	if(m_sort_index >= m_arr_game->count()) return;
	Dictionary* dic = (Dictionary* )m_arr_game->objectAtIndex(m_sort_index);
	Integer* nowInt = (Integer *)dic->objectForKey("n_t_kindid");
    EntityMgr::instance()->getDispatch()->SendPacketWithGetQiPaiYingkuiLog(currentPage, pageSize,true,s_t_start.c_str(),s_t_end.c_str(),nowInt->getValue());
}

void LotteryRecordGame::refreshLayer(Object *obj)
{
	sendPacket();
}

void LotteryRecordGame::getGameKindRet(Object *obj)
{
	Array* tempArr = (Array *)obj;
	if(tempArr == nullptr) return;

	if(m_arr_game)
	{
		m_arr_game->removeAllObjects();

		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create(0), "n_t_kindid");	
		dictionary->setObject(String::create(ConfigMgr::instance()->text("display_text.xml", "t2050")), "s_t_Name");	
		m_arr_game->addObject(dictionary);

		m_arr_game->addObjectsFromArray(tempArr);
	}	

	TableView* tvTemp = (TableView *)this->getChildByTag(9530)->getChildByTag(1111);
	if(tvTemp == nullptr) return;
	tvTemp->reloadData();	
}

void LotteryRecordGame::toSelect(Object *obj)
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
}
#include "LotteryRecordSubMember.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "BetLimit.h"
#include "LotterySubExchange.h"
#include "LotteryPE.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecordSubMember::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordSubMember *layer = LotteryRecordSubMember::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordSubMember::LotteryRecordSubMember()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubMember::updateView), "UpdateView", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubMember::refreshLayer), "Refresh_Data_Recon", NULL);
}

LotteryRecordSubMember::~LotteryRecordSubMember()
{
	clear();
}

bool LotteryRecordSubMember::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 6;
	this->initView();
	this->setKeypadEnabled(true);
	userId = EntityMgr::instance()->getDispatch()->m_dwUserID;
	return true;
}

void LotteryRecordSubMember::initView()
{
	setTitle(888);
    
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
		editBox->setAnchorPoint(Vec2(0,0.5f));
		editBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
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
    
    MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryRecordSubMember::pressConfirm,this));
	confirmItem->setPosition(Vec2(620, startY - gapY * 1));
    confirmItem->setScale(0.8);
    Menu *confirmMenu = Menu::create(confirmItem, NULL);
    confirmMenu->setPosition(Vec2(0,0));
    this->addChild(confirmMenu,1);
    
    Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
    gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
    gap1->setPosition(Vec2(0, startY-gapY*2));
    this->addChild(gap1,1);
    
	recordTable->setViewSize(cocos2d::Size(SCREEN_WIDTH, gap1->getPositionY() - 80));
    recordTable->reloadData();
    
    s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);
	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");

	EntityMgr::instance()->getDispatch()->SendPacketWithGetHYXXLogCount(true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
    
    
}

void LotteryRecordSubMember::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
}

cocos2d::Size LotteryRecordSubMember::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = recordTableSize.width;
	size.height = tableCellItemHeight*tableCellItemNum;
	return size;
}

TableViewCell* LotteryRecordSubMember::tableCellAtIndex(TableView *table, ssize_t idx)
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
	Dictionary *items = (Dictionary *)recordData->objectAtIndex(idx);
	Integer *n_t_id = (Integer *)items->objectForKey("n_t_id");
	String *s_t_account = (String *)items->objectForKey("s_t_account");
	String *time = (String *)items->objectForKey("time");
	Integer *n_t_type = (Integer *)items->objectForKey("n_t_type");
	Double *f_t_fandian = (Double *)items->objectForKey("f_t_fandian");
	Double *f_t_yue = (Double *)items->objectForKey("f_t_yue");
	Integer *n_t_online = (Integer *)items->objectForKey("n_t_online");
	String *s_t_qq = (String *)items->objectForKey("s_t_qq");
	//新增
	String *s_t_online = (String *)items->objectForKey("s_t_online");
	String *lasttime = (String *)items->objectForKey("lasttime");
	if(!n_t_id || n_t_id->getValue() == 0)
	{
		return cell;
	}
	for(int i = 0 ; i < 5 ; i++){
	
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 4585+i*2)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label->setColor(blackColor);
		cell->addChild(label, 1);

		String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 4586+i*2)->getCString()));
		LabelTTF *label1 = LabelTTF::create(labelStr1->getCString(), "", fontSize);
		label1->setAnchorPoint(Vec2(1,0.5f));
		label1->setPosition(Vec2(tableCellPosX2, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label1->setColor(blackColor);
		cell->addChild(label1, 1);
#ifdef VER_QIANBO
		if(i == 3) label1->setVisible(false);
		if(i == 4) label1->setPositionY(label1->getPositionY() + tableCellItemHeight);
#endif

#ifdef VER_369
		if(i == 3) label1->setVisible(false);
		if(i == 4) label1->setPositionY(label1->getPositionY() + tableCellItemHeight);
#endif
		
	}

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

	LabelTTF *timeLabel = LabelTTF::create(time->getCString(),
		"", fontSize);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	timeLabel->setColor(redColor);
	cell->addChild(timeLabel, 1);
	//
	String *n_t_typeStr = String::create(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 826+n_t_type->getValue())->getCString()));
	LabelTTF *n_t_typeLabel = LabelTTF::create(n_t_typeStr->getCString(),
		"", fontSize);
	n_t_typeLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_typeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	n_t_typeLabel->setColor(redColor);
	cell->addChild(n_t_typeLabel, 1);

	//返回点
	LabelTTF *f_t_fandianLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_fandian->getValue() * 100)->getCString(),
		"", fontSize);
	f_t_fandianLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_fandianLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*4));
	f_t_fandianLabel->setColor(redColor);
	cell->addChild(f_t_fandianLabel, 1);

	LabelTTF *f_t_yueLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_yue->getValue())->getCString(),"", fontSize);
	f_t_yueLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_yueLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	f_t_yueLabel->setColor(redColor);
	cell->addChild(f_t_yueLabel, 1);

	String *n_t_onlineStr = String::create(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 936+n_t_online->getValue())->getCString()));
	LabelTTF *n_t_onlineLabel = LabelTTF::create(n_t_onlineStr->getCString(),
		"", fontSize);
	n_t_onlineLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_onlineLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	n_t_onlineLabel->setColor(redColor);
	cell->addChild(n_t_onlineLabel, 1);

	LabelTTF *s_t_qqLabel = LabelTTF::create(s_t_qq->getCString(),
		"", fontSize);
	s_t_qqLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_qqLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	s_t_qqLabel->setColor(redColor);
	cell->addChild(s_t_qqLabel, 1);

#ifdef VER_QIANBO	
	f_t_fandianLabel->setPosition(s_t_qqLabel->getPosition());
	s_t_qqLabel->setVisible(false);
#endif

#ifdef VER_369	
	f_t_fandianLabel->setPosition(s_t_qqLabel->getPosition());
	s_t_qqLabel->setVisible(false);
#endif

	//在这里新增 在线人数 最后登陆时间
	LabelTTF *s_t_onlineLabel = LabelTTF::create(s_t_online->getCString(),
		"", fontSize);
	s_t_onlineLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_onlineLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*4));
	s_t_onlineLabel->setColor(redColor);
	cell->addChild(s_t_onlineLabel, 1);

	LabelTTF *lasttimeLabel = LabelTTF::create(lasttime->getCString(),
		"", fontSize);
	lasttimeLabel->setAnchorPoint(Vec2(0,0.5f));
	lasttimeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	lasttimeLabel->setColor(redColor);
	cell->addChild(lasttimeLabel, 1);

	//增加按钮
	float bottom_height = tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*5;
	int iItemSpace = 5;

	//按钮更多操作
	auto *btnMoreNormal = ui::Button::create("my_lottery_28.png","my_lottery_28.png");
	btnMoreNormal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnMoreNormal->setPosition(Vec2(iItemSpace, bottom_height));
	btnMoreNormal->setTag(1000);
	btnMoreNormal->setVisible(true);
	cell->addChild(btnMoreNormal, 1);

	auto *btnMorePress = ui::Button::create("my_lottery_28_2.png","my_lottery_28_2.png");
	btnMorePress->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnMorePress->setPosition(Vec2(iItemSpace, bottom_height));
	btnMorePress->setTag(1001);
	btnMorePress->setVisible(false);
	cell->addChild(btnMorePress, 1);

	auto nodeButton = Node::create();
	//nodeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);//无论设置什么，都是左中  child的middle对齐底？
	nodeButton->setPosition(Vec2(-SCREEN_WIDTH, bottom_height));	
	cell->addChild(nodeButton, 1);

	//----------------------------------------------------------------------------------------
	//修改配额 最右
	ui::Button *btnPeie = ui::Button::create("my_lottery_25.png", "my_lottery_25_2.png");
	btnPeie->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	iItemSpace += btnPeie->getContentSize().width;
	btnPeie->setPosition(Vec2(2 * iItemSpace, 0)); 
	nodeButton->addChild(btnPeie, 1);
	
	//下级转帐
	ui::Button *btnTransMoney = ui::Button::create("my_lottery_26.png","my_lottery_26_2.png");
	btnTransMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnTransMoney->setPosition(Vec2(iItemSpace, 0));
	nodeButton->addChild(btnTransMoney, 1);

	//修改返点
	ui::Button *btnFandian = ui::Button::create("my_lottery_27.png","my_lottery_27_2.png");
	btnFandian->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnFandian->setPosition(Vec2(0, 0));
	nodeButton->addChild(btnFandian, 1);	

	//------------------------------------------------------------------------------------------

	//结束
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	gap1->setPosition(Vec2(0, 0));
	
	cell->addChild(gap1,1);	

	//s_t_account->retain();
	//n_t_id->retain();

	//添加事件
	auto funMore = [=](Ref *sender)
	{
		auto btnMore = (ui::Button *)sender;
		//更多为空的时候
		if(btnMore == nullptr) return;

		if(btnMore->getTag() == 1000) //正常
		{		
			nodeButton->runAction(MoveTo::create(0.1f,Vec2(SCREEN_WIDTH - 3 * iItemSpace, nodeButton->getPositionY())));
			btnMoreNormal->setVisible(false);
			btnMorePress->setVisible(true);
		}else
		if(btnMore->getTag() == 1001) //展开
		{			
			nodeButton->runAction(MoveTo::create(0.1f,Vec2(-SCREEN_WIDTH, nodeButton->getPositionY())));
			btnMoreNormal->setVisible(true);
			btnMorePress->setVisible(false);
		}
	};
	//更多
	btnMoreNormal->addClickEventListener(funMore);
	btnMorePress->addClickEventListener(funMore);

	//下级反点
	btnFandian->addClickEventListener([=](Ref *){
		playButtonSound();	
		//分配反点
		LotteryPE *layer = LotteryPE::create();
		layer->setUserID(n_t_id->getValue());
		layer->setDisplayType(PE_fandian);
		this->addChild(layer, 200);

		Dictionary* data = new Dictionary();
		data->setObject(s_t_account, "s_t_account");
		data->setObject(f_t_fandian, "f_t_fandian");
		data->setObject(n_t_id, "n_t_id");	

		NotificationCenter::getInstance()->postNotification("XiaJiFanDian", data);
	});
		
	btnTransMoney->addClickEventListener([=](Ref* ){
		playButtonSound();
		//下级转帐
		Scene *scene = LotterySubExchange::scene(s_t_account->getCString(),n_t_id->getValue());
		Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));	

		clear();
	});	
	btnPeie->addClickEventListener([=](Ref* ){
		playButtonSound();	
		//分配配额
		LotteryPE *layer = LotteryPE::create();
		layer->setUserID(n_t_id->getValue());
		layer->setDisplayType(PE_peie);
		layer->setPeiE(f_t_fandian->getValue());
		this->addChild(layer, 200);

		EntityMgr::instance()->getDispatch()->SendPacketWithXiaJiPeiE();
	});

	//处理配额
	btnPeie->setEnabled(f_t_fandian->getValue() * 100 >= 2.8f);


	return cell;
}

ssize_t LotteryRecordSubMember::numberOfCellsInTableView(TableView *table)
{
	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordSubMember::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordSubMember::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordSubMember::refreshLayer(Object *obj)
{
	isQuerying = false;
	sendPacket();
}

void LotteryRecordSubMember::pressConfirm(Object *obj)
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
	//设置显示
	startDate->setText(s_t_start.c_str());
	endDate->setText(s_t_end.c_str());
	//设置查询值
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
			//
			resetInfo();
		}
	}

	currentPage = 1;

	sendPacket();
}

bool LotteryRecordSubMember::isValidUserID(const char *text)
{
	return true;
}

void LotteryRecordSubMember::sendPacket()
{
	if (checkIsQuerying()) return;LotteryRecord::sendPacket();
	EntityMgr::instance()->getDispatch()->SendPacketWithGetHYXXLog(currentPage, pageSize, true, s_t_start.c_str(),s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
}

void LotteryRecordSubMember::updateView(Object *obj)
{
	EntityMgr::instance()->getDispatch()->SendPacketWithGetHYXXLog(currentPage, pageSize, true, s_t_start.c_str(),s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
}

void LotteryRecordSubMember::clear()
{
	NotificationCenter::getInstance()->removeObserver(this,"UpdateView");
	NotificationCenter::getInstance()->removeObserver(this,"Refresh_Data_Recon");
}
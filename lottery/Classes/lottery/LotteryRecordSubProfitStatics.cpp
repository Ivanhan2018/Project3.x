#include "LotteryRecordSubProfitStatics.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecordSubProfitStatics::scene()
{
	Scene *scene = Scene::create();
	LotteryRecordSubProfitStatics *layer = LotteryRecordSubProfitStatics::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecordSubProfitStatics::LotteryRecordSubProfitStatics()
{
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubProfitStatics::getXJYKTjCountRet), "GetXJYKTjCountRet", NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubProfitStatics::getXJYKTjRet), "GetXJYKTjRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubProfitStatics::getParentRelativeRet), "GetParentRelativeRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecordSubProfitStatics::refreshLayer), "Refresh_Data_Recon", NULL);

}

LotteryRecordSubProfitStatics::~LotteryRecordSubProfitStatics()
{
    NotificationCenter::getInstance()->removeObserver(this, "GetXJYKTjCountRet");
    NotificationCenter::getInstance()->removeObserver(this, "GetXJYKTjRet");
	NotificationCenter::getInstance()->removeObserver(this, "GetParentRelativeRet");
	NotificationCenter::getInstance()->removeObserver(this, "Refresh_Data_Recon");
}

void LotteryRecordSubProfitStatics::getXJYKTjRet(Object *obj)
{
    Array *data = (Array *)obj;
    recordData->removeAllObjects();
    recordData->addObjectsFromArray(data);

	sortArray();

    recordTable->reloadData();
    this->refreshLogLabel();
    
    isQuerying = false;
}

void LotteryRecordSubProfitStatics::getParentRelativeRet(Object *obj)
{
	 Array *data = (Array *)obj;
	 if(data == nullptr) return;

	 //这里是上下级关系
	 int count = data->count();
	 //点击刷新
	 for(int i = 0;i < count;i ++)
	 {
		 auto btnTopNext = (ui::Button* )this->getChildByTag(8888 + i);
		 //数值
		 auto dic = (Dictionary *)data->objectAtIndex(count - i - 1);
		 String* tempStr = (String *)dic->objectForKey("s_t_desc");	
		 Integer* tempInt = (Integer *)dic->objectForKey("n_t_userid");
		 //没有下一个按钮，则退出
		 if(btnTopNext == nullptr)
		 {			 
			 auto btnTop = (ui::Button *)this->getChildByTag(8888 + i - 1);
			 if(btnTop == nullptr) return;
			 btnTopNext = (ui::Button *)btnTop->clone();
			 btnTopNext->setPosition(btnTop->getPosition() + Vec2(btnTop->getContentSize().width,0));
			 btnTopNext->setTag(btnTop->getTag() + 1);
			 this->addChild(btnTopNext);
		 }
		 btnTopNext->setTitleText(tempStr->getCString());
		 btnTopNext->setUserData((void *)tempInt->getValue());
	 }

	 //清除不需要的
	 for(int j = data->count();j < 10;j ++)
	 {
		 auto btnTopNext = (ui::Button* )this->getChildByTag(8888 + j);
		 //没有下一个按钮，则退出
		 if(btnTopNext == nullptr) break;
		 //有则删除
		 btnTopNext->removeFromParentAndCleanup(true);
	 }

}

void LotteryRecordSubProfitStatics::getXJYKTjCountRet(Object *obj)
{
    recordDataCount = ((Integer *)obj)->getValue();
    totalPage = recordDataCount%pageSize!=0?(recordDataCount/pageSize+1):recordDataCount/pageSize;
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


bool LotteryRecordSubProfitStatics::init()
{
	if (!LotteryRecord::init())
	{
		return false;
	}
	tableCellItemNum = 8;
	m_sort_index = 0;
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryRecordSubProfitStatics::initView()
{
    setTitle(893);
    
	float posX1 = 100;
	float posX2 = 150;
	float startY = SCREEN_HEIGHT - 160;
	float gapY = 60;
	float fontSize = 26;
	ccColor3B blackColor = ccc3(76, 76, 76);

	//顶端新加导航
	auto btnTop = ui::Button::create("my_lottery_38.png","my_lottery_38.png","my_lottery_38.png");
	btnTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	btnTop->setPosition(Vec2(0,startY));
	this->addChild(btnTop); 
	btnTop->setTag(8888);
	btnTop->setTitleAlignment(TextHAlignment::LEFT);
	btnTop->setTitleFontName("");
	btnTop->setTitleFontSize(fontSize);
	btnTop->setTitleColor(blackColor);
	btnTop->setTitleText(EntityMgr::instance()->getDispatch()->m_pAccounts);
	
	//添加点击事件
	btnTop->addClickEventListener([=](Ref *obj){
		playButtonSound();

		auto btnTop = static_cast<ui::Button *>(obj);
		if(btnTop == nullptr) return;
		//
		int tagIndex = btnTop->getTag() + 1;

		//重新查询
		userID = (long)btnTop->getUserData();		
		queryDataByUserName(btnTop->getTitleText().c_str());
	});

	startY -= 100;

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
    
    MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryRecordSubProfitStatics::pressConfirm,this));
	confirmItem->setPosition(Vec2(620, startY - gapY * 1));
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
	MenuItemSprite* moreItem = MenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(LotteryRecordSubProfitStatics::toSelect,this));
	moreItem->setPosition(Vec2(SCREEN_WIDTH - normalSprite->getContentSize().width / 2, startY));
	moreItem->setScale(0.8f);
	moreItem->setTag(9528);
	Menu *selectMenu = Menu::create(moreItem,NULL);
	selectMenu->setPosition(Vec2::ZERO);
	selectMenu->setTag(9529);
	this->addChild(selectMenu,2);

	//筛选条件
	LabelTTF* ltfSort = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t2030"), "", fontSize-4);
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
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
    gap1->setPosition(Vec2(0, startY-gapY*2));
    
    this->addChild(gap1,1);
    
	recordTable->setViewSize(cocos2d::Size(SCREEN_WIDTH, gap1->getPositionY() - 80));
    recordTable->reloadData();    
    
    /*******************************/	
    s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);

	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");

	EntityMgr::instance()->getDispatch()->SendPacketWithGetXJYKTjCount(true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, s_t_account.c_str());
}

void LotteryRecordSubProfitStatics::tableCellTouched(TableView* table, TableViewCell* cell)
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
			ltfDisplay->setString(ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d",2030 + m_sort_index)->getCString()));
		}
	}
}

cocos2d::Size LotteryRecordSubProfitStatics::cellSizeForTable(TableView *table)
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

TableViewCell* LotteryRecordSubProfitStatics::tableCellAtIndex(TableView *table, ssize_t idx)
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
	if(tag == 1111){	

		LabelTTF *label = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d",2030 + idx)->getCString()), "", 22);
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

	for(int i = 0 ; i < 7 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 2001+i*2)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(tableCellPosX1, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*i));
		label->setColor(blackColor);
		cell->addChild(label, 1);

		String *labelStr1 = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 2002+i*2)->getCString()));
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
	Integer *n_t_permillage = (Integer *)items->objectForKey("n_t_permillage");
    Double *f_t_chongzhi_ze = (Double *)items->objectForKey("f_t_chongzhi_ze");
    Double *f_t_qukuan_ze = (Double *)items->objectForKey("f_t_qukuan_ze");
    Double *f_t_touzhu_ze = (Double *)items->objectForKey("f_t_touzhu_ze");
    Double *f_t_zhongjiang_ze = (Double *)items->objectForKey("f_t_zhongjiang_ze");
    Double *f_t_fandian_ze = (Double *)items->objectForKey("f_t_fandian_ze");
    Double *f_t_xjfandian_ze = (Double *)items->objectForKey("f_t_xjfandian_ze");
    Double *f_t_huodong_ze = (Double *)items->objectForKey("f_t_huodong_ze");
    Double *f_t_tuandui_ye = (Double *)items->objectForKey("f_t_tuandui_ye");
	Double * f_t_yingkui_ze = (Double *)items->objectForKey("f_t_yingkui_ye");
	//新加
	Double * f_t_qipai_yk = (Double *)items->objectForKey("f_t_qipai_yk");
	Double * f_t_qipai_fd = (Double *)items->objectForKey("f_t_qipai_fd");

	float shouxufei = n_t_permillage->getValue() * f_t_chongzhi_ze->getValue() / 1000.0f;	
	//综合盈亏
	Double *f_t_zonghe_yk = Double::create(f_t_qipai_yk->getValue() + f_t_yingkui_ze->getValue() + shouxufei);

	//用户ID
	LabelTTF *n_t_idLabel = LabelTTF::create(String::createWithFormat("%d",n_t_id->getValue())->getCString(),
		"", fontSize);
	n_t_idLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_idLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	n_t_idLabel->setColor(redColor);
	cell->addChild(n_t_idLabel, 1);

	//帐号
	LabelTTF *s_t_accountLabel = LabelTTF::create(s_t_account->getCString(),"", fontSize);
	s_t_accountLabel->setAnchorPoint(Vec2(0,0.5f));
	s_t_accountLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*0));
	s_t_accountLabel->setColor(redColor);
	cell->addChild(s_t_accountLabel, 1);
    
	//会员和代理
    String *n_t_typeStr = String::create(ConfigMgr::instance()->text("display_text.xml",
            String::createWithFormat("t%d", 826+n_t_type->getValue())->getCString()));
    LabelTTF *n_t_typeLabel = LabelTTF::create(n_t_typeStr->getCString(),
                                                   "", fontSize);
	n_t_typeLabel->setAnchorPoint(Vec2(0,0.5f));
	n_t_typeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	n_t_typeLabel->setColor(redColor);
	cell->addChild(n_t_typeLabel, 1);

	//充值总额  团队充值
	LabelTTF *f_t_chongzhi_zeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_chongzhi_ze->getValue())->getCString(),"", fontSize);
	f_t_chongzhi_zeLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_chongzhi_zeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*1));
	f_t_chongzhi_zeLabel->setColor(redColor);
	cell->addChild(f_t_chongzhi_zeLabel, 1);

	//取款总额
	LabelTTF *f_t_qukuan_zeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_qukuan_ze->getValue())->getCString(),"", fontSize);
	f_t_qukuan_zeLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_qukuan_zeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	f_t_qukuan_zeLabel->setColor(redColor);
	cell->addChild(f_t_qukuan_zeLabel, 1);

	//投注总额
	LabelTTF *f_t_touzhu_zeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_touzhu_ze->getValue())->getCString(),"", fontSize);
	f_t_touzhu_zeLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_touzhu_zeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*2));
	f_t_touzhu_zeLabel->setColor(redColor);
	cell->addChild(f_t_touzhu_zeLabel, 1);

	//中奖去掉
	//LabelTTF *f_t_zhongjiang_zeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_zhongjiang_ze->getValue())->getCString(),"", fontSize);
	//f_t_zhongjiang_zeLabel->setAnchorPoint(Vec2(0,0.5f));
	//f_t_zhongjiang_zeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
	//f_t_zhongjiang_zeLabel->setColor(redColor);
 //   cell->addChild(f_t_zhongjiang_zeLabel, 1);
    
	//下级返点 + 代理返点 团队彩票返点
	LabelTTF *f_t_fandian_zeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_xjfandian_ze->getValue())->getCString(), "", fontSize);
    f_t_fandian_zeLabel->setAnchorPoint(Vec2(0,0.5f));
    f_t_fandian_zeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
    f_t_fandian_zeLabel->setColor(redColor);
    cell->addChild(f_t_fandian_zeLabel, 1);
    
	//团队彩票盈亏总和
    LabelTTF *f_t_yingkui_zeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_yingkui_ze->getValue() + shouxufei)->getCString(),"", fontSize);
    f_t_yingkui_zeLabel->setAnchorPoint(Vec2(0,0.5f));
    f_t_yingkui_zeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*3));
    f_t_yingkui_zeLabel->setColor(redColor);
    cell->addChild(f_t_yingkui_zeLabel, 1);

	//团队棋牌盈亏
	LabelTTF *f_t_qipai_ykLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_qipai_yk->getValue())->getCString(),"", fontSize);
	f_t_qipai_ykLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_qipai_ykLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*4));
	f_t_qipai_ykLabel->setColor(redColor);
	cell->addChild(f_t_qipai_ykLabel, 1);

	//团队棋牌返点
	LabelTTF *f_t_qipai_fdLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_qipai_fd->getValue())->getCString(),"", fontSize);
	f_t_qipai_fdLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_qipai_fdLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*4));
	f_t_qipai_fdLabel->setColor(redColor);
	cell->addChild(f_t_qipai_fdLabel, 1);
    
	//活动总额
    LabelTTF *f_t_huodong_zeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_huodong_ze->getValue())->getCString(),"", fontSize);
    f_t_huodong_zeLabel->setAnchorPoint(Vec2(0,0.5f));
    f_t_huodong_zeLabel->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*5));
    f_t_huodong_zeLabel->setColor(redColor);
    cell->addChild(f_t_huodong_zeLabel, 1);

	//综合盈亏
	LabelTTF *f_t_zonghe_ykLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_zonghe_yk->getValue())->getCString(),"", fontSize);
	f_t_zonghe_ykLabel->setAnchorPoint(Vec2(0,0.5f));
	f_t_zonghe_ykLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*5));
	f_t_zonghe_ykLabel->setColor(redColor);
	cell->addChild(f_t_zonghe_ykLabel, 1);
	
	//最后加充值手续费
	LabelTTF *f_chongzhi_lv = LabelTTF::create(String::createWithFormat("%.3lf", shouxufei)->getCString(),"", fontSize);
	f_chongzhi_lv->setAnchorPoint(Vec2(0,0.5f));
	f_chongzhi_lv->setPosition(Vec2(tableCellPosX1+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*6));
	f_chongzhi_lv->setColor(redColor);
	cell->addChild(f_chongzhi_lv, 1);

	//团队余额
    LabelTTF *f_t_tuandui_yeLabel = LabelTTF::create(String::createWithFormat("%.3lf", f_t_tuandui_ye->getValue())->getCString(),"", fontSize);
    f_t_tuandui_yeLabel->setAnchorPoint(Vec2(0,0.5f));
    f_t_tuandui_yeLabel->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*6));
    f_t_tuandui_yeLabel->setColor(redColor);
    cell->addChild(f_t_tuandui_yeLabel, 1);	

	//新加button
	ui::Button* btnSub = ui::Button::create("my_lottery_xj.png","my_lottery_xj_2.png","my_lottery_xj.png");
	btnSub->setAnchorPoint(Vec2(0,0.5f));
	btnSub->setPosition(Vec2(tableCellPosX2+tableCellGapX, tableCellItemHeight*tableCellItemNum-tableCellItemHeight/2-tableCellItemHeight*7));
	cell->addChild(btnSub, 1);

	//重新查询
	btnSub->addClickEventListener([=](Ref *){

		playButtonSound();

		//重新查询
		userID = n_t_id->getValue();
		auto accStr = String::create(s_t_account->getCString());
		accStr->retain();
		queryDataByUserName(accStr->getCString());	
		accStr->release();
	});

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	gap1->setPosition(Vec2(0, 0));
	
	cell->addChild(gap1,1);
	return cell;
}

ssize_t LotteryRecordSubProfitStatics::numberOfCellsInTableView(TableView *table)
{
	//下拉列表
	int tag = dynamic_cast<Node*>(table)->getTag();
	if(tag == 1111){
		return 10;
	}

	if(recordData->count() == 0){
		return 1;
	}
	return recordData->count();
}

void LotteryRecordSubProfitStatics::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecordSubProfitStatics::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryRecordSubProfitStatics::pressConfirm(Object *obj)
{
	playButtonSound();

	EditBox *accEditBox = (EditBox *)getChildByTag(EDITBOX_TAG);
	const char *accStr = accEditBox->getText();
	
	queryDataByUserName(accStr);
}

void LotteryRecordSubProfitStatics::queryDataByUserName(const char* userName)
{	
	EditBox *idEditBox = (EditBox *)getChildByTag(EDITBOX_TAG+1);
	EditBox *startDate = (EditBox *)getChildByTag(EDITBOX_TAG + 2);
	EditBox *endDate = (EditBox *)getChildByTag(EDITBOX_TAG + 3);

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

	if (strcmp(userName, "") != 0) {
		n_t_type = 2;
		s_t_account = BetLimit::GetStrWithoutSpace(userName);
	}else{
		if(strcmp(idStr, "") != 0)
		{
			n_t_type = 1;
			userID = atoi(idStr);
		}else{
			resetInfo();
		}
	}

	EntityMgr::instance()->getDispatch()->SendPacketWithGetXJYKTjCount(true, s_t_start.c_str(), s_t_end.c_str(), n_t_type, userID, userName);
}

void LotteryRecordSubProfitStatics::sendPacket()
{
	if (checkIsQuerying()) return;LotteryRecord::sendPacket();
    EntityMgr::instance()->getDispatch()->SendPacketWithGetXJYKTj(currentPage, pageSize, true, s_t_start.c_str(),s_t_end.c_str(), n_t_type, userID, s_t_account.c_str(),m_sort_index);
	EntityMgr::instance()->getDispatch()->SendPacketWithGetParentRelative(userID);
}

void LotteryRecordSubProfitStatics::refreshLayer(Object *obj)
{
	isQuerying = false;
	sendPacket();
}

void LotteryRecordSubProfitStatics::toSelect(Object *obj)
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

void LotteryRecordSubProfitStatics::sortArray()
{
	return;
	//m_sort_index对应列表 recordData
	int count = recordData->count();

	//比较字段
	String* compStr = String::create("");
	bool isAsc = false;

	for(int i = 0;i < count;i ++)
	{
		Dictionary *dici = (Dictionary *)recordData->objectAtIndex(i);
		Double* compValuei = (Double *)dici->objectForKey(compStr->getCString());

		for(int j = i + 1;j < count - i;j ++)
		{
			Dictionary *dicj = (Dictionary *)recordData->objectAtIndex(j);
			Double* compValuej = (Double *)dicj->objectForKey(compStr->getCString());

			if(isAsc)
			{
				if(compValuei < compValuej)
				{
					recordData->swap(i,j);
				}
			}else
			{
				if(compValuei > compValuej)
				{
					recordData->swap(i,j);
				}
			}
		}
	}
	

}
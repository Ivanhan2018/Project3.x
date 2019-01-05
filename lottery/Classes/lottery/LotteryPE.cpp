#include "LotteryPE.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "LotterySettingView.h"
#include "MyNSString.h"
#include "VersionControl.h"

int covert2Int(const double number)
{
	const char* tempStr = String::createWithFormat("%.0lf", number)->getCString();
	return atoi(tempStr);
}

LotteryPE::LotteryPE()
{
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryPE::xiaJiPeiERet), "XiaJiPeiERet", NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryPE::xiaJiPeiESetRet), "XiaJiPeiESetRet", NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryPE::xiaJiFanDian), "XiaJiFanDian", NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LotteryPE::xiaJiFanDianRet), "XiaJiFanDianRet", NULL);
}
LotteryPE::~LotteryPE()
{
	NotificationCenter::getInstance()->removeObserver(this,"XiaJiPeiERet");
	NotificationCenter::getInstance()->removeObserver(this,"XiaJiPeiESetRet");
	NotificationCenter::getInstance()->removeObserver(this,"XiaJiFanDian");
	NotificationCenter::getInstance()->removeObserver(this,"XiaJiFanDianRet");
}

bool LotteryPE::init()
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

void LotteryPE::initData()
{
	m_userID = 0;
	m_userName = NULL;
	m_fanDian = NULL;
	m_listFanDian = NULL;

	m_userPeie = 0;
	
	for(int i = 0;i < 5;i ++) m_peie[i] = 0;
}

void LotteryPE::initView()
{
	//初始化界面
	LayerColor* layer = LayerColor::create(ccc4(200, 200, 200, 200), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setOpacity(125);
	layer->setPosition(Vec2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2));
	this->addChild(layer);
	
	//禁掉点击事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){return true;};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//禁掉返回键
	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){
		if(keycode == EventKeyboard::KeyCode::KEY_BACK)
		{
			event->stopPropagation();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, this);

	//背景图
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame("dt_tz_bs04.png"));
	bk1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bk1->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50));
	bk1->setTag(9527);
	this->addChild(bk1,1);

	float fontSize = 37;
	ccColor3B blackColor = ccc3(76,76,76);

	//分配配额提示
	LabelTTF* ltfTitle = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml","t80"), "", fontSize);
	ltfTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfTitle->setPosition(Vec2(bk1->getContentSize().width / 2,bk1->getContentSize().height - 42));
	ltfTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfTitle->setFontFillColor(ccc3(255,255,255));
	ltfTitle->setTag(8888);
	bk1->addChild(ltfTitle,1);

	Sprite *confirmNormal = Sprite::createWithSpriteFrame(spriteFrame("beting22.png"));
	Sprite *confirmSelect = Sprite::createWithSpriteFrame(spriteFrame("beting23.png"));
	Sprite *cancelNormal = Sprite::createWithSpriteFrame(spriteFrame("beting24.png"));
	Sprite *cancelSelect = Sprite::createWithSpriteFrame(spriteFrame("beting25.png"));
	//确认
	MenuItemSprite *pConfirm = MenuItemSprite::create(confirmNormal,confirmSelect,CC_CALLBACK_1(LotteryPE::confirm,this));
	//取消
	MenuItemSprite *pCancel = MenuItemSprite::create(cancelNormal, cancelSelect,CC_CALLBACK_1(LotteryPE::back,this));
	pCancel->setTag(1000);

	Menu *pCloseButton = Menu::create(pConfirm,pCancel,NULL);
	pCloseButton->alignItemsHorizontallyWithPadding(80);
	pCloseButton->setTag(2000);
	pCloseButton->setPosition(Vec2(bk1->getContentSize().width / 2, 70));
	bk1->addChild(pCloseButton,1);
}

void LotteryPE::confirm(Object *obj)
{
	playButtonSound();

	//禁用按钮
	MenuItemSprite* btnConfirm = (MenuItemSprite *)obj;
	if(btnConfirm == nullptr) return;
	//禁用按钮
	btnConfirm->setEnabled(false);

	if(m_typeID == PE_fandian)
	{
		//EDITBOX_TAG_PE - 1
		EditBox* box = (EditBox *)this->getChildByTag(9527)->getChildByTag(EDITBOX_TAG_PE - 1);
		if (box == nullptr || m_userID == 0) return;
		double fandian = atof(box->getText()) / 100;
		//发送修改
		EntityMgr::instance()->getDispatch()->SendPacketSetXiaJiFanDian(m_userID,fandian);
		
	} else
	if(m_typeID == PE_peie)
	{		
		//初始化
		int curPeie[] = {0,0,0,0,0};
		int fandian = covert2Int(EntityMgr::instance()->getDispatch()->f_fandian * 1000); //返点

		//背景
		Sprite* background = (Sprite *)this->getChildByTag(9527);
		if(background == nullptr || m_userID == 0) return;	

		int tempCount = 0;
		for(int i = 0;i < 5;i ++)
		{
			//输入框
			EditBox *dtPeiE = (EditBox *)background->getChildByTag(EDITBOX_TAG_PE + i);  // 30,29,28,27,26
			if(dtPeiE)
			{
				int peiE = atoi(dtPeiE->getText());
				//计算和
				tempCount += peiE;
				if(peiE == 0) continue;  //分配为0忽略

				if(peiE > m_peie[i] || (m_userPeie <= 30 - i))
				{
					if(m_userPeie == 28 && i == 2 && peiE <= m_peie[i]) //情况比较特殊
					{
						curPeie[i] = peiE;
						continue;
					}
					//清0
					dtPeiE->setText("0");
					btnConfirm->setEnabled(true);
					return;
				} 

				//给值
				curPeie[i] = peiE;
			}			
		}	
	
		if (tempCount == 0) 
		{
			btnConfirm->setEnabled(true);
			return;
		}

		EntityMgr::instance()->getDispatch()->SendPacketSetXiaJiPeiE(m_userID,curPeie[0],curPeie[1],curPeie[2],curPeie[3],curPeie[4]);
	
	}
	
}

void LotteryPE::back(Object *obj)
{
	playButtonSound();	
	this->removeFromParent();
}

void LotteryPE::xiaJiPeiERet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;
	if(data == nullptr) return;	

	Integer* total30 = (Integer *)data->objectForKey("total30");
	Integer* total28 = (Integer *)data->objectForKey("total28");
	Integer* total27 = (Integer *)data->objectForKey("total27");
	Integer* pere30 = (Integer *)data->objectForKey("pere30");
	Integer* pere28 = (Integer *)data->objectForKey("pere28");
	Integer* pere27 = (Integer *)data->objectForKey("pere27");

	m_peie[0] = total30->getValue() - pere30->getValue();
	m_peie[2] = total28->getValue() - pere28->getValue();
	m_peie[3] = total27->getValue() - pere27->getValue();

	//总共显示个数
	m_count = 0;	
	int fandian = covert2Int(EntityMgr::instance()->getDispatch()->f_fandian * 1000);

#ifdef VER_QIANBO
	Integer* total29 = (Integer *)data->objectForKey("total29");
	Integer* pere29 = (Integer *)data->objectForKey("pere29");
	Integer* total26 = (Integer *)data->objectForKey("total26");
	Integer* pere26 = (Integer *)data->objectForKey("pere26");

	m_peie[0] = total30->getValue() - pere30->getValue();
	m_peie[1] = total29->getValue() - pere29->getValue();
	m_peie[2] = total28->getValue() - pere28->getValue();
	m_peie[3] = total27->getValue() - pere27->getValue();
	m_peie[4] = total26->getValue() - pere26->getValue();

	if(fandian  > 30)
	{
		m_count = 5;
	}else
		if(fandian == 30)
		{
			m_count = 4;
		}
		else
			if(fandian >= 28)
			{
				m_count = 3;
			}
#endif

#ifdef VER_369
	Integer* total29 = (Integer *)data->objectForKey("total29");
	Integer* pere29 = (Integer *)data->objectForKey("pere29");
	Integer* total26 = (Integer *)data->objectForKey("total26");
	Integer* pere26 = (Integer *)data->objectForKey("pere26");

	m_peie[0] = total30->getValue() - pere30->getValue();
	m_peie[1] = total29->getValue() - pere29->getValue();
	m_peie[2] = total28->getValue() - pere28->getValue();
	m_peie[3] = total27->getValue() - pere27->getValue();
	m_peie[4] = total26->getValue() - pere26->getValue();

	if(fandian  > 30)
	{
		m_count = 5;
	}else
		if(fandian == 30)
		{
			m_count = 4;
		}
		else
			if(fandian >= 28)
			{
				m_count = 3;
			}
#endif

#ifdef VER_9YI
	if(fandian >= 30)
	{
		m_count = 3;
	}else
	if(fandian >= 28)
	{
		m_count = 2;
	}else
	if(fandian >= 27)
	{
		m_count = 1;
	}
#endif // VER_QIANBO

	float fontSize = 28;
	ccColor3B blackColor = ccc3(76,76,76);

	//获得图片的位置和id
	Sprite* background = (Sprite *)this->getChildByTag(9527);
	if(background == nullptr) return;

	//创建地址
	for(int i = 0;i < m_count;i ++)
	{
		int tempId = 81 + 3 - m_count;
		int idelta = 0;
		int tagId = 5 - m_count + i;		
		if(tagId < 0) tagId = 0;
#ifdef VER_QIANBO
		tempId = 100 + 5 - m_count;		
		if(EntityMgr::instance()->getDispatch()->f_fandian * 1000 > 28)
		{			
			idelta = 80;
		}
#endif

#ifdef VER_369
		tempId = 100 + 5 - m_count;		
		if(EntityMgr::instance()->getDispatch()->f_fandian * 1000 > 28)
		{			
			idelta = 80;
		}
#endif

#ifdef VER_9YI
		if(tagId > 0) tagId ++;
#endif
		//内容
		LabelTTF* ltfContent = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",tempId + i)->getCString()), "", fontSize);
		ltfContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		ltfContent->setFontFillColor(blackColor);
		ltfContent->setPosition(Vec2(80,background->getContentSize().height / 2 + 60 + idelta - 60 * i));
		ltfContent->setHorizontalAlignment(TextHAlignment::LEFT);
		background->addChild(ltfContent, 1);

		Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
		inputBk->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		inputBk->setScaleX(100 / inputBk->getContentSize().width);
		inputBk->setPosition(Vec2(ltfContent->getPositionX() + ltfContent->getContentSize().width + 8,ltfContent->getPositionY()));
		background->addChild(inputBk);

		//配额个数
		EditBox *editBox = EditBox::create(cocos2d::Size(100, 30), "blank.png");
		editBox->setPosition(inputBk->getPosition());
		editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		editBox->setInputMode(EditBox::InputMode::NUMERIC);
		editBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		editBox->setFontColor(blackColor);
		editBox->setFontName("");
		editBox->setFontSize(fontSize);
		editBox->setColor(ccc3(112,112,112));
		editBox->setMaxLength(20);
		editBox->setTag(EDITBOX_TAG_PE + tagId);
		background->addChild(editBox, 1);	

		//后面内容
		String* PELeft = String::createWithFormat("%s %d",ConfigMgr::instance()->text("display_DuangDong.xml","t90"), m_peie[tagId]);
		LabelTTF* ltfTail = LabelTTF::create(PELeft->getCString(), "", fontSize);
		ltfTail->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		ltfTail->setFontFillColor(blackColor);
		ltfTail->setPosition(Vec2(editBox->getPositionX() + editBox->getContentSize().width + 8,ltfContent->getPositionY()));
		ltfTail->setHorizontalAlignment(TextHAlignment::LEFT);
		background->addChild(ltfTail, 1);
	}	
}

void LotteryPE::xiaJiPeiESetRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;
	if(data == nullptr) return;

	Integer* lResult = (Integer *)data->objectForKey("n_t_result");
	String* str_desc = (String *)data->objectForKey("s_t_Desc");

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str_desc->getCString(), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2));
	this->getParent()->addChild(layer,255);

	this->removeFromParent();
}

void LotteryPE::xiaJiFanDian(Object *obj)
{
	//数据处理
	Dictionary* data = (Dictionary *)obj;
	//空指针退出
	if(data == nullptr) return;
	m_userName = (String *)data->objectForKey("s_t_account");
	m_fanDian = (Double *)data->objectForKey("f_t_fandian");
	Integer* n_t_id = (Integer *)data->objectForKey("n_t_id");	

	//初始化变量
	float fontSize = 28;
	ccColor3B blackColor = ccc3(76,76,76);

	//获得图片的位置和id
	Sprite* background = (Sprite *)this->getChildByTag(9527);
	if(background == nullptr) return;

	//创建地址
	for(int i = 0;i < 3;i ++)
	{
		//内容
		LabelTTF* ltfContent = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml",String::createWithFormat("t%d",92 + i)->getCString()), "", fontSize);
		ltfContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		ltfContent->setFontFillColor(blackColor);
		ltfContent->setPosition(Vec2(SCREEN_WIDTH / 2,background->getContentSize().height / 2 + 60 - 60 * i));
		ltfContent->setHorizontalAlignment(TextHAlignment::RIGHT);
		background->addChild(ltfContent, 1);

		if(i < 2)
		{
			//后面内容
			String* PELeft = m_userName; //用户名
			if(i == 1)//返点
			{
				PELeft = String::createWithFormat("%.2lf", m_fanDian->getValue() * 100);				
			}
			LabelTTF* ltfTail = LabelTTF::create(PELeft->getCString(), "", fontSize);
			ltfTail->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			ltfTail->setFontFillColor(blackColor);
			ltfTail->setPosition(Vec2(SCREEN_WIDTH / 2 + 8,ltfContent->getPositionY()));
			ltfTail->setHorizontalAlignment(TextHAlignment::LEFT);
			background->addChild(ltfTail, 1);
		} else
		{
			Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
			inputBk->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			inputBk->setScaleX(120 / inputBk->getContentSize().width);
			inputBk->setPosition(Vec2(SCREEN_WIDTH / 2 + 8,ltfContent->getPositionY()));
			background->addChild(inputBk);

			//配额个数
			EditBox *editBox = EditBox::create(cocos2d::Size(120, 30), "blank.png");
			editBox->setPosition(inputBk->getPosition());
			editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
			editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
			editBox->setInputMode(EditBox::InputMode::NUMERIC);
			editBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			editBox->setFontColor(blackColor);
			editBox->setText("0.0");
			editBox->setFontName("");
			editBox->setFontSize(fontSize);
			editBox->setColor(ccc3(112,112,112));
			editBox->setMaxLength(20);
			editBox->setEnabled(false); //禁用
			editBox->setTag(EDITBOX_TAG_PE - 1);
			background->addChild(editBox, 1);

			//按钮	
			ui::Button* btnPop = ui::Button::create("beting17.png", "beting18.png");
			btnPop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			btnPop->setScale(0.8f);
			btnPop->setPosition(Vec2(editBox->getPositionX() + editBox->getContentSize().width, editBox->getPositionY()));
			background->addChild(btnPop, 1);

			//表格
			auto m_TableNode = Node::create();
			m_TableNode->setPosition(Vec2(2000,editBox->getPositionY() - 320));	
			m_TableNode->setTag(EDITBOX_TAG_PE - 2);
			background->addChild(m_TableNode, 1);
	
			auto fandianTableBk = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
			fandianTableBk->setAnchorPoint(Vec2(0,0));
			fandianTableBk->setScaleX(100/fandianTableBk->getContentSize().width);
			fandianTableBk->setScaleY(300/fandianTableBk->getContentSize().height);
			fandianTableBk->setPosition(Vec2::ZERO);
			m_TableNode->addChild(fandianTableBk,1);		

			//下拉菜单的弹出位置
			auto fandianTable = TableView::create(this, cocos2d::Size(100,300));
			fandianTable->setDelegate(this);
			fandianTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			fandianTable->setPosition(Vec2::ZERO);
			fandianTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
			fandianTable->setDirection(ScrollView::Direction::VERTICAL);			
			m_TableNode->addChild(fandianTable, 2);				

			//刷新数据
			fandianTable->reloadData();

			btnPop->addClickEventListener([=](Ref *){
				auto myMenu = (Menu *)this->getChildByTag(9527)->getChildByTag(2000);
				//取消按钮
				if(myMenu == nullptr) return;	

				if(m_TableNode->getPositionX() == 2000)
				{				
					myMenu->setEnabled(false);
					m_TableNode->setPositionX(editBox->getPositionX());

				} else
				{	
					myMenu->setEnabled(true);
					m_TableNode->setPositionX(2000);
				}
			}
			);
		}		
	}

	//暂时不考虑配额的情况
	//EntityMgr::instance()->getDispatch()->SendPacketWithXiaJiPeiE(n_t_id->getValue());
}

void LotteryPE::xiaJiFanDianRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;
	if(data == nullptr) return;

	Integer* lResult = (Integer *)data->objectForKey("n_t_result");
	Double* fandian = (Double *)data->objectForKey("f_fandian");
	String* str_desc = (String *)data->objectForKey("s_t_Account");

	String* result = NULL;

	if(lResult->getValue() == 0)
	{
		result = String::createWithFormat("%s %s %s %.02lf",
			ConfigMgr::instance()->text("display_DuangDong.xml","t1500"),
			str_desc->getCString(),
			ConfigMgr::instance()->text("display_DuangDong.xml","t1501"),
			fandian->getValue() * 100
			);
	}else
	if(lResult->getValue() == 1)
	{
		result = String::createWithFormat("%s",
			ConfigMgr::instance()->text("display_DuangDong.xml","t1502"));
	}else
	{
		result = String::createWithFormat("%s %s %s",
			ConfigMgr::instance()->text("display_DuangDong.xml","t1503"),
			str_desc->getCString(),
			ConfigMgr::instance()->text("display_DuangDong.xml","t1504")
			);
	}

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(result->getCString(), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2));
	this->getParent()->addChild(layer,255);

	this->removeFromParent();

	NotificationCenter::getInstance()->postNotification("UpdateView");
}

//设置userid
void LotteryPE::setUserID(const int userID)
{
	this->m_userID = userID;
}

void LotteryPE::setPeiE(const double userPeie)
{	
	this->m_userPeie = covert2Int(userPeie * 1000);
}

//设置显示类型 界面上的数据都是调用接口后返回的 这里不做界面元素添加
void LotteryPE::setDisplayType(const int typeID)
{
	this->m_typeID = typeID;

	//标题
	Sprite* background = (Sprite *)this->getChildByTag(9527);
	if(background == nullptr) return;
	LabelTTF* ltfTitle = (LabelTTF *) background->getChildByTag(8888);
	if(ltfTitle == nullptr) return;	

	switch (m_typeID)
	{
	case PE_peie:

#ifdef VER_QIANBO
		//换背景图
		if(EntityMgr::instance()->getDispatch()->f_fandian * 1000 > 28)
		{
			background->setSpriteFrame(spriteFrame("dt_tz_bs04_2.png"));
		}
#endif

#ifdef VER_369
		//换背景图
		if(EntityMgr::instance()->getDispatch()->f_fandian * 1000 > 28)
		{
			background->setSpriteFrame(spriteFrame("dt_tz_bs04_2.png"));
		}
#endif

		ltfTitle->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t80"));	
		ltfTitle->setPosition(Vec2(background->getContentSize().width / 2,background->getContentSize().height - 42));		
			
		break;
	case PE_fandian:
		ltfTitle->setString(ConfigMgr::instance()->text("display_DuangDong.xml","t91"));
		break;
	default:
		break;
	}
	
}

void LotteryPE::tableCellTouched(TableView* table, TableViewCell* cell)
{	
	//表格
	auto m_TableNode = table->getParent();
	if(m_TableNode == nullptr) return;
	auto background = (Sprite *)m_TableNode->getParent();
	if(background == nullptr) return;
	auto editBox = (EditBox *)background->getChildByTag(EDITBOX_TAG_PE - 1);
	if(editBox == nullptr) return;

	auto myMenu = (Menu *)this->getChildByTag(9527)->getChildByTag(2000);
	//取消按钮
	if(myMenu == nullptr) return;	

	//有值的时候
	if(m_TableNode->getPositionX() == 2000)
	{
		myMenu->setEnabled(false);
		m_TableNode->setPositionX(editBox->getPositionX());
	} else
	{
		myMenu->setEnabled(true);
		m_TableNode->setPositionX(2000);
	}

	int cellTag = cell->getTag();
	float value = 0.1f*cellTag;
	char valueStr[10] = {0};
	sprintf(valueStr, "%.1f", value);

	editBox->setText(valueStr);
}

cocos2d::Size LotteryPE::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(100, 30);
}

TableViewCell* LotteryPE::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();
	cell->autorelease();
	cell->setTag(idx);

	float value = 0.0;
	value+=0.1f*idx;

	char str[4] = {0};
	sprintf(str, "%.1f", value);

	float fontSize = 27;
	ccColor3B blackColor = ccc3(76,76,76);
	LabelTTF *label = LabelTTF::create(str, "", fontSize);
	label->setAnchorPoint(Vec2(0,0.5f));
	label->setPosition(Vec2(10, 15));
	label->setColor(blackColor);
	cell->addChild(label, 1);

	return cell;
}

ssize_t LotteryPE::numberOfCellsInTableView(TableView *table)
{
	int result = int(EntityMgr::instance()->getDispatch()->f_fandian * 1000);
	if (result == 28) return 29;
	return result;
}
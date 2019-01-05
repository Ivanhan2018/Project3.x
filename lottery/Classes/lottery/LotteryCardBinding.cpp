#include "LotteryCardBinding.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "LotteryRechargeDetail.h"
#include "LotteryMyLot.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "MyNSString.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryCardBinding::scene()
{
	Scene *scene = Scene::create();
	LotteryCardBinding *layer = LotteryCardBinding::create();
	scene->addChild(layer);
	return scene;
}

LotteryCardBinding::LotteryCardBinding()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryCardBinding::setQukuanZhanghaoRet), "SetQukuanZhanghaoRet", NULL);	
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryCardBinding::queryMyYinHangRet), "QueryMyYinHangRet", NULL);	
	//获取银行信息
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryCardBinding::queryBankTypeRet), "QueryBankTypeRet", NULL);
}
LotteryCardBinding::~LotteryCardBinding()
{
	NotificationCenter::getInstance()->removeObserver(this, "SetQukuanZhanghaoRet");
	NotificationCenter::getInstance()->removeObserver(this, "QueryMyYinHangRet");
	NotificationCenter::getInstance()->removeObserver(this,"QueryBankTypeRet");
}

void LotteryCardBinding::queryMyYinHangRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	int lResult = ((Integer *)dic->objectForKey("lResult"))->getValue();
	String *szKaihuYinghang = (String *)dic->objectForKey("szKaihuYinghang");
	String *szKaihuRen = (String *)dic->objectForKey("szKaihuRen");
	String *szYinhangZhanghu = (String *)dic->objectForKey("szYinhangZhanghu");
	int m_lock = ((Integer *)dic->objectForKey("m_lock"))->getValue();  //是否锁定 0 解锁 ,1 锁定
	if(strcmp(szKaihuYinghang->getCString(), "") != 0){
		//开户银行
		string tempStr = szKaihuYinghang->getCString();		
		bankNameInput->setString(tempStr.c_str());
		//开户人		
		bankUserNameInput->setText(BetLimit::GetNameWithXing(szKaihuRen->getCString()).c_str());
		//银行帐号		
		bankAccountInput->setText(BetLimit::GetBankCardWithXing(szYinhangZhanghu->getCString()).c_str());
				
		bankUserNameInput->setEnabled(false);
		bankAccountInput->setEnabled(false);
		passwordInput->setEnabled(false);
		passwordInput->setVisible(false);
		bangDingbutton->setVisible(false);
		nameInputBk->setVisible(false);
		nameLabel->setVisible(false);

		//不显示银行选择按钮
		btnMenu->setVisible(false);

		if(!m_lock)
		{
			//开户人		
			bankUserNameInput->setText(szKaihuRen->getCString());
			//银行帐号		
			bankAccountInput->setText(szYinhangZhanghu->getCString());
			bankUserNameInput->setEnabled(false);
			btnMenu->setVisible(true);			
			bankAccountInput->setEnabled(true);
			passwordInput->setEnabled(true);
			passwordInput->setVisible(true);
			bangDingbutton->setVisible(true);
			nameInputBk->setVisible(true);
			nameLabel->setVisible(true);
		}
	} else
	{
		btnMenu->setVisible(true);
	}

	
}

//查询结果返回
void LotteryCardBinding::queryBankTypeRet(Object *obj)
{	
	Array *data = (Array *)obj;
	int count = data->count();
	for (int i = 0; i < count; i ++){
		Dictionary *dic = (Dictionary *)data->objectAtIndex(i);		
		String *str = (String *)dic->objectForKey("bankType");
		bankNameVec.push_back(str->getCString());
		}
	m_Table->reloadData();
}

void LotteryCardBinding::setQukuanZhanghaoRet(Object *obj)
{
	int lResult = ((Integer *)obj)->getValue();
	if(lResult<0||lResult>3){
		lResult = 3;
	}
	String *str = String::createWithFormat("t%d", 629+lResult);
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
		ConfigMgr::instance()->text("display_text.xml", str->getCString()),Vec2(winSize.width * 0.5,winSize.height * 0.5));
	this->addChild(layer, 2);
}

bool LotteryCardBinding::init()
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

void LotteryCardBinding::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	posX1 = 250;
	posX2 = 280;
	gapY = 60;
}

void LotteryCardBinding::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//±≥æ∞
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	//返回
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryCardBinding::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//银行卡管理文字
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t1113"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 27;
	m_node = Node::create();
	this->addChild(m_node, 1);

	//温馨提示
	LabelTTF *title1 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t618"), "", fontSize);
	title1->setAnchorPoint(Vec2(0,0.5f));
	title1->setPosition(Vec2(10, SCREEN_HEIGHT - 220));
	title1->setColor(ccc3(171,71,71));
	title1->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_node->addChild(title1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, title1->getPositionY()-60));
	m_node->addChild(gap1);

	startY = gap1->getPositionY()-50;

	fontSize = 27;
	for(int i = 0 ; i < 6 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 619+i)->getCString()));
		if(i == 5){
			nameLabel = LabelTTF::create(labelStr->getCString(), "", fontSize);
			nameLabel->setAnchorPoint(Vec2(1,0.5f));
			nameLabel->setPosition(Vec2(posX1, startY-gapY*i));
			nameLabel->setColor(blackColor);
			m_node->addChild(nameLabel, 1);
			continue;
		}
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX1, startY-gapY*i));
		label->setColor(blackColor);
		m_node->addChild(label, 1);
	}

	//值的颜色
#ifdef VER_369
	blackColor = ccc3(50, 62, 123);
#endif

	fontSize = 30;
	LabelTTF *userNameLabel = LabelTTF::create(username.c_str(), "", fontSize);
	userNameLabel->setAnchorPoint(Vec2(0,0.5f));
	userNameLabel->setPosition(Vec2(posX2, startY));
	userNameLabel->setColor(blackColor);
	m_node->addChild(userNameLabel);

	String *userIDStr = String::createWithFormat("%ld", EntityMgr::instance()->getDispatch()->m_dwUserID);
	LabelTTF *userIDLabel = LabelTTF::create(userIDStr->getCString(), "", fontSize);
	userIDLabel->setAnchorPoint(Vec2(0,0.5f));
	userIDLabel->setPosition(Vec2(posX2, startY-gapY));
	userIDLabel->setColor(blackColor);
	m_node->addChild(userIDLabel);

	for(int i = 2 ; i < 6 ; i++){
		if(i == 5){
			nameInputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
			nameInputBk->setAnchorPoint(Vec2(0,0.5f));
			nameInputBk->setPosition(Vec2(posX2-5, startY-gapY*i));
			m_node->addChild(nameInputBk);
			continue;
		}
		Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
		inputBk->setAnchorPoint(Vec2(0,0.5f));
		inputBk->setPosition(Vec2(posX2-5, startY-gapY*i));
		m_node->addChild(inputBk);
	}

	fontSize = 25;
	int iTextWidth = 300;
	std::string tempBankName = "";
	MyNSString::GBKToUTF8(tempBankName);
	bankNameInput = LabelTTF::create();
	bankNameInput->setString(tempBankName.c_str());
	bankNameInput->setDimensions(cocos2d::Size(iTextWidth, 35)); 		
	bankNameInput->setFontSize(fontSize-2);
	bankNameInput->setColor(blackColor);	
	bankNameInput->setPosition(Vec2(posX2, startY-gapY*2));
	bankNameInput->setAnchorPoint(Vec2(0,0.5f));
	this->addChild(bankNameInput, 3);	

	//银行
	m_TableNode = Node::create();
    m_TableNode->setPosition(1000,1000);
    this->addChild(m_TableNode, 3);
	//白色背景 带边框
    m_TableBk = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
    m_TableBk->setAnchorPoint(Vec2(0, 0));
    m_TableBk->setPosition(Vec2::ZERO);
	m_TableBk->setScaleX(nameInputBk->getContentSize().width / m_TableBk->getContentSize().width);
    m_TableNode->addChild(m_TableBk,3);	

	//下拉按钮
	Sprite *btnBankSel = Sprite::createWithSpriteFrame(spriteFrame("beting17.png"));
	Sprite *btnBankSelDown = Sprite::createWithSpriteFrame(spriteFrame("beting18.png"));
	MenuItemSprite *menuBankName = MenuItemSprite::create(btnBankSel,btnBankSelDown,CC_CALLBACK_1(LotteryCardBinding::selectBank,this));	
	btnMenu = Menu::create(menuBankName,NULL);
	btnMenu->setPosition(Vec2(posX2 + nameInputBk->getContentSize().width - btnBankSel->getContentSize().width / 2, startY-gapY*2));
	btnMenu->setAnchorPoint(Vec2(0,0.5f));	
	this->addChild(btnMenu,4);

	btnMenu->setVisible(false);
   
	//银行列表 最多显示5行
	m_Table = TableView::create(this,cocos2d::Size(nameInputBk->getContentSize().width,250));
    m_Table->setDelegate(this);
    m_Table->setAnchorPoint(Vec2(0, 0));
    m_Table->setPosition(Vec2::ZERO);
    m_Table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_Table->setDirection(ScrollView::Direction::VERTICAL);
    m_Table->retain();
	m_Table->setTag(1111);
    m_TableNode->addChild(m_Table, 3);

	bankUserNameInput = EditBox::create(cocos2d::Size(iTextWidth, 35), "blank.png");
	bankUserNameInput->setPosition(Vec2(posX2, startY-gapY*3));
	bankUserNameInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	bankUserNameInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	bankUserNameInput->setAnchorPoint(Vec2(0,0.5f));
	bankUserNameInput->setFontColor(blackColor);
	bankUserNameInput->setFontName("");bankUserNameInput->setFontSize(fontSize);
	bankUserNameInput->setColor(ccc3(112,112,112));
	bankUserNameInput->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t626"));
	bankUserNameInput->setPlaceholderFontSize(25);
	bankUserNameInput->setMaxLength(20);
	this->addChild(bankUserNameInput, 1);

	bankAccountInput = EditBox::create(cocos2d::Size(iTextWidth, 35), "blank.png");
	bankAccountInput->setPosition(Vec2(posX2, startY-gapY*4));
	bankAccountInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	bankAccountInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	bankAccountInput->setAnchorPoint(Vec2(0,0.5f));
	bankAccountInput->setFontColor(blackColor);
	bankAccountInput->setFontName("");bankAccountInput->setFontSize(fontSize);
	bankAccountInput->setColor(ccc3(112,112,112));
	bankAccountInput->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t627"));
	bankAccountInput->setPlaceholderFontSize(25);
	bankAccountInput->setMaxLength(21);
	this->addChild(bankAccountInput, 1);

	passwordInput = EditBox::create(cocos2d::Size(iTextWidth, 35), "blank.png");
	passwordInput->setPosition(Vec2(posX2, startY-gapY*5));
	passwordInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	passwordInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	passwordInput->setInputMode(EditBox::InputMode::ANY);
	passwordInput->setAnchorPoint(Vec2(0,0.5f));
	passwordInput->setFontColor(blackColor);
	passwordInput->setFontName("");passwordInput->setFontSize(fontSize);
	passwordInput->setColor(ccc3(112,112,112));
	passwordInput->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t628"));
	passwordInput->setPlaceholderFontSize(25);
	passwordInput->setMaxLength(10);
	this->addChild(passwordInput, 1);

	Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap2->setAnchorPoint(Vec2(0,0.5));
	gap2->setPosition(Vec2(0, startY-gapY*7));
	m_node->addChild(gap2);

	//立即绑定
	Sprite *normalButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_11.png"));
	Sprite *selectButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_12.png"));
	MenuItemSprite *itemSprite = MenuItemSprite::create
		(normalButton,selectButton,CC_CALLBACK_1(LotteryCardBinding::bangDing,this));
	bangDingbutton = Menu::create(itemSprite,NULL);
	bangDingbutton->setPosition(Vec2(winSize.width / 2, startY - gapY * 7-100));
	m_node->addChild(bangDingbutton);

	EntityMgr::instance()->getDispatch()->QueryMyYinHang();
	EntityMgr::instance()->getDispatch()->QueryYinHang();
}

void LotteryCardBinding::bangDing(Object *obj)
{
	playButtonSound();

	char* bankAccount = const_cast<char*>( bankAccountInput->getText());
	char c = 0; 
	char szchinese[3] = {0}; 
	int i = 0, nlen = strlen(bankAccount); 

	string username = bankUserNameInput->getText();
	//用户名检测
	if(username.size() < 1 || BetLimit::ValidateStr(username.c_str()))
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t997"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		return;
	}
	//int pos1 = username.find_last_not_of(" ") + 1;	
	//if(pos1 < 6)
	//{
	//	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t997"),
	//		Vec2(winSize.width * 0.5,winSize.height * 0.5));
	//	this->addChild(layer, 255);
	//	return;
	//}
	//username.erase(pos1);//去掉空格
	//银行卡号检测
	if(nlen < 1)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t996"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		return;
	}

	for(; i < nlen; i++) 
	{ 
		if( bankAccount[i] >= 0 && bankAccount[i] <= 127 ) //不是全角字符
		{
			c = bankAccount[i];
			printf("%c\n", c);
			if(bankAccount[i] == 32)//空格
			{
				bankAccount[i] = '\0';
			}
		}else {//是全角字符(中文)
			//银行卡是中文...文字
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t11"),
		Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		return;
		}
	}
	EntityMgr::instance()->getDispatch()->SendPacketWithSetQukuanZhanghao(bankNameInput->getString(), 
		username,bankAccount,passwordInput->getText());
}

void LotteryCardBinding::back(Object *obj)
{
	playButtonSound();
	//Scene *scene = LotteryMyLot::scene();
	Director::getInstance()->popScene();
}

void LotteryCardBinding::selectBank(Object *obj)
{	
	playButtonSound();

	static bool isShowMenu = false;
	if (isShowMenu) 
	{     
		isShowMenu = !isShowMenu;
		m_TableNode->setPosition(Vec2(1000,1000));
		//激活
		//bankUserNameInput->setEnabled(true);
		bankUserNameInput->setVisible(true);
		bankAccountInput->setVisible(true);
		passwordInput->setVisible(true);
        return;
    }

	isShowMenu = !isShowMenu;
	m_TableNode->setPosition(bankNameInput->getPosition() - Vec2(5,250 + 30));
	//使editbox不能获得焦点 避免选择银行的时候乱入  之前是用setenable
	bankUserNameInput->setVisible(false);
	bankAccountInput->setVisible(false);
	passwordInput->setVisible(false);
}

void LotteryCardBinding::onEnter()
{
	Layer::onEnter();
}

void LotteryCardBinding::onExit()
{
	Layer::onExit();
}

void LotteryCardBinding::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryCardBinding::tableCellTouched(TableView* table, TableViewCell* cell)
{
	LabelTTF *label = (LabelTTF *)cell->getChildByTag(1100);

	//银行名称
	std::string tempBankName = label->getString();

	bankNameInput->setString(tempBankName.c_str());

	selectBank(btnMenu);

	return;
}

cocos2d::Size LotteryCardBinding::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = bankNameInput->getContentSize().width;
	size.height = 50;
	return size;
}

TableViewCell* LotteryCardBinding::tableCellAtIndex(TableView *table, ssize_t index)
{
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease();
	
	if (0 == bankNameVec.size()) return cell;

   //银行名称
	std::string tempBankName = bankNameVec.at(index);

	LabelTTF *label = LabelTTF::create(tempBankName.c_str(), "", 22);
	label->setTag(1100);
    label->setAnchorPoint(Vec2(0,0.5f));
    label->setPosition(Vec2(10, 50 / 2));
    label->setColor(ccc3(0,0,0));
    cell->addChild(label, 3);
	return cell;  
}

ssize_t LotteryCardBinding::numberOfCellsInTableView(TableView *table)
{
	if ( 0 == bankNameVec.size())
	{
		return 1;
	}
	return bankNameVec.size();
}

void LotteryCardBinding::scrollViewDidScroll(ScrollView* view)
{

}


void LotteryCardBinding::scrollViewDidZoom(ScrollView* view)
{

}
#include "LotteryMemberShip.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "LotteryRechargeDetail.h"
#include "LotteryMyLot.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryMemberShip::scene()
{
	Scene *scene = Scene::create();
	LotteryMemberShip *layer = LotteryMemberShip::create();
	scene->addChild(layer);
	return scene;
}

LotteryMemberShip::LotteryMemberShip()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMemberShip::getHyShjRet), "GetHyShjRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMemberShip::refreshLayer), "Refresh_Data_Recon", NULL);
}
LotteryMemberShip::~LotteryMemberShip()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetHyShjRet");
	NotificationCenter::getInstance()->removeObserver(this, "Refresh_Data_Recon");	

	for(int i = 0;i < recordData.size();i ++)    
	{
		auto tempObj = recordData.at(i);
		tempObj->release();
	}
	recordData.clear();
}

void LotteryMemberShip::getHyShjRet(Object *obj)
{
		if(obj == NULL) return;	

	Dictionary *dic = (Dictionary *)obj;
    Integer *n_t_xjrs = static_cast<Integer *>(dic->objectForKey("n_t_xjrs"));
    Integer *n_t_xjzx = static_cast<Integer *>(dic->objectForKey("n_t_xjzx"));
	Integer *n_t_qianfenlv = static_cast<Integer *>(dic->objectForKey("n_t_qianfenlv")); //新加
    Double *f_t_hyye = static_cast<Double *>(dic->objectForKey("f_t_hyye"));
    Double *f_t_jrfd = static_cast<Double *>(dic->objectForKey("f_t_jrfd"));
    Double *f_t_jrchz = static_cast<Double *>(dic->objectForKey("f_t_jrchz"));
    Double *f_t_jrqk = static_cast<Double *>(dic->objectForKey("f_t_jrqk")); //今日取款
    Double *f_t_jrtzh = static_cast<Double *>(dic->objectForKey("f_t_jrtzh"));//今日投注

	Integer *n_t_jinrizhuce = static_cast<Integer *>(dic->objectForKey("n_t_jinrizhuce"));
	Double *f_chedan_zonge = static_cast<Double *>(dic->objectForKey("f_chedan_zonge"));
	Double *f_zhongjiang_zonge = static_cast<Double *>(dic->objectForKey("f_zhongjiang_zonge"));
	Double *f_wodefandian = static_cast<Double *>(dic->objectForKey("f_wodefandian"));
	Double *f_caipiao_yingkui_pr = static_cast<Double *>(dic->objectForKey("f_caipiao_yingkui"));
	Double *f_t_xianma = static_cast<Double *>(dic->objectForKey("f_t_xianma"));
	Double *f_qp_fandian = static_cast<Double *>(dic->objectForKey("f_qp_fandian"));
	Double *f_qp_yingkui = static_cast<Double *>(dic->objectForKey("f_qp_yingkui"));
	Double *f_my_qp_fandian = static_cast<Double *>(dic->objectForKey("f_my_qp_fandian"));
	Double *f_touzhuyongjin = static_cast<Double *>(dic->objectForKey("f_touzhuyongjin"));
	Double *f_kuisunyongjin = static_cast<Double *>(dic->objectForKey("f_kuisunyongjin"));

	Integer *n_t_newchongzhi = static_cast<Integer *>(dic->objectForKey("n_t_newchongzhi"));
	Double *f_t_userfandian = static_cast<Double *>(dic->objectForKey("f_t_userfandian"));

	n_t_xjrs->retain();
	n_t_xjzx->retain();
	n_t_qianfenlv->retain();
	f_t_hyye->retain();
	f_t_jrfd->retain();
	f_t_jrchz->retain();
	f_t_jrqk->retain();
	f_t_jrtzh->retain();
	n_t_jinrizhuce->retain();
	f_chedan_zonge->retain();
	f_zhongjiang_zonge->retain();
	f_wodefandian->retain();
	
	f_t_xianma->retain();
	f_qp_fandian->retain();
	f_qp_yingkui->retain();
	f_my_qp_fandian->retain();
	//f_touzhuyongjin->retain();
	//f_kuisunyongjin->retain();
	n_t_newchongzhi->retain();
	f_t_userfandian->retain();


	Double* sxf = Double::create(n_t_qianfenlv->getValue() * f_t_jrchz->getValue() / 1000.0f);	//手续费
	sxf->retain();
	Double* f_caipiao_yingkui = Double::create(f_caipiao_yingkui_pr->getValue() + sxf->getValue());  //彩票盈亏要加上手续费
	f_caipiao_yingkui->retain();
	Double* zhyk = Double::create(f_caipiao_yingkui->getValue() + f_qp_yingkui->getValue());	//综合盈亏
	zhyk->retain();
	Double* yjze = Double::create(f_touzhuyongjin->getValue() + f_kuisunyongjin->getValue());	//
	yjze->retain();

	recordData.clear();
	//重新添加
	//综合
	recordData.push_back(f_t_hyye);	//团队余额
	recordData.push_back(n_t_xjrs);	//团队人数
	recordData.push_back(n_t_jinrizhuce);	//今天注册
	recordData.push_back(n_t_newchongzhi); //今天充值人数
	recordData.push_back(n_t_xjzx);	//在线人数
	recordData.push_back(f_t_jrchz);	//团队充值
	recordData.push_back(f_t_jrqk);	//团队取款
	recordData.push_back(zhyk);//综合盈亏
	recordData.push_back(yjze);//佣金总额
	recordData.push_back(sxf);	//充值手续费	
	//彩票
	recordData.push_back(f_t_jrtzh);	//投注总额
	recordData.push_back(f_zhongjiang_zonge);	//中奖总额
	recordData.push_back(f_chedan_zonge);	//撤单总额
	recordData.push_back(f_t_jrfd);	//团队返点		
	recordData.push_back(f_wodefandian);	//团队活动
	recordData.push_back(f_caipiao_yingkui);	//彩票盈亏
	//棋牌
	recordData.push_back(f_t_xianma);	//棋牌投注
	recordData.push_back(f_qp_yingkui);	//棋牌盈亏
	recordData.push_back(f_qp_fandian);	//棋牌返点	
	recordData.push_back(f_my_qp_fandian);	//我的返点

	refreshValue();
}

void LotteryMemberShip::refreshValue()
{
	for(int idx = 0;idx < MAX_SIZE;idx ++)
	{	
		auto label = (LabelTTF *)this->getChildByTag(8888 + idx);
		if(label == nullptr) continue;
		//显示值
		String* valueStr;
		if (idx == 1 || idx == 2 ||idx == 3 || idx == 4) 
		{
			Integer *temp = static_cast<Integer *>(recordData.at(idx));		
			valueStr = String::createWithFormat("%d",temp->getValue());
		} else
		{
			Double *temp = static_cast<Double *>(recordData.at(idx));
			double tempDouble = temp->getValue();
			if(idx == 10 && tempDouble < 0) tempDouble = -tempDouble;
			valueStr = String::createWithFormat("%.3f", tempDouble);
		}

		//加值
		label->setString(valueStr->getCString());
	}
}

bool LotteryMemberShip::init()
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

void LotteryMemberShip::pressConfirm(Object *obj)
{
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
	s_t_start = BetLimit::AjustDateStr(startDate->getText());
	s_t_end = BetLimit::AjustDateStr(endDate->getText());

	startDate->setText(s_t_start.c_str());
	endDate->setText(s_t_end.c_str());

	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");

	sendPacket();
}

void LotteryMemberShip::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;	
	s_t_start = "";
	s_t_end = "";
}

void LotteryMemberShip::initView()
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

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryMemberShip::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
    ccColor3B blackColor = ccc3(76,76,76);
    ccColor3B redColor = ccc3(235,44,49);
	ccColor3B wordColor = ccc3(221,141,48);

#ifdef VER_369
	wordColor = ccc3(50, 62, 123);
#endif

	//会员数据
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t892"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 27;	

	//位置
	float posX11 = 100;
	float posX21 = 150;
	float startY1 = SCREEN_HEIGHT - 240;
	float gapY1 = 60;

	time_t ct;
	ct = time(NULL);
	tm *st = localtime(&ct);
	const char* todayStr = String::createWithFormat("%04d-%02d-%02d",st->tm_year + 1900,st->tm_mon + 1,st->tm_mday)->getCString();

	for(int i = 0 ; i < 2 ; i++)
	{
		int deltaLen = i * 220;

		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 990+i)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX11+ 40 + deltaLen , startY1-gapY1*(i / 2) + 10));
		label->setColor(blackColor);
		this->addChild(label, 1);

		Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("beting26.png"));
		inputBk->setAnchorPoint(Vec2(0,0.5f));
		inputBk->setScale(1.2f);
		inputBk->setScaleX(1.4f);
		inputBk->setPosition(Vec2(posX21 + 10 + deltaLen, label->getPositionY()));
		this->addChild(inputBk);

		EditBox *editBox = EditBox::create(cocos2d::Size(inputBk->getContentSize().width * 1.4f, 35), "blank.png");
		editBox->setPosition(inputBk->getPosition());
		editBox->setInputFlag(EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		editBox->setReturnType(EditBox::KeyboardReturnType::DONE);
		editBox->setAnchorPoint(Vec2(0,0.5f));
		editBox->setFontColor(ccc3(0,0,0));
		editBox->setFontName("");editBox->setFontSize(fontSize);
		editBox->setColor(ccc3(112,112,112));
		editBox->setInputMode(EditBox::InputMode::NUMERIC);
		editBox->setMaxLength(10);
		editBox->setText(todayStr);
		editBox->setTag(EDITBOX_TAG+i);
		this->addChild(editBox, 1);
	}

	MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", this, menu_selector(LotteryMemberShip::pressConfirm));
	confirmItem->setPosition(Vec2(620, startY1 + 10));
	confirmItem->setScale(0.8f);
	Menu *confirmMenu = Menu::create(confirmItem, NULL);
	confirmMenu->setPosition(Vec2(0,0));
	this->addChild(confirmMenu,1);	

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, startY1-45));
	this->addChild(gap1, 1);

	startY = gap1->getPositionY()-25; 

	float posX1 = 34;
	float posX2 = SCREEN_WIDTH / 2;	
	float gapY = 60;
	int tempI = 0;
	//X值
	float posX = posX1;

	for(int idx = 0;idx < MAX_SIZE;idx ++)
	{
		if(idx == 0 || idx == 10 || idx == 16)
		{
			startY -= 10;

			//标题
			String *labelTitleStr = String::create(ConfigMgr::instance()->text("display_text.xml",
				String::createWithFormat("t%d", 5060+tempI)->getCString()));
			LabelTTF *labelTitle = LabelTTF::create(labelTitleStr->getCString(), "", fontSize);
			labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			labelTitle->setHorizontalAlignment(TextHAlignment::LEFT);
			labelTitle->setPosition(Vec2(posX1,startY));
			labelTitle->setColor(redColor);
			labelTitle->setFontName("");
			labelTitle->setFontSize(28);
			this->addChild(labelTitle, 1);	

			tempI ++;			
			startY -= 10;
			startY -= gapY;
		}
		//标题
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 1000+idx)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		label->setHorizontalAlignment(TextHAlignment::LEFT);
		label->setPosition(Vec2(posX,startY));
		label->setColor(blackColor);
		label->setFontName("");
		label->setFontSize(24);
		this->addChild(label, 1);		

		//值
		LabelTTF *label1 = LabelTTF::create("0", "", fontSize);
		label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		label1->setHorizontalAlignment(TextHAlignment::LEFT);
		label1->setPosition(Vec2(posX + label->getContentSize().width, startY));
		label1->setColor(wordColor);
		label1->setFontName("");
		label1->setFontSize(24);
		label1->setTag(8888 + idx);
		this->addChild(label1, 1);

		if(idx == 3) 
		{
			label1->setPositionX(posX + 140);
		}
		
		if(idx % 2 == 1)
		{		
			startY -= gapY;
			posX = posX1;
		}else
		{
			posX = posX2;
		}
	}	

	s_t_start = BetLimit::AjustDateStr(todayStr);
	s_t_end = BetLimit::AjustDateStr(todayStr);

	s_t_start.append(" 00:00:00.000");
	s_t_end.append(" 23:59:59.999");

	sendPacket();
}

void LotteryMemberShip::back(Object *obj)
{
	playButtonSound();
	Scene *scene = LotteryMyLot::scene();
	Director::getInstance()->popScene();
}

void LotteryMemberShip::onEnter()
{
	Layer::onEnter();
}

void LotteryMemberShip::onExit()
{
	Layer::onExit();
}

void LotteryMemberShip::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}


void LotteryMemberShip::sendPacket()
{	
	EntityMgr::instance()->getDispatch()->SendPacketWithGetHyShj(s_t_start.c_str(),s_t_end.c_str(),false);
}

void LotteryMemberShip::refreshLayer(Object *obj)
{
	sendPacket();	
}
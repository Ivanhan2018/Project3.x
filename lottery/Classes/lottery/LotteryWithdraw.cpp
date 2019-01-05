#include "LotteryWithdraw.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "LotteryRechargeDetail.h"
#include "LotteryMyLot.h"
#include "EntityMgr.h"
#include "LotteryCardBinding.h"
#include "LotterySceneControl.h"
#include "MyNSString.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryWithdraw::scene()
{
	Scene *scene = Scene::create();
	LotteryWithdraw *layer = LotteryWithdraw::create();
	scene->addChild(layer);
	return scene;
}

LotteryWithdraw::LotteryWithdraw()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryWithdraw::queryMyYinHangRet), "QueryMyYinHangRet", NULL);	
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryWithdraw::doQukuanRet), "DoQukuanRet", NULL);	
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryWithdraw::getLastYueRet), "GetLastYueRet", NULL);  ///余额查询
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryWithdraw::getQuKuanLimitRet), "GetQuKuanLimitRet", NULL);//取款限制
}
LotteryWithdraw::~LotteryWithdraw()
{
	clear();
}

void LotteryWithdraw::doQukuanRet(Object *obj)
{
	Dictionary* dic = (Dictionary*)obj;
	if (dic == nullptr) return;
	String *str = (String *)dic->objectForKey("szDesc");//String::createWithFormat("t%d", 644+lResult);
	Integer* I_Result = (Integer *)dic->objectForKey("nResult");
	//刷新余额
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
	EntityMgr::instance()->getDispatch()->sendPacketWithGetQuKuanLimit();

	EditBox *dtMoney = (EditBox *)this->getChildByTag(114);
	EditBox *dtQuKuan = (EditBox *)this->getChildByTag(115);
	EditBox *dtjs1 = (EditBox *)this->getChildByTag(116);
	EditBox *dtjs2 = (EditBox *)this->getChildByTag(117);
	EditBox *dtPassword = (EditBox *) this->getChildByTag(138);
	if(dtMoney && dtPassword && dtQuKuan && dtjs1 && dtjs2)
	{		
		if(I_Result->getValue() == 0)
		{
			float money = atof(dtQuKuan->getText());
			EntityMgr::instance()->getDispatch()->f_yue -= money;
			dtMoney->setText(String::createWithFormat("%.3f", EntityMgr::instance()->getDispatch()->f_yue)->getCString());
		}

		dtQuKuan->setEnabled(true);
		dtPassword->setEnabled(true);	
		dtQuKuan->setText("");
		dtPassword->setText("");
		dtjs1->setText("");
		dtjs2->setText("");
	}

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str->getCString(),Vec2(winSize.width * 0.5,winSize.height * 0.5));
	this->addChild(layer, 255);	
}

void LotteryWithdraw::getLastYueRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	Double* f_t_yue = (Double *)dic->objectForKey("f_t_yue"); 

	double money = f_t_yue->getValue();	

	//帐户余额
	EditBox *dtMoney = (EditBox *)this->getChildByTag(114);
	if(dtMoney)
	{
		dtMoney->setText(String::createWithFormat("%.3f", money)->getCString());
	}	
}

void LotteryWithdraw::getQuKuanLimitRet(Object *obj)
{
	//取款限制
	Dictionary *dic = (Dictionary *)obj;

	if(n_t_sxfpercent) n_t_sxfpercent->release();
	if(f_t_lowsxf) f_t_lowsxf->release();
	if(f_t_highsxf) f_t_highsxf->release();
	if(n_t_freelastcount) n_t_freelastcount->release();
	if(f_t_lowjine) f_t_lowjine->release();
	if(f_t_highjine) f_t_highjine->release();

	Double *f_t_nowxiaofei= (Double *)dic->objectForKey("f_t_nowxiaofei");
	Double *f_t_xiaofei= (Double *)dic->objectForKey("f_t_xiaofei");
	f_t_lowjine= (Double *)dic->objectForKey("f_t_lowjine");
	f_t_highjine= (Double *)dic->objectForKey("f_t_highjine");
	n_t_sxfpercent = (Integer *)dic->objectForKey( "n_t_sxfpercent");
	f_t_lowsxf= (Double *)dic->objectForKey("f_t_lowsxf");
	f_t_highsxf= (Double *)dic->objectForKey("f_t_highsxf");
	Integer *n_t_limitcount= (Integer *)dic->objectForKey( "n_t_limitcount");
	Integer *n_t_lastcount= (Integer *)dic->objectForKey( "n_t_lastcount");
	n_t_freelastcount= (Integer *)dic->objectForKey( "n_t_freelastcount");
	Integer *n_t_allcount= (Integer *)dic->objectForKey( "n_t_allcount");
	String *s_t_remark = (String *)dic->objectForKey( "s_t_remark");
	Integer *t_starttime= (Integer *)dic->objectForKey( "t_starttime");
	Integer *t_endtime= (Integer *)dic->objectForKey( "t_endtime");

	n_t_sxfpercent->retain();
	f_t_lowsxf->retain();
	f_t_highsxf->retain();
	n_t_freelastcount->retain();
	f_t_lowjine->retain();
	f_t_highjine->retain();

	//更新界面说明
	//底部说明
	LabelTTF *ltfTips = (LabelTTF *)this->getChildByTag(9527);
	if(ltfTips)
	{
		//std::string tempStr = s_t_remark->getCString();
		//BetLimit::SpanStringByLen(tempStr, 2000);
		//ltfTips->setString(tempStr);
	}
	//帐户余额 彩票所需消耗量
	LabelTTF *ltfYuE = (LabelTTF *)this->getChildByTag(1114);
	if(ltfYuE)
	{
		ltfYuE->setString(
			String::createWithFormat("%s%.0f%s",
			ConfigMgr::instance()->text("display_text.xml","t4530"), f_t_xiaofei->getValue(),
			ConfigMgr::instance()->text("display_text.xml","t78")
			)->getCString()
			);
	}
	//取款金额
	LabelTTF *ltfQuKuan = (LabelTTF *)this->getChildByTag(1115);
	if(ltfQuKuan)
	{
		ltfQuKuan->setString(String::createWithFormat("%s%.0f%s%.0f%s",
			ConfigMgr::instance()->text("display_text.xml","t4531"), 
			f_t_lowjine->getValue(), 
			ConfigMgr::instance()->text("display_text.xml","t4570"), 
			f_t_highjine->getValue(),
			ConfigMgr::instance()->text("display_text.xml","t4571") 
			)->getCString());		
	}
	//手续费
	LabelTTF *ltfShou = (LabelTTF *)this->getChildByTag(1116);
	if(ltfShou)
	{
		ltfShou->setString(String::createWithFormat("%s%d%s%.0f%s%.0f%s",
			ConfigMgr::instance()->text("display_text.xml","t4532"),
			n_t_sxfpercent->getValue(), 
			ConfigMgr::instance()->text("display_text.xml","t4572"),
			f_t_lowsxf->getValue(), 
			ConfigMgr::instance()->text("display_text.xml","t4573"),
			f_t_highsxf->getValue(),
			ConfigMgr::instance()->text("display_text.xml","t4571")
			)->getCString());
	}
	//实际到帐
	LabelTTF *ltfShiji = (LabelTTF *)this->getChildByTag(1117);
	if(ltfShiji)
	{
		ltfShiji->setString(String::createWithFormat("%s%d%s",
			ConfigMgr::instance()->text("display_text.xml","t4533"),
			n_t_freelastcount->getValue(),
			ConfigMgr::instance()->text("display_text.xml","t4574")
			)->getCString());
	}

}

void LotteryWithdraw::queryMyYinHangRet(Object *obj)
{
	float fontSize = 30;
	Dictionary *dic = (Dictionary *)obj;
	int lResult = ((Integer *)dic->objectForKey("lResult"))->getValue();
	String *szKaihuYinghang = (String *)dic->objectForKey("szKaihuYinghang");
	String *szKaihuRen = (String *)dic->objectForKey("szKaihuRen");
	String *szYinhangZhanghu = (String *)dic->objectForKey("szYinhangZhanghu");
	int m_lock = ((Integer *)dic->objectForKey("m_lock"))->getValue();  //是否锁定 不影响提款
	if(szYinhangZhanghu->compare("") != 0){
		m_node->removeFromParentAndCleanup(true);
		initWithdrawView();
		//游戏帐号
		LabelTTF *ltfAccount = (LabelTTF *)this->getChildByTag(100);
		if(ltfAccount)
		{
			ltfAccount->setString(EntityMgr::instance()->getDispatch()->m_szAccount);
		}
		//开户银行
		LabelTTF *yinHanglabel = (LabelTTF *)this->getChildByTag(101);
		if(yinHanglabel)		
		{
			yinHanglabel->setString(szKaihuYinghang->getCString());
		}
		//开户人
		LabelTTF *kaiHuRenlabel = (LabelTTF *)this->getChildByTag(103);
		if(kaiHuRenlabel)
		{
			kaiHuRenlabel->setString(BetLimit::GetNameWithXing(szKaihuRen->getCString()).c_str());
		}		
		//开户帐号
		LabelTTF *zhangHulabel =(LabelTTF *)this->getChildByTag(102);	
		{
			zhangHulabel->setString(BetLimit::GetBankCardWithXing(szYinhangZhanghu->getCString()).c_str());
		}
		//帐户余额
		EditBox *dtMoney = (EditBox *)this->getChildByTag(114);
		if(dtMoney)
		{
			dtMoney->setText(String::createWithFormat("%.3f", EntityMgr::instance()->getDispatch()->f_yue)->getCString());
		}
	}
}

void LotteryWithdraw::initWithdrawView()
{
	startY = SCREEN_HEIGHT - 162 - 60;

	ccColor3B blackColor = ccc3(76,76,76);
	ccColor3B yellowColor = ccc3(222,159,98);
	ccColor3B brownColor = ccc3(161,119, 83);

#ifdef VER_369
	yellowColor = ccc3(50, 62, 123);
	brownColor = ccc3(50, 62, 123);
#endif
	float fontSize= 27;	//项
	float fontSizeTitle = 30; //标题大小
	int tempTag = 0;  

	for(int i = 0;i < 3;i ++)
	{
		//数字，图片
		auto sptNum = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat("dt_tz_bs%02d.png", i + 1)->getCString()));
		sptNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sptNum->setPosition(Vec2(34,startY));
		this->addChild(sptNum, 1);

		//分类标题
		auto ltfTitle = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d", 4500 + i)->getCString()),"",fontSizeTitle);
		ltfTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		ltfTitle->setFontFillColor(yellowColor);
		ltfTitle->setPosition(Vec2(posX1,startY));
		this->addChild(ltfTitle, 1);

		//其它项
		for(int i_item = 0;i_item < 4;i_item ++)
		{
			//当前tag
			int nowTag = tempTag + 4 * i + i_item + 100;
			//项前面
			auto ltfItem = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d", 4510 + tempTag + i_item)->getCString()),"",fontSize);
			ltfItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT); 
			ltfItem->setFontFillColor(blackColor);
			ltfItem->setPosition(Vec2(posX1, startY - 5 - gapY * (i_item + 1)));
			this->addChild(ltfItem, 1);
			if(i == 0)
			{			
				//项内容 label
				auto ltfItemContent = LabelTTF::create("","",fontSize);
				ltfItemContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				ltfItemContent->setTag(nowTag);
				ltfItemContent->setFontFillColor(yellowColor);
				ltfItemContent->setPosition(Vec2(posX2, ltfItem->getPositionY()));
				this->addChild(ltfItemContent, 1);
			} else
			{
				//密码和取款数
				auto dtItemContent = EditBox::create(cocos2d::Size(150,50),"beting26.png");
				dtItemContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);				
				dtItemContent->setTag(nowTag);  //设置标记
				CCLOG("this is nowTag %d", nowTag);
				dtItemContent->setPosition(Vec2(posX2, ltfItem->getPositionY()));
				dtItemContent->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
				dtItemContent->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
				dtItemContent->setInputMode(EditBox::InputMode::SINGLE_LINE);
				dtItemContent->setEnabled(false);
				dtItemContent->setAnchorPoint(Vec2(0,0.5f));
				dtItemContent->setFontColor(yellowColor);
				dtItemContent->setFontName("");dtItemContent->setFontSize(21);				
				dtItemContent->setColor(ccc3(112,112,112));	
				dtItemContent->setMaxLength(20);				
				this->addChild(dtItemContent, 1);				

				if(nowTag == 115)
				{
					dtItemContent->setEnabled(true);
					dtItemContent->setInputMode(EditBox::InputMode::DECIMAL);	
					dtItemContent->setDelegate(this);				
				} else
				if(nowTag == 138)
				{
					dtItemContent->setEnabled(true);
					dtItemContent->setInputFlag(EditBox::InputFlag::PASSWORD);
				}	

				//说明	
				int curId = 4510 + tempTag + 10 + i_item;
				auto ltfHint = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml",String::createWithFormat("t%d", curId)->getCString()),"",fontSize - 7);
				ltfHint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				ltfHint->setFontFillColor(ccColor3B::RED);
				ltfHint->setHorizontalAlignment(TextHAlignment::LEFT);
				ltfHint->setTag(nowTag + 1000);
				ltfHint->setPosition(Vec2(posX3, ltfItem->getPositionY()));
				this->addChild(ltfHint);				
				
				if(i_item == 3)	tempTag += 10; //保证只加一次
			}

			
			//输入取款密码只有一项
			if(i == 2) break;
		}	

		if(i!= 2)
		{		
			Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
			gap1->setAnchorPoint(Vec2(0,0.5));
			gap1->setPosition(Vec2(0, startY - 300));
			this->addChild(gap1, 1);
		}

		//自增
		tempTag += 10;
		startY -= 330;
	}	

	//按钮
	Sprite *normalButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_17.png"));
	Sprite *selectButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_18.png"));
	MenuItemSprite *itemSprite = MenuItemSprite::create
		(normalButton,selectButton,CC_CALLBACK_1(LotteryWithdraw::confirmWithdraw,this));
	Menu *confirmButton = Menu::create(itemSprite,NULL);
	confirmButton->setPosition(Vec2(winSize.width/2, 200));
	this->addChild(confirmButton, 1);

	//取款提示
	LabelTTF *ltfTips = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t641"), "", fontSize - 7,cocos2d::Size(SCREEN_WIDTH - 100,120));
	ltfTips->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfTips->setPosition(Vec2(SCREEN_WIDTH / 2, 50));
	ltfTips->setColor(brownColor);
	ltfTips->setTag(9527);
	ltfTips->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(ltfTips, 5);

	//取款限制
	EntityMgr::instance()->getDispatch()->sendPacketWithGetQuKuanLimit();
}

void LotteryWithdraw::confirmWithdraw(Object *obj)
{
	playButtonSound();

	EditBox *dtMoney = (EditBox *)this->getChildByTag(115);
	EditBox *dtPassword = (EditBox *) this->getChildByTag(138);
	if(!dtMoney || !dtPassword)
	{
		return;
	}
	if(!dtMoney->isEnabled()) 
	{		
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			ConfigMgr::instance()->text("display_text.xml", "t4560"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);	
		return;
	}
	float money = atof(dtMoney->getText());
	if(money < f_t_lowjine->getValue() )
	{//f_t_lowjine
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			String::createWithFormat("%s%.1f%s",
			ConfigMgr::instance()->text("display_text.xml", "t4581"),
			f_t_lowjine->getValue(),
			ConfigMgr::instance()->text("display_text.xml", "t4571")
			)->getCString()	,Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);	
		return;
	}	
	if(money > f_t_highjine->getValue())
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			String::createWithFormat("%s%.1f%s",
			ConfigMgr::instance()->text("display_text.xml", "t4582"),
			f_t_highjine->getValue(),
			ConfigMgr::instance()->text("display_text.xml", "t4571")
			)->getCString()	,Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);	
		return;
	}
	if( money > EntityMgr::instance()->getDispatch()->f_yue)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			ConfigMgr::instance()->text("display_text.xml", "t4583"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);	
		return;
	}
	if(dtPassword->getText() == "")
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			ConfigMgr::instance()->text("display_text.xml", "t4580"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);	
		return;
	}
	dtMoney->setEnabled(false);
	dtPassword->setEnabled(false);

	EntityMgr::instance()->getDispatch()->SendPacketWithDoQukuan(money, dtPassword->getText());
}

bool LotteryWithdraw::init()
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

void LotteryWithdraw::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	money = g_GlobalUnits.getGolbalUserData()->lScore;
	posX1 = 60;
	posX2 = 200;
	posX3 = 350;
	gapY = 60;

	n_t_sxfpercent = NULL;
	f_t_lowsxf = NULL;
	f_t_highsxf = NULL;
	n_t_freelastcount = NULL;
	f_t_lowjine = NULL;
	f_t_highjine = NULL;
}

void LotteryWithdraw::initView()
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
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryWithdraw::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t615"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 27;
	m_node = Node::create();
	this->addChild(m_node, 1);

	//您还没有绑定银行卡
	LabelTTF *title1 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t616"), "", fontSize);
	title1->setAnchorPoint(Vec2(0,0.5f));
	title1->setPosition(Vec2(10, SCREEN_HEIGHT - 205));
	title1->setColor(ccc3(171,71,71));
	m_node->addChild(title1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, title1->getPositionY()-55));
	m_node->addChild(gap1);

	Sprite *cardSprite = Sprite::createWithSpriteFrame(spriteFrame("recharge_15.png"));
	cardSprite->setAnchorPoint(Vec2(0.5f,1));
	cardSprite->setPosition(Vec2(SCREEN_WIDTH/2, gap1->getPositionY() - 50));
	m_node->addChild(cardSprite);

	//确定提款
	Sprite *normalButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_17.png"));
	Sprite *selectButton = Sprite::createWithSpriteFrame(spriteFrame("recharge_18.png"));
	MenuItemSprite *itemSprite = MenuItemSprite::create
		(normalButton,selectButton,CC_CALLBACK_1(LotteryWithdraw::bangDing,this));
	Menu *button = Menu::create(itemSprite,NULL);
	button->setPosition(Vec2(winSize.width/2, cardSprite->getPositionY()-cardSprite->getContentSize().height-40));
	m_node->addChild(button);

	EntityMgr::instance()->getDispatch()->QueryMyYinHang();
}

void LotteryWithdraw::bangDing(Object *obj)
{
	playButtonSound();

	Scene *scene = LotteryCardBinding::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	clear();
}

void LotteryWithdraw::back(Object *obj)
{
	playButtonSound();
	Director::getInstance()->popScene();
}

void LotteryWithdraw::onEnter()
{
	Layer::onEnter();
}

void LotteryWithdraw::onExit()
{
	Layer::onExit();
}

void LotteryWithdraw::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryWithdraw::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	if (editBox->getTag() == 115)
	{
		EditBox* shouxufei = (EditBox *)this->getChildByTag(116);
		EditBox* shiji = (EditBox *)this->getChildByTag(117);

		//取款的钱
		double money = atof(text.c_str());
		//手续费
		double tempMoney = 0;
		if(n_t_freelastcount != NULL && n_t_freelastcount->getValue() <= 0)
		{	
			if(n_t_sxfpercent != NULL) tempMoney = money * n_t_sxfpercent->getValue() / 100.0f;	
			tempMoney = int(tempMoney + 0.5f);
			if (f_t_highsxf != NULL && tempMoney > f_t_highsxf->getValue()) tempMoney = 25;
			if (f_t_lowsxf != NULL && tempMoney < f_t_lowsxf->getValue()) tempMoney = 1;
		}

		//到帐
		double trueMoney = money - tempMoney;
		if(trueMoney < 0) {tempMoney = 0; trueMoney = 0;}
		 
		if(shouxufei)
		{			
			shouxufei->setText(String::createWithFormat("%.3f",tempMoney)->getCString());
		}
		if(shiji)
		{
			shiji->setText(String::createWithFormat("%.3f",trueMoney)->getCString());
		}
	}
}

void LotteryWithdraw::editBoxReturn(EditBox* editBox)
{

}

void LotteryWithdraw::editBoxEditingDidEnd(EditBox* editBox)
{

}

void LotteryWithdraw::editBoxEditingDidBegin(EditBox* editBox)
{

}

void LotteryWithdraw::clear()
{
	NotificationCenter::getInstance()->removeObserver(this, "QueryMyYinHangRet");
	NotificationCenter::getInstance()->removeObserver(this, "DoQukuanRet");
	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "GetQuKuanLimitRet");

	if(n_t_sxfpercent) n_t_sxfpercent->release();
	if(f_t_lowsxf) f_t_lowsxf->release();
	if(f_t_highsxf) f_t_highsxf->release();
	if(n_t_freelastcount) n_t_freelastcount->release();
	if(f_t_lowjine) f_t_lowjine->release();
	if(f_t_highjine) f_t_highjine->release();
}
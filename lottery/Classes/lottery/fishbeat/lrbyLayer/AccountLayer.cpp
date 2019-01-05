#include "AccountLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "LrbyClientView.h"
#include "ByMenuLayer.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

AccountLayer::AccountLayer(void)
{
	uiWidget=nullptr;
	m_btnExit=nullptr;
	m_btnCancel=nullptr;
	for (int i=0; i<24;++i)
	{
		m_txtFish[i]=nullptr;
	}
}

AccountLayer::~AccountLayer(void)
{

}

bool AccountLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	this->setContentSize(cocos2d::Size(NS_lrbyFish::kResolutionWidth,NS_lrbyFish::kResolutionHeight));
	auto winSize = Director::getInstance()->getWinSize();
	uiWidget = GUIReader::getInstance()->widgetFromJsonFile("709/GameAccount/GameAccount.ExportJson");
	uiWidget->setRotation(180.0f);
	uiWidget->setPosition(Vec2(uiWidget->getContentSize().width, uiWidget->getContentSize().height));
	this->addChild(uiWidget);
	this->setContentSize(uiWidget->getContentSize());
	
	//°´Å¥
	m_btnExit = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget, "Button_AccountAndQuit"));
	m_btnCancel = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget, "Button_Cancel"));
	m_btnExit->addTouchEventListener(this,SEL_TouchEvent(&AccountLayer::OnBtnCallBack));
	m_btnCancel->addTouchEventListener(this,SEL_TouchEvent(&AccountLayer::OnBtnCallBack));
	m_btnExit->setTag(TYPE_EXIT);
	m_btnCancel->setTag(TYPE_CANCLE);
	string strName;
	for (int i=0; i<24;++i)
	{
		strName = StringUtils::format("Label_fish%d",i);
		m_txtFish[i] = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,strName));
	}
	//ÆÁ±Îµ×²ã´¥Ãþ
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	this->setScale(0);
	this->runAction(Sequence::create(ScaleTo::create(0.3f,1.0f),CallFunc::create([&]{
		this->setOpacity(125);
	}),NULL));
	return true;
}

void AccountLayer::OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	LrbyClientView* pView = (LrbyClientView*)(this->getParent());
	switch (tag)
	{
	case TYPE_EXIT:
		{			
			if(pView != nullptr)
			{
				pView->onAccountGoldAndQuit();
			}
		}
		break;
	case TYPE_CANCLE:
		{
			this->removeFromParentAndCleanup(true);
			LrbyClientView::GetInstance()->setSuccess(true);
			ByMenuLayer::GetInstance()->setExitMenuStatue(true);
			if (pView != nullptr)
			{
				pView->onAccountCancle();
			}
		}
		break;
	default:
		break;
	}
}

void AccountLayer::setAccountLayerInfo(CMD_S_FireHarvest* fishHarvest)
{
	char strInfo[64] = {0};
	for (int i=0; i<24;++i)
	{
		sprintf(strInfo, "X %d", fishHarvest->nFishHarvest[i]);
		m_txtFish[i]->setText(strInfo);
	}
}

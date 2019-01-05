#include "CharityLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

CharityLayer::CharityLayer(void)
{
	m_widgetUI = nullptr;
	server_close = nullptr;
    btn_ok = nullptr;
	text_get = nullptr;
}

CharityLayer::~CharityLayer(void)
{

}

CharityLayer * CharityLayer::create(float fScale)
{
	CharityLayer *ret = new CharityLayer();
	if (ret && ret->init(fScale))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool CharityLayer::init(float fScale)
{
	if (!Layer::init())
	{
		return false;
	}

	m_widgetUI = GUIReader::getInstance()->widgetFromJsonFile("709/Charity/Charity.ExportJson");
	this->addChild(m_widgetUI);
	this->setContentSize(m_widgetUI->getContentSize());

	btn_ok	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_ok");
	btn_ok->addTouchEventListener(this,SEL_TouchEvent(&CharityLayer::OnCallBack));
	btn_ok->setTag(0);

	server_close	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_quit");
	server_close->addTouchEventListener(this,SEL_TouchEvent(&CharityLayer::OnCallBack));
	server_close->setTag(1);

	text_get	= (Text*)Helper::seekWidgetByName(m_widgetUI,"text_get");
	text_get->setText(StringUtils::format("0"));
	//刷新次数，最多领3次
	//tagTaskProgressInfo* pTPI = CMissionPlaza::GetInstance()->GetGameTaskInfo()->GetTaskProgress(2);
	//CHAR buf[20]={0};
	//if(pTPI!=nullptr)
	//{
		
//		TChar2Utf8(pTPI->szProgress,buf,sizeof(buf));
//		int temp = stoi(buf);
//		text_get->setText(StringUtils::format("%d",temp+1));
	//}
	//屏蔽底层触摸
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	return true;
}

void CharityLayer::OnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	dynamic_cast<Button*>(sender)->setEnabled(false);
	switch (tag)
	{
	case 0:
		{			
			this->removeFromParentAndCleanup(true);
		}break;
	case 1:
		{			
			this->removeFromParentAndCleanup(true);
		}break;
	default:
		break;
	}
}

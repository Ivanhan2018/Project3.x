#include "Result.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"

#include "Sound.h"


USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

Result::Result(void)
{
	m_uiWidget = nullptr;
	m_ResultBack = nullptr;
	m_ResultOk = nullptr;
	m_MyLabel = nullptr;
	m_BankLabel = nullptr;
}
Result::~Result(void)
{
}
bool Result::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	m_uiWidget = GUIReader::getInstance()->widgetFromJsonFile("ComLayer/Result/Result.json");
	this->addChild(m_uiWidget);

	//按比例绽放 长宽
	m_uiWidget->setScaleX((float)1136/1920);
	m_uiWidget->setScaleY((float)640/1080);

	m_ResultBack = dynamic_cast<Button*>(Helper::seekWidgetByName(m_uiWidget,"m_ResultBack"));
	m_ResultBack->setTag(Enum_ResultBack);
	//m_ResultBack->addTouchEventListener(this,SEL_TouchEvent(&Result::onBtnFace));
	m_ResultOk = dynamic_cast<Button*>(Helper::seekWidgetByName(m_uiWidget,"m_ResultOk"));
	m_ResultOk->setTag(Enum_ResultOk);
	//m_ResultOk->addTouchEventListener(this,SEL_TouchEvent(&Result::onBtnFace));
	m_MyLabel = dynamic_cast<Text*>(Helper::seekWidgetByName(m_uiWidget,"m_MyLabel"));
	m_BankLabel = dynamic_cast<Text*>(Helper::seekWidgetByName(m_uiWidget,"m_BankLabel"));
   
	updateView();
	return true;
}
void Result::updateView()
{
	//更新钱钱
	//m_MyLabel->setText(StringUtils::format("$ %lli",));
	//m_BankLabel->setText(StringUtils::format("$ %lli",));
}
void Result::onBtnFace(Ref* sender, TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	switch (tag)
	{
	case Enum_ResultBack:
		{
			//返回，删除层
		}
		break;
	case Enum_ResultOk:
		{

		}
		break;
	default:
		break;
	}
}

void Result::onCallback(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	m_ResultBack->addTouchEventListener(target,selector);
	m_ResultOk->addTouchEventListener(target,selector);
}
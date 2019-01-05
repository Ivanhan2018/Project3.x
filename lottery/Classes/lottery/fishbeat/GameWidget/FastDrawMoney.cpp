#include "FastDrawMoney.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "GameScene.h"
#include "VisibleRect.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

FastDrawMoney * FastDrawMoney::m_drawMoney=NULL;	

FastDrawMoney::FastDrawMoney(void)
{
	m_drawMoney	= this;
	UI= nullptr;
	m_szCinText[0] = nullptr;
	m_szCinText[1] = nullptr;
	textMoney_YH   = nullptr;
	textMoney_BB   = nullptr;
}

FastDrawMoney::~FastDrawMoney(void)
{
	m_drawMoney = nullptr;
}

FastDrawMoney* FastDrawMoney::create()
{
	FastDrawMoney *ret = new FastDrawMoney();
	if (ret && ret->init())
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
bool FastDrawMoney::init()
{
	UI = GUIReader::getInstance()->widgetFromJsonFile("FastDrawMoney/FastDrawMoney.json");
	this->addChild(UI);
	textMoney_YH = dynamic_cast<Text*>(Helper::seekWidgetByName(UI,"textMoney_YH"));
	textMoney_BB = dynamic_cast<Text*>(Helper::seekWidgetByName(UI,"textMoney_BB"));
	this->setContentSize(UI->getContentSize());

	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	textMoney_YH->setText(StringUtils::format("%lli",100));
	textMoney_BB->setText(StringUtils::format("%lli",100));
	for (int i=0;i<2;i++)
	{
		auto btn = dynamic_cast<Button*>(Helper::seekWidgetByName(UI,StringUtils::format("fdm_btn%d",i)));
		btn->setTag(i+1);
		btn->addTouchEventListener(this,SEL_TouchEvent(&FastDrawMoney::OnBtnCallBack));
		auto inputTxt = dynamic_cast<TextField*>(Helper::seekWidgetByName(UI,StringUtils::format("fdm_txt%d",i)));
		inputTxt->setTag(i+1);
		inputTxt->setTextHorizontalAlignment(TextHAlignment::CENTER);
		inputTxt->setTextVerticalAlignment(TextVAlignment::CENTER);
		inputTxt->addEventListenerTextField(this,SEL_TextFieldEvent(&FastDrawMoney::OnTextFieldCallBack));
	}
// 	updateView();
	//屏蔽底层触摸
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	this->setScale(0);
	this->runAction(Sequence::create(ScaleTo::create(0.3f,1),NULL));
	return true;
}
void FastDrawMoney::OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	if (tag == 1)
	{
		this->removeFromParentAndCleanup(true);
	}
	else if (tag == 2)
	{
		//取出的金币数量
		
	}
}
void FastDrawMoney::OnTextFieldCallBack(cocos2d::Ref* sender, cocos2d::ui::TextFiledEventType type)
{
	TextField* txtField = dynamic_cast<TextField*>(sender);
	BYTE tag = txtField->getTag();
	switch (type)
	{
	case cocos2d::ui::TEXTFIELD_EVENT_ATTACH_WITH_IME:
		{
			txtField->setText("");
			m_szCinText[tag-1] = txtField->getStringValue().c_str();
		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_DETACH_WITH_IME://输入结束事件
		{

		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_INSERT_TEXT:
		{
			const char* szTemp = txtField->getStringValue().c_str();
			switch (tag)
			{
			case 1:
				{
					char number[20] ={0};
					GetNumber(szTemp,number);
					txtField->setText(StringUtils::format("%s",number));
					m_szCinText[0] = txtField->getStringValue().c_str();
				}break;
			case 2:
				{
					m_szCinText[1] = txtField->getStringValue().c_str();
				}break;
			}
			break;
		}
	case cocos2d::ui::TEXTFIELD_EVENT_DELETE_BACKWARD:
		break;
	}
}

void FastDrawMoney::onUpdateView()
{
	textMoney_YH->setText(StringUtils::format("%lli",100));
	textMoney_BB->setText(StringUtils::format("%lli",100));
}

char* FastDrawMoney::GetNumber(const char* src,char* buf)
{
	int len = strlen(src);
	if (len > 11)
		return buf;
	for(int i=0;i<len;i++)
	{
		if((src[0]=='0' || src[i]<'0' || src[i]>'9'))
			return buf;
		buf[i]=src[i];
	}
	return buf;
}

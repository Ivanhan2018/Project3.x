
#include "ChatBoard.h"
#include "cocostudio/CocoStudio.h"
#include "UserInfoLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace cocostudio;


#define Def_Board_Size		cocos2d::Size(314,434)
#define Def_Spread_Speed	0.5
ChatBoard::ChatBoard()
{
	_isSpread = false;
	m_TouchListener = nullptr;
}

ChatBoard::~ChatBoard()
{
	SetLayerTouchEnable(false);
}

bool ChatBoard::init()
{
	if (!Layer::init())
	{
		return false;
	}


	loadUIFromJson();
	
	m_ChatList->setItemsMargin(3);
	m_CtrlBtn->addTouchEventListener(this,toucheventselector(ChatBoard::onCtrlBtn));
	m_CtrlBtn->setPressedActionEnabled(true);

	//tag 48
	m_BtnWorld->addTouchEventListener(this, toucheventselector(ChatBoard::onSwitchList));
	
	//tag 51
	m_BtnGroup->addTouchEventListener(this, toucheventselector(ChatBoard::onSwitchList));

	//tag 52
	m_BtnWisper->addTouchEventListener(this, toucheventselector(ChatBoard::onSwitchList));

	m_BtnSendMsg->addTouchEventListener(this, toucheventselector(ChatBoard::onSendMsg));
	m_BtnSendMsg->setPressedActionEnabled(true);

	m_BtnWorld->setBright(false);
	m_BtnGroup->setBright(true);
	m_BtnWisper->setBright(true);
	m_CurretMode = CHAT_WORD;

	testRecoverWord();

	return true;
}

void ChatBoard::SpreadBoard()
{
	if (_isSpread)
	{
		//收缩
		ActionInterval* act = MoveBy::create(Def_Spread_Speed, Vec2(-m_ChatPanel->getContentSize().width, 0));
		m_ChatPanel->runAction(act);
		SetLayerTouchEnable(false);
	}
	else
	{
		//弹出
		ActionInterval* act = MoveBy::create(Def_Spread_Speed, Vec2(m_ChatPanel->getContentSize().width, 0));
		m_ChatPanel->runAction(act);
		SetLayerTouchEnable(true);
	}
	_isSpread = !_isSpread;
}

void ChatBoard::onCtrlBtn(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
	{
		return;
	}
	SpreadBoard();
}


void ChatBoard::testRecoverWord()
{
	Text* tx = Text::create("test text", "", 20);
	tx->setColor(Color3B::RED);
	tx->setAnchorPoint(Vec2::ZERO);
	tx->setPosition(Vec2(10,2));


	m_ChatList->setItemModel(tx);

	m_ChatList->pushBackDefaultItem();
	m_ChatList->pushBackDefaultItem();
}

void ChatBoard::loadUIFromJson()
{
	m_widgetUI = GUIReader::getInstance()->widgetFromJsonFile("PopWindow/zhongyou_chat.json");
	this->addChild(m_widgetUI);

	m_ChatPanel = dynamic_cast<ImageView*>(Helper::seekWidgetByName(m_widgetUI, "img_back"));

	m_ChatList = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_widgetUI, "list_word"));

	m_InputBox = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_widgetUI, "TextField_input"));

	m_CtrlBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI, "btn_show"));
	m_BtnWorld = dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI, "btn_world"));
	m_BtnGroup = dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI, "btn_group"));
	m_BtnWisper = dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI, "btn_wisper"));

	m_BtnSendMsg = dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI, "btn_send"));

	m_PanelSize = m_ChatPanel->getContentSize();
}

void ChatBoard::onSwitchList(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
	{
		return;
	}

	Button* btn = (Button*)sender;
	switch (btn->getTag())
	{
	case 48://word
		{
			setChatMode(CHAT_WORD);
		}
		break;
	case 51://group
		{
			setChatMode(CHAT_GROUP);
		}
		break;
	case 52://wisper
		{
			setChatMode(CHAT_WISPER);
		}
		break;

	default:
		break;
	}
}

void ChatBoard::setChatMode(CHAT_MODE mode)
{
	switch (mode)
	{
	case ChatBoard::CHAT_WORD:
		{
			m_BtnWorld->setBright(false);
			m_BtnGroup->setBright(true);
			m_BtnWisper->setBright(true);

		}
		break;
	case ChatBoard::CHAT_GROUP:
		{
			m_BtnWorld->setBright(true);
			m_BtnGroup->setBright(false);
			m_BtnWisper->setBright(true);		

		}
		break;
	case ChatBoard::CHAT_WISPER:
		{
			m_BtnWorld->setBright(true);
			m_BtnGroup->setBright(true);
			m_BtnWisper->setBright(false);

		}
		break;
	default:
		break;
	}
}



void ChatBoard::onSendMsg(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	// 	SpreadBoard(true);
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
	{
		return;
	}
	
	SendMessage();
}

void ChatBoard::SendMessage()
{
	log("msg length = %d", m_InputBox->getStringLength());
	if (m_InputBox->getStringLength() == 0)
	{
		return;
	}
	float lyhigh = 30;
	std::string		WordStr;
	WordStr = WrapStr(m_InputBox->getStringValue(), 20);
	//以头像的位置为基准，生成头像和文字，以长的作为高度，文字一头像宽度为基准进行偏移

	ImageView* head = ImageView::create(StringUtils::format("Lobby_face_%d.png", 1%MAX_FACEID),TextureResType::PLIST) ;

	//

	LabelTTF* tx = LabelTTF::create(WordStr, "", 20);
	tx->setHorizontalAlignment(TextHAlignment::LEFT);

	tx->setFontFillColor(Color3B::RED);
	// 	Text* tx = Text::create(m_InputBox->getStringValue().c_str(), "", 20);
	// 	tx->ignoreContentAdaptWithSize(false);
	// 	tx->setContentSize(cocos2d::Size(150,90));

	tx->setColor(Color3B::WHITE);

	if (lyhigh < head->getContentSize().height)
	{
		lyhigh = head->getContentSize().height;
	}

	if (lyhigh < tx->getContentSize().height)
	{
		lyhigh = tx->getContentSize().height;
	}

	Layout* ly = Layout::create();
	// 	ly->setBackGroundColor(Color3B::GREEN);
	ly->setBackGroundColorType(Layout::BackGroundColorType::NONE);
	ly->setAnchorPoint(Vec2::ZERO);
	ly->setContentSize(cocos2d::Size(m_PanelSize.width* 0.9, lyhigh));

	head->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	head->setPosition(Vec2(ly->getContentSize().width, ly->getContentSize().height));

	tx->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	tx->setPosition(Vec2(ly->getContentSize().width - head->getContentSize().width - 20, lyhigh - 5));

	ly->addChild(head);
	ly->addChild(tx);

	m_ChatList->pushBackCustomItem(ly);

	m_InputBox->setText("");

	delayFreshList();
	// 	this->runAction(Sequence::createWithTwoActions());


}


std::string ChatBoard::WrapStr(std::string str, int length)
{
	unsigned int beginPos = 0;

	std::string resultStr;

	std::vector<std::string> str_vec;

	do 
	{
		str_vec.push_back(str.substr(beginPos, length));

		if (beginPos + length > str.size())
		{
			break;
		}
		else
		{
			beginPos += length;
		}



	} while (true);

	for (int i = 0; i < str_vec.size(); i++)
	{
		resultStr.append(str_vec.at(i).append("\n"));
	}

	return resultStr;
}

void ChatBoard::delayFreshList()
{
	scheduleOnce(schedule_selector(ChatBoard::onfreshList), 0.01);
}

void ChatBoard::onfreshList(float dt)
{
	m_ChatList->scrollToBottom(0.1, true);
}


void ChatBoard::SetLayerTouchEnable(bool var)
{
	if (var)
	{
		if (m_TouchListener == nullptr)
		{	
			m_TouchListener = EventListenerTouchOneByOne::create();
			m_TouchListener->onTouchBegan = [](Touch*,Event*)->bool{return true;};
			m_TouchListener->retain();
		}

		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_TouchListener, this);
		m_TouchListener->setEnabled(true);
		m_TouchListener->setSwallowTouches(true);
	}
	else
	{
		if (m_TouchListener)
		{
			m_TouchListener->setEnabled(false);
			_eventDispatcher->removeEventListener(m_TouchListener);
		}
	}
}
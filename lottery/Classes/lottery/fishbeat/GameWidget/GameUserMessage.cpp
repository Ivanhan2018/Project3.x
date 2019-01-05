#include "GameUserMessage.h"

#include "UserInfoLayer.h"

GameUserMessage* GameUserMessage::m_gameUserMsg = nullptr;

GameUserMessage::GameUserMessage(void)
	:m_uiWidget(nullptr),
	m_MessageBack(nullptr),
	m_MessageFace(nullptr),
	m_MessageNick(nullptr),
	m_MessageUid(nullptr),
	m_MessageVip(nullptr),
	m_MessageCoin(nullptr),
	m_vipImg(nullptr)
{
}
GameUserMessage::~GameUserMessage(void)
{
}
bool GameUserMessage::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("face/face.plist","face/face.png");
	m_uiWidget = GUIReader::getInstance()->widgetFromJsonFile("ComLayer/GameUserMessageUi/GameUserMessageUi.json");
	this->addChild(m_uiWidget);

	m_MessageBack = dynamic_cast<Button*>(Helper::seekWidgetByName(m_uiWidget,"MessageBack"));
	m_MessageBack->setTag(Enum_GameUserMessageBack);
	m_MessageBack->addTouchEventListener(this, SEL_TouchEvent(&GameUserMessage::onClose));
	m_MessageFace = dynamic_cast<ImageView*>(Helper::seekWidgetByName(m_uiWidget,"MessageFace"));
	m_MessageFace->setVisible(false);
	m_MessageNick = dynamic_cast<Text*>(Helper::seekWidgetByName(m_uiWidget,"MessageNick"));
	m_MessageUid = dynamic_cast<Text*>(Helper::seekWidgetByName(m_uiWidget,"MessageUid"));
	m_MessageVip = dynamic_cast<Text*>(Helper::seekWidgetByName(m_uiWidget,"MessageVip"));
	m_MessageCoin = dynamic_cast<Text*>(Helper::seekWidgetByName(m_uiWidget,"MessageCoin"));
	m_vipImg = dynamic_cast<ImageView*>(Helper::seekWidgetByName(m_uiWidget, "vipImg"));
	m_MessageNick->setFontName("Impact");
	m_MessageUid->setFontName("Impact");
	m_MessageVip->setFontName("Impact");
	m_MessageCoin->setFontName("Impact");
	return true;
}

void GameUserMessage::setRecordValue(WORD faceID,std::string strName, DWORD userID, BYTE VipLv, LONGLONG userCoin)
{
	
	if (strlen(strName.c_str()) != 0)
	{
		m_MessageFace->loadTexture(StringUtils::format("Lobby_face_%d.png", faceID),TextureResType::PLIST);

		m_MessageFace->setVisible(true);
		m_MessageNick->setString(strName);
		m_MessageUid->setString(StringUtils::format("%d", userID));
		m_MessageVip->setString(StringUtils::format("%d", VipLv));
		m_MessageCoin->setString(StringUtils::format("%lli", userCoin));		
		if (VipLv == 0)
		{
			m_vipImg->setVisible(false);
		}
		else
		{
			m_vipImg->loadTexture(StringUtils::format("ComLayer/VIPDisplay/icon_VIP%d-L.png", VipLv <= 3 ? VipLv : 3));
			m_vipImg->setVisible(true);
		}
	}	
	else
	{
		m_MessageFace->setVisible(false);
		m_MessageNick->setString("");
		m_MessageUid->setString("");
		m_MessageVip->setString("");
		m_MessageCoin->setString("");
		m_vipImg->setVisible(false);
	}
}
void GameUserMessage::onCallback(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	m_MessageBack->addTouchEventListener(target,selector);
}

GameUserMessage* GameUserMessage::getInstance(WORD nChairID)
{
	if (m_gameUserMsg == nullptr)
	{
		m_gameUserMsg = GameUserMessage::create();
	}
		
	return m_gameUserMsg;
}

void GameUserMessage::onClose(Ref* pSender, TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
	{
		return;
	}
	destoryInstance();
}

void GameUserMessage::destoryInstance(void)
{
	if (m_gameUserMsg != nullptr)
	{
		m_gameUserMsg->removeFromParentAndCleanup(true);
		m_gameUserMsg = nullptr;
	}
}

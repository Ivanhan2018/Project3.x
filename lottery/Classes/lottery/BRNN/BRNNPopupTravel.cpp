#include "BRNNPopupTravel.h"
#include "BRNNDefine.h"
#include "BRNNRes.h"

BRNNPopupTravel::BRNNPopupTravel()
{
}

BRNNPopupTravel::~BRNNPopupTravel()
{
}

void BRNNPopupTravel::addListItem(BRNN_tagServerGameRecord sData)
{
	bool bArray[E_BRNNBetArea_NULL] =
	{
		sData.bWinShunMen,
		sData.bWinDuiMen,
		sData.bWinDaoMen,
		sData.bWinHuang
	};

	if (getItemCount() >= BRNN_MAX_SCORE_HISTORY)
	{
		m_pListView->removeItem(0);
	}

	//	单列布局
	Layout* pLayout = Layout::create();
	m_pListView->pushBackCustomItem(pLayout);
	pLayout->setLayoutType(Layout::Type::VERTICAL);
	pLayout->setContentSize(BRNN_TRAVEL_COLUMN_SIZE);

	for (int i = E_BRNNBetArea_E; i < E_BRNNBetArea_NULL; i++)
	{
		//	底图
		ImageView* pItemBG = ImageView::create(BRNNTextureName::s_brnn_popup_travel_item_bg, Widget::TextureResType::PLIST);
		pLayout->addChild(pItemBG);
		//	图标
		ImageView* pIcon = nullptr;
		if (bArray[i] == true)
		{
			pIcon = ImageView::create(BRNNTextureName::s_brnn_popup_travel_icon_win, Widget::TextureResType::PLIST);
		}
		else
		{
			pIcon = ImageView::create(BRNNTextureName::s_brnn_popup_travel_icon_lose, Widget::TextureResType::PLIST);
		}
		pItemBG->addChild(pIcon);
		pIcon->setPosition(Vec2(pItemBG->getContentSize() / 2));
	}
	m_pListView->jumpToPercentHorizontal(100);
}

int BRNNPopupTravel::getItemCount()
{
	if (!m_pListView)
	{
		return -1;
	}
	int num = m_pListView->getItems().size();
	return num;
}

void BRNNPopupTravel::setPopupVisible(bool bIsVisible)
{
	if (bIsVisible)
	{
		this->setVisible(true);
		m_pListView->setEnabled(true);
	}
	else
	{
		this->setVisible(false);
		m_pListView->setEnabled(false);
	}
}

bool BRNNPopupTravel::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		initUI();

		//	添加触摸事件
		auto pListener = EventListenerTouchOneByOne::create();
		pListener->onTouchBegan = CC_CALLBACK_2(BRNNPopupTravel::onTouchBegan, this);
		pListener->onTouchMoved = CC_CALLBACK_2(BRNNPopupTravel::onTouchMoved, this);
		pListener->onTouchCancelled = CC_CALLBACK_2(BRNNPopupTravel::onTouchCancelled, this);
		pListener->onTouchEnded = CC_CALLBACK_2(BRNNPopupTravel::onTouchEnded, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);

		bRet = true;
	} while (0);

	return bRet;
}

void BRNNPopupTravel::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	m_pBG = Sprite::create(BRNNTextureName::s_brnn_popup_travel_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize.width * 0.5, sVSize.height * 0.5));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	std::string sArrTureName[E_BRNNBetArea_NULL] =
	{
		BRNNTextureName::s_brnn_popup_travel_icon_e,
		BRNNTextureName::s_brnn_popup_travel_icon_s,
		BRNNTextureName::s_brnn_popup_travel_icon_w,
		BRNNTextureName::s_brnn_popup_travel_icon_n
	};

	//	标题

	Layout* pLayout = Layout::create();
	m_pBG->addChild(pLayout);
	pLayout->setLayoutType(Layout::Type::VERTICAL);
	pLayout->setContentSize(BRNN_TRAVEL_COLUMN_SIZE);
	pLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pLayout->setPosition(Vec2(sBGSize.width * 0.113104, sBGSize.height * 0.5));
	for (int i = E_BRNNBetArea_E; i < E_BRNNBetArea_NULL; i++)
	{
		ImageView* pItemBG = ImageView::create(BRNNTextureName::s_brnn_popup_travel_item_bg, Widget::TextureResType::PLIST);
		pLayout->addChild(pItemBG);
		ImageView* pIcon = ImageView::create(sArrTureName[i], Widget::TextureResType::PLIST);
		pItemBG->addChild(pIcon);
		pIcon->setPosition(Vec2(pItemBG->getContentSize() / 2));
	}

	//	滚动列表

	m_pListView = ListView::create();
	m_pBG->addChild(m_pListView);
	m_pListView->setPosition(Vec2(sBGSize.width * 0.54, sBGSize.height * 0.5));
	m_pListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pListView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_pListView->setContentSize(BRNN_TRAVEL_VIEW_SIZE);
	m_pListView->setScrollBarEnabled(false);
}

bool BRNNPopupTravel::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (this->isVisible())
	{
		return true;
	}

	return false;
}

void BRNNPopupTravel::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void BRNNPopupTravel::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void BRNNPopupTravel::onTouchEnded(Touch *touch, Event *unused_event)
{
	Vec2 vTouchPos = touch->getLocation();
	if (m_pBG->getBoundingBox().containsPoint(vTouchPos))
	{
		return;
	}
	setPopupVisible(false);
}

#include "BRNNPopupApplyList.h"
#include "BRNNGameScene.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "ToolKit.h"
#include "MyConfig.h"

BRNNPopupApplyList::BRNNPopupApplyList():
	m_pBG(nullptr),
	m_pListView(nullptr)
{
}

BRNNPopupApplyList::~BRNNPopupApplyList()
{
}

void BRNNPopupApplyList::updateListView()
{
	BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
	if (!pScene)
	{
		return;
	}
	int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	if (m_mItemArray.size() <= 0)
	{
		return;
	}
	for (auto k : m_mItemArray)
	{
		int nChairID = k.first;
		Widget* pItem = k.second;
		IUserItem* pUserItem = EntityMgr::instance()->roomFrame()->getUserItemByChairID(nChairID);
		if (!pUserItem)
		{
			//	如果这个椅子号没有信息，下一个
			continue;
		}
		tagUserData* sData = pUserItem->getUserData();
		SCORE sGold = sData->lScore;
		//	更新最新金币数额
		Label* pLabGold = dynamic_cast<Label*>(pItem->getChildByTag(nChairID));
		if (!pLabGold)
		{
			continue;
		}
		char cGoldBuf[128];
		sprintf(cGoldBuf, "%.2f", sGold);
		pLabGold->setString(cGoldBuf);
	}
}

void BRNNPopupApplyList::setPopupVisible(bool bIsVisible)
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

void BRNNPopupApplyList::addListItem(int nChairID)
{
	bool bIsExist = isItemExist(nChairID);
	if (bIsExist)
	{
		return;
	}

	IUserItem* pUserItem = EntityMgr::instance()->roomFrame()->getUserItemByChairID(nChairID);
	if (!pUserItem)
	{
		return;
	}
	tagUserData* sData = pUserItem->getUserData();

	ImageView* pBG = ImageView::create(BRNNTextureName::s_brnn_dialog_apply_list_item_bg, TextureResType::PLIST);
	m_pListView->pushBackCustomItem(pBG);
	cocos2d::Size sBGSize = pBG->getContentSize();

	//	头像
	Sprite* pFace = nullptr;
	if (sData->cbGender == 1)	//	男
	{
		pFace = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_dialog_apply_list_item_face_man);
	}
	else
	{
		pFace = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_dialog_apply_list_item_face_woman);
	}
	pBG->addChild(pFace);
	pFace->setPosition(Vec2(sBGSize.width * 0.12, sBGSize.height * 0.5));

	//	昵称
	Label* pLabName = Label::create();
	pBG->addChild(pLabName);
	pLabName->setPosition(Vec2(sBGSize.width * 0.31, sBGSize.height * 0.5));
	pLabName->setSystemFontSize(25);
	pLabName->setColor(Color3B::WHITE);
	pLabName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabName->setString(sData->szNickName);

	//	财富
	char cBufGold[128];
	sprintf(cBufGold, "%.2f", sData->lScore);
	Label* pLabGold = Label::create();
	pBG->addChild(pLabGold);
	pLabGold->setPosition(Vec2(sBGSize.width * 0.71, sBGSize.height * 0.5));
	pLabGold->setTag(nChairID);
	pLabGold->setSystemFontSize(25);
	pLabGold->setColor(Color3B::WHITE);
	pLabGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabGold->setString(cBufGold);

	//	储存到子项数组中
	m_mItemArray[nChairID] = pBG;
}

void BRNNPopupApplyList::addListItem(BRNN_tagBankerListPlayer sBankerData)
{
	ImageView* pBG = ImageView::create(BRNNTextureName::s_brnn_dialog_apply_list_item_bg, TextureResType::PLIST);
	m_pListView->pushBackCustomItem(pBG);
	cocos2d::Size sBGSize = pBG->getContentSize();

	//	头像
	Sprite* pFace = nullptr;
	if (sBankerData.cbGender == 1)	//	男
	{
		pFace = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_dialog_apply_list_item_face_man);
	}
	else
	{
		pFace = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_dialog_apply_list_item_face_woman);
	}
	pBG->addChild(pFace);
	pFace->setPosition(Vec2(sBGSize.width * 0.12, sBGSize.height * 0.5));

	//	昵称
	string sName;
	char cNikeName[256] = {0};
	MyNSString::wc2mbs(sBankerData.szNickName, 32, cNikeName);
	sName = cNikeName;
	tagUserData* pSelfData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	if (sBankerData.wChairID != pSelfData->wChairID)
	{
		sName = ToolKit::dealStringOfPrivacy(sName);
	}

	Color3B cFontColor;
	float fFontSize = 25;

	MyConfig& myconfig=MyConfig::Instance();
	if(myconfig.m_platform==0)
	{
		cFontColor = Color3B::BLACK;
	}
	else
	{
		cFontColor = Color3B::WHITE;
	}

	Label* pLabName = Label::create();
	pBG->addChild(pLabName);
	pLabName->setPosition(Vec2(sBGSize.width * 0.31, sBGSize.height * 0.5));
	pLabName->setSystemFontSize(fFontSize);
	pLabName->setColor(cFontColor);
	pLabName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabName->setString(sName);

	//	财富
	char cBufGold[128];
	sprintf(cBufGold, "%.2f", sBankerData.lScore);
	Label* pLabGold = Label::create();
	pBG->addChild(pLabGold);
	pLabGold->setPosition(Vec2(sBGSize.width * 0.71, sBGSize.height * 0.5));
	pLabGold->setTag(sBankerData.wChairID);
	pLabGold->setSystemFontSize(fFontSize);
	pLabGold->setColor(cFontColor);
	pLabGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabGold->setString(cBufGold);

	//	储存到子项数组中
	m_mItemArray[sBankerData.wChairID] = pBG;
}

Widget* BRNNPopupApplyList::getListItem(int nChairID)
{
	return dynamic_cast<Widget*>(m_mItemArray[nChairID]);
}

void BRNNPopupApplyList::removeListItem(int nChairID)
{
	if (!getListItem(nChairID))
	{
		return;
	}
	int idx = m_pListView->getIndex(m_mItemArray[nChairID]);
	if (idx >= 0)
	{
		m_pListView->removeItem(idx);
		m_mItemArray.erase(nChairID);
	}
}

void BRNNPopupApplyList::removeAllListItem()
{
	m_pListView->removeAllItems();
	m_mItemArray.clear();
}

bool BRNNPopupApplyList::isItemExist(int nChairID)
{
	if (m_mItemArray[nChairID])
	{
		return true;
	}

	return false;
}

bool BRNNPopupApplyList::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		initUI();

		//	添加触摸事件
		auto pListener = EventListenerTouchOneByOne::create();
		pListener->onTouchBegan = CC_CALLBACK_2(BRNNPopupApplyList::onTouchBegan, this);
		pListener->onTouchMoved = CC_CALLBACK_2(BRNNPopupApplyList::onTouchMoved, this);
		pListener->onTouchCancelled = CC_CALLBACK_2(BRNNPopupApplyList::onTouchCancelled, this);
		pListener->onTouchEnded = CC_CALLBACK_2(BRNNPopupApplyList::onTouchEnded, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);

		bRet = true;
	} while (0);

	return bRet;
}

void BRNNPopupApplyList::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	m_pBG = Sprite::create(BRNNTextureName::s_brnn_apply_list_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize.width * 0.5, sVSize.height * 0.5));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pTitleFace = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_dialog_apply_list_title_face);
	m_pBG->addChild(m_pTitleFace);
	m_pTitleFace->setPosition(Vec2(sBGSize.width * 0.155, sBGSize.height * 0.7));

	m_pTitleNickname = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_dialog_apply_list_title_nickname);
	m_pBG->addChild(m_pTitleNickname);
	m_pTitleNickname->setPosition(Vec2(sBGSize.width * 0.375, sBGSize.height * 0.7));

	m_pTitleGold = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_dialog_apply_list_title_gold);
	m_pBG->addChild(m_pTitleGold);
	m_pTitleGold->setPosition(Vec2(sBGSize.width * 0.735, sBGSize.height * 0.7));

	m_pListView = ListView::create();
	m_pBG->addChild(m_pListView);
	m_pListView->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.37));
	m_pListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pListView->setDirection(ScrollView::Direction::VERTICAL);
	m_pListView->setContentSize(BRNN_APPLY_LIST_VIEW_SIZE);
	m_pListView->setScrollBarEnabled(false);
}

bool BRNNPopupApplyList::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (this->isVisible())
	{
		return true;
	}

	return false;
}

void BRNNPopupApplyList::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void BRNNPopupApplyList::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void BRNNPopupApplyList::onTouchEnded(Touch *touch, Event *unused_event)
{
	Vec2 vTouchPos = touch->getLocation();
	if (m_pBG->getBoundingBox().containsPoint(vTouchPos))
	{
		return;
	}
	this->setPopupVisible(false);
}

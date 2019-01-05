#include "SGLYPopupApplyList.h"
#include "SGLYRes.h"
#include "SGLYGameScene.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "ToolKit.h"
#include "MyConfig.h"
#include "SGLYTool.h"
#include "VersionControl.h"

SGLYPopupApplyList::SGLYPopupApplyList():
	m_pBG(nullptr),
	m_pListView(nullptr)
{
}

SGLYPopupApplyList::~SGLYPopupApplyList()
{
}

void SGLYPopupApplyList::updateListView()
{
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
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

void SGLYPopupApplyList::setPopupVisible(bool bIsVisible)
{
	this->setVisible(bIsVisible);
	m_pListView->setEnabled(bIsVisible);
	m_pListener->setSwallowTouches(bIsVisible);
}

void SGLYPopupApplyList::addListItem(SGLY_tagBankerListPlayer sBankerData)
{
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}

	ImageView* pBG = ImageView::create(SGLYTextureName::sSGLY_PopupApplyListItemBG, TextureResType::PLIST);
	m_pListView->pushBackCustomItem(pBG);
	cocos2d::Size sBGSize = pBG->getContentSize();

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

	Color3B cFontColor = Color3B::WHITE;
	float fFontSize = 25;

	Label* pLabName = Label::create();
	pBG->addChild(pLabName);
	pLabName->setSystemFontSize(fFontSize);
	pLabName->setColor(cFontColor);
	pLabName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabName->setString(sName);

	//	财富
	char cBufGold[128];
	LONGLONG llChipValue = SGLYTool::exchangeForChip(sBankerData.lScore);
	sprintf(cBufGold, "%lld", llChipValue);
	Label* pLabGold = Label::create();
	pBG->addChild(pLabGold);
	pLabGold->setTag(sBankerData.wChairID);
	pLabGold->setSystemFontSize(fFontSize);
	pLabGold->setColor(cFontColor);
	pLabGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabGold->setString(cBufGold);

#ifdef VER_9YI
	pLabName->setPosition(Vec2(sBGSize.width * 0.15, sBGSize.height * 0.4));
	pLabGold->setPosition(Vec2(sBGSize.width * 0.55, sBGSize.height * 0.4));
#endif // VER_9YI

#ifdef VER_QIANBO
	pLabName->setPosition(Vec2(sBGSize.width * 0.14, sBGSize.height * 0.5));
	pLabGold->setPosition(Vec2(sBGSize.width * 0.56, sBGSize.height * 0.5));
#endif // VER_QIANBO

#ifdef VER_369
	pLabName->setPosition(Vec2(sBGSize.width * 0.14, sBGSize.height * 0.5));
	pLabGold->setPosition(Vec2(sBGSize.width * 0.56, sBGSize.height * 0.5));
#endif // VER_QIANBO

	//	储存到子项数组中
	m_mItemArray[sBankerData.wChairID] = pBG;
}

Widget* SGLYPopupApplyList::getListItem(int nChairID)
{
	return dynamic_cast<Widget*>(m_mItemArray[nChairID]);
}

void SGLYPopupApplyList::removeListItem(int nChairID)
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

void SGLYPopupApplyList::removeAllListItem()
{
	m_pListView->removeAllItems();
	m_mItemArray.clear();
}

bool SGLYPopupApplyList::isItemExist(int nChairID)
{
	if (m_mItemArray[nChairID])
	{
		return true;
	}

	return false;
}

bool SGLYPopupApplyList::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BaseDialog::init());
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void SGLYPopupApplyList::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	m_pBG = Sprite::create(SGLYTextureName::sSGLY_PopupApplyListBG);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize.width * 0.5, sVSize.height * 0.5));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pTextTitle = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_PopupApplyListTextTitle);
	m_pBG->addChild(m_pTextTitle);
	m_pTextTitle->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.9));

	m_pTextNickname = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_PopupApplyListTextName);
	m_pBG->addChild(m_pTextNickname);
	m_pTextNickname->setPosition(Vec2(sBGSize.width * 0.2, sBGSize.height * 0.75));

	m_pTextChip = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_PopupApplyListTextChip);
	m_pBG->addChild(m_pTextChip);
	m_pTextChip->setPosition(Vec2(sBGSize.width * 0.6, sBGSize.height * 0.75));

	m_pListView = ListView::create();
	m_pBG->addChild(m_pListView);
	m_pListView->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.36));
	m_pListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pListView->setDirection(ScrollView::Direction::VERTICAL);
	m_pListView->setContentSize(SGLY_APPLY_LIST_VIEW_SIZE);
	m_pListView->setScrollBarEnabled(false);
}

bool SGLYPopupApplyList::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (this->isVisible())
	{
		return true;
	}

	return false;
}

void SGLYPopupApplyList::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void SGLYPopupApplyList::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void SGLYPopupApplyList::onTouchEnded(Touch *touch, Event *unused_event)
{
	Vec2 vTouchPos = touch->getLocation();
	if (m_pBG->getBoundingBox().containsPoint(vTouchPos))
	{
		return;
	}
	this->setPopupVisible(false);
}

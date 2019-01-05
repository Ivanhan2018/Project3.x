#include "PPCPopupApplyList.h"
#include "EntityMgr.h"
#include "PPCGameScene.h"
#include "MyNSString.h"

PPCPopupApplyList::PPCPopupApplyList()
{
}

PPCPopupApplyList::~PPCPopupApplyList()
{
}

bool PPCPopupApplyList::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void PPCPopupApplyList::updateListView()
{
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}
	SCORE sLimitGold = pGameScene->m_lApplyBankerCondition;
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

void PPCPopupApplyList::addListItem(int nChairID)
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

	ImageView* pBG = ImageView::create(PPCTextureName::s_ppc_popup_apply_list_item_bg, TextureResType::PLIST);
	m_pListView->pushBackCustomItem(pBG);
	cocos2d::Size sBGSize = pBG->getContentSize();

	//	头像
	Sprite* pFace = nullptr;
	if (sData->cbGender == 1)	//	男
	{
		pFace = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_apply_list_item_face_man);
	}
	else
	{
		pFace = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_apply_list_item_face_woman);
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

void PPCPopupApplyList::addListItem(ppcTagBankerListPlayer sBankerData)
{
	ImageView* pBG = ImageView::create(PPCTextureName::s_ppc_popup_apply_list_item_bg, TextureResType::PLIST);
	m_pListView->pushBackCustomItem(pBG);
	cocos2d::Size sBGSize = pBG->getContentSize();

	//	头像
	Sprite* pFace = nullptr;
	if (sBankerData.cbGender == 1)	//	男
	{
		pFace = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_apply_list_item_face_man);
	}
	else
	{
		pFace = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_apply_list_item_face_woman);
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

	Label* pLabName = Label::create();
	pBG->addChild(pLabName);
	pLabName->setPosition(Vec2(sBGSize.width * 0.31, sBGSize.height * 0.5));
	pLabName->setSystemFontSize(25);
	pLabName->setColor(Color3B::WHITE);
	pLabName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabName->setString(sName);

	//	财富
	char cBufGold[128];
	sprintf(cBufGold, "%.2f", sBankerData.lScore);
	Label* pLabGold = Label::create();
	pBG->addChild(pLabGold);
	pLabGold->setPosition(Vec2(sBGSize.width * 0.71, sBGSize.height * 0.5));
	pLabGold->setTag(sBankerData.wChairID);
	pLabGold->setSystemFontSize(25);
	pLabGold->setColor(Color3B::WHITE);
	pLabGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pLabGold->setString(cBufGold);

	//	储存到子项数组中
	m_mItemArray[sBankerData.wChairID] = pBG;
}

Widget* PPCPopupApplyList::getListItem(int nChairID)
{
	return dynamic_cast<Widget*>(m_mItemArray[nChairID]);
}

void PPCPopupApplyList::removeListItem(int nChairID)
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

void PPCPopupApplyList::removeAllListItem()
{
	m_pListView->removeAllItems();
	m_mItemArray.clear();
}

bool PPCPopupApplyList::isItemExist(int nChairID)
{
	if (m_mItemArray[nChairID])
	{
		return true;
	}

	return false;
}

void PPCPopupApplyList::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	m_pBG = Sprite::create(PPCTextureName::s_ppc_popup_apply_list_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize.width * 0.5, sVSize.height * 0.5));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pClosed = Button::create(
		PPCTextureName::s_ppc_btn_closed_normal,
		PPCTextureName::s_ppc_btn_closed_selected,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pClosed);
	m_pClosed->setPosition(Vec2(sBGSize.width * 0.95, sBGSize.height * 0.9));
	m_pClosed->addClickEventListener(CC_CALLBACK_1(PPCPopupApplyList::onClickBtnClosed, this));

	m_pListView = ListView::create();
	m_pBG->addChild(m_pListView);
	m_pListView->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.4));
	m_pListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pListView->setDirection(ScrollView::Direction::VERTICAL);
	m_pListView->setContentSize(PPC_APPLY_LIST_VIEW_SIZE);
	m_pListView->setScrollBarEnabled(false);
}

void PPCPopupApplyList::onClickBtnClosed(Ref* pSender)
{
	this->setVisible(false);
}
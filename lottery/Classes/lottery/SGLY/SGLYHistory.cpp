#include "SGLYHistory.h"
#include "SGLYDefine.h"
#include "SGLYRes.h"
#include "ToolKit.h"

SGLYHistory::SGLYHistory()
{
}

SGLYHistory::~SGLYHistory()
{
}

void SGLYHistory::addHistoryItem(int nLotteryIdx)
{
	int nItemCount = getHistoryItemCount();
	if (nItemCount >= SGLY_HISTORY_COUNT_MAX)
	{
		m_pListView->removeItem(0);
	}

	ImageView* pBG = ImageView::create(SGLYTextureName::sSGLY_HistoryItemBG, TextureResType::PLIST);
	m_pListView->pushBackCustomItem(pBG);
	m_pListView->jumpToPercentHorizontal(100);

	if (nLotteryIdx < 0 ||nLotteryIdx >= SGLY_LOTTERY_COUNT)
	{
		return;
	}
	char cTextureName[128];
	sprintf(cTextureName, SGLY_HISTORY_ITEM_FORMAT, nLotteryIdx);
	Sprite* pIcon = Sprite::createWithSpriteFrameName(cTextureName);
	pBG->addChild(pIcon);
	pIcon->setPosition(Vec2(pBG->getContentSize() / 2));
}

bool SGLYHistory::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void SGLYHistory::initUI()
{
	m_pListView = ListView::create();
	addChild(m_pListView);
	m_pListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pListView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_pListView->setContentSize(SGLY_HISTORY_LIST_VIEW_SIZE);
	m_pListView->setScrollBarEnabled(false);
	setContentSize(m_pListView->getContentSize());
}

int SGLYHistory::getHistoryItemCount()
{
	if (!m_pListView)
	{
		return -1;
	}
	int num = m_pListView->getItems().size();
	return num;
}

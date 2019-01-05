#include "PPCTravel.h"
#include "PPCCarLogo.h"

PPCTravel::PPCTravel()
{
}

PPCTravel::~PPCTravel()
{
}

void PPCTravel::addTravelItem(EPPCCarLogoType eType)
{
	if (eType >= EPPCCarLogo_Null)
	{
		return;
	}
	int nItemCount = getTraveItemCount();
	if (nItemCount >= PPC_MAX_SCORE_HISTORY)
	{
		m_pListView->removeItem(0);
	}
	Sprite* pLogo = nullptr;
	switch (eType)
	{
	case EPPCCarLogo_Porsche_Big:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_porsche);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		break;
	case EPPCCarLogo_Porsche_Small:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_porsche);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		break;
	case EPPCCarLogo_BMW_Big:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_bmw);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		break;
	case EPPCCarLogo_BMW_Small:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_bmw);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		break;
	case EPPCCarLogo_Benz_Big:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_benz);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		break;
	case EPPCCarLogo_Benz_Small:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_benz);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		break;
	case EPPCCarLogo_VW_Big:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_vw);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		break;
	case EPPCCarLogo_VW_Small:
		pLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_vw);
		pLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		break;
	case EPPCCarLogo_Null:
		break;
	default:
		break;
	}
	ImageView* pBG = ImageView::create(PPCTextureName::s_ppc_travel_item_bg, TextureResType::PLIST);
	pBG->addChild(pLogo);
	pLogo->setPosition(Vec2(pBG->getContentSize() / 2));
	m_pListView->pushBackCustomItem(pBG);
	m_pListView->jumpToPercentHorizontal(100);
}

bool PPCTravel::init()
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

void PPCTravel::initUI()
{
	m_pListView = ListView::create();
	addChild(m_pListView);
	m_pListView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pListView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_pListView->setContentSize(PPC_TRAVEL_LIST_VIEW_SIZE);
	m_pListView->setScrollBarEnabled(false);
}

int PPCTravel::getTraveItemCount()
{
	if (!m_pListView)
	{
		return -1;
	}
	int num = m_pListView->getItems().size();
	return num;
}

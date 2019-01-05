#include "PPCTableArea.h"
#include "ToolKit.h"

PPCTableArea::PPCTableArea():
	m_llChipValueSum(0),
	m_llChipValueSelf(0)
{
	m_pIcon = nullptr;
	m_pLight = nullptr;
}

PPCTableArea::~PPCTableArea()
{
}

void PPCTableArea::resetTableArea()
{
	//	清除数据

	m_llChipValueSum = 0;
	m_llChipValueSelf = 0;

	m_pLabChipValueSum->setString("");
	m_pLabChipValueSelf->setString("");

	//	关闭区域高亮

	m_pLight->setVisible(false);

	//	清除桌面筹码

	for (auto k : m_vVesselChip)
	{
		Node* pChip = dynamic_cast<Node*>(k);
		if (pChip)
		{
			pChip->removeFromParent();
		}
	}
	m_vVesselChip.clear();

}

void PPCTableArea::initArea(EPPCCarLogoType eArea)
{
	switch (eArea)
	{
	case EPPCCarLogo_Porsche_Big:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_porsche);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.05, 0));

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_top_left);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.04, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.04, PPC_AREA_SIZE.height * -0.38));

		break;
	case EPPCCarLogo_Porsche_Small:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_porsche);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.05, 0));
		m_pIcon->setScale(PPC_AREA_ICON_SCALE_SMALL);

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_bottom_left);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.04, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.04, PPC_AREA_SIZE.height * -0.38));
		break;
	case EPPCCarLogo_BMW_Big:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_bmw);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.1, 0));

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_top_middle);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * -0.38));

		break;
	case EPPCCarLogo_BMW_Small:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_bmw);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.1, 0));
		m_pIcon->setScale(PPC_AREA_ICON_SCALE_SMALL);

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_bottom);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * -0.38));

		break;
	case EPPCCarLogo_Benz_Big:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_benz);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.1, 0));

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_top_middle);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * -0.38));
		break;
	case EPPCCarLogo_Benz_Small:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_benz);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.1, 0));
		m_pIcon->setScale(PPC_AREA_ICON_SCALE_SMALL);

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_bottom);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.09, PPC_AREA_SIZE.height * -0.38));

		break;
	case EPPCCarLogo_VW_Big:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_vw);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.15, 0));

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_top_right);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.15, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.15, PPC_AREA_SIZE.height * -0.38));

		break;
	case EPPCCarLogo_VW_Small:
		m_pIcon = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_vw);
		m_pIcon->setPosition(Vec2(PPC_AREA_SIZE.width * -0.15, 0));
		m_pIcon->setScale(PPC_AREA_ICON_SCALE_SMALL);

		m_pLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_area_light_bottom_right);

		m_pLabChipValueSum = Label::create();
		m_pLabChipValueSum->setPosition(Vec2(PPC_AREA_SIZE.width * -0.15, PPC_AREA_SIZE.height * 0.38));

		m_pLabChipValueSelf = Label::create();
		m_pLabChipValueSelf->setPosition(Vec2(PPC_AREA_SIZE.width * -0.15, PPC_AREA_SIZE.height * -0.38));

		break;
	case EPPCCarLogo_Null:
		return;
	default:
		return;
	}
	addChild(m_pIcon);
	addChild(m_pLight);
	this->setContentSize(PPC_AREA_SIZE);
	
	int nFontSize = 23;

	addChild(m_pLabChipValueSum);
	m_pLabChipValueSum->setSystemFontSize(nFontSize);
	m_pLabChipValueSum->setColor(Color3B(226, 125, 33));

	addChild(m_pLabChipValueSelf);
	m_pLabChipValueSelf->setSystemFontSize(nFontSize);
	m_pLabChipValueSelf->setColor(Color3B(21, 156, 49));

/*
	m_pLabChipValueSum->setString("9999");
	m_pLabChipValueSelf->setString("9000");
	vector<Node*> vList;
	vList.push_back(m_pLabChipValueSum);
	vList.push_back(m_pLabChipValueSelf);
	ToolKit::studioUIMulti(this, vList);
*/

	m_eAreaType = eArea;

	setAreaHighLight(false);

	float fOriginX = this->getPosition().x - PPC_AREA_SIZE.width / 2;
	float fOriginY = this->getPosition().y - PPC_AREA_SIZE.height / 2;
	m_rAreaRect = Rect(Vec2(fOriginX, fOriginY), PPC_AREA_SIZE);
	auto rRect = getBoundingBox();
}

bool PPCTableArea::isContainsPoint(Vec2 vPos)
{
	return m_rAreaRect.containsPoint(vPos);
}

Sprite* PPCTableArea::addTableChip(EPPCChipType eType)
{
	string sBufChipNormal[EPPCChip_Null] = 
	{
		PPCTextureName::s_ppc_chip_normal_1,
		PPCTextureName::s_ppc_chip_normal_5,
		PPCTextureName::s_ppc_chip_normal_10,
		PPCTextureName::s_ppc_chip_normal_100,
		PPCTextureName::s_ppc_chip_normal_500,
		PPCTextureName::s_ppc_chip_normal_1k,
	};
	Sprite* pChip = Sprite::createWithSpriteFrameName(sBufChipNormal[eType]);
	this->addChild(pChip);
	pChip->setScale(PPC_AREA_CHIP_SCALE_SMALL);

	//	由于this的位置在区域中心，随机的范围需要适配
// 	Rect rRect = getBoundingBox();
	Rect rRect = m_rAreaRect;
	float fXScale = 0.1f;
	float fYScale = 0.2f;
	float fX = random(rRect.size.width * -fXScale, rRect.size.width * fXScale);
	float fY = random(rRect.size.height * -fYScale, rRect.size.height * fYScale);
	pChip->setPosition(Vec2(fX, fY));
	m_vVesselChip.push_back(pChip);
	return pChip;
}

void PPCTableArea::setAreaHighLight(bool isHighLight)
{
	m_pLight->setVisible(isHighLight);
	if (isHighLight)
	{
		auto pBlink = Blink::create(2.0f, 3);
		m_pLight->runAction(pBlink);
	}
}

//	设置筹码总注文本数值
void PPCTableArea::setLabChipValueSum(LONGLONG llValue)
{
	char cBufValue[128];
	sprintf(cBufValue, "%lld",llValue);
	m_pLabChipValueSum->setString(cBufValue);
// 	m_llChipValueSum += llValue;
}

//	设置筹码个注文本数值
void PPCTableArea::setLabChipValueSelf(LONGLONG llValue)
{
	char cBufValue[128];
	sprintf(cBufValue, "%lld",llValue);
	m_pLabChipValueSelf->setString(cBufValue);
// 	m_llChipValueSelf += llValue;
}

bool PPCTableArea::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initData();

		bRet = true;
	} while (0);

	return bRet;
}

void PPCTableArea::initData()
{
	m_rAreaRect = Rect();
	m_eAreaType = EPPCCarLogo_Null;
	m_llChipValueSum = 0;
	m_llChipValueSelf = 0;
}

#include "PPCCarLogo.h"
#include "ToolKit.h"

PPCCarLogo::PPCCarLogo()
{
}

PPCCarLogo::~PPCCarLogo()
{
}

void PPCCarLogo::initCarLogoWithType(EPPCCarLogoType eType)
{
	if (eType >= EPPCCarLogo_Null)
	{
		return;
	}
	switch (eType)
	{
	case EPPCCarLogo_Porsche_Big:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_porsche);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_big_bg);
		break;
	case EPPCCarLogo_Porsche_Small:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_porsche);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_small_bg);
		break;
	case EPPCCarLogo_Benz_Big:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_benz);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_big_bg);
		break;
	case EPPCCarLogo_Benz_Small:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_benz);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_small_bg);
		break;
	case EPPCCarLogo_BMW_Big:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_bmw);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_big_bg);
		break;
	case EPPCCarLogo_BMW_Small:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_bmw);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_small_bg);
		break;
	case EPPCCarLogo_VW_Big:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_vw);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_BIG);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_big_bg);
		break;
	case EPPCCarLogo_VW_Small:
		m_pImgCarLogo = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_vw);
		m_pImgCarLogo->setScale(PPC_CAR_LOGO_SCALE_SMALL);
		m_pImgBG = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_small_bg);
		break;
	case EPPCCarLogo_Null:
		return;
	default:
		return;
	}
	//	汽车logo背景
	addChild(m_pImgBG);

	cocos2d::Size sBGSize = m_pImgBG->getContentSize();

	//	汽车logo
	m_pImgBG->addChild(m_pImgCarLogo);
	m_pImgCarLogo->setPosition(Vec2(sBGSize / 2));
	this->setContentSize(sBGSize);

	//	汽车logo半透明遮罩
	m_pImgMask = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_mask);
	m_pImgBG->addChild(m_pImgMask);
	m_pImgMask->setPosition(Vec2(sBGSize / 2));

	//	汽车logo高亮光环
	m_pHighLight = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_logo_high_light);
	m_pImgBG->addChild(m_pHighLight);
	m_pHighLight->setPosition(Vec2(sBGSize / 2));
	m_pHighLight->setVisible(false);

	//	汽车
	m_pImgCar = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_icon_car);
	m_pImgBG->addChild(m_pImgCar);
	m_pImgCar->setVisible(false);

	m_eCarLogoType = eType;

	autoPosition();

/*
	//	logo索引，测试用

	Label* pTestNum = Label::create();
	m_pImgBG->addChild(pTestNum);
	pTestNum->setPosition(Vec2(sBGSize / 2));
	pTestNum->setSystemFontSize(30);
	pTestNum->setColor(Color3B::WHITE);
	pTestNum->setString(Value(m_nLogoID).asString());
*/

}

void PPCCarLogo::resetPPCCarLogo()
{
	m_pHighLight->setVisible(false);
	m_pImgCar->setVisible(false);
}

void PPCCarLogo::setHighLight(bool bIsHighLight)
{
	m_pHighLight->setVisible(bIsHighLight);
}

void PPCCarLogo::setCarVisible(bool bIsCarVisible)
{
	m_pImgCar->setVisible(bIsCarVisible);
}

EPPCCarLogoType PPCCarLogo::getCarLogoType()
{
	return m_eCarLogoType;
}

bool PPCCarLogo::init()
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

void PPCCarLogo::initData()
{
	m_eCarLogoType = EPPCCarLogo_Null;
	setLogoID(-1);
	m_pImgBG = nullptr;
	m_pImgCarLogo = nullptr;
	m_pImgMask = nullptr;
	m_pHighLight = nullptr;
	m_pImgCar = nullptr;
}

void PPCCarLogo::autoPosition()
{
	cocos2d::Size sBGSize = m_pImgBG->getContentSize();
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	Vec2 vCarPos;
	Vec2 vLogoPos;
	float fRadian = 0;
	switch (m_nLogoID)
	{
	case 0:
		vLogoPos = Vec2(sVSize.width * 0.166, sVSize.height * 0.882);
		vCarPos = Vec2(sBGSize.width * 0.65, sBGSize.height * -0.145);
		fRadian = -25;
		break;
	case 1:
		vLogoPos = Vec2(sVSize.width * 0.232, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 2:
		vLogoPos = Vec2(sVSize.width * 0.298, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 3:
		vLogoPos = Vec2(sVSize.width * 0.365, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 4:
		vLogoPos = Vec2(sVSize.width * 0.430, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 5:
		vLogoPos = Vec2(sVSize.width * 0.497, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 6:
		vLogoPos = Vec2(sVSize.width * 0.563, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 7:
		vLogoPos = Vec2(sVSize.width * 0.63, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 8:
		vLogoPos = Vec2(sVSize.width * 0.696, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 9:
		vLogoPos = Vec2(sVSize.width * 0.762, sVSize.height * 0.9);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * -0.15);
		break;
	case 10:
		vLogoPos = Vec2(sVSize.width * 0.827, sVSize.height * 0.884);
		vCarPos = Vec2(sBGSize.width * 0.35, sBGSize.height * -0.145);
		fRadian = -330;
		break;
	case 11:
		vLogoPos = Vec2(sVSize.width * 0.882, sVSize.height * 0.824);
		vCarPos = Vec2(sBGSize.width * 0, sBGSize.height * 0.05);
		fRadian = -310;
		break;
	case 12:
		vLogoPos = Vec2(sVSize.width * 0.919, sVSize.height * 0.729);
		vCarPos = Vec2(sBGSize.width * -0.2, sBGSize.height * 0.35);
		fRadian = -290;
		break;
	case 13:
		vLogoPos = Vec2(sVSize.width * 0.933, sVSize.height * 0.619);
		vCarPos = Vec2(sBGSize.width * -0.16, sBGSize.height * 0.5);
		fRadian = -270;
		break;
	case 14:
		vLogoPos = Vec2(sVSize.width * 0.920, sVSize.height * 0.508);
		vCarPos = Vec2(sBGSize.width * -0.2, sBGSize.height * 0.65);
		fRadian = -240;
		break;
	case 15:
		vLogoPos = Vec2(sVSize.width * 0.883, sVSize.height * 0.414);
		vCarPos = Vec2(sBGSize.width * 0, sBGSize.height * 1);
		fRadian = -220;
		break;
	case 16:
		vLogoPos = Vec2(sVSize.width * 0.827, sVSize.height * 0.354);
		vCarPos = Vec2(sBGSize.width * 0.3, sBGSize.height * 1.13);
		fRadian = -200;
		break;
	case 17:
		vLogoPos = Vec2(sVSize.width * 0.761, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 18:
		vLogoPos = Vec2(sVSize.width * 0.695, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 19:
		vLogoPos = Vec2(sVSize.width * 0.63, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 20:
		vLogoPos = Vec2(sVSize.width * 0.563, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 21:
		vLogoPos = Vec2(sVSize.width * 0.497, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 22:
		vLogoPos = Vec2(sVSize.width * 0.431, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 23:
		vLogoPos = Vec2(sVSize.width * 0.365, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 24:
		vLogoPos = Vec2(sVSize.width * 0.298, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 25:
		vLogoPos = Vec2(sVSize.width * 0.232, sVSize.height * 0.336);
		vCarPos = Vec2(sBGSize.width * 0.5, sBGSize.height * 1.16);
		fRadian = -180;
		break;
	case 26:
		vLogoPos = Vec2(sVSize.width * 0.167, sVSize.height * 0.354);
		vCarPos = Vec2(sBGSize.width * 0.7, sBGSize.height * 1.15);
		fRadian = -160;
		break;
	case 27:
		vLogoPos = Vec2(sVSize.width * 0.111, sVSize.height * 0.414);
		vCarPos = Vec2(sBGSize.width * 1, sBGSize.height * 1);
		fRadian = -140;
		break;
	case 28:
		vLogoPos = Vec2(sVSize.width * 0.074, sVSize.height * 0.509);
		vCarPos = Vec2(sBGSize.width * 1.2, sBGSize.height * 0.65);
		fRadian = -120;
		break;
	case 29:
		vLogoPos = Vec2(sVSize.width * 0.063, sVSize.height * 0.619);
		vCarPos = Vec2(sBGSize.width * 1.16, sBGSize.height * 0.5);
		fRadian = -90;
		break;
	case 30:
		vLogoPos = Vec2(sVSize.width * 0.0746, sVSize.height * 0.729);
		vCarPos = Vec2(sBGSize.width * 1.2, sBGSize.height * 0.35);
		fRadian = -60;
		break;
	case 31:
		vLogoPos = Vec2(sVSize.width * 0.112, sVSize.height * 0.822);
		vCarPos = Vec2(sBGSize.width * 1, sBGSize.height * 0.05);
		fRadian = -40;	
		break;
	default:
		break;
	}
	this->setPosition(vLogoPos);
	m_pImgCar->setPosition(vCarPos);
	m_pImgCar->setRotation(fRadian);
}

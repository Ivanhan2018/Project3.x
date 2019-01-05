#include "PPCEnding.h"
#include "ToolKit.h"
#include "MyConfig.h"//by hxh

PPCEnding::PPCEnding()
{
}

PPCEnding::~PPCEnding()
{
}

bool PPCEnding::init()
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

void PPCEnding::initUI()
{
	MyConfig& myConfig=MyConfig::Instance();
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	int nFontSize = 30;
	m_pBG = Sprite::create(PPCTextureName::s_ppc_popup_account_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize / 2));

	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pLabPlayerScore = Label::create();
	m_pBG->addChild(m_pLabPlayerScore);
	m_pLabPlayerScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabPlayerScore->setPosition(Vec2(sBGSize.width * myConfig.m_EndingPos[0][0], sBGSize.height * myConfig.m_EndingPos[0][1]));
	m_pLabPlayerScore->setSystemFontSize(nFontSize);

	m_pLabPlayerCapital = Label::create();
	m_pBG->addChild(m_pLabPlayerCapital);
	m_pLabPlayerCapital->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabPlayerCapital->setPosition(Vec2(sBGSize.width * myConfig.m_EndingPos[1][0], sBGSize.height * myConfig.m_EndingPos[1][1]));
	m_pLabPlayerCapital->setSystemFontSize(nFontSize);

	m_pLabBankerScore = Label::create();
	m_pBG->addChild(m_pLabBankerScore);
	m_pLabBankerScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabBankerScore->setPosition(Vec2(sBGSize.width * myConfig.m_EndingPos[2][0], sBGSize.height * myConfig.m_EndingPos[2][1]));
	m_pLabBankerScore->setSystemFontSize(nFontSize);

	m_pLabBankerCapital = Label::create();
	m_pBG->addChild(m_pLabBankerCapital);
	m_pLabBankerCapital->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pLabBankerCapital->setPosition(Vec2(sBGSize.width * myConfig.m_EndingPos[3][0], sBGSize.height * myConfig.m_EndingPos[3][1]));
	m_pLabBankerCapital->setSystemFontSize(nFontSize);
}

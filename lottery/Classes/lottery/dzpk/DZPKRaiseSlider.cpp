#include "DZPKRaiseSlider.h"
#include "DDZRes.h"
#include "DZPKTools.h"
#include "DZPKLayer.h"

using namespace ui;

#define DZPK_RAISE_SLIDER_MAX_PERCENT 10000	//	加注滑动条最大比值

DZPKRaiseSlider::DZPKRaiseSlider()
{
	m_pSliderBG = nullptr;
	m_pTextAllin = nullptr;
	m_pLabNum = nullptr;
	m_pSlider = nullptr;
	m_llRaiseValue = 0;
	m_llMinRaiseValue = 0;
}

DZPKRaiseSlider::~DZPKRaiseSlider()
{

}

bool DZPKRaiseSlider::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initUI();
		resetRaiseSlider();

		bRet = true;
	} while (0);

	return bRet;
}

void DZPKRaiseSlider::initUI()
{
	m_pSliderBG = Sprite::createWithSpriteFrameName(texture_name::s_dzpk_raise_slider_floor);
	addChild(m_pSliderBG);
	cocos2d::Size sSBGSize = m_pSliderBG->getContentSize();

	m_pSlider = Slider::create();
	m_pSliderBG->addChild(m_pSlider);
	m_pSlider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pSlider->setRotation(-90);
	m_pSlider->setPosition(Vec2(sSBGSize.width * 0.5, sSBGSize.height * 0.13));
	m_pSlider->loadBarTexture(texture_name::s_dzpk_raise_slider_bg, Widget::TextureResType::PLIST);
	m_pSlider->loadProgressBarTexture(texture_name::s_dzpk_raise_slider_progress, Widget::TextureResType::PLIST);
	m_pSlider->loadSlidBallTextures(texture_name::s_dzpk_raise_slider_thumb,
		texture_name::s_dzpk_raise_slider_thumb, "", Widget::TextureResType::PLIST);
	m_pSlider->addEventListenerSlider(this, sliderpercentchangedselector(DZPKRaiseSlider::onChangeSlider));
	m_pSlider->setMaxPercent(DZPK_RAISE_SLIDER_MAX_PERCENT);
	m_pTextAllin = Sprite::createWithSpriteFrameName(texture_name::s_dzpk_raise_slider_allin);
	m_pSliderBG->addChild(m_pTextAllin);
	m_pTextAllin->setPosition(Vec2(sSBGSize.width * 0.5, sSBGSize.height * 0.9));
	
	m_pLabNum = Label::create();
	m_pSliderBG->addChild(m_pLabNum);
	m_pLabNum->setPosition(Vec2(sSBGSize.width * 0.5, sSBGSize.height * 0.9));
	m_pLabNum->setSystemFontSize(25);
	m_pLabNum->setColor(Color3B::WHITE);
}

//	重置加注滑动条
void DZPKRaiseSlider::resetRaiseSlider()
{
	m_llRaiseValue = m_llMinRaiseValue;
	setLabNum(m_llRaiseValue);
	m_pSlider->setPercent(0);
	m_pTextAllin->setVisible(false);
}

//	显示Allin
void DZPKRaiseSlider::showTextAllin()
{
	m_pTextAllin->setVisible(true);
	m_pLabNum->setVisible(false);
}

//	设置滑动条数值
void DZPKRaiseSlider::setLabNum(LONGLONG llValue)
{
	m_pTextAllin->setVisible(false);
	m_pLabNum->setVisible(true);
	m_llRaiseValue = llValue;
	if (llValue >= 10000)
	{
		char szBuf[128] = ""; 
		double  jsdbhfvs=  (double)llValue / 10000;
		sprintf(szBuf, "%0.2f%s", jsdbhfvs, DZPK_loadText("t10"));
		m_pLabNum->setString(szBuf);
	}
	else
	{
		m_pLabNum->setString(Value((int)llValue).asString());
	}
}

LONGLONG DZPKRaiseSlider::getRaiseValue() const
{
	return m_llRaiseValue;
}

void DZPKRaiseSlider::onChangeSlider(Ref* sender, ui::Slider::EventType eEventTyp)
{
	//	只处理【持有筹码数】 >= 【最小下注值m_llMinRaiseValue】的表现
	if (eEventTyp == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Scene* pCurrScene = Director::getInstance()->getRunningScene();
		DZPKLayer* pDZPKLayer = dynamic_cast<DZPKLayer*>(pCurrScene->getChildByTag(DZPKLayerTag));
		DZPKOperaLayer* pOpera = pDZPKLayer->m_pOperaLayer;

		//	持有筹码数
		LONGLONG llMaxRaiseValue = pDZPKLayer->m_mPlayerList[EDZPKPlayerViewID_3]->getPlayerGold();

		//	最小加注值
		LONGLONG llMinRaiseValue = m_llMinRaiseValue;

		//	滑动条百分比
		float fPercent = (float)m_pSlider->getPercent() / DZPK_RAISE_SLIDER_MAX_PERCENT;

		//	最大进度加注值
		LONGLONG llMaxProgressValue = 0;

		//	实际加注值
		LONGLONG llRaiseValue = 0;

		if (llMaxRaiseValue < llMinRaiseValue)
		{
			return;
		}
		llMaxProgressValue = llMaxRaiseValue - llMinRaiseValue;
		llRaiseValue = LONGLONG(llMaxProgressValue * fPercent) + llMinRaiseValue;

		if (llRaiseValue >= llMaxRaiseValue)
		{
			llRaiseValue = llMaxRaiseValue;
			m_llRaiseValue = llRaiseValue;
			showTextAllin();
			return;
		}
		else
		{
			setLabNum(llRaiseValue);
		}

		log("fPercent = %0.2f \n llMinRaiseValue = %ld \n llMaxRaiseValue = %ld \n llMaxProgressValue = %ld \n llRaiseValue = %ld",
			fPercent,
			llMinRaiseValue,
			llMaxRaiseValue,
			llMaxProgressValue,
			llRaiseValue);
	}
}
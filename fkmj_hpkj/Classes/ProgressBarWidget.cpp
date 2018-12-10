#include "ProgressBarWidget.h"
#include "UtilityBase.h"
#include "WidgetFun.h"
USING_NS_CC;

ProgressBarWidget::ProgressBarWidget(std::string kBackImg,std::string kBarImg)
	:m_pBgImage(WidgetFun::createSprite(kBackImg.c_str()))
	,m_pBarImage(WidgetFun::createSprite(kBarImg.c_str()))
	,m_pText(NULL)
{
	init();
}

bool ProgressBarWidget::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_pText = CCLabelTTF::create("1","",20);
	m_pBarImage->setAnchorPoint(ccp(0,0.5f));
	m_pBarImage->setPositionX(-m_pBgImage->getContentSize().width/2);
	addChild(m_pBgImage);
	addChild(m_pBarImage);
	addChild(m_pText);

	return true;
}

void ProgressBarWidget::setProgressWithData( float fCurrent, float fMax )
{
	if (fMax <= 0.0f)
	{
		fMax = 1.0f;
	}
	float fPres = fCurrent/fMax;
	Size sz = m_pBgImage->getContentSize();
	sz.width = sz.width*fPres;
	m_pBarImage->setTextureRect(CCRectMake(0,0,sz.width,sz.height));

	if (m_pText)
	{
		std::string text = utility::toString(fCurrent,"/",fMax);
		m_pText->setString(text.c_str());
	}
}

void ProgressBarWidget::setFontSize( float fFontSize )
{
	if (fFontSize<=0)
	{
		m_pText->setVisible(false);
	}
	else
	{
		m_pText->setFontSize(fFontSize);
	}
}

void ProgressBarWidget::setTextColor( const cocos2d::Color3B& kColor )
{
	if (m_pText)
	{
		m_pText->setColor(kColor);
	}	
}

ProgressBarWidget::~ProgressBarWidget()
{
}




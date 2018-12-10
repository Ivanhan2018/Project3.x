#include "QYLabel.h"
#include "utility.h"
#include "WidgetFun.h"
#include "ActionEx.h"

USING_NS_CC;

QYLabel::QYLabel()
		:m_pUp(NULL)
		,m_pDown(NULL)
		,m_pLeft(NULL)
		,m_pRight(NULL)
		,m_iLineWidth(0)
{

}
QYLabel* QYLabel::create(const std::string& text, const std::string& font, float fontSize,
								   const Size& dimensions, TextHAlignment hAlignment,
								   TextVAlignment vAlignment)
{
	QYLabel *ret = new (std::nothrow) QYLabel();
	if(ret && ret->initWithString(text, font, fontSize, dimensions, hAlignment, vAlignment))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
void QYLabel::QYEnabelOutLine(const Color4B& color,int iLineWidth)
{
	setCascadeColorEnabled(false);
	m_iLineWidth = iLineWidth;
	if (m_pUp)
	{
		m_pUp->removeFromParent();
	}
	if (m_pDown)
	{
		m_pDown->removeFromParent();
	}
	if (m_pLeft)
	{
		m_pLeft->removeFromParent();
	}
	if (m_pRight)
	{
		m_pRight->removeFromParent();
	}

	m_pUp = LabelTTF::create(getString(),getFontName(),getFontSize());  
	m_pUp->setColor(ccc3(color.r,color.g,color.b));   
	this->addChild(m_pUp,-1);  


	m_pDown = LabelTTF::create(getString(),getFontName(),getFontSize());  
	m_pDown->setColor(ccc3(color.r,color.g,color.b));   
	this->addChild(m_pDown,-1);  


	m_pLeft = LabelTTF::create(getString(),getFontName(),getFontSize());  
	m_pLeft->setColor(ccc3(color.r,color.g,color.b));  
	this->addChild(m_pLeft,-1);  


	m_pRight = LabelTTF::create(getString(),getFontName(),getFontSize());  
	m_pRight->setColor(ccc3(color.r,color.g,color.b));  
	this->addChild(m_pRight,-1);  

	upOutLinePos();
}
void QYLabel::upOutLinePos()
{
	if (m_pUp)
	{
		m_pUp->setPosition(ccp(getContentSize().width*0.5, getContentSize().height*0.5+m_iLineWidth));  
	}
	if (m_pDown)
	{
		m_pDown->setPosition(ccp(getContentSize().width*0.5, getContentSize().height*0.5-m_iLineWidth));  
	}
	if (m_pLeft)
	{
		m_pLeft->setPosition(ccp(getContentSize().width*0.5+m_iLineWidth, getContentSize().height*0.5));  
	}
	if (m_pRight)
	{
		m_pRight->setPosition(ccp(getContentSize().width*0.5-m_iLineWidth, getContentSize().height*0.5));  
	}
}

void QYLabel::setString(const std::string& text)
{
	LabelTTF::setString(text);

	if (m_pUp)
	{
		m_pUp->setString(text);
	}
	if (m_pDown)
	{
		m_pDown->setString(text);
	}
	if (m_pLeft)
	{
		m_pLeft->setString(text);
	}
	if (m_pRight)
	{
		m_pRight->setString(text);
	}
	upOutLinePos();
}
void QYLabel::setSystemFontSize(float fontSize)
{
	LabelTTF::setFontSize(fontSize);

	if (m_pUp)
	{
		m_pUp->setFontSize(fontSize);
	}
	if (m_pDown)
	{
		m_pDown->setFontSize(fontSize);
	}
	if (m_pLeft)
	{
		m_pLeft->setFontSize(fontSize);
	}
	if (m_pRight)
	{
		m_pRight->setFontSize(fontSize);
	}
	upOutLinePos();
}
void QYLabel::setDimensions(const Size &dim)
{
	LabelTTF::setDimensions(dim);

	if (m_pUp)
	{
		m_pUp->setDimensions(dim);
	}
	if (m_pDown)
	{
		m_pDown->setDimensions(dim);
	}
	if (m_pLeft)
	{
		m_pLeft->setDimensions(dim);
	}
	if (m_pRight)
	{
		m_pRight->setDimensions(dim);
	}
	upOutLinePos();
}
void QYLabel::setColor(const Color3B& color)
{
	cocos2d::LabelTTF::setColor(color);
	_renderLabel->setColor(color);
}
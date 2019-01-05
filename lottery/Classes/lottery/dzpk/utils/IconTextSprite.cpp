#include "IconTextSprite.h"
#include "PlistReader.h"

IconTextSprite::IconTextSprite():
pIcon(NULL),
pTextTTF(NULL),
textColor(ccc3(255,255,255)),
margin(5)
{

}


IconTextSprite::~IconTextSprite()
{
	CC_SAFE_RELEASE_NULL(pIcon);
	CC_SAFE_RELEASE_NULL(pTextTTF);
}

IconTextSprite* IconTextSprite::createWith(CCDictionary* plistFrame,CCTexture2D* pTexture,const std::string iconName,const std::string text)
{
	IconTextSprite *pRet = new IconTextSprite();
	if (pRet && pRet->init() && pRet->initUI(plistFrame,pTexture,iconName,text))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

IconTextSprite* IconTextSprite::createWith(CCSprite* pIcon,const char* text)
{
	IconTextSprite *pRet = new IconTextSprite();
	if (pRet && pRet->init() && pRet->initUI(pIcon,text))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

IconTextSprite* IconTextSprite::createWith(const char* icon,const char* text)
{
	CCSprite* pIcon = CCSprite::create(icon);
	return createWith(pIcon,text);
}

bool IconTextSprite::init()
{
	return true;
}

bool IconTextSprite::initUI(CCDictionary* plistFrame,CCTexture2D* pTexture,const std::string iconName,const std::string text)
{
	if (NULL==plistFrame || iconName=="")
	{
		return false;
	}

	PlistReader* pRader = PlistReader::getInstance();

	pIcon = CCSprite::createWithSpriteFrame(pRader->getSpriteFrame(plistFrame,pTexture,iconName.c_str()));
	pTextTTF = CCLabelTTF::create(text.c_str(),"Arial",15);
	CC_SAFE_RETAIN(pIcon);
	CC_SAFE_RETAIN(pTextTTF);

	this->resetContentSize();

	pIcon->setAnchorPoint(Vec2::ZERO);
	pIcon->setPosition(Vec2::ZERO);
	this->addChild(pIcon);

	this->setTextPoint();
	this->addChild(pTextTTF);

	return true;
}

bool IconTextSprite::initUI(CCSprite* pIcon,const char* text)
{
	if (NULL==pIcon)
	{
		return false;
	}

	this->pIcon = pIcon;
	pTextTTF = CCLabelTTF::create(text,"Arial",15);
	CC_SAFE_RETAIN(this->pIcon);
	CC_SAFE_RETAIN(pTextTTF);

	this->resetContentSize();

	pIcon->setAnchorPoint(Vec2::ZERO);
	pIcon->setPosition(Vec2::ZERO);
	this->addChild(pIcon);

	this->setTextPoint();
	this->addChild(pTextTTF);

	return true;
}

void IconTextSprite::resetContentSize()
{
	CCSize iconSize = pIcon->getContentSize();
	CCSize textTTFSize = pTextTTF->getContentSize();
	CCSize nodeSize = CCSizeMake(iconSize.width+margin+textTTFSize.width,iconSize.height);
	this->setContentSize(nodeSize);
}

void IconTextSprite::setTextPoint()
{
	pTextTTF->setAnchorPoint(Vec2::ZERO);
	pTextTTF->setPosition(ccp(pIcon->boundingBox().size.width+margin,pIcon->boundingBox().size.height/2-pTextTTF->getContentSize().height/2));
}

void IconTextSprite::setText(const char* text)
{
	pTextTTF->setString(text);
	this->resetContentSize();
	this->setTextPoint();
}

void IconTextSprite::setFontSize(UNS_INT fontSize)
{
    pTextTTF->setFontSize(fontSize);
	this->resetContentSize();
	this->setTextPoint();
}

void IconTextSprite::setIconScale(float scale)
{
    pIcon->setScale(scale);
    
	this->resetContentSize();
	this->setTextPoint();
}

void IconTextSprite::setScale(float scale)
{
    pTextTTF->setScale(scale);
    this->resetContentSize();
	this->setTextPoint();
}

void IconTextSprite::setColor(ccColor3B textColor)
{
	this->textColor = textColor;
	pTextTTF->setColor(textColor);
}

void IconTextSprite::setMargin(UNS_INT margin)
{
    this->margin = margin;
	this->resetContentSize();
	this->setTextPoint();
}
#include "InputBoxSprite.h"

InputBoxSprite::InputBoxSprite()
{
	maxLength=6;
	color=Color3B::BLACK;
	placeHolder=":";
	text="";
	inputFlag=EditBox::InputFlag::SENSITIVE;
	inputMode=EditBox::InputMode::SINGLE_LINE;
	keyboardReturnType=EditBox::KeyboardReturnType::DONE;
	pEditBox=NULL;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	this->visibleW = visibleSize.width;
	this->visibleH = visibleSize.height;
	this->originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
}

InputBoxSprite::~InputBoxSprite()
{
    CC_SAFE_RELEASE_NULL(pEditBox);
}

InputBoxSprite* InputBoxSprite::createWith(CCPoint& absolutePoint,const char* inputFrame,const char* text)
{
    InputBoxSprite* pRet = new InputBoxSprite();
    if (pRet)
    {
        pRet->absolutePoint = absolutePoint;
        pRet->inputFrame = inputFrame;
        pRet->text = text;
    }
    if (pRet && pRet->init())
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

bool InputBoxSprite::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF( !Layer::init());
        this->initUI();
        bRet = true;
    }while(0);
    
    return bRet;
}

void InputBoxSprite::initUI()
{
    CCSize contentSize = CCSizeMake(this->visibleW,this->visibleH);
    this->setContentSize(contentSize);
    
    CCSprite* pFrameSprite = CCSprite::create(inputFrame);
    pFrameSprite->setAnchorPoint(Vec2::ZERO);
    pFrameSprite->setPosition(absolutePoint);
    this->addChild(pFrameSprite);
    
    EditBox* pEditBox = EditBox::create(CCSizeMake(0, 0), Scale9Sprite::create("popup/transparent_bg.png"));
    CC_SAFE_RETAIN(pEditBox);
    pEditBox->setPosition(absolutePoint);
    pEditBox->setFontColor(color);
    pEditBox->setText(text);
    pEditBox->setPlaceHolder(placeHolder);
    pEditBox->setMaxLength(maxLength);
    pEditBox->setInputFlag(inputFlag);
    pEditBox->setInputMode(inputMode);
    pEditBox->setReturnType(keyboardReturnType);
    //pEditBox->setTouchPriority(-128);
    this->addChild(pEditBox);
}

















//
//  TextField.cpp
//  ZJH
//
//  Created by luochuanting on 13-5-12.
//
//

#include "TextField.h"

using namespace std;

const static float DELTA = 0.5f;

static int _calcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}

TextField::TextField()
{
    //m_pInputText="";
	//m_ColorSpaceHolder = ccc3(213, 213, 213);
    m_pfnCallFunc2 = NULL;
    m_pobj = NULL;
    returnMode = kUIReturnKeyDone;
    CCTextFieldTTF();
}

TextField::~TextField()
{
    
}

void TextField::onEnter()
{
    CCTextFieldTTF::onEnter();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -256, false);
    this->setDelegate(this);
}

void TextField::onExit()
{
    CCTextFieldTTF::onExit();
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

TextField * TextField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize)
{
    TextField *pRet = new TextField();
    if(pRet && pRet->create(placeholder, fontName, fontSize,CCSizeMake(30, 100), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

TextField* TextField::textFieldWithPlaceHolder(const char *placeholder,ccColor3B placeholderColor, const char *fontName, float fontSize)
{
    TextField *pRet = new TextField();
    if(pRet && pRet->create(placeholder, fontName, fontSize,CCSizeMake(30, 100), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
            //pRet->m_ColorSpaceHolder = placeholderColor;
        }
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

CCRect TextField::getRect()
{
    CCSize size = getContentSize();

    return  CCRectMake(-size.width / 2, -size.height / 2, size.width, size.height);
}

bool TextField::isInTextField(cocos2d::CCTouch *pTouch)
{
    CCRect rect;
    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size = displaySize;
    return rect.containsPoint(convertTouchToNodeSpace(pTouch));
}

bool TextField::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    isInTextField(pTouch) ? openIME() : closeIME();
    return true;
}

void TextField::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

bool TextField::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    return false;
}

bool TextField::detachWithIME()
{
    CCTextFieldTTF::detachWithIME();
    return false;
}

void TextField::setEmptyString()
{
    //CCLabelTTF::setString("");
    m_pInputText = "";
    //m_nCharCount = _calcCharCount(m_pInputText.c_str());
}

bool TextField::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    return false;
}

bool TextField::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
{
    std::string sInsert(text, nLen);
    // insert \n means input end
    int nPos = sInsert.find('\n');
    if ((int)sInsert.npos != nPos)
    {
        this->editReturn();
        return false;
    }
    
    m_pInputText.append(text);

    int count = 0;
    int curL = 0;

    for(int i = 0; i < strlen(m_pInputText.c_str());i++)
    {
        if((m_pInputText.c_str()[i] >= 0 &&  m_pInputText.c_str()[i] <= 127))
        {
            count += 1;
            curL += 1;
            if(curL < maxLength * 2)
            {
                continue;
            }
            else
            {
                if(curL > maxLength * 2)
                {
                    count -= 1;
                }
                break;
            }
        }
        else
        {
            count += 3;
            curL += 2;
            if(curL < maxLength * 2)
            {
                i+=2;
                continue;
            }
            else
            {
                if(curL > maxLength * 2)
                {
                    count -= 3;
                }
                break;
            }
        }
    }
    
	char buf[1000/*count + 1*/]={0};
    snprintf(buf, sizeof(buf), "%s",m_pInputText.c_str());
    setString(buf);
    m_pInputText = buf;
    return true;
}

bool TextField::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
{
    if(m_pInputText.size() == 0)
    {
        return false;
    }
    m_pInputText.resize(m_pInputText.size() - nLen);
    setString(m_pInputText.c_str());
    return false;
}

void TextField::setStringDisplaySize(CCSize size)
{
    displaySize = size;
    this->setDimensions(displaySize.width,displaySize.height);
}

void TextField::openIME()
{
    bool bRet = CCIMEDelegate::attachWithIME();
    if (bRet)
    {
        // open keyboard
        CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
        if (pGlView)
        {
            //pGlView->setIMEKeyboard(returnMode);
        }
    }
}

void TextField::editReturn()
{
    closeIME();
    if (m_pobj != NULL && m_pfnCallFunc2 != NULL)
    {
        (m_pobj->*m_pfnCallFunc2)(0);
    }
}

void TextField::closeIME()
{
    this->detachWithIME();
}

void TextField::setSelector(CCObject *obj, SEL_CallFuncN selector)
{
    m_pfnCallFunc2 = selector;
    m_pobj = obj;
}

void TextField::update(float dt)
{
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(isIMEOpen() == 0)
    {
        unschedule(schedule_selector(TextField::update));
        if (m_pobj != NULL && m_pfnCallFunc2 != NULL)
        {
            (m_pobj->*m_pfnCallFunc2)(0);
        }
    }
#else

#endif*/
}
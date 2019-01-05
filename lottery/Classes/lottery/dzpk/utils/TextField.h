//
//  TextField.h
//  ZJH
//
//  Created by luochuanting on 13-5-12.
//
//

#ifndef __ZJH__TextField__
#define __ZJH__TextField__

#include "Inc.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Jni/MiscHelper.h"
#endif
using namespace cocos2d;
using namespace cocos2d::extension;

class TextField: public CCTextFieldTTF, public CCTextFieldDelegate//, public CCTouchDelegate
{
private:
    CCSize displaySize;
public:
    std::string m_pInputText;
    
    TextField();
    ~TextField();
    
    void onEnter();
    void onExit();
    
    static TextField* textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    static TextField* textFieldWithPlaceHolder(const char *placeholder,ccColor3B placeholderColor, const char *fontName, float fontSize);
    
    void setStringDisplaySize(CCSize size);
    
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
    virtual bool detachWithIME();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    

    bool isInTextField(CCTouch *pTouch);
    
    CCRect getRect();
   
    void openIME();
    void closeIME();
    //
    virtual void editReturn();
    //
    void setEmptyString();
    
    float maxLength;
    void setMaxLength(float value)
    {
        maxLength = value;
    };
    
    CCObject* m_pobj;
    
    SEL_CallFuncN m_pfnCallFunc2;
    void setSelector(CCObject *obj, SEL_CallFuncN selector);
    void update(float dt);
    
    int returnMode;
    void setReturnMode(int mode){returnMode = mode;};

    
};

#endif /* defined(__ZJH__TextField__) */

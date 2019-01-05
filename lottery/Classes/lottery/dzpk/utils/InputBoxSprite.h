#ifndef __TexasPoker__InputBoxSprite__
#define __TexasPoker__InputBoxSprite__

#include "Inc.h"

class InputBoxSprite : public Layer
{
    
public:
    InputBoxSprite();
    virtual ~InputBoxSprite();
    CREATE_FUNC(InputBoxSprite);
    
public:
    static InputBoxSprite* createWith(CCPoint& absolutePoint,const char* inputFrame,const char* text);
    virtual bool init();
    
private:
    void initUI();
    
private:
    unsigned int maxLength;
    ccColor3B color;
    EditBox::InputFlag inputFlag;
    EditBox::InputMode inputMode;
    EditBox::KeyboardReturnType keyboardReturnType;
    
    EditBox* pEditBox;
    CCPoint absolutePoint;
    const char* inputFrame;
    const char* text;
    const char* placeHolder;
    
public:
	UNS_INT visibleW;
	UNS_INT visibleH;
	CCPoint originPoint;
    
};

#endif

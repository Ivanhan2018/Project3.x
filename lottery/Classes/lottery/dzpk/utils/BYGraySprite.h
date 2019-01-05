//
//  BYGraySprite.h
//  MCOMGhoul
//
//  Created by hisoftking on 13-8-9.
//
//

#ifndef __MCOMGhoul__BYGraySprite__
#define __MCOMGhoul__BYGraySprite__

#include "inc.h"

class BYGraySprite : public CCSprite{
    
public:
    BYGraySprite();
    virtual ~BYGraySprite();
    CREATE_FUNC(BYGraySprite);
    
public:
    static BYGraySprite* createWithFile(const char* pszFileName);
    static BYGraySprite* createWithFrame(CCSpriteFrame* sf);
    bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
    //virtual void draw();    
    
};

#endif /* defined(__MCOMGhoul__BYGraySprite__) */

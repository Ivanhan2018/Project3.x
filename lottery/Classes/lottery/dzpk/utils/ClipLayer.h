#ifndef __CLIP_LAYER_H__
#define __CLIP_LAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

class ClipLayer:public cocos2d::Layer
{
public:
    static ClipLayer* create(CCSize size);
    
    virtual bool init();
    
    void setClipSize(CCSize size);
    
	CCSize getClipSize();
    
    //virtual void visit();
    
    CCSize m_clipSize;
};

#endif //__CLIP_LAYER_H__

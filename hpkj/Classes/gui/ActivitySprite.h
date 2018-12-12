//
//  SelectButton.h
//  Game
//
//  Created by ?? ???? on 13-6-7.
//
//

#ifndef __Game__ActivitySprite__
#define __Game__ActivitySprite__

#include "cocos2d.h"
#include "CocosUnits.h"
USING_NS_CC;

class ActivitySprite : public Sprite
{
public:
    static ActivitySprite * ActivitySpriteWith(int type);
    bool initActivitySprite(int type);
    ActivitySprite();
    ~ActivitySprite();
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *pTouch, Event *pEvent);

	void setSelect(bool select);
private:
	int m_nType;
};

#endif /* defined(__Game__SelectButton__) */

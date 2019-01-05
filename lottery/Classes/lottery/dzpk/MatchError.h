//
//  MatchError.h
//  Game
//
//  Created by ÂÞÃô on 13-7-10.
//
//

#ifndef __Game__MatchError__
#define __Game__MatchError__

#include "cocos2d.h"
#include "CocosUnits.h"

class MatchError : public Layer
{
public:
    static MatchError * MatchErrorWith();
    bool initMatchError();
    MatchError();
    ~MatchError();
    
    //virtual void registerWithTouchDispatcher();
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
public:
	void closeMatchErrorLayer(Object* obj);
    void agreeMatch(Object* obj);
    void refuseMatch(Object* obj);
private:
    Menu* m_pBackMenu;
    Menu* m_pSelectMenu;
    bool bBackMenu;
    bool bSelectMenu;
};

#endif /* defined(__Game__MatchError__) */

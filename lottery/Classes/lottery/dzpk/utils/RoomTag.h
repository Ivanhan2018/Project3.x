//
//  RoomTag.h
//  gamePoker
//
//  Created by xavier on 13-12-4.
//
//用于聊天框，聊天表情,胜利者动画,庄家标记

#ifndef gamePoker_RoomTag_h
#define gamePoker_RoomTag_h

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

class RoomTag : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(RoomTag);
    
    CCPoint getAnimatePos(int seatid);
    CCPoint getChatTextPos(int seatid);
    CCPoint getChatFacePos(int seatid);
    CCPoint getBankerTagPos(int seatid);
    
    void winAnimate(int seatid);
    void playerChatText(int seatid,string str);
    void playerChatFace(int seatid,string str);
    void addBankerTag(int seatid);
    
    int random(int x, int y);
    void winTagCallBack(CCNode* pSender);
    void chatCallBack(CCNode* pSender);
    
    void chatTextClear(CCNode* pSender , void* data);
    void bankerTagRemove(int seatid);
    
    void hideTag();
    void showTag();
    
};


#endif

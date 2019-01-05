//
//  RoomLight.h
//  gamePoker
//
//  Created by xavier on 14-2-14.
//
//

#ifndef __gamePoker__RoomLight__
#define __gamePoker__RoomLight__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

class RoomLight : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(RoomLight);
    
    Vec2 getLightPos(int id);
    
    double getAngle(int id);
    
    void LightAnimate(int id);
    
    void lightAnimateCallback(Node *pSender,int nViewID);
    
    float getLightLenth(int id);
    
    void reset();
    
    void hideLight();
    void showLight();
    
private:
    int m_light_id;
};

#endif /* defined(__gamePoker__RoomLight__) */

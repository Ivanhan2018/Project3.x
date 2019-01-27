//
//  RoomLight.cpp
//  gamePoker
//
//  Created by xavier on 14-2-14.
//
//

#include "RoomLight.h"
#include "BJLRes.h"//by hxh
#include "MyConfig.h"//by hxh
#include <math.h>


bool RoomLight::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

void RoomLight::reset()
{
    removeAllChildrenWithCleanup(true);
}


Vec2 RoomLight::getLightPos(int id)
{
	MyConfig& myConfig=MyConfig::Instance();
	return ccp(myConfig.m_headPos[id][0],myConfig.m_headPos[id][1]);
}

double RoomLight::getAngle(int id)
{
    cocos2d::Size winSize = Director::getInstance()->getVisibleSize();
    Vec2 end = getLightPos(id);
    Vec2 vector_1 = Vec2(winSize.width/2, 0);
    Vec2 vector_2 = Vec2(end.x-winSize.width/2, end.y-winSize.height/2);
    double angleNum = (vector_1.x*vector_2.x+vector_1.y*vector_2.y)/sqrt((vector_1.x*vector_1.x+vector_1.y*vector_1.y)*(vector_2.x*vector_2.x+vector_2.y*vector_2.y));

    if (end.y<=winSize.height/2)
        return acos(angleNum)*180/3.1415927;
    else
        return 360-acos(angleNum)*180/3.1415927;
}

void RoomLight::LightAnimate(int id)
{
    m_light_id=id;
    
    cocos2d::Size winSize = Director::getInstance()->getVisibleSize();
    if (getChildByTag(0)){
        do{
            Sprite *light = dynamic_cast<Sprite*>(getChildByTag(0));
            CC_BREAK_IF(!light);
            light->runAction(FadeOut::create(0.5f));
            light->runAction(RotateBy::create(0.5f, 40));
			auto pCall = CallFunc::create(CC_CALLBACK_0(RoomLight::lightAnimateCallback, this, light, id));
            light->runAction(Sequence::create(DelayTime::create(0.6f),pCall,FadeIn::create(0.3f),NULL));
        }while (0);
        
        return;
    }
    
    Sprite *light = Sprite::createWithSpriteFrameName(texture_name::s_room_light);
	light->setFlippedX(true);//by hxh
    light->setAnchorPoint(ccp(0.0f,0.5f));
    light->setPosition(ccp(winSize.width/2,winSize.height/2));
    light->setRotation((float)getAngle(id));
    light->setTag(0);
    cocos2d::Size light_sz = light->getContentSize();
    light->setScaleX(getLightLenth(id)/light_sz.width);
    this->addChild(light);
}

void RoomLight::lightAnimateCallback(Node *pSender, int nViewID)
{
    Sprite *light = dynamic_cast<Sprite*>(pSender);
    light->setRotation((float)getAngle(nViewID));
    cocos2d::Size light_sz = light->getContentSize();
    light->setScaleX(getLightLenth(nViewID)/light_sz.width);
}

float RoomLight::getLightLenth(int id)
{
    cocos2d::Size winSize = Director::getInstance()->getVisibleSize();
    Vec2 end = getLightPos(id);
    Vec2 vector = Vec2(end.x-winSize.width/2, end.y-winSize.height/2);
    
    return (float)sqrt(vector.x*vector.x+vector.y*vector.y);
}

void RoomLight::hideLight()
{
    if (getChildByTag(0)){
        getChildByTag(0)->setVisible(false);
    }
}

void RoomLight::showLight()
{
    if (getChildByTag(0)){
        getChildByTag(0)->setVisible(true);
        getChildByTag(0)->setRotation((float)getAngle(m_light_id));
        cocos2d::Size light_sz = getChildByTag(0)->getContentSize();
        getChildByTag(0)->setScaleX(getLightLenth(m_light_id)/light_sz.width);
    }
}





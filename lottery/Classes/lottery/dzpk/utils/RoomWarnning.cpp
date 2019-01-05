//
//  RoomWarnning.cpp
//  gamePoker
//
//  Created by xavier on 14-2-14.
//
//

#include "RoomWarnning.h"
#include "RoomScene.h"

void RoomWarnning::onEnter()
{
    Layer::onEnter();
    //cashAnimate();
}

void RoomWarnning::onExit()
{
    Layer::onExit();
    //CCAnimationCache::sharedAnimationCache()->removeAnimationByName("winAnimation");
}

bool RoomWarnning::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    removeAllChildrenWithCleanup(true);
    return true;
}

void RoomWarnning::cashAnimate()
{
    //CCArray *frame_array = CCArray::createWithCapacity(6);
	Vector<SpriteFrame *> animFrames;
    for (int i=1; i!=7; ++i) {
        CCSpriteFrame *frame = CCSpriteFrame::create(CCString::createWithFormat("rooms/win/room_win_ani_%d.png",i)->getCString(), CCRectMake(0,0, 460, 480));
        //frame_array->addObject(frame);
		animFrames.pushBack(frame);
    }
    
    //CCAnimation *animation = CCAnimation::createWithSpriteFrames(frame_array,0.12f);
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames,0.12f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "winAnimation");
}


CCPoint RoomWarnning::getAnimatePos(int id)
{
    int x,y;
    CCSize WinSize = CCDirector::sharedDirector()->getVisibleSize();
    
    int X[9] = {
        WinSize.width/2+0,
        WinSize.width/2-173,
        WinSize.width/2-340,
        WinSize.width/2-340,
        WinSize.width/2-173,
        WinSize.width/2+173,
        WinSize.width/2+340,
        WinSize.width/2+340,
        WinSize.width/2+173,
    };
    int Y[9] = {
        WinSize.height/2-110,
        WinSize.height/2-110,
        WinSize.height/2-33,
        WinSize.height/2+107,
        WinSize.height/2+165,
        WinSize.height/2+165,
        WinSize.height/2+107,
        WinSize.height/2-33,
        WinSize.height/2-110,
    };
    
    
    int mySeatID = RoomScene::getMySeatID();
    if (mySeatID!=-1)
	{
        if (mySeatID==id)
        {
            x = X[0];
            y = Y[0];
        }else if ( mySeatID<id )
		{
			x=X[id-mySeatID];
			y=Y[id-mySeatID];
		}else
		{
			x=X[9-mySeatID+id];
			y=Y[9-mySeatID+id];
		}
	}
    else
    {
		x=X[id];
		y=Y[id];
    }
    
    return ccp(x,y);
}

void RoomWarnning::winAnimate(int seatid)
{
    if (seatid!=RoomScene::getMySeatID())
        return;
    
    if (getChildByTag(10*seatid+0) || getChildByTag(10*seatid+1)){
        return;
    }
    this->setZOrder(20);
    
    CCPoint pos = getAnimatePos(seatid);
    pos.y += 110;
    
    CCParticleSystemQuad *emitter = CCParticleSystemQuad::create("rooms/dz_winer.plist");
    emitter->setPosition(pos);//设置发射粒子的位置
    emitter->setAutoRemoveOnFinish(true);
    addChild(emitter,10);
}

void RoomWarnning::winAnimateCallBack(float dt)
{
    //this->removeAllChildrenWithCleanup(true);
}

void RoomWarnning::addWarnning(int seatid)
{
    if (seatid<0 || seatid!=RoomScene::getMySeatID())
    {
        return;
    }
    
    for (int i=0;i!=9;i++)
    {
        if (getChildByTag(90+i))
        {
            this->removeChildByTag(90+i,true);
        }
    }
    
    this->setZOrder(4);
    CCSprite *warnning = CCSprite::createWithSpriteFrameName("room_animate_warnning.png");
    warnning->setAnchorPoint(ccp(0.5f,0.5f));
    warnning->setPosition(getAnimatePos(seatid));
    warnning->setVisible(false);
    warnning->setTag(90+seatid);
    this->addChild(warnning);
    
    warnning->runAction(CCBlink::create(5.0f, 10));
}

void RoomWarnning::removeWarnning(int seatid)
{
    if (getChildByTag(90+seatid))
    {
        this->removeChildByTag(90+seatid,true);
    }
}

void RoomWarnning::hideWarnning()
{
    for (int seatid=0;seatid!=9;++seatid)
    {
        if (getChildByTag(90+seatid)){
            getChildByTag(90+seatid)->setVisible(false);
        }
    }
}

void RoomWarnning::showWarnning()
{
    for (int seatid=0;seatid!=9;++seatid)
    {
        if (getChildByTag(90+seatid)){
            getChildByTag(90+seatid)->setVisible(true);
            getChildByTag(90+seatid)->setPosition(getAnimatePos(seatid));
        }
    }

}

#ifndef __TexasPoker__AnimLoading__
#define __TexasPoker__AnimLoading__

#include "PopupNode.h"
#include "ClipLayer.h"

class AnimLoading :  public PopupNode
{
    
public:
    AnimLoading();
    virtual ~AnimLoading();
    CREATE_FUNC(AnimLoading);
    
public:
    virtual bool init();
	void onEnterTransitionDidFinish();
    
public:
    virtual void ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event){};
	virtual void menuCallback(CCObject* pSender){};
    void move(float& differX);
    void moveCallback(float dt);
    void removeMyselfCallback(float dt);
    void netConnTimeCallback(float dt);
    void setResumeUpdate();
    void stopUpdate();
    
private:
    void initUI();
    
private:
    ClipLayer* pClip;
    CCSprite* pLineFrame;
    CCLabelTTF* pScaleTTF;
    CCSize lineFrameSize;
    float currScale;
    float moveStep;
    float pauseMoveCount;
    bool isPause;
    float maxMoveCount;
    
    float netConnTime;
    float currNetConnTime;
    
};

#endif /* defined(__TexasPoker__Loading__) */

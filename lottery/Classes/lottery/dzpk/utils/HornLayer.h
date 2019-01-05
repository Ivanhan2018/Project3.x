//
//  HornLayer.h
//  TexasPoker
//
//  Created by peniel on 12/26/13.
//
//

#ifndef __TexasPoker__HornLayer__
#define __TexasPoker__HornLayer__

#include "Inc.h"
//#include "TextField.h"
#include "MessageModel.h"

class HornLayer : public Layer
{
    
public:
    HornLayer();
    ~HornLayer();
    CREATE_FUNC(HornLayer);
    
public:
    static HornLayer* createWith(CCPoint hronPoint);
	virtual bool init();
    void onEnter();
    void onExit();
    
public:
    bool getIsMsgListShow();
    void setIsMsgListShow(bool showIs);
    void setMsgTTFVisibile(bool bVisible);
    void sendMsg(std::string msg);
    void notifiScrollingCallback(CCObject* pCCObjData);
    
private:
    void createToastLayer(const char* msg,float delay);
    void showToastCallback(CCNode* pSender);
    
private:
	void initUI();
    CCNode* createMsgNode();
    
    void msgScrollAnim(HronMsgModel* msg);
    void whoScrollAnimCallback(CCNode* pSender);
    void msgScrollAnimCallback(CCNode* pSender);
    void showEditBoxCallback(CCNode* pSender);
    
    void ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event);
    
    //判断输入的长度是否超出了最大得长度
    void checkInputSizeCallback(float dt);
    
private:
	UNS_INT         visibleW;
	UNS_INT         visibleH;
    CCNode*         pNoticeNode;
    CCSprite*       pNoticeBg;
    CCLabelTTF*     pMsgTTF;
    CCLabelTTF*     pWhoTTF;
    CCPoint         hronPoint;
    CCPoint         whoPoint;
    CCPoint         msgPoint;
    CCPoint         msgEditBoxPoint;
    bool            isMsgListShow;
    bool            isEditBoxShow;
    HronMsgModel    currRecvMsg;
    //TextField*      pMsgEditBox;
    const int       maxInputSize;
    
};

#endif /* defined(__TexasPoker__HornLayer__) */

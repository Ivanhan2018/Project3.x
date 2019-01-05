//
//  UserInfoScene.h
//  TexasPoker
//
//  Created by peniel on 12/31/13.
//
//

#ifndef __TexasPoker__UserInfoScene__
#define __TexasPoker__UserInfoScene__

#include "CommonLayer.h"
//#include "TextField.h"
//#include "ClipLayer.h"

class UserInfoScene : public CommonLayer
{
    
public:
    UserInfoScene();
	virtual ~UserInfoScene();
	CREATE_FUNC(UserInfoScene);
    
public:
    static CCScene* scene();
	virtual bool init();
	void onEnterTransitionDidFinish();
    //void keyBackClicked();
    void onEnter();
    void onExit();
    
public:
    virtual void ctrlBtnCallbck(CCObject* pSender, cocos2d::extension::Control::EventType event);
	virtual void menuCallback(CCObject* pSender){};
    virtual void popupTipLayerCallback(CCNode* pSender);
    
public:
    void showEditBox();
    void hiddeEditBox(CCNode* pSender);
    
private:
    void initUI();
    bool isCheckCommit();
    void modifyUserInfoCallback(float dt);
    
    void modifyUserHeader();
    void modifyUserHeaderCallback(float dt);
    
    void doPostUserHeader(std::string pathKey);
    void doPostUserHeaderCallback(float dt);
    
    void notifiUpdateRichCallback(CCObject* pCCObjData);
    
private:
    CCSize infoNodeSize;
    CCLabelTTF* playerNameTTF;
    CCSprite* pManTag;
    CCSprite* pWManTag;
    CCSprite* pUserHeader;
    //TextField* pEditBox;
    CCSprite* pEditBoxFrame;
    ControlButton* planCtrlBtn;
    CCPoint editboxPoint;
    //ClipLayer* pRow1;
    bool isMan;
    bool isCustomHeader;
    bool isClickBack;
    
 //   IconTextSprite* pShipIconText;
	//IconTextSprite* pRmbIconText;
    CCLabelTTF* winCountTTF;
    CCLabelTTF* failureCountTTF;
    CCLabelTTF* winSumTTF;
    bool isPopupTipLayerShow;

};

#endif /* defined(__TexasPoker__UserInfoScene__) */

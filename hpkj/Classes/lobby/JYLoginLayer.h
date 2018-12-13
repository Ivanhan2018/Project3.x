#ifndef __LOGINLAYER_H__
#define __LOGINLAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseObject.h"
USING_NS_CC;
//using namespace cocos2d;
using namespace extension;

#define P_SCREEN								(Director::getInstance()->getWinSize())
#define FRAME_CENTER_X       (P_SCREEN.width/2)
#define FRAME_CENTER_Y       (P_SCREEN.height/2)
#define GAP_X					30
#define GAP_Y					30
#define NOTICE_CENTER_X       (143+GAP_X)
#define NOTICE_CENTER_Y       (201+GAP_Y)
#define NOTICE_TEXT_CENTER_X       (50)
#define NOTICE_TEXT_UP       (340)
#define LOGIN_CENTER_X       (P_SCREEN.width-192-50)
#define LOGIN_CENTER_Y       (145+110)
#define REMEMBER_CENTER_X       (LOGIN_CENTER_X-90)
#define REMEMBER_CENTER_Y       (LOGIN_CENTER_Y-10)
#define AUTOMATIC_CENTER_X       (REMEMBER_CENTER_X+186)
#define AUTOMATIC_CENTER_Y       (REMEMBER_CENTER_Y)
#define ACCOUNT_CENTER_X        (LOGIN_CENTER_X-180)
#define ACCOUNT_CENTER_Y        (LOGIN_CENTER_Y+105)
#define ACCOUNT_TEXT_CENTER_X   (ACCOUNT_CENTER_X+80)
#define ACCOUNT_TEXT_CENTER_Y   (ACCOUNT_CENTER_Y)
#define TABLE_CENTER_X          (ACCOUNT_CENTER_X-60)
#define TABLE_CENTER_Y          (ACCOUNT_CENTER_Y)
#define LIST_CENTER_X           (FRAME_CENTER_X+150)
#define LIST_CENTER_Y           (FRAME_CENTER_Y-33)
#define PASSWORD_CENTER_X       (ACCOUNT_CENTER_X)
#define PASSWORD_CENTER_Y       (289)
#define PASSWORD_TEXT_CENTER_X  (ACCOUNT_TEXT_CENTER_X)
#define PASSWORD_TEXT_CENTER_Y  (PASSWORD_CENTER_Y)
#define LOGON_CENTER_X          (LOGIN_CENTER_X)
#define LOGON_CENTER_Y          (LOGIN_CENTER_Y-75)
#define REGISTER_CENTER_X       (LOGON_CENTER_X+100)
#define REGISTER_CENTER_Y       (65)
#define GUEST_CENTER_X          (LOGON_CENTER_X-100)
#define GUEST_CENTER_Y          (REGISTER_CENTER_Y)

typedef enum
{
	FLTAG_Zero				= 0,
	FLTAG_AccountsTex		= 1,
	FLTAG_PassWordTex		= 2,
	FLTAG_TableView			= 3,
	FLTAG_ClearAccountsBtn	= 4,
	FLTAG_ClearPassWordBtn	= 5,
	FLTAG_ListBtn			= 6,
	FLTAG_LogonBtn			= 7,
	FLTAG_RememberPW		= 8,
	FLTAG_RememberLogon		= 9,
	FLTAG_ACPWFont			= 10,
	FLTAG_AUTOFont			= 11,
	FLTAG_RememberPWYES		= 12,
	FLTAG_RememberLogonYES	= 13,

	FLTAG_Viewbg			= 20,
	FLTAG_Loadbg			= 21,
}FLTAG;

class LoginLayer : public Layer
{
public: 
    LoginLayer();
    ~LoginLayer();
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init(bool bLogin);

    //virtual void onEnter();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static Scene* scene();
    
    // a selector callback
    void checkIfAutoLogin(float dt);
    virtual void onEnter();
    virtual void onExit();
   
    void onLogin(Object* obj);
    void onRegist(Object * obj);
    void GuestLogon(Object * obj);
	void changePassWord(Object* obj);
    void back(Object * obj);
    void setIsfaildLogin(bool mIsFaildLogin);
    void onAnsLoginFinish(Object* obj);
    void onLoginFaild(Object* obj);
    void onFindPassword(Object* obj);
	void setLogin(bool loginBool);
    std::string createPassword();
    std::string createAccount();
    static LoginLayer* create(bool bLogin = true)
    {
		LoginLayer *pRet = new LoginLayer(); 
		if (pRet && pRet->init(bLogin)) 
		{ 
			pRet->autorelease();
			return pRet; 
		} 
		else
		{ 
			delete pRet;
			pRet = NULL;
			return NULL;
		}
    }
	// 需要在init中调用setKeyboardEnabled(true);或setKeypadEnabled(true);，这个回调才回执行
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event) override;
	void setBoolFrom(bool b);
private:
	bool m_bLogin;
	bool m_from;

    EditBox * userInput;
    EditBox * passwordInput;
    Menu* m_pLoginMenu;
    Menu* m_pRegisterMenu;
	cocos2d::Size winSize;
	MenuItemSprite * m_pDengluItemSprite;
};

#endif
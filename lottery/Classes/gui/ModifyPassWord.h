#ifndef __ModifyPassWord_SCENE_H__
#define __ModifyPassWord_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class ModifyPassWord : public Layer
{
public:
    
    ModifyPassWord();
    ~ModifyPassWord();
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init(bool bLogin);

    //virtual void onEnter();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static Scene* scene();
    
    // a selector callback
    
    EditBox * orginPasswordInput;
    EditBox * modifyPasswordInput;
	EditBox * surePasswordInput;
   
    void clickButton(Object* obj);							//确定修改
	void rBack(Object* obj);									//返回

	void onChangePassWordResult(Object* obj);					//修改密码返回

	bool isLegal(string str);

	void toFreeLayer(Object *obj);

    
    static ModifyPassWord* create(bool bLogin = true)
    {
        ModifyPassWord *pRet = new ModifyPassWord(); 
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

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//�ֻ����ذ�ť
    void pressLoginPass(Object *obj);
    void pressBankPass(Object *obj);
private:
    Menu* m_pLoginMenu;       //��½
    Menu* m_pRegisterMenu;    //ע��
    MenuItemImage *loginPass;
    MenuItemImage *bankPass;					//�ֻ����ذ�ť
};

#endif // __HELLOWORLD_SCENE_H__

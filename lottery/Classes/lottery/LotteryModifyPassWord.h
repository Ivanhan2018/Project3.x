#ifndef __LotteryModifyPassWord__
#define __LotteryModifyPassWord__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class LotteryModifyPassWord : public Layer
{
public:
    
    LotteryModifyPassWord();
    virtual ~LotteryModifyPassWord();
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init(bool bLogin);

    //virtual void onEnter();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static Scene* scene();
    
    // a selector callback    

    void clickButton(Object* obj);							//
	void rBack(Object* obj);									//

	void onChangePassWordResult(Object* obj);					//
	void bindPhoneNumberRet(Object* obj);
	void unbindPhoneNumberRet(Object *obj);
	void validateTypeRet(Object *obj);
	void onValidated(Object* obj);
	void sendValidateCode(Object *obj);

	bool isLegal(string str);
    
    static LotteryModifyPassWord* create(bool bLogin = true);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);		
	void pressLoginPass(Object *obj);
    void pressBankPass(Object *obj);

	void toBind(Object *obj);
	void toUnBind(Object *obj);
private:

	void initData();
	void initView();

	bool m_isQuerry;

	void setButtonChecked(Object *obj);

};

#endif // __HELLOWORLD_SCENE_H__

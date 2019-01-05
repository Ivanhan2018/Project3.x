#ifndef __Validate__Layer__
#define __Validate__Layer__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ValidateLayer : public Layer {
public:
    CREATE_FUNC(ValidateLayer);
	virtual bool init();
    ValidateLayer();
    virtual ~ValidateLayer();

	//iShowType 0为登录验证 1为绑定手机验证 2为取消绑定验证 3转帐 4分红
	static void show(const char* phoneNumber,const int iShowType = 0); 
	void setShowType(const char* phoneNumber,const int showType);
private:

	static bool m_isShow;
	int m_times;//发送验证码次数
	int iTimer;
	int iType;//0登陆 1绑定 2解绑 3转帐 4分红
	bool m_isValidating; //正在验证
	std::string m_phoneNumber;

	void close(Object* obj);
	void confirm(Object* obj);
	void validateCodeReback(Object *obj);
	//初始化
	void initView();
	void initData();

	void startTimer();
	void stopTimer();

	void sendHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *data);
};

#endif /* __Validate__Layer__ */

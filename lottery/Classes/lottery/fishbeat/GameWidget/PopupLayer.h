#ifndef _POPUP_LAYER_H_
#define _POPUP_LAYER_H_
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

class PopupLayer : public Layer{

public:
	static PopupLayer* popLayer;
	PopupLayer();
	~PopupLayer();
	virtual bool init();
	//需要重写触摸注册函数，重新给定触摸级别
	//virtual void registerWithTouchDispatcher();
	//重写触摸函数，返回true，屏蔽其它层，达到“模态”效果
	bool TouchBegan(Touch *pTouch, Event *pEvent);
	//静态创建函数，创建一个弹出层，设置背景图片
	static PopupLayer* create(const char* backgroundImage);
	//设置标题
	void setTitle(const char* title, int fontsize = 25);
	//设置文本内容，padding 为文字到对话框两边预留的距离，这是可控的，距上方的距离亦是如此
	void setContentText(const char* text, int fontsize=25, int padding=50, int paddingTop=150);
	//设置上层对象和上层回调函数，用于回调时传递CCNode参数
	void setCallBackFunc(const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack);
	//添加menuItem按钮，封装了一个函数，传入些必要的参数
	bool addButton(const char* normalImage, const char* selectedImage, int tag=0);
	//为了在显示层时的属性生效，选择在onEnter里动态生成
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(PopupLayer);
private:
	void buttonCallBack(CCObject* pSender,cocos2d::ui::TouchEventType type);
	//文字内容两边的空白区域
	int m_contentPadding;
	int m_contentPaddingTop;
	//CCObject* m_callbackListener;
	//SEL_CallFuncN m_callback;
	std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> m_callback;
	//定义了CCMenu*类型变量m_pMenu, 并且直接定义默认的set/get方法
	CC_SYNTHESIZE_RETAIN(ui::Button*, m_pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m_sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(ui::Scale9Sprite*, m_s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(LabelTTF*, m_ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(ui::Text*, m_ltContentText, LabelContentText);
};

#endif //!_POPUP_LAYER_H_

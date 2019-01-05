#ifndef _ServerLayer_H_
#define _ServerLayer_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "ui/UIRichText.h"
#include "tinyxml2/tinyxml2.h"
#include "ui/UIWebView.h"
USING_NS_CC;
using namespace ui;
USING_NS_CC_EXT;


class ServerLayer :  public LayerColor
{
private:
	Widget*	      m_widgetUI;
	Layout*		m_panel;
	Button*		  server_close;
	Text*		  m_txtIns;

	void callPhone(const char *number);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	experimental::ui::WebView* m_WebNode;
#endif
public:
	ServerLayer(void);
	~ServerLayer(void);

	virtual bool init(float fScale);
	static ServerLayer *create(float fScale);
	void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void btnCallback(cocos2d::Ref* pSender);
	void DestroyWebView();
};

#endif // !_ServerLayer_H_

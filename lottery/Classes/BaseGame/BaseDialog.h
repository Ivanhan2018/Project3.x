#ifndef __BASE_DIALOG_H__
#define __BASE_DIALOG_H__

#include "cocos2d.h"

USING_NS_CC;

//	对话框——基类
class BaseDialog: public Layer
{
public:
	BaseDialog();
	virtual ~BaseDialog();
	virtual bool init();

	//	关闭对话框
	virtual void closed();

protected:

	//	单点触摸事件——开始
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	//	单点触摸事件——移动
	virtual void onTouchMoved(Touch *touch, Event *unused_event);

	//	单点触摸事件——取消
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	//	单点触摸事件——结束
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//	可视窗口尺寸
	 cocos2d::Size m_sVSize;

	 //	监听者
	 EventListenerTouchOneByOne* m_pListener;

private:

};


#endif	//	__BASE_DIALOG_H__
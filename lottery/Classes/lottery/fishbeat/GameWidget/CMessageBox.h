
#pragma once

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

class CMessageBox
{
public:
	CMessageBox();
	~CMessageBox();

	static CMessageBox* getInstance();

	static void PopMsg(cocos2d::Node* father,const char* title,const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag=0);
	static void PopMsg(cocos2d::Node* father,const char* title,const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag1, int tag2);

private:

};


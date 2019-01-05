
#include "CMessageBox.h"
#include "PopupLayer.h"
#include "VisibleRect.h"

static CMessageBox *instance = nullptr;

CMessageBox::CMessageBox()
{
}

CMessageBox::~CMessageBox()
{
}

void CMessageBox::PopMsg(cocos2d::Node* father,const char* title,const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag/*=0*/)
{
	PopupLayer* msgBox = PopupLayer::create("msgbox_bg.png");
	msgBox->setPosition(VisibleRect::leftTop());
	msgBox->setTitle(title);
	msgBox->setContentText(text, 24, 50, 150);
	msgBox->setCallBackFunc(callBack);
	msgBox->addButton("msgbox_btn_ok_nor.png","msgbox_btn_ok_sel.png",tag);
	father->addChild(msgBox,98);
}

void CMessageBox::PopMsg(cocos2d::Node* father,const char* title,const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag1, int tag2)
{
	PopupLayer* msgBox = PopupLayer::create("msgbox_bg.png");
	msgBox->setPosition(VisibleRect::leftTop());
	msgBox->setTitle(title);
	msgBox->setContentText(text, 24, 50, 150);
	msgBox->setCallBackFunc(callBack);
	msgBox->addButton("msgbox_btn_ok_nor.png","msgbox_btn_ok_sel.png",tag1);
	msgBox->addButton("msgbox_btn_cancle_nor.png","msgbox_btn_cancle_sel.png",tag2);
	father->addChild(msgBox,99);
}

CMessageBox* CMessageBox::getInstance()
{
	if (instance == nullptr)
	{
		instance = new CMessageBox();
	}

	return instance;

}



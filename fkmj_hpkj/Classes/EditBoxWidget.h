#pragma once
#include "cocos2d.h"
#include "WidgetDefine.h"

#include "ui/UIEditBox/UIEditBox.h"


NS_CC_BEGIN;

class EditBoxWidget
	:public ui::EditBox
	,public ui::EditBoxDelegate
{
public:
	EditBoxWidget();
	~EditBoxWidget();
public:
	static EditBoxWidget* create(const Size& size, ui::Scale9Sprite* pNormal9SpriteBg, ui::Scale9Sprite* pPressed9SpriteBg = NULL, ui::Scale9Sprite* pDisabled9SpriteBg = NULL);
public:
	void initState();
	void SetReturnAction(std::function<void()> pFunReturn );
public:
    virtual void editBoxEditingDidBegin(EditBox* editBox);
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(EditBox* editBox);

	void ButtonReleaseEx( cocos2d::Ref* pObject,cocos2d::ui::Widget::TouchEventType _type);
private:
	std::function<void()> m_pReturnSelector;
};

NS_CC_END
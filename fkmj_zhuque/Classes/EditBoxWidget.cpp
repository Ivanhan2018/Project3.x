#include "EditBoxWidget.h"
#include "WidgetFun.h"
#include "ButtonEx.h"
#include "utility.h"
#include "WindowInfo.h"
#include "ui/UIEditBox/UIEditBoxImpl.h"


NS_CC_BEGIN;

EditBoxWidget::EditBoxWidget()
{

}
EditBoxWidget::~EditBoxWidget()
{

}
EditBoxWidget* EditBoxWidget::create(const Size& size, ui::Scale9Sprite* pNormal9SpriteBg, ui::Scale9Sprite* pPressed9SpriteBg, ui::Scale9Sprite* pDisabled9SpriteBg)
{
	EditBoxWidget* pRet = new  EditBoxWidget();

	if (pRet != nullptr && pRet->initWithSizeAndBackgroundSprite(size, pNormal9SpriteBg))
	{
		pRet->autorelease();
		pRet->setDelegate(pRet);
		pRet->initState();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}
void EditBoxWidget::initState()
{
	this->setFontColor(cocos2d::Color3B::RED);
	this->setPlaceholderFontColor(cocos2d::Color3B::WHITE);
	this->setMaxLength(20);
	this->setFontSize(25);
	setReturnType(KeyboardReturnType::DONE);


	cocos2d::ButtonEx* pButton = cocos2d::ButtonEx::create("public/FullEmpty.png","public/FullEmpty.png");
	pButton->setTag(1);
	pButton->setCascadeOpacityEnabled(true);
	pButton->setOpacity(0);
	pButton->setScale(20.0f);
	pButton->addTouchEventListener(CC_CALLBACK_2(EditBoxWidget::ButtonReleaseEx, this));
	pButton->setVisible(false);
	addChild(pButton);
}
void EditBoxWidget::ButtonReleaseEx( cocos2d::Ref* pObject,cocos2d::ui::Widget::TouchEventType _type)
{
	if (_type== cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Node* pNode = this->getChildByTag(1);
		pNode->setVisible(false);
	}
}
void EditBoxWidget::SetReturnAction(std::function<void()> pFunReturn )
{
	m_pReturnSelector = pFunReturn;
}

void EditBoxWidget::editBoxEditingDidBegin(EditBox* editBox) 
{
	cocos2d::Node* pNode = this->getChildByTag(1);
	pNode->setVisible(true);
}
void EditBoxWidget::editBoxEditingDidEnd(EditBox* editBox)
{
}
void EditBoxWidget::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
}
void EditBoxWidget::editBoxReturn(EditBox* editBox)
{
	if (m_pReturnSelector)
	{
		(m_pReturnSelector)();
	}
}
NS_CC_END
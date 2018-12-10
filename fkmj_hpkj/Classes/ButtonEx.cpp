
#include "ButtonEx.h"
#include "ui/UIScale9Sprite.h"

NS_CC_BEGIN;

ButtonEx* ButtonEx::create(const std::string& normalImage,
						const std::string& selectedImage,
						const std::string& disableImage,
						TextureResType texType)
{
	ButtonEx *btn = new (std::nothrow) ButtonEx;
	if (btn && btn->init(normalImage,selectedImage,disableImage,texType))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return nullptr;
}

void ButtonEx::setPressColor(cocos2d::Color3B kColor)
{
	loadTexturePressed(_normalFileName);
	_buttonClickedRenderer->setColor(kColor);
}
ui::Scale9Sprite* ButtonEx::getButtonNormalRenderer()
{
	return _buttonNormalRenderer;
}
ui::Scale9Sprite* ButtonEx::getButtonClickedRenderer()
{
	return _buttonClickedRenderer;
}
ui::Scale9Sprite* ButtonEx::getButtonDisableRenderer()
{
	return _buttonDisabledRenderer;
}

void ButtonEx::setActionPress( bool bValue )
{
	loadTexturePressed(_normalFileName);
	setPressedActionEnabled(bValue);
}

NS_CC_END;

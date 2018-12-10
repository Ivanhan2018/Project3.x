
#pragma once

#include "cocos2d.h"
#include "ui/UIButton.h"


NS_CC_BEGIN;

class ButtonEx
	:public ui::Button
{
public:
	static ButtonEx* create(const std::string& normalImage,
		const std::string& selectedImage = "",
		const std::string& disableImage = "",
		TextureResType texType = TextureResType::LOCAL);

	void setPressColor(cocos2d::Color3B kColor);
	void setActionPress(bool bValue);

	ui::Scale9Sprite* getButtonNormalRenderer();
	ui::Scale9Sprite* getButtonClickedRenderer();
	ui::Scale9Sprite* getButtonDisableRenderer();
};
NS_CC_END;


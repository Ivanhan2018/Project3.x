#pragma once

#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"


NS_CC_BEGIN;

class Scale9SpriteEx
	:public ui::Scale9Sprite
{
public:
    static Scale9SpriteEx* create(const std::string& file);

};
NS_CC_END;


#include "NumberSprite.h"

NumberSprite::NumberSprite(CCTexture2D *pTexture, CCSize size)
{
	initWithTexture(pTexture);
	this->size = size;
	this->rect = CCRectMake(0, 0, size.width, size.height);
	this->setTextureRect(rect);
}

NumberSprite::~NumberSprite(void)
{
}

void NumberSprite::setValue(int v, bool isShow)
{
	if(isShow)
	{
		rect.origin.x = v * size.width;
		this->setTextureRect(rect);
	}
	else
	{
		rect.origin.x = -size.width;
		this->setTextureRect(rect);
	}
}
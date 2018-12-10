#pragma once

#include "cocos2d.h"

namespace cocos2d
{
	class SpriteBlur : public Sprite
	{
	public:
		SpriteBlur();
		~SpriteBlur();
		static SpriteBlur* create();
		virtual void setTexture(Texture2D *texture);
		float getBlurRadius();
		void setBlurRadius(float radius);
		void SortPos();
	protected:
		cocos2d::Node* m_pNode;
	};

}
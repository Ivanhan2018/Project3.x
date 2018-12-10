#pragma once

#include "cocos2d.h"
NS_CC_BEGIN;

class QYRenderTarget : public cocos2d::RenderTexture
{
public:
	static QYRenderTarget * create(int w, int h);
public:
	void QYInit();
public:
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
};

NS_CC_END
#include "QYRenderTarget.h"

USING_NS_CC;

QYRenderTarget * QYRenderTarget::create(int w, int h)
{
	QYRenderTarget *ret = new QYRenderTarget();

	if(ret && ret->initWithWidthAndHeight(w, h,Texture2D::PixelFormat::RGBA8888))
	{
		ret->autorelease();
		ret->QYInit();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;

}
void QYRenderTarget::QYInit()
{
}
void QYRenderTarget::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (_autoDraw)
	{
		beginWithClear(0,0,0,0);

		for(const auto &child: _children)
		{
			if (child != _sprite)
				child->visit();
		}

		//End will pop the current render group
		end();
	}
}

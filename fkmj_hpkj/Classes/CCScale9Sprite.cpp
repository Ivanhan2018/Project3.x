
#include "CCScale9Sprite.h"

NS_CC_BEGIN;

Scale9SpriteEx* Scale9SpriteEx::create(const std::string& file)
{
	Scale9SpriteEx* pReturn = new Scale9SpriteEx();
	if ( pReturn && pReturn->initWithFile(file) )
	{
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return nullptr;
}

NS_CC_END;

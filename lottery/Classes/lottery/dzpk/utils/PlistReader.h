#ifndef __Game__PlistReader__
#define __Game__PlistReader__

#include "Inc.h"

struct PictureRect
{
	PictureRect():pointX(0),pointY(0),width(0),height(0),isRotate(false){}
	UNS_INT pointX;
	UNS_INT pointY;
	UNS_INT width;
	UNS_INT height;
	bool isRotate;
};

class PlistReader
{

private:
	PlistReader();

public:
	~PlistReader();

public:
	static PlistReader* getInstance();
	PictureRect getRectWithParent(CCDictionary* parent,const char* key);
	CCSpriteFrame* getSpriteFrame(CCDictionary* parent,CCTexture2D* pTexture2D,const char* key);
	CCSpriteFrame* getSpriteFrame(CCDictionary* parent,const char* max, const char* key);
};

#endif
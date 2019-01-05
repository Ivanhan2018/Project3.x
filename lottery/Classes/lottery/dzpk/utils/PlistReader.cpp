#include "PlistReader.h"
#include "StringFormat.h"

static PlistReader* instance = NULL;

PlistReader::PlistReader()
{

}

PlistReader::~PlistReader()
{
    CC_SAFE_DELETE(instance);
}

PlistReader* PlistReader::getInstance()
{
    if (NULL == instance) {
        instance = new PlistReader;
    }
    return instance;
}

PictureRect PlistReader::getRectWithParent(cocos2d::CCDictionary* parent,const char* key)
{
    PictureRect rect;
    CCDictionary* item = (cocos2d::CCDictionary*)parent->objectForKey(key);
    CCString frame = item->valueForKey("frame")->getCString();
    char* temp = StringFormat::replace(StringFormat::replace(frame.getCString(),'{'),'}');
    CCArray* vect = CCArray::create();
    StringFormat::Split(temp, ",", vect);
    
    rect.pointX = StringFormat::strToInt(((CCString*)vect->objectAtIndex(0))->getCString());
    rect.pointY = StringFormat::strToInt(((CCString*)vect->objectAtIndex(1))->getCString());
    rect.width = StringFormat::strToInt(((CCString*)vect->objectAtIndex(2))->getCString());
    rect.height = StringFormat::strToInt(((CCString*)vect->objectAtIndex(3))->getCString());
    rect.isRotate = item->valueForKey("rotated")->boolValue();
    
    return rect;
}

CCSpriteFrame* PlistReader::getSpriteFrame(CCDictionary* parent,CCTexture2D* pTexture2D,const char* key)
{
    PictureRect pictureRect = this->getRectWithParent(parent, key);
    const CCRect rect = CCRectMake(pictureRect.pointX,pictureRect.pointY,pictureRect.width,pictureRect.height);
    CCSpriteFrame* pSpriteFrame = CCSpriteFrame::createWithTexture(pTexture2D, rect);
    pSpriteFrame->setRotated(pictureRect.isRotate);
    return pSpriteFrame;
}

CCSpriteFrame* PlistReader::getSpriteFrame(CCDictionary* parent,const char* max, const char* key)
{
    PictureRect pictureRect = this->getRectWithParent(parent, key);
    CCRect rect = CCRectMake(pictureRect.pointX,pictureRect.pointY,pictureRect.width,pictureRect.height);
    CCSpriteFrame* pSpriteFrame = CCSpriteFrame::create(max, rect);
    pSpriteFrame->setRotated(pictureRect.isRotate);
    return pSpriteFrame;
}
//
//  GMFashionConfig.h
//  FishX
//
//  Created by peng on 13-1-6.
//
//
#ifndef FishX_GMFashionConfig_h
#define FishX_GMFashionConfig_h


#include "cocos2d.h"
USING_NS_CC;

#define WINSIZE         Director::getInstance()->getWinSize()

#define WINSIZE_W       (WINSIZE.width)
#define WINSIZE_H       (WINSIZE.height)

#define CCAddSpriteFramesWithFile(name)  \
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name)

#define CCRemoveSpriteFrameByName(name)  \
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(name);
//    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames()

#define CCRemoveTextureForKey(name)    \
    CCTextureCache::sharedTextureCache()->removeTextureForKey(name);

#define CCRemoveUnusedSpriteFrames()    \
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames(); \
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

#define CCRemoveUnusedAllSpriteFrames()    \
    SpriteFrameCache::getInstance()->removeSpriteFrames();\
    CCTextureCache::sharedTextureCache()->removeAllTextures();


/*
 *  同时加载到缓存中
 */
#define CCAddSpriteFramesAndTextureFile(plistName,picName)  \
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistName,picName);

#define CCRemoveSpriteFramesAndTextureForKey(plistName,picName)    \
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistName);\
    CCTextureCache::sharedTextureCache()->removeTextureForKey(picName);
    //释放所有的plistName纹理后
#endif
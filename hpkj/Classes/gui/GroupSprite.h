//
//  RoomUserInfoText.h
//  Game
//
//  Created by 罗敏 on 13-6-26.
//
//

#ifndef __Game__RoomUserInfoText__
#define __Game__RoomUserInfoText__

#include "cocos2d.h"
USING_NS_CC;

typedef enum
{
	GorupSpriteTypePhotoAndPhoto,		//图片和图片组合
	GorupSpriteTypePhotoAndWord,		//图片和文字组合
	GorupSpriteTypePhotoAndText0,        //针对大厅经典场，欢乐场，癞子场的图片和文字的组合
	GorupSpriteTypePhotoAndText1,        //针对大厅初级场，中级场，高级场的图片和文字的组合
}GorupSpriteType;

class GroupSprite : public Sprite
{
public:
    GroupSprite();
    ~GroupSprite();
    static GroupSprite * GroupSpriteWith(const char* str1,const char* str2,GorupSpriteType type,int level = 0);
    bool initGroupSprite(const char* str1,const char* str2,GorupSpriteType type,int level);

	void setTextSize(int size);
	void setTextColor(int r,int g,int b);

	void showCoinsLight(float dt);

	void actionDownAndRemove(Node* node);
private:
	LabelTTF* labelWord;
};

#endif /* defined(__Game__RoomUserInfoText__) */

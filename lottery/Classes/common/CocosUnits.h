//
//  cocosUnits.h
//  Game
//
//  Created by 罗敏 on 13-6-19.
//
//

#ifndef Game_CocosUnits_h
#define Game_CocosUnits_h
#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

inline  SpriteFrame* spriteFrame(std::string name)
{
	// 判断图片是否存在
    
	// 不存在 读取一张红色图片代替，并log不存在
    
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->spriteFrameByName(name.c_str());
	if (pFrame)
	{
		return pFrame;
	}
	Sprite* sprite  = Sprite::create(name.c_str());
	if(sprite != NULL)
		return sprite->displayFrame();
    return SpriteFrameCache::getInstance()->spriteFrameByName("gamble_15.png");
}

inline  void playButtonSound(const char *filePath="active_item.wav")
{
	bool hasEffect = UserDefault::getInstance()->getBoolForKey("hasEffect", true);
    if(hasEffect)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(filePath,false);
    }
}

inline unsigned int playSound(const char* str,bool isloop)
{
    char srcStr[256] = {0};
    memcpy(srcStr, str, strlen(str));
    char *p = strtok(srcStr, "/");
    char *q = p;
    while ((p = strtok(NULL, "/")) != NULL)
    {
        q = p;
    }
    if(UserDefault::getInstance()->getBoolForKey("hasEffect", true))
    {
        return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(q, isloop);
    }
    else
    {
        return NULL;
    }
}

inline void stopSound(unsigned int ids)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(ids);
}

inline void playBGM(const char *str)
{
    if(str)
    {
        if(UserDefault::getInstance()->getBoolForKey("hasMusic"))
        {
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(str,true);
        }
    }
}

inline void pauseBGM()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

inline void resumeBGM(const char* str)
{
    if(CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(str, true);
    }
}

inline void stopBGM()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

inline std::string addCommaToNumber(long long number)
{
    if(number == 0)
    {
        return "0";
    }
    char t[256];
    std::string oldNumber;
    sprintf(t, "%lld",number);
    oldNumber = t;
    std::ostringstream newNumberString;
    bool isIntNumber = true;
    int Dot_Position = 0;
    for (int i =0; i < oldNumber.length(); i++)
    {
        std::ostringstream oldNumberChar;
        oldNumberChar<<oldNumber.at(i);
        if(strcmp(oldNumberChar.str().c_str(), ".") == 0)
        {
            isIntNumber = false;
            Dot_Position = i;
        }
    }
    if(isIntNumber == true)
    {
        std::vector<char> string;
        int Comma_Position = 0;
        for (int i = oldNumber.length() - 1; i >= 0; i--)
        {
            string.push_back(oldNumber.at(i));
            Comma_Position++;
            if(Comma_Position %3 == 0 && Comma_Position > 0 && i!= 0)
            {
                string.push_back(',');
            }
        }
        for (int i = string.size() - 1; i >= 0; i--)
        {
            newNumberString<<string.at(i);
        }
		//std::vector<char>().swap(string);
    }
    else
    {
        std::vector<char> string;
        int Comma_Position = 0;
        for (int i = oldNumber.length() - 1; i >= 0; i--)
        {
            Comma_Position++;
            int CommaBegan_Position = Comma_Position - (oldNumber.length() - Dot_Position - 1) - 1;
            string.push_back(oldNumber.at(i));
            if (CommaBegan_Position % 3 == 0 && CommaBegan_Position > 0 && i != 0)
            {
                string.push_back(',');
            }
        }
        for (int i = string.size() - 1; i >= 0; i--)
        {
            newNumberString<<string.at(i);
        }
		//std::vector<char>().swap(string);
    }
    return newNumberString.str().c_str();
}

inline std::string addCommaToNumber2(int number)
{
    char t[256];
    std::string oldNumber;
    sprintf(t, "%d",number);
    oldNumber = t;
    std::ostringstream newNumberString;
    bool isIntNumber = true;
    int Dot_Position = 0;
    for (int i =0; i < oldNumber.length(); i++)
    {
        std::ostringstream oldNumberChar;
        oldNumberChar<<oldNumber.at(i);
        if(strcmp(oldNumberChar.str().c_str(), ".") == 0)
        {
            isIntNumber = false;
            Dot_Position = i;
        }
    }
    if(isIntNumber == true)
    {
        std::vector<char> string;
        int Comma_Position = 0;
        for (int i = oldNumber.length() - 1; i >= 0; i--)
        {
            string.push_back(oldNumber.at(i));
            Comma_Position++;
            if(Comma_Position %3 == 0 && Comma_Position > 0 && i!= 0)
            {
                string.push_back('<');
            }
        }
        for (int i = string.size() - 1; i >= 0; i--)
        {
            newNumberString<<string.at(i);
        }
		//std::vector<char>().swap(string);
    }
    else
    {
        std::vector<char> string;
        int Comma_Position = 0;
        for (int i = oldNumber.length() - 1; i >= 0; i--)
        {
            Comma_Position++;
            int CommaBegan_Position = Comma_Position - (oldNumber.length() - Dot_Position - 1) - 1;
            string.push_back(oldNumber.at(i));
            if (CommaBegan_Position % 3 == 0 && CommaBegan_Position > 0 && i != 0)
            {
                string.push_back('<');
            }
        }
        for (int i = string.size() - 1; i >= 0; i--)
        {
            newNumberString<<string.at(i);
        }
		//std::vector<char>().swap(string);
    }
    return newNumberString.str().c_str();
}

// add by hxh 20161015
inline const char* convertToChipNum(int count)
{
    
    if (count<1000000)
        return CCString::createWithFormat("%d",count)->getCString();
    
    
    return CCString::createWithFormat("%.1fK",(float)count/1000)->getCString();
}

#endif

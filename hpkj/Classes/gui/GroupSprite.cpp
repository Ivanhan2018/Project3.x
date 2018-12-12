//
//  RoomUserInfoText.cpp
//  Game
//
//  Created by 罗敏 on 13-6-26.
//
//

#include "GroupSprite.h"
#include "CocosUnits.h"
#include "ConfigMgr.h"
GroupSprite::GroupSprite()
{
    
}
GroupSprite::~GroupSprite()
{
    
}
GroupSprite * GroupSprite::GroupSpriteWith(const char* str1,const char* str2,GorupSpriteType type,int level /* = 0 */)
{
    GroupSprite * text = new GroupSprite();
    if(text && text->initWithSpriteFrame(spriteFrame(str1)))
	{
		text->initGroupSprite(str1,str2,type,level);
		text->autorelease();
		return text;
	}
    CC_SAFE_DELETE(text);
    return NULL;
}

bool GroupSprite::initGroupSprite(const char* str1,const char* str2,GorupSpriteType type,int level)
{
	srand(unsigned(time(NULL)));
    if(type == GorupSpriteTypePhotoAndPhoto)
	{
		Sprite* sprite = Sprite::createWithSpriteFrame(spriteFrame(str2));
		sprite->setPosition(Vec2(this->getContentSize().width * 0.5,this->getContentSize().height * 0.5));
		this->addChild(sprite);
	}
	else if(type == GorupSpriteTypePhotoAndWord)
	{
		labelWord = LabelTTF::create(str2,"",22);
		labelWord->setPosition(Vec2(this->getContentSize().width * 0.5,this->getContentSize().height * 0.5));
		this->addChild(labelWord);
	}else if(type == GorupSpriteTypePhotoAndText0)
	{
		const char * str = String::createWithFormat("%s%s",str2,pConfigMgr->text("t453"))->getCString();
		labelWord = LabelTTF::create(str,"",22);
		labelWord->setPosition(Vec2(this->getContentSize().width * 0.540,this->getContentSize().height * 0.145));
		this->addChild(labelWord);
		if(level == 5)
		{
			schedule(schedule_selector(GroupSprite::showCoinsLight),1.5f);
		}
	}else if(type == GorupSpriteTypePhotoAndText1)
	{
		int nIndex = level;
		int a = pConfigMgr->m_Config.nServerEnter[nIndex];
		int b = 0;
		char szLess[24] = {0};
		if (a >= 10000)
		{
			if (a%10000)
			{
				snprintf(szLess, sizeof(szLess)-1, "%.1f%s", a/10000.f, pConfigMgr->text("t469"));
			}
			else
			{
				snprintf(szLess, sizeof(szLess)-1, "%d%s", a/10000, pConfigMgr->text("t469"));
			}
		}
		else
		{
			snprintf(szLess, sizeof(szLess)-1, "%d", a);
		}
		
		char szMax[24] = {0};
		if (b >= 10000)
		{
			if (b%10000)
			{
				snprintf(szMax, sizeof(szMax)-1, "%.1f%s", b/10000.f, pConfigMgr->text("t469"));
			}
			else
			{
				snprintf(szMax, sizeof(szMax)-1, "%d%s", b/10000, pConfigMgr->text("t469"));
			}
		}
		else
		{
			snprintf(szMax, sizeof(szMax)-1, "%d", b);
		}

		char szText[64] = {0};
		if (0 != a && 0 != b)
		{
			snprintf(szText, sizeof(szText) - 1, "%s-%s%s", szLess, szMax, pConfigMgr->text("t470"));	// a-b以下
		}
		else if (0 == b)
		{
			snprintf(szText, sizeof(szText) - 1, "%s%s", szLess, pConfigMgr->text("t471"));	// a以上
		}
		else if (0 == a)
		{
			snprintf(szText, sizeof(szText) - 1, "%s%s", szMax, pConfigMgr->text("t470"));	// b以下
		}
		else
		{
			snprintf(szText, sizeof(szText) - 1, "0%s", pConfigMgr->text("t471"));	// 0以上
		}
		LabelTTF* label = LabelTTF::create(szText,"Verdana-Bold",20);
		label->setColor(ccc3(255,255,255));
		label->setPosition(Vec2(this->getContentSize().width * 0.5,this->getContentSize().height * 0.3));
		//this->addChild(label);
		labelWord = LabelTTF::create(str2,"",22);
		labelWord->setAnchorPoint(Vec2(1,0.5));
		labelWord->setPosition(Vec2(this->getContentSize().width * 0.6,this->getContentSize().height * 0.13));
		//this->addChild(labelWord);
		if(level == 1)
		{
			Sprite* spriteXigua = Sprite::createWithSpriteFrame(spriteFrame("dt_xiguaba.png"));
			spriteXigua->setPosition(Vec2(this->getContentSize().width * 0.28,this->getContentSize().height * 0.67));
			spriteXigua->setAnchorPoint(Vec2(0.5,0.0));
			this->addChild(spriteXigua);

			CCRotateBy* action1 = CCRotateBy::create(0.8f,-10);
			CCRotateBy* action2 = CCRotateBy::create(0.8f,10);
			spriteXigua->runAction(RepeatForever::create(Sequence::create(action1,action2,NULL)));
		}
		if(level == 2)
		{
			Sprite* spirteFlow = Sprite::createWithSpriteFrame(spriteFrame("dt_flow.png"));
			spirteFlow->setPosition(Vec2(this->getContentSize().width * 0.28,this->getContentSize().height * 0.68));
			this->addChild(spirteFlow);
			spirteFlow->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.4f,1.5),ScaleTo::create(0.7f,0.9),NULL)));
		}
		if(level == 3)
		{
			Sprite* spriteTree = Sprite::createWithSpriteFrame(spriteFrame("dt_tree.png"));
			spriteTree->setPosition(Vec2(17,this->getContentSize().height * 0.7));
			spriteTree->setAnchorPoint(Vec2(0.0,0.5));
			this->addChild(spriteTree);
			spriteTree->setScale(0.5);

			CCRotateBy* action1 = CCRotateBy::create(0.9f,-10);
			CCRotateBy* action2 = CCRotateBy::create(0.9f,10);
			spriteTree->runAction(RepeatForever::create(Sequence::create(action1,action2,NULL)));
		}
	}
    return true;
}

void GroupSprite::showCoinsLight(float dt)
{
	int randNumber0 = 10+rand()%80;
	int randNumber2 = 30+rand()%30;
	Sprite* spriteLight = Sprite::createWithSpriteFrame(spriteFrame("dt_coinslight.png"));
	spriteLight->setPosition(Vec2(this->getContentSize().width * randNumber0 *0.01,this->getContentSize().height * randNumber2*0.01));
	this->addChild(spriteLight);
	ScaleTo* action1 = ScaleTo::create(0.4f,1.3f);
	ScaleTo* action2 = ScaleTo::create(0.7f,0.4f);
	CallFuncN* actionDown = CallFuncN::create(this,callfuncN_selector(GroupSprite::actionDownAndRemove));
	spriteLight->runAction(Sequence::create(action1,action2,actionDown,NULL));
}

void GroupSprite::actionDownAndRemove(Node* node)
{
	Sprite* sprite = (Sprite*)node;
	sprite->removeFromParentAndCleanup(true);
}

void GroupSprite::setTextSize(int size)
{
	labelWord->setFontSize(size);
}

void GroupSprite::setTextColor(int r,int g,int b){
	labelWord->setColor(ccc3(r,g,b));
}
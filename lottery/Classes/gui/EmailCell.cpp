#include "EmailCell.h"
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"

EmailCell::EmailCell(void)
{
}


EmailCell::~EmailCell(void)
{
}

EmailCell* EmailCell::EmailCellWith(int type)
{
	EmailCell* buyCell = new EmailCell();
	if(buyCell && buyCell->initWithSpriteFrame(spriteFrame("dt_email9.png")))
	{
		buyCell->initEmailCellWith(type);
		buyCell->autorelease();
		return buyCell;
	}
	CC_SAFE_DELETE(buyCell);
	return NULL;
}

void EmailCell::setCellIdx(int idx)
{
	if(idx < 10)
	{
		messageTitle = LabelTTF::create(ConfigMgr::instance()->text("t406"),"",20);
		messageTitle->setPosition(Vec2(this->getContentSize().width * 0.01,this->getContentSize().height * 0.81));
		messageTitle->setAnchorPoint(Vec2(0.0,0.5));
		messageTitle->setColor(ccc3(0,0,0));
		this->addChild(messageTitle);
		String* strGonggao0 = String::createWithFormat("%s",ConfigMgr::instance()->text("t404"));
		messageContext->setString(strGonggao0->getCString());
		messageContext->setPosition(Vec2(this->getContentSize().width * 0.01,this->getContentSize().height * 0.4));
		messageContext->setZOrder(10);
	}
	else
	{
		messageTitle = LabelTTF::create(ConfigMgr::instance()->text("t405"),"",20);
		messageTitle->setPosition(Vec2(this->getContentSize().width * 0.01,this->getContentSize().height * 0.81));
		messageTitle->setAnchorPoint(Vec2(0.0,0.5));
		messageTitle->setColor(ccc3(0,0,0));
		this->addChild(messageTitle);
		std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
		std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
		String* strGonggao0 = String::createWithFormat("%s%s%s%s",ConfigMgr::instance()->text(String::createWithFormat("t%d",401)->getCString()),userAccount.c_str(),ConfigMgr::instance()->text(String::createWithFormat("t%d",402)->getCString()),ConfigMgr::instance()->text("t403"));
		messageContext->setString(strGonggao0->getCString());
		messageContext->setPosition(Vec2(this->getContentSize().width * 0.01,this->getContentSize().height * 0.4));
		messageContext->setZOrder(10);
	}
}
bool EmailCell::initEmailCellWith(int type)
{
	//É¾³ý
	//Sprite* backNormal= Sprite::createWithSpriteFrame(spriteFrame("dt_email3.png"));
	//Sprite* backSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_email3.png"));
	//MenuItemSprite* backItemSprite = MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(EmailCell::DeleteMessage));
	//backSelect->setScale(0.9);
	//backSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	//Menu* backMenu= Menu::create(backItemSprite,NULL);
	//backMenu->setPosition(Vec2(this->getContentSize().width - backSelect->getContentSize().width * 0.5,this->getContentSize().height - backSelect->getContentSize().height * 0.5));
	//this->addChild(backMenu);
	if(type < 10)
	{
		String* strGonggao0 = String::createWithFormat("%s",ConfigMgr::instance()->text("t404"));
		messageContext = LabelTTF::create(strGonggao0->getCString(),"", 18,cocos2d::Size(750.0f,0.0f),kCCTextAlignmentLeft);
		messageContext->setPosition(Vec2(this->getContentSize().width * 0.05,this->getContentSize().height * 0.4));
		messageContext->setColor(ccc3(0,0,0));
		messageContext->setAnchorPoint(Vec2(0.0,0.5));
		this->addChild(messageContext,10);
	}
	else
	{
		std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
		std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
		String* strGonggao0 = String::createWithFormat("%s%s%s%s%s",ConfigMgr::instance()->text(String::createWithFormat("t%d",401)->getCString()),userAccount.c_str(),ConfigMgr::instance()->text(String::createWithFormat("t%d",402)->getCString()),userPassword.c_str(),ConfigMgr::instance()->text("403"));
		messageContext = LabelTTF::create(strGonggao0->getCString(),"", 18,cocos2d::Size(750.0f,0.0f),kCCTextAlignmentLeft);
		messageContext->setPosition(Vec2(this->getContentSize().width * 0.05,this->getContentSize().height * 0.4));
		messageContext->setColor(ccc3(0,0,0));
		messageContext->setAnchorPoint(Vec2(0.0,0.5));
		this->addChild(messageContext,10);
	}
	return true;
}

void EmailCell::DeleteMessage(Object* obj)
{
	this->removeFromParentAndCleanup(true);
}

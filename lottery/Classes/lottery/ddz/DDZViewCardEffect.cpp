#include "DDZViewCardEffect.h"
#include "DDZSceneData.h"
#include "MyTools.h"//<by hxh>
#include "SoundControl.h"//<by hxh>
#include "CocosUnits.h"//by hxh

bool DDZViewCardEffect::init()
{
	bool ret =false;
	do 
	{
		m_leftAlarm = false;
		m_rightAlarm = false;
		m_RobotNode=Node::create();
		this->addChild(m_RobotNode);
		ret =true;
	} while (0);
	return ret;
}

void DDZViewCardEffect::runEffect( EffectType _type, int userId)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	Color3B color = ccc3(255,255,255);
	switch (_type)
	{
	case Effect_gameStart://普通开始
		effectOpenStart(false,userId);
		break;
	case Effect_OpenStart://明牌开始
		effectOpenStart(true,userId);
		break;
	case Effect_openCard://发牌时候明牌
		effectOpenCard(userId);
		break;
	case 	Effect_LandLord://叫地主
		effectLandLord(true,userId);
		break;
	case Effect_NoLandLord://不叫地主
		effectLandLord(false,userId);
		break;
	case Effect_RobLord://抢地主
		effectRodLord(true,userId);
		break;
	case Effect_NoRobLord://不抢
		effectRodLord(false,userId);
		break;
	case Effect_PassCard://过牌
		effectPassCard(userId);
		break;
	case Effect_jiaBei://加倍
		effectJiaBei(userId,true);
		break;
	case Effect_alarm://警报
		effectAlarm(userId);
		break;
	case Effect_outcardErr://出牌错误
		{
			addEffect("outcarderr",ccp(size.width*0.5,size.height*0.2),color,20,true);
		}
		break;
	case Effect_noBigCard:
		{
			addEffect("nobigcard",ccp(size.width*0.5,size.height*0.2),color,20,true);
		}
		break;
	case Effect_task:
		{
			addEffect("tasktips",ccp(size.width*0.5,size.height*0.2),color,20,true);
		}
		break;
	case Effect_LlinkOFF:
		{
			addEffect("linkofftips",ccp(size.width*0.5,size.height*0.5),color,20,true);
		}
		break;
	}
}

void DDZViewCardEffect::effectOpenStart( bool isOpen,int userId )
{
	return;
	cocos2d::Size size =Director::getInstance()->getWinSize();
	Vec2 point = getEffectPos(userId);
	Color3B color = ccc3(161,84,32);
	if (isOpen)
	{
		addPictureEffect(DDZtexture_name::s_font_visiblecard,point);
		SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_mingpai,userId);
	}else
	{
//		addEffect("ptks",point,color,12);
	}
}

void DDZViewCardEffect::effectOpenCard( int userId )
{
	return;
	cocos2d::Size size =Director::getInstance()->getWinSize();
	Vec2 point = getEffectPos(userId);
	SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_mingpai,userId);
	Color3B color = ccc3(161,84,32);
	addPictureEffect(DDZtexture_name::s_font_visiblecard,point);
}

void DDZViewCardEffect::effectLandLord( bool isLand, int userId)
{
 	cocos2d::Size size =Director::getInstance()->getWinSize();
	Vec2 point = getEffectPos(userId);
	if(isLand)
	{
		addPictureEffect(DDZtexture_name::s_font_jiaodizhu,point);
		SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_jiaodizhu,userId);
	}else{
		addPictureEffect(DDZtexture_name::s_font_bujiao,point);
		SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_bujiao,userId);
	}
}

void DDZViewCardEffect::effectRodLord( bool isRob ,int userId)
{
	cocos2d::Size size =Director::getInstance()->getWinSize();
	Vec2 point = getEffectPos(userId);
	if(isRob)
	{
		addPictureEffect(DDZtexture_name::s_font_qiangdizhu,point);
		SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_qiangdizhu,userId);
	}else{
		addPictureEffect(DDZtexture_name::s_font_buqiang,point);
		SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_buqiang,userId);
	}
}

void DDZViewCardEffect::effectPassCard(int userId)
{
	cocos2d::Size size =Director::getInstance()->getWinSize();
	Color3B color = ccc3(161,84,32);
	Vec2 point = getEffectPos(userId);
	SoundEffect::sharedSoundEffect()->playPassEffect(userId);
	addPictureEffect(DDZtexture_name::s_font_pass,point);
}

void DDZViewCardEffect::addEffect(const char* key,Vec2 point,Color3B color,int fontSize,bool showDi)
{
	Dictionary * strings = Dictionary::createWithContentsOfFile("ddz_strings.xml");
	const char * str = ((CCString*)strings->objectForKey(key))->getCString();
	LabelTTF * pLableTTF = LabelTTF::create(str,"",fontSize);
	//CCStrokeLabel * strokLable = CCStrokeLabel::create(pLableTTF,ccc3(0,0,0),ccc3(255,255,255),1.0f);
	pLableTTF->setAnchorPoint(ccp(0.5f,0.5f));
	pLableTTF->setColor(color);
	pLableTTF->setPosition(point);
	//pLableTTF->setAnchorPoint(ccp(0.5f,0.5f));
	Sprite * notifySprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_notify);
	notifySprite->setPosition(point);
	Node * effectNode = Node::create();
	if (showDi)
	{
		effectNode->addChild(notifySprite);
	}
	effectNode->addChild(pLableTTF);
	this->addChild(effectNode);
	effectNode->setPosition(Vec2::ZERO);
	effectNode->runAction(Sequence::create(MoveTo::create(0.8f,ccp(0,30)),DelayTime::create(1.0f),
		CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
}

void DDZViewCardEffect::addUserInputMsgEffect(const char* key,Vec2 point,Color3B color,int fontSize,bool showDi)
{
	LabelTTF * pLableTTF = LabelTTF::create(key,"",fontSize);
	//CCStrokeLabel * strokLable = CCStrokeLabel::create(pLableTTF,ccc3(0,0,0),ccc3(255,255,255),1.0f);
	pLableTTF->setAnchorPoint(ccp(0.5f,0.5f));
	pLableTTF->setColor(color);
	pLableTTF->setPosition(point);
	Sprite * notifySprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_notify);
	notifySprite->setPosition(point);
	Node * effectNode = Node::create();
	if (showDi)
	{
		effectNode->addChild(notifySprite);
	}
	effectNode->addChild(pLableTTF);
	this->addChild(effectNode);
	effectNode->setPosition(Vec2::ZERO);
	effectNode->runAction(Sequence::create(MoveTo::create(0.8f,ccp(0,30)),DelayTime::create(1.0f),
		CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
}

void DDZViewCardEffect::addPictureEffect(const char* key,Vec2 point,bool isAddPartiCar)
{
	//Sprite * sprite = Sprite::createWithSpriteFrameName(key);
	Sprite * sprite=Sprite::createWithSpriteFrame(spriteFrame(key));
	sprite->setAnchorPoint(ccp(0.5,0.5));
	sprite->setPosition(point);
	this->addChild(sprite);
	//	if (isAddPartiCar)
// 	{
// 		CCParticleSystemQuad* partcall=CCParticleSystemQuad::create("hlddz_particle/cardtype/duizi.plist");
// 		partcall->setPosition(Vec2::ZERO);
// 		sprite->addChild(partcall);
// 	}

	sprite->runAction(Sequence::create(MoveTo::create(0.2f,ccp(sprite->getPositionX(),
		sprite->getPositionY()+30)),DelayTime::create(1.0f),
		CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
}

void DDZViewCardEffect::removerSelf(Node* node)
{
	if (node)
	{
		node->removeFromParent();
	}
}

void DDZViewCardEffect::effectJiaBei(int userId,bool isJiaBei)
{
	cocos2d::Size size =Director::getInstance()->getWinSize();
	Vec2 point = getEffectPos(userId);
	Color3B color = ccc3(161,84,32);
	if (isJiaBei)
	{
		addPictureEffect(DDZtexture_name::s_font_jiabei,point);
		SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_jiabei,userId);
		return;
	}
}

void DDZViewCardEffect::runCardEffect(int cardType,int userId,int cardNum,int userOutCardState)
{
	cocos2d::Size size =Director::getInstance()->getWinSize();
	Vec2 point = getPokerKindPos(userId);
	switch(cardType)
	{
	case CT_ERROR://错误牌型
		break;
	case CT_SINGLE://单牌
		if (userOutCardState!=3)//表示普通出牌
		{
			SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_yizhang,userId);
		}else{
			SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
		}
		break;
	case CT_DOUBLE://对牌
		if (userOutCardState!=3)
		{
			SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_yidui,userId);
		}else{
			SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
		}
		break;
	case CT_THREE://三条
		if (userOutCardState!=3)
		{
			SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_sanzhang,userId);
		}else{
			SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
		}
		break;
	case CT_SINGLE_LINE://顺子
		{
			addPictureEffect(DDZtexture_name::s_ctSingleLine,point);
			if (userOutCardState!=3)
			{
				SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_shunzi,userId);
			}else{
				SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
			}
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_shunzi);
		}
		break;
	case CT_DOUBLE_LINE://连对
		{
			addPictureEffect(DDZtexture_name::s_ctDoubleLine,point);
			if (userOutCardState!=3)
			{
				SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_liandui,userId);
			}else{
				SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
			}
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_liandui);
		}
		break;
	case CT_THREE_LINE://三连
		{
			addPictureEffect(DDZtexture_name::s_ctThreeLine,point);
			if (userOutCardState!=3)
			{
				SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_sanshun,userId);
			}else{
				SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
			}
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_sanshun);
		}
		break;
	case CT_THREE_LINE_TAKE_ONE://三带一
		{
			if (cardNum>=8)//飞机
			{
				SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_feiji,userId);
				DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_feiji);
				Node * animNode = Node::create();
				Sprite * planeSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_plane);//飞机
				planeSprite->setAnchorPoint(ccp(0.5f,0));
				animNode->addChild(planeSprite,1,0);
				Sprite * planeyySpriteLeft = Sprite::createWithSpriteFrameName(DDZtexture_name::s_planeyy);//飞机左边阴影
				planeyySpriteLeft->setAnchorPoint(ccp(1.0f,1.0f));
				Sprite * planeyySpriteRight = Sprite::createWithSpriteFrameName(DDZtexture_name::s_planeyy);//飞机右边阴影
				planeyySpriteRight->setFlipX(true);
				planeyySpriteRight->setAnchorPoint(ccp(0,1.0f));
				animNode->addChild(planeyySpriteRight);
				animNode->addChild(planeyySpriteLeft);
				//头部螺旋桨
				//Array * animFrames = new Array(2);
				Vector<SpriteFrame *> animFrames;
				for (int i=0;i<2;i++)
				{
					CCString str;
					str.initWithFormat("lxj%d.png",i);
					SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
					//animFrames->addObject(frame);
					animFrames.pushBack(frame);
				}
				CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
				//Sprite * lxjHead = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames->objectAtIndex(0));
				Sprite * lxjHead = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames.at(0));
				lxjHead->setAnchorPoint(ccp(0,0));
				lxjHead->runAction(RepeatForever::create(Animate::create(animation)));
				lxjHead->setPosition(ccp(-planeSprite->getContentSize().width/2-15,15.0f));
				animNode->addChild(lxjHead,2,1);
				animNode->setPosition(ccp(size.width/2,size.height/2));
				//尾部螺旋桨
				//Array * animFrames2 = new Array(2);
				Vector<SpriteFrame *> animFrames2;
				for (int i=0;i<2;i++)
				{
					CCString str;
					str.initWithFormat("lxj%d.png",i);
					SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
					//animFrames2->addObject(frame);
					animFrames2.pushBack(frame);
				}
				CCAnimation * animation2 = CCAnimation::createWithSpriteFrames(animFrames2,0.2f);
				//Sprite * lxjEnd = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames2->objectAtIndex(0));
				Sprite * lxjEnd = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames2.at(0));
				lxjEnd->setAnchorPoint(ccp(1.0f,0));
				lxjEnd->runAction(RepeatForever::create(Animate::create(animation2)));
				lxjEnd->setPosition(ccp(planeSprite->getContentSize().width/2+15,15.0f));
				animNode->addChild(lxjEnd,0,3);
				animNode->setPosition(ccp(size.width+planeSprite->getContentSize().width/2,size.height/2));
				animNode->runAction(Sequence::create(MoveTo::create(1.5f,ccp(-planeSprite->getContentSize().width/2-100,size.height/2)),
					CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
				this->addChild(animNode);

			}else
			{
				if (userOutCardState!=3)
				{
					SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_sandaiyi,userId);
				}else{
					SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
				}
			}
		}
		break;
	case CT_THREE_LINE_TAKE_TWO://三带一对
		{
			if (cardNum>=8)//飞机
			{
				SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_feiji,userId);
				DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_feiji);
				Node * animNode = Node::create();
				Sprite * planeSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_plane);//飞机
				planeSprite->setAnchorPoint(ccp(0.5f,0));
				animNode->addChild(planeSprite,1,0);
				Sprite * planeyySpriteLeft = Sprite::createWithSpriteFrameName(DDZtexture_name::s_planeyy);//飞机左边阴影
				planeyySpriteLeft->setAnchorPoint(ccp(1.0f,1.0f));
				Sprite * planeyySpriteRight = Sprite::createWithSpriteFrameName(DDZtexture_name::s_planeyy);//飞机右边阴影
				planeyySpriteRight->setFlipX(true);
				planeyySpriteRight->setAnchorPoint(ccp(0,1.0f));
				animNode->addChild(planeyySpriteRight);
				animNode->addChild(planeyySpriteLeft);
				//头部螺旋桨
				//Array * animFrames = new Array(2);
				Vector<SpriteFrame *> animFrames;
				for (int i=0;i<2;i++)
				{
					CCString str;
					str.initWithFormat("lxj%d.png",i);
					SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
					//animFrames->addObject(frame);
					animFrames.pushBack(frame);
				}
				CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
				//Sprite * lxjHead = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames->objectAtIndex(0));
				Sprite * lxjHead = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames.at(0));
				lxjHead->setAnchorPoint(ccp(0,0));
				lxjHead->runAction(RepeatForever::create(Animate::create(animation)));
				lxjHead->setPosition(ccp(-planeSprite->getContentSize().width/2-15,15.0f));
				animNode->addChild(lxjHead,2,1);
				animNode->setPosition(ccp(size.width/2,size.height/2));
				//尾部螺旋桨
				//Array * animFrames2 = new Array(2);
				Vector<SpriteFrame *> animFrames2;
				for (int i=0;i<2;i++)
				{
					CCString str;
					str.initWithFormat("lxj%d.png",i);
					SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
					//animFrames2->addObject(frame);
					animFrames2.pushBack(frame);
				}
				CCAnimation * animation2 = CCAnimation::createWithSpriteFrames(animFrames2,0.2f);
				//Sprite * lxjEnd = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames2->objectAtIndex(0));
				Sprite * lxjEnd = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames2.at(0));
				lxjEnd->setAnchorPoint(ccp(1.0f,0));
				lxjEnd->runAction(RepeatForever::create(Animate::create(animation2)));
				lxjEnd->setPosition(ccp(planeSprite->getContentSize().width/2+15,15.0f));
				animNode->addChild(lxjEnd,0,3);
				animNode->setPosition(ccp(size.width+planeSprite->getContentSize().width/2,size.height/2));
				animNode->runAction(Sequence::create(MoveTo::create(1.5f,ccp(-planeSprite->getContentSize().width/2-100,size.height/2)),
					CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
				this->addChild(animNode);
				
			}else
			{
				if (userOutCardState!=3)
				{
					SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_sandaier,userId);
				}else{
					SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
				}
			}
		}
		break;
	case CT_FOUR_LINE_TAKE_ONE://四带二
		{
			if (userOutCardState!=3)
			{
				SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_sidaier,userId);
			}else{
				SoundEffect::sharedSoundEffect()->playDaPaiEffect(userId);
			}
		}
		break;
	case CT_FOUR_LINE_TAKE_TWO://四代两对
		{

		}
		break;
	case CT_BOMB_CARD://炸弹
		{
			Node * animNode = Node::create();
			Sprite * bombSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_bomb);
			bombSprite->setAnchorPoint(ccp(0.5f,0));
			animNode->addChild(bombSprite,0,0);
			//Array * animFrames = new Array(8);
            Vector<SpriteFrame *> animFrames;
			for (int i=0;i<8;i++)
			{
				CCString str;
				str.initWithFormat("bomb%d.png",i);
				SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
				//animFrames->addObject(frame);
				animFrames.pushBack(frame);
			}
			CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
			Sprite * bombAnimation = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames.at(0));
			bombAnimation->setScale(2.6f);
			bombAnimation->setAnchorPoint(ccp(0.5f,0));
			bombAnimation->setPositionY(10.0f);
			animNode->addChild(bombAnimation,1,1);
			bombAnimation->runAction(Animate::create(animation));
			bombSprite->runAction(FadeOut::create(1.6f));
			animNode->setPosition(ccp(size.width/2,150.0f));
			animNode->runAction(Sequence::create(DelayTime::create(1.6f),
				CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
			this->addChild(animNode);
			SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_zhadan,userId);
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_zhadan);
			Director::getInstance()->getRunningScene()->runAction(CCShake::create(0.4f,20));
		}
		break;
	case CT_MISSILE_CARD://火箭
		{
			Node*		_node=Node::create();
			this->addChild(_node);
			Sprite *_sptTou=Sprite::createWithSpriteFrameName(DDZtexture_name::s_huojian);
			_sptTou->setAnchorPoint(ccp(0.5f,0.0));
			_sptTou->setPosition(ccp(size.width*0.5f,size.height*0.4f));
			_sptTou->runAction(Sequence::create(DelayTime::create(0.1f),MoveTo::create(0.6f,ccp(size.width*0.5,size.height+300)),NULL));
			Sprite* _sptWei=Sprite::createWithSpriteFrameName(DDZtexture_name::s_huoyan);
			_sptWei->setAnchorPoint(ccp(0.5f,1.0f));
			_sptWei->setPosition(ccp(_sptTou->getContentSize().width*0.5,0));
			_sptWei->runAction(FadeIn::create(0.4f));
			_sptTou->addChild(_sptWei);
			//Array * animFrames = new Array(4);
			Vector<SpriteFrame *> animFrames;
			for (int i=0;i<4;i++)
			{
				CCString str;
				str.initWithFormat("yun%d.png",i);
				SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
				//animFrames->addObject(frame);
				animFrames.pushBack(frame);
			}
			CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
			//Sprite * _yunAnimation = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames->objectAtIndex(0));
			Sprite * _yunAnimation = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames.at(0));
			_yunAnimation->setAnchorPoint(ccp(0.5f,0.5f));
			_yunAnimation->setPosition(ccp(size.width*0.5,size.height*0.4));
			_yunAnimation->runAction(Sequence::create(DelayTime::create(0.2f),Animate::create(animation),NULL));

			_node->addChild(_sptTou);
			_node->addChild(_yunAnimation);
			_node->runAction(Sequence::create(DelayTime::create(1.0f),CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
			Director::getInstance()->getRunningScene()->runAction(CCShake::create(0.4f,20));
			SoundEffect::sharedSoundEffect()->playEffect(DDZsound_name::s_huojian,userId);
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_huojian);
		}
		break;
	}
	
}

void DDZViewCardEffect::runRobot( bool isOnRobot )
{
	m_RobotNode->removeAllChildren();
	if (isOnRobot)
	{
		cocos2d::Size size= Director::getInstance()->getWinSize();
		SpriteFrame * pFrameNormal = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_cancelRobotNormal);
		SpriteFrame * pFrameSelected = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_cancelRobotPressed);
		SpriteFrame * pFrameDisable = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_cancelRobotPressed);
		Sprite * normalSprite = Sprite::createWithSpriteFrame(pFrameNormal);
		Sprite * selectedSprite = Sprite::createWithSpriteFrame(pFrameSelected);
		selectedSprite->setColor(ccc3(100,100,100));
		Sprite * disableSprite = Sprite::createWithSpriteFrame(pFrameDisable);
		MenuItemSprite * menuItemSprite = MenuItemSprite::create(normalSprite,selectedSprite,disableSprite,this,menu_selector(DDZViewCardEffect::RobotCallBack));
		Menu* menu=Menu::create(menuItemSprite,NULL);
		menu->setPosition(ccp(size.width*0.5,size.height*0.2));
		m_RobotNode->addChild(menu,100,100);

/*
		//机器人
		Sprite* spt=Sprite::createWithSpriteFrameName(DDZtexture_name::s_robot_pic);
		spt->setPosition(ccp(menu->getPositionX(),menu->getPositionY()+70));
		spt->setAnchorPoint(ccp(0.5,0.5));
		m_RobotNode->addChild(spt,2,2);

		Sprite* spt_eyeL=Sprite::createWithSpriteFrameName(DDZtexture_name::s_eye);
		spt_eyeL->setPosition(ccp(spt->getPositionX()+8,spt->getPositionY()+7));
		spt_eyeL->setAnchorPoint(ccp(0.5,0.5));
		m_RobotNode->addChild(spt_eyeL,3,3);
		spt_eyeL->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.2f),DelayTime::create(0.5f),FadeOut::create(0.2f),NULL)));
		Sprite* spt_eyeR=Sprite::createWithSpriteFrameName(DDZtexture_name::s_eye);
		spt_eyeR->setPosition(ccp(spt->getPositionX()-11,spt->getPositionY()+7));
		spt_eyeR->setAnchorPoint(ccp(0.5,0.5));
		spt_eyeR->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.2f),DelayTime::create(0.5f),FadeOut::create(0.2f),NULL)));
		m_RobotNode->addChild(spt_eyeR,4,4);

		Sprite* spt_banshou=Sprite::createWithSpriteFrameName(DDZtexture_name::s_banshou);
		Vec2 banshou_pt=ccp(spt->getPositionX()+spt->getContentSize().width*0.4,spt->getPositionY()-30);
		spt_banshou->setPosition(banshou_pt);
		spt_banshou->setAnchorPoint(ccp(0.5,0.5));
		spt_banshou->runAction(RepeatForever::create(CCJumpBy::create(1.0f,Vec2::ZERO,10,1)));
		m_RobotNode->addChild(spt_banshou,5,5);
*/
	}
}

void DDZViewCardEffect::RobotCallBack(Object* obj)
{
	m_RobotNode->removeAllChildren();
	NotificationCenter::getInstance()->postNotification(MESSAGE_MENUCLICKED,Integer::create(DDZmenu_OutAgency));
}

void DDZViewCardEffect::runDDZVoiceAndFace( int _type,int _key,int _userID )
{	
	bool voiceIsOpen = UserDefault::getInstance()->getBoolForKey("voiceIsOpen",true);
	bool faceIsOpen = UserDefault::getInstance()->getBoolForKey("faceIsOpen",true);
	Vec2 point	=getEffectPos(_userID);
	if (_type==1 && faceIsOpen)		//--表情
	{
		addPictureEffect(CCString::createWithFormat("face_%d.png",_key)->getCString(),point);
	}
	else if (_type ==2 && voiceIsOpen)	//--语音
	{
		Color3B color = ccc3(255,255,255);
		addEffect(CCString::createWithFormat("wav%d",_key)->getCString(),point,color,20,true);
		SoundEffect::sharedSoundEffect()->playEffect(CCString::createWithFormat("wav%d.mp3",_key)->getCString(),_userID);
	}
}

void DDZViewCardEffect::runVoice(string voiceString,int _userID )
{
	bool voiceIsOpen = UserDefault::getInstance()->getBoolForKey("voiceIsOpen",true);
	Vec2 point	=getEffectPos(_userID);
	if(voiceIsOpen)
	{
		Color3B color = ccc3(255,255,255);
		addUserInputMsgEffect(voiceString.c_str(),point,color,20,true);
		//SoundEffect::sharedSoundEffect()->playEffect(CCString::createWithFormat("wav%d.mp3",_key)->getCString(),_userID);
	}
}

cocos2d::Vec2 DDZViewCardEffect::getEffectPos( int _userID )
{
	cocos2d::Size size=Director::getInstance()->getWinSize();
	Vec2 point;
	switch(_userID)
	{
	case 0://上家
		point = ccp(size.width*0.3,size.height*0.58);
		break;
	case 1://自己
		point = ccp(size.width*0.5,size.height*0.4);
		break;
	case 2://下家
		point = ccp(size.width*0.7,size.height*0.58);
		break;
	}
	return point;
}

cocos2d::Vec2 DDZViewCardEffect::getPokerKindPos(int _userID)
{
	cocos2d::Size size=Director::getInstance()->getWinSize();
	Vec2 point;
	switch(_userID)
	{
	case 0://上家
		point = ccp(size.width*0.3,size.height*0.635);
		break;
	case 1://自己
		point = ccp(size.width*0.5,size.height*0.435);
		break;
	case 2://下家
		point = ccp(size.width*0.7,size.height*0.635);
		break;
	}
	return point;
}

void DDZViewCardEffect::effectAlarm(int userId)
{
	cocos2d::Size size=Director::getInstance()->getWinSize();
	Vec2 point;
	switch(userId)
	{
	case 0://上家
		{
			if (m_leftAlarm)
			{
				return;
			}
			point = ccp(size.width * 0.11,size.height * 0.62);
			//Array * animFrames = new Array(6);
			Vector<SpriteFrame *> animFrames;
			for(int i=0;i<6;i++)
			{
				CCString str;
				str.initWithFormat("jingbao%d.png",i);
				SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
				//animFrames->addObject(frame);
				animFrames.pushBack(frame);
			}
			CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
			//Sprite * jingBaoSprite = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames->objectAtIndex(0));
			Sprite * jingBaoSprite = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames.at(0));
			jingBaoSprite->runAction(RepeatForever::create(Animate::create(animation)));
			jingBaoSprite->setPosition(point);
			jingBaoSprite->setScale(0.4f);
			this->addChild(jingBaoSprite,0,100);
			jingBaoSprite->setName(DDZ_ALARM_NAME_LAST);
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_warning);
			m_leftAlarm = true;
		}
		break;
	case 1://自己
		point = ccp(size.width*0.5,size.height*0.3);
		break;
	case 2://下家
		{
			if (m_rightAlarm)
			{
				return;
			}
			point = ccp(size.width * 0.89,size.height * 0.62);
			//Array * animFrames = new Array(6);
			Vector<SpriteFrame *> animFrames;
			for(int i=0;i<6;i++)
			{
				CCString str;
				str.initWithFormat("jingbao%d.png",i);
				SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
				//animFrames->addObject(frame);
				animFrames.pushBack(frame);
			}
			CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
			//Sprite * jingBaoSprite = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames->objectAtIndex(0));
			Sprite * jingBaoSprite = Sprite::createWithSpriteFrame((SpriteFrame*)animFrames.at(0));
			jingBaoSprite->runAction(RepeatForever::create(Animate::create(animation)));
			jingBaoSprite->setPosition(point);
			jingBaoSprite->setScale(0.4f);
			this->addChild(jingBaoSprite,0,101);
			jingBaoSprite->setName(DDZ_ALARM_NAME_NEXT);
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_warning);
			m_rightAlarm = true;
		}
		break;
	}
}

void DDZViewCardEffect::runSettlementEffect(Integer * obj,int tableConsume,DDZCardTableLayer * layer)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	g_cardTableLayer = layer;
	//this->removeAllChildren();
	//金币动画
	removeAlarm();//移除警报
 	DDZCCSettmentUserMessage * user = (DDZCCSettmentUserMessage*)obj;
 	Vec2 point = getEffectPos(user->m_userId);
	CCString str;
	if (user->m_updScore>0)
	{
		str.initWithFormat(":%d",user->m_updScore-tableConsume);
	}
	else
	{
		str.initWithFormat(";%d",user->m_updScore-tableConsume);
	}
	LabelAtlas * labelAtlas = LabelAtlas::create(str.getCString(),DDZtexture_name::s_coinNumber1,16,22,'0');
	labelAtlas->setAnchorPoint(ccp(0.5f,0.5f));
	labelAtlas->setScale(1.2f);
	labelAtlas->setPosition(point);
// 	switch(user->m_userId)
// 	{
// 	case 0:
// 		{
// 			labelAtlas->runAction(Sequence::create(MoveTo::create(0.4f,ccp(point.x,point.y+40.0f)),
// 				DelayTime::create(6.0f),
// 				Spawn::create(MoveTo::create(1.0f,ccp(100.0f,400.0f)),ScaleTo::create(1.0f,0.3f,0.3f),NULL),
// 				CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
// 		}
// 		break;
// 	case 1:
// 		{
// 			labelAtlas->runAction(Sequence::create(MoveTo::create(0.4f,ccp(point.x,point.y+40.0f)),
// 				DelayTime::create(6.0f),
// 				CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
// 		}
// 		break;
// 	case 2:
// 		{
// 			labelAtlas->runAction(Sequence::create(MoveTo::create(0.4f,ccp(point.x,point.y+40.0f)),
// 				DelayTime::create(6.0f),
// 				Spawn::create(MoveTo::create(1.0f,ccp(754.0f,400.0f)),ScaleTo::create(1.0f,0.3f,0.3f),NULL),
// 				CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
// 		}
// 		break;
// 	}
	labelAtlas->runAction(Sequence::create(MoveTo::create(0.4f,ccp(point.x,point.y+40.0f)),
		DelayTime::create(6.0f),
		CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),NULL));
	this->addChild(labelAtlas);
	//左下角金币变化动画
	if (user->m_userId==1)
	{
		g_udpCoinNumber = user->m_updScore-tableConsume;
		LabelAtlas * udpAtlas = LabelAtlas::create(str.getCString(),DDZtexture_name::s_coinNumber1,16,22,'0');
		udpAtlas->setAnchorPoint(ccp(0.5f,0.5f));
		udpAtlas->setPosition(ccp(size.width/2,90.0f));
		udpAtlas->runAction(Sequence::create(MoveTo::create(1.0f,ccp(size.width/2,30.0f)),
			CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),
			CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::changeCoinNumber)),NULL));
		this->addChild(udpAtlas);
	}

	//输赢动画
	if(user->m_userId==1)
	{
		cocos2d::Size size = Director::getInstance()->getWinSize();
		if (user->m_updScore>0)
		{
			Sprite * sprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_winner);
			sprite->setPosition(ccp(size.width/2,size.height/2));
			sprite->runAction(Sequence::create(FadeIn::create(0.2f),CCJumpBy::create(0.4f,ccp(0,-10),20.0f,2),NULL));
			this->addChild(sprite);
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_winner);
		}
		if (user->m_updScore<0)
		{
			Sprite * sprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_loser);
			sprite->setPosition(ccp(size.width/2,size.height/2));
			sprite->runAction(Sequence::create(FadeIn::create(0.2f),CCJumpBy::create(0.4f,ccp(0,-10),20.0f,2),NULL));
			this->addChild(sprite);
			DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_loser);
		}
	}
}

void DDZViewCardEffect::removeAlarm()
{
	if(this->getChildByTag(100))
	{
		this->getChildByTag(100)->removeFromParent();
	}
	if(this->getChildByTag(101))
	{
		this->getChildByTag(101)->removeFromParent();
	}
}

void DDZViewCardEffect::changeCoinNumber(Node * node)
{
	g_cardTableLayer->setCoinNumber(g_cardTableLayer->getCoinNum()+g_udpCoinNumber);
}

void DDZViewCardEffect::runHfdEffect(Integer * obj)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	DDZCChuafeidian * hfd = (DDZCChuafeidian*)obj;
	g_hfdCoinNumber = hfd->m_huafeidian;
	CCString str;
	str.initWithFormat(":%d",g_hfdCoinNumber);

	LabelAtlas * udpAtlas = LabelAtlas::create(str.getCString(),DDZtexture_name::s_coinNumber1,16,22,'0');
	udpAtlas->setAnchorPoint(ccp(0.5f,0.5f));
	udpAtlas->setPosition(ccp(size.width-udpAtlas->getContentSize().width/2-15,90.0f));
	udpAtlas->runAction(Sequence::create(MoveTo::create(1.0f,ccp(size.width-udpAtlas->getContentSize().width/2-15,30.0f)),
		CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::removerSelf)),
		/*CallFuncN::create(this,callfuncN_selector(DDZViewCardEffect::changeHfdNumber)),*/NULL));
	this->addChild(udpAtlas);
}

/*
void DDZViewCardEffect::changeHfdNumber(Node * node)
{
	g_cardTableLayer->setHuaFeiDian(g_cardTableLayer->getHhdNumber()+g_hfdCoinNumber);
}
*/

// CCAnimation * DDZViewCardEffect::getAnimation(const char * key,int frameNum)
// {
//  	Array * animFrames = new Array(frameNum);
//  	for (int i=0;i<frameNum;i++)
//  	{
//  		CCString str;
//  		str.initWithFormat("%s%d.png",key,i);
//  		SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
//  		animFrames->addObject(frame);
//  	}
//  	CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
// 	return animation;
// }



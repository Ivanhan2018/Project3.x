#include "Settlement.h"
#include "SceneData.h"
#include "SoundControl.h"

Settlement::Settlement()
{

}

Settlement::~Settlement()
{

}

bool Settlement::init()
{
	if (!Node::init())
	{
		return false;
	}
	do 
	{
		m_bgLeftSprite = Sprite::createWithSpriteFrameName(texture_name::s_settlementbg);
		m_bgLeftSprite->setAnchorPoint(ccp(1.0f,0.5f));
		m_bgLeftSprite->getTexture()->setAntiAliasTexParameters();
		m_bgLeftSprite->setPositionX(0.5f);
		this->addChild(m_bgLeftSprite);
		Sprite * bgRightSprite = Sprite::createWithSpriteFrameName(texture_name::s_settlementbg);
		bgRightSprite->setFlipX(true);
		bgRightSprite->setAnchorPoint(ccp(0,0.5f));
		bgRightSprite->getTexture()->setAntiAliasTexParameters();
		bgRightSprite->setPositionX(-0.5f);
		m_batchNode = SpriteBatchNode::create(texture_name::s_cardTable);
		this->addChild(bgRightSprite);
		this->addChild(m_batchNode);
	} while (0);
	return true;
}

void Settlement::addString(char* key,Vec2 point,Color3B  color,int fontSize,int tag,int num)
{
	Dictionary * strings = Dictionary::createWithContentsOfFile("strings.xml");//by hxh
	String str;
	str.initWithFormat("%s%d",((String*)strings->objectForKey(key))->getCString(),num);
	LabelTTF * pLableTTF = LabelTTF::create(str.getCString(),"Arial",fontSize);
	pLableTTF->setColor(color);
	pLableTTF->setPosition(point);
	this->addChild(pLableTTF,1,tag);
}

void Settlement::setUserMessage(Integer * obj)
{
	CCSettmentUserMessage * user = (CCSettmentUserMessage*)obj;
	Color3B color = ccc3(161,84,32);
	switch(user->m_userId)
	{
	case 0:
		{
			if(1==SoundEffect::sharedSoundEffect()->m_leftUser.gender)
			{
				addUserString(texture_name::s_man_head_icon,50.0f,color,18,user->m_updScore,user->m_nikeName,user->m_isJiabei,user->m_userId==user->m_diZhuId?true:false,false);
			}
			else
			{
				addUserString(texture_name::s_woman_head_icon,50.0f,color,18,user->m_updScore,user->m_nikeName,user->m_isJiabei,user->m_userId==user->m_diZhuId?true:false,false);
			}
			
		}
		break;
	case 1:
		{
			if (1==SoundEffect::sharedSoundEffect()->m_self.gender)
			{
				addUserString(texture_name::s_man_head_icon,100.0f,color,18,user->m_updScore,user->m_nikeName,user->m_isJiabei,user->m_userId==user->m_diZhuId?true:false,true);
			}
			else
			{
				addUserString(texture_name::s_woman_head_icon,100.0f,color,18,user->m_updScore,user->m_nikeName,user->m_isJiabei,user->m_userId==user->m_diZhuId?true:false,true);
			}
			
		}
		break;
	case 2:
		{
			if (1==SoundEffect::sharedSoundEffect()->m_rightUser.gender)
			{
				addUserString(texture_name::s_man_head_icon,0,color,18,user->m_updScore,user->m_nikeName,user->m_isJiabei,user->m_userId==user->m_diZhuId?true:false,false);
			}
			else
			{
				addUserString(texture_name::s_woman_head_icon,0,color,18,user->m_updScore,user->m_nikeName,user->m_isJiabei,user->m_userId==user->m_diZhuId?true:false,false);
			}
		}
		break;
	}
}

void Settlement::setTimesMessage(CMD_S_GameEnd* _gameEnd,WORD _userID)
{
	float yPos1 = -50.0f;
	float yPos2 = -90.0f;
	float fontSize1 = 18;
	float fontSize2 = 20;
 	int NumData=0;
	Color3B color = ccc3(161,84,32);
	//明牌
	//NumData=1;
	//for(int i=0;i<3;i++)
	//{
	//	if (_gameEnd->bOpenCard[i]>0)
	//	{
	//		NumData *=_gameEnd->bOpenCard[i];
	//	}
	//}
	//addString("showcard",ccp(-175.0f,-40.0f),color,14,tag_showcard,NumData>1?NumData:0);

	//加倍
//  	NumData=1;
// 	for(int i=0;i<3;i++)
//// 	{
//// 		if (_gameEnd->bAdd[i])
//// 		{
//// 			NumData *=2;
//// 		}
//// 	}
//// 	addString("jiabei",ccp(-60.0f,-40.0f),color,14,tag_jiabei,NumData);
//
//	//抢地主
//	NumData=0;
//	for(int i=0;i<3;i++)
//	{
//		if (_gameEnd->bLandLand[i]==4){
//			NumData +=2;
//		}else if(_gameEnd->bLandLand[i]>=2){
//			NumData +=1;
//		}else{
//		}
//	}
//	addString("qiangdizhu",/*ccp(60.0f,-40.0f)*/ccp(-113.0f,yPos1),color,fontSize1,tag_qiangdizhu,_gameEnd->bLandLand[0]);
//	//炸弹
//	if (_gameEnd->wBombNum<=0)
//	{
//		addString("bomb",/*ccp(180.0f,-40.0f)*/ccp(0,yPos1),color,fontSize1,tag_bomb,0);
//	}else
//	{
//		addString("bomb",/*ccp(180.0f,-40.0f)*/ccp(0,yPos1),color,fontSize1,tag_bomb,_gameEnd->wBombNum);
//	}
//	
//	//春天
//	addString("spring",/*ccp(-180.0f,-75.0f)*/ccp(113.0f,yPos1),color,fontSize1,tag_spring,_gameEnd->bSpring==true?2:0);
//	//底牌类型
//	//addString("lowcardtype",/*ccp(-60.0f,-75.0f)*/ccp(-175.0f,-75.0f),color,14,tag_lowcardtype,_gameEnd->backType);
//	//结束牌型
//	//addString("overcardtype",/*ccp(60.0f,-75.0f)*/ccp(-55.0f,-75.0f),color,14,tag_overcardtype,_gameEnd->taskType);
//	//底分
//	addString("basescore",/*ccp(180.0f,-75.0f)*/ccp(-75.0f,yPos2),color,fontSize2,tag_basescore,_gameEnd->lCellScore);
//	//总倍率
//	addString("total",ccp(75,yPos2),color,fontSize2,tag_total,_gameEnd->allBeillu[0]);
//	//税收
//	//addString("tableconsume",ccp(-165.0f,-100.0f),color,14,tag_tableconsume,_gameEnd->lGameTax[_userID]);

}

void Settlement::addUserString(const char * res,float pointY,Color3B color,int fontSize,long udpScore,const char * nikeName,bool isJiaBei,bool isDiZhu,bool isSelf)
{
 	Sprite * userIcon = Sprite::createWithSpriteFrameName(res);
 	userIcon->setPosition(ccp(-180.0f,pointY));
	userIcon->setScale(0.6f);
 	m_batchNode->addChild(userIcon);
	if (isJiaBei)
	{
		//Sprite * jiaBei = Sprite::createWithSpriteFrameName(texture_name::s_jiaBeiTag);
		//jiaBei->setPosition(ccp(40.0f,pointY));
		//m_batchNode->addChild(jiaBei);
	}
	if (isSelf)
	{
		//Sprite * showSelf = Sprite::createWithSpriteFrameName(texture_name::s_showSelf);
		//showSelf->setPosition(ccp(0,pointY));
		//m_batchNode->addChild(showSelf);
	}
	if (isDiZhu)
	{
		//Sprite * diZhuTag = Sprite::createWithSpriteFrameName(texture_name::s_diZhuTag);
		//diZhuTag->setPosition(ccp(-180.0f,pointY));
		//m_batchNode->addChild(diZhuTag);
	}
	LabelTTF * nikeNameTTF = LabelTTF::create(nikeName,"Arial",fontSize);
	nikeNameTTF->setColor(color);
	nikeNameTTF->setPosition(ccp(-75.0f,pointY));
	this->addChild(nikeNameTTF);
	String str;
	if (udpScore>=0)
	{
		str.initWithFormat("+%ld",udpScore);
	}else
	{
		str.initWithFormat("%ld",udpScore);
	}
	
	LabelTTF * udpScoreTTF = LabelTTF::create(str.getCString(),"Arial",fontSize);
	udpScoreTTF->setColor(color);
	udpScoreTTF->setPosition(ccp(130.0f,pointY));
	this->addChild(udpScoreTTF);
	
}
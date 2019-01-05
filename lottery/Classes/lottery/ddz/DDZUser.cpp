#include "DDZUser.h"
#include "DDZMyTools.h"
#include "CocosUnits.h"//by hxh
#include "DDZUserInfoManager.h"
#include "EntityMgr.h"
#include "ToolKit.h"

DDZUser::DDZUser()
	:m_IsGetLord(false),m_IsLord(false),
	m_isLeave(true)
{
	m_lnUserID = 0;
	m_nViewID = -1;
}

DDZUser::~DDZUser()
{

}

bool DDZUser::init()
{
	bool ret = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

		
		m_noUser = Sprite::create("ddz_no_user_icon.png");
		this->addChild(m_noUser);
		m_noUser->setScale(0.8);
		m_noUser->setVisible(false);
		
		if (DDZUserInfoManager::getInstance()->m_gender == 1)
		{
			m_userIcon=Sprite::createWithSpriteFrame(spriteFrame(DDZtexture_name::s_usericon));
		}
		else
		{
			m_userIcon = Sprite::createWithSpriteFrame(spriteFrame(DDZtexture_name::s_womanIcon));
		}
		
		m_userIcon->setAnchorPoint(ccp(0.5f,0.5f));
		m_userIcon->setPosition(ccp(-500,-500));
		m_userIcon->setVisible(false);
		m_userIcon->setScale(0.8);
		this->addChild(m_userIcon);

		m_userInfor=Sprite::createWithSpriteFrame(spriteFrame(DDZtexture_name::s_userMessage));
		m_userInfor->setAnchorPoint(ccp(0.5f,0.5f));
		this->addChild(m_userInfor, 999);
		m_userInfor->setVisible(false);
		m_pos =ccp(0,0);
		m_UserSptKey=0;
		m_userEye=NULL;

		m_pImgReady = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddz_img_text_ready);
		addChild(m_pImgReady);
		m_pImgReady->setPosition(Vec2(sVSize.width * 0.85, sVSize.height * 0.5));
		m_pImgReady->setVisible(false);

		setTouchEnabled(true);
		setTouchMode(kCCTouchesOneByOne);

		m_isOut=true;
		m_isOnline=true;

		ret = true;
	} while (0);

	return ret;
}

void DDZUser::setIsOnline(bool isOnline)
{
	m_isOnline=isOnline;
	if (isOnline)
	{
		m_userIcon->setColor(ccc3(255,255,255));
		if(m_userEye)m_userEye->setColor(ccc3(255,255,255));

		if (m_userIcon->getChildByTag(1031))
		{
			m_userIcon->getChildByTag(1031)->setVisible(false);
		}
	}
	else
	{
		m_userIcon->setColor(ccc3(100,100,100));
		if(m_userEye)m_userEye->setColor(ccc3(100,100,100));

		if (m_userIcon->getChildByTag(1031))
		{
			m_userIcon->getChildByTag(1031)->setVisible(true);
			return;
		}

		Dictionary * strings = Dictionary::createWithContentsOfFile("ddz_strings.xml");
		std::string text_offline = ((CCString*)strings->objectForKey("offline_text"))->getCString();
		LabelTTF* text=LabelTTF::create(text_offline, "", 30);
		text->setAnchorPoint(ccp(0.5f,0.5f));
		//text->setColor(ccc3(255,255,255));
		text->setPosition(m_userIcon->getContentSize().width /2, m_userIcon->getContentSize().height * 0.2);
		text->setTag(1031);
		if (m_pos.x>Director::getInstance()->getWinSize().width*0.5)
			text->setRotationY(180.0f);
		m_userIcon->addChild(text, 100);
	}
}

void DDZUser::setUserIcon(int gender)
{
	auto size = Director::getInstance()->getWinSize();
	m_userInfor->setPosition(m_userInfoPos);
	
	if (m_noUser)
	{
		m_noUser->setVisible(false);
	}
	
    if(m_userIcon){
		m_userIcon->removeFromParent();
	}
	else
	{
	    CCLOG("!m_userIcon 1");
	}
	m_userEye=NULL;

	if (gender==1)	//--男
	{
		m_UserSptKey=1;
		m_userIcon = Sprite::createWithSpriteFrame(spriteFrame(DDZtexture_name::s_usericon));
	}
	else
	{
		m_UserSptKey=2;
		m_userIcon = Sprite::createWithSpriteFrame(spriteFrame(DDZtexture_name::s_womanIcon));
	}
	if(!m_userIcon){
		CCLOG("!m_userIcon 2");
	    return;
	}
	if (m_pos.x>Director::getInstance()->getWinSize().width*0.5)
		m_userIcon->setRotationY(180.0f);
	m_userIcon->setScale(0.8f);
	m_userIcon->setPosition(m_pos);
	addChild(m_userIcon);
	eyeCallUp(NULL);

	setIsOnline(m_isOnline);
	m_isOut=true;
	if (m_nViewID == 1)
	{
		return;
	}
	//cocos2d::Size size=Director::getInstance()->getWinSize();
	Vec2 pt=m_userInfoPos;
	Vec2 posPt=m_userInfoPos;
	if (m_pos.x>size.width*0.5)
	{
		pt.x+=(m_userInfor->getContentSize().width+100);
		posPt.x-=100;
	}
	else
	{
		pt.x-=(m_userInfor->getContentSize().width+100);
		posPt.x+=100;
	}
	m_userInfor->stopAllActions();
	m_userInfor->setVisible(true);
	m_userInfor->setPosition(posPt);
	m_userInfor->runAction(Sequence::create(DelayTime::create(5.0f),MoveTo::create(0.2f,pt),NULL));
}

void DDZUser::setLordIcon( bool blord )
{
	
	if (m_noUser)
	{
		m_noUser->setVisible(false);
	}
	
	this->m_IsGetLord = true;
	this->m_IsLord = blord;
	m_userInfor->setPosition(m_pos);
	m_userIcon->stopAllActions();
	m_userIcon->removeFromParent();
	m_userEye=NULL;

	m_userIcon=Sprite::createWithSpriteFrameName(DDZtexture_name::s_usericon);
	SpriteFrame * frame=NULL;
	//Array * animFrames = new Array(6);
	Vector<SpriteFrame *> animFrames;
	for(int i=0;i<5;i++)
	{
		CCString str;
		str.initWithFormat("yanwu%d.png",i);
		frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
		//animFrames->addObject(frame);
		animFrames.pushBack(frame);
	}
	if (blord)	//--地主
	{	
		if (m_gender==1)
		{
			m_UserSptKey=3;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_dizhu_man);
		}
		else
		{
			m_UserSptKey=4;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_dizhu_woman);
		}
	}
	else
	{
		if (m_gender==1)
		{
			m_UserSptKey=5;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_farmer_man);
		}
		else
		{
			m_UserSptKey=6;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_farmer_woman);
		}
	}
	//animFrames->addObject(frame);
	animFrames.pushBack(frame);
	CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
	m_userIcon->setScale(0.8f);
	m_userIcon->runAction(Sequence::create(Animate::create(animation),CCCallFuncO::create(this,callfuncO_selector(DDZUser::eyeCallUp),NULL),NULL));
	m_userIcon->setPosition(m_pos);
	this->addChild(m_userIcon);
	if (m_pos.x>Director::getInstance()->getWinSize().width*0.5)
		m_userIcon->setRotationY(180.0f);

	setIsOnline(m_isOnline);
	m_isOut=false;
	userIconCallBack(NULL);
}

void DDZUser::userIconCallBack( Object* obj )
{
	if (m_nViewID == 1)
	{
		return;
	}
	m_isOut=!m_isOut;

	cocos2d::Size size=Director::getInstance()->getWinSize();
	Vec2 pt=m_userInfoPos;
	Vec2 posPt=m_userInfoPos;
	m_pos;
	//pt = m_userInfor->getPosition();
	//posPt = m_userInfor->getPosition();
	//pt.y+=26;
	//posPt.y+=26;
	if (m_pos.x>size.width*0.5)
	{
		pt.x+=(m_userInfor->getContentSize().width+100);
		posPt.x-=100;
	}
	else
	{
		pt.x-=(m_userInfor->getContentSize().width+100);
		posPt.x+=100;
	}
	m_userInfor->stopAllActions();
	m_userInfor->setVisible(true);
	if (m_isOut)
	{
		//m_userInfor->setPosition(posPt);
		m_userInfor->runAction(MoveTo::create(0.2f,pt));
	}
	else
	{
		m_userInfor->setPosition(pt);
		m_userInfor->runAction(Sequence::create(MoveTo::create(0.2f,posPt),DelayTime::create(3.0f),MoveTo::create(0.2f,pt),NULL));
	}
}

bool DDZUser::onTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if (m_userIcon==NULL)return false;
	if (m_nViewID == 1)
	{
		return false;
	}
	if (m_isLeave)
	{
		return false;
	}
	Vec2 pt=pTouch->getLocation();
	if (m_userIcon->boundingBox().containsPoint(pt))
	{
		userIconCallBack(NULL);
	}
	return false;
}

//	设置用户信息
void DDZUser::setUserInfor(DDZCCuserInfor* pInfor)
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	this->setVisible(true);
	auto selfUserID = EntityMgr::instance()->roomFrame()->getMeUserDate()->dwUserID;
	string sName = pInfor->m_name;
	if (pInfor->m_userInforID != selfUserID)
	{
		sName = ToolKit::dealStringOfPrivacy(sName);
	}

	setScore(pInfor->m_score);
	setGender(pInfor->m_gender);
	setUserID(pInfor->m_userInforID);
	if (!m_IsGetLord)
	{
		setUserIcon(pInfor->m_gender);
	}
	else
	{
		setLordIcon(m_IsLord);
	}
	this->m_IsGetLord = false;

	if (m_nViewID == 0)
	{
		m_pImgReady->setPosition(Vec2(sVSize.width * 0.15, sVSize.height * 0.5));
	}
	else if (m_nViewID == 2)
	{
		m_pImgReady->setPosition(Vec2(sVSize.width * 0.85, sVSize.height * 0.5));
	}
	if (m_nViewID == 1)
	{
		return;
	}
	m_userInfor->removeAllChildren();
	cocos2d::Size size=m_userInfor->getContentSize();
	LabelTTF* laberName=LabelTTF::create(sName, "", 22);
	laberName->setAnchorPoint(ccp(0.5f,0.5f));
	laberName->setColor(ccc3(255,255,255));
	laberName->setPosition(ccp(size.width*0.5 + 27,size.height*0.67));
	m_userInfor->addChild(laberName);

	LabelTTF* goldname=LabelTTF::create(Value(pInfor->m_score).asString(), "", 22);
	goldname->setAnchorPoint(ccp(0.5f,0.5f));
	goldname->setColor(ccc3(255, 255, 255));
	goldname->setPosition(ccp(size.width*0.5 + 27,size.height*0.45));
	m_userInfor->addChild(goldname);

	LabelTTF* huafeidianname=LabelTTF::create(pInfor->m_PosName, "", 22);
	huafeidianname->setAnchorPoint(ccp(0.5f,0.5f));
	huafeidianname->setColor(ccc3(255,255,255));
	huafeidianname->setPosition(ccp(size.width*0.5 + 27,size.height*0.22));
	m_userInfor->addChild(huafeidianname);

}

void DDZUser::eyeCallUp( Object* _obj )
{
	if (m_userIcon==NULL)return;
	if (m_userEye)
	{
		m_userEye->stopAllActions();
		m_userEye->removeFromParent();
	}

	SpriteFrame * frame=NULL;
	//Array * animFrames = new Array(3);
	Vector<SpriteFrame *> animFrames;
	Vec2 pt;
	for(int i=0;i<3;i++)
	{
		CCString str;
		switch (m_UserSptKey)
		{
		case 1:
			str.initWithFormat("m_eye%d.png",i);
			pt=ccp(55,126);
			break;
		case 2:
			str.initWithFormat("wm_eye%d.png",i);
			pt=ccp(55+12,125);
			break;
		case 3:
			str.initWithFormat("dz_m_eye%d.png",i);
			pt=ccp(54,126+38+12+3);
			break;
		case 4:
			str.initWithFormat("dz_wm_eye%d.png",i);
			pt=ccp(54.5,126+21);
			break;
		case 5:
			str.initWithFormat("fm_m_eye%d.png",i);
			pt=ccp(55+1,126+38);
			break;
		case 6:
			str.initWithFormat("fm_wm_eye%d.png",i);
			pt=ccp(55+12,126+36);
			break;
		}
		frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
		//animFrames->addObject(frame);
		animFrames.pushBack(frame);
	}
	CCAnimation * animation = CCAnimation::createWithSpriteFrames(animFrames,0.2f);
	m_userEye=Sprite::createWithSpriteFrame(frame);
	m_userEye->setAnchorPoint(ccp(0,0));
	m_userEye->setPosition(pt);
	m_userEye->runAction(Sequence::create(Animate::create(animation),DelayTime::create(rand()%10),CCCallFuncO::create(this,callfuncO_selector(DDZUser::eyeCallUp),NULL),NULL));
	m_userIcon->addChild(m_userEye);
	setIsOnline(m_isOnline);
}

void DDZUser::setUserChairId(int id)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	if (m_noUser)
	{
		//m_noUser->setTexture("ddz_no_user_icon.png");

		switch (id)
		{
		case 0:
			m_noUser->setPosition(ccp(60.0f,size.height/2+80.0f));
			m_noUser->setVisible(true);
			m_userIcon->setVisible(false);
			break;
		case 1:
			{
				m_userIcon->setPosition(ccp(72, 118));
				m_userIcon->setVisible(true);
				m_noUser->setVisible(false);
			}
			//m_noUser->setVisible(false);
			break;
		case 2:
			m_noUser->setPosition(ccp(size.width-60,size.height/2+80.0f));
			
			m_noUser->setVisible(true);
			m_userIcon->setVisible(false);
			break;
		default:
			break;
		}
	}
}

void DDZUser::setUserLeave(int userId, bool isLeave)
{
	this->m_isLeave = isLeave;
	if (isLeave)
	{
		m_noUser->setVisible(true);
		m_userIcon->setVisible(false);
		m_userInfor->setVisible(false);
	}
}

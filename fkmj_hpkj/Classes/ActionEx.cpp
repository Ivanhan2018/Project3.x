#include "ActionEx.h"
#include "utility.h"
#include "WidgetFun.h"
#include "SpriteBlur.h"

NS_CC_BEGIN

MoveExTxtTime* MoveExTxtTime::create(float fDurTime)
{
	return MoveExTxtTime::create(fDurTime,fDurTime,0.f);
}
MoveExTxtTime* MoveExTxtTime::create(float fDurTime,float fromTime,float toTime)
{
	MoveExTxtTime *pRet = new MoveExTxtTime();
	pRet->setStrType(Type_NONE);
	pRet->initWithDuration(fromTime, toTime,fDurTime);
	pRet->autorelease();

	return pRet;
}

bool MoveExTxtTime::initWithDuration(float fromTime,float toTime,float fDurTime)
{
	if (ActionInterval::initWithDuration(utility::fAbs(fDurTime)))
	{
		m_fFromTime = fromTime;
		m_fToTime = toTime;
		return true;
	}

	return false;
}

MoveExTxtTime* MoveExTxtTime::clone() const
{
	auto a = new MoveExTxtTime();
	a->initWithDuration(m_fFromTime, m_fToTime,_duration);
	a->autorelease();
	return a;
}

void MoveExTxtTime::setStrType(Type eType)
{
	m_eStrType = eType;
}
MoveExTxtTime* MoveExTxtTime::reverse() const
{
	return MoveExTxtTime::create(m_fFromTime, m_fToTime,_duration);
}

void MoveExTxtTime::update(float time)
{
	float fTime = m_fFromTime+(m_fToTime-m_fFromTime)*time;
	if (!this->_target)
	{
		return;
	}
	std::string kOutStr;
	if (m_eStrType == Type_NONE)
	{
		kOutStr = utility::toString((int)(fTime));
	}
	else if (m_eStrType == Type_Full)
	{
		kOutStr = utility::toString((int)(fTime/60),":",(int)((int)fTime%60));
	}
	else if (m_eStrType == Type_1Float)
	{
		char kTemp1[20];
		sprintf(kTemp1,"%.1f",fTime);
		kOutStr = kTemp1;
	}
	Label* pLabel = dynamic_cast<Label*>(_target);
	if (pLabel)
	{
		pLabel->setString(utility::toString(kOutStr));
	}
	ui::Text* pTTFLabel = dynamic_cast<ui::Text*>(_target);
	if (pTTFLabel)
	{
		pTTFLabel->setString(utility::toString(kOutStr));
	}
	LabelAtlas* pAtlas = dynamic_cast<LabelAtlas*>(_target);
	if (pAtlas)
	{
		pAtlas->setString(utility::toString(kOutStr));
	}
	
}

MoveExTxtTimeCallBack* MoveExTxtTimeCallBack::create( float fDurTime,float fromTime,float toTime,const std::function<void()>& pCallBack,float fCheakTime /*= 0*/ )
{
	MoveExTxtTimeCallBack *pRet = new MoveExTxtTimeCallBack();
	pRet->setCallBack(pCallBack,fCheakTime);
	pRet->setStrType(MoveExTxtTime::Type_NONE);
	pRet->initWithDuration(fromTime, toTime,fDurTime);
	pRet->autorelease();

	return pRet;
}

void MoveExTxtTimeCallBack::setCallBack( const std::function<void()>& pCallBack,float fCheakTime /*= 0*/ )
{
	m_fDeta = 0.01;
	m_pCallBack = pCallBack;
	m_fCheckTime = fCheakTime;
}

void MoveExTxtTimeCallBack::update( float time )
{
	float fTime = m_fFromTime+(m_fToTime-m_fFromTime)*time;
	if ( utility::Abs(fTime - m_fCheckTime) < m_fDeta)
	{
		m_fDeta = -1;
		m_pCallBack();
	}
	MoveExTxtTime::update(time);
}


CCVisibleAction* CCVisibleAction::create(float duration,bool bVisible)
{
	CCVisibleAction *pRet = new CCVisibleAction();
	pRet->initWithDuration(duration,bVisible);
	pRet->autorelease();

	return pRet;
}

bool CCVisibleAction::initWithDuration(float duration,bool bVisible)
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_bVisible = bVisible;
		return true;
	}

	return false;
}

CCVisibleAction* CCVisibleAction::clone() const
{
	auto a = new CCVisibleAction();
	a->initWithDuration(this->_duration, m_bVisible);
	a->autorelease();
	return a;
}


CCVisibleAction* CCVisibleAction::reverse() const
{
	return CCVisibleAction::create(_duration, m_bVisible);
}
void CCVisibleAction::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
}

ActionInterval* CCVisibleAction::reverse(void)
{
	return CCVisibleAction::create(_duration,m_bVisible);
}


void CCVisibleAction::stop(void)
{
}
void CCVisibleAction::update(float t)
{
	if (t >= 1.0)
	{
		_target->setVisible(m_bVisible);
	}
}


CCRemoveAction* CCRemoveAction::create(float duration)
{
	CCRemoveAction *pRet = new CCRemoveAction();
	pRet->initWithDuration(duration);
	pRet->autorelease();

	return pRet;
}

bool CCRemoveAction::initWithDuration(float duration)
{
	if (ActionInterval::initWithDuration(duration))
	{
		return true;
	}

	return false;
}

CCRemoveAction* CCRemoveAction::clone() const
{
	auto a = new CCRemoveAction();
	a->initWithDuration(this->_duration);
	a->autorelease();
	return a;
}


CCRemoveAction* CCRemoveAction::reverse() const
{
	return CCRemoveAction::create(_duration);
}
void CCRemoveAction::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
}

ActionInterval* CCRemoveAction::reverse(void)
{
	return CCRemoveAction::create(_duration);
}


void CCRemoveAction::stop(void)
{
	if (_target)
	{
		_target->removeFromParent();
	}
}
void CCRemoveAction::update(float t)
{
}


CCTire* CCTire::create(float duration,std::string kTexterName,cocos2d::Node* pNode)
{
	CCTire *pRet = new CCTire();
	pRet->initWithDuration(duration,kTexterName,pNode);
	pRet->autorelease();

	return pRet;
}

bool CCTire::initWithDuration(float duration,std::string kTexterName,cocos2d::Node* pNode)
{
	m_fTireTime = 0;
	if (ActionInterval::initWithDuration(duration))
	{
		m_kTexterName = kTexterName;
		m_pSrcNode = pNode;
		return true;
	}

	return false;
}

CCTire* CCTire::clone() const
{
	auto a = new CCTire();
	a->initWithDuration(_duration, m_kTexterName,m_pSrcNode);
	a->autorelease();
	return a;
}


CCTire* CCTire::reverse() const
{
	return CCTire::create(_duration, m_kTexterName,m_pSrcNode);
}
void CCTire::startWithTarget(Node *pTarget)
{
	m_pSrcNode->retain();
	ActionInterval::startWithTarget(pTarget);
}

ActionInterval* CCTire::reverse(void)
{
	return CCTire::create(_duration,m_kTexterName,m_pSrcNode);
}


void CCTire::stop(void)
{
	if (_target)
	{
		m_pSrcNode->release();
	}
}
void CCTire::update(float t)
{
	float fDTime = cocos2d::CCDirector::getInstance()->getDeltaTime();
	m_fTireTime -= fDTime;
	if (m_fTireTime < 0)
	{
		m_fTireTime = 0.02f;
		cocos2d::Sprite* pSprite = cocos2d::Sprite::create(m_kTexterName);
		if (m_pSrcNode->getParent())
		{
			m_pSrcNode->getParent()->addChild(pSprite);
		}
		pSprite->setPosition(m_pSrcNode->getPosition());
		pSprite->runAction(cocos2d::ScaleTo::create(0.2f,0.1f));
		pSprite->runAction(cocos2d::CCRemoveAction::create(0.2f));
		
	}
}



CCAutoRotation* CCAutoRotation::create()
{
	CCAutoRotation *pRet = new CCAutoRotation();
	pRet->initWithDuration(100.f);
	pRet->autorelease();

	return pRet;
}
CCAutoRotation* CCAutoRotation::clone() const
{
	auto a = new CCAutoRotation();
	a->initWithDuration(_duration);
	a->autorelease();
	return a;
}
CCAutoRotation* CCAutoRotation::reverse() const
{
	return CCAutoRotation::create();
}
void CCAutoRotation::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_kLastPos = pTarget->getPosition();
}

void CCAutoRotation::update(float t)
{
	if (!_target)
	{
		return;
	}
	cocos2d::Vec2 kNodePos = _target->getPosition();
	if (kNodePos == m_kLastPos)
	{
		return;
	}
	float fRotation = utility::getRotation(m_kLastPos,kNodePos);
	_target->setRotation(fRotation);
	m_kLastPos = kNodePos;
}



BlurTo* BlurTo::create(float duration,float fBlur)
{
	BlurTo *pRet = new BlurTo();
	pRet->setDestBlur(fBlur);
	pRet->initWithDuration(duration);
	pRet->autorelease();
	return pRet;
}
void BlurTo::startWithTarget(Node *target)
{
	SpriteBlur* pBlur = dynamic_cast<SpriteBlur*>(target);
	m_fStartBlur = pBlur->getBlurRadius();
	ActionInterval::startWithTarget(target);
}
void BlurTo::setDestBlur(float fValue)
{
	m_fDestBlur = fValue;
}
void BlurTo::update(float time)
{
	float fValue = m_fStartBlur+(m_fDestBlur-m_fStartBlur)*time;
	if (_target)
	{
		SpriteBlur* pBlur = dynamic_cast<SpriteBlur*>(_target);
		pBlur->setBlurRadius(fValue);
	}
}


SizeTo* SizeTo::create(float duration,cocos2d::Size kSize)
{
	SizeTo *pRet = new SizeTo();
	pRet->setSize(kSize);
	pRet->initWithDuration(duration);
	pRet->autorelease();
	return pRet;
}
void SizeTo::setSize(cocos2d::Size kSize)
{
	m_kDestSize = kSize;
}
void SizeTo::startWithTarget(Node *target)
{
	m_kStartSize = target->getContentSize();
	ActionInterval::startWithTarget(target);
}
void SizeTo::update(float time)
{
	cocos2d::Size kSize = m_kStartSize+(m_kDestSize-m_kStartSize)*time;
	if (_target)
	{
		_target->setContentSize(kSize);
	}
}

GravityAction::GravityAction()
	:m_fGravity(0)
	,m_fMoveSpd(0)
{

}

GravityAction* GravityAction::create(float duration,float fGravity)
{
	GravityAction *pRet = new GravityAction();
	pRet->initWithDuration(duration);
	pRet->setGravity(fGravity);
	pRet->autorelease();
	return pRet;
}
void GravityAction::setGravity(float fValue)
{
	m_fGravity = fValue;
}
void GravityAction::update(float time)
{
	float dt = cocos2d::Director::getInstance()->getDeltaTime();
	m_fMoveSpd += dt*m_fGravity;
	if (!_target)
	{
		return;
	}
	float fPosY = _target->getPositionY();
	fPosY-=m_fMoveSpd*m_fMoveSpd*dt;
	_target->setPositionY(fPosY);
}

NS_CC_END
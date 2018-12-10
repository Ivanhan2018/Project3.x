#pragma once

#include "cocos2d.h"

NS_CC_BEGIN

class MoveExTxtTime : public ActionInterval
{
public:
	enum Type
	{
		Type_NONE,
		Type_Full,
		Type_1Float,
	};
public:
    /** initializes the action */
    bool initWithDuration(float fDurTime,float fromTime,float toTime);
    /**
     *  @js NA
     *  @lua NA
	 */
	void setStrType(Type eType);

    virtual void update(float time);

	virtual MoveExTxtTime* clone() const override;
	virtual MoveExTxtTime* reverse(void) const  override;
public:
	/** creates the action */
	static MoveExTxtTime* create(float fDurTime);
    static MoveExTxtTime* create(float fDurTime,float fromTime,float toTime);
protected:
	float	m_fFromTime;
	float	m_fToTime;
	Type	m_eStrType;
};

class MoveExTxtTimeCallBack : public MoveExTxtTime
{
public:
	/**
     *  @js NA
     *  @lua NA
	 */
	void setCallBack(const std::function<void()>& pCallBack,float fCheakTime = 0);

    virtual void update(float time);
public:
	/** creates the action */
	static MoveExTxtTimeCallBack* create(float fDurTime,float fromTime,float toTime,const std::function<void()>& pCallBack,float fCheakTime = 0);
protected:
	float	m_fDeta;
	float	m_fCheckTime;
	std::function<void()> m_pCallBack;
};



class CCVisibleAction : public ActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration,bool bVisible);
    /**
     *  @js NA
     *  @lua NA
     */
	virtual void startWithTarget(Node *pTarget);
    virtual ActionInterval* reverse(void);
    virtual void update(float time);
	virtual void stop(void);
	CCVisibleAction* clone() const;
	CCVisibleAction* reverse() const;
public:
    /** creates the action */
    static CCVisibleAction* create(float duration,bool bVisible);
private:
	bool m_bVisible;
};


class CCRemoveAction : public ActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration);
    /**
     *  @js NA
     *  @lua NA
     */
	virtual void startWithTarget(Node *pTarget);
    virtual ActionInterval* reverse(void);
    virtual void update(float time);
	virtual void stop(void);
	CCRemoveAction* clone() const;
	CCRemoveAction* reverse() const;
public:
    /** creates the action */
    static CCRemoveAction* create(float duration);
private:
};


class CCTire : public ActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration,std::string kTexterName,cocos2d::Node* pNode);
    /**
     *  @js NA
     *  @lua NA
     */
	virtual void startWithTarget(Node *pTarget);
    virtual ActionInterval* reverse(void);
    virtual void update(float time);
	virtual void stop(void);
	CCTire* clone() const;
	CCTire* reverse() const;
public:
    /** creates the action */
    static CCTire* create(float duration,std::string kTexterName,cocos2d::Node* pNode);
private:
	std::string m_kTexterName;
	cocos2d::Node* m_pSrcNode;
	float m_fTireTime;
};

class CCAutoRotation : public ActionInterval
{
public:
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time);
	virtual CCAutoRotation* clone() const override;
	virtual CCAutoRotation* reverse(void) const override;
public:
    /** creates the action */
    static CCAutoRotation* create();
private:
	cocos2d::Vec2 m_kLastPos;
};


class BlurTo : public ActionInterval
{
public:
	static BlurTo* create(float duration,float fBlur);
public:
	void setDestBlur(float fValue);
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;
private:
	float m_fStartBlur;
	float m_fDestBlur;
};

class SizeTo : public ActionInterval
{
public:
	static SizeTo* create(float duration,cocos2d::Size kSize);
public:
	void setSize(cocos2d::Size kSize);
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;
private:
	cocos2d::Size m_kStartSize;
	cocos2d::Size m_kDestSize;
};

class GravityAction : public ActionInterval
{
public:
	GravityAction();
	static GravityAction* create(float duration,float fGravity);
    virtual void update(float time) override;

	void setGravity(float fValue);
protected:
	float m_fGravity;
	float m_fMoveSpd;
};
NS_CC_END

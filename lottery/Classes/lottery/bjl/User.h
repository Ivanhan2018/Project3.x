//  [3/26/2014 xieyi]
#ifndef __USER__
#define __USER__

#include "cocos2d.h"
#include "BJLRes.h"
USING_NS_CC;

class User : public Layer
{
public:
	User();
	~User();
	virtual bool init();
	CREATE_FUNC(User);
	void setUserIcon(int gender);
	void setLordIcon(bool blord);
	void setIsOnline(bool isOnline);//设置是否在线
	void setUserInfor(char* name,int gold,char* huafeidian);
	void userIconCallBack(Object* obj);
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent){};
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent){};
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent){};

	void  eyeCallUp(Object* _obj);
	//void setSpt
private:
	Sprite*					 m_userInfor;//玩家信息
	Sprite*					 m_userEye;
	CC_SYNTHESIZE(Sprite*,m_userIcon,userIcon);
	CC_SYNTHESIZE(Vec2,m_pos,IconPos);
	CC_SYNTHESIZE(int ,m_gender,Gender);//玩家性别,1表示男
	CC_SYNTHESIZE(bool,m_isDiZhu,IsDiZhu);//是不是地主

	CC_SYNTHESIZE(int,m_userId,UserId);//用户Id；
	CC_SYNTHESIZE(int,m_score,Score);//用户分数（也就是金币）
	CC_SYNTHESIZE(int,m_goldEggs,GooldEggs);//话费点
	bool        m_isOnline;
	bool        m_isOut;
	int		  m_UserSptKey;				//12玩家男女，34 地主男女,56农民男女
};

#endif
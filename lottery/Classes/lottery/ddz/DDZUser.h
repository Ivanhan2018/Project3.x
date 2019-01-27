//  [3/26/2014 xieyi]
#ifndef DDZ___USER__
#define DDZ___USER__

#include "cocos2d.h"
#include "DDZRes.h"
#include "DDZSceneData.h"

USING_NS_CC;

class DDZUser : public Layer
{
public:
	DDZUser();
	~DDZUser();
	virtual bool init();
	CREATE_FUNC(DDZUser);
	void setUserIcon(int gender);
	void setLordIcon(bool blord);
	void setIsOnline(bool isOnline);//设置是否在线
	void userIconCallBack(Object* obj);
	virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
	virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){};
	virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){};

	void  eyeCallUp(Object* _obj);

	//	设置用户信息
	void setUserInfor(DDZCCuserInfor* pInfor);

	Sprite* m_pImgReady;	//	准备标识

	Sprite* m_noUser;		//无人物头像

	//	玩家视图ID
	CC_SYNTHESIZE(int ,m_nViewID, ViewID);

	void setUserChairId(int id);

	CC_SYNTHESIZE(Vec2,m_userInfoPos,UserInfoPos);
	CC_SYNTHESIZE(Vec2,m_pos,IconPos);

	void setUserLeave(int userId, bool isLeave);

	bool m_isLeave;				//用户离开牌桌

private:
	Sprite*					 m_userInfor;//玩家信息
	Sprite*					 m_userEye;
	CC_SYNTHESIZE(Sprite*,m_userIcon,userIcon);
	
	CC_SYNTHESIZE(int ,m_gender,Gender);//玩家性别,1表示男
	CC_SYNTHESIZE(bool,m_isDiZhu,IsDiZhu);//是不是地主

	CC_SYNTHESIZE(int long, m_lnUserID, UserID);//用户Id；
	CC_SYNTHESIZE(int,m_score,Score);//用户分数（也就是金币）
	bool        m_isOnline;
	bool        m_isOut;
	int		  m_UserSptKey;				//12玩家男女，34 地主男女,56农民男女

	

	bool			m_IsGetLord;			//是否已经产生了地主

	bool		m_IsLord;				//是否为地主
};

#endif
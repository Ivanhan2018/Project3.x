//  [3/26/2014 xieyi]
#ifndef DDZ___CARDTABLE_LAYER__
#define DDZ___CARDTABLE_LAYER__

#include "cocos2d.h"
#include "DDZRes.h"
#include "DDZSceneData.h"
#include "DDZTimer.h"
#include "DDZUser.h"
USING_NS_CC;

class DDZCardTableLayer : public Layer
{
public:

	DDZCardTableLayer();
	~DDZCardTableLayer();
	CREATE_FUNC(DDZCardTableLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	void setMultiplyingPower(int base,int add);		//设置倍率
	void setCoinNumber(int coinNum);				//设置金币数量
	void waitingMatching(bool isWaiting);			//等待排队
	void setUserIsOnline(int userId,bool userIsOnline);		//设置用户在线状态
	void setUserInfor(DDZCCuserInfor* pInfor);										//设置用户普通头像信息
	void setLordIcon(int userId);										//设置用户地主形象
	void setNickName(const char * nikeName);				//设置昵称
	void setDiFen(int num);							//设置底分

	//	游戏准备相关逻辑处理
	void dealGameReady();

	//	游戏结束相关逻辑处理
	void dealGameOver();

	//	通过用户ID获取用户信息类
	DDZUser* getUserByUserID(long int lnUserID);

	DDZUser * m_userRight;								//下家
	DDZUser * m_userLeft;								//上家
	DDZUser * m_userSelf;								//上家
	Sprite* m_pImgMiddleReady;						//	图片——自家准备

private:

	CC_SYNTHESIZE(int,m_coinNumber,CoinNum); 		//金币数量
	CC_SYNTHESIZE(int,m_hhdNumber,HhdNumber);		//话费点数量
	CC_SYNTHESIZE(int,m_times,Times);				//倍率
	Label* m_pLabNumGold;							//	金币数字
	Label* m_pLabNumDiFen;							//	底分数字
	Label* m_pLabNumMultiple;						//	倍数数字
	LabelTTF * m_nikeName;							//玩家昵称
	SpriteBatchNode * m_pdNode;						//排队动画

	int m_diFen;										//底分
};

#endif

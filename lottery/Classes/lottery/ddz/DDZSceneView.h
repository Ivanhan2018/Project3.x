//  [3/19/2014 wucan]
#ifndef  DDZ___CC_SCENEVIEW_WUCAN_
#define DDZ___CC_SCENEVIEW_WUCAN_
#include "cocos2d.h"
#include "DDZSceneData.h"
#include "DDZViewHandCard.h"
#include "DDZCardTableLayer.h"
#include "DDZViewCardEffect.h"
#include "DDZSettlement.h"
#include "DDZViewTask.h"
#include "DDZSetUpLayer.h"
#include "DDZVoiceAndFace.h"
#include "DDZDialogLayer.h"
#include "DDZSoundControl.h"
#include "DDZGameTaskCommon.h"
USING_NS_CC;

class DDZSceneLogic;
class DDZViewMenuLayer;

class DDZSceneView:public Node
{
public:
	DDZSceneView(void);
	virtual ~DDZSceneView(void);
	CREATE_FUNC(DDZSceneView);
	virtual bool init();
	virtual void onExit();//add by hxh 20160819
	void LoadRes();//add by hxh 20160818
	void OnMessage(Object* obj);
	void receiveMenuClickCallfunc(Object * obj);	//--菜单按钮点击后的回调
	void onBankrupt(Object * obj);							//--游戏破产返回大厅
	void onLlinkOFF(float ft);							//--网络断开返回大厅
	void wangZha(Object *obj);
	bool						m_isLandLord;					//--第一次叫地主

	//add by zengxin 设置是否第一次进行叫地主
	void setIsFistLandLord(bool first);

	CC_SYNTHESIZE(int, m_bankId, BankId);//当前地主位置号,  1的时候为当前玩家，0、2不是
	//--功能函数
private:
	void userInforDeal(Integer* _data);
	void dealSitDown(Integer* _data);
	void startGameDeal();
	void startGameViewDeal(Integer* _data);
	void onBackTaskDeal(Integer* _data);
	void addViewDeal(Integer* _data);
	void sendCardDeal(Integer* _data);
	void openCardDeal(Integer* _data);
	void landLordDeal(Integer* _data);
	void timeConreolDral(Integer* _data);
	void outCardDeal(Integer* _data);
	void passCardDeal(Integer* _data);
	void gameOverDeal(Integer* _data);
	void timeCallBackDeal(Integer* _data);
	void timeAgencyDeal(float ft);
	//	坐下失败
	void onSitFaild(Object* obj);

	//	房间登录失败
	void onLoginFaild(Object* obj);

	//	游戏结束相关逻辑处理
	void dealGameOver();

	//	处理等待状态
	DWORD dealWaitingState();

private:
	DDZViewHandCard*     m_handCardView;
	DDZViewCardEffect*    m_effectView;
	CC_SYNTHESIZE(DDZViewMenuLayer *,m_menuLayer,MenuLayer);
	Layer * m_dialogLayer;
	CC_SYNTHESIZE(DDZCardTableLayer*,m_cardTableLayer,DDZCardTableLayer);
	DDZSceneLogic* m_pSceneLogic;

	int						m_AgencyNum;				//--玩家没打轮数
	bool						m_isAgency;						//--机器人代打

	//--用于游戏托管
	DDZGame_State			m_gameStae;					//--游戏状态
	int						m_timeKey;
	unsigned int m_userOutCardState;				//用户出牌状态（//地主出牌且不能明牌，地主首轮出牌能明牌，上轮最大方，现在第一轮出牌）
	unsigned int g_tableConsume;					//牌桌消费

};
#endif

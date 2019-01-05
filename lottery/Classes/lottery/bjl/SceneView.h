//  [3/19/2014 wucan]
#ifndef  __CC_SCENEVIEW_WUCAN_
#define __CC_SCENEVIEW_WUCAN_
#include "cocos2d.h"
#include "SceneData.h"
#include "ViewHandCard.h"
#include "CardTableLayer.h"
#include "ViewMenuLayer.h"
#include "ViewCardEffect.h"
#include "Settlement.h"
#include "ViewTask.h"
#include "SetUpLayer.h"
#include "VoiceAndFace.h"
#include "DialogLayer.h"
#include "SoundControl.h"
#include "GameTaskCommon.h"
#include "SceneLogic.h"

USING_NS_CC;

class SceneView:public Node
{
public:
	// 切换到竖屏<add by hxh 20160702>
	static void ToPortrait();
	// 切换到横屏<add by hxh 20160702>
	static void ToLandscape();
public:
	SceneView(void);
	~SceneView(void);
	CREATE_FUNC(SceneView);
	virtual bool init();
	virtual void onExit();//add by hxh 20160819
	void LoadRes();//add by hxh 20160617
	void OnMessage(Object* obj);
	void receiveMenuClickCallfunc(Object * obj);	//--菜单按钮点击后的回调
	void onBankrupt(Object * obj);							//--游戏破产返回大厅
	void onLlinkOFF(float ft);							//--网络断开返回大厅
	void onLoginFaild(Object* obj);					//登录失败<by hxh>
	void onSitFaild(Object* obj);					    //坐下失败<by hxh>
    void CloseNetWorkShut(Object *obj);
	void OnBankerListAdd(Object *obj);// 上庄列表增加一条记录<by hxh>
	void OnBankerListDelete(Object *obj);// 上庄列表删除一条记录<by hxh>
	void OnMeApplyBankerSuc(Object *obj);//自己申请上庄成功<by hxh>
	void OnMeUpBankerSuc(Object *obj);//自己上庄成功<by hxh>
	void OnMeCancelUpBanker(Object *obj);//自己取消了申请<by hxh>
	void OnMeDownBanker(Object *obj);//自己下庄<by hxh>
	void SetCloseCall( Object *obj );
	void OnUpBanker( Object *obj );//上庄<by hxh>
	void OnCancelUpBanker( Object *obj );//取消申请<by hxh>
	void OnDownBanker( Object *obj );//下庄<by hxh>
	void OnBankerList( Object *obj );//上庄列表<by hxh>

	//--功能函数
	void ShowCurrentMenu(int j);//<by hxh>

	SceneLogic* m_pSceneLogic;
private:
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
	
	void Strameobjet(Object *obj);
	void StrameNumberbjet(Object *obj);

	WORD m_wBankerUser;						//当前庄家
private:
	ViewCardEffect*    m_effectView;
	CC_SYNTHESIZE(ViewMenuLayer *,m_menuLayer,MenuLayer);
    CC_SYNTHESIZE(int,m_currentMenu,CurrentMenu);//1申请上庄，2取消申请，3我要下庄<by hxh>
	//Settlement * m_settlementNode;
	//VoiceAndFace*     m_face;
	GameTaskCommon* m_gameTask;
	Layer * m_dialogLayer;

	Menu * m_Menu1;
	Menu * m_Menu2;
	Menu * m_Menu3;
	cocos2d::Size winSize;// 设计分辨率大小<by hxh>
	int  cAddScoreUseid;   //庄家id
	bool ScoreUseidbool;
	int						m_AgencyNum;				//--玩家没打轮数
	bool						m_isAgency;						//--机器人代打

	//--用于游戏托管
	Game_State			m_gameStae;					//--游戏状态
	int						m_timeKey;
	bool						m_isLandLord;					//--第一次叫地主
	unsigned int m_userOutCardState;				//用户出牌状态（//地主出牌且不能明牌，地主首轮出牌能明牌，上轮最大方，现在第一轮出牌）
	unsigned int g_tableConsume;					//牌桌消费

};
#endif

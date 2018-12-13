#ifndef  __CC_SCENEVIEW_WUCAN_
#define __CC_SCENEVIEW_WUCAN_
#include "cocos2d.h"
#include "CardTableLayer.h"
#include "ViewMenuLayer.h"
#include "SetUpLayer.h"
#include "DialogLayer.h"
#include "SoundControl.h"

#include "UpBankerList.h"

USING_NS_CC;

class SceneView:public Node
{
public:
	// 切换到竖屏
	static void ToPortrait();
	// 切换到横屏
	static void ToLandscape();
public:
	SceneView(void);
	~SceneView(void);
	CREATE_FUNC(SceneView);
	virtual bool init();
	void LoadRes();
	void OnMessage(Object* obj);
	void receiveMenuClickCallfunc(Object * obj);	//--菜单按钮点击后的回调
	void onBankrupt(Object * obj);							//--游戏破产返回大厅
	void onLlinkOFF(float ft);							//--网络断开返回大厅
	void onLoginFaild(Object* obj);					//登录失败
	void onSitFaild(Object* obj);					    //坐下失败
    void CloseNetWorkShut(Object *obj);
	void OnBankerListAdd(Object *obj);// 上庄列表增加一条记录
	void OnBankerListDelete(Object *obj);// 上庄列表删除一条记录
	void OnMeApplyBankerSuc(Object *obj);//自己申请上庄成功
	void OnMeUpBankerSuc(Object *obj);//自己上庄成功
	void OnMeCancelUpBanker(Object *obj);//自己取消了申请
	void OnMeDownBanker(Object *obj);//自己下庄
	void SetCloseCall( Object *obj );
	void OnUpBanker( Object *obj );//上庄
	void OnCancelUpBanker( Object *obj );//取消申请
	void OnDownBanker( Object *obj );//下庄
	void OnBankerList( Object *obj );//上庄列表

	//--功能函数
	void ShowCurrentMenu(int j);
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

	int longNumberuserInforID;   //庄家Id
private:
	CC_SYNTHESIZE(ViewMenuLayer *,m_menuLayer,MenuLayer);
    CC_SYNTHESIZE(int,m_currentMenu,CurrentMenu);//1申请上庄，2取消申请，3我要下庄
	UpBankerList* m_UpBankerList;
	Layer * m_dialogLayer;

	Menu * m_Menu1;
	Menu * m_Menu2;
	Menu * m_Menu3;
	cocos2d::Size winSize;
	int  cAddScoreUseid;   //庄家id
	bool ScoreUseidbool;
	int						m_AgencyNum;				//--玩家没打轮数
	bool						m_isAgency;						//--机器人代打

	//--用于游戏托管
	int			            m_gameStae;					//--游戏状态
	int						m_timeKey;
	bool						m_isLandLord;					//--第一次叫地主
	unsigned int m_userOutCardState;				//用户出牌状态（//地主出牌且不能明牌，地主首轮出牌能明牌，上轮最大方，现在第一轮出牌）
	unsigned int g_tableConsume;					//牌桌消费

};
#endif

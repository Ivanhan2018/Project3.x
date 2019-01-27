#ifndef		FISH___CC_SCENEDATA_WUCAN__
#define		FISH___CC_SCENEDATA_WUCAN__
#include "cocos2d.h"
USING_NS_CC;
#include "CMD_DDZ.h"
#include "DDZLogic.h"
#include "Define.h"
#define CREATE_FUNC_MSG(__TYPE__) \
static __TYPE__* create(int v) \
{ \
	__TYPE__ *pRet = new __TYPE__(v); \
	pRet->autorelease(); \
	return pRet; \
}

enum DDZGame_State
{
	DDZGame_null = 0,			//无
	DDZGame_Wait,				//等待状态
	DDZGame_Start,				//开始
	DDZGame_landLord,			//抢地主
	DDZGame_jiaBei,				//加倍
	DDZGame_Run,				//打牌
	DDZGame_Over,				//游戏结束
};

enum
{
	FishmsgToView_UserInfor=1,				//--跟新任务信息
	FishmsgToView_SitDown,					//--坐下
	FishmsgToView_start,						//--游戏开始
	FishmsgToView_startView,					//--开始效果表现
	FishmsgToView_addView,					//--加倍表现
	FishmsgToView_sendCard,					//--发牌
	FishmsgToView_backCard,					//--底牌
	FishmsgToView_openCard,					//--明牌
	FishmsgToView_landLord,					//--抢地主
	FishmsgToView_beiLv,						//--倍率变化
	FishmsgToview_LordView,					//--地主表现变化
	FishmsgToView_time,						//	时间		
	FishmsgToView_outCard,					//	出牌
	FishmsgToView_passCard,					//	过牌
	FishmsgToView_gameOver,					//--游戏结束
	FishmsgToView_SettmentUser,				//--游戏结算人物消息
	FishmsgToView_openCardNum,				//--viewHandCard控制
	FishmsgToview_TimeCallBack,				//--时间回调  Time 控制
	FishmsgToView_OnLine,					//--玩家在线提示
	FishmsgToView_Reconnect,					//--断线重连
	FishmsgToView_socketoffConnect,			//--网络断开自动重连
	FishmsgToView_socketoff,					//--网络断开，返回大厅
	FishmsgToView_DDZVoiceAndFace,			//--表情和语音
	FishmsgToView_huafeidian,				//--花费点
	FishmsgToView_Close,						//--强制关闭
	FishmsgToView_RoomLimit,					//--房间限制

	FishmsgToLogic_openStart,				//	明牌开局
	FishmagToLogc_gameStart,					//	普通开局
	FishmsgToLogc_openCard,					//	明牌
	FishmsgToLogc_LandLord,					//	叫地主
	FishmsgToLogic_outCard,					//	出牌
	FishmsgToLogic_passCard,					//	过牌
	FishmsgToLogic_MoreGame,					//	再来一局
	FishmagToLogc_jiabei,					//--加倍
	FishmsgToLogic_face,						//--表情
	FishmsgToLogic_changeTable				//	换桌
};

//牌桌菜单枚举
enum FishMenuItemType
{
	DDZmenu_visibleStartMenuItem = 1,	//明牌开始
	DDZmenu_startMenuItem,				//普通开始
//	menu_menuBarItem,				//菜单栏
	DDZmenu_rechargemenuItem,			//快速充值
	DDZmenu_outCardMenuItem,			//出牌
	DDZmenu_passOutCardMenuItem,		//不要
	DDZmenu_hintMenuItem,				//提示
	DDZmenu_jiaoDiZhuItem,				//叫地主
	DDZmenu_buJiaoItem,					//不叫
	DDZmenu_qiangDiZhuItem,				//抢地主
	DDZmenu_passQiangDiZhuItem,			//不抢
	DDZmenu_visibleCardItem,			//明牌X2
	DDZmenu_doubleItem,					//加倍
	DDZmenu_unDoubleItem,				//不加倍
	DDZmenu_settlementDetial,			//结算详情
	DDZmenu_again,						//再来一局
	DDZmenu_OutAgency,					//取消托管
	DDZmenu_changeTable,				//换桌
	DDZmenu_visibleCardStartItem,		//发牌过程中的明牌开始
	DDZmenu_number,						//明牌开始按钮数字
	DDZmenu_menuBarRobotItem,			//菜单栏托管按钮
	DDZmenu_menuBarOptionsItem,			//菜单栏设置按钮
	DDZmenu_menuBarBackItem,			//菜单栏返回按钮
	DDZmenu_Count						//	总数
};

//--人物信息更新
class FishCCuserInfor:public Integer
{
public:
	FishCCuserInfor(int v):Integer(v){};
	CREATE_FUNC_MSG(FishCCuserInfor);
	DWORD		m_userInforID;		//用户id
	WORD		m_ChairID;		    //椅子id={0,1,2,3,4,5,6,7}
	WORD		m_ViewID;		    //视图id={0左边上家、1下方自己、2右边下家}
	LONG		m_score;			//分数
	WORD		m_gender;			//性别
	LONG		m_GoldEggs;			//话费点
	TUserState	m_UserState;		//	用户状态
	char		m_PosName[32];		//城市
	char        m_name[32];
};

//--明牌开始消息
class FishCCopenStart:public Integer
{
public:
	FishCCopenStart(int v):Integer(v){};
	CREATE_FUNC_MSG(FishCCopenStart);
	WORD				m_bCurrentUser;                     //开始用户 
	bool					m_bOpenCard;							//明牌开始
};

//--发牌消息
class DDZCCsendCard: public Integer
{
public:
	DDZCCsendCard(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCsendCard);
	//	手牌数据
	BYTE m_handCardData[MAX_COUNT];
	//	手牌数目
	WORD m_handCardNum;
	//	是否明牌
	bool m_bOpen;
	//	是否发牌效果
	bool m_bEffect;
	//	玩家ID
	WORD m_userID;
};

//--明牌消息
class DDZCCopenCard: public Integer
{
public:
	DDZCCopenCard(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCopenCard);
	WORD m_openID;
};

//--抢地主
class DDZCClandLord: public Integer
{
public:
	DDZCClandLord(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCClandLord);
	WORD		m_landID;
	WORD		m_LandKey;
	bool			m_isLand;
};

//--显示时钟
class DDZCCtimeControl:public Integer
{
public:
	DDZCCtimeControl(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCtimeControl);
	WORD		m_timeUserID;
	WORD		m_timeNum;
	DDZGame_State	m_gameState;			//--Game_landLord <2 为叫地主 >2为抢地主
	WORD		m_timeKey;					//--Game_Run 0为地主首次出牌并且能明牌，1为地主首次出牌不能明牌，2为自己首次出牌，3为普通出牌			
	WORD		m_BankUser;
};

//--玩家出牌
class DDZCCoutCard:public Integer
{
public:
	DDZCCoutCard(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCoutCard);
	BYTE		m_outkCard[MAX_COUNT];				//底牌扑克
	WORD  m_numCard;
	WORD  m_outUser;
	WORD  m_bankUser;
};

//--玩家弃牌
class DDZCCpassCard:public Integer
{
public:
	DDZCCpassCard(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCpassCard);
	WORD m_passUser;
};

//--游戏结束
class DDZCCgameOver:public Integer
{
public:
	DDZCCgameOver(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCgameOver);
	//CMD_S_GameConclude m_gameEndData;
	DDZCMD_S_GameEnd* m_gameEndData;
	//	玩家ID
	DWORD m_UserID;
	//	是否破产
	bool m_isPochang;
};

//--明牌倍数消息
class DDZCCviewOpenCardNum:public Integer
{
public:
	DDZCCviewOpenCardNum(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCviewOpenCardNum);
	WORD  m_num;
};

//--加倍表现
class DDZCCaddView:public Integer
{
public:
	DDZCCaddView(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCaddView);
	WORD			m_addID;
	WORD			m_AddType;		//--加倍类型，1 表现， 2影藏，3.请求
};

//--底牌变化
class DDZCCbackData:public Integer
{
public:
	DDZCCbackData(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCbackData);
	BYTE   m_data[DDZGAME_PLAYER];
	WORD	m_taskKey;
	bool    m_isNew;
};

//--倍率变化
class DDZCCbeilv:public Integer
{
public:
	DDZCCbeilv(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCbeilv);
	WORD		m_beilv;
	WORD		m_base;
};

//--在线提醒
class DDZCConline: public Integer
{
public:
	DDZCConline(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCConline);
	WORD    m_userID;
	bool        m_isOnline;
};

//--表情
class DDZCCfaceAndVioce: public Integer
{
public:
	DDZCCfaceAndVioce(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCfaceAndVioce);
	int m_Type;//（1表情 2语音）
	int m_KeyID;
	int m_UserID;
	char voice[512];
};

//--地主表现
class DDZCClandView:public Integer
{
public:
	DDZCClandView(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCClandView);
	int m_landUserID;
};

//结算玩家信息
class DDZCCSettmentUserMessage:public Integer
{
public:
	DDZCCSettmentUserMessage(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCSettmentUserMessage);
	int m_userId;//用户id
	int m_diZhuId;//地主id
	int m_updScore;//变化的分数
	int m_isJiabei;//是否加倍
	char m_nikeName[32];//昵称
};

//--画费点变化
class DDZCChuafeidian:public Integer
{
public:
	DDZCChuafeidian(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCChuafeidian);
	int  m_huafeidian;
};

//--房间限制
class DDZCCRoomLimit:public Integer
{
public:
	DDZCCRoomLimit(int v):Integer(v){};
	CREATE_FUNC_MSG(DDZCCRoomLimit);
	int  m_RoomLimitKey;			//--1，太底了，2太高了
};
#endif



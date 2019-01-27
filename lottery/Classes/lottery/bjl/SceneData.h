#ifndef		 __CC_SCENEDATA_WUCAN__
#define		__CC_SCENEDATA_WUCAN__
#include "cocos2d.h"
USING_NS_CC;
#include "CMD_BJL.h"

#define CREATE_FUNC_MSG(__TYPE__) \
static __TYPE__* create(int v) \
{ \
	__TYPE__ *pRet = new __TYPE__(v); \
	pRet->autorelease(); \
	return pRet; \
}

enum Game_State
{
	Game_null=1,				//没开始
	Game_Start,					//明牌开始
	Game_landLord,			//抢地主
	Game_Run,					//打牌
	Game_Over,				//游戏结束
};

enum
{
	msgToView_UserInfor=1,				//--跟新任务信息
	msgToView_start,					//--游戏开始
	msgToView_startView,				//--开始效果表现
	msgToView_addView,					//--加倍表现
	msgToView_sendCard,					//--发牌
	msgToView_backCard,					//--底牌
	msgToView_openCard,					//--明牌
	msgToView_landLord,					//--抢地主
	msgToView_beiLv,							//--倍率变化
	msgToview_LordView,					//--地主表现变化
	msgToView_time,						
	msgToView_outCard,
	msgToView_passCard,
	msgToView_gameOver,							//--游戏结束
	msgToView_SettmentUser,					//--游戏结算人物消息
	msgToView_openCardNum,					//--viewHandCard控制
	msgToview_TimeCallBack,						//--时间回调  Time 控制
	msgToView_OnLine,								//--玩家在线提示
	msgToView_Reconnect,						//--断线重连
	msgToView_socketoffConnect,				//--网络断开自动重连
	msgToView_socketoff,							//--网络断开，返回大厅
	msgToView_VoiceAndFace,					//--表情和语音
	msgToView_huafeidian,							//--花费点
	msgToView_Close,								//--强制关闭
	msgToView_RoomLimit,						//--房间限制

	msgToLogic_openStart,
	magToLogc_gameStart,
	msgToLogc_openCard,
	msgToLogc_LandLord,
	msgToLogic_outCard,
	msgToLogic_passCard,
	msgToLogic_MoreGame,
	magToLogc_jiabei,							//--加倍
	msgToLogic_face,								//--表情
	msgToLogic_changeTable
};

//牌桌菜单枚举
enum
{
	menu_visibleStartMenuItem = 1,	//明牌开始
	menu_startMenuItem,				//普通开始
//	menu_menuBarItem,				//菜单栏
	menu_rechargemenuItem,			//快速充值
	menu_outCardMenuItem,			//出牌
	menu_passOutCardMenuItem,		//不要
	menu_hintMenuItem,				//提示
	menu_jiaoDiZhuItem,				//叫地主
	menu_buJiaoItem,				//不叫
	menu_qiangDiZhuItem,			//抢地主
	menu_passQiangDiZhuItem,		//不抢
	menu_visibleCardItem,			//明牌X2
	menu_doubleItem,				//加倍
	menu_unDoubleItem,				//不加倍
	menu_settlementDetial,			//结算详情
	menu_again,						//再来一局
	menu_menuBarTalkItem,			//菜单栏聊天按钮
	menu_menuBarRobotItem,			//菜单栏托管按钮
	menu_menuBarOptionsItem,		//菜单栏设置按钮
	menu_menuBarTaskItem,			//菜单栏任务按钮
	menu_menuBarBackItem,			//菜单栏返回按钮
	menu_MenuBarPullItem,			//菜单栏拉环按钮
	menu_visibleCardStartItem,		//发牌过程中的明牌开始
	menu_number,					//明牌开始按钮数字
	menu_OutAgency,			//取消托管
	menu_changeTable,	//明牌开始
};

//--人物信息更新
class CCuserInfor:public Integer
{
public:
	CCuserInfor(int v):Integer(v){};
	CREATE_FUNC_MSG(CCuserInfor);
	DWORD		m_userInforID;		//用户id<by hxh 20160628>
	LONG		m_score;			//分数
	WORD		m_gender;			//性别
	LONG		m_GoldEggs;			//话费点
	char			m_PosName[32];		//城市
	char          m_name[32];
};

//--明牌开始消息
class CCopenStart:public Integer
{
public:
	CCopenStart(int v):Integer(v){};
	CREATE_FUNC_MSG(CCopenStart);
	WORD				m_bCurrentUser;                     //开始用户 
	bool					m_bOpenCard;							//明牌开始
};

//--发牌消息
class CCsendCard: public Integer
{
public:
	CCsendCard(int v):Integer(v){};
	CREATE_FUNC_MSG(CCsendCard);
	BYTE			m_handCardData[MAX_COUNT];
	WORD		m_handCardNum;
	bool			m_bOpen;
	bool			m_bEffect;
	WORD		m_userID;
};

//--明牌消息
class CCopenCard: public Integer
{
public:
	CCopenCard(int v):Integer(v){};
	CREATE_FUNC_MSG(CCopenCard);
	WORD m_openID;
};

//--抢地主
class CClandLord: public Integer
{
public:
	CClandLord(int v):Integer(v){};
	CREATE_FUNC_MSG(CClandLord);
	WORD		m_landID;
	WORD		m_LandKey;
	bool			m_isLand;
};

//--显示时钟
class CCtimeControl:public Integer
{
public:
	CCtimeControl(int v):Integer(v){};
	CREATE_FUNC_MSG(CCtimeControl);
	WORD		m_timeUserID;
	WORD		m_timeNum;
	Game_State	m_gameState;			//--Game_landLord <2 为叫地主 >2为抢地主
	WORD		m_timeKey;					//--Game_Run 0为地主首次出牌并且能明牌，1为地主首次出牌不能明牌，2为自己首次出牌，3为普通出牌			
	WORD		m_BankUser;
};

//--玩家出牌
class CCoutCard:public Integer
{
public:
	CCoutCard(int v):Integer(v){};
	CREATE_FUNC_MSG(CCoutCard);
	BYTE		m_outkCard[MAX_COUNT];				//底牌扑克
	WORD  m_numCard;
	WORD  m_outUser;
	WORD  m_bankUser;
};

//--玩家弃牌
class CCpassCard:public Integer
{
public:
	CCpassCard(int v):Integer(v){};
	CREATE_FUNC_MSG(CCpassCard);
	WORD m_passUser;
};

//--游戏结束
class CCgameOver:public Integer
{
public:
	CCgameOver(int v):Integer(v){};
	CREATE_FUNC_MSG(CCgameOver);
	//CMD_S_GameConclude m_gameEndData;
	CMD_S_GameEnd m_gameEndData;
	WORD					m_UserID;
	bool						m_isPochang;
};

//--明牌倍数消息
class CCviewOpenCardNum:public Integer
{
public:
	CCviewOpenCardNum(int v):Integer(v){};
	CREATE_FUNC_MSG(CCviewOpenCardNum);
	WORD  m_num;
};

//--加倍表现
class CCaddView:public Integer
{
public:
	CCaddView(int v):Integer(v){};
	CREATE_FUNC_MSG(CCaddView);
	WORD			m_addID;
	WORD			m_AddType;		//--加倍类型，1 表现， 2影藏，3.请求
};

//--底牌变化
class CCbackData:public Integer
{
public:
	CCbackData(int v):Integer(v){};
	CREATE_FUNC_MSG(CCbackData);
	BYTE   m_data[GAME_PLAYER];
	WORD	m_taskKey;
	bool    m_isNew;
};

//--倍率变化
class CCbeilv:public Integer
{
public:
	CCbeilv(int v):Integer(v){};
	CREATE_FUNC_MSG(CCbeilv);
	WORD		m_beilv;
	WORD		m_base;
};

//--在线提醒
class CConline: public Integer
{
public:
	CConline(int v):Integer(v){};
	CREATE_FUNC_MSG(CConline);
	WORD    m_userID;
	bool        m_isOnline;
};

//--表情
class CCfaceAndVioce: public Integer
{
public:
	CCfaceAndVioce(int v):Integer(v){};
	CREATE_FUNC_MSG(CCfaceAndVioce);
	int m_Type;//（1表情 2语音）
	int m_KeyID;
	int m_UserID;
	char voice[512];
};

//--地主表现
class CClandView:public Integer
{
public:
	CClandView(int v):Integer(v){};
	CREATE_FUNC_MSG(CClandView);
	int m_landUserID;
};

//结算玩家信息
class CCSettmentUserMessage:public Integer
{
public:
	CCSettmentUserMessage(int v):Integer(v){};
	CREATE_FUNC_MSG(CCSettmentUserMessage);
	int m_userId;//用户id
	int m_diZhuId;//地主id
	int m_updScore;//变化的分数
	int m_isJiabei;//是否加倍
	char m_nikeName[32];//昵称
};

//--画费点变化
class CChuafeidian:public Integer
{
public:
	CChuafeidian(int v):Integer(v){};
	CREATE_FUNC_MSG(CChuafeidian);
	int  m_huafeidian;
};

//--房间限制
class CCRoomLimit:public Integer
{
public:
	CCRoomLimit(int v):Integer(v){};
	CREATE_FUNC_MSG(CCRoomLimit);
	int  m_RoomLimitKey;			//--1，太底了，2太高了
};
#endif



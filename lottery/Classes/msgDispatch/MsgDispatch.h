
#ifndef __SDH__gameFrame__
#define __SDH__gameFrame__

#include <iostream>
#include "GlobalDef.h"
#include "CMD_Plaza.h"
#include "TcpNetwork.h"
#include "GBEvent.h"
#include "BaseObject.h"
#include "GlobalUnits.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma comment(lib, "WS2_32.lib")
#endif

//win下wchar_t是2字节，但ios和android是4字节，所以得提升空间	 LOCAL_CPP_FLAGS := -fshort-wchar

using namespace cocos2d;
// 数据分类
typedef enum
{
	CMD_Default     = 0,		// 默认
	CMD_Logon		= 1,		// 登录
	CMD_Room		= 2,		// 房间
	CMD_Game		= 3,		// 游戏
	CMD_End         = 10,		// 结束

}DataType;

class MsgDispatch : public BaseObject,Node
{
public:

	MsgDispatch();

	~MsgDispatch();

	bool init();

	//开始消息循环
	void startSchedule();
	//暂停消息循环
	void pauseSchedule();
	//继续消息循环
	void resumeSchedule();

	//登陆消息读取
	bool onEventTcpLoginRead(WORD wSocketId,CMD_Command* pCommand,void* pData,WORD wDataSize);

	//房间信息读取			add by wangjun 2014-12-15
	bool  OnEventTcpRoomRead(WORD wSocketId,CMD_Command* pCommand,void* pData,WORD wDataSize);

	//游戏消息读取
	bool  OnEventTcpGameRead(WORD wSocketId,CMD_Command* pCommand,void* pData,WORD wDataSize);

	//void onDispatch(float dt);

	virtual void update(float dt);

	//重置校验错误
	void resetConTimes();
	//是不是在登陆界面
	void setIsLogin(bool isLogin);

	void onEventSocketRecv(GBEventConstArg& arg);

	void dispEntityMsg(CMD_Command* pCommand, void * pBuffer, WORD wDataSize,GBEVENT_ID eid);
	/******************************登陆服务器消息*******************************************/
	//登录消息
	bool			onSocketMainLogin(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//列表消息
	bool			onSocketMainServerList(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//系统消息
	bool			onSocketMainSystem(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户消息
	bool			onSocketMainUser(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);


	/******************************游戏服务器消息*******************************************/
	//µ«¬ºœ˚œ¢
	bool			OnSocketGameLogin(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户消息
	bool			OnSocketGameUser(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//配置信息
	bool			OnSocketGameInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//状态信息
	bool			OnSocketGameStatus(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//系统消息
	bool            OnSocketGameSystem(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//房间信息
	bool            OnSocketGameServerInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);

	/******************************游戏服务器用户消息*******************************************/

	//设置回调接口（用来区分消息类型）
	void setTcpSink(BaseObject* obj);

	//用户登陆
	void setUserLogin(string account,string password,bool bAnonymous = false);

	//用户注册
	void setUserRegister(string account,string password,int nfaceId,int nGender, int visiter);

	//匿名登陆
	void setAnonymous();

	//发送登陆包
	bool sendLoginPacket();

	//第三方支付
	void sendOrderInfo(const char* PayName,int chargeNum);

	//修改昵称
	void modifyNickName(const char* nickName);

	//修改头像
	void modifyTouxiang(int nHand,int nSex);

	//紫金岛卡
	void isLnadCardRecarge(const char* cardNum,const char* cardPass);

	//电话卡
	void phoneCardRechange(const char* cardType,const char* cardFree,const char* cardNum,const char* cardPass);

	//获得登陆奖励
	void getLoginAward();

	//得到版本
	void getVersion();

	//修改密码
	void modifyPassWord(string oldPassword,string newPassword, bool isLoginPass);

	//领取每日奖励
	void getDarlyAward(int taskID);

	//获得下载奖励（情况）
	void getDownLoadAward(int gameID,int subcmd);

	//完善注册
	void overRegist(string newAccount,string newPassword);

	// 领取救济金
	void getRelieve(int nUserID);

	//发送赠送包
	void sendScoreGiftPacket(const char* szAccount,long lValue);

	//发送金币话费领取包
	void sendChargePacket(int ntype,const char* des);

	//找回密码
	void findPassword(const char* szAccount);

	//发送数据
	void sendSocketDate(WORD wMainCmdID,WORD wSubCmdID);

	//发送数据
	void sendSocketDate(WORD wMainCmdID,WORD wSubCmdID, void * pData, WORD wDataSize);

	void SendPacketWithCompilatio(int tag);
	//连接登陆服务器(是否直接登陆)
	bool connectLoginServer();

	//被卡游戏房间  add by hxh 2016-11-23
	bool OnSocketSubListLastGameServer(void *buffer, WORD size);

	//连接游戏服务器<by hxh 20161213>
	bool connectGameServer(int nGameKindId,int nServerId=0);
	//连接游戏服务器
	bool connectGameServerAdr(DWORD dwAdrIP, WORD wPort);

	//发送数据
	void sendSocketDate(WORD wSubCmdID, void * pData, WORD wDataSize);
	//发送表情、文字      add by wangjun 2014-12-10
	void sendFrameDate(WORD wSubCmdID, void * pData, WORD wDataSize);

	//发送数据
	void sendSocketDate(WORD wSubCmdID);

	//设置登陆状态
	void setLoginState(enLoginState state);

	//发送排队请求
	void sendQueuePacket(int cbQueueCell = 0);

	//获取服务器配置信息
	void sendGetConfigPacket(int nVersion);

	//获取登陆状态
	enLoginState getLoginState() { return m_loginState; }

	//关闭登陆连接
	void closeLoginSocket();

	//关闭游戏服务连接
	void closeGameSocket();

	//获取全服总人数
	DWORD getServerOnLineCount();

	// add by hxh 20161213
	GameServerList getGameServerList(unsigned int nKind);

	//获取mac地址
	std::string& getMacaddress();

	//设置匿名登陆
	bool getAnonymous() { return m_bAnonymous; }
	void setAnonymous(bool bAnonymous) { m_bAnonymous = m_bAnonymous; }

	//设置注册登陆
	bool isRegLogin() 
	{
		return m_bRegLogin; 
	}
	void setRegLogin(bool bReg) 
	{ 
		m_bRegLogin = bReg; 
	}

	int getLevelbyScore(LONG lScore);
	int getBaseCell(int type,int nLevel);   //获取倍率

	void setGameStatus(bool bStatus);
	bool isGameStatuc() { return  m_bGameStatus; }
	bool isGameLinkOK(){return m_pClientGameSock->isServerConected();};
	void setGameKindId(int gameId);
	void setServerId(int serverId);

	//-----------主命令码分类-------------------
	//-------------大厅登录--------------
	//登录消息      add by wangjun 2014-12-10
//	bool OnTCPNetworkMainMBLogon(int wSubCmdID, void *buffer, WORD size);		  				
	//列表消息		add by wangjun 2014-12-10
	bool OnTCPNetworkMainMBServerList(int wSubCmdID, void *buffer, WORD size);						
	//服务消息		add by wangjun 2014-12-10
	bool OnTCPNetworkMainMBIphoneUserService(int wSubCmdID, void *buffer, WORD size);	

	//---------------------大厅登录子命令码处理--------------------------
	//登录成功      add by wangjun 2014-12-10
//	bool OnSocketSubLogonSuccess(void *buffer, WORD size);	
	bool OnSocketSubGPLogonSuccess(void *buffer, WORD size);	

	//登录失败      add by wangjun 2014-12-10
//	bool OnSocketSubLogonFailure(void *buffer, WORD size);	
	bool OnSocketSubGPLogonFailure(void *buffer, WORD size);				

	//---------------------列表处理子命令码处理--------------------------
	//列表获取      add by wangjun 2014-12-10
	bool OnSocketSubListKind(void *buffer, WORD size);					
	//房间列表      add by wangjun 2014-12-10
	bool OnSocketSubListServer(void *buffer, WORD size);				
	//列表完成      add by wangjun 2014-12-10
	bool OnSocketSubListFinish(void *buffer, WORD size);	

	//登陆房间      add by wangjun 2014-12-10
	bool connectGameServer();
	void SendPacketWithEnterRoomPageCount(WORD count);

	//设置和获取StartType      add by wangjun 2014-12-10
	bool getStartType(){return m_bStartType;};
	void setStartType(bool type){m_bStartType = type;};

	//设置BehaviorFlag      add by wangjun 2014-12-10
	WORD getBehaviorFlag(){return m_wBehaviorFlags;};
	void setBehaviorFlag(WORD flag){m_wBehaviorFlags = flag;};

	//发送游戏规则      add by wangjun 2014-12-10
	void SendPacketWithGameOption();

	//获取用户 或申请用户信息
	void SendPacketWithRequestUserInfo(DWORD dwUserID, WORD wTableID);
	void SendPacketWithRequestChairUserInfo(WORD wTableID, WORD wChairID);
	//请求发送自己桌子的用户信息
	void SendPacketWithRequestMyTableUserInfo(int playerNum);

	//发送换桌消息      add by wangjun 2014-12-22
	void SendPacketWithChangeTable();
	//Sit Down    add by wangjun 2014-12-22
	void SendPacketWithSitDown(WORD wTableID = INVALID_TABLE, WORD wChairID = INVALID_CHAIR);

	//发送用户状态      add by wangjun 2014-12-22
	void SendPacketWithUserStatus(BYTE status);

	//查询保险柜信息      add by wangjun 2014-12-22
	void SendPacketWithQueryBankInfo();

	//保险柜命令      add by wangjun 2014-12-22
	bool OnTCPNetworkBankInsure(WORD wSubCmdID, void* pBuffer, WORD wDataSize);			
	//保险柜信息      add by wangjun 2014-12-10
	bool OnSocketWithInsureInfo(void *buffer, WORD size);			
	//查询保险柜成功      add by wangjun 2014-12-10
	bool OnSocketWithBankSucceed(void *buffer, WORD size);			
	//查询保险柜失败      add by wangjun 2014-12-10
	bool OnSocketWithBankFailure(void *buffer, WORD size);	

	/*************±≥∞¸≤Ÿ◊˜*************/
	//π∫¬ÚŒÔ∆∑      add by wangjun 2015-05-19
	bool OnSocketWithSearchGoods(void *buffer, WORD size);
	//π∫¬ÚŒÔ∆∑      add by wangjun 2015-05-19
	bool OnSocketWithBuyGoods(void *buffer, WORD size);
	//‘˘ÀÕŒÔ∆∑      add by wangjun 2015-05-19
	bool OnSocketWithGiveGoods(void *buffer, WORD size);
	//      add by wangjun 2015-05-19
	bool OnSocketWithGetUserPacket(void *buffer, WORD size);
	//猜硬币返回      add by wangjun 2015-05-19
	bool OnSocketWithGuessCoinRet(void *buffer, WORD size);
	//大转盘返回      add by wangjun 2015-05-19
	bool OnSocketWithTurnTableRet(void *buffer, WORD size);
	//返点查询返回
	bool OnSocketWithQueryMyFanDianRet(void *buffer, WORD size);
	//韩国1,5分彩的偏移量
	bool OnSocketWithQueryHGYDWFDelta(void *buffer, WORD size);
	//加拿大3.5分彩的期号和时间
	bool OnSocketWithQueryCanadaSDWFQiHao(void *buffer, WORD size);
	//跨年期号
	bool OnSocketWithQueryKuaNianQiHao(void *buffer, WORD size);
	//签到
	bool OnSocketWithSignUp(void *buffer, WORD size);
	//欢乐颂
	bool OnSocketWithQueryHuanLeSong(void *buffer, WORD size);
	//游戏查询返回      add by wangjun 2015-05-19
	bool OnSocketWithQueryGameRet(void *buffer, WORD size);
	//获取更多开奖结果返回      add by wangjun 2015-05-19
	bool OnSocketWithQueryMoreGameRet(void *buffer, WORD size);
	//CQSSC 下注返回      add by wangjun 2015-05-19
	bool OnSocketWithCQSSCRET(void *buffer, WORD size);
	// 下注返回      add by wangjun 2015-05-19
	bool OnSocketWithTouZhuContinueRet(void *buffer, WORD size);
	//获取投注记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetTouZhuLogCountRet(void *buffer, WORD size);
	//获取投注信息返回      add by wangjun 2015-05-19
	bool OnSocketWithGetTouZhuLogRet(void *buffer, WORD size);
	//查询会员信息日志数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetHYXXLogCountRet(void *buffer, WORD size);
	//查询会员信息返回      add by wangjun 2015-05-19
	bool OnSocketWithGetHYXXLogRet(void *buffer, WORD size);
	//获取下级投注统计记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJTZHLogCountRet(void *buffer, WORD size);
	//获取下级投注统计信息返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJTZHLog(void *buffer, WORD size);
	//查看下级投注记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithCHKXJTZHLogCountRet(void *buffer, WORD size);
	//查看下级投注返回      add by wangjun 2015-05-19
	bool OnSocketWithCHKXJTZHLog(void *buffer, WORD size);
	//查看下级游戏盈亏记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJYXTjCountRet(void *buffer, WORD size);
	//查看下级游戏盈亏返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJYXTj(void *buffer, WORD size);
	//查询下级盈亏信息日志数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJYKLogCountRet(void *buffer, WORD size);
	//查询下级盈亏返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJYKLogRet(void *buffer, WORD size);
	//查询下级盈亏统计日志数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJYKTjCountRet(void *buffer, WORD size);
	//查询上下级关系
	bool OnSocketWithGetParentRelativeRet(void *buffer, WORD size);
	//游戏列表
	bool OnSocketWithGetGameKindRet(void *buffer, WORD size);
	//查询下级盈亏统计返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJYKTjRet(void *buffer, WORD size);
	//查询下级充值日志数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXjChzhLogCountRet(void *buffer, WORD size);
	//查询下级充值返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJCHZHLogRet(void *buffer, WORD size);
	//查询下级提现日志数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXjTxLogCountRet(void *buffer, WORD size);
	//查询下级提现返回      add by wangjun 2015-05-19
	bool OnSocketWithGetXJTxLogRet(void *buffer, WORD size);
	//获取提现记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetTiXianLogCountRet(void *buffer, WORD size);
	//获取提现信息返回      add by wangjun 2015-05-19
	bool OnSocketWithGetTiXianLogRet(void *buffer, WORD size);
	//获取充值记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetChongzhiLogCountRet(void *buffer, WORD size);
	//获取充值信息返回      add by wangjun 2015-05-19
	bool OnSocketWithGetChongzhiLogRet(void *buffer, WORD size);
	//获取盈亏记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetYingkuiLogCountRet(void *buffer, WORD size);
	//获取盈亏信息返回      add by wangjun 2015-05-19
	bool OnSocketWithGetYingkuiLogRet(void *buffer, WORD size);
	//获取棋牌盈亏记录数返回      add by wangjun 2015-05-19
	bool OnSocketWithGetQiPaiYingkuiLogCountRet(void *buffer, WORD size);
	//获取棋牌盈亏信息返回      add by wangjun 2015-05-19
	bool OnSocketWithGetQiPaiYingkuiLogRet(void *buffer, WORD size);
	//获取投注返回      add by wangjun 2015-05-19
	bool OnSocketWithGetTouZhuXXRet(void *buffer, WORD size);
	//获取客服地址
	bool OnSocketWithGetKeFuURLRet(void *buffer, WORD size);
	//验证完验证码返回
	bool OnSocketWithGetValidateCodeRet(void *buffer, WORD size);
	//转帐是否需要验证码
	bool OnSocketWithGetTransPhoneRet(void *buffer, WORD size);
	//绑定手机返回
	bool OnSocketWithBindPhoneRet(void *buffer,WORD size);
	//解除绑定手机返回
	bool OnSocketWithUnBindPhoneRet(void *buffer,WORD size);
	//获取验证码应用分类返回
	bool OnSocketWithGetValidateTypeRet(void *buffer,WORD size);
	//获取系统时间返回      add by wangjun 2015-05-19
	bool OnSocketWithGetSystemTimeRet(void *buffer, WORD size);

	//撤单返回      add by wangjun 2015-08-19
	bool OnSocketWithCancelTouZhuRet(void *buffer, WORD size);

	//充值信息查询返回      add by wangjun 2015-08-19
	bool OnSocketWithGetChongzhiTypeRet(void *buffer, WORD size);
	//提款信息查询返回      add by wangjun 2015-08-19
	bool OnSocketWithGetQukuanInfoRet(void *buffer, WORD size);
	//提款返回      add by wangjun 2015-08-19
	bool OnSocketWithDoQukuanRet(void *buffer, WORD size);
	//取款限制
	bool onSocketWithQuKuanLimitRet(void *buffer, WORD size);
	//绑卡返回      add by wangjun 2015-08-19
	bool OnSocketWithSetQukuanZhanghaoRet(void *buffer, WORD size);
	//绑卡查询返回      add by wangjun 2015-08-19
	bool OnSocketWithQueryMyYinHangRet(void *buffer, WORD size);
	//查询所有银行信息
	bool OnSocketWithQueryYinHangRet(void *buffer, WORD size);
	//添加下级返回      add by wangjun 2015-08-19
	bool OnSocketWithAddHuiYuanRet(void *buffer, WORD size);
	//配额返回
	bool OnSocketWithPeiERet(void *buffer, WORD size);
	//被强制下线
	bool OnSocketWithForceOut(void *buffer, WORD size);
	//设置网页返点
	bool OnSocketWithWebFanDianRet(void *buffer, WORD size);
	//查询网页返点
	bool OnSocketWithQueryWebFanDianRet(void *buffer, WORD size);
	//查询会员数据返回      add by wangjun 2015-08-19
	bool OnSocketWithGetHyShjRet(void *buffer, WORD size);
	//设置返点返回
	bool OnSocketWithSetusedBonusRet(void *buffer, WORD size);
	//转换返回
	bool OnSocketWithZhuanHuanRet(void *buffer, WORD size);
	//获取个人基本信息返回
	bool OnSocketWithGetUserInfoRet(void *buffer, WORD size);
	//获取最新余额信息返回
	bool OnSocketWithGetLastYueRet(void *buffer, WORD size);
	//六合彩的封单时间和期号
	bool OnSocketWithGetLiuHeFenDan(void *buffer,WORD size);
	//排行榜
	bool OnSocketWithGetRankListRet(void *buffer,WORD size);
	//代理领奖
	bool OnSocketWithDaiLiQianDaoRet(void *buffer,WORD size);
	//查询充值送
	bool OnSocketWithChongZhiSongGetRet(void *buffer, WORD size);
	//查询充值送领奖
	bool OnSocketWithChongZhiSongSetRet(void *buffer, WORD size);
	//代理领奖查询
	bool OnSocketWithQuerryDailiQianDaoRet(void *buffer,WORD size);
	//下级转帐
	bool OnSocketWithZhuanZhangRet(void *buffer, WORD size);
	//下级配额
	bool OnSocketWithXiajiPeiERet(void *buffer, WORD size);
	//设置下级配额返回
	bool OnSocketWithSetXiaJiPeiERet(void *buffer, WORD size);
	//设置下级返点
	bool OnSocketWithSetXiaJiFanDian(void *buffer, WORD size);
	//退出
	bool OnSocketWithExit(void *buffer, WORD size);
	//新加彩种
	bool OnSocketWithCaiZhongList(void *buffer,WORD size);

	//∑¢ÀÕ¥ÊøÓœ˚œ¢      add by wangjun 2014-12-22
	void SendPacketWithDeposit(SCORE score);

	//发送取款消息      add by wangjun 2014-12-22
	void SendPacketWithWithdraw(SCORE score, string password);

	//发送赠送消息      add by wangjun 2014-12-22
	void SendPacketWithSendScore(string nickname, SCORE score, string password, BYTE byNickName);

	//give goods     add by wangjun 2015-05-21
	void SendPacketWithGiveGoods(WORD wPropertyIndex, DWORD dwTargetUserID);
	//π∫¬ÚŒÔ∆∑      add by wangjun 2015-05-19
	void SendPacketWithBuyGoods(DWORD dwBuyGoodsID, DWORD dwBuyCount = 1);

	//≤È—ØŒÔ∆∑      add by wangjun 2015-05-19
	void SendPacketWithGetUserPacket();

	//猜硬币     add by wangjun 2015-05-19
	void SendPacketWithGuessCoin(bool bCoinSide, SCORE	 lScore);

	//大转盘     add by wangjun 2015-05-19
	void SendPacketWithTurnTable();
	/*********用户请求**********/
	//请求起立
	void SendPacketWithPerformStandup(WORD wTableID = INVALID_TABLE, WORD wChairID = INVALID_CHAIR, BYTE cbForceLeave = 1);

	void SendPacketWithInviteReq(DWORD dwUserID);

	void SendHappyDouWithShare();

	/************************************************
	***********************lottery*************************
	************************************************/
	void SendPacketWithTouZhuCQSSC_FanDian( int gameType, int gameKind);
	//获取投注信息
	void SendPacketQueryGameResult(int kindID);
	//获取更多开奖结果
	void SendPacketQueryMoreGameResult(int kindID);
	//投注
	void SendPacketWithTouZhuCQSSC(string qiShu, int gameType, int gameKind, string betNumber, int zhuShu = 1,
		int beiTou = 1, int moshi = 0, int sign = 1, int zhuiHao = 0, int nIndex = 0);
	void SendPacketWithTouZhuCQSSC_Dan(string qiShu, int gameType, int gameKind, string betNumber, int zhuShu = 1,
		int beiTou = 1, int moshi = 0, int sign = 1, int isEnd = 1, int zhuiHao = 0, int nIndex = 0);
	void SendPacketWithTouZhuCQSSC_ZhioHao(int zhuiHaoQiShu, char qiShu[100][20], int gameType, int gameKind, string betNumber, int zhuShu,
		int beiTou[100], int moshi = 0, int sign = 1, int isEnd = 0, int zhuiHao = 2, int nIndex = 0);
	void SendPacketWithGetTouZhuLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", const int caizhong = 0, const int status = -1);
	void SendPacketWithGetTouZhuLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", const int caizhong = 0,const int status = -1);
	//设置玩家返点
	void SendPacketWithGetTouZhuSetUserBonus(int Type_id,int Kind_id,DOUBLE	bonus,DOUBLE bonuspercent);
	//查询会员信息日志数
	void SendPacketWithGetHYXXLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询会员信息
	void SendPacketWithGetHYXXLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//获取下级投注统计数目
	void SendPacketWithGetXJTZHLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//获取下级投注统计
	void SendPacketWithGetXJTZHLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查看下级投注数目
	void SendPacketWithCHKXJTZHLogCount(int caizhong = 0,int status = -1, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查看下级投注
	void SendPacketWithCHKXJTZHLog(int caizhong = 0,int status = -1,int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询下级游戏盈亏信息日志数
	void SendPacketWithGetXJYXTjCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询下级游戏盈亏
	void SendPacketWithGetXJYXTj(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询下级盈亏明细信息日志数
	void SendPacketWithGetXJYKLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "",const int nType = 0);
	//查询下级盈亏明细
	void SendPacketWithGetXJYKLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "",const int nType = 0);
	//查询下级盈亏统计信息日志数
	void SendPacketWithGetXJYKTjCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询父子关系
	void SendPacketWithGetParentRelative(const int xjUserId);	
	//获得游戏列表
	void SendPacketWithGetGameKind(const int typeId);
	//查询下级盈亏统计
	void SendPacketWithGetXJYKTj(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "",const int sortType = 0);
	//获得平台公告
	void SendPacketWithGetNews();
	//获得活动
	void SendPacketWithGetActive();
	//获得公告返回
	bool OnSocketWithGetNews(void *buffer, WORD size);
	//获取活动返回
	bool OnSocketWithGetActive(void *buffer, WORD size);
	//站内信
	void SendPacketWithGetInboxMessage(const int page,const int size,const int type);
	//站内信返回
	bool OnSocketWithGetInboxMessage(void *buffer, WORD size);
	//查看站内信
	void SendPacketWithChkInboxMessage(const int id);
	//查看站内信返回
	bool OnSocketWithChkInboxMessage(void *buffer, WORD size);
	//获得所有下级用户信息
	void SendPacketWithGetXJUserInfo(const char* name = "",const int type = 0,const int size = 1,const int type_id = 1);
	//删除消息
	void SendPacketWithDeleteMessageAt(const int type_id,const int id);
	//获得所有下级用户信息返回
	bool OnSocketWithGetXJUserInfo(void *buffer, WORD size);
	//删除站内信返回
	bool OnSocketWithDeleteMessageRet(void *buffer, WORD size);
	//发送站内信
	void SendPacketWithSendMessage(const int revid,const int sign,const int type,const char* content,const char* title);
	//发送站内信返回
	bool OnSocketWithSendMessage(void *buffer, WORD size);
	//获得站内信数量
	void SendPacketWithGetMessageCount();
	//站内信返回数量
	bool OnSocketWithGetMessageCount(void *buffer, WORD size);
	//查询下级充值日志数
	void SendPacketWithGetXjChzhLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询下级充值
	void SendPacketWithGetXJCHZHLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询下级提现日志数
	void SendPacketWithGetXjTxLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "", int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询下级提现
	void SendPacketWithGetXJTxLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",int n_t_type = 0, int n_t_user_id = 0, const char *s_t_account = "");
	//查询下级游戏记录日志数
	void SendPacketWithGetQiPaiYingkuiCount(DWORD userID, int nTypeID = 1, int nByTime = false,const char* username = "", const char *szTimeStart = "", const char *szTimeEnd = "",const int gameid = 0);
	//查询下级游戏记录
	void SendPacketWithGetQiPaiYingkui(const char *accStr,DWORD userID, int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",const int gameKind = 0);
	void SendPacketWithGetTiXianLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "");
	void SendPacketWithGetTiXianLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "");
	void SendPacketWithGetChongzhiLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "");
	void SendPacketWithGetChongzhiLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "");
	void SendPacketWithGetYingkuiLogCount(bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "");
	void SendPacketWithGetYingkuiLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "");
	void SendPacketWithGetQiPaiYingkuiLogCount(int nTypeID = 1, int nByTime = 0,const char* username = "", const char *szTimeStart = "", const char *szTimeEnd = "",const int gameid = 0);
	void SendPacketWithGetQiPaiYingkuiLog(int nPage = 1, int nSize = 19, bool bTime = false, const char *szTimeStart = "", const char *szTimeEnd = "",const int gameKind = 0);
	void SendPacketWithGetTouZhuXX(int id);
	void SendPacketWithGetTouZhuXXXX(int id);
	void sendPacketWithGetKeFuURL();
	//获取验证码
	void SendPacketWithValidateCode();
	//绑定手机号码
	void SendPacketWithBindMobileNum(const char* mobileNumber);
	//解除绑定手机
	void SendPacketWithUnBindMobileNum();
	//发送验证码 itype= 0登陆 1绑定 2解绑 3转帐 4分红
	void SendPacketWithSendYanZhengMa(const char* validateCode, const int iType = 0);
	//验证对比验证码 itype= 0登陆 1绑定 2解绑 3转帐 4分红
	void SendPacketWithValidateYanZhengMa(const char* validateCode, const int iType = 0);
	//获取转帐是否需要验证码
	void SendPacketWithGetTransPhone(const int iType = 1);
	//设置验证码 应用地方
	void SendPacketWithSetValidateType(const int fenhong,const int login,const int transfer);
	//获取验证码 应用的地方
	void SendPacketWithGetValidateType();
	//取款
	void sendPacketWithGetQuKuanLimit();

	void SendPacketWithGetSystemTime();
	void SendPacketWithCancelTouZhu(int id, int t_kindid, string strQiHao);
	void SendPacketWithGetChongzhiType(BYTE	cbType);
	void SendPacketWithGetQuKuanInfo(BYTE	cbType);
	void SendPacketWithDoQukuan(DOUBLE fJine, string password);
	void SendPacketWithSetQukuanZhanghao(string szKaihuYinghang,string szKaihuRen,
		string szYinhangZhanghu,string szQukuanPass);
	void QueryMyYinHang();
	void QueryYinHang();
	void SendPacketWithAddHuiYuan(int n_type, DOUBLE f_t_fandian,
		string s_t_account,string s_t_login_pass,string s_t_qukuan_pass,string s_t_qq);
	//增加配额
	void SendPacketWithPeiE();
	//增加网页返点
	void SendPacketWithWebFanDian(DOUBLE f_fandian);
	//增加查询网页返点
	void SendPacketWithQueryWebFanDian();
	//会员数据
	void SendPacketWithGetHyShj(string start, string end, bool isOnlyToday);
	//棋牌彩票互转
	void SendPacketWithZhuanHuanMoney(int n_t_type, DOUBLE f_t_jine);
	//获取个人基本信息
	void SendPacketWithGetUserInfo();
	//发送退出消息
	void SendExitGame();
	//重新连接
	void SendReConnect(int typeId = 0);
	//获取最新余额信息
	void SendPacketWithGetLastYue();
	//六合彩封单时间和期号
	void SendPacketWithGetLiuHeFenDan();
	//获取公告
	void SendPacketWithGetRankList();
	//期号差
	void SendPacketWithGetHgydwfcDelta();
	//加拿大3.5分彩
	void SendPacketWithGetCanadaSDWFQiHao();
	//获取跨年期号
	void SendPacketWithGetKuaNianQiHao(int gameKind);
	//签到
	void SendPacketWithSignUp();
	//代理签到
	void SendPacketWithDailiSignUp();
	//充值送查询
	void SendPacketWithChongZhiSongGet();
	//充值送领取
	void SendPacketWithChongZhiSongSet();
	//代理签到查询信息
	void SendPacketWithDailiSignUpInfo();
	//下级转帐
	void SendPacketWithZhuanZhang(DOUBLE money,int targetID,int i_type,string password);
	//下级配额查询
	void SendPacketWithXiaJiPeiE();
	//设置下级配额
	void SendPacketSetXiaJiPeiE(const int user_id,const int peiE1,const int peiE2,const int peiE3,const int peiE4,const int peiE5);
	//设置下级返点
	void SendPacketSetXiaJiFanDian(const int user_id,const double fandian);
	//查询签到欢乐颂信息
	void SendPacketWithGetSignUpInfo();
	//退出程序
	void onExitProgram(Node* obj);
	
	//获取系统时间
	void GetTime(time_t &t);
	void TimeIncrease(){current_time_t++;};
public:
	LONG m_lUserLevel[7];             //用户等级
private:
	ClientSock*   m_pClientLoginSock;
	ClientSock*   m_pClientGameSock;
	bool         m_bAnonymous;      //是否匿名登陆
	bool         m_bRegLogin;       //是否注册登陆
private:
	int m_dwRecvPacketCount;
	int m_loseConTimes;  //4秒内连接的次数
	int m_isLogin;
	int netWorkType;

	WORD m_wRecvSize;
	BYTE m_cbRecvBuff[81920];
	int m_wTouZhuSign;
private:
	CServerListMgr m_ServerListMgr;
	typedef std::vector<CMD_GP_UserLevelResult> UserLevel;
	UserLevel m_UserLevelList;
	WORD userServiceSubCmd;
public:
	string m_szAccount;
	string m_szPassword; //密码保存
	enLoginState m_loginState;
	bool  m_bGameStatus;                                  //游戏标识（服务器多次发送切换场景消息 防止客户端多次切换）
	WORD m_wBehaviorFlags;
	DataType m_eDataType;
	bool m_bStartType;

	//用户信息			add by wangjun 2014-12-10
	string					m_pAccounts;					// 用户帐号
	string					m_pPassword;					// 用户密码
	string					m_pBankWord;					// 保险柜密码
	string					m_pNickname;					// 用户昵称 
	string					m_pPhoneNum;					// 手机号码


	int                         m_cbType;
	DOUBLE                      f_fandian;
	DOUBLE						f_yue;
	DOUBLE						f_qipai_yue;

	DWORD						m_dwUserID;						// 用户I D
	WORD						m_wLastServerID;				// 被卡游戏房间索引<为0表示没有被卡>
	WORD						m_wLastKindID;				    // 被卡游戏类型ID<为0表示没有被卡>
	DWORD						m_dwGameID;						// 游戏I D
	DWORD						m_dwKindID;						// 类型I D
	WORD						m_wFaceID;						// 用户头像
	BYTE						m_cbGender;						// 用户性别
	SCORE                       m_dwGoldScore;                  // 用户拥有的豆豆
	DWORD                       m_dwExperience;                 // 经验值

	TCHART						m_pPassWordMD5[LEN_MD5];		// md5密码
	TCHART						m_pMachineMD5[LEN_MD5];			// 机器标识

	BYTE						m_cbDeviceType;					// 设备类型

	WORD						m_wTableID;						// 桌子号
	WORD						m_wChairID;						// 椅子号
	BYTE						m_cbStatus;						// 用户状态

	string						m_pSystemMsg;
	WORD                        m_wSystemMsgType;               // 系统类型消息

	//保险柜传回来数据
	WORD										m_wRevenueTake;				//税收
	WORD										m_wRevenueTransfer;			//税收
	WORD										m_wServerID;					//房间标识
	SCORE									m_dwlUserScore;				//用户豆豆
	SCORE									m_dwlUserInsure;				//保险柜豆豆
	SCORE									m_dwlTransferPrerequisite;		//转账条件
	char										szDescribeString[256];			//描述信息

	int                         m_wArticleNum[20];
	time_t						current_time_t;
	int							m_nDeltaTime;
	int							m_bValidate;

	//new add by zengxin 20161208
	/*
	发送恢复对局消息次数的计数；
	作用：目前由于捕鱼和碰碰车，可能不会返回 登录成功消息，导致不会发送恢复对局，不能正常进入游戏；

	使用方法： 在每次调用SendPacketWithGameOption（）方法时加1，（及每启动一次游戏，只发送一次 该消息）
	在每次离开游戏时，清0， 在发送站立消息时调用，SendPacketWithPerformStandup（）；
	*/
	int							m_nSendGameOption;
};

#endif /* defined(__SDH__gameFrame__) */

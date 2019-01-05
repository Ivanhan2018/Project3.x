
#if !defined(_GAME_MESSAGE_DEFINE_H_)
#define _GAME_MESSAGE_DEFINE_H_

//房间
enum emGameRoom
{
	em_GR_LogonFinish,				//登录完成
	em_GR_LogonFailure,				//登录失败
	em_GR_LogonSuccess,				//登录成功
	em_GR_StartGame,				//启动游戏
	em_GR_EndGame,					//退出游戏
	em_GR_TCPShut,					//网络关闭
	em_GR_TCPLink,					//网络连接
	em_GR_SYS_MSG,					//系统消息
	em_GR_UpdateNotify,				//游戏升级
	em_GR_CreateTable,
	em_GR_UpdateTable,
	em_GR_SystemMSG,				//系统消息
	em_GR_LogonValidate,			//登录验证
	em_GR_ObjPartake,				//众筹
	em_GR_UserSaveScore,
	em_GR_ApplyOrderResult,
	em_GR_PayResult,
	em_GR_WaitDistribute,
	em_GRCreateRoomFinish,				//创建桌子完成
};
//二人捕鱼
enum emGameMessage
{
	em_GM_FishTrace,				//创建鱼群
	em_GM_ExchangeFishScore,		//
	em_GM_UserFire,					//用户开火
	em_GM_CatchFish,				//抓住鱼
	em_GM_BulletIONTimeOut,			//能量炮超时
	em_GM_LockTime,					//锁定超时
	em_GM_CatchSweepFish,			//
	em_GM_CatchSweepFishResult,		//
	em_GM_HitFishLK,				//
	em_GM_SwitchScene,				//
	em_GM_StockOperateResult,		//
	em_GM_SceneEnd,					//
	em_GM_FireScore,				//
	em_GM_HARVEST,
	em_GM_CONFIG,
	em_GM_HELPMONEY,
	em_GM_MESSAGE,
	em_GM_OnLineAward,				//游戏在线奖励
	em_GM_AwardScore				//游戏在线奖励
};
//百人牛牛
enum emNiuniuMsg
{

};
//百家乐
enum emBaccaratMsg
{
	em_BA_GameFree,
	em_BA_GameStart,
	em_BA_PlaceBet,
	em_BA_ApplyBanker,
	em_BA_CancelBanker,
	em_BA_ChangeBanker,
	em_BA_GameEnd,
	em_BA_GameRecord,
	em_BA_PlaceBetFail,
	em_BA_CommandResult,
	em_BA_CancelPlaceBet,
	em_BA_RandcardList,
	em_BA_StatusFree,
	em_BA_StatusPlay
};
#endif // !defined(_GAME_MESSAGE_DEFINE_H_)
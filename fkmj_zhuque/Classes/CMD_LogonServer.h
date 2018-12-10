#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#include "datastream.h"

//////////////////////////////////////////////////////////////////////////
#pragma pack(8)//zhuque,CMD_GP_LogonSuccess

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID			2//1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS		1//2									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_GP_LOGON_VISITOR		5									//游客登录
#define SUB_GP_LOGON_USER_GRADE_INFO    6                               //等级信息

//登录结果
#define SUB_GP_LOGON_SUCCESS    100										//登录成功
#define SUB_GP_LOGON_FAILURE    101										//登录失败
#define SUB_GP_LOGON_FINISH     102										//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败
#define SUB_GP_VALIDATE_PASSPORT  104									//登录失败  
#define SUB_GP_VERIFY_RESULT    105										//验证结果
#define SUB_GP_MATCH_SIGNUPINFO   106									//报名信息
#define SUB_GP_GROWLEVEL_CONFIG   107									//等级配置

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本
//

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_ONLINE			3									//获取在线
#define SUB_GP_GET_COLLECTION		4									//获取收藏

//在线更新
struct CMD_GP_UpdateNotify
{
	byte							cbMustUpdate;						//强行升级
	byte							cbAdviceUpdate;						//建议升级
	dword							dwCurrentVersion;					//当前版本
	char							szDownLoadHttp[128];				//下载地址
};
//获取在线
struct CMD_GP_GetOnline
{
	word							wServerCount;						//房间数目
	word							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	word							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	word							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};


//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
//#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			103//104									//房间列表
#define SUB_GP_LIST_CONFIG			105								    //列表配置
//#define SUB_GP_LIST_MATCH			105									//比赛列表
#define SUB_GP_VIDEO_OPTION			106									//视频配置
	
//完成信息
#define SUB_GP_LIST_FINISH			104//200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线
#define SUB_GR_ONLINE_FINISH		302									//在线完成


//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE			3									//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE		100									//修改机器
#define SUB_GP_MODIFY_LOGON_PASS	101									//修改密码
#define SUB_GP_MODIFY_INSURE_PASS	102									//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE	103									//修改签名
#define SUB_GP_MODIFY_ACCOUNTS		104									//修改帐号
#define SUB_GP_MODIFY_SPREADER		105									//修改推荐人

//修改密码
struct CMD_GP_ModifyLogonPass
{
	dword							dwUserID;							//用户 I D
	char							szDesPassword[LEN_PASSWORD];		//用户密码
	char							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	dword							dwUserID;							//用户 I D
	char							szDesPassword[LEN_PASSWORD];		//用户密码
	char							szScrPassword[LEN_PASSWORD];		//用户密码
};


//修改推荐人
struct CMD_GP_ModifySpreader
{
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_PASSWORD];		//用户密码
	char							szSpreader[LEN_NICKNAME];		//用户密码
};

//修改头像
#define SUB_GP_USER_FACE_INFO		120									//头像信息
#define SUB_GP_SYSTEM_FACE_INFO		121									//系统头像
#define SUB_GP_CUSTOM_FACE_INFO		122									//自定头像

//修改头像
struct CMD_GP_CustomFaceInfo
{
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_PASSWORD];			//用户密码
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
	dword							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//比赛服务
#define SUB_GP_MATCH_SIGNUP			200									//比赛报名
#define SUB_GP_MATCH_UNSIGNUP		201									//取消报名
#define SUB_GP_MATCH_SIGNUP_RESULT	202									//报名结果
#define SUB_GP_MATCH_AWARD_LIST		203									//比赛奖励

//签到服务
#define SUB_GP_CHECKIN_QUERY      220									//查询签到
#define SUB_GP_CHECKIN_INFO       221									//签到信息
#define SUB_GP_CHECKIN_DONE       222									//执行签到
#define SUB_GP_CHECKIN_RESULT     223									//签到结果
#define SUB_GP_CHECKIN_AWARD	  224									//签到奖励

//新手引导
#define SUB_GP_BEGINNER_QUERY		240								//新手引导签到
#define SUB_GP_BEGINNER_INFO		241								//新手引导信息
#define SUB_GP_BEGINNER_DONE		242								//新手引导执行
#define SUB_GP_BEGINNER_RESULT		243								//新手引导结果

//轮盘服务
#define SUB_GP_ROULETTE_USERINFO	250									//轮盘信息
#define SUB_GP_ROULETTE_DONE		251									//执行

//低保服务
#define SUB_GP_BASEENSURE_LOAD      260									//加载低保
#define SUB_GP_BASEENSURE_TAKE      261									//领取低保
#define SUB_GP_BASEENSURE_PARAMETER   262								//低保参数
#define SUB_GP_BASEENSURE_RESULT    263									//低保结果


//个人资料
#define SUB_GP_USER_INDIVIDUAL		301									//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL		302									//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL	303									//修改资料
#define SUB_GP_QUERY_ACCOUNTINFO	304									//个人信息
#define SUB_GP_QUERY_INGAME_SEVERID	305									//游戏状态
#define SUB_GP_MODIFY_STARVALUE		306									//评分

//银行服务
#define SUB_GP_USER_SAVE_SCORE		400									//存款操作
#define SUB_GP_USER_TAKE_SCORE		401									//取款操作
#define SUB_GP_USER_TRANSFER_SCORE	402									//转账操作
#define SUB_GP_USER_INSURE_INFO		403									//银行资料
#define SUB_GP_QUERY_INSURE_INFO	404									//查询银行
#define SUB_GP_USER_INSURE_SUCCESS	405									//银行成功
#define SUB_GP_USER_INSURE_FAILURE	406									//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//用户信息

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
	word							wRevenueTake;						//税收比例
	word							wRevenueTransfer;					//税收比例
	word							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//存入金币
struct CMD_GP_UserSaveScore
{
	dword							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	dword							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	char							szPassword[LEN_MD5];				//银行密码
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
	dword							dwUserID;							//用户 I D
	byte                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	char							szPassword[LEN_MD5];				//银行密码
	char							szNickName[LEN_NICKNAME];			//目标用户
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	dword							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	char							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	unsigned int					lResultCode;						//错误代码
	char							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	dword							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	dword							dwUserID;							//用户 I D
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	byte                            cbByNickName;                       //昵称赠送
	char							szNickName[LEN_NICKNAME];			//目标用户
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	dword							dwTargetGameID;						//目标用户
	char							szNickName[LEN_NICKNAME];			//目标用户
};
//个人信息
struct CMD_GP_UserAccountInfo
{
	//属性资料
	word							wFaceID;							//头像标识
	dword							dwUserID;							//用户标识
	dword							dwGameID;							//游戏标识
	dword							dwGroupID;							//社团标识
	dword							dwCustomID;							//自定索引
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//经验数值
	dword							dwLoveLiness;						//用户魅力
	dword							dwSpreaderID;						//推广ID
	char							szPassword[LEN_MD5];				//登录密码
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	char							szNickName[LEN_NICKNAME];			//用户昵称
	char							szGroupName[LEN_GROUP_NAME];		//社团名字
	char							szLogonIp[LEN_ACCOUNTS];			//登录IP

	//用户成绩
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//用户银行

	//用户资料
	byte							cbGender;							//用户性别
	byte							cbMoorMachine;						//锁定机器
	char							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//会员资料
	byte							cbMemberOrder;						//会员等级
	systemtime						MemberOverDate;						//到期时间
};
//游戏状态
struct CMD_GP_UserInGameServerID
{
	dword							dwUserID;							//用户 I D
};
//操作成功
struct CMD_GP_InGameSeverID
{
	dword							LockKindID;			
	dword							LockServerID;			
};
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//比赛报名
struct CMD_GP_MatchSignup
{
	//比赛信息
	word							wServerID;							//房间标识
	dword							dwMatchID;							//比赛标识
	dword							dwMatchNO;							//比赛场次

	//用户信息
	dword							dwUserID;							//用户标识
	char							szPassword[LEN_MD5];				//登录密码

	//机器信息
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取消报名
struct CMD_GP_MatchUnSignup
{
	//比赛信息
	word							wServerID;							//房间标识
	dword							dwMatchID;							//比赛标识
	dword							dwMatchNO;							//比赛场次

	//用户信息
	dword							dwUserID;							//用户标识
	char							szPassword[LEN_MD5];				//登录密码

	//机器信息
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//报名结果
struct CMD_GP_MatchSignupResult
{
	bool							bSignup;							//报名标识
	bool							bSuccessed;							//成功标识
	char							szDescribeString[128];				//描述信息
};
//报名结果
struct CMD_GP_MatchGetAward
{
	dword							dwUserID;	
	dword							dwMatchID;		//比赛标识
	dword							dwMatchNO;		//比赛场次	
};

//排行信息
struct tagMatchAwardkInfo
{
	word							MatchRank;							//比赛名次
	SCORE							RewardGold;							//奖励金币
	dword							RewardMedal;						//奖励元宝
	dword							RewardExperience;					//奖励经验
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(MatchRank);
		Stream_VALUE(RewardGold);
		Stream_VALUE(RewardMedal);
		Stream_VALUE(RewardExperience);
	}
};

//比赛奖励
struct CMD_GR_MatchAwardList
{
	dword							dwMatchID;							//比赛标识
	dword							dwMatchNO;							//比赛场次
	std::vector<tagMatchAwardkInfo> kAwards;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwMatchID);
		Stream_VALUE(dwMatchNO);
		StructVecotrMember(tagMatchAwardkInfo,kAwards);
	}
};
//////////////////////////////////////////////////////////////////////////////////


//自定义字段查询 公告
#define SUB_GP_QUERY_PUBLIC_NOTICE	500									//自定义字段查询
#define	SUB_GP_PUBLIC_NOTICE		501

//设置推荐人结果
#define SUB_GP_SPREADER_RESOULT		520									//设置推荐人结果

//赚金排行榜
#define SUB_GP_ADDRANK_GET_AWARD_INFO	540									//获得奖励信息
#define SUB_GP_ADDRANK_BACK_AWARD_INFO	541									//返回奖励信息
#define SUB_GP_ADDRANK_GET_RANK			542									//获得排行
#define SUB_GP_ADDRANK_BACK_RANK		543									//返回排行

//游戏记录
#define SUB_GP_GAME_RECORD_LIST			550			
#define SUB_GP_GAME_RECORD_TOTAL		551		
#define SUB_GP_GAME_RECORD_CHILD		552	

//客服信息
#define SUB_GP_KEFU					600									//客服信息
#define SUB_GP_KEFU_RESULT			601									//客服信息结果

//邮件信息
#define SUB_GP_MESSAGE_LIST			700									//邮件列表
#define SUB_GP_MESSAGE_LIST_RESULT	701									//邮件列表返回
#define SUB_GP_MESSAGE_AWARD		702									//获取排行榜奖励
#define SUB_GP_MESSAGE_AWARD_RESULT	703									//获取排行榜结果

//操作结果
#define SUB_GP_OPERATE_SUCCESS		900									//操作成功
#define SUB_GP_OPERATE_FAILURE		901									//操作失败

//话费兑换
#define  SUB_GP_EXCHANGEHUAFEI_GET_LIST_INFO    1000						//获取兑换话费列表
#define  SUB_GP_EXCHANGEHUAFEI_BACK				1001						//兑换话费列表返回
#define  SUB_GP_EXCHANGE_DONE					1002						//兑换

//游戏记录
struct CMD_GP_GetGameRecordList
{
	dword							dwUserID;							
};

//商城数据
#define  SUB_GP_SHOPINFO_GET_LIST_INFO		1100                     //获取商城列表
#define  SUB_GP_SHOPINFO_BACK				1101					//商城列表返回
#define  SUB_GP_SHOPINFO_BUYODER			1102					//购买
#define  SUB_GP_SELLITEM					1103					//
#define  SUB_GP_SELLLIST_BACK				1104					//

//游戏记录
struct CMD_GP_GetGameTotalRecord
{
	dword							dwUserID;			
	dword							dwRecordID;						
};

//游戏记录
struct CMD_GP_GetGameChildRecord
{
	dword							dwUserID;			
	dword							dwRecordID;						
};

struct tagPrivateRandRecordChild
{
	dword						dwKindID;
	dword						dwVersion;
	int							iRecordID;
	int							iRecordChildID;
	std::vector<int>			kScore;
	systemtime					kPlayTime;
	datastream					kRecordGame;
	std::string					kUserDefine;
	tagPrivateRandRecordChild()
	{
		iRecordID = 0;
		iRecordChildID = 0;
	}
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(iRecordID);
		Stream_VALUE(iRecordChildID);
		Stream_VECTOR(kScore);
		Stream_DATA(kRecordGame);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
		Stream_VALUE(kUserDefine);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
	}
};

//用户一轮总输赢
struct tagPrivateRandTotalRecord
{
	tagPrivateRandTotalRecord()
	{
		iRoomNum = 0;
		iRecordID = 0;
	}
	dword						dwKindID;
	dword						dwVersion;
	int							iRoomNum;
	int							iRecordID;
	std::vector<int>			kScore;
	std::vector<int>			kUserID;
	std::vector<std::string>	kNickName;
	systemtime					kPlayTime;
	std::string					kUserDefine;

	std::vector<tagPrivateRandRecordChild>	kRecordChild;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(iRoomNum);
		Stream_VECTOR(kUserID);
		Stream_VECTOR(kNickName);
		Stream_VECTOR(kScore);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
		StructVecotrMember(tagPrivateRandRecordChild,kRecordChild);
		Stream_VALUE(iRecordID);
		Stream_VALUE(kUserDefine);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
	}
};
struct tagPrivateRandTotalRecordList
{
	dword		dwUserID;
	std::vector<tagPrivateRandTotalRecord> kList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		StructVecotrMember(tagPrivateRandTotalRecord,kList);
	}
};
struct CMD_GP_BackAddBankAwardInfo
{
	int							kRewardGold[3][LEN_ADDRANK];				
	int							kRewardType[3][LEN_ADDRANK];				
};

struct CMD_GP_GetAddBank
{
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_PASSWORD];			//用户密码
	int								iRankIdex;
};

struct CMD_GP_BackAddBank
{
	int iRankIdex;
	std::vector<std::string>		kNickName;
	std::vector<int>				kUserID;
	std::vector<int>				kFaceID;
	std::vector<int>				kCustomID;
	std::vector<int>				kUserPoint;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(iRankIdex);
		Stream_VECTOR(kNickName);
		Stream_VECTOR(kUserID);
		Stream_VECTOR(kFaceID);
		Stream_VECTOR(kCustomID);
		Stream_VECTOR(kUserPoint);
	}
};

struct CMD_GP_GetExchangeHuaFei
{
	dword						dwUserID;						//用户 I D
	char							szPassword[LEN_PASSWORD];			//用户密码
};

struct CMD_GP_ExchangeDone
{
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_PASSWORD];			//用户密码
	dword							dwExchangeID;						//兑换 I D
};

struct CMD_GP_BackExchangeHuaFei
{
	std::vector<int>				kExchangeID;            //兑换id
	std::vector<int>				kUseType;			   //兑换道具类型
	std::vector<int>				kUseNum;				   //兑换道具个数
	std::vector<int>				kGetType;			   //兑换商品类型
	std::vector<int>				kGetNum;				   //兑换商品个数
	std::vector<std::string>		kGoodsName;			   //兑换商品名称
	std::vector<std::string>		kExchangeDesc;		   //兑换描述
	std::vector<std::string>		kImgIcon;			   //图标名称
	std::vector<int>				kFlag;				   //标记

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(kExchangeID);
		Stream_VECTOR(kUseType);
		Stream_VECTOR(kUseNum);
		Stream_VECTOR(kGetType);
		Stream_VECTOR(kGetNum);
		Stream_VECTOR(kGoodsName);
		Stream_VECTOR(kExchangeDesc);
		Stream_VECTOR(kImgIcon);
		Stream_VECTOR(kFlag);
	}
};
struct CMD_GP_GetShopInfo
{
	dword						dwUserID;						//用户 I D
	char						szPassword[LEN_PASSWORD];		//用户密码
};

struct CMD_GP_UpShopOder
{
	dword						dwUserID;						//用户 I D
	dword						dwItemID;						//商品id
	dword						total_fee;						//金额
	std::string					appid;							//微信id
	std::string					mch_id;							//商户号
	std::string					prepay_id;						//准备订单号
	std::string					strOSType;						//系统号
	
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(dwItemID);
		Stream_VALUE(total_fee);
		Stream_VALUE(appid);
		Stream_VALUE(mch_id);
		Stream_VALUE(prepay_id);
		Stream_VALUE(strOSType);
	}
};


struct CMD_GP_BackShopInfo
{
	std::vector<int>				kItemID;					 //商品id
	std::vector<int>				kItemType;				 //商品类型
	std::vector<int>				kOrderID_IOS;				 //商品订单号 苹果
	std::vector<int>				kOrderID_Android;			 //商品订单号 安卓
	std::vector<int>				kPrice;					 //商品价格
	std::vector<int>				kGoodsNum;				 //商品数量
	std::vector<std::string>		kItemTitle;				 //标题
	std::vector<std::string>		kItemDesc;				 //描述
	std::vector<std::string>		kItemIcon;				 //图标名称
	std::vector<std::string>		kItemName;				 //商品名称

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(kItemID);
		Stream_VECTOR(kItemType);
		Stream_VECTOR(kOrderID_IOS);
		Stream_VECTOR(kOrderID_Android);
		Stream_VECTOR(kPrice);
		Stream_VECTOR(kGoodsNum);
		Stream_VECTOR(kItemTitle);
		Stream_VECTOR(kItemDesc);
		Stream_VECTOR(kItemIcon);
		Stream_VECTOR(kItemName);
	}
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//用户昵称
#define DTP_GP_UI_USER_NOTE			2									//用户说明
#define DTP_GP_UI_UNDER_WRITE		3									//个性签名
#define DTP_GP_UI_QQ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		7									//移动电话
#define DTP_GP_UI_COMPELLATION		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	9									//联系地址
#define DTP_GP_UI_HEAD_HTTP			10									//头像
#define DTP_GP_UI_IP				11									//IP
#define DTP_GP_UI_CHANNEL			12									//渠道号
#define DTP_GP_UI_GPS				13									//GPS
//////////////////////////////////////////////////////////////////////////////////

//查询信息
struct CMD_GP_QueryIndividual
{
	dword							dwUserID;							//用户 I D
};
//查询信息
struct CMD_GP_QueryAccountInfo
{
	dword							dwUserID;							//用户 I D
};

//个人资料
struct CMD_GP_UserIndividual
{
	dword							dwUserID;							//用户 I D
	dword							dwUserStarValue;					//评分
	dword							dwUserStarCout;						//评分
};


//个人资料
struct TAG_UserIndividual
{
	dword							dwUserID;							//用户 I D
	dword							dwStarValue;
	dword							dwStarCout;
	std::string						kIP;
	std::string						kHttp;
	std::string						kChannel;
	std::string						kGPS;
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	byte							cbGender;							//用户性别
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_PASSWORD];			//用户密码
};

//修改评分
struct  CMD_GP_ModifyStarValue
{
	dword							dwUserID;							
	dword							dwStarValue;						
};
//操作成功
struct CMD_GP_SpreaderResoult
{
	unsigned int							lResultCode;						//操作代码
	SCORE							lScore;
	char							szDescribeString[128];				//成功消息
};
//操作成功
struct CMD_GP_OperateSuccess
{
	unsigned int					lResultCode;						//操作代码
	char							szDescribeString[128];				//成功消息
};
//操作失败
struct CMD_GP_OperateFailure
{
	unsigned int					lResultCode;						//错误代码
	char							szDescribeString[128];				//描述消息
};

//签到奖励
struct DBO_GP_CheckInReward
{
	SCORE							lRewardGold[LEN_SIGIN];				//奖励金额
	byte							lRewardType[LEN_SIGIN];				//奖励类型 1金币 2道具
	byte							lRewardDay[LEN_SIGIN];				//奖励天数 
};
//查询签到
struct CMD_GP_CheckInQueryInfo
{
	dword							dwUserID;							//用户标识
	char							szPassword[LEN_PASSWORD];			//登录密码
};

//签到信息
struct CMD_GP_CheckInInfo
{	
	word							wSeriesDate;						//连续日期
	word							wAwardDate;							//物品日期
	bool							bTodayChecked;						//签到标识
	SCORE							lRewardGold[LEN_SIGIN];				//奖励金额
	byte							lRewardType[LEN_SIGIN];				//奖励类型 1金币 2道具
	byte							lRewardDay[LEN_SIGIN];				//奖励天数 
};

//执行签到
struct CMD_GP_CheckInDone
{
	dword							dwUserID;							//用户标识
	char							szPassword[LEN_PASSWORD];			//登录密码
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//签到结果
struct CMD_GP_CheckInResult
{
	bool							bType;								//是否是达到天数领取物品
	bool							bSuccessed;							//成功标识
	SCORE							lScore;								//当前金币
	char							szNotifyContent[128];				//提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//轮盘
//签到结果
struct CMD_GP_ReqRouletteUeserInfo
{
	dword							dwUserID;							//用户标识
	dword							dwVaildDay;							//有效时间
};
struct CMD_GP_BackRouletteUeserInfo
{
	dword							dwUserID;							//用户标识
	dword							dwHaveDone;
	byte							cbCout;
	SCORE							lRewardGold[LEN_ROULETTE];			//奖励金额
	byte							cbRewardType[LEN_ROULETTE];			//奖励类型 1金币 2房卡
	byte							cbRouletteIdex[LEN_ROULETTE];		//奖励索引
	byte							cbAwardPercent[LEN_ROULETTE];		//奖励索引
};
struct CMD_GP_ReqRouletteDone
{
	dword							dwUserID;							//用户标识
};
struct CMD_GP_BackRouletteDone
{
	dword							dwUserID;							//用户标识
	SCORE							lRewardGold;						//奖励金额
	byte							cbRewardType;						//奖励类型 1金币 2房卡
	byte							cbRewardIdex;	

};
//////////////////////////////////////////////////////////////////////////////////
//新手活动

struct CMD_GP_BeginnerQueryInfo
{
	dword							dwUserID;							//用户标识
	char							szPassword[LEN_PASSWORD];			//登录密码
};
struct CMD_GP_BeginnerInfo
{	
	enum AwardType
	{
		Type_Gold = 1,
		Type_Phone = 2,
	};
	word							wSeriesDate;						//连续日期
	bool							bTodayChecked;						//签到标识
	bool							bLastCheckIned;						//签到标识
	SCORE							lRewardGold[LEN_BEGINNER];			//奖励金额
	byte							lRewardType[LEN_BEGINNER];			//奖励类型 1金币 2道具
};
struct CMD_GP_BeginnerDone
{
	dword							dwUserID;							//用户标识
	char							szPassword[LEN_PASSWORD];			//登录密码
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};
struct CMD_GP_BeginnerResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lAwardCout;							//奖励数量
	SCORE							lAwardType;							//奖励类型
	char							szNotifyContent[128];				//提示内容
};
//领取低保
struct CMD_GP_BaseEnsureTake
{
	dword							dwUserID;							//用户 I D
	char							szPassword[LEN_PASSWORD];			//登录密码
	char							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//低保参数
struct CMD_GP_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	byte							cbTakeTimes;						//领取次数	
};
//低保结果
struct CMD_GP_BaseEnsureResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lGameScore;							//当前游戏币
	char							szNotifyContent[128];				//提示内容
};

//自定义字段查询 公告
struct CMD_GP_QueryNotice
{
	char							szKeyName[LEN_NICKNAME];			//关键字
};
struct CMD_GP_PublicNotice
{
	unsigned int					lResultCode;						//操作代码
	char							szDescribeString[512];				//成功消息
};
//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//时间奖励
#define SUB_GP_C_TIME_AWARD_CHECK	110									//时间奖励信息查询
#define SUB_GP_C_TIME_AWARD_GET		111									//时间奖励领取

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找


//时间奖励
#define SUB_GP_S_TIME_AWARD_CHECK	210									//时间奖励信息查询结果
#define SUB_GP_S_TIME_AWARD_GET		211									//时间奖励领取结果



//登录模式
#define SUB_MB_LOGON_GAMEID			2									//I D 登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

//帐号登录
struct CMD_GP_LogonAccounts
{
#if 1
	dword								dwPlazaVersion;					//广场版本
	char								szAccounts[NAME_LEN];			//登录帐号
	char								szPassWord[PASS_LEN];			//登录密码
#else
	//登录信息
	dword							dwPlazaVersion;						//广场版本
	char							szMachineID[LEN_MACHINE_ID];		//机器序列

	char							szPassword[LEN_MD5];				//登录密码
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	dword							cbValidateFlags;			        //校验标识
#endif
};

//登录失败
struct DBR_GP_LogonError
{
	unsigned int					lErrorCode;							//错误代码
	char							szErrorDescribe[128];				//错误消息
};


//I D登录
struct CMD_GP_LogonByUserID
{
	//登录信息
	dword							dwGameID;							//游戏 I D
	char							szPassword[LEN_MD5];				//登录密码
	byte							cbValidateFlags;			        //校验标识
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	//系统信息
	dword							dwPlazaVersion;						//广场版本
	char							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	char							szLogonPass[LEN_MD5];				//登录密码
	char							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	word							wFaceID;							//头像标识
	byte							cbGender;							//用户性别
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	char							szNickName[LEN_NICKNAME];			//用户昵称
	char							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	char							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	char							szCompellation[LEN_COMPELLATION];	//真实名字
	byte							cbValidateFlags;			        //校验标识
};

//游客登录
struct CMD_GP_VisitorLogon
{
	word								wFaceID;						//头像标识
	byte								cbGender;						//用户性别
	dword								dwPlazaVersion;					//广场版本
	byte								cbValidateFlags;			        //校验标识
	char								szNickName[LEN_NICKNAME];		//用户昵称
	char								szSpreader[LEN_NICKNAME];		//推广人名
	char								szPassWord[LEN_PASSWORD];		//登录密码
	char								szPassWordBank[LEN_PASSWORD];	//登录密码
};


//登陆成功
struct CMD_GP_LogonSuccess
{
#if 1
	word								wFaceID;						//头像索引
	byte								cbGender;						//用户性别
	byte								cbMember;						//会员等级
	dword								dwUserID;						//用户 I D
	dword								dwGameID;						//游戏 I D
	dword								dwExperience;					//用户经验
	
	//扩展信息
	dword								dwCustomFaceVer;				//头像版本
	byte								cbMoorMachine;					//锁定机器
	dword                               dwLockServerID;                 //被锁的服务器id,不为0就是被锁了

	char								szNickName[NAME_LEN];	            //昵称
	longlong							lUserScore;							//游戏金币
	longlong                            lUserInsure;                          //用户的金币 2011-9-30前为宝石 lGems
	longlong                            lGem;                           //用户的宝石 2011-9-30前为荣誉 lHonor
	dword                               dwGrade;                        //用户的等级（不超过256） 2011-9-30前为dwClass

	int								lWinCount;							//胜利盘数
	int								lLostCount;							//失败盘数
	int								lDrawCount;							//和局盘数
	int								lFleeCount;							//断线数目

	int                               utIsAndroid;						//是否是机器人
	char                              szKey[33];							//网站用的密钥
	int                               lGiftNum;							//领取赠送的次数（目前是一天一次）

	// add by HouGuoJiang 2011-11-25
	char				szHashID[33];		// 碎片升级用的版本号
	char				szReserve[48];		// 保留，日后扩展使用，就不需要每个玩家更新大厅了，若用扩充大容量的内容，就单独发消息好了

	int									nMasterRight;						//权限管理
	int									nMasterOrder;						//权限等级
	char							    szUnknown[53];
#else
	word								wFaceID;							//头像标识
	dword								dwUserID;							//用户 I D
	dword								dwGameID;							//游戏 I D
	dword								dwGroupID;							//社团标识
	dword								dwCustomID;							//自定标识
	dword								dwUserMedal;						//用户奖牌
	dword								dwExperience;						//经验数值
	dword								dwLoveLiness;						//用户魅力
	dword								dwSpreaderID;						//推广ID
	byte								cbInsureEnabled;					//银行开通

	longlong							lUserScore;							//用户金币
	longlong							lUserInsure;						//用户银行

	//用户信息
	byte								cbGender;							//用户性别
	byte								cbMoorMachine;						//锁定机器
	char								szAccounts[LEN_ACCOUNTS];			//登录帐号
	char								szNickName[LEN_ACCOUNTS];			//用户昵称
	char								szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	byte								 cbShowServerStatus;                 //显示服务器状态
#endif
};

//列表配置
struct CMD_GP_ListConfig
{
	byte		bShowOnLineCount;				//显示人数
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST			101									//列表信息

//列表信息
#define SUB_MB_LIST_TYPE			100									//游戏类型
#define SUB_MB_LIST_KIND			101									//种类列表
#define SUB_MB_LIST_NODE			102									//节点列表
#define SUB_MB_LIST_PAGE			103									//定制列表
#define SUB_MB_LIST_SERVER			104									//房间列表
#define SUB_MB_LIST_MATCH			105									//比赛列表
#define SUB_MB_LIST_FINISH			200									//列表完成

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif

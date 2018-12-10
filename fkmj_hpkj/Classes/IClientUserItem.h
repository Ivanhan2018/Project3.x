#ifndef _IClientUserItem_H_
#define _IClientUserItem_H_

#include "PlatformHeader.h"

//用户接口
class IClientUserItem
{
public:
	virtual ~IClientUserItem(){};

	//属性接口
public:
	//重要等级
	virtual long GetImportOrder()=0;
	//用户信息
	virtual tagUserInfo * GetUserInfo()=0;
	//自定头像
	virtual tagCustomFaceInfo * GetCustomFaceInfo()=0;
	//道具包裹
	virtual tagPropertyPackage * GetPropertyPackage()=0;

	//头像信息
public:
	//头像索引
	virtual word GetFaceID()=0;
	//自定索引
	virtual dword GetCustomID()=0;

	//属性信息
public:
	//用户性别
	virtual byte GetGender()=0;
	//用户标识
	virtual dword GetUserID()=0;
	//游戏标识
	virtual dword GetGameID()=0;
	//社团标识
	virtual dword GetGroupID()=0;
	//用户昵称
	virtual const char* GetNickName()=0;
	//社团名字
	virtual const char* GetGroupName()=0;
	//个性签名
	virtual const char* GetUnderWrite()=0;

	//经验魅力
public:
	//用户奖牌
	//virtual dword GetUserMedal()=0;
	//经验数值
	virtual dword GetUserExperience()=0;
	//魅力数值
	virtual long GetUserLoveLiness()=0;

	//等级信息
public:
	//会员等级
	virtual byte GetMemberOrder()=0;
	//管理等级
	virtual byte GetMasterOrder()=0;

	//用户状态
public:
	//用户桌子
	virtual word GetTableID()=0;
	//用户椅子
	virtual word GetChairID()=0;
	//用户状态
	virtual byte GetUserStatus()=0;
	//用户数
	//virtual dword GetActiveUserCount() = 0;

	//游戏信息
public:
	//积分数值
	virtual SCORE GetUserScore()=0;
	//成绩数值
	virtual SCORE GetUserGrade()=0;
	//银行数值
	virtual SCORE GetUserInsure()=0;

	//游戏信息
public:
	//胜利盘数
	virtual dword GetUserWinCount()=0;
	//失败盘数
	virtual dword GetUserLostCount()=0;
	//和局盘数
	virtual dword GetUserDrawCount()=0;
	//逃跑盘数
	virtual dword GetUserFleeCount()=0;
	//游戏局数
	virtual dword GetUserPlayCount()=0;

	//比率信息
public:
	//用户胜率
	virtual float GetUserWinRate()=0;
	//用户输率
	virtual float GetUserLostRate()=0;
	//用户和率
	virtual float GetUserDrawRate()=0;
	//用户逃率
	virtual float GetUserFleeRate()=0;

	//用户关系
public:
	//获取关系
	virtual byte GetUserCompanion()=0;
	//设置关系
	virtual void SetUserCompanion(byte cbCompanion)=0;

	//用户备注
public:
	//获取备注
	virtual const char* GetUserNoteInfo()=0;
	//设置备注
	virtual void SetUserNoteInfo(const char* pszUserNote)=0;
};

#endif //_IClientUserItem_H_
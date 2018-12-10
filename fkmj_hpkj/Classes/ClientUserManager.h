#ifndef _CGameUserManager_H_
#define _CGameUserManager_H_

#include <vector>

#include "IUserManagerSink.h"
#include "IClientUserItem.h"
#include "PlatformHeader.h"
//////////////////////////////////////////////////////////////////////////////////

//用户信息
class CClientUserItem : public IClientUserItem
{
	//友元定义
	friend class CGameUserManager;
	friend class CPlazaUserManager;

	//属性变量
protected:
	tagUserInfo						m_UserInfo;							//用户信息
	tagCustomFaceInfo				m_CustomFaceInfo;					//自定头像
	tagPropertyPackage              m_PropertyPackage;                  //道具包裹

	//扩展属性
protected:
	byte							m_cbCompanion;						//用户关系
	char							m_szUserNote[LEN_USERNOTE];			//用户备注

	//函数定义
public:
	//构造函数
	CClientUserItem();
	//析构函数
	virtual ~CClientUserItem();

	//属性接口
public:
	//重要等级
	virtual long GetImportOrder();
	//用户信息
	virtual tagUserInfo * GetUserInfo() { return &m_UserInfo; }
	//自定头像
	virtual tagCustomFaceInfo * GetCustomFaceInfo() { return &m_CustomFaceInfo; }
	//道具包裹
	virtual tagPropertyPackage * GetPropertyPackage() { return &m_PropertyPackage; };

	//头像信息
public:
	//头像索引
	virtual word GetFaceID() { return m_UserInfo.wFaceID; }
	//自定索引
	virtual dword GetCustomID() { return m_UserInfo.dwCustomID; }

	//属性信息
public:
	//用户性别
	virtual byte GetGender() { return m_UserInfo.cbGender; }
	//用户标识
	virtual dword GetUserID() { return m_UserInfo.dwUserID; }
	//游戏标识
	virtual dword GetGameID() { return m_UserInfo.dwGameID; }
	//社团标识
	virtual dword GetGroupID() { return m_UserInfo.dwGroupID; }
	//用户昵称
	virtual const char* GetNickName() { return m_UserInfo.szNickName; }
	//社团名字
	virtual const char* GetGroupName() { return m_UserInfo.szGroupName; }
	//个性签名
	virtual const char* GetUnderWrite() { return m_UserInfo.szUnderWrite; }

	//经验魅力
public:
	//用户奖牌
//	virtual dword GetUserMedal() { return m_UserInfo.dwUserMedal; }
	//经验数值
	virtual dword GetUserExperience() { return m_UserInfo.lExperience; }
	//魅力数值
	virtual long GetUserLoveLiness() { return m_UserInfo.lLoveLiness; }

	//等级信息
public:
	//会员等级
	virtual byte GetMemberOrder() { return m_UserInfo.cbMemberOrder; }
	//管理等级
	virtual byte GetMasterOrder() { return m_UserInfo.cbMasterOrder; }

	//用户状态
public:
	//用户桌子
	virtual word GetTableID() { return m_UserInfo.wTableID; }
	//用户椅子
	virtual word GetChairID() { return m_UserInfo.wChairID; }
	//用户状态
	virtual byte GetUserStatus() { return m_UserInfo.cbUserStatus; }


	//积分信息
public:
	//积分数值
	virtual SCORE GetUserScore() { return m_UserInfo.lScore; }
	//成绩数值
	virtual SCORE GetUserGrade() { return m_UserInfo.lGrade; }
	//银行数值
	virtual SCORE GetUserInsure() { return m_UserInfo.lInsureScore; }

	//游戏信息
public:
	//胜利盘数
	virtual dword GetUserWinCount() { return m_UserInfo.lWinCount; }
	//失败盘数
	virtual dword GetUserLostCount() { return m_UserInfo.lLostCount; }
	//和局盘数
	virtual dword GetUserDrawCount() { return m_UserInfo.lDrawCount; }
	//逃跑盘数
	virtual dword GetUserFleeCount() { return m_UserInfo.lFleeCount; }
	//游戏局数
	virtual dword GetUserPlayCount() { return m_UserInfo.lWinCount+m_UserInfo.lLostCount+m_UserInfo.lDrawCount+m_UserInfo.lFleeCount; }

	//比率信息
public:
	//用户胜率
	virtual float GetUserWinRate();
	//用户输率
	virtual float GetUserLostRate();
	//用户和率
	virtual float GetUserDrawRate();
	//用户逃率
	virtual float GetUserFleeRate();

	//用户关系
public:
	//获取关系
	virtual byte GetUserCompanion() { return m_cbCompanion; }
	//设置关系
	virtual void SetUserCompanion(byte cbCompanion) { m_cbCompanion=cbCompanion; }

	//用户备注
public:
	//设置备注
	virtual void SetUserNoteInfo(const char* pszUserNote);
	//获取备注
	virtual const char* GetUserNoteInfo() { return m_szUserNote; }
};


//用户数组
typedef std::vector<CClientUserItem *> ClientUserItemVector;

//////////////////////////////////////////////////////////////////////////////////

//用户管理
class CPlazaUserManager
{
	//变量定义
protected:
	ClientUserItemVector		m_UserItemActive;					//活动数组
	ClientUserItemVector		m_PoolItem;					//删除数组

	//组件接口
protected:
	IUserManagerSink *				m_pIUserManagerSink;			//通知接口
	//IUserInformation *				m_pIUserInformation;		//用户信息

	//函数定义
public:
	//构造函数
	CPlazaUserManager();
	//析构函数
	virtual ~CPlazaUserManager();

	void addPool(CClientUserItem * pIClientUserItem);
	CClientUserItem * getPool();

	//配置接口
public:
	////设置接口
	//virtual bool SetUserInformation(IUserInformation * pIUserInformation);
	//设置接口
	virtual bool SetUserManagerSink(IUserManagerSink * pIUserManagerSink);

	//管理接口
public:
	//重置用户
	virtual bool ResetUserItem();
	//删除用户
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem);
	//增加用户
	virtual IClientUserItem * ActiveUserItem(const tagUserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo);

	//更新接口
public:
	//更新积分
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, const tagUserScore * pUserScore);
	//更新状态
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus);
	//更新属性
	virtual bool UpdateUserItemAttrib(IClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib);
	//更新头像
	virtual bool UpdateUserCustomFace(IClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo);


	//查找接口
public:
	//枚举用户
	virtual IClientUserItem * EnumUserItem(word wEnumIndex);
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(dword dwUserID);
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(dword dwGameID);
	//查找用户
	virtual IClientUserItem * SearchUserByNickName(const char* pszNickName);

public:
	//获得人数
	virtual dword GetActiveUserCount(){return (dword)m_UserItemActive.size();}
};

//////////////////////////////////////////////////////////////////////////////////

#endif
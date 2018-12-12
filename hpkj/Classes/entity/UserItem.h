//
//  UserItem.h
//  Game
//
//  Created by zhouwei on 13-6-21.
//
//

#ifndef __Game__UserItem__
#define __Game__UserItem__

#include <iostream>
#include "IUserItem.h"

//用户信息类
class CUserItem : public IUserItem
{
	//函数定义
public:
	//构造函数
	CUserItem();
    
    CUserItem(tagUserData* tag);
	//析构函数
	virtual ~CUserItem();
    
	//基础接口
public:
    
	//对象接口
public:
	//访问判断
	virtual bool  IsActive() { return m_bActive; }
    
	//属性接口
public:
	//游戏局数
	virtual unsigned int getUserPlayCount();
	//用户 I D
	virtual DWORD getUserID() { return m_UserData.dwUserID; }
    //用户id
    virtual unsigned int getFaceID() { return m_UserData.wFaceID; }
	//用户名字
	virtual  char* getUserName() { return m_UserData.szName; }
	//获取用户
	virtual tagUserData * getUserData() { return &m_UserData; }
    //获取用户椅子号
    virtual unsigned int getChairID() { return m_UserData.wChairID; }
    //设置用户椅子号
    virtual void setChairID(unsigned int nChairID) { m_UserData.wChairID =  nChairID; }
    //获取用户桌子号
    virtual unsigned int getTableID() { return m_UserData.wTableID; }
	//用户昵称<>
	virtual char* getUserNickName() { return m_UserData.szNickName; }
	//用户金币<>
	virtual LONG getUserGold() { return m_UserData.lScore; }
    //设置用户桌子号
    //virtual void setTableID(unsigned int nTableID){ m_UserData.wTableID = nTableID; }
	//设置接口
public:
	//设置积分
	virtual void setUserScore(const tagUserScore * pUserScore);
	virtual void setUserScore(const tagMobileUserScore * pUserScore);
	//设置金币
	virtual void setUserGold(const tagUserGold * pUserGold);
	//设置状态
	virtual void setUserStatus(const tagUserStatus * pUserStatus);
	//设置名次
	virtual void setUserSort(const unsigned int nSortID, const EN_MatchStatus enMatchStatus);
	//设置比赛积分
	virtual void setUserMatchScore(const unsigned int nMatchScore);
    
	//功能函数
public:
    //设置用户数据
    void setUserDate(tagUserData* tag);
    
	//清理数据
	void cleanData();
    
    //变量定义
public:
	tagUserData						m_UserData;						//用户信息
    
	//内部变量
private:
	bool							m_bActive;						//激活有效
};


#endif /* defined(__Game__UserItem__) */

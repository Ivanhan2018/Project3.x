//
//  IUserItem.h
//  用户信息接口类
//
//  Created by zhouwei on 13-6-21.
//
//

#include <stdio.h>
#include <string>
#include "GlobalDef.h"

using namespace std;

struct IUserItem
{
    //用户id
    virtual DWORD getUserID() = 0;
    
    //用户id
    virtual unsigned int getFaceID() = 0;
    
    //用户名字
    virtual char* getUserName() = 0;
    
    //游戏局数
    virtual unsigned int getUserPlayCount() = 0;
    
    //获取用户
	virtual tagUserData*  getUserData() = 0;
    
    //获取用户椅子号
    virtual unsigned int getChairID() = 0;
    
    //设置用户椅子号
    virtual void setChairID(unsigned int nChairID) = 0;
    
    //获取用户桌子号
    virtual unsigned int getTableID() = 0;
    
    //设置用户桌子号
   // virtual void setTableID(unsigned int nTableID) = 0;
    
	//设置接口
public:
	//设置积分
	virtual void setUserScore(const tagUserScore * pUserScore)=0;
	
    //设置金币
	virtual void setUserGold(const tagUserGold * pUserGold)=0;
	
    //设置状态
	virtual void setUserStatus(const tagUserStatus * pUserStatus)=0;
	
    //设置名次
	virtual void setUserSort(const unsigned int nSortID, const EN_MatchStatus enMatchStatus)=0;
	
    //设置比赛积分
	virtual void setUserMatchScore(const unsigned int nMatchScore) = 0;
};
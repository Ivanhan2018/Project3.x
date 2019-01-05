/*
** desc: 游戏实体类
** code: zhouwei
** date: 2013.06.9
*/

#ifndef	__ENTITYMGR_H__
#define __ENTITYMGR_H__

#include "AnsSingleton.h"
//#include "BaseObject.h"
#include "MsgDispatch.h"
#include "Login.h"
#include "RoomFrame.h"

class EntityMgr : public ans::Singleton<EntityMgr>
{
private:
	SINGLETON_FRIEND_SUB(EntityMgr);
	
public:
	EntityMgr();
	~EntityMgr();	

    MsgDispatch* getDispatch() { return m_pDispatch; }

	Login* login() { return m_pLogin; }
	RoomFrame* roomFrame(){ return m_pRoomFrame; }

	std::string& getMacAddress() {return macAddress;}
    void setMacAddress(const char* mac);
	void setDownLoadPesent(int pesent,int gameID);
	int getPesent(int gameID);
	int m_nDownLoadPe;
	bool m_bIsShowRuker;

	void startSchedule();
	void setOpenLockFlag(bool value); //用户是否已经解锁
	bool getOpenLockFlag();
private:
	Login* m_pLogin;
	RoomFrame* m_pRoomFrame;
    MsgDispatch* m_pDispatch;
    bool m_bLogin;             //登陆状态（登陆服务器或者游戏服务器）
	std::string macAddress;
	int m_nDownLoadPesent[4];
	bool isOpenLock;		//用户解锁
};

#endif //__ENTITYMGR_H__
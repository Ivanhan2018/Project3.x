#include "EntityMgr.h"
#include "platform/CCPlatformMacros.h"


SINGLETON_DEFINE_INST(EntityMgr);

EntityMgr::EntityMgr()
:m_pLogin(new Login())
,m_pRoomFrame(new RoomFrame())
,m_pDispatch(new MsgDispatch())
,macAddress("M123")
,isOpenLock(false)
{
	
}

EntityMgr::~EntityMgr()
{
	CC_SAFE_DELETE(m_pDispatch);
	CC_SAFE_DELETE(m_pLogin);
	CC_SAFE_DELETE(m_pRoomFrame);
}

void EntityMgr::startSchedule()
{
	for (int i = 0; i < 4; i ++)
	{
		m_nDownLoadPesent[i] = 0;
	}
	m_bIsShowRuker = false;
	m_pDispatch->startSchedule();	
}

void EntityMgr::setOpenLockFlag(bool value)
{
	isOpenLock = value;
}

bool EntityMgr::getOpenLockFlag()
{
	return isOpenLock;
}

void EntityMgr::setMacAddress(const char* mac)
{
	macAddress = mac;
}

void EntityMgr::setDownLoadPesent(int pesent,int gameID)
{
	m_nDownLoadPesent[gameID] = pesent;
}

int EntityMgr::getPesent(int gameID)
{
	return m_nDownLoadPesent[gameID];
}

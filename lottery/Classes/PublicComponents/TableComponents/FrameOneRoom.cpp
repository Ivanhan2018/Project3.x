#pragma once
#include "FrameOneRoom.h"
//#include "PlazaComponents/FramePlaza.h"

CFrameOneRoom *CFrameOneRoom::frameOneRoomWithGameServer(void *server, int index)
{
	CFrameOneRoom *cFrameOneRoom = new CFrameOneRoom();
	cFrameOneRoom->InitFrameOneRoomWithGameServer(server, index);
	return cFrameOneRoom;
}

void CFrameOneRoom::InitFrameOneRoomWithGameServer(void *server, int index)
{
	Layer::init();
	memset(&m_GameServer, 0, sizeof(tagGameServer));
	memcpy(&m_GameServer, server, sizeof(tagGameServer));

	m_nTableCount	= 6;
	m_nRoomIndex	= index;
	m_nPageIndex	= 0;
	m_RuleScore     = 20;
	this->m_pRoomName = "";
}


//进入房间事件
void CFrameOneRoom::quickEnterGame(Object *pSender)
{
	
	// 快速进入房间
	      // important
    //if ([CFramePlaza shareFramePlaza].m_cbType == SCROLLTYPE_ROOM)
	{
		// 提示
		//CFramePlaza::shareFramePlaza()->showFrameSpinnerView("正在查找位置..."/*@"正在查找位置..."*/, true);
		
		// 连接地址
		/*string
		NSString* address = [NSString CreateStringWithTHCAR:self.m_GameServer.szServerAddr size:sizeof(self.m_GameServer.szServerAddr)];
		if ([address compare:@"127.0.0.1"] == NSOrderedSame)
			address = [CFrameDate shareFrameDate].m_pAddress;
		[[CFrameDate shareFrameDate] DoConnect:address port:self.m_GameServer.wServerPort];
		
		// 设置登录标识
		[CFrameDate shareFrameDate].m_wBehaviorFlags = (BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
		
		// 进入房间
		[[CFrameDate shareFrameDate] SendPacketWithEnterRoomPageCount:m_nTableCount];
		
		// 登录标识
		[CFrameDate shareFrameDate].m_wBehaviorFlags = BEHAVIOR_LOGON_NORMAL;
		[CFrameDate shareFrameDate].m_bStartType = true;
        
        // 设置房间名字
        NSString* roomName=[NSString CreateStringWithTHCAR:self.m_GameServer.szServerName size:sizeof(self.m_GameServer.szServerName)];
        [[CFramePlaza shareFramePlaza] setObject:roomName key:@"roomName"];*/
        
	}
}

void CFrameOneRoom::ButtonEventWithEnterRoom(Object *pSender)
{
    //NSLog(@"ButtonEventWithEnterRoom enter");
    
	// 获得等待控件
	/*[[CFramePlaza shareFramePlaza] showFrameSpinnerView:@"正在进入房间..." close:YES];
	
	// 设置标识
	[CFrameDate shareFrameDate].m_wBehaviorFlags = (BEHAVIOR_LOGON_NORMAL|VIEW_MODE_PART);
	
	// 连接房间
	NSString* address=[NSString CreateStringWithTHCAR:m_GameServer.szServerAddr size:sizeof(m_GameServer.szServerAddr)];
	if ([address compare:@"127.0.0.1"] == NSOrderedSame)
		address=[CFrameDate shareFrameDate].m_pAddress;
	[[CFrameDate shareFrameDate] DoConnect:address port:m_GameServer.wServerPort];
	
	// 发送进入消息
	[[CFrameDate shareFrameDate] SendPacketWithEnterRoomPageCount:m_nTableCount];
	
	// 启动标识
	[CFrameDate shareFrameDate].m_bStartType = false;
    
    // 设置房间名字
    [[CFramePlaza shareFramePlaza] setObject:m_pRoomName key:@"roomName"];*/
}

void CFrameOneRoom::onTouchesEnded (const std::vector<Touch*>& pTouches, Event *pEvent)
{
	Touch *touch = (Touch *)pTouches.at(0);
	Vec2 location = touch->getLocationInView();
	Rect rect = this->boundingBox();
	if (rect.containsPoint(location))
    {
        this->quickEnterGame(NULL);
    }
}
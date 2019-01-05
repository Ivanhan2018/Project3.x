#ifndef __FrameOneRoom__
#define __FrameOneRoom__

#include "Define.h"
#include "struct.h"

class CFrameOneRoom: public Layer
{
	//新
private:
	int										m_nRoomIndex;			// 房间编号		0
	int										m_nPageIndex;			// 当前索引页		0
	int										m_nTableCount;			// 桌子数     默认：6

	long long                               m_RuleScore;            // 规则分数 这个有什么用。

	tagGameServer							m_GameServer;			// 房间信息

	string									m_pRoomName;            // 房间名字
public :
	int getRoomIndex(){return m_nRoomIndex;};
	void setRoomIndex(int flag){m_nRoomIndex = flag;};
	int getPageIndex(){return m_nPageIndex;};
	void setPageIndex(int index){m_nPageIndex = index;};
	tagGameServer getTagGameServer(){return m_GameServer;};
	void setTagGameServer(tagGameServer server){m_GameServer = server;};
	string getRoomName(){return m_pRoomName;};
	void setRoomName(string name){m_pRoomName = name;};
	//单个房间
	void InitFrameOneRoomWithGameServer(void *server, int index);
	static CFrameOneRoom *frameOneRoomWithGameServer(void *server, int index);
	//房间进入事件
	void quickEnterGame(Object *pSender);
	void ButtonEventWithEnterRoom(Object *pSender);
	virtual void  onTouchesEnded (const std::vector<Touch*>& pTouches, Event *pEvent);
};


#endif
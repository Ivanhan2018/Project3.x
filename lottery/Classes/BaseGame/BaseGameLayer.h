#ifndef __BASE_GAME_LAYER_H__
#define __BASE_GAME_LAYER_H__

#include "cocos2d.h"
#include "GBEventIDs.h"

USING_NS_CC;

#define NOTIFICATION_GAME_ENTERGAME "EnterGame"	//	消息——游戏——进入游戏

/*
**	基础类
**	游戏主场景层
*/
class BaseGameLayer: public Layer
{
public:
	BaseGameLayer();
	virtual ~BaseGameLayer();
	virtual bool init();

	//	处理进入游戏
	virtual void dealEnterGame(Ref* pSender) = 0;
	//	处理玩家信息
	virtual void dealPlayerInfo(Ref* pSender) = 0;
	//	处理游戏消息
	virtual void dealMessageGame(Ref* pSender) = 0;
	//	游戏场景
	virtual void onMessageScene(unsigned char cbStation,const void* pBuffer,unsigned short wDataSize) = 0;
	//	处理游戏场景消息
	virtual void dealMessageScene(Ref* sender);
	//	处理网络断开
	virtual void dealShutNetWork(Ref* sender);
	//	处理登录房间失败
	virtual void dealLoginRoomFailed(Ref* sender);
	//	处理坐下失败消息
	virtual void dealSitDownFaild(Ref* sender);

protected:
	//	添加通告消息
	virtual void addNotification();

	//	移除通告消息
	virtual void removeNotification();


};


#endif  // __BASE_GAME_LAYER_H__

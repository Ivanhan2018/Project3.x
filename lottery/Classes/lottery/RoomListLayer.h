#ifndef _RoomListLayer_H_
#define _RoomListLayer_H_

#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
#include "BaseLayer.h"

USING_NS_CC;
using namespace cocostudio;

class RoomListScrollView;

class RoomListLayer : public BaseLayer
{
public:
	//CREATE_FUNC(RoomListLayer);
	static RoomListLayer* create(int idx)
	{
		RoomListLayer *pRet = new RoomListLayer();
		if (pRet && pRet->init(idx))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}
	bool init(int idx);
	RoomListLayer();
	~RoomListLayer();

private:
	RoomListScrollView*		 m_ListView;
	ui::Button*					m_btnQuit;

	void buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
};


#endif // _RoomListLayer_H_


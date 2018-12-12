//
//  BaseLayer.h
//  Game
//
//  Created by ÂÞÃô on 13-6-19.
//
//

#ifndef __Game__BaseLayer__
#define __Game__BaseLayer__
#include "cocos2d.h"
#include "EntityMgr.h"
#include "GlobalUnits.h"
#include "GBEvent.h"
#include "CocosUnits.h"

USING_NS_CC;

class BaseLayer : public Layer
{
public:
    BaseLayer();
    virtual ~BaseLayer();
    
    CREATE_FUNC(BaseLayer);
    
    virtual bool init();
    
	//virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){Layer::onKeyReleased(keyCode,event);}

	void CloseNetWorkShut(Object *obj);

	void ReConNetWork(Object *obj);

	void ReadyToConNet(Object *obj);	

	virtual void refreshLayer(){}
};
#endif /* defined(__Game__BaseLayer__) */


#ifndef _SceneTableInterfaceLayer_Game_
#define _SceneTableInterfaceLayer_Game_

#include "cocos2d.h"
#include "CMD_BJL.h"
USING_NS_CC;

class SceneTableInterfaceLayer :public cocos2d::Layer
{
public:
		SceneTableInterfaceLayer();
		~SceneTableInterfaceLayer();
		CREATE_FUNC(SceneTableInterfaceLayer);
		virtual bool init();
		static cocos2d::Scene* scene();
		void  claosre(Object *obj);        //·µ»Ø
		void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);		

};



#endif 

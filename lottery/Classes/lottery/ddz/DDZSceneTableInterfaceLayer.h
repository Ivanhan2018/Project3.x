
#ifndef DDZ__DDZSceneTableInterfaceLayer_Game_
#define DDZ__DDZSceneTableInterfaceLayer_Game_

#include "cocos2d.h"
#include "DDZSceneData.h"
USING_NS_CC;

class DDZSceneTableInterfaceLayer :public cocos2d::Layer
{
public:
		DDZSceneTableInterfaceLayer();
		~DDZSceneTableInterfaceLayer();
		CREATE_FUNC(DDZSceneTableInterfaceLayer);
		virtual bool init();
		static cocos2d::Scene* scene();
		void  claosre(Object *obj);        //返回

		//virtual void keyBackClicked(void);					//手机返回按钮
};



#endif 

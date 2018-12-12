#ifndef __Game__ExitLayer__
#define __Game__ExitLayer__

#include "cocos2d.h"
#include "CocosUnits.h"
#include "cocos-ext.h"
USING_NS_CC;

class ExitLayer : public Layer {
public:
    CREATE_FUNC(ExitLayer);
	virtual bool init();
    ExitLayer();
    ~ExitLayer();

	void onOverRegist(Object* obj);
	void onFreeGetCoins(Object* obj);

	void close(Object* obj);
public:
    void resumeGame(Object* obj);		//继续游戏
	void exitGame(Object* obj);		//退出游戏
private:

};

#endif

#ifndef  _NET_POOL_H_
#define  _NET_POOL_H_

#include "cocos2d.h"
#include "NetSprite.h"
USING_NS_CC;

class NetPool : public Ref
{
public:
    static NetPool* createNetPool(int nNum);
	void addNet(int nNum);
	NetSprite* getNet(int nNetStatue);

private:
	bool initNetPool(int nNum);
	Vector<NetSprite*> m_vecNets;
};

#endif // _NET_POOL_H_


#include "NetPool.h"

NetPool* NetPool::createNetPool( int nNum )
{
	NetPool *pool = new NetPool;
	if (pool && pool->initNetPool(nNum))
	{
		pool->autorelease();
		return pool;
	}
	CC_SAFE_DELETE(pool);
	return nullptr;
}

bool NetPool::initNetPool( int nNum )
{
	m_vecNets.clear();
	this->addNet(nNum);
	return true;
}

void NetPool::addNet( int nNum )
{
	for (int i = 0; i < nNum; i++)
	{
		NetSprite* net = NetSprite::create(NET_2_NORMAL);
		m_vecNets.pushBack(net);
		net = NetSprite::create(NET_3_NORMAL);
		m_vecNets.pushBack(net);
		net = NetSprite::create(NET_4_NORMAL);
		m_vecNets.pushBack(net);
	}
}

NetSprite* NetPool::getNet(int nNetStatue)
{
	if((nNetStatue < NET_2_NORMAL) || (nNetStatue > NET_4_NORMAL))
	{
		nNetStatue = NET_2_NORMAL;
	}

	NetSprite *pNet = nullptr;
	for (int i = 0; i < m_vecNets.size(); i++)
	{
		pNet = (NetSprite*)m_vecNets.at(i);
		if (!pNet->getLive() && pNet->m_NetStatue == nNetStatue)
		{
			pNet->setLive(true);
			return pNet;
		}		
	}

	pNet = NetSprite::create(nNetStatue);
	m_vecNets.pushBack(pNet);
	pNet->setLive(true);
	return pNet;
}



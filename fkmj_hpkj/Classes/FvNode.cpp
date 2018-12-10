#include "FvNode.h"

NodeEx* NodeEx::create()
{
	NodeEx* pRet = new NodeEx();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

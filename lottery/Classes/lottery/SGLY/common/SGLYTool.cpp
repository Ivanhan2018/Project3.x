#include "SGLYTool.h"
#include "SGLYGameScene.h"

SGLYTool::SGLYTool()
{
}

SGLYTool::~SGLYTool()
{
}

DWORD SGLYTool::m_dwExchangeRate = 0;
LONGLONG SGLYTool::m_llApplyBankerCondition = 0;

LONGLONG SGLYTool::exchangeForGold(LONGLONG llValue)
{
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return -1;
	}
	LONGLONG llResult = llValue * m_dwExchangeRate;
	return llResult;
}

LONGLONG SGLYTool::exchangeForChip(LONGLONG llValue)
{
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return -1;
	}
	if (m_dwExchangeRate == 0)
	{
		return 0;
	}
	LONGLONG llResult = llValue / m_dwExchangeRate;
	return llResult;
}

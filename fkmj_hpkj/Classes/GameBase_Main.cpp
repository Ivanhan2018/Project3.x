#include "GameBase.h"

#include "GameLib.h"
#include "ScenceManagerBase.h"
#include "GameManagerBase.h"


//启动游戏
bool GameBase::SetupGameClient()
{
	return true;
}
//重置游戏
void GameBase::ResetGameClient()
{
	return;
}
//关闭游戏
void GameBase::CloseGameClient()
{
	ExitGame();
	return;
}
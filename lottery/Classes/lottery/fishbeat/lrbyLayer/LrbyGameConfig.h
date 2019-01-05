#ifndef __LKPY_GAMECONFIG_H__
#define __LKPY_GAMECONFIG_H__

#pragma once
#include "CMD_Fish.h"

class CLrbyGameConfig
{
public:
	CLrbyGameConfig(void);
	~CLrbyGameConfig(void);

	void setGameConfig(const NS_lrbyFish::CMD_S_GameConfig* config);
	bool IsSwitchingScene();
	void setSwitchingScene(bool bSwitch);
public:
	struct NS_lrbyFish::CMD_S_GameConfig m_gameConfig;

private:
	bool m_bSwitchingScene;
};

#endif //__GAME_GLOBALDATA_H__

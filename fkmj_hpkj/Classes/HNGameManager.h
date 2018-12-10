#pragma once

#include "cocos2d.h"
#include "GameManagerBase.h"

#include "FvSingleton.h"

class HNGameManager 
	:public GameManagerBase
	,public FvSingleton<HNGameManager>
{
public:
	HNGameManager();
	~HNGameManager();
public:
	void initGame();
	void StartXZDD();
	void StartXZDDRecord(datastream& kDataStream);
	void hideAll();

	IClientKernelSink* CreateGame(word wKindID);
	void loadGameBaseData(word wKindID);
};
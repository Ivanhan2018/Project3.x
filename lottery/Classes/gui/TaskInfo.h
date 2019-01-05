#ifndef __Task__Info__
#define __Task__Info__

#include "EntityMgr.h"

struct GameOverInfo
{
	int nScore;
	int nGoldEgg;
	GameOverInfo()
	{
		nScore = 0;
		nGoldEgg = 0;
	}
};

class TaskInfo
{
public:
	TaskInfo();
	~TaskInfo();
	static TaskInfo* ShareTaskInfo();

	void initData(DWORD dwData);
	int m_nGameCont;
	int m_nWinGoldEggCount;
	BYTE m_nGetState[3];
	int m_nTaskID[3];

	int m_nTaskConfig[6];	// »ŒŒÒ≈‰÷√
	int m_nUMMessage;

	void setGameOverDate(GameOverInfo& info);
private:

};
#endif  //__Game__WarningLayler__
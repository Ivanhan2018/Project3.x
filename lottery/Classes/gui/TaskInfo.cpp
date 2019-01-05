#include "TaskInfo.h"
#include "cocos2d.h"

TaskInfo::TaskInfo()
{
	memset(m_nGetState, 0, sizeof(m_nGetState));
	memset(m_nTaskID, 0, sizeof(m_nTaskID));
	m_nUMMessage = 0;
}

TaskInfo::~TaskInfo()
{

}

void TaskInfo::initData(DWORD dwData)
{
	for (int i=0; i<3; ++i)
	{
		m_nTaskID[i] = i+1;
		m_nGetState[i] = (dwData >> m_nTaskID[i]) & 1;
	}
}
TaskInfo* TaskInfo::ShareTaskInfo()
{
	static TaskInfo* instaceTaskInfo = NULL;
	if(!instaceTaskInfo)
	{
		instaceTaskInfo = new TaskInfo();
	}
	return instaceTaskInfo;
}

void TaskInfo::setGameOverDate(GameOverInfo& info)
{
	m_nGameCont++;
	if(info.nGoldEgg > 0 )
	{
		m_nWinGoldEggCount++;
	}
}
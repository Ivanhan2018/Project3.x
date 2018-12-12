/*
** implement thread safe task pool
** author: zhouwei
** modified date: 2012.06.21
*/

#ifndef __TASK_POOL_H__
#define __TASK_POOL_H__

#include <deque>
#include "AnsThread.h"

namespace ans {

template<typename Task>
class TaskPool 
{
public:
    TaskPool()
	{
		sp_thread_mutex_init(&m_mutex, NULL);
	}

	~TaskPool()
	{
		sp_thread_mutex_destroy(&m_mutex);
	}

    void pushTask(Task *pTask)
	{
		sp_thread_mutex_lock(&m_mutex);
		m_dqTasks.push_back(pTask);
		sp_thread_mutex_unlock(&m_mutex);
	}

    Task *popTask()
	{
		Task *pTask = NULL;
		sp_thread_mutex_lock(&m_mutex);
		if (m_dqTasks.size() > 0) 
		{
			pTask = m_dqTasks.front();
			m_dqTasks.pop_front();
		}
		sp_thread_mutex_unlock(&m_mutex);
		return pTask;
	}

	template<typename Func>
	void foreachFunc(Func func)
	{
		sp_thread_mutex_lock(&m_mutex);
		typename std::deque <Task*>::iterator iter = m_dqTasks.begin();
		while(iter != m_dqTasks.end())
			func(*iter);					// don't call "pushTask" and "popTask" function in Func
		sp_thread_mutex_unlock(&m_mutex);
	}

private:
    std::deque <Task*> m_dqTasks;
    sp_thread_mutex_t m_mutex;
};

}

#endif

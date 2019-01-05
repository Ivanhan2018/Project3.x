#ifndef GLOBALSCHEDULE_H_
#define GLOBALSCHEDULE_H_

#include "Inc.h"

/**
 *	全局定时器
 */
class GlobalSchedule: public CCNode
{
    
public:
    static void start();

    static void start(float fInterval = 0.0f, float fDelay = 0.0f);

    static void stop();

    static void pause();
    
    static void resume();
	
	void globalUpdate(float dt);
    
private:
    GlobalSchedule(float fInterval, float fDelay);
    ~GlobalSchedule();

    static GlobalSchedule* m_pSchedule;
    
    int index;
    
    bool schedule_mutex;
    
    bool deltaTime;
    
};

#endif

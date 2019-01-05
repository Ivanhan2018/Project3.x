#include "GlobalSchedule.h"
#include "PlayerModel.h"

#define SCHEDULE CCDirector::sharedDirector()->getScheduler()

GlobalSchedule* GlobalSchedule::m_pSchedule = NULL;

GlobalSchedule::GlobalSchedule(float fInterval, float fDelay)
{
    CCAssert(!m_pSchedule, "GlobalSchedule已定义，不能重复定义");
	SCHEDULE->scheduleSelector(schedule_selector(GlobalSchedule::globalUpdate),this,fInterval,kRepeatForever,fDelay,false);

    m_pSchedule = this;
    
    index=0;
    schedule_mutex=true;
    deltaTime=false;
}

GlobalSchedule::~GlobalSchedule()
{
    //DLog::showLog("GlobalSchedule().~()");
	SCHEDULE->unscheduleSelector(schedule_selector(GlobalSchedule::globalUpdate), this);
}

void GlobalSchedule::start(float fInterval, float fDelay)
{
	m_pSchedule=new GlobalSchedule(fInterval, fDelay);
}

void GlobalSchedule::stop()
{
	CC_SAFE_DELETE(m_pSchedule);
}

void GlobalSchedule::pause()
{
	SCHEDULE->pauseTarget(m_pSchedule);
}

void GlobalSchedule::resume()
{
	SCHEDULE->resumeTarget(m_pSchedule);
}

void GlobalSchedule::globalUpdate(float dt)
{
    //index++;
    //DLog::showLog("globalUpdate..and serverTime Update.....%d",index);
    if (!schedule_mutex)
    {
        //CCHttpPacket& heartBeatPacket = CCHttpAgent::getInstance()->packets[atoi(string(CMD_HEARTBEAT).c_str()) - 1000];
        
        if (true/* && heartBeatPacket.status != 0*/)
        {
            //if (heartBeatPacket.status != 3)
            //{
            //    heartBeatPacket.status = 0;
            //    return;
            //}

            //int currRmb = heartBeatPacket.recvVal["data"]["gold"].asInt();
            //int currShip = heartBeatPacket.recvVal["data"]["ship"].asInt();
            //PlayerModel::getInstance()->cur_time = heartBeatPacket.recvVal["data"]["cur_time"].asInt();
            //PlayerModel::getInstance()->total_board = heartBeatPacket.recvVal["data"]["total_board"].asInt();
            //PlayerModel::getInstance()->total_win = heartBeatPacket.recvVal["data"]["total_win"].asInt();
            //PlayerModel::getInstance()->servVenueStatus = heartBeatPacket.recvVal["data"]["gamelist"].asInt();
            //PlayerModel::getInstance()->exp = heartBeatPacket.recvVal["data"]["exp"].asInt();
            //同步手机时间
            
            //if (currRmb != PlayerModel::getInstance()->rmb ||
            //    currShip != PlayerModel::getInstance()->money)
            {
                //PlayerModel::getInstance()->rmb = currRmb;
                //PlayerModel::getInstance()->money = currShip;
                
                CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFI_UPDATE_RICHINFO_KEY);
            }
            
            schedule_mutex=true;
            //SCHEDULE->setTimeScale(10.0f);
            SCHEDULE->unscheduleSelector(schedule_selector(GlobalSchedule::globalUpdate), this);
            SCHEDULE->scheduleSelector(schedule_selector(GlobalSchedule::globalUpdate),this,10.0f,kRepeatForever,0,false);
        }
        return;
    }else{
        schedule_mutex=false;
        //CCHttpAgent::getInstance()->doHeartBeat();
        SCHEDULE->unscheduleSelector(schedule_selector(GlobalSchedule::globalUpdate), this);
        SCHEDULE->scheduleSelector(schedule_selector(GlobalSchedule::globalUpdate),this,0.1f,kRepeatForever,0,false);
    }
    
}













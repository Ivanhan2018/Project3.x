#include "DataThread.h"
#include "scene_fish_trace.h"
#include "FishManager.h"
#include "UINotificationCenter.h"


USING_NS_CC;

CDataThread g_DataThread;

CDataThread::CDataThread()
{
	m_nLoading = 0;
	m_bRun = false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	InitializeCriticalSection(&m_crsec);
	m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

#else
	m_mtx = PTHREAD_MUTEX_INITIALIZER;
	m_cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_init( &m_mtx , NULL);
	pthread_cond_init( &m_cond , NULL);

#endif	
}

CDataThread::~CDataThread()
{
	m_bRun = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	::CloseHandle(m_hEvent);
	DeleteCriticalSection(&m_crsec);
#else
	pthread_cond_destroy( &m_cond );
	pthread_mutex_destroy( &m_mtx );
#endif
}

void CDataThread::start()
{
	m_nLoading = 0;
	m_bRun = true;
	std::thread cft(&CDataThread::funcThread, this);
	cft.detach();
}

void CDataThread::stop()
{
	m_bRun = false;
}

void CDataThread::funcThread()
{
	m_nLoading = 0;
	BuildSceneKind1Trace(NS_lrbyFish::kResolutionWidth, NS_lrbyFish::kResolutionHeight, false);
	m_nLoading+=2;

	BuildSceneKind2Trace(NS_lrbyFish::kResolutionWidth, NS_lrbyFish::kResolutionHeight, false);
	m_nLoading+=2;

	BuildSceneKind3Trace(NS_lrbyFish::kResolutionWidth, NS_lrbyFish::kResolutionHeight, false);
	m_nLoading+=2;

	BuildSceneKind4Trace(NS_lrbyFish::kResolutionWidth, NS_lrbyFish::kResolutionHeight, false);
	m_nLoading+=2;

	BuildSceneKind5Trace(NS_lrbyFish::kResolutionWidth, NS_lrbyFish::kResolutionHeight, false);
	m_nLoading+=2;

	std::vector<NS_lrbyFish::CMD_S_FishTrace> vecFishTrace;
	// 等待事件触发
	while (m_bRun)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
		WaitForSingleObject(m_hEvent, INFINITE);
		EnterCriticalSection(&m_crsec);
		ResetEvent(m_hEvent);
		
		vecFishTrace.clear();
		// copy data		
		std::copy(m_vecFishTrace.begin(), m_vecFishTrace.end(), std::back_inserter(vecFishTrace));

		m_vecFishTrace.clear();

		LeaveCriticalSection(&m_crsec);		
#else
		struct timespec tv;
		tv.tv_sec = time(0) + 2;
		tv.tv_nsec = 0;	

		//
		pthread_mutex_lock( &m_mtx );
		int ret = pthread_cond_timedwait( &m_cond , &m_mtx ,&tv );
		if (ETIMEDOUT == ret)
		{
			pthread_mutex_unlock( &m_mtx );
			//USleep(1);
			continue;
		}
		else
		{
			// copy data
			vecFishTrace.clear();
			// copy data		
			std::copy(m_vecFishTrace.begin(), m_vecFishTrace.end(), std::back_inserter(vecFishTrace));
			m_vecFishTrace.clear();

			pthread_mutex_unlock( &m_mtx );
		}
#endif
		// 处理数据	// 运算轨迹
		//std::vector<std::int64_t> VecFishPtrValue;
		for (int i = 0; i < vecFishTrace.size(); i++)
		{
			NS_lrbyFish::CMD_S_FishTrace &sft = vecFishTrace[i];
			FishSprite* pFish = FishManager::GetInstance()->createFish(i, sft.fish_kind, sft.fish_id, sft.trace_type, sft.init_pos, sft.init_count);
			if (pFish != nullptr)
			{
				CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_FishTrace,(std::int64_t)pFish, nullptr, 0);
				CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);
				
				//VecFishPtrValue.push_back((std::int64_t)pFish);
			}
		}

		//CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_FishTrace,0, &VecFishPtrValue[0], VecFishPtrValue.size()*sizeof(std::int64_t));
		//CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);
	}
}

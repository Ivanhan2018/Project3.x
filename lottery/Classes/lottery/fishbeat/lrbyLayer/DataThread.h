#ifndef  _DATA_THREAD_H_
#define  _DATA_THREAD_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "CMD_Fish.h"

class CDataThread
{
public:
	CDataThread();
	~CDataThread();

	//线程函数
	void funcThread();

	//启动线程
	void start();

	void stop();

	//加载资源回调函数
	void loadingTextureCallBack(cocos2d::Texture2D* texture){}
public:
	int m_nLoading;
	bool m_bRun;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	HANDLE m_hEvent;
	CRITICAL_SECTION  m_crsec;
#else
	pthread_mutex_t m_mtx;
	pthread_cond_t m_cond;
#endif
	std::vector<NS_lrbyFish::CMD_S_FishTrace> m_vecFishTrace;
};

#endif // _DATA_THREAD_H_


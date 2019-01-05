#include "DDZTimeCheckControl.h"
using namespace std;

static DDZTimeCheckControl *instance = nullptr;

DDZTimeCheckControl::DDZTimeCheckControl()
	:m_pauseTime(0),
	m_backTime(0)
{

}

DDZTimeCheckControl::~DDZTimeCheckControl()
{

}

DDZTimeCheckControl* DDZTimeCheckControl::getInstance()
{
	if(instance == nullptr)
		instance = new DDZTimeCheckControl();  
	return instance;  
}

void  DDZTimeCheckControl::saveEnterBackgroundTime()
{
	struct timeval tv;
	gettimeofday (&tv, nullptr);
	//m_pauseTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	m_pauseTime = tv.tv_sec;
}
void  DDZTimeCheckControl::saveEnterForegroundTime()
{
	struct timeval tv;
	gettimeofday (&tv, nullptr);
	//m_pauseTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	m_backTime = tv.tv_sec;
}

int DDZTimeCheckControl::getPauseTime()
{
	if (m_backTime == 0 || m_pauseTime == 0)
	{
		return 0;
	}
	int temp = m_backTime - m_pauseTime;
	resetTimes();
	return temp;
}

void DDZTimeCheckControl::resetTimes()
{
	m_backTime = 0;
	m_pauseTime = 0;
}


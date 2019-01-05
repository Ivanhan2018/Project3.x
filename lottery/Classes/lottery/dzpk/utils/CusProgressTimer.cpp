#include "CusProgressTimer.h"

CusProgressTimer::CusProgressTimer():
duration(0),
progressSprite(NULL),
offerset_r(0),time(0),
offerset_g(0),
m_percent(100.0),
m_pobj2(NULL)
{
    
}

CusProgressTimer::~CusProgressTimer()
{

}

CusProgressTimer* CusProgressTimer::createWith(const char* progressPic,float duration)
{
    CCSprite* progressSprite = CCSprite::create(progressPic);
    return createWith(progressSprite,duration);
}

CusProgressTimer* CusProgressTimer::createWith(CCSprite* progressSprite,float duration)
{
    CusProgressTimer *pRet = new CusProgressTimer();
	if (pRet)
	{
		pRet->progressSprite = progressSprite;
		pRet->duration = duration;
	}
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool CusProgressTimer::init()
{
    bool bRet = false;
    
	do
	{
		CC_BREAK_IF(!CCNode::create());
        
		this->initUI();
        
		bRet = true;
        
	} while (0);
    
	return bRet;
}

void CusProgressTimer::initUI()
{
    this->setContentSize(progressSprite->getContentSize());
    
    //progressSprite->setColor(ccc3(132,255,0));
    CCProgressTimer* pProgressTimer = CCProgressTimer::create(progressSprite);
    pProgressTimer->setPosition(Vec2::ZERO);
    pProgressTimer->setReverseProgress(true);
    pProgressTimer->setType(kCCProgressTimerTypeRadial);
    pProgressTimer->setPercentage(100);
    this->addChild(pProgressTimer,100,100);
    
	//ccc3(132,255,0)->ccc3(252,255,0)->ccc3(255,138,0)->ccc3(255,0,0)
    offerset_r = (float)(255-132)*3/(duration*10);
    offerset_g = (float)(255-0)*3/(duration*10);
    
    if (int(offerset_r*10)%10>5)
    {
        offerset_r += 1;
    }
    
    if (int(offerset_g*10)%10>5)
    {
        offerset_g += 1;
    }
    
    this->schedule(schedule_selector(CusProgressTimer::updateProgressCallback), 0.1f);
}


void CusProgressTimer::updateProgressCallback(float dt)
{
    time +=dt;
    CCProgressTimer* pProgressTimer = (CCProgressTimer*)this->getChildByTag(100);
    /*
    CCSprite * pProgress = pProgressTimer->getSprite();
    
    ccColor3B color = pProgress->getColor();
    float rgb_r = color.r;
    float rgb_g = color.g;

    if ( rgb_r< 255.0)
    {
        rgb_r += offerset_r;
        
        if (rgb_r > 255.0)
        {
            rgb_r = 255.0;
        }
    }
    else if(rgb_g > 0)
    {
        rgb_g -= offerset_g;
        if (rgb_g < 0 )
        {
            rgb_g = 0;
        }
    }else{
        rgb_r=255.0;
        rgb_g=0;
    }
    pProgress->setColor(ccc3(rgb_r,rgb_g,0));
   */ 
    if (pProgressTimer->getPercentage()== 0)
    {
        this->unschedule(schedule_selector(CusProgressTimer::updateProgressCallback));
        if (m_pobj && m_pfnCallFunc)
        {
            (m_pobj->*m_pfnCallFunc)(3);
        }
    }
  
    if (m_left_time-time<=5)
    {
        if (m_pobj2 && m_pfnCallFunc2)
        {
            (m_pobj2->*m_pfnCallFunc2)(duration-time);
        }
        //m_pobj2=NULL;
        //m_pfnCallFunc2=NULL;
    }
    
    if (m_left_time-time<=5)
    {
        if (m_pobj3 && m_pfnCallFunc3)
        {
            (m_pobj3->*m_pfnCallFunc3)(duration-time);
        }
        m_pobj3=NULL;
        m_pfnCallFunc3=NULL;
    }
  
}
/*
void CusProgressTimer::updatePointPos()
{
    
}
*/
void CusProgressTimer::setSelector(CCObject* obj, SEL_COMMON selector)
{
    m_pobj = obj;
    m_pfnCallFunc = selector;
}

void CusProgressTimer::setSelector2(CCObject* obj, SEL_COMMON selector)
{
    m_pobj2 = obj;
    m_pfnCallFunc2 = selector;
}

void CusProgressTimer::setSelector3(CCObject* obj, SEL_COMMON selector)
{
    m_pobj3 = obj;
    m_pfnCallFunc3 = selector;
}

void CusProgressTimer::setLeftTime(float times)
{
    m_left_time = times;
    updateProgress();
}

void CusProgressTimer::updateProgress()
{
    CCProgressTimer* pProgressTimer = (CCProgressTimer*)this->getChildByTag(100);
    //CCSprite * pProgress = pProgressTimer->getSprite();
    
    //float rgb_r = 132.0;
    //float rgb_g = 255.0;
    
    m_percent = m_left_time/duration*100;

    CCProgressFromTo* pFromTo = CCProgressFromTo::create(m_left_time,m_percent,0);
    pProgressTimer->stopAllActions();
    pProgressTimer->runAction(pFromTo);
   /*
    if (m_left_time > 2*duration/3)
    {
        rgb_r += ((255-132)*3/duration)*(duration-m_left_time);
        rgb_g=255.0;
    }else if (m_left_time > duration/3){
        rgb_r = 255;
        rgb_g -= (255*3/duration)*(2*duration/3-m_left_time);
    }else
    {
        rgb_r = 255;
        rgb_g = 0 ;
    }
    
    pProgress->setColor(ccc3(rgb_r,rgb_g,0));
    */
}







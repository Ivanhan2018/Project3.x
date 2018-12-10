#include "AnimationWidget.h"
#include "utility.h"
#include "WidgetFun.h"
USING_NS_CC;

AnimationWidget::AnimationWidget()
	:m_fFrameTime(1/60.f)
	,m_bIsLoop(true)
	,m_fActTime(0)
	,m_iActIdex(0)
	,m_fDelayTime(0)
	,m_fPassTime(0)
{
}

AnimationWidget::~AnimationWidget()
{
}

bool AnimationWidget::init()
{
	if (!MaskSprite::init())
	{
		return false;
	}
	scheduleUpdate();
	return true;
} 
void AnimationWidget::update(float delta)
{
	m_fPassTime += delta;
	if (m_fPassTime<m_fDelayTime)
	{
		setVisible(false);
		return;
	}
	m_fActTime += delta;
	if (m_fActTime < m_fFrameTime)
	{
		return;
	}
	if (getFrameCout() == 0)
	{
		return;
	}
	setVisible(true);
	m_fActTime = 0;
	m_iActIdex++;
	if (m_iActIdex >= getFrameCout()&& !m_bIsLoop)
	{
		if (m_kEndCallFun)
		{
			m_kEndCallFun();
		}
		setVisible(false);
		return;
	}
	if (m_iActIdex >= getFrameCout())
	{
		if (m_kEndCallFun)
		{
			m_kEndCallFun();
		}
		m_iActIdex = 0;
	}
	setActFrame(m_iActIdex);
}
AnimationWidget* AnimationWidget::createAnimation( )
{
	AnimationWidget* pAniWidget = new AnimationWidget();
	pAniWidget->autorelease();
	pAniWidget->init();
	return pAniWidget;
}
void AnimationWidget::addPlist(std::string kPlist,std::string kTexture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kPlist.c_str(),kTexture.c_str());
}
void AnimationWidget::setActFrame(int iIdex)
{
	m_iActIdex = iIdex;
	if (m_iActIdex < (int)m_kFrames.size())
	{
		setSpriteFrame(m_kFrames[m_iActIdex]);
	}
	else if (m_iActIdex < (int)m_kTextures.size())
	{
		setTexture(m_kTextures[m_iActIdex]);
	}
	else
	{
		CCAssert(false,"");
	}
}
int AnimationWidget::getFrameCout()
{
	if (m_kFrames.size() > 0)
	{
		return (int)m_kFrames.size();
	}
	return (int)m_kTextures.size();
}
std::string AnimationWidget::ToIdexString(int iIdex)
{
	CCAssert(iIdex < 100,"");
	if (iIdex < 10)
	{
		return utility::toString("0",iIdex);
	}
	return utility::toString(iIdex);
}
void AnimationWidget::getPlistName(std::string kFrames,std::vector<std::string>& kListName,float& fFrameTime)
{
	fFrameTime = 1;
	std::vector<std::string> kSplit = utility::split(kFrames,":");
	if (kSplit.size() == 4 || kSplit.size() == 5)
	{
		int iStart = utility::parseInt(kSplit[1]);
		int iDest = utility::parseInt(kSplit[2]);
		for (int i = iStart;i<=iDest;i++)
		{
			std::string kName = utility::toString(kSplit[0],i,kSplit[3]);
			kListName.push_back(kName);
		}
	}
	if (kSplit.size() == 5 && kListName.size()!= 0)
	{
		fFrameTime = utility::parseFloat(kSplit[4]);
	}
}
void AnimationWidget::getPlistName(std::string kFrames,std::vector<cocos2d::Vec2>& kOffectPos,float& fFrameTime)
{
	std::vector<std::string> kListName;
	getPlistName(kFrames,kListName,fFrameTime);
	if (kListName.size() == 0)
	{
		return;
	}
	Vec2 kFirstOffect = SpriteFrameCache::getInstance()->getSpriteFrameByName(kListName[0].c_str())->getOffset();
	for (int i = 0;i<(int)kListName.size();i++)
	{
		SpriteFrame* frame  = SpriteFrameCache::getInstance()->getSpriteFrameByName(kListName[i].c_str());
		kOffectPos.push_back(frame->getOffset() - kFirstOffect);
	}
}
void AnimationWidget::runFrames(std::string kFrames,float fTime)
{
	if (kFrames == "")
	{
		return;
	}
	m_kActRunAnimName = kFrames;
	m_kTextures.clear();
	m_kFrames.clear();
	std::vector<std::string> kList;
	float fTotalTime = 0;
	getPlistName(kFrames,kList,fTotalTime);
	if (kList.size() == 0)
	{
		setTexture(kFrames);
		return;
	}
	for (int i = 0;i<(int)kList.size();i++)
	{
		addFrame(kList[i]);
	}
	CCAssert(m_kFrames.size() != 0 || m_kTextures.size() != 0,"");
	setActFrame(0);
	setVisible(true);
	if (fTime > 0)
	{
		fTotalTime = fTime;
		setIntervalTime(fTotalTime/(float)kList.size());
	}
}

void AnimationWidget::addFrame( std::string kFrameName )
{
	CCAssert(!kFrameName.empty(),"frame name error");
	SpriteFrame* frame  = SpriteFrameCache::getInstance()->getSpriteFrameByName(kFrameName.c_str());
	if (frame)
	{
		m_kFrames.push_back(frame);
	}
	else
	{
		m_kTextures.push_back(kFrameName);
	}
}

void AnimationWidget::setIntervalTime( float fTime )
{
	CCAssert(fTime>0,"time error");
	m_fFrameTime = fTime;
}
void AnimationWidget::setLoop(bool bValue)
{
	m_bIsLoop = bValue;
}
void AnimationWidget::setEndCallFun(std::function<void()> pFun)
{
	m_kEndCallFun = pFun;
}
int AnimationWidget::getActFrameIdex()
{
	return m_iActIdex;
}

void AnimationWidget::setDelayTime(float fTime)
{
	m_fDelayTime = fTime;
}



#pragma once
#include "OutLineEffect.h"

class AnimationWidget:public MaskSprite
{
public:
	AnimationWidget();
	~AnimationWidget();

	virtual bool init();
	virtual void update(float delta);
	static AnimationWidget* createAnimation();
	static void addPlist(std::string kPlist,std::string kTexture);
	void setActFrame(int iIdex);
	void addFrame(std::string kFrameName);
	int getFrameCout();
	void runFrames(std::string kFrames,float fTime = 0);
	void setIntervalTime(float fTime);
	void setLoop(bool bValue);
	void setEndCallFun(std::function<void()> pFun);
	int getActFrameIdex();

	void setDelayTime(float fTime);
public:
	static std::string ToIdexString(int iIdex);
	static void getPlistName(std::string kFrames,std::vector<std::string>& kListName,float& fFrameTime);
	static void getPlistName(std::string kFrames,std::vector<cocos2d::Vec2>& kOffectPos,float& fFrameTime);
private:
	float							m_fFrameTime;
	float							m_fActTime;
	std::string						m_kActRunAnimName;
	bool							m_bIsLoop;
	int								m_iActIdex;
	std::vector<cocos2d::SpriteFrame*>	  m_kFrames;
	std::vector<std::string>		m_kTextures;
	std::function<void()>			m_kEndCallFun;
	float                        m_fDelayTime;
	float                        m_fPassTime;
};
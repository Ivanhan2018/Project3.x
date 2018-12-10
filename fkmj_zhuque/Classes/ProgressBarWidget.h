#pragma once
#include "cocos2d.h"

class ProgressBarWidget:public cocos2d::Node
{
public:
	ProgressBarWidget(std::string kBackImg,std::string kBarImg);
	~ProgressBarWidget();

	bool init();
	void setProgressWithData(float fCurrent, float fMax);
	void setFontSize(float fFontSize);
	void setTextColor(const cocos2d::Color3B& kColor);
private:
	cocos2d::Sprite*     m_pBgImage;
	cocos2d::Sprite*     m_pBarImage;
	cocos2d::LabelTTF*   m_pText;
};
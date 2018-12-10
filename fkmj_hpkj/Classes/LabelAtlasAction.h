#pragma once

#include "cocos2d.h"

class LabelAtlasAction
	: public cocos2d::LabelAtlas
{
public:
	LabelAtlasAction();

	static LabelAtlasAction* create(const std::string& string, const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap);
public:
	virtual void setString(const std::string &label) override;
public:
	void setStringAction(int iValue);
	void setIsAction(bool bAction);
	void setIsInShaortTxt(bool bAction);
public:
	void OnEffect(int iIdex,int iCharIdex,int iMaxCout,float fAddScale = 0.f);
	std::string getShortGoldValue(int iGold);
	void EndAction();
	void InShortTxt();
private:
	float	m_fVaildTime[20];
	int		m_iActNum;
	bool	m_bAction;
	bool	m_bInShaortTxt;
};
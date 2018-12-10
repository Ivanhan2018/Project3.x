#include "LabelAtlasAction.h"
#include "ActionEx.h"
#include "utility.h"

USING_NS_CC;

LabelAtlasAction::LabelAtlasAction()
	:m_iActNum(0)
	,m_bAction(true)
	,m_bInShaortTxt(false)
{
	memset(m_fVaildTime,0,sizeof(float)*20);
}
LabelAtlasAction* LabelAtlasAction::create(const std::string& string, const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap)
{
	LabelAtlasAction* ret = new (std::nothrow) LabelAtlasAction();
	if(ret && ret->initWithString(string, charMapFile, itemWidth, itemHeight, startCharMap))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
void LabelAtlasAction::setString(const std::string &label)
{
	std::string kDestStr = label;
	std::string kStr = getString();
	for (int i = (int)kDestStr.size() - 1; i>=0 ;i--)
	{
		if (kDestStr[i] == '-')
		{
			kDestStr[i] = '9'+4;
		}
		if (kDestStr[i] == '+')
		{
			kDestStr[i] = '9'+5;
		}
	}
	LabelAtlas::setString(kDestStr);
	if (!m_bAction)
	{
		return;
	}
	for (int i = (int)kDestStr.size() - 1; i>=0 ;i--)
	{
		if (kDestStr[i] > '9')
		{
			continue;
		}
		if (kDestStr[i] < '0')
		{
			continue;
		}
		bool bChange = false;
		if ((int)kStr.size() <= i || kStr[i] != kDestStr[i])
		{
			bChange = true;
		}
		float fPastTime = cocos2d::Director::getInstance()->getDeltaTime();
		if (fPastTime > 1)
		{
			fPastTime = 0.018f;
		}
		m_fVaildTime[i] -= fPastTime;
		if (m_fVaildTime[i] > 0)
		{
			continue;
		}
		if (i >= 20)
		{
			CCASSERT(false,"");
			continue;
		}
		if (bChange)
		{
			m_fVaildTime[i] = 0.15f+0.02f*i;
			OnEffect(i,kDestStr[i]-_mapStartChar,(int)kDestStr.size());
		}
	}
}
void LabelAtlasAction::setStringAction(int iValue)
{
	if (m_iActNum == iValue)
	{
		return;
	}
	
	bool bIsVisible = true;
	cocos2d::Node* pParent = getParent();
	while (pParent)
	{
		if (!pParent->isVisible())
		{
			bIsVisible = false;
			break;
		}
		pParent = pParent->getParent();
	}
	if (bIsVisible && m_bAction)
	{
		stopAllActions();
		memset(m_fVaildTime,0,sizeof(float)*20);
		FiniteTimeAction*  action1 = cocos2d::CCSequence::create(
			cocos2d::MoveExTxtTime::create(2.0f,m_iActNum,iValue),
			cocos2d::CallFunc::create(this,callfunc_selector(LabelAtlasAction::EndAction)),
			cocos2d::DelayTime::create(1.0f),
			cocos2d::CallFunc::create(this,callfunc_selector(LabelAtlasAction::InShortTxt)),
			NULL);
		runAction(action1);
		m_iActNum = iValue;
	}
	else
	{
		m_iActNum = iValue;
		InShortTxt();
	}
}
void LabelAtlasAction::setIsAction(bool bAction)
{
	m_bAction = bAction;
}
void LabelAtlasAction::setIsInShaortTxt(bool bAction)
{
	m_bAction = bAction;
}
void LabelAtlasAction::EndAction()
{
	std::string kStr = getString();
	for (int i = (int)kStr.size() - 1; i>=0 ;i--)
	{
		OnEffect(i,kStr[i]-_mapStartChar,(int)kStr.size(),1.f);
	}
}
std::string LabelAtlasAction::getShortGoldValue(int iGold)
{
	std::string kFuHao = "";
	if (iGold < 0)
	{
		iGold = -iGold;
		kFuHao = '9'+4;
	}
	std::string kTempTxt;
	std::string kGoldStr = utility::toString(iGold);
	int iCout = 0;
	int iTempGold = iGold;
	int iLastValue = 0;
	while (iTempGold >= 10)
	{
		iTempGold = iTempGold/10;
		iCout ++;
	}
	if (iCout > 7)
	{
		iTempGold = iGold/100000000;
		iLastValue = (iGold/10000)%10000;
		kTempTxt = '9'+2;
	}
	else if (iCout > 3)
	{
		iTempGold = iGold/10000;
		iLastValue = iGold%10000;
		kTempTxt = '9'+1;
	}
	else
	{
		return utility::toString(kFuHao,iGold);
	}
	std::string kTxt1 = utility::toString(iTempGold);
	std::string kTxt2 = utility::toString(iLastValue);
	if (iLastValue == 0)
	{
		kTxt1.push_back('9'+3);
		kTxt1.push_back('0');
	}
	else
	{
		int iSize = kTxt2.size();
		for (int i = 0;i<4-iSize;i++)
		{
			kTxt2 = utility::toString("0",kTxt2);
		}
		int iTempCout1 = kTxt1.size();
		if (iTempCout1 < 4)
		{
			kTxt1.push_back('.');
			for (int i = 0;i<4-iTempCout1 && i < (int)kTxt2.size();i++)
			{
				kTxt1.push_back(kTxt2[i]);
			}
		}
	}
	return utility::toString(kFuHao,kTxt1,kTempTxt);
}
void LabelAtlasAction::InShortTxt()
{
	if (m_bInShaortTxt)
	{
		LabelAtlas::setString(getShortGoldValue(m_iActNum));
	}
	else
	{
		LabelAtlas::setString(utility::toString(m_iActNum));
	}
}
void LabelAtlasAction::OnEffect(int iIdex,int iCharIdex,int iMaxCout,float fAddScale)
{
	int iPosX = _itemWidth*iIdex + _itemWidth/2;
	int iPosY = _itemHeight/2;
	cocos2d::Rect kRect;
	kRect.origin = cocos2d::Vec2(_itemWidth*iCharIdex,0);
	kRect.size = cocos2d::Size(_itemWidth,_itemHeight);
	Sprite* pSprite = Sprite::createWithTexture(getTexture(),kRect);
	pSprite->setScale(1.0f);
	pSprite->setPosition(cocos2d::Vec2(iPosX,iPosY));
	addChild(pSprite,iMaxCout-iIdex);
	float fTime = 0.3f+(iMaxCout-iIdex)*0.1f;
	pSprite->setOpacity(150+fAddScale*100);
	pSprite->runAction(cocos2d::ScaleTo::create(fTime,2.5f+(iMaxCout-iIdex)*0.05f+fAddScale/2.0f));
	CCFiniteTimeAction*  action = cocos2d::CCSequence::create(
		cocos2d::FadeOut::create(fTime),
		cocos2d::RemoveSelf::create(),
		NULL);
	pSprite->runAction(action);
}
#include "WidgetSkinXMLparse.h"
#include "utility.h"
#include "ScriptData.h"
#include "utility.h"
#include "WidgetManager.h"
#include "WidgetFun.h"

USING_NS_CC;

WidgetSkinXMLparse::WidgetSkinXMLparse(  std::string xmlFileName)
	:m_pActionInfos(NULL)
{
	SAXParser parser;
	parser.setDelegator(this);
	std::string kFullPath = utility::FullPath(xmlFileName);
	parser.parse(kFullPath.c_str());
}
void WidgetSkinXMLparse::parseAction(std::string& elementName,std::map<std::string, std::string>& attributeDict)
{
	QYActionInfo* pTempActionInfo = NULL;
	if (elementName == QYActionInfo::getType())
	{
		QYActionInfo* pAction = new QYActionInfo;

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionWaiteTime::getType())
	{
		QYActionWaiteTime* pAction = new QYActionWaiteTime;
		pAction->fWaiteTime = utility::valueForRandFloat("Time",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetPos::getType())
	{
		QYActionSetPos* pAction = new QYActionSetPos;
		pAction->kDestPos = utility::valueForRandPos("Pos",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionRemoveSelf::getType())
	{
		QYActionRemoveSelf* pAction = new QYActionRemoveSelf;
		pAction->iParentTag = utility::valueForInt("ParentTag",attributeDict);
		
		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetVisible::getType())
	{
		QYActionSetVisible* pAction = new QYActionSetVisible;
		pAction->bVisible = utility::valueForBool("Visible",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionStopAllAction::getType())
	{
		QYActionStopAllAction* pAction = new QYActionStopAllAction;

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetOder::getType())
	{
		QYActionSetOder* pAction = new QYActionSetOder;
		pAction->iOder = utility::valueForInt("Oder",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSplineTo::getType())
	{
		QYActionSplineTo* pAction = new QYActionSplineTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->fTension = utility::valueForRandFloat("Tension",attributeDict);
		int iIdex = 0;
		while (utility::valueForKey(utility::toString("Pos",iIdex).c_str(),attributeDict) != "")
		{
			RandPos kPos = utility::valueForRandPos(utility::toString("Pos",iIdex).c_str(),attributeDict);
			pAction->kPosition.push_back(kPos);
			iIdex++;
		}
		CCASSERT(iIdex != 0,"");
		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionBizerTo::getType())
	{
		QYActionBizerTo* pAction = new QYActionBizerTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->kPos1 = utility::valueForRandPos("Pos1",attributeDict);
		pAction->kDestPos = utility::valueForRandPos("DestPos",attributeDict);
		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionBizerBy::getType())
	{
		QYActionBizerBy* pAction = new QYActionBizerBy;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->kPos1 = utility::valueForRandPos("Pos1",attributeDict);
		pAction->kDestPos = utility::valueForRandPos("DestPos",attributeDict);
		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionMoveTo::getType())
	{
		QYActionMoveTo* pAction = new QYActionMoveTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->kDestPos = utility::valueForRandPos("Pos",attributeDict);
		pAction->bDestX = utility::valueForBool("DestX",attributeDict);
		pAction->bDestY = utility::valueForBool("DestY",attributeDict);
		
		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionMoveBy::getType())
	{
		QYActionMoveBy* pAction = new QYActionMoveBy;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->kDestPos = utility::valueForRandPos("Pos",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionMoveToBySpd::getType())
	{
		QYActionMoveToBySpd* pAction = new QYActionMoveToBySpd;
		pAction->fMoveSpd = utility::valueForRandFloat("MoveSpd",attributeDict);
		pAction->kDestPos = utility::valueForRandPos("Pos",attributeDict);
		pAction->bDestX = utility::valueForBool("DestX",attributeDict);
		pAction->bDestY = utility::valueForBool("DestY",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionGravity::getType())
	{
		QYActionGravity* pAction = new QYActionGravity;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->fGravity = utility::valueForRandFloat("Gravity",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetScale::getType())
	{
		QYActionSetScale* pAction = new QYActionSetScale;
		pAction->fDestScale = utility::valueForRandFloat("Scale",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionScaleTo::getType())
	{
		QYActionScaleTo* pAction = new QYActionScaleTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->fDestScale = utility::valueForRandFloat("Scale",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionScaleXTo::getType())
	{
		QYActionScaleXTo* pAction = new QYActionScaleXTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->fDestScaleX = utility::valueForRandFloat("ScaleX",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionScaleYTo::getType())
	{
		QYActionScaleYTo* pAction = new QYActionScaleYTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->fDestScaleY = utility::valueForRandFloat("ScaleY",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetProgress::getType())
	{
		QYActionSetProgress* pAction = new QYActionSetProgress;
		pAction->fPercent = utility::valueForRandFloat("Percent",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionProgressTo::getType())
	{
		QYActionProgressTo* pAction = new QYActionProgressTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->fPercent = utility::valueForRandFloat("Percent",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetSize::getType())
	{
		QYActionSetSize* pAction = new QYActionSetSize;
		pAction->kDestSize = utility::valueForRandPos("Size",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSizeTo::getType())
	{
		QYActionSizeTo* pAction = new QYActionSizeTo;
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		pAction->kDestSize = utility::valueForRandPos("Size",attributeDict);

		pTempActionInfo = pAction;
	}

	else if (elementName == QYActionSetAple::getType())
	{
		QYActionSetAple* pAction = new QYActionSetAple;
		pAction->fDestAple = utility::valueForRandFloat("Aple",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionApleTo::getType())
	{
		QYActionApleTo* pAction = new QYActionApleTo;
		pAction->fDestAple = utility::valueForRandFloat("Aple",attributeDict);
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);
		
		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetRota::getType())
	{
		QYActionSetRota* pAction = new QYActionSetRota;
		pAction->fDestRota = utility::valueForRandFloat("Rota",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionRotaTo::getType())
	{
		QYActionRotaTo* pAction = new QYActionRotaTo;
		pAction->fDestRota = utility::valueForRandFloat("Rota",attributeDict);
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionRotaBy::getType())
	{
		QYActionRotaBy* pAction = new QYActionRotaBy;
		pAction->fDestRota = utility::valueForRandFloat("Rota",attributeDict);
		pAction->fTime = utility::valueForRandFloat("Time",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionCreateWidget::getType())
	{
		QYActionCreateWidget* pAction = new QYActionCreateWidget;
		pAction->kWidgetName = utility::valueForKey("WidgetName",attributeDict);
		pAction->bRootNode = utility::valueForBool("RootNode",attributeDict);
		pAction->kOffectPos = utility::valueForPoint("OffectPos",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionRunAnim::getType())
	{
		QYActionRunAnim* pAction = new QYActionRunAnim;
		pAction->kAnimName = utility::valueForKey("AnimName",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionPlaySound::getType())
	{
		QYActionPlaySound* pAction = new QYActionPlaySound;
		pAction->kSoundName = utility::valueForKey("SoundName",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetTxt::getType())
	{
		QYActionSetTxt* pAction = new QYActionSetTxt;
		pAction->kTxt = utility::valueForKey("Txt",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionCallAction::getType())
	{
		QYActionCallAction* pAction = new QYActionCallAction;
		pAction->kOtherNodeName = utility::valueForKey("OtherNodeName",attributeDict);
		pAction->kOtherActionName = utility::valueForKey("OtherActionName",attributeDict);
		pAction->kSelfActionName = utility::valueForKey("SelfActionName",attributeDict);
		pAction->kActionParentName = utility::valueForKey("ActionParentName",attributeDict);
		
		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionCallButton::getType())
	{
		QYActionCallButton* pAction = new QYActionCallButton;
		pAction->kButtonName = utility::valueForKey("ButtonName",attributeDict);

		pTempActionInfo = pAction;
	}
	else if (elementName == QYActionSetImage::getType())
	{
		QYActionSetImage* pAction = new QYActionSetImage;
		pAction->kImageFile = utility::valueForUserString("ImageFile",attributeDict);
		pTempActionInfo = pAction;
	}

	if (pTempActionInfo)
	{
		pTempActionInfo->kType = elementName;
		pTempActionInfo->kName = utility::valueForKey("Name",attributeDict);
		pTempActionInfo->bSaveAction = utility::valueForBool("SaveAction",attributeDict);
		pTempActionInfo->bRepeat = utility::valueForBool("Repeat",attributeDict);
		if (m_pActionInfos.size())
		{
			pTempActionInfo->pParent = m_pActionInfos.back();
			m_pActionInfos.back()->kNextAction.push_back(pTempActionInfo);
		}
		else
		{
			m_pWidgetInfos.back()->kQYAction.push_back(pTempActionInfo);
		}
		m_pActionInfos.push_back(pTempActionInfo);
	}
	
}
void WidgetSkinXMLparse::parseSkin(std::string& elementName,std::map<std::string, std::string>& attributeDict)
{
	WidgetInfo* pTempWidgetInfo = NULL;

	if (elementName == WidgetImagic::getType())
	{
		WidgetImagic* pImagic = new WidgetImagic;
		pImagic->kTexture = WidgetFun::paseTextureInfo(utility::valueForKey("TextureInfo",attributeDict));
		pImagic->bUsePixWidth = utility::valueForBool("UsePixWidth",attributeDict);
		
		pTempWidgetInfo = pImagic;
	}
	else if (elementName == WidgetNode::getType())
	{
		WidgetNode* pNode = new WidgetNode;
		pTempWidgetInfo = pNode;
	}
	else if (elementName == WidgetNodeUI::getType())
	{
		WidgetNodeUI* pNode = new WidgetNodeUI;

		pTempWidgetInfo = pNode;
	}
	else if (elementName == WidgetTxt::getType())
	{
		WidgetTxt* pTxt = new WidgetTxt;
		pTxt->kShowTxt = utility::valueForKey("ShowTxt",attributeDict);
		pTxt->kFontName = utility::valueForKey("FontName",attributeDict);
		pTxt->kFontSize = utility::valueForInt("FontSize",attributeDict);
		pTempWidgetInfo = pTxt;
	}
	else if (elementName == WidgetColorTxt::getType())
	{
		WidgetColorTxt* pTxt = new WidgetColorTxt;
		pTxt->kFontName = utility::valueForKey("FontName",attributeDict);
		pTxt->kFontSize = utility::valueForInt("FontSize",attributeDict);
		pTxt->kDimensions = utility::valueForSize("Dimensions",attributeDict);
		pTempWidgetInfo = pTxt;
	}
	else if (elementName == WidgetTxtAtlas::getType())
	{
		WidgetTxtAtlas* pAtlas = new WidgetTxtAtlas;
		pAtlas->kShowTxt = utility::valueForKey("ShowTxt",attributeDict);
		pAtlas->kImagic = utility::valueForKey("Texture",attributeDict);
		pAtlas->iWidth = utility::valueForInt("Width",attributeDict);
		pAtlas->iHeight = utility::valueForInt("Height",attributeDict);
		pAtlas->iStartChar = utility::valueForInt("StartChar",attributeDict);
		pAtlas->bActionNum = utility::valueForBool("ActionNum",attributeDict);

		pTempWidgetInfo = pAtlas;
	}
	else if (elementName == WidgetEdit::getType())
	{
		WidgetEdit* pEdit = new WidgetEdit;
		pEdit->kBackTexture = utility::valueForKey("BackTexture",attributeDict);
		pEdit->kSize = utility::valueForSize("Size",attributeDict);
		pEdit->kShowTxt = utility::valueForKey("ShowTxt",attributeDict);
		pEdit->kFontName = utility::valueForKey("FontName",attributeDict);
		pEdit->kFontSize = utility::valueForInt("FontSize",attributeDict);

		pTempWidgetInfo = pEdit;
	}
	else if (elementName==WidgetButton::getType())
	{
		WidgetButton* pButton = new WidgetButton;
		pButton->kNomalTexture = utility::valueForKey("NomalTexture",attributeDict);
		pButton->kPressTexture = utility::valueForKey("PressTexture",attributeDict);
		pButton->kDisableTexture = utility::valueForKey("DisableTexture",attributeDict);
		pButton->bUseViewMenu = utility::valueForBool("UseViewMenu",attributeDict);
		pButton->kShowTxt = utility::valueForKey("ShowTxt",attributeDict);
		pButton->kFontName = utility::valueForKey("FontName",attributeDict);
		pButton->kFontSize = utility::valueForInt("FontSize",attributeDict);
		pTempWidgetInfo = pButton;
	}
	else if (elementName==WidgetCheckBox::getType())
	{
		WidgetCheckBox* pCheck = new WidgetCheckBox;
		pCheck->kUnSelectTexture = utility::valueForKey("UnSelectTexture",attributeDict);
		pCheck->kSelectTexture = utility::valueForKey("SelectTexture",attributeDict);
		pTempWidgetInfo = pCheck;
	}
	else if (elementName==WidgetSelectBox::getType())
	{
		WidgetSelectBox* pSelectBox = new WidgetSelectBox;
		pSelectBox->kUnSelectTexture = utility::valueForKey("UnSelectTexture",attributeDict);
		pSelectBox->kSelectTexture = utility::valueForKey("SelectTexture",attributeDict);
		pTempWidgetInfo = pSelectBox;
	}
	else if (elementName==WidgetProgressBar::getType())
	{
		WidgetProgressBar* pProgress = new WidgetProgressBar;
		pProgress->kBackTexture =  utility::valueForKey("BackTexture",attributeDict);
		pProgress->kBarTexture =  utility::valueForKey("BarTexture",attributeDict);
		pProgress->kShowTxt = utility::valueForKey("ShowTxt",attributeDict);
		pProgress->kFontName = utility::valueForKey("FontName",attributeDict);
		pProgress->kFontSize = utility::valueForInt("FontSize",attributeDict);
		pTempWidgetInfo = pProgress;
	}
	else if (elementName==WidgetProgressTimer::getType())
	{
		WidgetProgressTimer* pProgress = new WidgetProgressTimer;
		pProgress->kTexture =  utility::valueForKey("Texture",attributeDict);
		pTempWidgetInfo = pProgress;
	}
	else if (elementName==WidgetSider::getType())
	{
		WidgetSider* pButton = new WidgetSider;
		pButton->kNomalTexture = utility::valueForKey("NomalTexture",attributeDict);
		pButton->kPressTexture = utility::valueForKey("PressTexture",attributeDict);
		pButton->kDisableTexture = utility::valueForKey("DisableTexture",attributeDict);
		pButton->fMaxPosY = utility::valueForFloat("MaxPosY",attributeDict);
		pButton->fMaxValue = utility::valueForFloat("MaxValue",attributeDict);
		pButton->bMoveByX = utility::valueForBool("MoveByX",attributeDict);
		pTempWidgetInfo = pButton;
	}
	else if (elementName==Widget9Sprite::getType())
	{
		Widget9Sprite* p9Sprite = new Widget9Sprite;
		p9Sprite->kBackTexture =  utility::valueForKey("BackTexture",attributeDict);
		p9Sprite->kSize = utility::valueForSize("Size",attributeDict);
		p9Sprite->kInsetsRect = utility::valueForRect("InsetsRect",attributeDict);
		pTempWidgetInfo = p9Sprite;
	}
	else if (elementName==WidgetList::getType())
	{
		WidgetList* pList = new WidgetList;

		pTempWidgetInfo = pList;
	}
	else if (elementName==WidgetRichText::getType())
	{
		WidgetRichText* pRich = new WidgetRichText;

		pTempWidgetInfo = pRich;
	}
	else if (elementName==WidgetAnimation::getType())
	{
		WidgetAnimation* pAni = new WidgetAnimation;
		pAni->kPlistFile = utility::valueForKey("PlistFile",attributeDict);
		pAni->kImgFile = utility::valueForKey("ImgFile",attributeDict);

		pTempWidgetInfo = pAni;
	}
	else if (elementName==WidgetPanle::getType())
	{
		WidgetPanle* pPanle = new WidgetPanle;
		pPanle->kSize = utility::valueForSize("Size",attributeDict);
		pPanle->kColor = utility::parseColour(utility::valueForKey("Color",attributeDict));

		pTempWidgetInfo = pPanle;
	}
	else if (elementName==WidgetSwitch_Button::getType())
	{
		WidgetSwitch_Button* pPanle = new WidgetSwitch_Button;

		pTempWidgetInfo = pPanle;
	}
	else if (elementName==WidgetLayout::getType())
	{
		WidgetLayout* pPanle = new WidgetLayout;

		pTempWidgetInfo = pPanle;
	}
	else if (elementName==WidgetClipper::getType())
	{
		WidgetClipper* pClipper = new WidgetClipper;
		pClipper->kSize = utility::valueForSize("Size",attributeDict);

		pTempWidgetInfo = pClipper;
	}
	
	else if (elementName=="WidgetSkinChild")
	{
		std::string kSkinTempName = utility::valueForKey("SkinTempName",attributeDict);
		WidgetInfo* pWidgetInfo = WidgetManager::Instance().getWidgetInfo(kSkinTempName);
		WidgetInfo* pCopy = pWidgetInfo->copyAll();
		pTempWidgetInfo = pCopy;
		elementName = pCopy->kType;
	}
	else if (elementName == "Property")
	{
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);

		WidgetInfo* pTemp = m_pWidgetInfos.back();
		pTemp->kProperty.push_back(stringKeyValue(kKey,kValue));
	}
	else if (elementName == "UserInfo")
	{
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		WidgetInfo* pTemp = m_pWidgetInfos.back();
		pTemp->kUserInfo.insert(std::make_pair(kKey,kValue));
	}
	else if (elementName == "Action")
	{
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		WidgetInfo* pTemp = m_pWidgetInfos.back();
		pTemp->kAction.push_back(stringKeyValue(kKey,kValue));
	}
	if (pTempWidgetInfo)
	{
		pTempWidgetInfo->kType = elementName;
		pTempWidgetInfo->kName = utility::valueForKey("Name",attributeDict);
		pTempWidgetInfo->iOder = utility::valueForInt("Oder",attributeDict);	
		if (m_pWidgetInfos.size())
		{
			WidgetInfo* pTemp = m_pWidgetInfos.back();
			pTemp->kChild.push_back(pTempWidgetInfo);
		}
		else
		{
			WidgetManager::Instance().addWidgetInfo(pTempWidgetInfo);
		}
		m_pWidgetInfos.push_back(pTempWidgetInfo);
	}

}
void WidgetSkinXMLparse::startElement(void *ctx, const char *name, const char **atts)
{
	std::string elementName = (char*)name;
	std::map<std::string, std::string> attributeDict;
	utility::createAttributeDict(attributeDict,atts);
	parseAction(elementName,attributeDict);
	parseSkin(elementName,attributeDict);
	if(elementName == "LoadPlist")
	{
		std::string kPicName1 = utility::valueForKey("Texture", attributeDict);
		std::string kPlist = utility::valueForKey("Plist", attributeDict);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kPlist.c_str(),kPicName1.c_str());
	}
	if(elementName == "LoadSkin")
	{
		std::string kSkin = utility::valueForKey("Skin", attributeDict);
		WidgetSkinXMLparse kXml(kSkin);
	}
}
void WidgetSkinXMLparse::endElement(void *ctx, const char *name)
{
	std::string elementName = (char*)name;
	if (m_pWidgetInfos.size() && (elementName == m_pWidgetInfos.back()->kType ||
		elementName == m_pWidgetInfos.back()->kType || elementName=="WidgetSkinChild"))
	{
		m_pWidgetInfos.pop_back();
	}
	if (m_pActionInfos.size() && elementName == m_pActionInfos.back()->kType)
	{
		m_pActionInfos.pop_back();
	}
}

void WidgetSkinXMLparse::textHandler(void *ctx, const char *s, int len)
{

}

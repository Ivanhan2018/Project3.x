#include "WidgetManager.h"
#include "ScriptData.h"
#include "utility.h"
#include "WidgetFun.h"
#include "WidgetSkinXMLparse.h"
#include "EditBoxWidget.h"
#include "SliderMenu.h"
#include "ProgressBarWidget.h"
#include "CCScale9Sprite.h"
#include "AnimationWidget.h"
#include "ColorTxt.h"
#include "2d/CCProgressTimer.h"
#include "SoundFun.h"
#include "WindowInfo.h"
#include "SelectBox.h"
#include "ColorTxt.h"
#include "ButtonEx.h"
#include "ListViewEx.h"
#include "LabelAtlasAction.h"
#include "QYLabel.h"

FV_SINGLETON_STORAGE(WidgetManager);

ButtonCallBackList WidgetManager::s_kCBInfoListRelease;
ButtonCallBackList WidgetManager::s_kCBInfoListBegin;

USING_NS_CC;

using namespace cocos2d::ui;

WidgetManager::WidgetManager()
{
	Init();
}
WidgetManager::~WidgetManager(void)
{
}

void WidgetManager::Init()
{
}
void WidgetManager::addWidgetInfo(WidgetInfo* pInfo)
{
	if (pInfo->kName == "")
	{
		return;
	}
	m_kWidgets.insert(std::make_pair(pInfo->kName,pInfo));
}
void WidgetManager::clearWidgetSkin()
{
	WidgetInfoListP::iterator itor = m_kWidgets.begin();
	for (;itor != m_kWidgets.end();itor++)
	{
		clearWidgetSkin(itor->second);
	}
	m_kWidgets.clear();
}
void WidgetManager::clearWidgetSkin(WidgetInfo* pInfo)
{
	for (int i = 0;i<(int)pInfo->kChild.size();i++)
	{
		clearWidgetSkin(pInfo->kChild[i]);
	}
	delete pInfo;
}
void WidgetManager::ButtonAction(Node* pBase,Node* pMenu,std::string kKey,std::string kValue)
{
	if (kKey == "Show")
	{
		WidgetFun::setVisible(pBase,kValue,true);
	}
	if (kKey == "Hide")
	{
		WidgetFun::setVisible(pBase,kValue,false);
	}
	if (kKey == "Disable")
	{
		WidgetFun::setEnable(pBase,kValue,false);
	}
	if (kKey == "Able")
	{
		WidgetFun::setEnable(pBase,kValue,true);
	}
	if (kKey == "CallAction")
	{
		std::vector<std::string> kVector = utility::split(kValue,":");
		if (kVector.size() != 2)
		{
			CCAssert(false,"");
			return;
		}
		WidgetFun::CallAction(pBase,kVector[0],kVector[1]);
	}
	if (kKey == "ActionStart")
	{
		WidgetFun::ActionStart(pBase,kValue);
	}
	if (kKey == "ActionEnd")
	{
		WidgetFun::ActionEnd(pBase,kValue);
	}
	if (kKey == "AutoShow")
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kValue);
		pNode->setVisible(!pNode->isVisible());
	}
	if (kKey == "SetPos")
	{
		std::vector<std::string> kVector = utility::split(kValue,":");
		if (kVector.size() != 2)
		{
			CCAssert(false,"");
			return;
		}
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kVector[0]);
		pNode->setPosition(utility::parsePoint(kVector[1]));
	}
	if (kKey == "Sound")
	{
		if (kValue != "")
		{
			SoundFun::Instance().playEffect(kValue);
		}
	}
	if (kKey == "ButtonProgress")
	{
		SliderMenu* pSlider = WidgetFun::castNode<SliderMenu>(pMenu);
		ProgressBarWidget* pProgressBar = WidgetFun::castNode<ProgressBarWidget>(pMenu->getParent());
		pProgressBar->setProgressWithData(pSlider->getValue(),pSlider->getMaxValue());
	}
	if (kKey == "AutoMove")
	{
		bool bIsMove = utility::parseBool(WidgetFun::getWidgetUserInfo(pMenu,"MoveState"));
		bIsMove = !bIsMove;
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kValue);
		setBakListVisible(pNode,bIsMove,false);
		WidgetFun::setWidgetUserInfo(pMenu,"MoveState",utility::toString(bIsMove));
	}
	if (kKey == "Move")
	{
		std::vector<std::string> kVector = utility::split(kValue,":");
		if (kVector.size() != 3)
		{
			CCAssert(false,"");
			return;
		}
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kVector[0]);
		cocos2d::Vec2 kDestPos = utility::parsePoint(kVector[1]);
		float	fTime = utility::parseFloat(kVector[2]);
		pNode->stopAllActions();
		pNode->runAction(cocos2d::CCMoveTo::create(fTime,kDestPos));
	}
	if (kKey == "StartMove")
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kValue);
		setBakListVisible(pNode,true,false);
	}
	if (kKey == "BackMove")
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kValue);
		setBakListVisible(pNode,false,false);
	}
	if (kKey == "SetOder")
	{
		std::vector<std::string> kSplit = utility::split(kValue,":");
		if (kSplit.size() != 2)
		{
			return;
		}
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kSplit[0]);
		pNode->getParent()->reorderChild(pNode,utility::parseInt(kSplit[1]));
	}
	if (kKey == "SetUserInfo")
	{
		std::vector<std::string> kSplit = utility::split(kValue,":");
		if (kSplit.size() != 3)
		{
			return;
		}
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pBase,kSplit[0]);
		WidgetFun::setWidgetUserInfo(pNode,kSplit[1],kSplit[2]);
	}
}
void WidgetManager::setBakListVisible(cocos2d::Node* pButtonList,bool bVisible,bool bDirect)
{
	cocos2d::Vec2 kOutPoint = utility::parsePoint(WidgetFun::getWidgetUserInfo(pButtonList,"OutPos"));
	cocos2d::Vec2 kInPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pButtonList,"InPos"));
	float fTime = utility::parseFloat(WidgetFun::getWidgetUserInfo(pButtonList,"MoveTime"));
	cocos2d::Vec2 kDest = kInPos;
	if (bVisible)
	{
		kDest = kOutPoint;
	}
	pButtonList->stopAllActions();
	if (bDirect)
	{
		pButtonList->setPosition(kDest);
	}
	else
	{
		ActionInterval*  move1 = CCMoveTo::create(fTime, kDest);
		pButtonList->runAction(move1);
	}
}
cocos2d::Node* WidgetManager::getBaseNode(cocos2d::Node* pNode)
{
	Node* pBase = NULL;
	Node* pParent = pNode;
	while (!pBase)
	{
		if (!pParent)
		{
			break;
		}
		WidgetUserInfo* pTemp = dynamic_cast<WidgetUserInfo*>(pParent->getUserObject());
		if (pTemp && pTemp->bRoot)
		{
			pBase = pParent;
		}
		pParent = pParent->getParent();
	}
	return pBase;
}
void WidgetManager::ButtonAction(Node* pMenu,WidgetUserInfo* pUserInfo,bool bNeedSound)
{
	for (int i = 0;i<(int)s_kCBInfoListRelease.size();i++)
	{
		Button_CBInfo& kInfo = s_kCBInfoListRelease[i];
		if (kInfo.kCommand == pUserInfo->kName)
		{
			(kInfo.pObject->*kInfo.pCallBack)(pMenu,pUserInfo);
		}
	}
	Node* pBase = getBaseNode(pMenu);
	bool bHaveSound = false;
	for (int i = 0;i<(int)pUserInfo->kAction.size();i++)
	{
		stringKeyValue& kAction = pUserInfo->kAction[i];
		ButtonAction(pBase,pMenu,kAction.kKey,kAction.kValue);
		if (kAction.kKey == "Sound")
		{
			bHaveSound = true;
		}
	}
	if (!bHaveSound && bNeedSound)
	{
		SoundFun::Instance().playButtonEffect();
	}
}
void WidgetManager::ButtonRelease( cocos2d::Ref* pObject)
{
	ui::Widget* pButton = dynamic_cast<ui::Widget*>(pObject);
	CCAssert(pButton,"");

	WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pButton->getUserObject());
	CCAssert(pUserInfo,"");

	ButtonAction(pButton,pUserInfo);
}
void WidgetManager::ButtonReleaseEx( cocos2d::Ref* pObject,cocos2d::ui::Widget::TouchEventType _type)
{
	ui::Widget* pButton = dynamic_cast<ui::Widget*>(pObject);
	CCAssert(pButton,"");
	WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pButton->getUserObject());
	CCAssert(pUserInfo,"");
	if (_type== cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		ButtonAction(pButton,pUserInfo);
		return ;
	}
	if (_type== cocos2d::ui::Widget::TouchEventType::BEGAN)
	{
		for (int i = 0;i<(int)s_kCBInfoListBegin.size();i++)
		{
			Button_CBInfo& kInfo = s_kCBInfoListBegin[i];
			if (kInfo.kCommand == pUserInfo->kName)
			{
				(kInfo.pObject->*kInfo.pCallBack)(pButton,pUserInfo);
			}
		}
	}
}
void WidgetManager::CheckButtonReleaseEx( cocos2d::Ref* pObject,cocos2d::ui::CheckBoxEventType _type)
{
	ButtonRelease(pObject);
}
void WidgetManager::SilderValue(cocos2d::Ref* pObject)
{
	SliderMenu* pSlider = dynamic_cast<SliderMenu*>(pObject);
	CCAssert(pSlider,"");
	WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pSlider->getUserObject());
	ButtonCallBackList::iterator itor = s_kCBInfoListRelease.begin();
	for (;itor != s_kCBInfoListRelease.end();itor++)
	{
		Button_CBInfo& kInfo = *itor;
		if (kInfo.kCommand == pUserInfo->kName)
		{
			(kInfo.pObject->*kInfo.pCallBack)(pSlider,pUserInfo);
		}
	}
	ButtonAction(pSlider,pUserInfo);
}
void WidgetManager::addButtonCB(std::string kWidgetName,cocos2d::Ref* pScence,CB_BUTTON_Handler pCallBack)
{
	Button_CBInfo kInfo;
	kInfo.kCommand = kWidgetName;
	kInfo.pCallBack = pCallBack;
	kInfo.pObject = pScence;
	s_kCBInfoListRelease.push_back(kInfo);
}
void WidgetManager::addButtonCBBegin(std::string kWidgetName,cocos2d::Ref* pScence,CB_BUTTON_Handler pCallBack)
{
	Button_CBInfo kInfo;
	kInfo.kCommand = kWidgetName;
	kInfo.pCallBack = pCallBack;
	kInfo.pObject = pScence;
	s_kCBInfoListBegin.push_back(kInfo);
}
void WidgetManager::DelBySecnce(cocos2d::Node* pScence)
{
	ButtonCallBackList::iterator itor = s_kCBInfoListRelease.begin();
	while (itor != s_kCBInfoListRelease.end())
	{
		Button_CBInfo& kInfo = *itor;
		if (kInfo.pObject == pScence)
		{
			itor = s_kCBInfoListRelease.erase(itor);
		}
		else
		{
			itor++;
		}
	}
}
WidgetInfo* WidgetManager::getWidgetInfo(std::string kName,bool bAssert)
{
	WidgetInfoListP::iterator itor = m_kWidgets.find(kName);
	if (itor != m_kWidgets.end())
	{
		return itor->second;
	}
	//if (bAssert)
	{
		std::string kTxt = kName + " can'fin widget";
		CCAssert(false,kTxt.c_str());
	}
	
	return NULL;
}
cocos2d::Node* WidgetManager::createWidget(std::string kName,cocos2d::Node* pRootNode)
{
	WidgetInfo* pWidgetInfo = getWidgetInfo(kName);
	cocos2d::Node* pNode = createWidget(pWidgetInfo,pRootNode);
	return pNode;
}
cocos2d::Node* WidgetManager::createWidget(WidgetInfo* pWidgetInfo,cocos2d::Node* pRootNode)
{

#define CREATE_WIDGET(widgetType)\
	if (pWidgetInfo->kType == Widget##widgetType::getType())\
	{\
		Widget##widgetType* pUserWidgetInfo = WidgetFun::castWidget<Widget##widgetType>(pWidgetInfo);\
		pNode = createWidget##widgetType(pUserWidgetInfo);\
		pRootNode->addChild(pNode,pWidgetInfo->iOder);\
		for (int i = 0;i<(int)pWidgetInfo->kChild.size();i++)\
		{\
			WidgetInfo* pTempInfo = pWidgetInfo->kChild[i];\
			createWidget(pTempInfo,pNode);\
		}\
		changeProperty(pNode,pWidgetInfo->kProperty,&WidgetManager::change##widgetType##Property);\
	}

	if (!pRootNode)
	{
		CCAssert(false,"if (!pRootNode)");
		return NULL;
	}

	CCAssert(pWidgetInfo,"can't find");

	Node* pNode = NULL;
	CREATE_WIDGET(Node);
	CREATE_WIDGET(Imagic);
	CREATE_WIDGET(Txt);
	CREATE_WIDGET(ColorTxt);
	CREATE_WIDGET(Button);
	CREATE_WIDGET(CheckBox);
	CREATE_WIDGET(TxtAtlas);
	CREATE_WIDGET(Edit);
	CREATE_WIDGET(Sider);
	CREATE_WIDGET(ProgressBar);
	CREATE_WIDGET(ProgressTimer);
	CREATE_WIDGET(9Sprite);
	CREATE_WIDGET(List);
	CREATE_WIDGET(Animation);
	CREATE_WIDGET(Panle);
	CREATE_WIDGET(Switch_Button);
	CREATE_WIDGET(SelectBox);
	CREATE_WIDGET(Layout);
	CREATE_WIDGET(NodeUI);
	CREATE_WIDGET(Clipper);
	CREATE_WIDGET(RichText);
	if (!pNode)
	{
		std::string kStr = pWidgetInfo->kName + "can't find";
		CCAssert(false,kStr.c_str());
	}
	else
	{
		CCAssert(pNode->getUserObject() == NULL,"");
		WidgetUserInfo* pUserInfo = new WidgetUserInfo;
		pUserInfo->kName = pWidgetInfo->kName;
		pUserInfo->kType = pWidgetInfo->kType;
		pUserInfo->kUserInfo = pWidgetInfo->kUserInfo;
		pUserInfo->kAction = pWidgetInfo->kAction;
		pUserInfo->pWidgetInfo = pWidgetInfo;
		pUserInfo->kSkinTempName = pWidgetInfo->kName;
		pNode->setUserObject(pUserInfo);
		CreateAction(pNode);
	}
	return pNode;
}

void WidgetManager::changePropertyByType(cocos2d::Node* pNode,std::string kType,std::string kKey,std::string kValue)
{
#define CHANGE_WIDGET_PROPERTY(widgetType)\
	if (kType == Widget##widgetType::getType())\
	{\
	change##widgetType##Property(pNode,kKey,kValue);\
}

	CHANGE_WIDGET_PROPERTY(Node);
	CHANGE_WIDGET_PROPERTY(Imagic);
	CHANGE_WIDGET_PROPERTY(Txt);
	CHANGE_WIDGET_PROPERTY(ColorTxt);
	CHANGE_WIDGET_PROPERTY(TxtAtlas);
	CHANGE_WIDGET_PROPERTY(Edit);
	CHANGE_WIDGET_PROPERTY(Button);
	CHANGE_WIDGET_PROPERTY(CheckBox);
	CHANGE_WIDGET_PROPERTY(Sider);
	CHANGE_WIDGET_PROPERTY(ProgressBar);
	CHANGE_WIDGET_PROPERTY(ProgressTimer);
	CHANGE_WIDGET_PROPERTY(9Sprite);
	CHANGE_WIDGET_PROPERTY(List);
	CHANGE_WIDGET_PROPERTY(Animation);
	CHANGE_WIDGET_PROPERTY(Panle);
	CHANGE_WIDGET_PROPERTY(Switch_Button);
	CHANGE_WIDGET_PROPERTY(SelectBox);
	CHANGE_WIDGET_PROPERTY(Layout);
	CHANGE_WIDGET_PROPERTY(NodeUI);
	CHANGE_WIDGET_PROPERTY(Clipper);
	CHANGE_WIDGET_PROPERTY(RichText);

	changeProperty(pNode,kKey,kValue);
}
cocos2d::Node* WidgetManager::createWidgetNode(WidgetNode* pInfo)
{
	Node* pNode = cocos2d::Node::create();
	return cocos2d::Node::create();
}
void WidgetManager::changeNodeProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{

}
cocos2d::Node* WidgetManager::createWidgetNodeUI(WidgetNodeUI* pInfo)
{
	ui::Widget* pNode = ui::Widget::create();
	return pNode;
}
void WidgetManager::changeNodeUIProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{

}
cocos2d::Node* WidgetManager::createWidgetClipper(WidgetClipper* pInfo)
{
	cocos2d::ClippingNode* pClipper = cocos2d::ClippingNode::create();
	return pClipper;
}
void WidgetManager::changeClipperProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	cocos2d::ClippingNode* pClipper = WidgetFun::castNode<cocos2d::ClippingNode>(pNode);
	if (kKey == "Stencil")
	{
		cocos2d::Node* pTempNode = WidgetFun::getChildWidget(pClipper,kValue);
		pClipper->setStencil(pTempNode);
	}

	if (kKey == "alphaThreshold")
	{
		pClipper->setAlphaThreshold(utility::parseFloat(kValue));
	}
    
	if (kKey == "inverted")
	{
		pClipper->setInverted(utility::parseBool(kValue));
	}
	if (kKey == "ClipperPos")
	{
		pClipper->getStencil()->setPosition(utility::parsePoint(kValue));
	}
}
cocos2d::Node* WidgetManager::createWidgetImagic(WidgetImagic* pInfo)
{
	if (pInfo->bUsePixWidth)
	{
		MaskSprite* pSprite = MaskSprite::create(pInfo->kTexture.kName);
		return pSprite;
	}
	Sprite* pSprite = WidgetFun::createSprite(pInfo->kTexture);
	CCAssert(pSprite && pSprite->getTexture(),"");
	return pSprite;
}
void WidgetManager::changeImagicProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	Sprite* pSprite = WidgetFun::castNode<Sprite>(pNode);

	if (kKey == "FrameTexture")
	{
		pSprite->setSpriteFrame(kValue);
	}
	if (kKey == "ImagicTexture")
	{
		WidgeTextureInfo kTextureInfo = WidgetFun::paseTextureInfo(kValue);
		WidgetFun::setTexture(pSprite,kTextureInfo);
		//utility::saveFilePath(kTextureInfo.kName);
	}
	if (kKey == "Aple")
	{
		float fValue = utility::parseFloat(kValue);
		pSprite->setOpacity(fValue*255);
	}
	if (kKey == "Rect")
	{
		cocos2d::Rect kRect = utility::parseRect(kValue);
		pSprite->setTextureRect(kRect);
	}
	if (kKey == "ImagicSize")
	{
		Size kSize = utility::parseSize(kValue);
		utility::setSpriteScaleBySize(pSprite,kSize);
	}
	if (kKey == "FlipX")
	{
		bool bFlipX = utility::parseBool(kValue);
		pSprite->setFlipX(bFlipX);
	}
	if (kKey == "UV")
	{
		CCTexture2D* pTexture = pSprite->getTexture();
		Texture2D::TexParams kSet;
		kSet.minFilter = GL_LINEAR;
		kSet.magFilter = GL_LINEAR;
		if (kValue == "GL_REPEAT")
		{
			kSet.wrapS = GL_REPEAT;
			kSet.wrapT = GL_REPEAT;
			pTexture->setTexParameters(&kSet);
		}
		if (kValue == "GL_CLAMP_TO_EDGE")
		{
			kSet.wrapS = GL_CLAMP_TO_EDGE;
			kSet.wrapT = GL_CLAMP_TO_EDGE;
			pTexture->setTexParameters(&kSet);
		}
	}
	if (kKey == "FlipY")
	{
		bool bFlipY = utility::parseBool(kValue);
		pSprite->setFlipY(bFlipY);
	}
	if (kKey == "OPacity")
	{
		float fValue = utility::parseFloat(kValue);
		pSprite->setOpacity(fValue);
	}
	if (kKey == "PixWidth")
	{
		MaskSprite* pSprite = WidgetFun::castNode<MaskSprite>(pNode);
		pSprite->setPixWidth(utility::parseFloat(kValue));
	}
	if (kKey == "Color")
	{
		pSprite->setColor(utility::parseColour(kValue));
	}
}

cocos2d::Node* WidgetManager::createWidgetTxt(WidgetTxt* pInfo)
{
	ui::Text* pLable = ui::Text::create(
		pInfo->kShowTxt.c_str(),"",pInfo->kFontSize);
	CCAssert(pLable,"");
	return pLable;
}
void WidgetManager::changeTxtProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	ui::Text* pLable = WidgetFun::castNode<ui::Text>(pNode);
	if (kKey == "AnchorPoint")
	{
		Point kPoint = utility::parsePoint(kValue);
		pLable->setAnchorPoint(kPoint);
	}
	if (kKey == "IsLeft" && utility::parseBool(kValue))
	{
		pLable->setTextHorizontalAlignment(TextHAlignment::LEFT);
	}
	if (kKey == "Dimensions")
	{
		cocos2d::Size kSize = utility::parseSize(kValue);
		pLable->setTextAreaSize(kSize);
	}
	if (kKey =="ShowTxt")
	{
		pLable->setString(kValue.c_str());
	}
	if (kKey == "Aple")
	{
		float fValue = utility::parseFloat(kValue);
		pLable->setOpacity(fValue*255);
	}
	else if(kKey=="FontName")
	{
		pLable->setFontName(kValue.c_str());
	}
	else if(kKey=="FontSize")
	{
		pLable->setFontSize(utility::parseInt(kValue));
	}
	else if(kKey=="Color")
	{
		cocos2d::Color4B kColor(utility::parseColour(kValue));

		pLable->setTextColor(kColor);
	}
	else if("OutLine" == kKey)
	{
		int iWidth = 2;
		Color4B kColor;
		std::vector<std::string> kStr = utility::split(kValue,":");
		if (kStr.size() == 2)
		{
			iWidth = utility::parseInt(kStr[1]);
			kColor = utility::parseColour4(kStr[0]);
		}
		if (kStr.size() == 1)
		{
			kColor = utility::parseColour4(kStr[0]);
		}
		pLable->enableOutline(kColor,iWidth);
	}
}

cocos2d::Node* WidgetManager::createWidgetColorTxt(WidgetColorTxt* pInfo)
{
	cocos2d::ColorTxt*  pColorTxt = cocos2d::ColorTxt::create();
	return pColorTxt;
}

void WidgetManager::changeColorTxtProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	cocos2d::ColorTxt* pColorTxt = WidgetFun::castNode<cocos2d::ColorTxt>(pNode);
	if (kKey =="MaxLineLength")
	{
		pColorTxt->setMaxLineLength(utility::parseInt(kValue));
	}
	if (kKey =="ShowTxt")
	{
		pColorTxt->setShowTxt(kValue);
	}
	
}
cocos2d::Node* WidgetManager::createWidgetTxtAtlas(WidgetTxtAtlas* pInfo)
{
	LabelAtlas* pAtlas = NULL;
	if (pInfo->bActionNum)
	{
		pAtlas = LabelAtlasAction::create(pInfo->kShowTxt.c_str(),
			pInfo->kImagic.c_str(),pInfo->iWidth,pInfo->iHeight,pInfo->iStartChar);
	}
	else
	{
		pAtlas = LabelAtlas::create(pInfo->kShowTxt.c_str(),
			pInfo->kImagic.c_str(),pInfo->iWidth,pInfo->iHeight,pInfo->iStartChar);
	}
	return pAtlas;
}

void WidgetManager::changeTxtAtlasProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	LabelAtlas* pLable = WidgetFun::castNode<LabelAtlas>(pNode);
	if (kKey =="ShowTxt")
	{
		pLable->setString(kValue.c_str());
	}
	if (kKey =="Opacity")
	{
		pLable->setOpacity(utility::parseInt(kValue.c_str()));
	}
	if (kKey =="IsAction")
	{
		LabelAtlasAction* pLableAction = WidgetFun::castNode<LabelAtlasAction>(pNode);
		pLableAction->setIsAction(utility::parseBool(kValue.c_str()));
	}
	if (kKey =="IsInShaortTxt")
	{
		LabelAtlasAction* pLableAction = WidgetFun::castNode<LabelAtlasAction>(pNode);
		pLableAction->setIsInShaortTxt(utility::parseBool(kValue.c_str()));
	}
}
cocos2d::Node* WidgetManager::createWidgetEdit(WidgetEdit* pInfo)
{
	EditBoxWidget* pTextField = EditBoxWidget::create(pInfo->kSize,Scale9SpriteEx::create(pInfo->kBackTexture));
	pTextField->setPlaceHolder(pInfo->kShowTxt.c_str());
	return pTextField;
}
void WidgetManager::changeEditProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	EditBoxWidget* pEdit = dynamic_cast<EditBoxWidget*>(pNode);
	if (kKey=="PlaceHolderTxt")
	{
		pEdit->setPlaceHolder(kValue.c_str());
	}
	if (kKey=="ShowTxt")
	{
		pEdit->setText(kValue.c_str());
	}
	if (kKey=="holderFontColor")
	{
		pEdit->setPlaceholderFontColor(utility::parseColour(kValue));
	}
	else if(kKey=="FontName")
	{
		pEdit->setFontName(kValue.c_str());
	}
	else if(kKey=="Dimensions")
	{
		pEdit->setContentSize(utility::parseSize(kValue));
	}
	else if(kKey=="FontSize")
	{
		pEdit->setFontSize(utility::parseInt(kValue));
	}
	else if(kKey=="Color")
	{
		pEdit->setFontColor(utility::parseColour(kValue));
	}
	else if(kKey=="PassWord")
	{
		pEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	}
	else if(kKey=="PlaceHolder")
	{
		pEdit->setPlaceHolder((kValue.c_str()));
	}
	else if(kKey=="MaxCout")
	{
		pEdit->setMaxLength(utility::parseInt(kValue.c_str()));
	}
	else if(kKey=="TypeNumber")
	{
		pEdit->setInputMode(EditBoxWidget::InputMode::PHONE_NUMBER);
	}
	else if(kKey =="InputMode"&&kValue=="ANY")
	{
		pEdit->setInputMode(EditBoxWidget::InputMode::ANY);
	}
}
cocos2d::Node* WidgetManager::createWidgetCheckBox(WidgetCheckBox* pInfo)
{
	ui::CheckBox* checkBox = ui::CheckBox::create(pInfo->kUnSelectTexture,
		pInfo->kSelectTexture,
		pInfo->kSelectTexture, pInfo->kSelectTexture, pInfo->kSelectTexture);
	checkBox->addEventListenerCheckBox(this,checkboxselectedeventselector(WidgetManager::CheckButtonReleaseEx));
	return checkBox;
}
void WidgetManager::changeCheckBoxProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	cocos2d::ui::CheckBox* pCheckBox = dynamic_cast<cocos2d::ui::CheckBox*>(pNode);
	if (kKey=="UnSelectTexture")
	{
		pCheckBox->loadTextureBackGround((kValue.c_str()));
		pCheckBox->loadTextureBackGroundSelected((kValue.c_str()));
		pCheckBox->getRendererBackgroundSelected()->setColor(ccc3(150,150,150));
	}
	if (kKey=="SelectTexture")
	{
		pCheckBox->loadTextureFrontCross((kValue.c_str()));
	}
	if (kKey=="Checked")
	{
		pCheckBox->setSelectedState(utility::parseBool(kValue));
	}
}

cocos2d::Node* WidgetManager::createWidgetButton(WidgetButton* pInfo)
{
	std::string kPressTxture = pInfo->kPressTexture;
	if (pInfo->kPressTexture == "AutoPress")
	{
		kPressTxture = pInfo->kNomalTexture;
	}
	cocos2d::ButtonEx* pButton = cocos2d::ButtonEx::create(pInfo->kNomalTexture,kPressTxture,pInfo->kDisableTexture);
	if (pInfo->kPressTexture == "AutoPress")
	{
		pButton->setPressColor(ccc3(150,150,150));
	}
	else if (pInfo->kPressTexture == "ActionPress")
	{
		pButton->setActionPress(true);
	}
	pButton->addTouchEventListener(CC_CALLBACK_2(WidgetManager::ButtonReleaseEx, this));
	return pButton;
}

void WidgetManager::changeButtonProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	cocos2d::ButtonEx* pButton = dynamic_cast<cocos2d::ButtonEx*>(pNode);
	if (kKey=="NomalTexture")
	{
		pButton->loadTextureNormal((kValue.c_str()));
	}
	if (kKey=="Scale9")
	{
		pButton->setScale9Enabled(utility::parseBool(kValue));
	}
	if (kKey=="ContentSize")
	{
		pButton->setContentSize(utility::parseSize(kValue));
	}
	
	if (kKey=="PressTexture")
	{
		if (kValue == "AutoPress")
		{
			pButton->setPressColor(ccc3(150,150,150));
		}
		else if (kValue == "ActionPress")
		{
			pButton->setActionPress(true);
		}
		else
		{
			pButton->loadTexturePressed((kValue.c_str()));
			pButton->setPressColor(ccc3(255,255,255));
		}
	}
	if (kKey=="DisableTexture")
	{
		pButton->loadTextureDisabled(kValue.c_str());
	}

	if (kKey=="Enable")
	{
		WidgetFun::setEnable(pButton,utility::parseBool(kValue));
	}
	if (kKey=="ShowTxt")
	{
		pButton->setTitleText(kValue);
	}
	if (kKey=="FontName")
	{
		pButton->setTitleFontName(kValue.c_str());
	}
	if (kKey=="FontSize")
	{
		pButton->setTitleFontSize(utility::parseFloat(kValue.c_str()));
	}
	if (kKey=="Color")
	{
		pButton->setTitleColor(utility::parseColour(kValue));
	}
	if (kKey == "Aple")
	{
		float fValue = utility::parseFloat(kValue);
		pButton->setCascadeOpacityEnabled(true);
		pButton->setOpacity(fValue*255);
	}
	if("OutLineColor" == kKey)
	{
		pButton->getTitleRenderer()->enableOutline(utility::parseColour4(kValue));
	}
	if("Alignment" == kKey)
	{
		pButton->setTitleAlignment((TextHAlignment)utility::parseInt(kValue));
	}
	
}
cocos2d::Node* WidgetManager::createWidgetSider(WidgetSider* pInfo)
{
	MenuItemImage* pButton = WidgetFun::createMenu(pInfo->kNomalTexture.c_str(),pInfo->kPressTexture.c_str(),
		pInfo->kDisableTexture.c_str());
	pButton->setUserObject(new WidgetUserInfo("Button"));
	SliderMenu* pMenu = SliderMenu::create(pButton);
	pMenu->setontrolItem(pButton);
	pMenu->setMaxPosY(pInfo->fMaxPosY);
	pMenu->setMaxValue(pInfo->fMaxValue);
	pMenu->setTarget(this,menu_selector(WidgetManager::SilderValue));
	pMenu->setPosition(ccp(0,0));
	if (pInfo->bMoveByX)
	{
		pMenu->setMoveByX(pInfo->bMoveByX);
	}
	
	return pMenu;
}
void WidgetManager::changeSiderProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	SliderMenu* pMenu = dynamic_cast<SliderMenu*>(pNode);
	if (kKey=="MoveByX")
	{
		pMenu->setMoveByX(utility::parseBool(kValue));
	}
}

cocos2d::Node* WidgetManager::createWidgetLayout( WidgetLayout* pInfo )
{
	ui::Layout* pRet = ui::Layout::create();
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
	pRet->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	pRet->setContentSize(Size(200,200));
	pRet->setBackGroundColor(Color3B(10,10,10));
#endif
	return pRet;
}

void WidgetManager::changeLayoutProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue )
{
	ui::Layout* pPanle = dynamic_cast<ui::Layout*>(pNode);
	if (kKey=="Color")
	{
		pPanle->setColor(utility::parseColour(kValue));
	}
	if (kKey=="BgColor")
	{
		pPanle->setBackGroundColor(utility::parseColour(kValue));
	}
	if(kKey=="Texture")
	{
		pPanle->setBackGroundImage(kValue);
	}
	if(kKey=="ClippingEnabled")
	{
		pPanle->setClippingEnabled(utility::parseBool(kValue));
	}
	if (kKey =="TouchEnable")
	{
		pPanle->setTouchEnabled(utility::parseBool(kValue));
		if (utility::parseBool(kValue))
		{
			pPanle->addTouchEventListener(CC_CALLBACK_2(WidgetManager::ButtonReleaseEx,this));
		}
	}
	if (kKey =="HideBack")
	{
		pPanle->setBackGroundColorType(ui::Layout::BackGroundColorType::NONE);
	}
}
cocos2d::Node* WidgetManager::createWidgetList(WidgetList* pInfo)
{
	ListViewEx *pRet = ListViewEx::create();
	pRet->setBounceEnabled(true);
	pRet->setDirection(ui::ScrollView::Direction::VERTICAL);
	pRet->setGravity(ui::ListView::Gravity::TOP);
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
	pRet->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	pRet->setContentSize(Size(200,200));
	pRet->setBackGroundColor(Color3B(10,10,10));
#endif
	return pRet;
}
void WidgetManager::changeListProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	ListViewEx *pRet = WidgetFun::castNode<ListViewEx>(pNode);

	if (kKey =="Margin")
	{
		pRet->setItemsMargin(utility::parseFloat(kValue));
	}
	if (kKey=="Bounce")
	{
		pRet->setBounceEnabled(utility::parseBool(kValue));
	}
	if (kKey=="Gravity")
	{
		pRet->setGravity((ui::ListView::Gravity)utility::parseInt(kValue));
	}
	if (kKey=="ClippingEnabled")
	{
		pRet->setClippingEnabled(utility::parseBool(kValue));
	}
	if (kKey=="Direction")
	{
		pRet->setDirection((ui::ScrollView::Direction)utility::parseInt(kValue));
	}
	if (kKey =="BackGroundColor")
	{
		pRet->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
		pRet->setBackGroundColor(utility::parseColour(kValue));
	}
	if (kKey =="UpDate")
	{
		pRet->forceDoLayout();
	}
	if (kKey =="HideBack")
	{
		pRet->setBackGroundColorType(ui::Layout::BackGroundColorType::NONE);
	}
}
cocos2d::Node* WidgetManager::createWidgetRichText(WidgetRichText* pInfo)
{
	ui::RichText* _richText = ui::RichText::create();
	return _richText;
}
void WidgetManager::changeRichTextProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	ui::RichText *pRet = WidgetFun::castNode<ui::RichText>(pNode);

	if (kKey =="Size")
	{
		pRet->ignoreContentAdaptWithSize(false);
		pRet->setSize(utility::parseSize(kValue));
	}
	if (kKey =="ElementText")
	{
		std::vector<std::string> kVecString = utility::split(kValue,":");
		if (kVecString.size() != 4)
		{
			CCAssert(false,"");
			return;
		}
		 ui::RichElementText* re = ui::RichElementText::create(utility::parseInt(kVecString[0]), 
			 utility::parseColour(kVecString[1]),
			 255, 
			kVecString[3], 
			 "", 
			 utility::parseInt(kVecString[2]));
		 pRet->pushBackElement(re);
	}
	if (kKey =="ElementImagic")
	{
		std::vector<std::string> kVecString = utility::split(kValue,":");
		if (kVecString.size() != 3)
		{
			CCAssert(false,"");
			return;
		}
		ui::RichElementImage* re = ui::RichElementImage::create(utility::parseInt(kVecString[0]), 
			Color3B(255,255,255),
			utility::parseInt(kVecString[1]), 
			kVecString[2]);
		pRet->pushBackElement(re);
	}
	if (kKey =="ElementCustomNode")
	{
		std::vector<std::string> kVecString = utility::split(kValue,":");
		if (kVecString.size() != 3)
		{
			CCAssert(false,"");
			return;
		}
		cocos2d::Node* pNode = cocos2d::Node::create();
		cocos2d::Node* pTempNode = createWidget(kVecString[2],pNode);
		pTempNode->retain();
		pTempNode->removeFromParent();
		ui::RichElementCustomNode* re = ui::RichElementCustomNode::create(utility::parseInt(kVecString[0]), 
			Color3B(255,255,255),
			utility::parseInt(kVecString[1]), 
			pTempNode);
		pRet->pushBackElement(re);
		pTempNode->release();
	}
}
void WidgetManager::changeProperty(cocos2d::Node* pNode,stringKeyValueList& kProperty,WidgetManager_Handler pHander)
{
	stringKeyValueList::iterator itor = kProperty.begin();
	for (;itor != kProperty.end();itor++)
	{
		(this->*pHander)(pNode,itor->kKey,itor->kValue);
		changeProperty(pNode,itor->kKey,itor->kValue);
	}
}
void WidgetManager::changeProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	
	if (kKey == "Scale")
	{
		float fScale = utility::parseFloat(kValue);
		pNode->setScale(fScale);
	}
	if (kKey == "Size")
	{
		pNode->setContentSize(utility::parseSize(kValue));
	}
	if (kKey == "ScaleX")
	{
		float fScale = utility::parseFloat(kValue);
		pNode->setScaleX(fScale);
	}
	if (kKey == "Rotation")
	{
		float fScale = utility::parseFloat(kValue);
		pNode->setRotation(fScale);
	}
	if (kKey == "ScaleY")
	{
		float fScale = utility::parseFloat(kValue);
		pNode->setScaleY(fScale);
	}
	if (kKey == "Pos")
	{
		cocos2d::Vec2 kPoint = utility::parsePoint(kValue);
		pNode->setPosition(kPoint);
	}
	if (kKey == "ParentPos" && pNode->getParent())
	{
		pNode->setPosition(pNode->getParent()->getPosition());
	}
	if (kKey == "AnchorPos")
	{
		cocos2d::Vec2 kPoint = utility::parsePoint(kValue);
		pNode->setAnchorPoint(kPoint);
	}
	if (kKey == "Tag")
	{
		float iTag = utility::parseInt(kValue);
		pNode->setTag(iTag);
	}
	if (kKey == "Oder")
	{
		float iOder = utility::parseInt(kValue);
		CCAssert(pNode->getParent(),"changeProperty Oder");
		pNode->getParent()->reorderChild(pNode,iOder);
	}
	if (kKey == "Visible")
	{
		bool bValue = utility::parseBool(kValue);
		pNode->setVisible(bValue);
	}
	if (kKey == "BackWinSizeX")
	{
		std::vector<std::string> kStr = utility::split(kValue,":");
		if (kStr.size() == 1 && utility::parseBool(kStr[0]))
		{
			WindowInfo::Instance().BackWinSizeX(pNode,0);
		}
		if (kStr.size() == 2 && utility::parseBool(kStr[0]))
		{
			WindowInfo::Instance().BackWinSizeX(pNode,utility::parseInt(kStr[1]));
		}
	}
	if (kKey == "BackWinSizeY" )
	{
 		std::vector<std::string> kStr = utility::split(kValue,":");
 		if (kStr.size() == 1 && utility::parseBool(kStr[0]))
 		{
 			WindowInfo::Instance().BackWinSizeY(pNode,0);
 		}
 		if (kStr.size() == 2 && utility::parseBool(kStr[0]))
 		{
 			WindowInfo::Instance().BackWinSizeY(pNode,utility::parseInt(kStr[1]));
 		}
	}

	if (kKey == "UserInfo")
	{
		std::vector<std::string> kStr = utility::split(kValue,"|");
		CCAssert(kStr.size() == 2,"");
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		pUserInfo->kUserInfo.insert(std::make_pair(kStr[0],kStr[1]));
	}
	if (kKey == "ChildProperty")
	{
		std::vector<std::string> kStr = utility::split(kValue,":");
		CCAssert(kStr.size() == 3,"");
		cocos2d::Node* pChildNode = WidgetFun::getChildWidgetByName(pNode,kStr[0]);
		WidgetFun::setWidgetProperty(pChildNode,kStr[1],kStr[2]);
	}
	if (kKey == "CascadeOpacityEnabled")
	{
		pNode->setCascadeOpacityEnabled(utility::parseBool(kValue));
	}
	if (kKey == "Card")
	{
		pNode->setCascadeOpacityEnabled(utility::parseBool(kValue));
	}

	if (kKey == "AddMotion")
	{
		std::vector<std::string> kVecotr = utility::split(kValue,":");
		CCAssert(kValue.size() == 4,"");
		float fade = utility::parseFloat(kVecotr[0]);//2.0
		float minSeg = utility::parseFloat(kVecotr[1]);//1.0
		float stroke = utility::parseFloat(kVecotr[2]);//50
		MotionStreak* streak = MotionStreak::create(fade, minSeg, stroke, Color3B(255, 255, 255),kVecotr[3]);
		pNode->addChild(streak);
	}
}

cocos2d::Node* WidgetManager::createWidgetProgressBar( WidgetProgressBar* pInfo )
{
	ProgressBarWidget* pProgress = new ProgressBarWidget(pInfo->kBackTexture,pInfo->kBarTexture);
	pProgress->autorelease();
	pProgress->setFontSize(pInfo->kFontSize);
	pProgress->setProgressWithData(0,1);
	return pProgress;
}

void WidgetManager::changeProgressBarProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue )
{
	ProgressBarWidget* pProgress = dynamic_cast<ProgressBarWidget*>(pNode);
	if (kKey=="FontSize")
	{
		pProgress->setFontSize(utility::parseFloat(kValue));
	}
	else if(kKey=="Color")
	{
		pProgress->setTextColor(utility::parseColour(kValue));
	}
}

cocos2d::Node* WidgetManager::createWidgetProgressTimer(WidgetProgressTimer* pInfo)
{
	cocos2d::Sprite* pSprite = Sprite::create(pInfo->kTexture.c_str());
	CCProgressTimer *pProgress = CCProgressTimer::create(pSprite);
	return pProgress;
}
void WidgetManager::changeProgressTimerProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
{
	CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer*>(pNode);
	if (kKey=="Radial" && utility::parseBool(kValue))
	{
		pProgress->setType( kCCProgressTimerTypeRadial );
	}
	if (kKey=="Bar" && utility::parseBool(kValue))
	{
		pProgress->setType(ProgressTimer::Type::BAR);
	}
	if (kKey=="Reverse")
	{
		pProgress->setReverseProgress(utility::parseBool(kValue));
	}
	if (kKey=="ChangeRate")
	{
		pProgress->setBarChangeRate(utility::parsePoint(kValue));
	}
	if (kKey=="Midpoint")
	{
		pProgress->setMidpoint(utility::parsePoint(kValue));
	}
	if (kKey=="Percent")
	{
		pProgress->setPercentage(utility::parseFloat(kValue));
	}
	
}
cocos2d::Node* WidgetManager::createWidget9Sprite( Widget9Sprite* pInfo )
{
	Scale9SpriteEx* p9Sprite = Scale9SpriteEx::create(pInfo->kBackTexture.c_str());
	p9Sprite->setPreferredSize(pInfo->kSize);
	p9Sprite->setCapInsets(pInfo->kInsetsRect);
	return p9Sprite;
}

void WidgetManager::change9SpriteProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue )
{
	Scale9SpriteEx* p9Sprite = dynamic_cast<Scale9SpriteEx*>(pNode);

	if (kKey=="ImagicTexture")
	{
		p9Sprite->initWithFile(kValue.c_str());
	}
	else if (kKey=="PreferredSize")
	{
		p9Sprite->setPreferredSize(utility::parseSize(kValue));
	}
	else if (kKey=="Aple")
	{
		p9Sprite->setOpacity(utility::parseFloat(kValue)*255);
	}
	else if (kKey=="InsetsRect")
	{
		p9Sprite->setCapInsets(utility::parseRect(kValue));
	}
	else if (kKey=="FlipX")
	{
		p9Sprite->setFlippedX(utility::parseBool(kValue));
	}
	else if (kKey=="FlipY")
	{
		p9Sprite->setFlippedY(utility::parseBool(kValue));
	}
}

cocos2d::Node* WidgetManager::createWidgetAnimation( WidgetAnimation* pInfo )
{
	AnimationWidget*  pAnimation = AnimationWidget::createAnimation();
	return pAnimation;
}

void WidgetManager::changeAnimationProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue )
{
	AnimationWidget* pAnimation = dynamic_cast<AnimationWidget*>(pNode);
	if (kKey=="Frame")
	{
		pAnimation->addFrame(kValue);
	}
	if (kKey=="Time")
	{
		pAnimation->setIntervalTime(utility::parseFloat(kValue));
	}
	if (kKey=="Loop")
	{
		pAnimation->setLoop(utility::parseBool(kValue));
	}
	if (kKey=="runFrames")
	{
		pAnimation->runFrames((kValue));
	}
	if (kKey=="addPlist")
	{
		std::vector<std::string> kVector = utility::split(kValue,":");
		if (kVector.size() == 2)
		{
			AnimationWidget::addPlist(kVector[0],kVector[1]);
		}
		else
		{
			CCAssert(false,"");
		}
	}
	if (kKey=="DelayTime")
	{
		pAnimation->setDelayTime(utility::parseFloat(kValue));
	}
	
}
std::vector<std::string>& WidgetManager::getOnlyVaildButton()
{
	return m_kOnlyVaildButton; 
}

cocos2d::Node* WidgetManager::createWidgetPanle( WidgetPanle* pInfo )
{
	CCLayerColor* pPanle = CCLayerColor::create(ccc4(pInfo->kColor.r,pInfo->kColor.g,pInfo->kColor.b,255),pInfo->kSize.width,pInfo->kSize.height);
	return pPanle;
}

void WidgetManager::changePanleProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue )
{
	CCLayerColor* pPanle = dynamic_cast<CCLayerColor*>(pNode);
	if (kKey=="Color")
	{
		pPanle->setColor(utility::parseColour(kValue));
	}
	if (kKey=="Aple")
	{
		pPanle->setOpacity(utility::parseFloat(kValue)*255);
	}
}

cocos2d::Node* WidgetManager::createWidgetSwitch_Button(WidgetSwitch_Button* pInfo)
{
	CCAssert(false,"");
	return NULL;
}
void WidgetManager::changeSwitch_ButtonProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue )
{
}
cocos2d::Node* WidgetManager::createWidgetSelectBox( WidgetSelectBox* pInfo )
{
	SelectBox* pSelectBox = nullptr;
	pSelectBox = SelectBox::create(pInfo->kUnSelectTexture,pInfo->kSelectTexture);

	return pSelectBox;
}
void WidgetManager::changeSelectBoxProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue )
{
	SelectBox* pSwitch = dynamic_cast<SelectBox*>(pNode);

// 	if ()
// 	{
// 	}
}


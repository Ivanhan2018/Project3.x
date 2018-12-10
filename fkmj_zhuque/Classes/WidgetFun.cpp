#include "WidgetFun.h"
#include "utility.h"
#include "ActionEx.h"
#include "WidgetManager.h"
#include "EditBoxWidget.h"
#include "ProgressBarWidget.h"
#include "AnimationWidget.h"
#include "SliderMenu.h"
#include "ColorTxt.h"
#include "SelectBox.h"
#include "ButtonEx.h"
#include "CCScale9Sprite.h"
#include "ActionTagDefine.h"
#include "LabelAtlasAction.h"

USING_NS_CC;

namespace WidgetFun
{
	bool isSpriteTouched(cocos2d::Sprite* pSprite,cocos2d::Vec2 kWorldPos)
	{
		if (!pSprite)
		{
			return false;
		}
		
		Rect rect = CCRectMake(0, 0, pSprite->getContentSize().width, pSprite->getContentSize().height);
		return rect.containsPoint(
			pSprite->convertToNodeSpace(kWorldPos));
	}
	WidgeTextureInfo paseTextureInfo(std::string kValue)
	{
		WidgeTextureInfo kInfo;
		std::vector<std::string> kStr = utility::split(kValue,":");
		if(kStr.size() == 2)
		{
			kInfo.kName = (kStr[0]);
			kInfo.kRect = utility::parseRect(kStr[1]);
			kInfo.bRect = true;
		}
		else if(kStr.size() == 1)
		{
			kInfo.kName = (kStr[0]);
			kInfo.kRect = Rect::ZERO;
			kInfo.bRect = false;
		}
		else
		{
			utility::CCAssertWithDes(false,kValue," paseTextureInfo Error");
		}
		return kInfo;
	}
	cocos2d::Sprite* createSprite(const WidgeTextureInfo& kInfo)
	{
		Sprite* pSprite = NULL;
		pSprite = createSprite(kInfo.kName.c_str());
		CCAssert(pSprite,"");
		setTexture(pSprite,kInfo);
		return pSprite;
	}
	cocos2d::MenuItemImage* createMenu(std::string normalImage,std::string selectedImage,std::string disabledImage)
	{
		MenuItemImage *pRet = MenuItemImage::create(normalImage,selectedImage,disabledImage);
		return pRet;
	}
	cocos2d::SpriteFrame* getFrame( const std::string& kFileName )
	{
		return SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(kFileName.c_str());
	}
	cocos2d::Sprite* createSprite(const char *kFileName)
	{
		if (!kFileName || std::string(kFileName) == "")
		{
			return NULL;
		}
		Sprite * pSprite = NULL;
		pSprite = Sprite::create(kFileName);
		if (!pSprite)
		{
			pSprite = Sprite::create("public/Error.png");
			CCAssert(pSprite,"");
		}
		return pSprite;
	}

	void setTexture(Sprite* pSprite,const WidgeTextureInfo& kInfo)
	{
		pSprite->setTexture(kInfo.kName);
		if (kInfo.bRect)
		{
			pSprite->setTextureRect(kInfo.kRect);
		}
	}
	cocos2d::Node* getChildWidget(cocos2d::Node* pNode,std::string kName)
	{
		cocos2d::Vec2 kPoint;
		std::vector<std::string> kStr = utility::split(kName,":");
		if(kStr.size() == 2)
		{
			return getChildWidgetSkin(pNode,kStr[0],kStr[1]);
		}
		return WidgetFun::getChildWidgetByName(pNode,kName);
	}
	cocos2d::Node* getChildWidgetByName(cocos2d::Node* pNode,std::string kName)
	{
		if (!pNode)
		{
			return NULL;
		}
		WidgetUserInfo* pUserInfo = getUserInfo(pNode);
		if (pUserInfo && pUserInfo->kName == kName)
		{
			return pNode;
		}

		Vector<Node*>& pChildren = pNode->getChildren();

		if (pChildren.size() <= 0)
		{
			return NULL;
		}
		for(Node* pChild : pChildren)
		{
			WidgetUserInfo* pUserInfo = getUserInfo(pChild);
			if (!pUserInfo)
			{
				continue;
			}
			if (pUserInfo && pUserInfo->kName == kName)
			{
				return pChild;
			}
		}
		for(Node* pChild : pChildren)
		{
			Node* pGetChild = getChildWidgetByName(pChild,kName);
			if (pGetChild)
			{
				return pGetChild;
			}
		}
		return NULL;
	}
	cocos2d::Node* getChildWidgetSkin(cocos2d::Node* pNode,std::string kScencName,std::string kSkinName)
	{
		Node* pScenceNode = getChildWidgetByName(pNode,kScencName);
		if (!pScenceNode)
		{
			return NULL;
		}
		return getChildWidgetByName(pScenceNode,kSkinName);
	}
	WidgetUserInfo* getUserInfo(cocos2d::Node* pNode)
	{
		CCAssert(pNode,"");
		if (!pNode->getUserObject())
		{
			return NULL;
		}
		return dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
	}
	void setWidgetUserInfo(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kValue)
	{
		cocos2d::Node* pTemp = getChildWidget(pNode,kName);
		setWidgetUserInfo(pTemp,kKey,kValue);
	}
	void setWidgetUserInfo(cocos2d::Node* pNode,std::string kKey,std::string kValue)
	{
		if (!pNode)
		{
			CCAssert(false,kValue.c_str());
		}
		if (!pNode->getUserObject())
		{
			WidgetUserInfo* pUserInfo = new WidgetUserInfo;
			pNode->setUserObject(pUserInfo);
		}
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		StringMap::iterator itor = pUserInfo->kUserInfo.find(kKey);
		if (itor != pUserInfo->kUserInfo.end())
		{
			itor->second = kValue;
		}
		else
		{
			pUserInfo->kUserInfo.insert(std::make_pair(kKey,kValue));
		}
	}
	std::string getUserInfoValue(WidgetUserInfo* pUserInfo ,std::string kKey)
	{
		StringMap::iterator itor = pUserInfo->kUserInfo.find(kKey);
		if (itor != pUserInfo->kUserInfo.end())
		{
			return itor->second;
		}
		return WidgetNotFindUserInfo;
	}
	int getUserInfoInt(WidgetUserInfo* pUserInfo,std::string kKey)
	{
		return utility::parseInt(getUserInfoValue(pUserInfo,kKey));
	}
	std::string setUserInfoValue(WidgetUserInfo* pUserInfo ,std::string kKey)
	{
		StringMap::iterator itor = pUserInfo->kUserInfo.find(kKey);
		if (itor != pUserInfo->kUserInfo.end())
		{
			return itor->second;
		}
		return WidgetNotFindUserInfo;
	}
	std::string getWidgetUserInfo(cocos2d::Node* pNode,std::string kName,std::string kKey)
	{
		cocos2d::Node* pTemp = getChildWidget(pNode,kName);
		return getWidgetUserInfo(pTemp,kKey);
	}
	std::string getWidgetUserInfo(cocos2d::Node* pNode,std::string kKey)
	{
		if (!pNode || !pNode->getUserObject())
		{
			return WidgetNotFindUserInfo;
		}
		WidgetUserInfo* pUserInfo = getUserInfo(pNode);
		return getUserInfoValue(pUserInfo,kKey);
	}
	void setWidgetProperty(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kValue)
	{
		Node* pScenceNode = getChildWidget(pNode,kName);
		if (!pScenceNode)
		{
			CCAssert(false,utility::toString("setWidgetProperty: ",kName).c_str());
		}
		if (pScenceNode && pScenceNode->getUserObject())
		{
			WidgetUserInfo* pUserInfo = getUserInfo(pScenceNode);
			WidgetManager::Instance().changePropertyByType(pScenceNode,pUserInfo->kType,kKey,kValue);
		}
	}
	void setWidgetProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue)
	{
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		if (pUserInfo)
		{
			WidgetManager::Instance().changePropertyByType(pNode,pUserInfo->kType,kKey,kValue);
		}
	}
	void setPos(cocos2d::Node* pNode,cocos2d::Vec2 kPoint)
	{
		CCAssert(pNode,"");
		pNode->setPosition(kPoint);

	}
	void setPos(cocos2d::Node* pNode,std::string kName,cocos2d::Vec2 kPoint)
	{
		setPos(getChildWidget(pNode,kName),kPoint);
	}
	cocos2d::Vec2 getPos(cocos2d::Node* pNode,std::string kName)
	{
		Node* pWidget = getChildWidgetByName(pNode,kName);
		return pWidget->getPosition();
	}
	void setTextStr(cocos2d::Node* pNode,std::string kTxt)
	{
		CCAssert(pNode,"");
		ui::Text* pLabel = castNode<ui::Text>(pNode);
		pLabel->setString(kTxt.c_str());
	}
	void setTextStr(cocos2d::Node* pNode,std::string kName,std::string kTxt)
	{
		setTextStr(getChildWidget(pNode,kName),kTxt);
	}
	void setTextColor(cocos2d::Node* pNode,std::string kTxt)
	{
		ui::Text* pLabel = castNode<ui::Text>(pNode);
		pLabel->setColor(utility::parseColour(kTxt));
	}
	void setTextColor(cocos2d::Node* pNode,std::string kName,std::string kTxt)
	{
		setTextColor(getChildWidget(pNode,kName),kTxt);
	}
	void setColorText(cocos2d::Node* pNode,std::string kTxt)
	{
		CCAssert(pNode,"");
		ColorTxt* pColorTxt = castNode<ColorTxt>(pNode);
		CCAssert(pColorTxt,"");
		pColorTxt->setShowTxt(kTxt);

	}
	void setColorText(cocos2d::Node* pNode,std::string kName,std::string kTxt)
	{
		setColorText(getChildWidget(pNode,kName),kTxt);
	}
	void setColorIdexTxt(cocos2d::Node* pNode,int Idex,std::string kTxt)
	{
		CCAssert(pNode,"");
		ColorTxt* pColorTxt = castNode<ColorTxt>(pNode);
		CCAssert(pColorTxt,"");
	}
	void setColorIdexTxt(cocos2d::Node* pNode,int Idex,std::string kName,std::string kTxt)
	{
		setColorIdexTxt(getChildWidget(pNode,kName),Idex,kTxt);
	}
	
	std::string getText(cocos2d::Node* pNode,std::string kName)
	{
		Node* pWidget = getChildWidgetByName(pNode,kName);
		ui::Text* pLabel = castNode<ui::Text>(pWidget);
		CCAssert(pLabel,"");
		return pLabel->getString();
	}
	void setTextColor(cocos2d::Node* pNode, std::string kName,const cocos2d::Color3B& color )
	{
		Node* pWidget = getChildWidget(pNode,kName);
		ui::Text* pLabel = castNode<ui::Text>(pWidget);
		CCAssert(pLabel,"");
		return pLabel->setColor(color);
	}
	void setEditeText(cocos2d::Node* pNode,std::string kTxt)
	{
		CCAssert(pNode,"");
		EditBoxWidget* pField = castNode<EditBoxWidget>(pNode);
		CCAssert(pField,"");
		pField->setText(kTxt.c_str());
	}
	void setEditeText(cocos2d::Node* pNode,std::string kName,std::string kTxt)
	{
		setEditeText(getChildWidget(pNode,kName),kTxt);
	}
	std::string getEditeText(cocos2d::Node* pNode,std::string kName)
	{
		Node* pWidget = getChildWidget(pNode,kName);
		EditBoxWidget* pField = castNode<EditBoxWidget>(pWidget);
		CCAssert(pField,"");
		return pField->getText();
	}
	void SetEditReturnAction(cocos2d::Node* pNode,std::function<void()> pFun)
	{
		EditBoxWidget* pField = castNode<EditBoxWidget>(pNode);
		pField->SetReturnAction(pFun);
	}
	void SetEditReturnAction(cocos2d::Node* pNode,std::string kName,std::function<void()> pFun)
	{
		SetEditReturnAction(getChildWidget(pNode,kName),pFun);
	}
	void setImagic(cocos2d::Node* pNode,std::string kImagic,int bSame)
	{
		CCAssert(pNode,"");
		Sprite* pImagic = castNode<Sprite>(pNode);
		CCAssert(pImagic,"");
		Size sz = pImagic->getContentSize();
		float fx= pImagic->getScaleX()*sz.width;
		float fy = pImagic->getScaleY()*sz.height;
		pImagic->setTexture(kImagic);
		CCAssert(pImagic->getTexture(),kImagic.c_str());
		if (bSame)
		{
			utility::setSpriteScaleBySize(pImagic,CCSizeMake(fx,fy));
		}
	}

	void setImagic(cocos2d::Node* pNode,std::string kName,std::string kImagic,bool bSame)
	{
		setImagic(getChildWidget(pNode,kName),kImagic,bSame);
	}
	void setImagicAple(cocos2d::Node* pNode,float fAple)
	{
		if (fAple > 1)
		{
			fAple = 1;
		}
		if (fAple < 0)
		{
			fAple = 0;
		}
		CCAssert(pNode,"");
		Sprite* pImagic = castNode<Sprite>(pNode);
		CCAssert(pImagic,"");
		pImagic->setOpacity(fAple*255);
	}
	void setImagicAple(cocos2d::Node* pNode,std::string kName,float fAple)
	{

		setImagicAple(getChildWidget(pNode,kName),fAple);
	}

	void setImagicWithMask( cocos2d::Node* pNode,std::string kName,std::string kImagic,std::string kMask )
	{
		Sprite * pSprite = castNode<Sprite>(getChildWidget(pNode,kName));
		setImagic(pSprite,kImagic,false);
	}
	
	void addImagicWithMask( cocos2d::Node* pNode,std::string kName,std::string kMask )
	{
		Sprite * pSprite = castNode<Sprite>(getChildWidget(pNode,kName));
	}

	void setImagicPercent(cocos2d::Node* pNode,float fPercent)
	{
		Sprite * pSprite = castNode<Sprite>(pNode);
		float fWide = pSprite->getTexture()->getPixelsWide();
		float fHigh = pSprite->getTexture()->getPixelsHigh();
		Rect kRect;
		kRect.size.width = fWide*fPercent;
		kRect.size.height = fHigh;
		pSprite->setTextureRect(kRect);
	}
	void setImagicPercent(cocos2d::Node* pNode,std::string kName,float fPercent)
	{
		Sprite * pSprite = castNode<Sprite>(getChildWidget(pNode,kName));
		setImagicPercent(pSprite,fPercent);
	}
	void setFace(cocos2d::Node* pNode,std::string kFace)
	{
		CCAssert(pNode,"");
		AnimationWidget* pAnimation = castNode<AnimationWidget>(pNode);
		CCAssert(pAnimation,"");
		pAnimation->runFrames(kFace);
	}
	void setFace(cocos2d::Node* pNode,std::string kName,std::string kFace)
	{
		setFace(getChildWidget(pNode,kName),kFace);
	}
	void setVisible(cocos2d::Node* pNode,bool bVisible)
	{
		CCAssert(pNode,"");
		pNode->setVisible(bVisible);
	}
	void setVisible(cocos2d::Node* pNode,std::string kName,bool bVisible)
	{
		cocos2d::Node* pTempNode = getChildWidget(pNode,kName);
		if (!pTempNode)
		{
			utility::log("setVisible");
			utility::log(kName.c_str());
		}
		setVisible(pTempNode,bVisible);
	}
	bool isWidgetVisble( cocos2d::Node* pNode,std::string kName )
	{
		return getChildWidget(pNode,kName)->isVisible();
	}
	void setName(cocos2d::Node* pNode,std::string kValue)
	{
		WidgetUserInfo* pUserInfo = getUserInfo(pNode);
		if (!pUserInfo)
		{
			pUserInfo = new WidgetUserInfo;
			pNode->setUserObject(pUserInfo);
		}
		CCAssert(pUserInfo,"");
		pUserInfo->kName = kValue;
	}
	void setName(cocos2d::Node* pNode,std::string kName,std::string kValue)
	{
		return setName(getChildWidget(pNode,kName),kValue);
	}

	void setAtlasTxt(cocos2d::Node* pNode,std::string kTxt)
	{
		CCAssert(pNode,"");
		LabelAtlas* pAtlas = castNode<LabelAtlas>(pNode);
		CCAssert(pAtlas,"");
		
		pAtlas->setString(kTxt.c_str());
	}
	void setAtlasTxt(cocos2d::Node* pNode,std::string kName,std::string kTxt)
	{
		setAtlasTxt(getChildWidget(pNode,kName),kTxt);
	}

	void setAtlasActionTxt(cocos2d::Node* pNode,int iValue)
	{
		CCAssert(pNode,"");
		LabelAtlasAction* pAtlas = castNode<LabelAtlasAction>(pNode);
		CCAssert(pAtlas,"");

		pAtlas->setStringAction(iValue);
	}
	void setAtlasActionTxt(cocos2d::Node* pNode,std::string kName,int iValue)
	{
		setAtlasActionTxt(getChildWidget(pNode,kName),iValue);
	}

	int getAtlasActionTxt(cocos2d::Node* pNode)
	{
		CCAssert(pNode,"");
		LabelAtlasAction* pAtlas = castNode<LabelAtlasAction>(pNode);
		CCAssert(pAtlas,"");

		std::string kStr = pAtlas->getString();
		return utility::parseInt(kStr);
	}
	int getAtlasActionTxt(cocos2d::Node* pNode,std::string kName)
	{
		return getAtlasActionTxt(getChildWidget(pNode,kName));
		
	}
	void setEnable(cocos2d::Node* pNode,bool bEnable)
	{
		CCAssert(pNode,"");
		cocos2d::ButtonEx* pButton = castNode<cocos2d::ButtonEx>(pNode);
		pButton->setBright(bEnable);
		pButton->setEnabled(bEnable);
		pButton->setTouchEnabled(bEnable);
	}
	void setEnable(cocos2d::Node* pNode,std::string kName,bool bEnable)
	{
		setEnable(getChildWidget(pNode,kName),bEnable);
	}
	void setChecked(cocos2d::Node* pNode,bool bChecked)
	{
		CCAssert(pNode,"");
		cocos2d::ui::CheckBox* pCheckBox = castNode<cocos2d::ui::CheckBox>(pNode);
		pCheckBox->setSelected(bChecked);
	}
	void setChecked(cocos2d::Node* pNode,std::string kName,bool bChecked)
	{
		setChecked(getChildWidget(pNode,kName),bChecked);
	}
	bool isChecked(cocos2d::Node* pNode)
	{
		CCAssert(pNode,"");
		cocos2d::ui::CheckBox* pCheckBox = castNode<cocos2d::ui::CheckBox>(pNode);
		return pCheckBox->isSelected();
	}
	bool isChecked(cocos2d::Node* pNode,std::string kName)
	{
		return isChecked(getChildWidget(pNode,kName));
	}
	void setLabelKeepLeft( cocos2d::Node* pNode,std::string kName )
	{
		CCAssert(pNode,"");
		CCLabelTTF* pLabel = castNode<CCLabelTTF>(getChildWidget(pNode,kName));
		CCAssert(pLabel,"");
		Point pos = pLabel->getPosition();
		Size sz = pLabel->getContentSize();
		pLabel->setPosition(ccp(pos.x+sz.width/2,pos.y));
	}

	void setNodeKeepAfter( cocos2d::Node* pNode,cocos2d::Node* leftNode,std::string kName )
	{
		CCAssert(pNode,"");
		CCAssert(leftNode,"left node error");
		Node* pNodeRight = getChildWidget(pNode,kName);
		CCAssert(pNodeRight,"");

		Point pos1 = leftNode->getPosition();
		Size  sz1 = leftNode->getContentSize();

		pNodeRight->setPosition(ccp(pos1.x+sz1.width,pos1.y));
	}


	void setSelectBoxTag( cocos2d::Node* pNode,std::string kName,int nTag )
	{
		CCAssert(pNode,"");
		Node* pSelectBox = getChildWidget(pNode,kName);
		CCAssert(pSelectBox," not find btn");
		pSelectBox->setTag(nTag);
	}


	void setProgress( cocos2d::Node* pNode,std::string kName,float fCurrent,float fMax)
	{
		CCAssert(pNode,"");
		ProgressBarWidget* pProgress = castNode<ProgressBarWidget>(getChildWidget(pNode,kName));
		CCAssert(pProgress,"pProgress error");
		pProgress->setProgressWithData(fCurrent,fMax);
	}
	void AnimStart( cocos2d::Node* pNode,std::string kName )
	{
		CCAssert(pNode,"");
		AnimationWidget* pAnim = castNode<AnimationWidget>(getChildWidget(pNode,kName));
		CCAssert(pAnim,"pAnim error");
	}

	void AnimStop( cocos2d::Node* pNode,std::string kName )
	{
		CCAssert(pNode,"");
		AnimationWidget* pAnim = castNode<AnimationWidget>(getChildWidget(pNode,kName));
		CCAssert(pAnim,"pAnim error");
	}


	cocos2d::ListViewEx* getListViewWidget( cocos2d::Node* pNode,std::string kName )
	{
		CCAssert(pNode,"");
		cocos2d::ListViewEx* pListView = castNode<cocos2d::ListViewEx>(getChildWidget(pNode,kName));
		CCAssert(pListView,"pListView error");
		return pListView;
	}

	float getWidgetScale( cocos2d::Node* pNode,std::string kName )
	{
		CCAssert(pNode,"");
		Node* node = getChildWidget(pNode,kName);
		CCAssert(node,"node error");
		return node->getScaleX();
	}

	void setWidgetScale( cocos2d::Node* pNode,std::string kName,float fScale )
	{
		CCAssert(pNode,"");
		Node* node = getChildWidget(pNode,kName);
		CCAssert(node,"node error");
		node->setScale(fScale);
	}

	cocos2d::Size getWidgetSize( cocos2d::Node* pNode,std::string kName )
	{
		CCAssert(pNode,"");
		Node* node = getChildWidget(pNode,kName);
		CCAssert(node,"node error");
		return node->getContentSize();
	}

	void setSliderMenuActValue( cocos2d::Node* pNode,std::string kName,float fValue )
	{
		CCAssert(pNode,"");
		Node* pMenu = getChildWidget(pNode,kName);
		SliderMenu* pItem = castNode<SliderMenu>(pMenu);
		CCAssert(pItem,"pItem  error");
		pItem->setActValue(fValue);

	}

	cocos2d::Sprite* createMaskedSprite(cocos2d::Node* src,std::string kFileName) 
	{
		return NULL;
	}
	cocos2d::Sprite* createMaskedSprite(cocos2d::Node* tempSrc,std::string kFileName, const char* maskFile) 
	{ 
		return NULL;
	}

	void addSelectBoxCB( cocos2d::Node* src,std::string kName,std::function<void(Node* , bool)> cb )
	{
		SelectBox *pSelectBox = dynamic_cast<SelectBox*>(getChildWidgetByName(src,kName));
		pSelectBox->addCallBack(cb);
	}

	void addSelectBoxCB( cocos2d::Node* src,std::string kName,std::function<void(cocos2d::Node* , bool)> cb,int tag )
	{
		SelectBox *pSelectBox = dynamic_cast<SelectBox*>(getChildWidgetByName(src,kName));
		pSelectBox->addCallBack(cb);
		pSelectBox->setTag(tag);
	}

	void setSelectStatus( cocos2d::Node* src,std::string kName,bool _select )
	{
		SelectBox *pSelectBox = dynamic_cast<SelectBox*>(getChildWidgetByName(src,kName));
		pSelectBox->setSelectStatus(_select);
	}

	void FadeIn(cocos2d::Node* pNode,float fWaiteTime,float fTime)
	{
		pNode->setOpacity(0);
		if (fWaiteTime > 0)
		{
			CCFiniteTimeAction*  action1 = cocos2d::CCSequence::create(
				cocos2d::DelayTime::create(fWaiteTime),
				cocos2d::FadeIn::create(fTime),NULL
				);
			pNode->runAction(action1);
		}
		else
		{
			pNode->runAction(cocos2d::FadeIn::create(fTime));
		}
	}
	void WaiteCall(cocos2d::Node* pNode,float fWaiteTime,FiniteTimeAction* pAction)
	{
		CCFiniteTimeAction*  action1 = cocos2d::CCSequence::create(
			cocos2d::DelayTime::create(fWaiteTime),
			pAction,
			NULL
			);
		pNode->runAction(action1);
	}
	void ScaleTo(cocos2d::Node* pNode,float fWaiteTime,float fTime,float fFromScale,float fToScale)
	{
		pNode->setScale(fFromScale);
		if (fWaiteTime > 0)
		{
			CCFiniteTimeAction*  action1 = cocos2d::CCSequence::create(
				cocos2d::DelayTime::create(fWaiteTime),
				cocos2d::ScaleTo::create(fTime,fToScale,fToScale)
				,NULL
				);
			pNode->runAction(action1);
		}
		else
		{
			pNode->runAction(cocos2d::ScaleTo::create(fTime,fToScale,fToScale));
		}
	}
	void CallFuncN(cocos2d::Node* pNode,float fWaiteTime,Ref* selectorTarget,SEL_CallFuncN selector,int iTag)
	{
		CCFiniteTimeAction*  action2 = cocos2d::CCSequence::create(
			cocos2d::DelayTime::create(fWaiteTime),
			CCCallFuncN::create(selectorTarget, selector),
			NULL);
		action2->setTag(iTag);
		pNode->runAction(action2);
	}

	void useMaskSprite(cocos2d::Node* pNode,std::string kName)
	{
		MaskSprite *pMaskSprite = dynamic_cast<MaskSprite*>(pNode);
		pMaskSprite->useMaskTexture(kName);
	}
	void useMaskSprite(cocos2d::Node* pNode,std::string kChildName,std::string kName)
	{
		useMaskSprite(getChildWidget(pNode,kChildName),kName);
	}
	void useMaskSpriteAll(cocos2d::Node* pNode,std::string kName)
	{
		MaskSprite *pMaskSprite = dynamic_cast<MaskSprite*>(pNode);
		if (pMaskSprite)
		{
			pMaskSprite->useMaskTexture(kName);
		}
		for (int i = 0;i<pNode->getChildrenCount();i++)
		{
			useMaskSpriteAll(pNode->getChildren().at(i),kName);
		}
	}
	void runWidgetAction(cocos2d::Node* pNode,std::string kName,std::string kAction)
	{
		runWidgetAction(WidgetFun::getChildWidget(pNode,kName),kAction);
	}
	void runWidgetAction(cocos2d::Node* pNode,std::string kAction)
	{
		pNode->stopAllActions();
		WidgetManager::Instance().createActionByName(pNode,kAction);
	}
	void runAnim(cocos2d::Node* pNode,std::string kName,std::string kAnim)
	{
		runAnim(WidgetFun::getChildWidget(pNode,kName),kAnim);
	}
	void runAnim(cocos2d::Node* pNode,std::string kAnim)
	{
		AnimationWidget *pAnim = dynamic_cast<AnimationWidget*>(pNode);
		CCAssert(pAnim,"");
		if (pAnim)
		{
			pAnim->runFrames(kAnim);
		}
	}

	void set9Size(cocos2d::Node* pNode,cocos2d::Size kSize)
	{
		Scale9SpriteEx *pScale9 = dynamic_cast<Scale9SpriteEx*>(pNode);
		pScale9->setPreferredSize(kSize);
	}
	void set9Size(cocos2d::Node* pNode,std::string kName,cocos2d::Size kSize)
	{
		set9Size(WidgetFun::getChildWidget(pNode,kName),kSize);
	}
	void setVisibleByTime(cocos2d::Node* pNode,float fTime,bool bVisible)
	{
		cocos2d::CCVisibleAction* pAction = cocos2d::CCVisibleAction::create(fTime,bVisible);
		pAction->setTag(ActionTag::VisibleAction);
		pNode->stopActionByTag(ActionTag::VisibleAction);
		pNode->runAction(pAction);
	}
	void setVisibleByTime(cocos2d::Node* pNode,std::string kName,float fTime,bool bVisible)
	{
		setVisibleByTime(WidgetFun::getChildWidget(pNode,kName),fTime,bVisible);
	}
	void setButtonImagic(cocos2d::Node* pNode,std::string kFile,int bAutoPress)
	{
		ButtonEx *pButton = dynamic_cast<ButtonEx*>(pNode);
		Size sz = pButton->getContentSize();
		pButton->loadTextureNormal(kFile);
		if (bAutoPress)
		{
			pButton->loadTexturePressed(kFile);
			pButton->setPressColor(ccc3(150,150,150));
		}
		float fx= pButton->getScaleX()*sz.width;
		float fy = pButton->getScaleY()*sz.height;
		utility::setSpriteScaleBySize(pButton,CCSizeMake(fx,fy));
	}
	void setButtonImagic(cocos2d::Node* pNode,std::string kName,std::string kFile,int bAutoPress)
	{
		setButtonImagic(WidgetFun::getChildWidget(pNode,kName),kFile,bAutoPress);
	}
	void setButtonTxt(cocos2d::Node* pNode,std::string kTxt)
	{
		ButtonEx *pButton = dynamic_cast<ButtonEx*>(pNode);
		pButton->setTitleText(kTxt);
	}
	void setButtonTxt(cocos2d::Node* pNode,std::string kName,std::string kTxt)
	{
		setButtonTxt(WidgetFun::getChildWidget(pNode,kName),kTxt);
	}
	std::string getButtonTxt(cocos2d::Node* pNode)
	{
		ButtonEx *pButton = dynamic_cast<ButtonEx*>(pNode);
		return pButton->getTitleText();
	}
	std::string getButtonTxt(cocos2d::Node* pNode,std::string kName)
	{
		return getButtonTxt(WidgetFun::getChildWidget(pNode,kName));
	}

	void setButtonEnabled(cocos2d::Node* pNode,bool bEnble)
	{
		ButtonEx *pButton = dynamic_cast<ButtonEx*>(pNode);
		pButton->setEnabled(bEnble);
	}
	void setButtonEnabled(cocos2d::Node* pNode,const std::string& kName,bool bEnble)
	{
		setButtonEnabled(getChildWidget(pNode,kName),bEnble);
	}
	cocos2d::Point getWidgetPoint(cocos2d::Node* pNode)
	{
		return pNode->getPosition();
	}
	cocos2d::Point getWidgetPoint(cocos2d::Node* pNode,std::string kName)
	{
		return getWidgetPoint(WidgetFun::getChildWidget(pNode,kName));
	}
	void CallAction(cocos2d::Node* pNode,std::string kActionName)
	{
		WidgetManager::Instance().createActionByName(pNode,kActionName);
	}
	void CallAction(cocos2d::Node* pNode,std::string kName,std::string kActionName)
	{
		WidgetManager::Instance().createActionByName(WidgetFun::getChildWidget(pNode,kName),kActionName);
	}
	void ActionStart(cocos2d::Node* pNode)
	{
		CallAction(pNode,"ActionStart");
	}
	void ActionStart(cocos2d::Node* pNode,std::string kName)
	{
		CallAction(pNode,kName,"ActionStart");
	}

	void ActionEnd(cocos2d::Node* pNode)
	{
		CallAction(pNode,"ActionEnd");
	}
	void ActionEnd(cocos2d::Node* pNode,std::string kName)
	{
		CallAction(pNode,kName,"ActionEnd");
	}
	void setPlaceTextKey(cocos2d::Node* pNode,std::string kKey,std::string kPlaceTxt)
	{
		std::string kTxt = WidgetFun::getWidgetUserInfo(pNode,kKey);
		utility::StringReplace(kTxt,"#",kPlaceTxt);
		WidgetFun::setText(pNode,kTxt);
	}
	void setPlaceTextKey(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kPlaceTxt)
	{
		setPlaceTextKey(WidgetFun::getChildWidget(pNode,kName),kKey,kPlaceTxt);
	}
	void setPlaceTextKey2(cocos2d::Node* pNode,std::string kKey,std::string kPlaceTxt1,std::string kPlaceTxt2)
	{
		std::string kTxt = WidgetFun::getWidgetUserInfo(pNode,kKey);
		utility::StringReplace(kTxt,"#",kPlaceTxt1);
		utility::StringReplace(kTxt,"#",kPlaceTxt2);
		WidgetFun::setText(pNode,kTxt);
	}
	void setPlaceTextKey2(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kPlaceTxt1,std::string kPlaceTxt2)
	{
		setPlaceTextKey2(WidgetFun::getChildWidget(pNode,kName),kKey,kPlaceTxt1,kPlaceTxt2);
	}
	void setPlaceText(cocos2d::Node* pNode,std::string kPlaceTxt)
	{
		setPlaceTextKey(pNode,"Txt",kPlaceTxt);
	}
	void setPlaceText(cocos2d::Node* pNode,std::string kName,std::string kPlaceTxt)
	{
		setPlaceTextKey(pNode,kName,"Txt",kPlaceTxt);
	}
	void setPlaceImagicKey(cocos2d::Node* pNode,std::string kKey)
	{
		std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,kKey);
		WidgetFun::setImagic(pNode,kImagic,false);
	}
	void setPlaceImagicKey(cocos2d::Node* pNode,std::string kName,std::string kKey)
	{
		setPlaceImagicKey(WidgetFun::getChildWidget(pNode,kName),kKey);
	}
}
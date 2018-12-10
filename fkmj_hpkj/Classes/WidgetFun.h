#pragma once
#include "cocos2d.h"
#include "WidgetDefine.h"
#include "ListViewEx.h"
#include "utility.h"

namespace WidgetFun
{
	template<class T>
	static T* castNode(cocos2d::Node* pNode)
	{
		T* pChangeNode = dynamic_cast<T*>(pNode);
		CCAssert(pChangeNode,"castNode");
		return pChangeNode;
	}
	template<class T>
	static T* castWidget(WidgetInfo* pInfo)
	{
		T* pWidget = dynamic_cast<T*>(pInfo);
		CCAssert(pWidget,"castWidget");
		return pWidget;
	}

	bool isSpriteTouched(cocos2d::Sprite* pSprite,cocos2d::Vec2 kWorldPos);
	WidgeTextureInfo paseTextureInfo(std::string kValue);
	cocos2d::Sprite* createSprite(const WidgeTextureInfo& kInfo);
	cocos2d::SpriteFrame* getFrame( const std::string& kFileName );
	cocos2d::Sprite* createSprite(const char * kFileName );
	cocos2d::MenuItemImage* createMenu(std::string normalImage,std::string selectedImage,std::string disabledImage);
	void setTexture(cocos2d::Sprite* pSprite,const WidgeTextureInfo& kInfo);
	cocos2d::Node* getChildWidget(cocos2d::Node* pNode,std::string kName);
	cocos2d::Node* getChildWidgetByName(cocos2d::Node* pNode,std::string kName);
	cocos2d::Node* getChildWidgetSkin(cocos2d::Node* pNode,std::string kScencName,std::string kSkinName);
	WidgetUserInfo* getUserInfo(cocos2d::Node* pNode);
	void setWidgetUserInfo(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kValue);
	void setWidgetUserInfo(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	std::string getUserInfoValue(WidgetUserInfo* pUserInfo,std::string kKey);
	int getUserInfoInt(WidgetUserInfo* pUserInfo,std::string kKey); 
	std::string getWidgetUserInfo(cocos2d::Node* pNode,std::string kName,std::string kKey);
	std::string getWidgetUserInfo(cocos2d::Node* pNode,std::string kKey);
	void setWidgetProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void setWidgetProperty(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kValue);

	void setPos(cocos2d::Node* pNode,cocos2d::Vec2 kPoint);
	void setPos(cocos2d::Node* pNode,std::string kName,cocos2d::Vec2 kPoint);
	cocos2d::Vec2 getPos(cocos2d::Node* pNode,std::string kName);

	void setTextStr(cocos2d::Node* pNode,std::string kTxt); 
	void setTextStr(cocos2d::Node* pNode,std::string kName,std::string kTxt);

	template<class T>
	void setText(cocos2d::Node* pNode,T kTxt)
	{
		setTextStr(pNode,utility::toString(kTxt));
	}
	template<class T>
	void setText(cocos2d::Node* pNode,std::string kName,T kTxt)
	{
		setTextStr(pNode,kName,utility::toString(kTxt));
	}

	void setTextColor(cocos2d::Node* pNode,std::string kTxt);
	void setTextColor(cocos2d::Node* pNode,std::string kName,std::string kTxt);
	std::string getText(cocos2d::Node* pNode,std::string kName);
	void setTextColor(cocos2d::Node* pNode, std::string kName,const cocos2d::Color3B& color );

	void setColorText(cocos2d::Node* pNode,std::string kTxt);
	void setColorText(cocos2d::Node* pNode,std::string kName,std::string kTxt);

	void setColorIdexTxt(cocos2d::Node* pNode,int Idex,std::string kTxt);
	void setColorIdexTxt(cocos2d::Node* pNode,int Idex,std::string kName,std::string kTxt);

	void setEditeText(cocos2d::Node* pNode,std::string kTxt);
	void setEditeText(cocos2d::Node* pNode,std::string kName,std::string kTxt);
	std::string getEditeText(cocos2d::Node* pNode,std::string kName);
	void SetEditReturnAction(cocos2d::Node* pNode,std::function<void()> pFun);
	void SetEditReturnAction(cocos2d::Node* pNode,std::string kName,std::function<void()> pFun);

	void setImagic(cocos2d::Node* pNode,std::string kImagic,int bSameSize);
	void setImagic(cocos2d::Node* pNode,std::string kName,std::string kImagic,bool bSameSize = true);
	void setImagicAple(cocos2d::Node* pNode,float fAple);
	void setImagicAple(cocos2d::Node* pNode,std::string kName,float fAple);
	void setImagicWithMask(cocos2d::Node* pNode,std::string kName,std::string kImagic,std::string kMask);
	void addImagicWithMask(cocos2d::Node* pNode,std::string kName,std::string kMask);
	
	void setImagicPercent(cocos2d::Node* pNode,float fPercent);
	void setImagicPercent(cocos2d::Node* pNode,std::string kName,float fPercent);

	void setFace(cocos2d::Node* pNode,std::string kFace);
	void setFace(cocos2d::Node* pNode,std::string kName,std::string kFace);

	void setVisible(cocos2d::Node* pNode,bool bVisible);
	void setVisible(cocos2d::Node* pNode,std::string kName,bool bVisible);
	bool isWidgetVisble(cocos2d::Node* pNode,std::string kName);

	void setName(cocos2d::Node* pNode,std::string kValue);
	void setName(cocos2d::Node* pNode,std::string kName,std::string kValue);

	void setAtlasTxt(cocos2d::Node* pNode,std::string kTxt);
	void setAtlasTxt(cocos2d::Node* pNode,std::string kName,std::string kTxt);
	void setAtlasActionTxt(cocos2d::Node* pNode,int iValue);
	void setAtlasActionTxt(cocos2d::Node* pNode,std::string kName,int iValue);
	int  getAtlasActionTxt(cocos2d::Node* pNode);
	int  getAtlasActionTxt(cocos2d::Node* pNode,std::string kName);

	void setEnable(cocos2d::Node* pNode,bool bEnable);
	void setEnable(cocos2d::Node* pNode,std::string kName,bool bEnable);

	void setChecked(cocos2d::Node* pNode,bool bChecked);
	void setChecked(cocos2d::Node* pNode,std::string kName,bool bChecked);

	bool isChecked(cocos2d::Node* pNode);
	bool isChecked(cocos2d::Node* pNode,std::string kName);

	void setLabelKeepLeft(cocos2d::Node* pNode,std::string kName);
	void setNodeKeepAfter(cocos2d::Node* pNode,cocos2d::Node* leftNode,std::string kName);


	void setTextFieldTxt(cocos2d::Node* pNode,std::string kName,std::string kTxt);
	std::string getTextFieldTxt(cocos2d::Node* pNode,std::string kName);

	void setSelectBoxTag(cocos2d::Node* pNode,std::string kName,int nTag);
	void setProgress(cocos2d::Node* pNode,std::string kName,float fCurrent,float fMax);

	void AnimStart(cocos2d::Node* pNode,std::string kName);
	void AnimStop(cocos2d::Node* pNode,std::string kName);

	cocos2d::ListViewEx* getListViewWidget(cocos2d::Node* pNode,std::string kName);

	float getWidgetScale(cocos2d::Node* pNode,std::string kName);
	void setWidgetScale(cocos2d::Node* pNode,std::string kName,float fScale);

	cocos2d::Size getWidgetSize(cocos2d::Node* pNode,std::string kName );

	void setSliderMenuActValue( cocos2d::Node* pNode,std::string kName,float fValue );

	cocos2d::Sprite* createMaskedSprite(cocos2d::Node* src,std::string kFileName) ;
	cocos2d::Sprite* createMaskedSprite(cocos2d::Node* src,std::string kFileName, const char* maskFile) ;

	void addSelectBoxCB(cocos2d::Node* src,std::string kName,std::function<void(cocos2d::Node* , bool)>);
	void addSelectBoxCB(cocos2d::Node* src,std::string kName,std::function<void(cocos2d::Node* , bool)>,int tag);
	void setSelectStatus(cocos2d::Node* src,std::string kName,bool _select);

	void FadeIn(cocos2d::Node* pNode,float fWaiteTime,float fTime);
	void ScaleTo(cocos2d::Node* pNode,float fWaiteTime,float fTime,float fFromScale,float fToScale);
	void WaiteCall(cocos2d::Node* pNode,float fWaiteTime,cocos2d::FiniteTimeAction* pAction);

	void CallFuncN(cocos2d::Node* pNode,float fWaiteTime,cocos2d::Ref* selectorTarget,cocos2d::SEL_CallFuncN selector,int iTag = 0);

	void useMaskSprite(cocos2d::Node* pNode,std::string kName);
	void useMaskSprite(cocos2d::Node* pNode,std::string kChildName,std::string kName);
	void useMaskSpriteAll(cocos2d::Node* pNode,std::string kName);

	void runWidgetAction(cocos2d::Node* pNode,std::string kName,std::string kAction);
	void runWidgetAction(cocos2d::Node* pNode,std::string kAction);

	void runAnim(cocos2d::Node* pNode,std::string kName,std::string kAnim);
	void runAnim(cocos2d::Node* pNode,std::string kAnim);

	void set9Size(cocos2d::Node* pNode,cocos2d::Size kSize);
	void set9Size(cocos2d::Node* pNode,std::string kName,cocos2d::Size kSize);

	void setVisibleByTime(cocos2d::Node* pNode,float fTime,bool bVisible);
	void setVisibleByTime(cocos2d::Node* pNode,std::string kName,float fTime,bool bVisible);

	void setButtonImagic(cocos2d::Node* pNode,std::string kFile,int bAutoPress);
	void setButtonImagic(cocos2d::Node* pNode,std::string kName,std::string kFile,int bAutoPress);

	void setButtonTxt(cocos2d::Node* pNode,std::string kTxt);
	void setButtonTxt(cocos2d::Node* pNode,std::string kName,std::string kTxt);

	std::string getButtonTxt(cocos2d::Node* pNode);
	std::string getButtonTxt(cocos2d::Node* pNode,std::string kName);
	
	void setButtonEnabled(cocos2d::Node* pNode,bool bEnble);
	void setButtonEnabled(cocos2d::Node* pNode,const std::string& kName,bool bEnble);

	cocos2d::Point getWidgetPoint(cocos2d::Node* pNode);
	cocos2d::Point getWidgetPoint(cocos2d::Node* pNode,std::string kName);

	void CallAction(cocos2d::Node* pNode,std::string kActionName);
	void CallAction(cocos2d::Node* pNode,std::string kName,std::string kActionName);

	void ActionStart(cocos2d::Node* pNode);
	void ActionStart(cocos2d::Node* pNode,std::string kName);

	void ActionEnd(cocos2d::Node* pNode);
	void ActionEnd(cocos2d::Node* pNode,std::string kName);

	void setPlaceTextKey(cocos2d::Node* pNode,std::string kKey,std::string kPlaceTxt);
	void setPlaceTextKey(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kPlaceTxt);

	void setPlaceTextKey2(cocos2d::Node* pNode,std::string kKey,std::string kPlaceTxt1,std::string kPlaceTxt2);
	void setPlaceTextKey2(cocos2d::Node* pNode,std::string kName,std::string kKey,std::string kPlaceTxt1,std::string kPlaceTxt2);

	void setPlaceText(cocos2d::Node* pNode,std::string kPlaceTxt);
	void setPlaceText(cocos2d::Node* pNode,std::string kName,std::string kPlaceTxt);

	void setPlaceImagicKey(cocos2d::Node* pNode,std::string kKey);
	void setPlaceImagicKey(cocos2d::Node* pNode,std::string kName,std::string kKey);
};

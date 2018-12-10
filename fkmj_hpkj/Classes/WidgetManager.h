#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "ui/UIWidget.h"
#include "ui/UICheckBox.h"

class WidgetManager
	:public FvSingleton<WidgetManager>
	,cocos2d::Ref
{
public:
	WidgetManager();
	~WidgetManager();
public:
	typedef void (WidgetManager::*WidgetManager_Handler)(cocos2d::Node* pNode,std::string kKey,std::string kValue);
public:
	WidgetInfo* getWidgetInfo(std::string kName,bool bAssert = true);
public:
	static ButtonCallBackList s_kCBInfoListRelease;
	static ButtonCallBackList s_kCBInfoListBegin;
public:

	static void addButtonCB(std::string kWidgetName,cocos2d::Ref* pScence,CB_BUTTON_Handler pCallBack);
	static void addButtonCBBegin(std::string kWidgetName,cocos2d::Ref* pScence,CB_BUTTON_Handler pCallBack);
	static void DelBySecnce(cocos2d::Node* pScence);

public:
	void Init();

	void addWidgetInfo(WidgetInfo* pInfo);
	void clearWidgetSkin();
	void clearWidgetSkin(WidgetInfo* pInfo);
	void setBakListVisible(cocos2d::Node* pButtonList,bool bVisible,bool bDirect);
	cocos2d::Node* getBaseNode(cocos2d::Node* pNode);
	void ButtonAction(cocos2d::Node* pBase,cocos2d::Node* pMenu,std::string kKey,std::string kValue);
	void ButtonAction(cocos2d::Node* pMenu,WidgetUserInfo* pUserInfo,bool bNeedSound = true);
	void ButtonRelease( cocos2d::Ref* pObject);
	void ButtonReleaseEx( cocos2d::Ref* pObject,cocos2d::ui::Widget::TouchEventType _type);
	void CheckButtonReleaseEx( cocos2d::Ref* pObject,cocos2d::ui::CheckBoxEventType _type);
	void SilderValue(cocos2d::Ref* pObject);

	std::vector<std::string>& getOnlyVaildButton();
public:
	cocos2d::Node* createWidget(std::string kName,cocos2d::Node* pRootNode);
	cocos2d::Node* createWidget(WidgetInfo* pWidgetInfo,cocos2d::Node* pRootNode);
	cocos2d::Node* createWidgetImagic(WidgetImagic* pInfo);
	cocos2d::Node* createWidgetTxt(WidgetTxt* pInfo);
	cocos2d::Node* createWidgetColorTxt(WidgetColorTxt* pInfo);
	cocos2d::Node* createWidgetTxtAtlas(WidgetTxtAtlas* pInfo);
	cocos2d::Node* createWidgetEdit(WidgetEdit* pInfo);
	cocos2d::Node* createWidgetButton(WidgetButton* pInfo);
	cocos2d::Node* createWidgetCheckBox(WidgetCheckBox* pInfo);
	cocos2d::Node* createWidgetSider(WidgetSider* pInfo);
	cocos2d::Node* createWidgetNode(WidgetNode* pInfo);
	cocos2d::Node* createWidgetProgressBar(WidgetProgressBar* pInfo);
	cocos2d::Node* createWidgetProgressTimer(WidgetProgressTimer* pInfo);
	cocos2d::Node* createWidget9Sprite(Widget9Sprite* pInfo);
	cocos2d::Node* createWidgetList(WidgetList* pInfo);
	cocos2d::Node* createWidgetRichText(WidgetRichText* pInfo);
	cocos2d::Node* createWidgetAnimation(WidgetAnimation* pInfo);
	cocos2d::Node* createWidgetPanle(WidgetPanle* pInfo);
	cocos2d::Node* createWidgetSwitch_Button(WidgetSwitch_Button* pInfo);
	cocos2d::Node* createWidgetSelectBox(WidgetSelectBox* pInfo);
	cocos2d::Node* createWidgetLayout( WidgetLayout* pInfo );
	cocos2d::Node* createWidgetNodeUI(WidgetNodeUI* pInfo);
	cocos2d::Node* createWidgetClipper(WidgetClipper* pInfo);
	
	void changeImagicProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeTxtProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeColorTxtProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeTxtAtlasProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeEditProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeButtonProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeCheckBoxProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeSiderProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeNodeProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeProgressBarProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeProgressTimerProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void change9SpriteProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeListProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeRichTextProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeAnimationProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changePanleProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeSwitch_ButtonProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeSelectBoxProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeLayoutProperty( cocos2d::Node* pNode,std::string kKey,std::string kValue );
	void changeNodeUIProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
	void changeClipperProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);

	void changePropertyByType(cocos2d::Node* pNode,std::string kType,std::string kKey,std::string kValue);
	void changeProperty(cocos2d::Node* pNode,stringKeyValueList& kProperty,WidgetManager_Handler pHander);
	void changeProperty(cocos2d::Node* pNode,std::string kKey,std::string kValue);
public:
	void CreateAction(cocos2d::Node* pNode);
	void NextCreateAction(cocos2d::Node* pNode,void* pAction);
	void CreateAction(cocos2d::Node* pNode,QYActionInfo* pAction);
	void createActionByName(cocos2d::Node* pNode,std::string kActionName);
	QYActionInfo* getAction(cocos2d::Node* pNode,std::string kActionName);
	QYActionInfo* getAction(QYActionInfo* pAction,std::string kActionName);
	cocos2d::Node* getNodeByTag(cocos2d::Node* pNode,int iTag);
private:
	void CB_QYActionSetPos(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetSize(cocos2d::Node* pNode,void* data);
	void CB_QYActionRemoveSelf(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetScale(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetAple(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetVisible(cocos2d::Node* pNode,void* data);
	void CB_QYActionStopAllAction(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetProgress(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetRota(cocos2d::Node* pNode,void* data);
	void CB_QYActionCallAction(cocos2d::Node* pNode,void* data);
	void CB_QYActionCallButton(cocos2d::Node* pNode,void* data);
	void CB_QYActionCreateWidget(cocos2d::Node* pNode,void* data);
	void CB_QYActionRunAnim(cocos2d::Node* pNode,void* data);
	void CB_QYActionPlaySound(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetTxt(cocos2d::Node* pNode,void* data);
	void CB_QYActionSetImge(cocos2d::Node* pNode,void* data);
	cocos2d::FiniteTimeAction* OnCreateAction(cocos2d::Node* pNode,QYActionInfo* pAction);
	
private:
	WidgetInfoListP m_kWidgets;
	ButtonCallBackList kButtonCallList;

	std::vector<std::string> m_kOnlyVaildButton;
};

#include "WidgetManager.h"
#include "utility.h"
#include "WidgetFun.h"
#include "QYActionDefine.h"
#include "ActionEx.h"
#include "SoundFun.h"

void WidgetManager::CreateAction(cocos2d::Node* pNode)
{
	CCASSERT(pNode,"");
	if (!pNode)
	{
		return;
	}
	WidgetUserInfo* pUserInfo = WidgetFun::getUserInfo(pNode);
	if (!pUserInfo)
	{
		return;
	}
	if (!pUserInfo->pWidgetInfo)
	{
		return;
	}
	std::vector<QYActionInfo*>& kActions = pUserInfo->pWidgetInfo->kQYAction;
	for (int i = 0;i<(int)kActions.size();i++)
	{
		if (kActions[i]->bSaveAction)
		{
			continue;
		}
		CreateAction(pNode,kActions[i]);
	}
}
void WidgetManager::NextCreateAction(cocos2d::Node* pNode,void* pAction)
{
	QYActionInfo* pTempAction = (QYActionInfo*)pAction;
	for (int i = 0;i<(int)pTempAction->kNextAction.size();i++)
	{
		CreateAction(pNode,pTempAction->kNextAction[i]);
	}
}
void WidgetManager::CreateAction(cocos2d::Node* pNode,QYActionInfo* pAction)
{
	CCASSERT(pNode,"");
	if (pAction->kType == QYActionInfo::getType())
	{
		NextCreateAction(pNode,pAction);
	}
	else
	{
		cocos2d::Sequence* pSequence = cocos2d::Sequence::create(OnCreateAction(pNode,pAction),
			cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::NextCreateAction),pAction)
			,NULL);
		pNode->runAction(pSequence);
	}
}
void WidgetManager::createActionByName(cocos2d::Node* pNode,std::string kActionName)
{
	QYActionInfo* pTemp = getAction(pNode,kActionName);
	if (pTemp)
	{
		CreateAction(pNode,pTemp);
	}
	else
	{
		CCAssert(false,"");
	}
}
QYActionInfo* WidgetManager::getAction(cocos2d::Node* pNode,std::string kActionName)
{
	CCASSERT(pNode,"");
	if (!pNode)
	{
		return NULL;
	}
	WidgetUserInfo* pUserInfo = WidgetFun::getUserInfo(pNode);
	if (!pUserInfo)
	{
		return NULL;
	}
	if (!pUserInfo->pWidgetInfo)
	{
		return NULL;
	}
	std::vector<QYActionInfo*>& kActions = pUserInfo->pWidgetInfo->kQYAction;
	for (int i = 0;i<(int)kActions.size();i++)
	{
		QYActionInfo* pTemp = getAction(kActions[i],kActionName);
		if (pTemp)
		{
			return pTemp;
		}
	}
	return NULL;
}
QYActionInfo* WidgetManager::getAction(QYActionInfo* pAction,std::string kActionName)
{
	if (!pAction)
	{
		return NULL;
	}
	if (pAction->kName == kActionName)
	{
		return pAction;
	}
	for (int i = 0;i<(int)pAction->kNextAction.size();i++)
	{
		QYActionInfo* pTemp = getAction(pAction->kNextAction[i],kActionName);
		if (pTemp)
		{
			return pTemp;
		}
	}
	return NULL;
}
cocos2d::Node* WidgetManager::getNodeByTag(cocos2d::Node* pNode,int iTag)
{
	if (!pNode)
	{
		return NULL;
	}
	if (pNode->getTag() == iTag)
	{
		return pNode;
	}
	return getNodeByTag(pNode->getParent(),iTag);
}
void WidgetManager::CB_QYActionSetPos(cocos2d::Node* pNode,void* data)
{
	QYActionSetPos* pTempAction = (QYActionSetPos*)data;
	pNode->setPosition(utility::getRandPos(pTempAction->kDestPos,pNode));
}
void WidgetManager::CB_QYActionSetSize(cocos2d::Node* pNode,void* data)
{
	QYActionSetSize* pTempAction = (QYActionSetSize*)data;
	cocos2d::Vec2 kSize = utility::getRandPos(pTempAction->kDestSize,pNode);
	cocos2d::Size kTempSize = cocos2d::Size(kSize.x,kSize.y);
	pNode->setContentSize(kTempSize);
}
void WidgetManager::CB_QYActionRemoveSelf(cocos2d::Node* pNode,void* data)
{
	QYActionRemoveSelf* pTempAction = (QYActionRemoveSelf*)data;
	if (pTempAction->iParentTag)
	{
		cocos2d::Node* pTempNode = getNodeByTag(pNode,pTempAction->iParentTag);
		if (pTempNode)
		{
			pTempNode->removeFromParent();
		}
		else
		{
			CCASSERT(false,"");
		}
	}
	else
	{
		pNode->removeFromParent();
	}
}
void WidgetManager::CB_QYActionSetScale(cocos2d::Node* pNode,void* data)
{
	QYActionSetScale* pTempAction = (QYActionSetScale*)data;
	pNode->setScale(utility::getRandFloat(pTempAction->fDestScale,pNode));
}
void WidgetManager::CB_QYActionSetAple(cocos2d::Node* pNode,void* data)
{
	QYActionSetAple* pTempAction = (QYActionSetAple*)data;
	pNode->setOpacity(utility::getRandFloat(pTempAction->fDestAple,pNode)*255);
}
void WidgetManager::CB_QYActionSetVisible(cocos2d::Node* pNode,void* data)
{
	QYActionSetVisible* pTempAction = (QYActionSetVisible*)data;
	pNode->setVisible(pTempAction->bVisible);
}
void WidgetManager::CB_QYActionStopAllAction(cocos2d::Node* pNode,void* data)
{
	pNode->stopAllActions();
}
void WidgetManager::CB_QYActionSetProgress(cocos2d::Node* pNode,void* data)
{
	QYActionSetProgress* pTempAction = (QYActionSetProgress*)data;
	cocos2d::CCProgressTimer* pProgress = dynamic_cast<cocos2d::CCProgressTimer*>(pNode);
	if (!pProgress)
	{
		CCASSERT(false,"");
		return;
	}
	pProgress->setPercentage(utility::getRandFloat(pTempAction->fPercent,pNode));
}
void WidgetManager::CB_QYActionSetRota(cocos2d::Node* pNode,void* data)
{
	QYActionSetRota* pTempAction = (QYActionSetRota*)data;
	pNode->setRotation(utility::getRandFloat(pTempAction->fDestRota,pNode));
}
void WidgetManager::CB_QYActionCallAction(cocos2d::Node* pNode,void* data)
{
	QYActionCallAction* pTempAction = (QYActionCallAction*)data;
	cocos2d::Node* pTempNode = pNode;
	if (pTempAction->kOtherNodeName != "")
	{
		pTempNode = WidgetFun::getChildWidget(pNode,pTempAction->kOtherNodeName);
	}
	if (!pTempNode)
	{
		CCAssert(false,"");
		return;
	}
	QYActionInfo* pGetAction = NULL;
	if (pTempAction->kSelfActionName != "")
	{
		pGetAction = getAction(pNode,pTempAction->kSelfActionName);
	}
	if (pTempAction->kOtherActionName != "")
	{
		pGetAction = getAction(pTempNode,pTempAction->kOtherActionName);
	}
	if (pTempAction->kActionParentName != "")
	{
		QYActionInfo* pParent = pTempAction->pParent;
		while (pParent)
		{
			if (pTempAction->kActionParentName == pParent->kName)
			{
				pGetAction = pParent;
				break;
			}
			pParent = pParent->pParent;
		}
	}
	CCASSERT(pGetAction,pTempAction->kSelfActionName.c_str());
	if(pGetAction)
	{
		CreateAction(pTempNode,pGetAction);
	}
}
void WidgetManager::CB_QYActionCallButton(cocos2d::Node* pNode,void* data)
{
	QYActionCallButton* pTempAction = (QYActionCallButton*)data;

	for (int i = 0;i<(int)s_kCBInfoListRelease.size();i++)
	{
		Button_CBInfo& kInfo = s_kCBInfoListRelease[i];
		if (kInfo.kCommand == pTempAction->kButtonName)
		{
			(kInfo.pObject->*kInfo.pCallBack)(pNode,NULL);
		}
	}
}
void WidgetManager::CB_QYActionCreateWidget(cocos2d::Node* pNode,void* data)
{
	QYActionCreateWidget* pTempAction = (QYActionCreateWidget*)data;
	if (pTempAction->bRootNode)
	{
		cocos2d::Node* pTempNode = createWidget(pTempAction->kWidgetName,pNode->getParent());
		cocos2d::Vec2 kOffect = utility::getOffectParentPos(pNode,pTempAction->kOffectPos);
		pTempNode->setPosition(pNode->getPosition()+kOffect);
	}
	else
	{
		createWidget(pTempAction->kWidgetName,pNode);
	}
}
void WidgetManager::CB_QYActionRunAnim(cocos2d::Node* pNode,void* data)
{
	QYActionRunAnim* pTempAction = (QYActionRunAnim*)data;
	WidgetFun::runAnim(pNode,pTempAction->kAnimName);
}
void WidgetManager::CB_QYActionPlaySound(cocos2d::Node* pNode,void* data)
{
	QYActionPlaySound* pTempAction = (QYActionPlaySound*)data;
	SoundFun::Instance().playEffect(pTempAction->kSoundName);
}
void WidgetManager::CB_QYActionSetTxt(cocos2d::Node* pNode,void* data)
{
	QYActionSetTxt* pTempAction = (QYActionSetTxt*)data;
	WidgetFun::setText(pNode,pTempAction->kTxt.c_str());
}

void WidgetManager::CB_QYActionSetImge( cocos2d::Node* pNode,void* data )
{
	QYActionSetImage* pTempAction = (QYActionSetImage*)data;
	WidgetFun::setImagic(pNode,utility::getUserString(pTempAction->kImageFile,pNode),true);
}
cocos2d::FiniteTimeAction* WidgetManager::OnCreateAction(cocos2d::Node* pNode,QYActionInfo* pAction)
{
	cocos2d::FiniteTimeAction* pActionInterval = NULL;
	if (pAction->kType == QYActionWaiteTime::getType())
	{
		QYActionWaiteTime* pTempAction = (QYActionWaiteTime*)pAction;
		pActionInterval = cocos2d::CCDelayTime::create(utility::getRandFloat(pTempAction->fWaiteTime,pNode));
	}
	if (pAction->kType == QYActionSizeTo::getType())
	{
		QYActionSizeTo* pTempAction = (QYActionSizeTo*)pAction;
		cocos2d::Vec2 kSize = utility::getRandPos(pTempAction->kDestSize,pNode);
		cocos2d::Size kTempSize = cocos2d::Size(kSize.x,kSize.y);
		pActionInterval = cocos2d::SizeTo::create(utility::getRandFloat(pTempAction->fTime,pNode),kTempSize);
	}
	if (pAction->kType == QYActionSetSize::getType())
	{
		QYActionSetSize* pTempAction = (QYActionSetSize*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetSize),pAction);
	}
	if (pAction->kType == QYActionSetPos::getType())
	{
		QYActionSetPos* pTempAction = (QYActionSetPos*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetPos),pAction);
	}
	if (pAction->kType == QYActionRemoveSelf::getType())
	{
		QYActionRemoveSelf* pTempAction = (QYActionRemoveSelf*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionRemoveSelf),pAction);
	}
	if (pAction->kType == QYActionSetVisible::getType())
	{
		QYActionSetVisible* pTempAction = (QYActionSetVisible*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetVisible),pAction);
	}
	if (pAction->kType == QYActionStopAllAction::getType())
	{
		QYActionStopAllAction* pTempAction = (QYActionStopAllAction*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionStopAllAction),pAction);
	}
	if (pAction->kType == QYActionSetProgress::getType())
	{
		QYActionSetProgress* pTempAction = (QYActionSetProgress*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetProgress),pAction);
	}
	if (pAction->kType == QYActionProgressTo::getType())
	{
		QYActionProgressTo* pTempAction = (QYActionProgressTo*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fPercent = utility::getRandFloat(pTempAction->fPercent,pNode);
		pActionInterval = cocos2d::ProgressTo::create(fTime,fPercent);
	}
	if (pAction->kType == QYActionSplineTo::getType())
	{
		QYActionSplineTo* pTempAction = (QYActionSplineTo*)pAction;

		int iSize = (int)pTempAction->kPosition.size();
		auto array = cocos2d::PointArray::create(iSize);
		for (int i = 0;i<iSize;i++)
		{
			cocos2d::Vec2 kPos = utility::getRandPos(pTempAction->kPosition[i],pNode);
			array->addControlPoint(kPos);
		}
		float fTension = utility::getRandFloat(pTempAction->fTension,pNode);
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		pActionInterval = cocos2d::CardinalSplineTo::create(fTime, array, fTension);
	}
	if (pAction->kType == QYActionBizerTo::getType())
	{
		QYActionBizerTo* pTempAction = (QYActionBizerTo*)pAction;
		cocos2d::ccBezierConfig bezier;
		bezier.controlPoint_1 = pNode->getPosition();
		bezier.controlPoint_2 = utility::getRandPos(pTempAction->kPos1,pNode);
		bezier.endPosition = utility::getRandPos(pTempAction->kDestPos,pNode);
		
		pActionInterval = cocos2d::BezierTo::create(utility::getRandFloat(pTempAction->fTime,pNode), bezier);
	}
	if (pAction->kType == QYActionBizerBy::getType())
	{
		QYActionBizerBy* pTempAction = (QYActionBizerBy*)pAction;
		cocos2d::ccBezierConfig bezier;
		bezier.controlPoint_1 = cocos2d::Vec2(0,0);
		bezier.controlPoint_2 = utility::getRandPos(pTempAction->kPos1,pNode);
		bezier.endPosition = utility::getRandPos(pTempAction->kDestPos,pNode);
		if (bezier.controlPoint_2.x == 0)
		{
			bezier.controlPoint_2.x = bezier.endPosition.x/2;
		}
		pActionInterval = cocos2d::BezierBy::create(utility::getRandFloat(pTempAction->fTime,pNode), bezier);
	}
	
	if (pAction->kType == QYActionMoveTo::getType())
	{
		QYActionMoveTo* pTempAction = (QYActionMoveTo*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		cocos2d::Vec2 kDestPos = utility::getRandPos(pTempAction->kDestPos,pNode);
		if (pTempAction->bDestX)
		{
			kDestPos.x += pNode->getPositionX();
		}
		if (pTempAction->bDestY)
		{
			kDestPos.y += pNode->getPositionY();
		}
		pActionInterval = cocos2d::CCMoveTo::create(fTime,kDestPos);
	}
	if (pAction->kType == QYActionMoveBy::getType())
	{
		QYActionMoveBy* pTempAction = (QYActionMoveBy*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		cocos2d::Vec2 kDestPos = utility::getRandPos(pTempAction->kDestPos,pNode);
		pActionInterval = cocos2d::CCMoveBy::create(fTime,kDestPos);
	}
	if (pAction->kType == QYActionMoveToBySpd::getType())
	{
		QYActionMoveToBySpd* pTempAction = (QYActionMoveToBySpd*)pAction;
		float fMoveSpd = utility::getRandFloat(pTempAction->fMoveSpd,pNode);
		cocos2d::Vec2 kDestPos = utility::getRandPos(pTempAction->kDestPos,pNode);
		if (pTempAction->bDestX)
		{
			kDestPos.x += pNode->getPositionX();
		}
		if (pTempAction->bDestY)
		{
			kDestPos.y += pNode->getPositionY();
		}
		float fTime = utility::getMoveTime(pNode->getPosition(),kDestPos,fMoveSpd);
		pActionInterval = cocos2d::CCMoveTo::create(fTime,kDestPos);
	}
	if (pAction->kType == QYActionGravity::getType())
	{
		QYActionGravity* pTempAction = (QYActionGravity*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fGravity = utility::getRandFloat(pTempAction->fGravity,pNode);
		pActionInterval = cocos2d::GravityAction::create(fTime,fGravity);
	}
	
	if (pAction->kType == QYActionSetScale::getType())
	{
		QYActionSetScale* pTempAction = (QYActionSetScale*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetScale),pAction);
	}

	if (pAction->kType == QYActionScaleTo::getType())
	{
		QYActionScaleTo* pTempAction = (QYActionScaleTo*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fDestScale = utility::getRandFloat(pTempAction->fDestScale,pNode);
		pActionInterval = cocos2d::ScaleTo::create(fTime,fDestScale);
	}
	if (pAction->kType == QYActionScaleXTo::getType())
	{
		QYActionScaleXTo* pTempAction = (QYActionScaleXTo*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fDestScale = utility::getRandFloat(pTempAction->fDestScaleX,pNode);
		pActionInterval = cocos2d::ScaleTo::create(fTime,fDestScale,pNode->getScaleY());
	}
	if (pAction->kType == QYActionScaleYTo::getType())
	{
		QYActionScaleYTo* pTempAction = (QYActionScaleYTo*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fDestScale = utility::getRandFloat(pTempAction->fDestScaleY,pNode);
		pActionInterval = cocos2d::ScaleTo::create(fTime,pNode->getScaleX(),fDestScale);
	}
	if (pAction->kType == QYActionSetAple::getType())
	{
		QYActionSetAple* pTempAction = (QYActionSetAple*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetAple),pAction);
	}
	if (pAction->kType == QYActionApleTo::getType())
	{
		QYActionApleTo* pTempAction = (QYActionApleTo*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fDestAple = utility::getRandFloat(pTempAction->fDestAple,pNode);
		pActionInterval = cocos2d::FadeTo::create(fTime,fDestAple*255);
	}

	if (pAction->kType == QYActionSetRota::getType())
	{
		QYActionSetRota* pTempAction = (QYActionSetRota*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetRota),pAction);
	}
	if (pAction->kType == QYActionRotaTo::getType())
	{
		QYActionRotaTo* pTempAction = (QYActionRotaTo*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fDestRota = utility::getRandFloat(pTempAction->fDestRota,pNode);
		pActionInterval = cocos2d::RotateTo::create(fTime,fDestRota);
	}
	if (pAction->kType == QYActionRotaBy::getType())
	{
		QYActionRotaBy* pTempAction = (QYActionRotaBy*)pAction;
		float fTime = utility::getRandFloat(pTempAction->fTime,pNode);
		float fDestRota = utility::getRandFloat(pTempAction->fDestRota,pNode);
		pActionInterval = cocos2d::RotateBy::create(fTime,fDestRota);
	}
	if (pAction->kType == QYActionCallAction::getType())
	{
		QYActionCallAction* pTempAction = (QYActionCallAction*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionCallAction),pAction);
	}
	if (pAction->kType == QYActionCallButton::getType())
	{
		QYActionCallButton* pTempAction = (QYActionCallButton*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionCallButton),pAction);
	}
	if (pAction->kType == QYActionCreateWidget::getType())
	{
		QYActionCreateWidget* pTempAction = (QYActionCreateWidget*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionCreateWidget),pAction);
	}
	if (pAction->kType == QYActionRunAnim::getType())
	{
		QYActionRunAnim* pTempAction = (QYActionRunAnim*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionRunAnim),pAction);
	}
	if (pAction->kType == QYActionPlaySound::getType())
	{
		QYActionPlaySound* pTempAction = (QYActionPlaySound*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionPlaySound),pAction);
	}
	if (pAction->kType == QYActionSetTxt::getType())
	{
		QYActionSetTxt* pTempAction = (QYActionSetTxt*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetTxt),pAction);
	}
	if (pAction->kType == QYActionSetImage::getType())
	{
		QYActionSetImage* pTempAction = (QYActionSetImage*)pAction;
		pActionInterval = cocos2d::CCCallFuncND::create(this,callfuncND_selector(WidgetManager::CB_QYActionSetImge),pAction);
	}
	CCASSERT(pActionInterval,"");
	return pActionInterval;
}
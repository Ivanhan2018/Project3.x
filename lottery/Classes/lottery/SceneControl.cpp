//  [3/19/2014 wucan]


#include "SceneControl.h"
//#include "SceneView.h"
//#include "DDZRes.h"
#include "PromptBox.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
SceneControl* SceneControl::sharedSceneControl()
{
	static SceneControl* 	s_SceneControl=NULL;
	if (!s_SceneControl)
	{
		s_SceneControl=new SceneControl();
	}
	return s_SceneControl;
}

Scene* SceneControl::getScene(int index,bool bEffect,float t)
{
	Scene* s=Scene::create();

	//--select Scene
	switch(index)
	{
	case SCENE_start:
		{
			RoomLayer* layer = RoomLayer::create();
			layer->automaticLogin(false);
			s->addChild(layer);
			EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
			EntityMgr::instance()->getDispatch()->closeGameSocket();
			EntityMgr::instance()->roomFrame()->reset();
			ConfigMgr::instance()->m_Config.m_isGameRun=false;
		}
		break;
	case SCENE_game:
		{
			//s->addChild(SceneView::create());
			//ConfigMgr::instance()->m_Config.m_isGameRun=true;
		}
		break;
	case SCENE_endAgain:
		{
			//s->addChild(SceneView::create());
			EntityMgr::instance()->getDispatch()->closeGameSocket();
			EntityMgr::instance()->roomFrame()->reset();
			bool ret = EntityMgr::instance()->getDispatch()->connectGameServer(300);
			ConfigMgr::instance()->m_Config.m_isGameRun=true;
			if (!ret)
			{
				//cocos2d::Size winSize=Director::getInstance()->getWinSize();
				//PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeMakeSureModifyNickName);
				//box->setPromptText(ConfigMgr::instance()->text("t475"));
				//s->addChild(box,100);
			}
		}
		break;
	default:
		return NULL;
	}

	do 
	{
		//--select effect
		srand(time(NULL));
		int i = rand()%41;
		if(!bEffect)break;
		switch(i)
		{
		case 0: return CCTransitionJumpZoom::create(t, s);
		case 1: return CCTransitionProgressRadialCCW::create(t, s);
		case 2: return CCTransitionProgressRadialCW::create(t, s);
		case 3: return CCTransitionProgressHorizontal::create(t, s);
		case 4: return CCTransitionProgressVertical::create(t, s);
		case 5: return CCTransitionProgressInOut::create(t, s);
		case 6: return CCTransitionProgressOutIn::create(t, s);
		case 7: return CCTransitionCrossFade::create(t,s);
		case 8: return CCTransitionPageTurn::create(t, s,false);
		case 9: return CCTransitionPageTurn::create(t, s,true);
		case 10: return CCTransitionFadeTR::create(t, s);
		case 11: return CCTransitionFadeBL::create(t, s);
		case 12: return CCTransitionFadeUp::create(t, s);
		case 13: return CCTransitionFadeDown::create(t, s);
		case 14: return CCTransitionTurnOffTiles::create(t, s);
		case 15: return CCTransitionSplitRows::create(t, s);
		case 16: return CCTransitionSplitCols::create(t, s);
		case 17: return TransitionFade::create(t, s);
		case 18: return TransitionFade::create(t, s,Color3B::WHITE);
		case 19: return CCTransitionFlipX::create(t, s,kCCTransitionOrientationLeftOver);
		case 20: return CCTransitionFlipX::create(t, s,kCCTransitionOrientationRightOver);
		case 21: return CCTransitionFlipY::create(t, s,kCCTransitionOrientationUpOver);
		case 22: return CCTransitionFlipY::create(t, s,kCCTransitionOrientationDownOver);
		case 23: return CCTransitionFlipAngular::create(t, s,kCCTransitionOrientationLeftOver);
		case 24: return CCTransitionFlipAngular::create(t, s,kCCTransitionOrientationRightOver);
		case 25: return CCTransitionZoomFlipX::create(t, s,kCCTransitionOrientationLeftOver);
		case 26: return CCTransitionZoomFlipX::create(t, s,kCCTransitionOrientationRightOver);
		case 27: return CCTransitionZoomFlipY::create(t, s,kCCTransitionOrientationUpOver);
		case 28: return CCTransitionZoomFlipY::create(t, s,kCCTransitionOrientationDownOver);
		case 29: return CCTransitionZoomFlipAngular::create(t, s,kCCTransitionOrientationLeftOver);
		case 30: return CCTransitionZoomFlipAngular::create(t, s,kCCTransitionOrientationRightOver);
		case 31: return CCTransitionShrinkGrow::create(t, s);
		case 32: return CCTransitionRotoZoom::create(t, s);
		case 33: return CCTransitionMoveInL::create(t, s);
		case 34: return CCTransitionMoveInR::create(t, s);
		case 35: return CCTransitionMoveInT::create(t, s);
		case 36: return CCTransitionMoveInB::create(t, s);
		case 37: return CCTransitionSlideInL::create(t, s);
		case 38: return CCTransitionSlideInR::create(t, s);
		case 39: return CCTransitionSlideInT::create(t, s);
		case 40: return CCTransitionSlideInB::create(t, s);
		default: break;
		}
	} while (0);
	return s;
}


#include "SceneControl.h"
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
			pMsgDispatch->SendPacketWithPerformStandup();
			pMsgDispatch->closeGameSocket();
			EntityMgr::instance()->roomFrame()->reset();
			pConfigMgr->m_Config.m_isGameRun=false;
		}
		break;
	case SCENE_game:
		{

		}
		break;
	case SCENE_endAgain:
		{
			pMsgDispatch->closeGameSocket();
			EntityMgr::instance()->roomFrame()->reset();
			bool ret = pMsgDispatch->connectGameServer(300);
			pConfigMgr->m_Config.m_isGameRun=true;
			if (!ret)
			{

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

Scene* SceneControl::getScene(Scene *s)
{
	do 
	{
		//--select effect
		float t = 0.5f;
		srand(time(NULL));
		static int i = rand()%41;
		if(i == 32)
		{
			i = 33;
		}
		switch(i)
		{
		case 0: return TransitionJumpZoom::create(t, s);
		case 1: return TransitionProgressRadialCCW::create(t, s);
		case 2: return TransitionProgressRadialCW::create(t, s);
		case 3: return TransitionProgressHorizontal::create(t, s);
		case 4: return TransitionProgressVertical::create(t, s);
		case 5: return TransitionProgressInOut::create(t, s);
		case 6: return TransitionProgressOutIn::create(t, s);
		case 7: return TransitionCrossFade::create(t,s);
		case 8: return TransitionPageTurn::create(t, s,false);
		case 9: return TransitionPageTurn::create(t, s,true);
		case 10: return TransitionFadeTR::create(t, s);
		case 11: return TransitionFadeBL::create(t, s);
		case 12: return TransitionFadeUp::create(t, s);
		case 13: return TransitionFadeDown::create(t, s);
		case 14: return TransitionTurnOffTiles::create(t, s);
		case 15: return TransitionSplitRows::create(t, s);
		case 16: return TransitionSplitCols::create(t, s);
		case 17: return TransitionFade::create(t, s);
		case 18: return TransitionFade::create(t, s,Color3B::WHITE);
		case 19: return TransitionFlipX::create(t, s,kCCTransitionOrientationLeftOver);
		case 20: return TransitionFlipX::create(t, s,kCCTransitionOrientationRightOver);
		case 21: return TransitionFlipY::create(t, s,kCCTransitionOrientationUpOver);
		case 22: return TransitionFlipY::create(t, s,kCCTransitionOrientationDownOver);
		case 23: return TransitionFlipAngular::create(t, s,kCCTransitionOrientationLeftOver);
		case 24: return TransitionFlipAngular::create(t, s,kCCTransitionOrientationRightOver);
		case 25: return TransitionZoomFlipX::create(t, s,kCCTransitionOrientationLeftOver);
		case 26: return TransitionZoomFlipX::create(t, s,kCCTransitionOrientationRightOver);
		case 27: return TransitionZoomFlipY::create(t, s,kCCTransitionOrientationUpOver);
		case 28: return TransitionZoomFlipY::create(t, s,kCCTransitionOrientationDownOver);
		case 29: return TransitionZoomFlipAngular::create(t, s,kCCTransitionOrientationLeftOver);
		case 30: return TransitionZoomFlipAngular::create(t, s,kCCTransitionOrientationRightOver);
		case 31: return TransitionShrinkGrow::create(t, s);
		case 32: return TransitionRotoZoom::create(t, s);
		case 33: return TransitionMoveInL::create(t, s);
		case 34: return TransitionMoveInR::create(t, s);
		case 35: return TransitionMoveInT::create(t, s);
		case 36: return TransitionMoveInB::create(t, s);
		case 37: return TransitionSlideInL::create(t, s);
		case 38: return TransitionSlideInR::create(t, s);
		case 39: return TransitionSlideInT::create(t, s);
		case 40: return TransitionSlideInB::create(t, s);
		default: break;
		}
	} while (0);
	return s;
}

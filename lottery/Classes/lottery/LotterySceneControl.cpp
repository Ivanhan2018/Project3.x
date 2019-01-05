//  [3/19/2014 wucan]


#include "LotterySceneControl.h"

LotterySceneControl* LotterySceneControl::sharedSceneControl()
{
	static LotterySceneControl* 	s_SceneControl=NULL;
	if (!s_SceneControl)
	{
		s_SceneControl=new LotterySceneControl();
	}
	return s_SceneControl;
}

Scene* LotterySceneControl::getScene(Scene *s)
{
	do 
	{
		//--select effect
		float t = 0.5f;
		srand(time(NULL));
		static int i = rand()%8;	
		//i = i ++ % 7;
		switch(i)
		{
		case 0: return TransitionSlideInR::create(t, s);
		case 1: return TransitionProgressRadialCCW::create(t, s);
		case 2: return TransitionProgressRadialCW::create(t, s);
		case 3: return TransitionProgressHorizontal::create(t, s);
		case 4: return TransitionProgressVertical::create(t, s);
		case 5: return TransitionProgressInOut::create(t, s);
		case 6: return TransitionProgressOutIn::create(t, s);
		case 7: return TransitionSlideInL::create(t, s);
		case 8: return TransitionPageTurn::create(t, s,false);//
		case 9: return TransitionPageTurn::create(t, s,true);//
		case 10: return TransitionFadeTR::create(t, s);//
		case 11: return TransitionFadeBL::create(t, s);//
		case 12: return TransitionFadeUp::create(t, s);//
		case 13: return TransitionFadeDown::create(t, s);//
		case 14: return TransitionTurnOffTiles::create(t, s);//
		default: break;
		}
	} while (0);
	return s;
}


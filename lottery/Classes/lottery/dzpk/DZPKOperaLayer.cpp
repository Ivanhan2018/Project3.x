/***************************************
操作面板
游戏界面
xxxx 2014.1.5
***************************************/

#include "DZPKOperaLayer.h"
#include "SimpleAudioEngine.h"
#include "BJLRes.h"//by hxh
#include "MyConfig.h"//by hxh
#include "MyNSString.h"//by hxh
#include "DZPKLayer.h"//by hxh
#include "SoundControl.h"//by hxh

using namespace CocosDenshion;
#define BTN_ANIM_TIME						0.25f								//按钮动画时间
#define BTN_PRESSED							  100								//按钮被按下
#define BTN_NOT_PRESSED						    0								//按钮没被按下

#define MSG_OPERALAYER_BTN_PRESSED			"MSG_OPERALAYER_BTN_PRESSED"       //操作面板按钮已按下，取消定时器

using namespace cocos2d;

DZPKOperaLayer *DZPKOperaLayer::create(){
	DZPKOperaLayer *operaLayer = new DZPKOperaLayer();
	if (operaLayer&&operaLayer->initOperaLayer())
	{
		operaLayer->autorelease();
		return operaLayer;
	}	
	CC_SAFE_DELETE(operaLayer);
	return NULL;
}

const char * DZPKOperaLayer::getBtnTexName(int idx)
{
	const char * s_szArr[SPRITE_NUM]={texture_name::s_ol_btn_ckpx_normal,texture_name::s_ol_bg_ckpx,texture_name::s_ol_btn_gryz_normal,texture_name::s_ol_btn_qp,texture_name::s_ol_btn_qx,texture_name::s_gz_btn,texture_name::s_jz_btn,texture_name::s_gp_btn,texture_name::s_xz_btn};
	const char * s=(idx>=0&&idx<SPRITE_NUM?s_szArr[idx]:"");
	return s;
}

bool DZPKOperaLayer::initOperaLayer()
{
	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(DZPKOperaLayer::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(DZPKOperaLayer::onTouchEnded,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	bool bRet = false;
	do 
	{
	    memset(&m_addScore, 0, sizeof(CMD_C_AddScore));
        
		MyConfig& myConfig=MyConfig::Instance();
		for(int i=0;i<SPRITE_NUM;i++)
		{
		    m_pBTN[i]=Sprite::createWithSpriteFrame(spriteFrame(getBtnTexName(i)));
			m_pBTN[i]->getTexture()->setAliasTexParameters(); 
			m_pBTN[i]->setOpacity(0);
			m_pBTN[i]->setScale(myConfig.m_DZPKOperaLayerScale);
			m_pBTN[i]->setPosition(ccp(myConfig.m_DZPKOperaLayerPos[i][0],myConfig.m_DZPKOperaLayerPos[i][1]));
			addChild(m_pBTN[i]);
		}
		m_pBTN[1]->setOpacity(255);
		m_pBTN[1]->setScale(0);
		m_pBTN[1]->setAnchorPoint(ccp(0,0));
		m_pCKPX_BTN=m_pBTN[0];
		m_pCKPX_PIC=m_pBTN[1];
		m_pGRYZ_BTN=m_pBTN[2];
		m_pQP_BTN=m_pBTN[3];
		m_pQX_BTN=m_pBTN[4];
		m_pGZ_BTN=m_pBTN[5];
		m_pJZ_BTN=m_pBTN[6];
		m_pGP_BTN=m_pBTN[7];
		m_pXZ_BTN=m_pBTN[8];
		m_pGRYZ_BTN->setVisible(false);
		m_pQP_BTN->setVisible(false);
		m_pGZ_BTN->setVisible(false);
		m_pJZ_BTN->setVisible(false);
		m_pQX_BTN->setVisible(false);
#ifndef SHOW_BET_XZ_BTN
	    m_pXZ_BTN->setVisible(false);
#endif
		m_bGRYZ = false;

		Vec2 vOffset = Vec2(0, 30);
		m_pRaiseSlider = DZPKRaiseSlider::create();
		addChild(m_pRaiseSlider);
		m_pRaiseSlider->setPosition(Vec2(myConfig.m_DZPKOperaLayerPos[9][0],myConfig.m_DZPKOperaLayerPos[9][1]) + vOffset);
		dismissRaiseSlider();

		bRet = true;
	} while (0);

	return bRet;
}

void DZPKOperaLayer::showDXGZJZBtns(int iGZ1GP2)
{
	if (iGZ1GP2 == 1)
	{
		showBtn(CALL_GZ_BTN,true);
	}else
	{
		showBtn(CHECK_GP_BTN,true);
	}
	showBtn(FOLD_QP_BTN,true);
	showBtn(RAISE_JZ_BTN,true);
	showBtn(ALLIN_QX_BTN,true);
}

void DZPKOperaLayer::dismissDXBtns()
{
	showBtn(FOLD_QP_BTN,false);
	showBtn(CALL_GZ_BTN,false);
	showBtn(RAISE_JZ_BTN,false);
	showBtn(ALLIN_QX_BTN,false);
	showBtn(CHECK_GP_BTN,false);
	dismissRaiseSlider();
#ifndef SHOW_BET_XZ_BTN
	showBtn(BET_XZ_BTN,false);
#endif
}

void DZPKOperaLayer::showRaiseSlider()
{
	m_pRaiseSlider->setVisible(true);
	m_pRaiseSlider->resetRaiseSlider();
}

void DZPKOperaLayer::dismissRaiseSlider()
{
	m_pRaiseSlider->setVisible(false);
	m_pRaiseSlider->resetRaiseSlider();
}

void DZPKOperaLayer::showBtn(int idx,bool visible)
{
	Sprite *sprite=m_pBTN[idx];
	sprite->stopAllActions();
	if(visible)
	{
	    sprite->runAction(Sequence::create(Show::create(),FadeTo::create(BTN_ANIM_TIME,255),NULL));
	}
	else
	{
	    sprite->runAction(Sequence::create(FadeTo::create(BTN_ANIM_TIME,0),Hide::create(),NULL));
	}
}

void DZPKOperaLayer::doGXOpera(float t)
{
	unschedule(schedule_selector(DZPKOperaLayer::doGXOpera));
	if (m_bGRYZ)
	{
		if (!m_bGetRaiseInfo)
		{
			EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CHECK,&cNull,sizeof(cNull));		
		}
		else
		{
			m_addScore.lScore = 1;
			EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_ADD_SCORE,&m_addScore,sizeof(m_addScore));
		}
	}
	m_bGRYZ = false;
	m_pGRYZ_BTN->setDisplayFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_ol_btn_gryz_normal));
}

void DZPKOperaLayer::SendAddScoreReq(LONGLONG lScore,int idx)
{
	m_addScore.lScore = lScore;
	m_addScore.cbJudgeAction=getJudgeAction(idx);
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_ADD_SCORE,&m_addScore,sizeof(m_addScore));
#if 1
	string str=String::createWithFormat("加注数目【%lld】【%s】",lScore,SPRITE_NAME(idx))->getCString();
	MyNSString::GBKToUTF8(str);
	NotificationCenter::getInstance()->postNotification("PrintTest1",String::create(str));
#endif
}

void DZPKOperaLayer::onBtnBegan(int idx)
{
	// 点击音效<add by hxh 20161011>
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_dianji);

	string str=String::createWithFormat("您点击了按钮【%s】",SPRITE_NAME(idx))->getCString();
	MyNSString::GBKToUTF8(str);
	NotificationCenter::getInstance()->postNotification("PrintTest",String::create(str));
}


void DZPKOperaLayer::onBtnEnded(int idx)
{
	switch (idx)
	{
	case CKPX_BTN: //查看牌型按钮0
		{
			if (m_pBTN[CKPX_PIC]->getScale()!=1)
			{
				m_pBTN[CKPX_PIC]->setTag(1);
				m_pBTN[CKPX_PIC]->stopAllActions();
				m_pBTN[CKPX_PIC]->runAction(Sequence::create(ScaleTo::create(0.2f,1.0f),ScaleTo::create(0.1f,0.95f),ScaleTo::create(0.2f,1.0f),NULL));
				CCLOG("OperaLayer------check card type action open------");

				CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
				DZPKLayer *layer=(DZPKLayer *)scene->getChildByTag(DZPKLayerTag);
				if (!layer)
				{
					return;
				}
			}
		}
		break;
	case CKPX_PIC: //查看牌型图片1
		break;
	case GRYZ_BTN: //勾选跟任意注按钮2
		{		
			if (!m_bGRYZ)
			{
				m_bGRYZ = true;
				m_pGRYZ_BTN->setDisplayFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_ol_btn_gryz_pressed));
				CCLOG("OperaLayer------follow any call action on------");
			}
			else
			{
				m_bGRYZ = false;
				m_pGRYZ_BTN->setDisplayFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_ol_btn_gryz_normal));
				CCLOG("OperaLayer------follow any call off------");
			}			
		}
		break;
	case FOLD_QP_BTN://弃牌按钮3
		{
			dismissDXBtns();
			EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_GIVE_UP,&cNull,sizeof(cNull));
			CCLOG("OperaLayer------fold action------");
		}
		break;
	case ALLIN_QX_BTN: //全下按钮4
		{
			dismissDXBtns();
			LONGLONG  cAddScore3=getDZPKLayer()->m_lTurnMaxScore;    //最大下注
			SendAddScoreReq(cAddScore3,ALLIN_QX_BTN);
			CCLOG("OperaLayer------allin action------");			
		}
		break;
	case CALL_GZ_BTN: //跟注按钮5
		{
			dismissDXBtns();
			LONGLONG  cAddScore4=getDZPKLayer()->m_lTurnLessScore;    //加注下注
			SendAddScoreReq(cAddScore4,CALL_GZ_BTN);
			CCLOG("OperaLayer------call action------");			
		}
		break;
	case RAISE_JZ_BTN: //加注按钮6
		{
			Scene* pScene = Director::getInstance()->getRunningScene();
			DZPKLayer* pLayer = dynamic_cast<DZPKLayer*>(pScene->getChildByTag(DZPKLayerTag));
			LONGLONG llMinRaiseValue = m_pRaiseSlider->getMinRaiseValue();
			LONGLONG llCurrGold = pLayer->m_lTurnMaxScore;
			if (llCurrGold <= llMinRaiseValue)
			{
				//	持有金币值小于最少下注值时，加注按钮不生效
				string sText = "持有金币值小于最少加注值时，加注按钮不生效";
				DZPKLayer::showDebugTips(sText);
				break;
			}

			if (!m_pRaiseSlider->isVisible())
			{
				showRaiseSlider();
				CCLOG("OperaLayer------raise panel open------");
			}
			else
			{
				if (llMinRaiseValue <= 0)
				{
					string sText = "加注数额不得为0";
					DZPKLayer::showDebugTips(sText);
					break;
				}
				else
				{
					LONGLONG llValue = m_pRaiseSlider->getRaiseValue();
					SendAddScoreReq(llValue, RAISE_JZ_BTN);
				}
				dismissRaiseSlider();
				dismissDXBtns();
				CCLOG("OperaLayer------raise action------");
			}			
		}
		break;
	case CHECK_GP_BTN: //过牌按钮7
		{
			dismissDXBtns();
            SendAddScoreReq(0,CHECK_GP_BTN);
			CCLOG("OperaLayer------check action------");			
		}
		break;
	case BET_XZ_BTN: //下注按钮8
		{
			if (m_pRaiseSlider->isVisible())
			{
				showRaiseSlider();
				CCLOG("OperaLayer------call panel open------");
			}
			else
			{
			}
		}
		break;
	default:
		break;
	}
}

bool DZPKOperaLayer::onTouchBegan( Touch *pTouch, Event *pEvent )
{
	Vec2 location = pTouch->getLocation();
#if 1
	for(int i=0;i<SPRITE_NUM;i++)
	{
		if(i==CKPX_PIC)
			continue;
		if(m_pBTN[i]->getOpacity()==255&&m_pBTN[i]->boundingBox().containsPoint(location))
		{
			if(i!=CKPX_BTN)
               m_pBTN[i]->setColor(ccc3(100,100,100));    
			m_pBTN[i]->setTag(BTN_PRESSED);
		    onBtnBegan(i);
		}
	}
#else

#endif
	return true;
}

void DZPKOperaLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 location = pTouch->getLocation();
	if (m_pCKPX_PIC->getTag()==1)
	{
		m_pCKPX_PIC->setTag(0);
		m_pCKPX_PIC->stopAllActions();
		m_pCKPX_PIC->runAction(ScaleTo::create(0.25f,0.0f));
		CCLOG("OperaLayer------check card type action off------");
	}
#if 1
	for(int i=0;i<SPRITE_NUM;i++)
	{
		if(i==CKPX_PIC)
			continue;
		if(m_pBTN[i]->getOpacity()==255&&m_pBTN[i]->getTag()==BTN_PRESSED)
		{
			if(m_pBTN[i]->boundingBox().containsPoint(location))
			{
			   onBtnEnded(i);
			}
		}
		if(i!=GRYZ_BTN)
			m_pBTN[i]->setColor(ccc3(255,255,255));
		m_pBTN[i]->setTag(BTN_NOT_PRESSED);
	}
	//加注(下注)滑动条
	if (m_pRaiseSlider->isVisible()&&!m_pRaiseSlider->boundingBox().containsPoint(location)&&!m_pBTN[RAISE_JZ_BTN]->boundingBox().containsPoint(location))
	{
		dismissRaiseSlider();
		CCLOG("OperaLayer------panel close------");
	}
#else

#endif
}

DZPKLayer* DZPKOperaLayer::getDZPKLayer()
{
	DZPKLayer* layer = (DZPKLayer*)this->getParent();
	return layer;
}

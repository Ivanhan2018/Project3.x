#include "PPCGameLayer.h"
#include "RoomLayer.h"
#include "EntityMgr.h"
#include "PPCRes.h"
#include "ToolKit.h"
#include "PPCGameScene.h"
#include "PPCDialogQuit.h"
#include "PPCSetDialog.h"

PPCGameLayer::PPCGameLayer()
{
}

PPCGameLayer::~PPCGameLayer()
{
}

void PPCGameLayer::resetGameLayer()
{
	resetCarLogo();
}

void PPCGameLayer::resetCarLogo()
{
	for (auto it = m_mCarLogoList.begin(); it != m_mCarLogoList.end(); it++)
	{
		PPCCarLogo* pLogo = it->second;
		pLogo->resetPPCCarLogo();
	}

}

bool PPCGameLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		initUI();
		setApplyState(EPPC_Apply_State_Null);

		bRet = true;
	} while (0);

	return bRet;
}

void PPCGameLayer::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pPPCBG = Sprite::create(PPCTextureName::s_ppc_table_bg);
	addChild(m_pPPCBG);
	m_pPPCBG->setPosition(Vec2(sVSize / 2));

	initCarLogo();

	m_pBtnQuit = Button::create(PPCTextureName::s_ppc_quit_normal, "", "", TextureResType::PLIST);
	addChild(m_pBtnQuit);
	m_pBtnQuit->setPosition(Vec2(sVSize.width * 0.96, sVSize.height * 0.94));
	m_pBtnQuit->addClickEventListener(CC_CALLBACK_0(PPCGameLayer::onClickBtnQuit, this));

	auto setting = Button::create(PPCTextureName::s_ppc_set_normal, PPCTextureName::s_ppc_set_selected, "");
	addChild(setting);
	setting->setPosition(Vec2(sVSize.width * 0.04, sVSize.height * 0.94));
	setting->addClickEventListener(CC_CALLBACK_0(PPCGameLayer::onClickBtnSetting, this));

	m_pBtnApplyList = Button::create(PPCTextureName::s_ppc_apply_lsit_normal, PPCTextureName::s_ppc_apply_lsit_pressed, "", TextureResType::PLIST);
	addChild(m_pBtnApplyList);
	m_pBtnApplyList->setPosition(Vec2(sVSize.width * 0.95, sVSize.height * 0.09));
	m_pBtnApplyList->addClickEventListener(CC_CALLBACK_0(PPCGameLayer::onClickBtnApplyList, this));

	m_pBtnApply = Button::create(PPCTextureName::s_ppc_apply_normal, PPCTextureName::s_ppc_apply_selected, "", TextureResType::PLIST);
	addChild(m_pBtnApply);
	m_pBtnApply->setPosition(Vec2(sVSize.width * 0.93, sVSize.height * 0.23));
	m_pBtnApply->addClickEventListener(CC_CALLBACK_0(PPCGameLayer::onClickBtnApply, this));

	m_pBtnCancelApply = Button::create(PPCTextureName::s_ppc_cancel_normal, PPCTextureName::s_ppc_cancel_selected, "", TextureResType::PLIST);
	addChild(m_pBtnCancelApply);
	m_pBtnCancelApply->setPosition(Vec2(sVSize.width * 0.93, sVSize.height * 0.23));
	m_pBtnCancelApply->addClickEventListener(CC_CALLBACK_0(PPCGameLayer::onClickBtnCancelApply, this));

	m_pBtnDown = Button::create(PPCTextureName::s_ppc_down_normal, PPCTextureName::s_ppc_down_selected, "", TextureResType::PLIST);
	addChild(m_pBtnDown);
	m_pBtnDown->setPosition(Vec2(sVSize.width * 0.93, sVSize.height * 0.23));
	m_pBtnDown->addClickEventListener(CC_CALLBACK_0(PPCGameLayer::onClickBtnDown, this));

	m_pTravel = PPCTravel::create();
	addChild(m_pTravel);
	m_pTravel->setPosition(Vec2(sVSize.width * 0.5, sVSize.height * 0.07));
	//¼àÌý·µ»Ø¼ü

	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){	

		if(keycode == EventKeyboard::KeyCode::KEY_BACK)
		{
			event->stopPropagation();
			PPCDialogQuit* pQuit = PPCDialogQuit::create();
			Director::getInstance()->getRunningScene()->addChild(pQuit);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, this);

}

void PPCGameLayer::initCarLogo()
{
	for (int i = 0, j = EPPCCarLogo_Null; i < PPC_CARLGOG_COUNT; i++, j++)
	{
		if (j == EPPCCarLogo_Null)
		{
			j = EPPCCarLogo_Porsche_Big;
		}
		PPCCarLogo* pLogo = PPCCarLogo::create();
		m_pPPCBG->addChild(pLogo);
		pLogo->setPosition(Vec2(m_pPPCBG->getContentSize() / 2));
		pLogo->setLogoID(i);
		pLogo->initCarLogoWithType((EPPCCarLogoType)j);
		m_mCarLogoList[i] = pLogo;

/*
		//	µ÷Î»ÖÃ
		vector<Node*> vList;
		vList.push_back(pLogo->m_pImgCar);
		ToolKit::studioUIMulti(pLogo->m_pImgBG, vList);
		if (i == 12)
		{
			Scene* pCurrScene = Director::getInstance()->getRunningScene();
			cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

			Layer* pLayer = Layer::create();
			this->addChild(pLayer,9999);

			Button* pAdd = Button::create(
				"nd_btn_add.png",
				"nd_btn_add_press", "");
			pLayer->addChild(pAdd);
			pAdd->setPosition(Vec2(sVSize.width * 0.5,sVSize.height * 0.5));
			pAdd->addClickEventListener([=](Ref* sender)
			{
				if (!pLogo->m_pImgCar)
				{
					return;
				}
				pLogo->m_pImgCar->setRotation(pLogo->m_pImgCar->getRotation() + 5);
				log("pLogo->m_pImgCar->setRotation : %f", pLogo->m_pImgCar->getRotation());
			});

			Button* pReduce = Button::create(
				"nd_btn_reduce.png",
				"nd_btn_reduce_press.png", "");
			pLayer->addChild(pReduce);
			pReduce->setPosition(Vec2(sVSize.width * 0.6,sVSize.height * 0.5));
			pReduce->addClickEventListener([=](Ref* sender)
			{
				if (!pLogo->m_pImgCar)
				{
					return;
				}
				pLogo->m_pImgCar->setRotation(pLogo->m_pImgCar->getRotation() - 5);
				log("pLogo->m_pImgCar->setRotation : %f", pLogo->m_pImgCar->getRotation());
			});
		}
*/
	}
}

void PPCGameLayer::setApplyState(EPPCApplyState eType)
{
	m_eApplyState = eType;
	switch (eType)
	{
	case EPPC_EPPC_Apply_State_Apply:
		m_pBtnApply->setVisible(false);
		m_pBtnCancelApply->setVisible(true);
		m_pBtnDown->setVisible(false);
		break;
	case EPPC_EPPC_Apply_State_Banker:
		m_pBtnApply->setVisible(false);
		m_pBtnCancelApply->setVisible(false);
		m_pBtnDown->setVisible(true);
		break;
	case EPPC_Apply_State_Null:
		m_pBtnApply->setVisible(true);
		m_pBtnCancelApply->setVisible(false);
		m_pBtnDown->setVisible(false);
		break;
	default:
		break;
	}
}

EPPCApplyState PPCGameLayer::getApplyState()
{
	return m_eApplyState;
}

void PPCGameLayer::showSingleCarLogoHighLight(int nIdx, bool bShowCar)
{
	for (auto k : m_mCarLogoList)
	{
		int i = k.first;
		PPCCarLogo* pLogo = k.second;
		if (i == nIdx)
		{
			pLogo->setHighLight(true);
			if (bShowCar)
			{
				pLogo->setCarVisible(true);
			}
		}
		else
		{
			pLogo->setHighLight(false);
			pLogo->setCarVisible(false);
		}
	}
}

void PPCGameLayer::showAniChangeBanker(int nChairID)
{
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	float fMoveTime = 1.0f;
	float fShowTime = 2.0f;

	int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	Sprite* pImgTips = nullptr;
	if (nChairID == INVALID_CHAIR)
	{
		pImgTips = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_tips_none_banker);
	}
	else if (nChairID == nSelfChairID)
	{
		pImgTips = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_tips_officeholding_banker);
	}
	else
	{
		pImgTips = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_tips_turn_banker);
	}
	pGameScene->addChild(pImgTips);
	pImgTips->setPosition(Vec2(sVSize.width / 2, sVSize.height + pImgTips->getContentSize().height / 2));
	auto pMoveTo = MoveTo::create(fMoveTime, Vec2(sVSize / 2));
	auto pFadeIn = FadeIn::create(fShowTime);
	auto pRemoveSelf = RemoveSelf::create();
	auto pSeq = Sequence::create(pMoveTo, pFadeIn, pRemoveSelf, nullptr);
	pImgTips->runAction(pSeq);
}

void PPCGameLayer::onClickBtnQuit()
{
	PPCGameScene* pScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pScene)
	{
		return;
	}
	PPCDialogQuit* pQuit = PPCDialogQuit::create();
	if (pQuit)
	{
		pScene->addChild(pQuit);
	}
}

void PPCGameLayer::onClickBtnSetting()
{
	PPCGameScene* pScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pScene)
	{
		return;
	}
	auto setting = PPCSetDialog::create();
	if (setting)
	{
		pScene->addChild(setting);
	}
}

void PPCGameLayer::onClickBtnApplyList()
{
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_GET_PLAYLIST);
}

void PPCGameLayer::onClickBtnApply()
{
	DWORD dSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	ppcCMD_S_ApplyBanker pData;
	pData.wApplyUser = dSelfChairID;
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_APPLY_BANKER, &pData, sizeof(pData));
}

void PPCGameLayer::onClickBtnCancelApply()
{
	tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	DWORD dSelfChairID = pUserData->wChairID;
	ppcCMD_S_CancelBanker pData;
	pData.wChairID = dSelfChairID;
	memset(pData.szCancelUser, 0, sizeof(pData.szCancelUser));
	memcpy(pData.szCancelUser, pUserData->szNickName, sizeof(pUserData->szNickName));
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CANCEL_BANKER, &pData, sizeof(pData));
}

void PPCGameLayer::onClickBtnDown()
{
	tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	DWORD dSelfChairID = pUserData->wChairID;
	ppcCMD_S_CancelBanker pData;
	pData.wChairID = dSelfChairID;
	memset(pData.szCancelUser, 0, sizeof(pData.szCancelUser));
	memcpy(pData.szCancelUser, pUserData->szNickName, sizeof(pUserData->szNickName));
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CANCEL_BANKER, &pData, sizeof(pData));
}

void PPCGameLayer::addGameRecord(EPPCCarLogoType eType)
{
	if (eType < EPPCCarLogo_Porsche_Big || eType > EPPCCarLogo_VW_Small)
	{
		return;
	}
	m_pTravel->addTravelItem(eType);
}

#include "DDZSceneView.h"
#include "DDZSceneLogic.h"
#include "DDZDDZRes.h"
#include "RoomLayer.h"
#include "DDZSceneControl.h"
#include "SoundControl.h"//<by hxh>
#include "MovingLabelLayer.h"
#include "MyNSString.h"
#include "DDZViewMenuLayer.h"
#include "DDZTimeCheckControl.h"
#include "DDZUserInfoManager.h"

DDZSceneView::DDZSceneView(void)
	:m_bankId(0)
{
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneView::OnMessage),MESSAGE_TOVIEW,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneView::receiveMenuClickCallfunc),MESSAGE_MENUCLICKED,NULL);//监听按钮点击消息
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneView::wangZha),"WANGZHA",NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DDZSceneView::onLoginFaild), MSG_UI_ANS_ROOMLOGINFAIL, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DDZSceneView::onSitFaild), MSG_UI_ANS_SITFAIL, NULL);

	m_userOutCardState = 3 ;//3为普通出牌
}


DDZSceneView::~DDZSceneView(void)
{
	NotificationCenter::getInstance()->removeObserver(this, MESSAGE_TOVIEW);
	NotificationCenter::getInstance()->removeObserver(this,MESSAGE_MENUCLICKED);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_ROOMLOGINFAIL);
	NotificationCenter::getInstance()->removeObserver(this,"WANGZHA");
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_SITFAIL);
}

bool DDZSceneView::init()
{

	cocos2d::Size size = Director::getInstance()->getWinSize();
	bool ret = false;
	do 
	{
		if (!Node::init())
		{
			break;
		}
		// 加载游戏场景资源到内存<by hxh>
		LoadRes();

		DDZTimeCheckControl::getInstance()->resetTimes();

		Sprite * background = Sprite::createWithSpriteFrameName(DDZtexture_name::s_background);
		background->setAnchorPoint(Vec2::ZERO);
		this->addChild(background);

		m_pSceneLogic = DDZSceneLogic::create();						//--逻辑
		m_pSceneLogic->setDDZSceneView(this);
		this->addChild(m_pSceneLogic);

		m_handCardView =DDZViewHandCard::create();					//--卡牌
		this->addChild(m_handCardView);


		m_menuLayer = DDZViewMenuLayer::create();							//--按钮
		this->addChild(m_menuLayer);

		m_cardTableLayer = DDZCardTableLayer::create();
		this->addChild(m_cardTableLayer);


		m_effectView =DDZViewCardEffect::create();						//--效果
		this->addChild(m_effectView);

		m_dialogLayer = Layer::create();
		this->addChild(m_dialogLayer);
		
		m_isAgency =false;												//--机器人
		m_isLandLord=true;												//--叫地主按钮和表现
		m_AgencyNum=0;

		m_gameStae=DDZGame_null;
		m_timeKey=0;
		m_menuLayer->setRobotEnable(false);
		m_menuLayer->enterGameView();//进入给出换桌按钮

		ret = true;
	} while (0);

	return ret;
}

void DDZSceneView::onExit()
{
	Node::onExit();

	//	在换桌时会被调用，导致崩溃，场景切换调用顺序问题
	//由于斗地主界面在换桌操作时自己 replaceScene 自己， 导致自己图片释放先后问题，从而改为在回到彩票大厅的时候回收斗地主图片
	// 	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("ddzCardtable.plist");
	// 	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("face.plist");
}

void DDZSceneView::LoadRes()
{

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ddzCardtable.plist","ddzCardtable.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ddz_resources.plist","ddz_resources.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("face.plist","face.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ddz_popups_summary.plist","ddz_popups_summary.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ddz_pokers_img.plist","ddz_pokers_img.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player_info.plist","player_info.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pic_dating.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("normal_button.plist","normal_button.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plaza.plist","plaza.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite1.plist","sprite1.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite2.plist","sprite2.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tab_button.plist","tab_button.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shop.plist","shop.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("exchange.plist","exchange.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("luckyDraw1.plist","luckyDraw1.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("luckyDraw2.plist","luckyDraw2.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("medal.plist","medal.png");
}

void DDZSceneView::onBankrupt( Object * obj )
{
	/*
	tagBankruptObj* tag=(tagBankruptObj*)obj;
	if (tag->bBankrupt == false)	//第一次破产 送你5000金返回大厅
	{
	m_dialogLayer->removeAllChildren();
	DDZDialogLayer * dialog = DDZDialogLayer::create(dialog_poChanSongCoin);
	m_dialogLayer->addChild(dialog);

	}
	else //破产放回大厅
	{
	m_dialogLayer->removeAllChildren();
	DDZDialogLayer * dialog = DDZDialogLayer::create(dialog_poChan);
	m_dialogLayer->addChild(dialog);
	}
	*/
	Director::getInstance()->replaceScene(DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_start,false));
}

void DDZSceneView::OnMessage( Object* obj )
{
	Integer* _int=(Integer*)obj;
	switch(_int->getValue())
	{
	case DDZmsgToView_UserInfor:	//--用户星信息表现
		userInforDeal(_int);
		break;
	case DDZmsgToView_SitDown:	//--坐下
		dealSitDown(_int);
		break;
	case DDZmsgToView_addView:	//--游戏加倍表现
		addViewDeal(_int);
		break;
	case DDZmsgToView_backCard:	//--底牌变化
		onBackTaskDeal(_int);
		break;
	case DDZmsgToView_start:	//--准备明牌开始
		startGameDeal();
		break;
	case DDZmsgToView_startView:	//--明牌开始表现
		startGameViewDeal(_int);
		break;
	case DDZmsgToView_sendCard:	//--发牌表现
		sendCardDeal(_int);
		break;
	case DDZmsgToView_openCard:	//--明牌表现
		openCardDeal(_int);
		break;
	case DDZmsgToView_landLord:	//--抢地主表现
		landLordDeal(_int);
		break;
	case DDZmsgToView_time:	//--时钟变化
		timeConreolDral(_int);
		break;
	case DDZmsgToView_outCard:	//--出牌表现
		outCardDeal(_int);
		break;
	case DDZmsgToView_passCard:	//--过牌表现
		passCardDeal(_int);
		break;
	case DDZmsgToView_gameOver:	//--游戏结束表现
		gameOverDeal(_int);
		break;
	case DDZmsgToView_openCardNum:	//--手里明牌按钮倍数变化
		{
			DDZCCviewOpenCardNum* _num =(DDZCCviewOpenCardNum*) _int;
			m_menuLayer->setVisibleStartNumber(_num->m_num);
		}
		break;
	case DDZmsgToView_beiLv:	//--倍率变化表现
		{
			DDZCCbeilv*_beiData=(DDZCCbeilv*)_int;
			m_cardTableLayer->setMultiplyingPower(_beiData->m_base,_beiData->m_beilv);
			m_cardTableLayer->setDiFen(_beiData->m_base);
		}
		break;
	case DDZmsgToview_TimeCallBack:	//--时钟回调
		timeCallBackDeal(_int);
		break;
	case DDZmsgToView_OnLine:	//--在线表现
		{
			DDZCConline* _onLineData =(DDZCConline*)_int;
			m_cardTableLayer->setUserIsOnline(_onLineData->m_userID,_onLineData->m_isOnline);
		}
		break;
	case DDZmsgToView_Reconnect:	//--断线重连
		{
			m_cardTableLayer->waitingMatching(false);
		}
		break;
	case DDZmsgToView_socketoff:	//--网络断开
		{
			log("DDZSceneView::OnMessage	case DDZmsgToView_socketoff");
			/*
			if (m_dialogLayer->getChildrenCount()>0&&m_dialogLayer->getChildren().at(0))//->objectAtIndex(0)
			{
				DDZDialogLayer * dialog = (DDZDialogLayer *)m_dialogLayer->getChildren().at(0);
				if (DDZdialog_dismissTable==dialog->m_type)
				{
					break;
				}
			}
			*/
			m_dialogLayer->removeAllChildren();
			DDZDialogLayer * dialog = DDZDialogLayer::create(DDZdialog_hints);
			m_dialogLayer->addChild(dialog);

		}
		break;
	case DDZmsgToView_socketoffConnect:	//--自动重连
		{
			m_effectView->runEffect(Effect_LlinkOFF,1);
			schedule(schedule_selector(DDZSceneView::onLlinkOFF),2.0f,1,2.0f);
		}
		break;
	case DDZmsgToView_DDZVoiceAndFace:	//--语音变化
		{
			DDZCCfaceAndVioce* _face=(DDZCCfaceAndVioce*)_int;
			if(_face->m_Type == 2 && _face->m_KeyID < 0)
			{
				string voiceString = _face->voice;
				m_effectView->runVoice(voiceString, _face->m_UserID);
			}else
			{
				m_effectView->runDDZVoiceAndFace(_face->m_Type,_face->m_KeyID,_face->m_UserID);
			}
		}
		break;
	case DDZmsgToview_LordView:	//--地主表现
		{
			DDZCClandView* _landView=(DDZCClandView*)_int;
			m_cardTableLayer->setLordIcon(_landView->m_landUserID);
			if (_landView->m_landUserID == 1)
			{
				m_handCardView->showLoadIconOnPokers(true);
			}
			else
			{
				m_handCardView->showLoadIconOnPokers(false);
			}
		}
		break;
	case DDZmsgToView_SettmentUser:	//结算人物表现
		{
			m_effectView->runSettlementEffect(_int,g_tableConsume,m_cardTableLayer);

		}
		break;
	case DDZmsgToView_huafeidian:
		{
			DDZCChuafeidian* _data=(DDZCChuafeidian*)_int;
			m_effectView->runHfdEffect(_data);
		}
		break;
	case DDZmsgToView_Close:
		{
			m_dialogLayer->removeAllChildren();
			DDZDialogLayer * dialog = DDZDialogLayer::create(DDZdialog_dismissTable);
			m_dialogLayer->addChild(dialog);
		}
		break;
	case DDZmsgToView_RoomLimit:
		{
			DDZCCRoomLimit* _data=(DDZCCRoomLimit*)_int;
			m_dialogLayer->removeAllChildren();
			DDZdialog_type type=DDZdialog_poChan;
			switch(_data->m_RoomLimitKey)
			{
			case 0:
				type=DDZdialog_poChan;
				break;
			case 1:
				type=DDZdialog_roomLimitMin;
				break;
			case 2:
				type=DDZdialog_roomLimitMax;
				break;
			default:
				return;
			}
			DDZDialogLayer * dialog = DDZDialogLayer::create(type);
			m_dialogLayer->addChild(dialog);
		}
		break;
	}
}

void DDZSceneView::userInforDeal( Integer* _data )
{
	DDZCCuserInfor* _inforData=(DDZCCuserInfor*)_data;
	if (_inforData->m_userInforID == EntityMgr::instance()->roomFrame()->getMeUserDate()->dwUserID)//自己
	{
		if (_inforData->m_score > 0 || _inforData->m_name == nullptr || _inforData->m_name == "")
		{
			DDZUserInfoManager::getInstance()->setUserId(_inforData->m_userInforID);
			DDZUserInfoManager::getInstance()->setUserGold(_inforData->m_score);
			DDZUserInfoManager::getInstance()->setGender(_inforData->m_gender);
			std::string name = _inforData->m_name;
			DDZUserInfoManager::getInstance()->setUserName(name);

			//由于玩家状态更新消息，积分更新消息， 都只返回了单独的用户状态，可能导致没有昵称，积分等信息
			m_cardTableLayer->setCoinNumber(_inforData->m_score);
			m_cardTableLayer->setNickName(_inforData->m_name);
		}


		if (_inforData->m_UserState == US_READY)
		{
			m_cardTableLayer->m_pImgMiddleReady->setVisible(true);
		}
		else
		{
			m_cardTableLayer->m_pImgMiddleReady->setVisible(false);
		}
		Tag tag;
		tag.userId = 1;
		tag.gender = _inforData->m_gender;
		SoundEffect::sharedSoundEffect()->initSelfTag(tag);

		m_cardTableLayer->setUserInfor(_inforData);
	}
	else
	{
		//	设置用户信息
		m_cardTableLayer->setUserInfor(_inforData);

		DDZUser* pUser = m_cardTableLayer->getUserByUserID(_inforData->m_userInforID);
		if (pUser)
		{
			if (_inforData->m_UserState <= US_FREE)
			{
				pUser->setUserLeave(_inforData->m_userInforID, true);
			}
			else
			{
				pUser->setUserLeave(_inforData->m_userInforID, false);
			}

			if (_inforData->m_UserState == US_FREE || _inforData->m_UserState == US_OFFLINE)
			{
				//pUser->setVisible(false);
				pUser->m_pImgReady->setVisible(false);
			}

			if (_inforData->m_UserState == US_READY)
			{
				pUser->m_pImgReady->setVisible(true);
			}
			else
			{
				pUser->m_pImgReady->setVisible(false);
			}

			if (pUser->getViewID() == 0)
			{
				UserDefault::getInstance()->setStringForKey("shangJia", _inforData->m_name);
				Tag tag;
				tag.userId = 0;
				tag.gender = _inforData->m_gender;
				SoundEffect::sharedSoundEffect()->initLeftTag(tag);
			}
			else if (pUser->getViewID() == 2)
			{
				UserDefault::getInstance()->setStringForKey("xiaJia", _inforData->m_name);
				Tag tag;
				tag.userId = 2;
				tag.gender = _inforData->m_gender;
				SoundEffect::sharedSoundEffect()->initRightTag(tag);
			}
		}
	}
	dealWaitingState();
}

void DDZSceneView::dealSitDown(Integer* _data)
{
	m_menuLayer->startGame();
	m_cardTableLayer->waitingMatching(false);
	EntityMgr::instance()->getDispatch()->SendPacketWithRequestMyTableUserInfo(DDZGAME_PLAYER);
}

void DDZSceneView::startGameDeal()
{
	// 	m_menuLayer->startGame();
	// 	m_menuLayer->setTimer(1,DDZGame_Start,5,true,0);
	// 	m_cardTableLayer->waitingMatching(false);
}

void DDZSceneView::startGameViewDeal( Integer* _data )
{
	DDZCCopenStart* _openStart =(DDZCCopenStart*)_data;
	m_effectView->runEffect(_openStart->m_bOpenCard?Effect_OpenStart:Effect_gameStart,_openStart->m_bCurrentUser);

	m_handCardView->dealGameStart();
}

void DDZSceneView::onBackTaskDeal( Integer* _data )
{
	DDZCCbackData* _backData=(DDZCCbackData*)_data;
	((DDZViewHandCard*)m_handCardView)->showDiPai(_backData->m_data,3,_backData->m_taskKey,_backData->m_isNew);
}

void DDZSceneView::addViewDeal( Integer* _data )
{
	DDZCCaddView* _addData=(DDZCCaddView*)_data;
	switch(_addData->m_AddType)
	{
	case 1:	//--表现
		m_effectView->runEffect(Effect_jiaBei,_addData->m_addID);
		break;
	case 2:	//--影藏
		m_menuLayer->jiaBei(false);
		break;
	case 3:	//--请求
		m_menuLayer->jiaBei(true);
		break;
	}
}

void DDZSceneView::sendCardDeal( Integer* _data )
{
	DDZCCsendCard* _sendCardData=(DDZCCsendCard*)_data;
	m_handCardView->setHardCard(_sendCardData->m_userID,
		_sendCardData->m_handCardData,
		_sendCardData->m_handCardNum,
		_sendCardData->m_bEffect,
		_sendCardData->m_bOpen);
	if (_sendCardData->m_bEffect==true && _sendCardData->m_userID==1)		//--第一次发牌取消托管
	{
		m_isAgency =false;
		m_effectView->runRobot(false);
	}
	//_sendCardData->m_handCardNum = 2;//test code
	if (_sendCardData->m_handCardNum==2||_sendCardData->m_handCardNum==1)
	{
		m_effectView->runEffect(Effect_alarm,_sendCardData->m_userID);
	}
}

void DDZSceneView::openCardDeal( Integer* _data )
{
	DDZCCopenCard*_openCardData=(DDZCCopenCard*)_data;
	m_effectView->runEffect(Effect_openCard,_openCardData->m_openID);
}

void DDZSceneView::landLordDeal( Integer* _data )
{
	DDZCClandLord* _landLordData = (DDZCClandLord*) _data;
	EffectType type ;
	if (_landLordData->m_LandKey==1)//不叫
	{
		if (m_isLandLord)
		{
			type =Effect_NoLandLord;
		}
		else
		{
			type =Effect_NoRobLord;
		}
	}
	else if (_landLordData->m_LandKey==2)//叫地主
	{
		if (m_isLandLord)//第一个叫，显示叫地主
		{
			type =Effect_LandLord;
			m_isLandLord=false;
		}
		else
		{//抢地主
			type =Effect_RobLord;
		}
	}else if (_landLordData->m_LandKey==3)
	{
		type =Effect_NoRobLord;
	}
	else if (_landLordData->m_LandKey==4)
	{
		type =Effect_RobLord;
	}

	m_effectView->runEffect(type,_landLordData->m_landID);
}

void DDZSceneView::timeConreolDral( Integer* _data )
{
	DDZCCtimeControl* _timeControl = (DDZCCtimeControl*)_data;
	m_userOutCardState = _timeControl->m_timeKey;
	m_gameStae =_timeControl->m_gameState;
	m_timeKey =_timeControl->m_timeKey;
	//根据时钟状态，屏蔽部分按钮点击事件
	if (m_gameStae>=DDZGame_Start/*&&m_gameStae<Game_Over*/)
	{
		m_menuLayer->setRobotEnable(true);
	}
	else
	{
		m_menuLayer->setRobotEnable(false);
	}
	m_menuLayer->setTimer(_timeControl->m_timeUserID,_timeControl->m_gameState,_timeControl->m_timeNum,true,_timeControl->m_timeKey);
	if (_timeControl->m_timeUserID!=1)	//如果不是轮到自己操作，那么不显示菜单按钮
	{
		m_menuLayer->finishClick();
		return;
	}
	BYTE data[20];
	if (m_isAgency || (m_handCardView->getCardData(data)==1 && _timeControl->m_BankUser==1))			//托管或者只有一张牌
	{
		m_isAgency=true;
		schedule(schedule_selector(DDZSceneView::timeAgencyDeal),2.0f);
		return;
	}

	if (_timeControl->m_gameState ==DDZGame_landLord)
	{
		if (_timeControl->m_timeKey<2)
		{
			if (m_isLandLord)
			{
				m_menuLayer->jiaoDiZhu();
			}
			else
			{
				m_menuLayer->qiangDiZhu();
			}
		}
		else
		{
			m_menuLayer->qiangDiZhu();
		}
	}else if (_timeControl->m_gameState ==DDZGame_Run)
	{
		//1地主出牌且不能明牌，0地主首轮出牌能明牌，2上轮最大方，现在第一轮出牌）
		m_menuLayer->outCard(_timeControl->m_timeKey);
		if (_timeControl->m_timeKey==3 && m_handCardView->isNoCardout()==false)
		{
			//提示没有牌打过上家
			m_effectView->runEffect(Effect_noBigCard,1);
		}
	}
	else if (_timeControl->m_gameState == DDZGame_jiaBei)
	{
		//加倍
		/*DDZCCaddView* _addView =DDZCCaddView::create(DDZmsgToView_addView);
		_addView->m_addID = 1;
		_addView->m_AddType =3;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_addView);*/
		m_menuLayer->jiaBei(true);
	}
}

void DDZSceneView::outCardDeal( Integer* _data )
{
	DDZCCoutCard* _outCard =(DDZCCoutCard*)_data;
	((DDZViewHandCard*)m_handCardView)->setOutCard(_outCard->m_outUser,_outCard->m_outkCard,_outCard->m_numCard,_outCard->m_bankUser);
	DDZDDZLogic _logic;
	m_effectView->runCardEffect(_logic.getCardType(_outCard->m_outkCard,_outCard->m_numCard),_outCard->m_outUser,_outCard->m_numCard,m_userOutCardState);
}

void DDZSceneView::passCardDeal( Integer* _data )
{
	DDZCCpassCard* _passCard =(DDZCCpassCard* )_data;
	m_effectView->runEffect(Effect_PassCard,_passCard->m_passUser);
	switch (_passCard->m_passUser)
	{
	case 0:	//	上家
		m_handCardView->clearOutCard(DDZViewChair_Last);
		break;
	case 1:	//	自家
		m_handCardView->clearOutCard(DDZViewChair_Self);
		break;
	case 2:	//	下家
		m_handCardView->clearOutCard(DDZViewChair_Next);
		break;
	default:
		break;
	}
}

void DDZSceneView::gameOverDeal( Integer* _data )
{
	DDZCCgameOver* _gameOver =(DDZCCgameOver*)_data;	
	DDZCMD_S_GameEnd* pGameEndData = _gameOver->m_gameEndData;
	m_cardTableLayer->setUserIsOnline(0, 1);
	m_cardTableLayer->setUserIsOnline(2, 1);
	if (_gameOver->m_isPochang==true)
	{
		//--游戏破产 3秒后退到大厅
		this->runAction(Sequence::create(DelayTime::create(2.0f),CCCallFuncO::create(this,callfuncO_selector(DDZSceneView::onBankrupt),NULL),NULL));
		return;
	}

	//	展示玩家手牌
	int cardDataIdx = 0;
	for (int i = 0; i < DDZGAME_PLAYER; i++)
	{
		int handNum = pGameEndData->cbCardCount[i];
		if (handNum == 0)
		{
			continue;
		}
		int viewChairID = m_pSceneLogic->switchViewChairID(i);
		bool isOpen = viewChairID == 1 ? false : true;
		BYTE handData[MAX_COUNT];
		memcpy(handData, &(pGameEndData->cbHandCardData[cardDataIdx]), sizeof(BYTE) * handNum);
		m_handCardView->setHardCard(
			viewChairID,
			handData,
			handNum,
			false,
			isOpen);
		cardDataIdx += handNum;
	}

	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	auto pScene = Director::getInstance()->getRunningScene();

	DDZSettlement* pSettlement = DDZSettlement::create(pGameEndData, m_pSceneLogic->m_nBankerUser);
	pScene->addChild(pSettlement);
	if (pGameEndData->bChunTian != 0 || pGameEndData->bFanChunTian != 0)
	{
		Sprite* pIMGChunTian = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTitleSpring);
		pScene->addChild(pIMGChunTian);
		pIMGChunTian->setPosition(Vec2(sVSize / 2));

		auto pDelay = DelayTime::create(1.0f);
		auto pFadeOut = FadeOut::create(1.0f);
		auto pRemove = RemoveSelf::create();
		pIMGChunTian->runAction(Sequence::create(pDelay, pFadeOut, pRemove, nullptr));
	}

	//	游戏结束相关处理
	m_menuLayer->gameDDZSettlement();
	dealGameOver();

	//g_tableConsume = (&_gameOver->m_gameEndData)->lGameTax[_gameOver->m_UserID];
}

void DDZSceneView::timeCallBackDeal( Integer* _data )
{
	DDZCCtimeCallBack* _callBackData=(DDZCCtimeCallBack*)_data;
	switch(_callBackData->m_gameState)
	{
	case DDZGame_null:	//	无
		break;
	case DDZGame_Wait:	//	等待状态
		{
			DWORD nUserID = dealWaitingState();
			IUserItem* pItem = EntityMgr::instance()->roomFrame()->getUserItem(nUserID);
			if (pItem)
			{
				EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup(pItem->getTableID(), pItem->getChairID());
				m_menuLayer->closeTimer();
				int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
				if (pItem->getChairID() == nSelfChairID)
				{
					DDZDialogLayer* pDialogLayer = DDZDialogLayer::create(DDZdialog_Reject);
					auto pScene = Director::getInstance()->getRunningScene();
					pScene->addChild(pDialogLayer);
				}
			}

		}
		break;
	case DDZGame_Start:	//	开始
		break;
	case DDZGame_landLord:	//	叫地主
		{
			m_menuLayer->finishClick();
			DDZCClandLord* landord =DDZCClandLord::create(DDZmsgToLogc_LandLord);
			landord->m_isLand = false;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,landord);
		}
		break;
	case DDZGame_jiaBei://加倍
		{
			//加倍倒计时结束，发送不加倍的消息  add by zengxin
			m_menuLayer->finishClick();
		}
		break;
	case  DDZGame_Run:							//--出牌
		{
			if (m_isAgency==false && (++m_AgencyNum>=2))
			{
				m_isAgency=true;
				m_AgencyNum=0;
				m_effectView->runRobot(true);
			}
			if (_callBackData->m_timeKey <=2)		//首次出牌
				((DDZViewHandCard*)m_handCardView)->UserAutoCard();
			else
				((DDZViewHandCard*)m_handCardView)->UserOutCard(m_isAgency?((DDZViewHandCard*)m_handCardView)->setHintCard():false);
		}
		break;
	case DDZGame_Over://游戏结束超时，自动返回大厅
		{
			Director::getInstance()->replaceScene(DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_start,false));
		}
		break;
	default:
		break;
	}
}

void DDZSceneView::receiveMenuClickCallfunc(Object * obj)
{
	Integer * integer = (Integer*)obj;
	switch(integer->getValue())
	{
	case DDZmenu_changeTable:	//换桌
		{
			m_menuLayer->finishClick();
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(DDZmsgToLogic_changeTable));
		}
		break;
	case DDZmenu_visibleStartMenuItem:	//明牌开始
		{
			//m_menuLayer->finishClick();
			//NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(msgToLogic_openStart));
		}
		break;
	case DDZmenu_startMenuItem://普通开始
		{
			m_menuLayer->finishClick();
			m_cardTableLayer->dealGameReady();
			m_handCardView->dealGameReady();
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(DDZmagToLogc_gameStart));
		}
		break;
	case DDZmenu_rechargemenuItem://快速充值
		CCLOG("DDZSceneView recive menu_rechargemenuItem click");
		NotificationCenter::getInstance()->postNotification(MESSAGE_JNICONTROL,Integer::create(MSG_JNI_Recharge));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		EntityMgr::instance()->getDispatch()->sendOrderInfo("tbf",2);
#endif
		break;
	case DDZmenu_outCardMenuItem://出牌
		CCLOG("DDZSceneView recive menu_outCardMenuItem click");
		{
			CCLOG("DDZViewCardEffect recive menu_outCardMenuItem click");
			m_AgencyNum=0;					//--玩家出牌，重置托管轮数
			if(!((DDZViewHandCard*)m_handCardView)->UserOutCard(true))
			{
				//m_menuLayer->outCard(3);
				m_effectView->runEffect(Effect_outcardErr,1);//出牌错误
			}
			else
			{
				//影藏按钮，如果网络延迟 会多次按下出牌
				m_menuLayer->finishClick();
			}
		}
		break;
	case DDZmenu_passOutCardMenuItem:	//不要
		{
			m_AgencyNum=0;					//--玩家出牌，重置托管轮数
			m_menuLayer->finishClick();
			((DDZViewHandCard*)m_handCardView)->UserOutCard(false);
		}
		break;
	case DDZmenu_hintMenuItem:	//提示
		{
			m_AgencyNum=0;					//--玩家出牌，重置托管轮数
			if(!((DDZViewHandCard*)m_handCardView)->setHintCard())
			{
				//--不要
				((DDZViewHandCard*)m_handCardView)->UserOutCard(false);
				m_menuLayer->finishClick();
			}
		}
		break;
	case DDZmenu_jiaoDiZhuItem:	//叫地主
	case DDZmenu_qiangDiZhuItem://抢地主
		{
			m_menuLayer->finishClick();
			DDZCClandLord* landord =DDZCClandLord::create(DDZmsgToLogc_LandLord);
			landord->m_isLand = true;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,landord);
		}
		break;
	case DDZmenu_buJiaoItem://不叫地主e
	case DDZmenu_passQiangDiZhuItem://不抢地主
		{
			m_menuLayer->finishClick();
			CCLOG("DDZViewCardEffect recive menu_passQiangDiZhuItem click");
			DDZCClandLord* landord =DDZCClandLord::create(DDZmsgToLogc_LandLord);
			landord->m_isLand = false;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,landord);
		}
		break;
	case DDZmenu_visibleCardItem://明牌
		{
			//CCviewOpenCardNum* _cardNum= CCviewOpenCardNum::create(msgToLogc_openCard);
			//_cardNum->m_num =2;
			//NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_cardNum);
			//m_menuLayer->hintVisibleCardItem();
		}
		break;
	case DDZmenu_doubleItem://加倍
		{
			//NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(magToLogc_jiabei));
			m_menuLayer->jiaBei(false);
			m_effectView->runEffect(Effect_jiaBei,0);
			m_effectView->runEffect(Effect_jiaBei,1);
			m_effectView->runEffect(Effect_jiaBei,2);
			/*DDZCCaddView* _addView =DDZCCaddView::create(DDZmsgToView_addView);
			_addView->m_addID = switchViewChairID(_daofang->m_bUserDaofanID);
			_addView->m_AddType =1;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_addView);*/
			//test code  后期需改到，在收到加倍阶段结束消息返回后调用
			if (m_bankId == 1)
			{
				//展示出牌按钮
				CCLOG("xin:::::: 我 出牌 按钮显示");
				//--玩家出牌
				DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
				timeControl->m_timeUserID = 1;
				timeControl->m_timeNum	 = 20;
				//DDZGame_Run
				timeControl->m_gameState = DDZGame_Run;
				//if (m_wOpenUser[m_wCurrentUser] )
				//	timeControl->m_timeKey =1;	//不能明牌
				//else
				//	timeControl->m_timeKey   = 0;	//能明牌
				timeControl->m_timeKey =1;	//不能明牌
				timeControl->m_BankUser=m_bankId;
				NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
			}
		}
		break;
	case DDZmenu_unDoubleItem://不加倍
		{
			m_menuLayer->jiaBei(false);
		}
		break;
	case DDZmenu_settlementDetial://结算详情
		{
		}
		break;
	case DDZmenu_again://再来一局
		{
			// 			int a = m_cardTableLayer->getCoinNum();
			// 			if(m_cardTableLayer->getCoinNum()<500)
			// 			{
			// 				//	removeAllDialog();
			// 				m_dialogLayer->removeAllChildren();
			// 				DDZDialogLayer * dialog = DDZDialogLayer::create(dialog_poChan);
			// 				m_dialogLayer->addChild(dialog);
			// 				
			// 				break;
			// 			}
			CCLOG("DDZSceneView recive menu_again click");
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(DDZmsgToLogic_MoreGame));
		}
		break;
	case DDZmenu_menuBarRobotItem://菜单栏托管按钮
		{
			if (m_isAgency)
			{
				return;
			}
			CCLOG("DDZSceneView recive menu_menuBarRobotItem click");
			//if(m_gameStae>=Game_Start)//达到抢地主的状态后才能开始进入托管
			//{
			m_isAgency = true;
			m_AgencyNum=0;
			m_effectView->runRobot(true);
			if (m_menuLayer->isShowBtns())
			{
				m_menuLayer->finishClick();
				schedule(schedule_selector(DDZSceneView::timeAgencyDeal),0.5f);
			}

			//schedule(schedule_selector(DDZSceneView::timeAgencyDeal),2.0f);//--机器人代打
			//}
		}
		break;
	case DDZmenu_number:	//明牌开始按钮上的数字
		CCLOG("DDZSceneView recive menu_number click");
		break;
	case DDZmenu_visibleCardStartItem:	//发牌过程中的明牌开始按钮
		{
			m_menuLayer->finishClick();
			DDZCCviewOpenCardNum* _cardNum= DDZCCviewOpenCardNum::create(DDZmsgToLogc_openCard);
			_cardNum->m_num =((DDZCCviewOpenCardNum *)integer)->m_num;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_cardNum);
			m_menuLayer->m_isVisibleStart = true;
		}
		break;
	case DDZmenu_OutAgency:				//--取消托管
		{
			m_isAgency =false;
		}
		break;
	}
}

void DDZSceneView::timeAgencyDeal(float ft)
{
	unschedule(schedule_selector(DDZSceneView::timeAgencyDeal));
	DDZCCtimeCallBack* _timeCallBack=DDZCCtimeCallBack::create(0);
	_timeCallBack->m_gameState =m_gameStae;
	_timeCallBack->m_timeKey =m_timeKey;
	timeCallBackDeal(_timeCallBack);  //--机器人代打
}

//	坐下失败
void DDZSceneView::onSitFaild(Object* obj)
{
	CCLOG("SceneView::onSitFaild");
	tagRequestFailureObj* pObj = (tagRequestFailureObj*)obj;
	char str[256]={0};
	MyNSString::wc2mbs(pObj->szDescribeString, CountArray(pObj->szDescribeString), str);
	/*
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str, ccp(SCREEN_HEIGHT/2,SCREEN_WIDTH/2));
	this->addChild(layer,20);
	*/
	DDZDialogLayer* pDialogLayer = DDZDialogLayer::create(DDZdialog_ChangeTableFailed);
	addChild(pDialogLayer);
	pDialogLayer->setTipsString(str);
}

void DDZSceneView::onLoginFaild(Object* obj)
{
	CCLOG("DDZSceneView::onLoginFaild");
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	tagRoomLogonFailObj* pObj = (tagRoomLogonFailObj*)obj;
	int DataSize=128;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);
	PromptBox* promptBox = PromptBox::PromptBoxWith(Vec2(sVSize.width * 0.5, sVSize.height * 0.5), mPromptTypeGetError);
	promptBox->setPromptText(str);
	this->addChild(promptBox,1);
}

void DDZSceneView::onLlinkOFF( float ft )
{
	unschedule(schedule_selector(DDZSceneView::onLlinkOFF));
	DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_endAgain,false);
}

void DDZSceneView::wangZha(Object *obj)
{
	((DDZViewHandCard*)m_handCardView)->newRound();
}

void DDZSceneView::dealGameOver()
{
	m_menuLayer->closeTimer();
	m_isAgency=false;
	m_effectView->runRobot(false);
	//m_gameStae = Game_null;
	m_gameStae = DDZGame_Over;
	m_timeKey=0;
	m_menuLayer->setRobotEnable(false);
	// 	m_menuLayer->setTalkEnable(false);
	/*
	if (!m_face->getIsOut())
	{
	m_face->runOutOrIn();
	}
	*/

	//	清除警报
	m_effectView->m_leftAlarm = false;
	m_effectView->m_rightAlarm = false;
	auto pLastAlarm = m_effectView->getChildByName(DDZ_ALARM_NAME_LAST);
	if (pLastAlarm)
	{
		pLastAlarm->removeFromParent();
	}
	auto pNextAlarm = m_effectView->getChildByName(DDZ_ALARM_NAME_NEXT);
	if (pNextAlarm)
	{
		pNextAlarm->removeFromParent();
	}

	//	清除底牌
	memset(m_pSceneLogic->m_BackCardData, 0, sizeof(BYTE)*DDZGAME_PLAYER);

	m_isLandLord = true;

	m_handCardView->dealGameOver();
	m_cardTableLayer->dealGameOver();

}

DWORD DDZSceneView::dealWaitingState()
{
	if (m_pSceneLogic->getGameState() != DDZGame_Wait)
	{
		//	不是等待阶段，返回
		return -1;
	}

	vector<IUserItem*> vSaveList, vRemoveList;
	int nSelfTableID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wTableID;
	for (int i = 0; i < DDZGAME_PLAYER; i++)
	{
		IUserItem* pItem = EntityMgr::instance()->roomFrame()->getUserItemByChairID(i);
		if (!pItem)
		{
			continue;
		}
		if (pItem->getTableID() != nSelfTableID)
		{
			//	不与自身玩家同在一牌桌上，过滤
			continue;
		}
		if (pItem->getUserData()->cbUserStatus == US_READY)
		{
			vSaveList.push_back(pItem);
		}
		else
		{
			vRemoveList.push_back(pItem);
		}
	}

	//	当且仅当2个已准备状态，1个不是准备状态的情况下，需要踢人
	if (vSaveList.size() == 2 && vRemoveList.size() == 1)
	{
		IUserItem* pItem = vRemoveList.at(0);
		int nChairID = DDZSceneLogic::switchViewChairID(pItem->getChairID());
		m_menuLayer->setTimer(nChairID, DDZGame_Wait, 30, true, 1);
		return pItem->getUserID();
	}
	else
	{
		m_menuLayer->closeTimer();
	}
	return -1;
}

void DDZSceneView::setIsFistLandLord(bool first)
{
	this->m_isLandLord = first;
}





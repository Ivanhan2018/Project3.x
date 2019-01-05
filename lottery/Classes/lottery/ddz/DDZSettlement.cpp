#include "DDZSettlement.h"
#include "DDZSceneData.h"
#include "DDZSoundControl.h"
#include "SoundControl.h"//<by hxh>
#include "EntityMgr.h"
#include "DDZSceneLogic.h"

USING_NS_CC;

DDZSettlement::DDZSettlement()
{
}

DDZSettlement::~DDZSettlement()
{

}

DDZSettlement* DDZSettlement::create(DDZCMD_S_GameEnd* sGameData, int nLandlordID)
{
	DDZSettlement* pRet = new DDZSettlement();
	if (pRet && pRet->init(sGameData, nLandlordID))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

void DDZSettlement::onClickBtnClose(Ref* pSender)
{
	this->removeFromParent();
}

bool DDZSettlement::init(DDZCMD_S_GameEnd* sGameData, int nLandlordID)
{
	bool ret = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

		//	背景
		m_pBG = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzBGSummary);
		addChild(m_pBG);
		m_pBG->setPosition(Vec2(sVSize / 2));
		cocos2d::Size sBGSize = m_pBG->getContentSize();

		//close
		//auto btnClose = ui::Button::create(DDZtexture_name::s_btnOKNormal, DDZtexture_name::s_btnOKPressed, "", ui::Widget::TextureResType::PLIST);
		//m_pBG->addChild(btnClose);
		//btnClose->addClickEventListener(CC_CALLBACK_1(DDZSettlement::onClickBtnClose, this));
		//m_pBG->setPosition(Vec2(sBGSize.width - 0, sBGSize.height * 0.25));
		//
		//关闭对话框按钮
		Sprite* closeNormal = Sprite::create("dt_guanbibig.png");
		Sprite* closeSelect = Sprite::create("dt_guanbibig.png");
		MenuItemSprite* closeI = MenuItemSprite::create(closeNormal,closeSelect,CC_CALLBACK_1(DDZSettlement::onClickBtnClose,this));
		//closeSelect->setScale(0.9f);
		//closeSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu* closeMenu = Menu::create(closeI,NULL);
		closeMenu->setPosition(Vec2(m_pBG->getContentSize().width - closeNormal->getContentSize().width * 0.5+10,m_pBG->getContentSize().height - closeNormal->getContentSize().height * 0.5+10));
		m_pBG->addChild(closeMenu);
		//

		int nSelfChairID =  DDZSceneLogic::switchViewChairID(EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID);
		for (int i = 0; i < DDZGAME_PLAYER; i++)
		{
			int nChairID = DDZSceneLogic::switchViewChairID(i);
			//	用户信息
			tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getUserItemByChairID(i)->getUserData();

			//	玩家头像
			if (nChairID == nLandlordID)
			{
				if (pUserData->cbGender == 1)
				{
					m_pImgPlayer[nChairID] = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzPlayerLandlordMan);
				}
				else
				{
					m_pImgPlayer[nChairID] = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzPlayerLandlordWoman);
				}
			}
			else
			{
				if (pUserData->cbGender == 1)
				{
					m_pImgPlayer[nChairID] = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzPlayerPeasantMan);
				}
				else
				{
					m_pImgPlayer[nChairID] = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzPlayerPeasantWoman);
				}
			}
			m_pBG->addChild(m_pImgPlayer[nChairID]);
			m_pImgPlayer[nChairID]->setPosition(Vec2(sBGSize.width * 0.17, sBGSize.height * (0.72 - nChairID * 0.2)));

			//	胜负标识
			if (sGameData->cbCardCount[i] == 0)
			{
				if (nChairID == nLandlordID)
				{
					if (nChairID == nSelfChairID)
					{
						m_pImgText = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTextWin);
						m_pTitle = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTitleWin);
					}
					else
					{
						m_pImgText = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTextLose);
						m_pTitle = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTitleLose);
					}
				}
				else
				{
					if (nSelfChairID != nLandlordID)
					{
						m_pImgText = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTextWin);
						m_pTitle = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTitleWin);
					}
					else
					{
						m_pImgText = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTextLose);
						m_pTitle = Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddzTitleLose);
					}
				}
				//	胜负标识
				m_pBG->addChild(m_pImgText, 999);
				m_pImgText->setPosition(Vec2(sBGSize.width * 0.12, sBGSize.height * 0.5));

				//	标题
				m_pBG->addChild(m_pTitle);
				m_pTitle->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 1.13));
			}

			//	炸弹数字
			m_pLabBomb[nChairID] = Label::create();
			m_pBG->addChild(m_pLabBomb[nChairID]);
			m_pLabBomb[nChairID]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			m_pLabBomb[nChairID]->setPosition(Vec2(sBGSize.width * 0.75, sBGSize.height * (0.685 - nChairID * 0.2)));

			m_pLabBomb[nChairID]->setSystemFontSize(30);
			m_pLabBomb[nChairID]->setTextColor(Color4B::WHITE);
			char cBombNum[32];
			sprintf(cBombNum, "X %d", sGameData->cbEachBombCount[i]);
			m_pLabBomb[nChairID]->setString(cBombNum);

			//	得分数字
			m_pLabOnceScore[nChairID] = Label::create();
			m_pBG->addChild(m_pLabOnceScore[nChairID]);
			m_pLabOnceScore[nChairID]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			m_pLabOnceScore[nChairID]->setPosition(Vec2(sBGSize.width * 0.355, sBGSize.height * (0.675 - nChairID * 0.2)));
			m_pLabOnceScore[nChairID]->setSystemFontSize(22);
			m_pLabOnceScore[nChairID]->setTextColor(Color4B::WHITE);
			char cScoreNum[32];
			sprintf(cScoreNum, "%.0f", sGameData->lGameScore[i]);
			m_pLabOnceScore[nChairID]->setString(cScoreNum);

			//	玩家昵称
			m_pLabPlayerName[nChairID] = Label::create();
			m_pBG->addChild(m_pLabPlayerName[nChairID]);
			m_pLabPlayerName[nChairID]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			m_pLabPlayerName[nChairID]->setPosition(Vec2(sBGSize.width * 0.355, sBGSize.height * (0.76 - nChairID * 0.2)));
			m_pLabPlayerName[nChairID]->setSystemFontSize(22);
			m_pLabPlayerName[nChairID]->setTextColor(Color4B::WHITE);
			m_pLabPlayerName[nChairID]->setString(pUserData->szNickName);
		}

		//	底分数字
		m_pLabDiFen = Label::create();
		m_pBG->addChild(m_pLabDiFen);
		m_pLabDiFen->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		m_pLabDiFen->setPosition(Vec2(sBGSize.width * 0.355, sBGSize.height * 0.15));
		m_pLabDiFen->setSystemFontSize(30);
		m_pLabDiFen->setTextColor(Color4B::WHITE);
		//char cDiFenNum[32];
		//sprintf(cDiFenNum, "%d", sGameData->cbBankerScore);
		LONG difen = sGameData->lCellScore;
		m_pLabDiFen->setString(StringUtils::toString(difen));


		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(DDZSettlement::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(DDZSettlement::onTouchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(DDZSettlement::onTouchEnded, this);
		listener->onTouchCancelled = CC_CALLBACK_2(DDZSettlement::onTouchCancelled, this);
		listener->setSwallowTouches(true);

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		ret = true;
	} while (0);

	return ret;
}

bool DDZSettlement::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZSettlement::onTouchBegan");
	return true;
}

void DDZSettlement::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZSettlement::onTouchMoved");
}

void DDZSettlement::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZSettlement::onTouchEnded");
	Vec2 vTouchPiont = pTouch->getLocation();
	if (!m_pBG->getBoundingBox().containsPoint(vTouchPiont))
	{
		this->removeFromParent();
	}
}

void DDZSettlement::onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	log("DDZSettlement::onTouchCancelled");
}

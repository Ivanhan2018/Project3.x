#include "NCMJGameScence.h"
#include "GameLib.h"
#include "NCMJPlayer.h"
#include "NCMJGameLogic.h"

void NCMJGameScence::updateUserRight()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// 控制按钮 536870912
	if (!WidgetFun::getChildWidget(this,"NCMJButton_Master"))
	{
		return;
	}
	if( CUserRight::IsGameCheatUser(UserInfo::Instance().getUserRight()))
	{
		WidgetFun::setVisible(this,"NCMJButton_Master",true);
	}
	else
	{
		WidgetFun::setVisible(this,"NCMJButton_Master",false);
	}
#endif
}

void NCMJGameScence::initMaster()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (WidgetFun::getChildWidget(this,"GameMaterPlane"))
	{
		return;
	}
	WidgetScenceXMLparse kScence("GameNCMJ/xml/MasterGameScence.xml",this);

	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(this,"MasterCardNode");

	cocos2d::Vec2 kAddPosX = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"HandAddPosX"));
	cocos2d::Vec2 kAddPosY = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"HandAddPosY"));
	std::string kImagic = WidgetFun::getWidgetUserInfo(pCardNode,"MingImagic");

	cocos2d::Vec2 startPos = cocos2d::Vec2(0,100);
	for (int i=0;i<MAX_INDEX;i++)
	{
		if (i%9 == 0)
		{
			startPos.y += kAddPosY.y;
			startPos.x = 0;
		}
		BYTE iCard = NCMJ::CGameLogic::Instance().SwitchToCardData(i);
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("NCMJ_MASTER_LEFTCARD",pCardNode);

		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,"HuScale"));
		pNode->setScale(fHuScale);

		BYTE cbValue=((BYTE)iCard&MASK_VALUE);
		BYTE cbColor=(((BYTE)iCard&MASK_COLOR)>>4)+1;
		WidgetFun::setButtonImagic(pNode,utility::toString(kImagic,(int)cbColor,(int)cbValue,".png"),true);
		WidgetFun::setWidgetUserInfo(pNode,"MasterCardData",utility::toString((int)iCard));
		WidgetFun::setWidgetUserInfo(pNode,"MasterCardindex",utility::toString((int)i));

		pNode->setPosition(startPos);
		startPos +=kAddPosX;
	}
#endif
}

void NCMJGameScence::showMater()
{
	initMaster();
	WidgetFun::setVisible(this,"GameMaterPlane",true);
	SendSocketData(SUB_C_MASTER_LEFTCARD,0,0);

	WidgetFun::setVisible(this,"MasterCheakCard",false);
}

void NCMJGameScence::defaultMaster()
{
	WidgetFun::setVisible(this,"NCMJButton_Master",false);
	updateUserRight();
}

void NCMJGameScence::NCMJButton_Master(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	showMater();
	for (int i=0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setMingPai(true);
		m_pPlayer[i]->showHandCard();
	}
}

void NCMJGameScence::Button_MasterClose(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	defaultMaster();
	for (int i=0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setMingPai(false);
		m_pPlayer[i]->showHandCard();
	}
}

void NCMJGameScence::OnMasterHandCard( const void *pBuffer, WORD wDataSize )
{
	if (!IsInGame())
	{
		return;
	}
	datastream kStream(const_cast<void *>(pBuffer),wDataSize);
	MasterHandCard kMasterHandCard;
	kMasterHandCard.StreamValue(kStream,false);

	ASSERT(kMasterHandCard.kMasterHandCardList.size() == GAME_PLAYER);

	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_pPlayer[i]->setMaskHandCard(kMasterHandCard.kMasterHandCardList);	
	}
}

void NCMJGameScence::OnMasterLeftCard( const void *pBuffer, WORD wDataSize )
{
	if (!IsInGame())
	{
		return;
	}
	//效验数据
	ASSERT(wDataSize==sizeof(MasterLeftCard));
	if (wDataSize!=sizeof(MasterLeftCard)) 
		return;
	
	MasterLeftCard* pMasterLeft =(MasterLeftCard*)pBuffer; 

	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(this,"MasterCardNode");
	for (int i =0;i<MAX_INDEX;i++)
	{
		cocos2d::Node* pNode = pCardNode->getChildren().at(i);
		int nCardIndex = utility::parseInt(WidgetFun::getWidgetUserInfo(pNode,"MasterCardindex"));
		if (nCardIndex == i)
		{
			WidgetFun::setText(pNode,"LeftNum",(int)pMasterLeft->kMasterLeftIndex[i]);
		}
		WidgetFun::setButtonEnabled(pNode,pMasterLeft->kMasterLeftIndex[i]>0);
	}

	setMasterCheakCard(pMasterLeft->kMasterCheakCard);
	
}
void NCMJGameScence::NCMJ_MASTER_LEFTCARD(cocos2d::Ref*,WidgetUserInfo* pUseInfo)
{
	if (!IsInGame())
	{
		return;
	}
	BYTE iCard = utility::parseInt(WidgetFun::getUserInfoValue(pUseInfo,"MasterCardData"));
	setMasterCheakCard(iCard);

	MaterCheckCard kMaterCheckCard;
	kMaterCheckCard.cbCheakCard = iCard;

	SendSocketData(SUB_C_MASTER_CHEAKCARD,&kMaterCheckCard,sizeof(kMaterCheckCard));
}

void NCMJGameScence::setMasterCheakCard(BYTE cbCard)
{
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(this,"MasterCheakCard");
	if (cbCard<0)
	{
		pCardNode->setVisible(false);
		return;
	}
	pCardNode->setVisible(true);
	std::string kImagic = WidgetFun::getWidgetUserInfo(pCardNode,"MingImagic");
	m_pLocal->setCardImagic(pCardNode,cbCard,kImagic);
}
#include "XZDDGameScence.h"
#include "GameLib.h"
#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"

void XZDDGameScence::updateUserRight()
{
	// 控制按钮 536870912
	if (!WidgetFun::getChildWidget(this,"Button_Master"))
	{
		utility::log("XZDDGameScence::updateUserRight()");
		return;
	}
	if( CUserRight::IsGameCheatUser(UserInfo::Instance().getUserRight()))
	{
		WidgetFun::setVisible(this,"Button_Master",true);
	}
	else
	{
		WidgetFun::setVisible(this,"Button_Master",false);
	}
}

void XZDDGameScence::initMaster()
{
	if (WidgetFun::getChildWidget(this,"GameMaterPlane"))
	{
		return;
	}
	WidgetScenceXMLparse kScence("GameXZDD/xml/MasterGameScence.xml",this);

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
		BYTE iCard = XZDD::CGameLogic::Instance().SwitchToCardData(i);
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("NHMJ_MASTER_LEFTCARD",pCardNode);

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
}

void XZDDGameScence::showMater()
{
	initMaster();
	WidgetFun::setVisible(this,"GameMaterPlane",true);
	SendSocketData(SUB_C_MASTER_LEFTCARD,0,0);
	WidgetFun::setVisible(this,"MasterCheakCard",false);
}

void XZDDGameScence::defaultMaster(bool bRestZhaNiao)
{
	if (WidgetFun::getChildWidget(this,"GameMaterPlane"))
	{
		if (bRestZhaNiao)
		{
			for (int i = 1;i<7;i++)
			{
				WidgetFun::setChecked(this,utility::toString("MasterZhaNiao",i),false);
			}
		}
		WidgetFun::setVisible(this,"GameMaterPlane",false);
	}
	updateUserRight();
}

void XZDDGameScence::HNMJButton_Master(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	showMater();
}

void XZDDGameScence::Button_MasterClose(cocos2d::Ref*,WidgetUserInfo*)
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

void XZDDGameScence::OnMasterHandCard( const void *pBuffer, WORD wDataSize )
{
	if (!IsInGame())
	{
		return;
	}
	datastream kStream(const_cast<void *>(pBuffer),wDataSize);
	XZDD_MasterHandCard kMasterHandCard;
	kMasterHandCard.StreamValue(kStream,false);

	ASSERT(kMasterHandCard.kMasterHandCardList.size() == GAME_PLAYER);

	for (int i=0;i<GAME_PLAYER;i++)
	{
		if (m_pPlayer[i] == m_pLocal)
		{
			continue;
		}
		m_pPlayer[i]->setMaskHandCard(kMasterHandCard.kMasterHandCardList);	
	}
	for (int i=0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->setMingPai(true);
		m_pPlayer[i]->showHandCard();
	}
}

void XZDDGameScence::OnMasterLeftCard( const void *pBuffer, WORD wDataSize )
{
	if (!IsInGame())
	{
		return;
	}
	//效验数据
	ASSERT(wDataSize==sizeof(XZDD_MasterLeftCard));
	if (wDataSize!=sizeof(XZDD_MasterLeftCard)) 
		return;
	
	XZDD_MasterLeftCard* pMasterLeft =(XZDD_MasterLeftCard*)pBuffer; 

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
void XZDDGameScence::NHMJ_MASTER_LEFTCARD(cocos2d::Ref*,WidgetUserInfo* pUseInfo)
{
	if (!IsInGame())
	{
		return;
	}
	BYTE iCard = utility::parseInt(WidgetFun::getUserInfoValue(pUseInfo,"MasterCardData"));
	setMasterCheakCard(iCard);

	XZDD_MaterCheckCard kMaterCheckCard;
	kMaterCheckCard.cbCheakCard = iCard;

	SendSocketData(SUB_C_MASTER_CHEAKCARD,&kMaterCheckCard,sizeof(kMaterCheckCard));
}
void XZDDGameScence::MasterZhaNiao(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	BYTE iNiaoCout = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"NiaoCout"));
	BYTE iLastCout = INVALID_BYTE;
	for (int i = 1;i<7;i++)
	{
		if (WidgetFun::isChecked(this,utility::toString("MasterZhaNiao",i)))
		{
			iLastCout = i;
		}
	}
	if (iLastCout == INVALID_BYTE)
	{
		iNiaoCout = INVALID_BYTE;
	}
	for (int i = 1;i<7;i++)
	{
		WidgetFun::setChecked(this,utility::toString("MasterZhaNiao",i),i == iNiaoCout);
	}
	XZDD_MaterNiaoCout kMaterNiaoCout;
	kMaterNiaoCout.cbNiaoCout = iNiaoCout;

	SendSocketData(SUB_C_MASTER_ZHANIAO,&kMaterNiaoCout,sizeof(kMaterNiaoCout));
}

void XZDDGameScence::setMasterCheakCard(BYTE cbCard)
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
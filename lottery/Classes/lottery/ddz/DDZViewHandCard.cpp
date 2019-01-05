#include "DDZViewHandCard.h"
#include "DDZDDZRes.h"
#include "DDZSceneData.h"
#include "DDZDDZLogic.h"
#include "DDZSoundControl.h"
#include "CocosUnits.h"//by hxh
#include "DDZSceneLogic.h"

class CCcardDate:public Integer
{
public:
	CCcardDate(int v):Integer(v){};
	CREATE_FUNC_MSG(CCcardDate);
	bool m_isup;		//是否升起
	bool m_isfocus;		//是否获得焦点
	bool m_isfist;		//是不是选中的第一张牌
	bool m_isselect;	//是否被选中
};

DDZViewHandCard::DDZViewHandCard()
{
	memset(m_outCardData,0,sizeof(m_outCardData));
	m_outCardNum = 0;
	m_isOpenCard=false;
}

DDZViewHandCard::~DDZViewHandCard()
{
	
}

bool DDZViewHandCard::init()
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	bool ret =false;
	do 
	{
		m_unallowToucn = false;
		m_distance = 0;
		m_maxDistance = 0;
		m_isRay = true;
		m_isSelect = false;

		m_isLord = false;

		for(int i=0;i<20;i++)
		{
			m_leftCardNum[i] = 5 - rand()%10;
			m_rightCardNum[i] = 5 - rand()%10;
		}

		//上家手牌
		m_leftCardNode = SpriteBatchNode::create(DDZtexture_name::s_card);
		this->addChild(m_leftCardNode);
		m_leftCardNode->setPosition(Vec2(size.width * 0.25, size.height * 0.65));

		//下家手牌
		m_rightCardNode = SpriteBatchNode::create(DDZtexture_name::s_card);
		this->addChild(m_rightCardNode);
		m_rightCardNode->setPosition(Vec2(size.width * 0.75, size.height * 0.65));

		//上家盖着的扑克牌
		m_hintLeftCardNode = SpriteBatchNode::create(DDZtexture_name::s_cardTable);
		this->addChild(m_hintLeftCardNode);
		m_hintLeftCardNode->setPosition(Vec2(size.width * 0.13, size.height * 0.53));

		//下家盖着的扑克牌
		m_hintRightCardNode = SpriteBatchNode::create(DDZtexture_name::s_cardTable);
		this->addChild(m_hintRightCardNode);
		m_hintRightCardNode->setPosition(Vec2(size.width * 0.87, size.height * 0.53));

		//自己手中的牌
		m_handCardNode = Node::create();
		this->addChild(m_handCardNode);

		m_outCardNodeL = Node::create();
		this->addChild(m_outCardNodeL);
		m_outCardNodeL->setPosition(Vec2(size.width*0.3f, size.height*0.75f));

		m_outCardNodeM = Node::create();
		this->addChild(m_outCardNodeM);
		m_outCardNodeM->setPosition(Vec2(size.width*0.5f, size.height*0.65f));

		m_outCardNodeR = Node::create();
		this->addChild(m_outCardNodeR);
		m_outCardNodeR->setPosition(Vec2(size.width*0.7f, size.height*0.75f));

		m_viewTask = DDZViewTask::create();									//底牌任务
		this->addChild(m_viewTask);
		
		//底牌（地主牌）
		m_diPaiNode = Node::create();
		this->addChild(m_diPaiNode);

		m_pDDZLeftCardNum = Label::createWithBMFont(DDZtexture_name::s_fntDDZTimeNum, "");
		addChild(m_pDDZLeftCardNum);
		m_pDDZLeftCardNum->setPosition(Vec2(size.width * 0.125, size.height * 0.48));

		m_pDDZRightCardNum = Label::createWithBMFont(DDZtexture_name::s_fntDDZTimeNum, "");
		addChild(m_pDDZRightCardNum);
		m_pDDZRightCardNum->setPosition(Vec2(size.width * 0.875, size.height * 0.48));

		setTouchEnabled(true);
		setTouchMode(kCCTouchesOneByOne);
		ret = true;
	} while (0);
	return ret;
}

bool DDZViewHandCard::onTouchBegan( Touch *pTouch, Event *pEvent )
{
	if (m_unallowToucn)
	{
		return true;
	}
	m_origin = pTouch->getLocation().x;
	Vec2 pt= pTouch->getLocation();
	cocos2d::Size size= Director::getInstance()->getWinSize();
	Object* pObj=NULL;
	//逆向
	//Array_FOREACH_REVERSE(m_handCardNode->getChildren(),pObj)
	for(int i = m_handCardNode->getChildrenCount() - 1; i >= 0;i --)
	{
		//Sprite* pNode=(Sprite*)pObj;
		Sprite * pNode=(Sprite*)m_handCardNode->getChildren().at(i);
		if (pNode && pNode->boundingBox().containsPoint(pt))
		{
			CCcardDate* cardObj = (CCcardDate*)(pNode->getUserObject());
			cardObj->m_isselect = !cardObj->m_isselect;
			cardObj->m_isfist = true;
			cardObj->m_isfocus = true;
			m_isSelect = true;
			pNode->setColor(ccc3(220,220,220));
// 			float _h = (cardObj->m_isup)?0.20f:0.15f;
// 			pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
			break;
		}
	}
	return true;
}

void DDZViewHandCard::onTouchMoved( Touch *pTouch, Event *pEvent )
{
	if(!m_isSelect) return;
	m_distance = abs(pTouch->getLocation().x-m_origin);
	if (m_distance>=m_maxDistance)
	{
		m_isRay = true;
	}else{
		m_isRay = false;
	}
	m_maxDistance = m_distance;
	

	Vec2 pt= pTouch->getLocation();
	cocos2d::Size size= Director::getInstance()->getWinSize();
	Object* pObj=NULL;
	//逆向
	int num = 0;
	//Array_FOREACH_REVERSE(m_handCardNode->getChildren(),pObj)
	for(int i = m_handCardNode->getChildrenCount() - 1;i >= 0;i --)
	{
		//Sprite* pNode=(Sprite*)pObj;
		Sprite * pNode = (Sprite*)m_handCardNode->getChildren().at(i);
		if (pNode && pNode->boundingBox().containsPoint(pt))
		{
			num++;
			CCcardDate* cardObj = (CCcardDate*)(pNode->getUserObject());
			if(!cardObj->m_isfocus)
			{
				//Object * pObj2 = NULL;
				//Array_FOREACH_REVERSE(m_handCardNode->getChildren(),pObj2)
				for(int j = m_handCardNode->getChildrenCount() - 1;j >= 0;j --)
				{
					//Sprite * pNode2 = (Sprite*)pObj2;
					Sprite * pNode2 = (Sprite*)m_handCardNode->getChildren().at(j);
					CCcardDate * cardObj2 = (CCcardDate*)pNode2->getUserObject();
					//收缩状态失去焦点，取消选中扑克牌
					if (!m_isRay&&cardObj2->m_isfocus)
					{
						cardObj2->m_isselect = !cardObj2->m_isselect;
						if (!cardObj2->m_isselect)
						{
							pNode2->setColor(ccc3(255,255,255));
						}else{
							pNode2->setColor(ccc3(220,220,220));
						}
// 						float _h = (cardObj2->m_isup)?0.20f:0.15f;
// 						pNode2->runAction(MoveTo::create(0.1f,ccp(pNode2->getPositionX(),_h*size.height)));
					}
					cardObj2->m_isfocus = false;
					
				}
				cardObj->m_isfocus = true;
				if(!cardObj->m_isfist&&m_isRay)
				{
					cardObj->m_isselect = !cardObj->m_isselect;
					if (!cardObj->m_isselect)
					{
						pNode->setColor(ccc3(255,255,255));
					}else{
						pNode->setColor(ccc3(220,220,220));
					}
// 					float _h = (cardObj->m_isup)?0.20f:0.15f;
// 					pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
				}
				
			}
			break;
		}
	}
	if (0==num)
	{
		m_isSelect = false;
	}

}

void DDZViewHandCard::onTouchEnded( Touch *pTouch, Event *pEvent )
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_isSelect = false;
	bool isAllUp=true;
	//移除焦点
	Object * pObj2 = NULL;
	int MoveNum =0;
	BYTE  Card[20];
	memset(Card,0,sizeof(BYTE)*20);
	//Array_FOREACH(m_handCardNode->getChildren(),pObj2)
	for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
	{
		Sprite * pNode2 = (Sprite*)m_handCardNode->getChildren().at(i);
		CCcardDate * cardObj2 = (CCcardDate*)pNode2->getUserObject();
		pNode2->setColor(ccc3(255,255,255));
		if (cardObj2->m_isselect)
		{
			if(cardObj2->m_isup==false)isAllUp=false;
			cardObj2->m_isup=!cardObj2->m_isup;
			Card[MoveNum++]=cardObj2->getValue();
			float _h = (cardObj2->m_isup)?0.15f:0.12f;
			pNode2->stopAllActions();
			pNode2->runAction(MoveTo::create(0.1f,ccp(pNode2->getPositionX(),_h*size.height)));
		}
		cardObj2->m_isfocus = false;
		cardObj2->m_isfist = false;
		cardObj2->m_isselect = false;
	}

	if (isAllUp==true)return;
	if (MoveNum<=1)return;

	DDZDDZLogic m_GameLogic;
	int MoveUpNum =0;
	BYTE  MoveUpCard[20];
	memset(MoveUpCard,0,sizeof(BYTE)*20);
	do 
	{
		if (m_outCardNum==0)
		{
			DDZtagAnalyseResult  AnalyseResult;
			memset(&AnalyseResult,0,sizeof(AnalyseResult));
			m_GameLogic.analysebCardData(Card,MoveNum,AnalyseResult);

			//优先选择炸弹
			if (AnalyseResult.cbFourCount>=1)
			{
				MoveUpNum=0;
				for (int i=0;i<AnalyseResult.cbFourCount;i++)
				{
					MoveUpCard[MoveUpNum++]=AnalyseResult.cbFourCardData[i];
				}
				break;
			}

			//飞机
			int index=0;
			int key=0;
			BYTE  tempCard[20];
			memset(tempCard,0,20);
			MoveUpNum=0;
			if (AnalyseResult.cbThreeCount>=2)
			{
				for (int i=0;i<AnalyseResult.cbThreeCount-1;i++)
				{
					tempCard[key++]=AnalyseResult.cbThreeCardData[i*3];
					tempCard[key++]=AnalyseResult.cbThreeCardData[i*3+1];
					tempCard[key++]=AnalyseResult.cbThreeCardData[i*3+2];
					for (int j=i+1;j<AnalyseResult.cbThreeCount;j++)
					{

						if (m_GameLogic.getCardLogicValue(AnalyseResult.cbThreeCardData[(j-1)*3])-
							m_GameLogic.getCardLogicValue(AnalyseResult.cbThreeCardData[j*3])==1)
						{
							tempCard[key++]=AnalyseResult.cbThreeCardData[j*3];
							tempCard[key++]=AnalyseResult.cbThreeCardData[j*3+1];
							tempCard[key++]=AnalyseResult.cbThreeCardData[j*3+2];
							index++;
						}else{
							break;
						}
					}
					if (index>=1)
					{
						memcpy(MoveUpCard,tempCard,sizeof(BYTE)*20);
						MoveUpNum=key;
						break;
					}
				}
				if (MoveUpNum>=6)break;
			}

			//顺子
			if (AnalyseResult.cbSignedCount+AnalyseResult.cbDoubleCount+
				AnalyseResult.cbThreeCount+AnalyseResult.cbFourCount>=5||
				AnalyseResult.cbDoubleCount+AnalyseResult.cbThreeCount+AnalyseResult.cbFourCount>=3)
			{
				for (int i=0;i<MoveNum-2;i++)
				{
					for (int j=MoveNum-1;j>i;j--)
					{
						BYTE ItoJ[20];
						memset(ItoJ,0,20);
						memcpy(ItoJ,Card+i,j-i+1);
						if(true==OnLianduiWidom(ItoJ,j-i+1,MoveUpCard,MoveUpNum))return;
						if(true==OnShunziWidom(ItoJ,j-i+1,MoveUpCard,MoveUpNum))return;
					}
				}
			}

			//三张
			if (AnalyseResult.cbThreeCount>=1)
			{
				for (int i=0;i<AnalyseResult.cbThreeCount;i++)
				{
					MoveUpCard[MoveUpNum++]=AnalyseResult.cbThreeCardData[i];
				}
				break;
			}

		}else
		{
			DDZtagOutCardResult outCardData;
			if(true==m_GameLogic.searchOutCard(Card,MoveNum,m_outCardData,m_outCardNum,outCardData))
			{
				MoveUpNum =outCardData.cbCardCount;
				memcpy(MoveUpCard,outCardData.cbResultCard,sizeof(BYTE)*MoveUpNum);
				break;
			}
		}
	} while (0);

	if (MoveUpNum>1)
			setCardUp(MoveUpCard,MoveUpNum);

}

void DDZViewHandCard::onTouchCancelled(Touch *pTouch,Event *pEvent )
{

}

/*
void DDZViewHandCard::setHardCard(DDZCCsendCard* pCardData)
{
	BYTE card = ;

	switch(_charID)
	{
	case 0:
		if (_open)
		{
			showLeftUserHardCard(_data,_num);
		}
		showLeftHintCard(_num);
		break;
	case 1:
		showMeUserHardCard(_data,_num,_open);
		if(_effect)
		{
			//Object* pObj=NULL;
			//Array_FOREACH(m_handCardNode->getChildren(),pObj)
			for(int i = 0; i < m_handCardNode->getChildrenCount(); i ++)
			{
				//Sprite* pNode=(Sprite*)pObj;
				Sprite* pNode=(Sprite*)m_handCardNode->getChildren().at(i);
				if (pNode)pNode->setVisible(false);
			}
			m_isOpenCard = _open;
			m_effectNum=0;
			unschedule(schedule_selector(DDZViewHandCard::showhardCardEffect));
			schedule(schedule_selector(DDZViewHandCard::showhardCardEffect),0.2f,17,0.0f);
		}
		break;
	case 2:
		if (_open)
		{
			showRightUserHardCard(_data,_num);
		}
		showRightHintCard(_num);
		break;
	}
}
*/

void DDZViewHandCard::setHardCard(WORD _charID,BYTE _data[],int _num,bool _effect,bool _open)
{
	switch(_charID)
	{
	case 0:
		if (_open)
		{
			showLeftUserHardCard(_data,_num);
			return;
		}
		showLeftHintCard(_num);
		break;
	case 1:
		showMeUserHardCard(_data,_num,_open);
		if(_effect)
		{
			//Object* pObj=NULL;
			//Array_FOREACH(m_handCardNode->getChildren(),pObj)
			for(int i = 0; i < m_handCardNode->getChildrenCount(); i ++)
			{
				//Sprite* pNode=(Sprite*)pObj;
				Sprite* pNode=(Sprite*)m_handCardNode->getChildren().at(i);
				if (pNode)pNode->setVisible(false);
			}
			m_isOpenCard = _open;
			m_effectNum=0;
			unschedule(schedule_selector(DDZViewHandCard::showhardCardEffect));
			schedule(schedule_selector(DDZViewHandCard::showhardCardEffect),0.2f,17,0.0f);
		}
		break;
	case 2:
		if (_open)
		{
			showRightUserHardCard(_data,_num);
			return;
		}
		showRightHintCard(_num);
		break;
	}
}

void DDZViewHandCard::setOutCard( WORD _charID, BYTE* _data, int _num, WORD BankUserID)
{
	cocos2d::Size size= Director::getInstance()->getWinSize();

	//打出去的牌缩小到0.8
	float scale = 0.59f;
	float tempSpace = CARD_SPACE * scale;
	int _baseL = (_num - 1) * tempSpace; 
	m_outCardNum = _num;
	memcpy(m_outCardData, _data, sizeof(BYTE) * _num);

	Node* node = NULL;
	Vec2 vPosi;
	switch(_charID)
	{
	case 0:
		node = m_outCardNodeM;
		vPosi = Vec2(size.width*0.3f, size.height*0.7f);
		break;
	case 1:
		node = m_outCardNodeM;
		vPosi = Vec2(size.width*0.5f, size.height*0.5f);
		break;
	case 2:
		node = m_outCardNodeM;
		vPosi = Vec2(size.width*0.7f, size.height*0.7f);
		break;
	default:
		m_outCardNodeL->removeAllChildren();
		m_outCardNodeR->removeAllChildren();
		m_outCardNodeM->removeAllChildren();
		m_outCardNum = 0;
		memset(m_outCardData, 0, sizeof(BYTE) * 20);
		return;
	}
	node->removeAllChildren();
	
	for (int i=0;i<_num;i++)
	{
		Sprite *spt =getCardSprite(_data[i],1,true);
		CC_BREAK_IF(!spt);
		CCcardDate* cardObj =CCcardDate::create(_data[i]);
		cardObj->m_isup = false;
		spt->setUserObject(cardObj);
		spt->setPosition(ccp(-_baseL * 0.5 + i * tempSpace, 0));
		spt->setScale(scale);
		node->addChild(spt);
		node->setPosition(vPosi);
		if(i==_num-1 &&_charID==BankUserID)
		{
			//Sprite* bankWar=Sprite::create(DDZtexture_name::s_card,CCRectMake(335,0,36,40));
			Sprite* bankWar=Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddz_poker_lord_icon);
			bankWar->setAnchorPoint(ccp(1.0f,1.0f));
			bankWar->setPosition(ccp(spt->getContentSize().width,spt->getContentSize().height));
			spt->addChild(bankWar);
		}
	}
}

Sprite* DDZViewHandCard::getCardSprite( char _data,int _type, bool isBigCard)
{
	Sprite* sptbk=NULL;			//白底
	Sprite* sptColUP=NULL;			//花色
	Sprite* sptNumUP=NULL;		//数字
	Sprite* sptColDo=NULL;			//花色
	Sprite* sptNumDo=NULL;		//数字

	SpriteFrame* sptFrame=NULL;
	if (_data == 0x4E)  //小王
	{
		
		if (isBigCard)
		{
			sptbk=Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddz_poker_black);
		}
		else
		{
			sptbk=Sprite::create(DDZtexture_name::s_card,isBigCard?CCRectMake(190,0,95,127):CCRectMake(421,0,50,56));
		}
		
	}
	else if (_data == 0x4F)
	{
		if (isBigCard)
		{
			sptbk=Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddz_poker_red);
		}
		else
		{
			sptbk=Sprite::create(DDZtexture_name::s_card,isBigCard?CCRectMake(95,0,95,127):CCRectMake(371,0,50,56));
		}
	}
	else
	{
		DDZDDZLogic _logic;
		int color =_logic.getCardColor(_data);
		int value =_logic.getCardValue(_data);
		if (isBigCard)
		{
			sptbk=Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddz_poker_back);
			//sptbk=Sprite::create(DDZtexture_name::s_card,CCRectMake(0,0,95,127));				//底版  old
			sptNumUP=Sprite::create(DDZtexture_name::s_ddz_pokers_num,(color/16)%2==1?CCRectMake(37*(value-1),0,37,40):CCRectMake(37*(value-1),40,37,40));
			sptColUP=Sprite::create(DDZtexture_name::s_ddz_pokers_color,CCRectMake(29*(color/16),0,29,29));
		}
		else
		{
			sptbk=Sprite::create(DDZtexture_name::s_card,CCRectMake(285,0,50,56));
			sptNumUP=Sprite::create(DDZtexture_name::s_card,(color/16)%2==1?CCRectMake(30*(value-1),140,30,29):CCRectMake(30*(value-1),169,30,30));
			sptColUP=Sprite::create(DDZtexture_name::s_card,CCRectMake(285+20*(color/16),57,20,20));
		}
		
		sptNumUP->setAnchorPoint(ccp(0.0f,1.0f));
		sptNumUP->setPosition(ccp(sptbk->getContentSize().width*0.07,sptbk->getContentSize().height*0.95));
		
		sptColUP->setAnchorPoint(ccp(0.0f,1.0f));
		sptColUP->setPosition(ccp(sptbk->getContentSize().width*0.1,sptbk->getContentSize().height*0.7));

		if (isBigCard)
		{
			//sptNumDo=Sprite::create(DDZtexture_name::s_card,(color/16)%2==1?CCRectMake(30*(value-1),140,30,29):CCRectMake(30*(value-1),169,30,30));
			sptNumDo=Sprite::create(DDZtexture_name::s_ddz_pokers_num,(color/16)%2==1?CCRectMake(37*(value-1),0,37,40):CCRectMake(37*(value-1),40,37,40));
			sptNumDo->setRotationX(180.0f);
			sptNumDo->setRotationY(180.0f);
			sptNumDo->setAnchorPoint(ccp(0.0f,1.0f));
			sptNumDo->setPosition(ccp(sptbk->getContentSize().width*0.93,sptbk->getContentSize().height*0.05));

			//sptColDo=Sprite::create(DDZtexture_name::s_card,CCRectMake(285+20*(color/16),57,20,20));
			sptColDo=Sprite::create(DDZtexture_name::s_ddz_pokers_color,CCRectMake(29*(color/16),0,29,29));
			sptColDo->setRotationX(180.0f);
			sptColDo->setRotationY(180.0f);
			sptColDo->setAnchorPoint(ccp(0.0f,1.0f));
			sptColDo->setPosition(ccp(sptbk->getContentSize().width*0.9,sptbk->getContentSize().height*0.3));
		}
		else
		{
			sptNumUP->setScale(0.8f);
			sptColUP->setScale(0.8f);
			sptColUP->setPosition(ccp(sptbk->getContentSize().width*0.12,sptbk->getContentSize().height*0.50));
		}
		if(sptColUP)sptbk->addChild(sptColUP);
		if(sptNumUP)sptbk->addChild(sptNumUP);
		if(sptNumDo)sptbk->addChild(sptNumDo);
		if(sptColDo)sptbk->addChild(sptColDo);
	}
	if (_type == -1)
	{
		sptbk->setScale(0.59f);
	}
	return sptbk;
}

bool DDZViewHandCard::UserOutCard(bool isOut)
{
	//--不要
	if(isOut==false)
	{
		this->setCardRecover();//--复原扑克
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,Integer::create(DDZmsgToLogic_passCard));
		return true;
	}
	//--出牌
	BYTE			tempCardData[MAX_COUNT];
	WORD		tempCardNum=0;
	Object* pObj=NULL;
	//正向
	//Array_FOREACH(m_handCardNode->getChildren(),pObj)
	for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
	{
		//Sprite * pNode = (Sprite*)pObj;
		Sprite* pNode=(Sprite*)m_handCardNode->getChildren().at(i);
		CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
		if ( cardObj && cardObj->m_isup)
		{
			tempCardData[tempCardNum++] = cardObj->getValue();
		}
	}
	DDZDDZLogic _logic;
	if (m_outCardNum==0)	//--一轮首出
	{
		if ( _logic.getCardType(tempCardData,tempCardNum) == CT_ERROR)	
			return false;
	}
	else
	{
		if (_logic.compareCard(m_outCardData,tempCardData,m_outCardNum,tempCardNum)==false)
			return false;
	}

	//出牌成功
	DDZCCsendCard* _sendCard=DDZCCsendCard::create(DDZmsgToLogic_outCard);
	_sendCard->m_handCardNum =tempCardNum;
	memcpy(_sendCard->m_handCardData,tempCardData,sizeof(BYTE)*tempCardNum);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_sendCard);
	return true;
}

void DDZViewHandCard::UserAutoCard()
{
	//--出牌
	BYTE			tempCardData[MAX_COUNT];
	WORD		tempCardNum=0;
	Object* pObj=NULL;
	DDZDDZLogic _logic;
	//逆向
	//Array_FOREACH_REVERSE(m_handCardNode->getChildren(),pObj)
	for(int i = m_handCardNode->getChildrenCount() - 1;i >= 0;i --)
	{
		//Sprite * pNode = (Sprite*)pObj;
		Sprite* pNode=(Sprite*)m_handCardNode->getChildren().at(i);
		CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
		if ( tempCardNum ==0)
		{
			tempCardData[tempCardNum++]  = cardObj->getValue();
		}
		else if (_logic.getCardLogicValue(tempCardData[0]) ==_logic.getCardLogicValue(cardObj->getValue()))
		{
			tempCardData[tempCardNum++]  = cardObj->getValue();
		}
	}
	//出牌成功
	DDZCCsendCard* _sendCard=DDZCCsendCard::create(DDZmsgToLogic_outCard);
	_sendCard->m_handCardNum =tempCardNum;
	memcpy(_sendCard->m_handCardData,tempCardData,sizeof(BYTE)*tempCardNum);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_sendCard);
}

bool DDZViewHandCard::setHintCard()
{
	bool ret= false;
	DDZDDZLogic _logic;
	cocos2d::Size size =Director::getInstance()->getWinSize();
	DDZtagOutCardResult outCardData;
	Object* pObj=NULL;
	//--手牌
	BYTE			tempCardData[MAX_COUNT];
	WORD		tempCardNum=getCardData(tempCardData);
	//--已经起来的牌
	BYTE			tempCardDataUp[MAX_COUNT];
	WORD		tempCardNumUp=getCardUpData(tempCardDataUp);
	//--对手出的牌
	BYTE        tempOutCardData[MAX_COUNT];
	WORD		tempOutCardNum=m_outCardNum;
	memcpy(tempOutCardData,m_outCardData,sizeof(BYTE)*MAX_COUNT);

	if (_logic.compareCard(m_outCardData,tempCardDataUp,m_outCardNum,tempCardNumUp))
	{
		//--已经智能了
		tempOutCardNum=tempCardNumUp;
		memcpy(tempOutCardData,tempCardDataUp,sizeof(BYTE)*MAX_COUNT);
		ret=true;
	}

	if(_logic.searchOutCard(tempCardData,tempCardNum,tempOutCardData,tempOutCardNum,outCardData))
	{
		ret=true;

		//Array_FOREACH(m_handCardNode->getChildren(),pObj)
		for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
		{
			//Sprite * pNode = (Sprite*)pObj;
			Sprite* pNode=(Sprite*)m_handCardNode->getChildren().at(i);
			CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
			if (cardObj==NULL)continue;
			for (int i =0;i<outCardData.cbCardCount;i++)
			{
				if ( cardObj && cardObj->getValue()==outCardData.cbResultCard[i])
				{
					cardObj->m_isup =true;

					float _h = (cardObj->m_isup)?0.15f:0.12f;
					pNode->stopAllActions();
					pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
					break;
				}
				else
				{
					cardObj->m_isup =false;
					float _h = (cardObj->m_isup)?0.15f:0.12f;
					pNode->stopAllActions();
					pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
				}

			}
		}
	}
	else if (_logic.searchOutCard(tempCardData,tempCardNum,m_outCardData,m_outCardNum,outCardData))
	{
		ret=true;

		//Array_FOREACH(m_handCardNode->getChildren(),pObj)
		for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
		{
			//Sprite * pNode = (Sprite*)pObj;
			Sprite* pNode=(Sprite*)m_handCardNode->getChildren().at(i);
			CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
			if (cardObj==NULL)continue;
			for (int i =0;i<outCardData.cbCardCount;i++)
			{
				if ( cardObj && cardObj->getValue()==outCardData.cbResultCard[i])
				{
					cardObj->m_isup =true;

					float _h = (cardObj->m_isup)?0.15f:0.12f;
					pNode->stopAllActions();
					pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
					break;
				}
				else
				{
					cardObj->m_isup =false;
					float _h = (cardObj->m_isup)?0.15f:0.12f;
					pNode->stopAllActions();
					pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
				}

			}
		}
	}

	return ret;
}

bool DDZViewHandCard::isNoCardout()
{
	DDZDDZLogic _logic;
	DDZtagOutCardResult outCardData;
	//--手牌
	BYTE			tempCardData[MAX_COUNT];
	WORD		tempCardNum=getCardData(tempCardData);
	return _logic.searchOutCard(tempCardData,tempCardNum,m_outCardData,m_outCardNum,outCardData);

}

void DDZViewHandCard::showLoadIconOnPokers(bool isLord)
{
	if (!isLord)
	{
		return;
	}
	else
	{
		this->m_isLord = isLord;
	}
	int num = m_handCardNode->getChildrenCount();
	if (num <=0)
	{
		return;
	}
	for (int i = 0; i < m_handCardNode->getChildrenCount(); i++)
	{
		
		if ( i == num-1)
		{
			/*Sprite * sptOpen=Sprite::create(DDZtexture_name::s_card,CCRectMake(325,77,55,63));
			sptOpen->setAnchorPoint(ccp(1.0f,1.0f));
			sptOpen->setPosition(ccp(spt->getContentSize().width,spt->getContentSize().height));
			spt->addChild(sptOpen);*/
			
			//spt->setVisible(true);

			/*auto spt = m_handCardNode->getChildren().at(i)->getChildren().at(0);
			spt->setVisible(true);*/
			m_handCardNode->getChildren().at(i)->getChildByName("Lord_icon_xxx")->setVisible(true);
		}
	}

}

void DDZViewHandCard::showMeUserHardCard( BYTE* _data,int _num,bool _open )
{
	cocos2d::Size size= Director::getInstance()->getWinSize();
	int _baseL = (_num - 1)*CARD_SPACE;

	m_handCardNode->removeAllChildren();
	for (int i=0;i<_num;i++)
	{
		Sprite *spt =getCardSprite(_data[i],1,true);
		float widthTem = spt->getContentSize().width - CARD_SPACE;
		CC_BREAK_IF(!spt);
		CCcardDate* cardObj =CCcardDate::create(_data[i]);
		cardObj->m_isup = false;
		cardObj->m_isfocus = false;
		cardObj->m_isfist = false;
		cardObj->m_isselect = false;
		spt->setUserObject(cardObj);
		spt->setAnchorPoint(ccp(0.5f,0));
		spt->setPosition(ccp(size.width*0.5 - _baseL*0.5 + i * CARD_SPACE , size.height*0.12f));
		m_handCardNode->addChild(spt);
		if (i==_num-1 && _open)
		{
			Sprite * sptOpen=Sprite::create(DDZtexture_name::s_card,CCRectMake(325,77,55,63));
			sptOpen->setAnchorPoint(ccp(1.0f,1.0f));
			sptOpen->setPosition(ccp(spt->getContentSize().width,spt->getContentSize().height));
			spt->addChild(sptOpen);
		}
		if (!_open)
		{
			//Sprite * sptOpen=Sprite::create(DDZtexture_name::s_card,CCRectMake(325,77,55,63));
			Sprite* sptOpen=Sprite::createWithSpriteFrameName(DDZtexture_name::s_ddz_poker_lord_icon);
			sptOpen->setAnchorPoint(ccp(1.0f,1.0f));
			sptOpen->setPosition(ccp(spt->getContentSize().width,spt->getContentSize().height));
			sptOpen->setVisible(false);
			sptOpen->setName("Lord_icon_xxx");
			spt->addChild(sptOpen);
			//spt->getChildByName("Lord_icon_xxx")->setVisible(true);
			if (m_isLord && i==_num-1)
			{
				sptOpen->setVisible(true);
			}
		}
		
	}
}

void DDZViewHandCard::showhardCardEffect( float deley )
{
	DDZSoundControl::sharedDDZSoundControl()->playSoundEffect(DDZsound_name::s_ct_fapai);
	m_effectNum++;
	Object* pObj=NULL;
	m_handCardNode->setVisible(true);
	int _num=0;
	Vec2 ptEnd;
	//正向
	//Array_FOREACH(m_handCardNode->getChildren(),pObj)
	for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
	{
		_num++;
		//Sprite* pNode=(Sprite*)pObj;
		Sprite* pNode=(Sprite*)m_handCardNode->getChildren().at(i);
		if (pNode )
			pNode->setVisible(_num<m_effectNum?true:false);
		if (_num==m_effectNum)
		{
			ptEnd=pNode->getPosition();
			pNode->setPosition(ccp(ptEnd.x-CARD_SPACE,ptEnd.y));
			pNode->setVisible(true);
			pNode->runAction(MoveTo::create(0.15f,ptEnd));
		}
	}
	if (((m_effectNum%4==0  )|| m_effectNum==1 ) && m_isOpenCard==false)
	{
		DDZCCviewOpenCardNum* _num=DDZCCviewOpenCardNum::create(DDZmsgToView_openCardNum);
		_num->m_num =5- m_effectNum/4;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_num);
	}
	if(m_effectNum>=17)
	{
		unschedule(schedule_selector(DDZViewHandCard::showhardCardEffect));
	}
}

void DDZViewHandCard::showLeftUserHardCard(BYTE* _data,int _num)
{
	/*
	//大牌展示
	cocos2d::Size size = Director::getInstance()->getWinSize();
	float scale = 0.7f;
	float tempSpace = CARD_SPACE * scale;
	int _baseL = (_num - 1) * tempSpace; 
	//int _baseL = (_num-1)*CARD_SPACE;
	m_leftCardNode->removeAllChildren();
	for (int i=0;i<_num;i++)
	{
		Sprite *spt = getCardSprite(_data[i],1,true);
		spt->setScale(scale);
		CC_BREAK_IF(!spt);
		spt->setPosition(ccp(-_baseL * 0.5 + i * tempSpace, 0));
		m_leftCardNode->addChild(spt);
	}
	*/

	
	//小牌展示
	cocos2d::Size size = Director::getInstance()->getWinSize();
	int _baseL = (_num-1)*SMALLCARD_SPACE;
	m_leftCardNode->removeAllChildren();
	for (int i=0;i<_num;i++)
	{
		Sprite *spt = getCardSprite(_data[i],1,false);
		CC_BREAK_IF(!spt);
		spt->setPosition(ccp(-_baseL * 0.5 + i * SMALLCARD_SPACE, 0));
		m_leftCardNode->addChild(spt);
	}
	
}

void DDZViewHandCard::showRightUserHardCard(BYTE* _data,int _num)
{
	/*
	//大牌展示
	cocos2d::Size size = Director::getInstance()->getWinSize();
	float scale = 0.7f;
	float tempSpace = CARD_SPACE * scale;
	int _baseL = (_num - 1) * tempSpace; 
	//int _baseL = (_num-1)*SMALLCARD_SPACE;
	m_rightCardNode->removeAllChildren();
	for (int i=0;i<_num;i++)
	{
		Sprite *spt = getCardSprite(_data[i],1,true);
		spt->setScale(scale);
		CC_BREAK_IF(!spt);
		spt->setPosition(ccp(-_baseL * 0.5 + i * tempSpace, 0));
		m_rightCardNode->addChild(spt);
	}
	*/

	
	//小牌展示
	cocos2d::Size size = Director::getInstance()->getWinSize();
	int _baseL = (_num-1)*SMALLCARD_SPACE;
	m_rightCardNode->removeAllChildren();
	for (int i=0;i<_num;i++)
	{
		Sprite *spt = getCardSprite(_data[i],1,false);
		CC_BREAK_IF(!spt);
		spt->setPosition(ccp(-_baseL * 0.5 + i * SMALLCARD_SPACE, 0));
		m_rightCardNode->addChild(spt);
	}
	
}

void DDZViewHandCard::showDiPai(BYTE * _data,int _num,int _lastTask,bool isNew)
{
	m_diPaiNode->removeAllChildren();
	cocos2d::Size size=Director::getInstance()->getWinSize();
	memcpy(m_BackData,_data,sizeof(BYTE)*3);
	m_lastTaskKey=_lastTask;
	if (isNew==false)
	{
		showDipaiEffect(this);
		return;
	}

	Sprite *spt[3];
	//Array * animFrames[3];
	Vector<SpriteFrame *> animFrames;
	CCAnimation * animation[3];
	Vec2 pt=ccp(size.width*0.5,size.height*0.5 + 180);

	for (int i=0;i<3;i++)
	{
		spt[i] = Sprite::createWithSpriteFrameName(DDZtexture_name::s_paiBei0);
		spt[i]->setPosition(pt);
		spt[i]->setUserObject(Integer::create(_data[i]));
		m_diPaiNode->addChild(spt[i]);

		//animFrames[i]=new Array(3);
		SpriteFrame * frame1 = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_paiBei0);
		//animFrames[i]->addObject(frame1);
		animFrames.pushBack(frame1);

		SpriteFrame * frame2 = SpriteFrameCache::getInstance()->spriteFrameByName(DDZtexture_name::s_paiBei1);
		//animFrames[i]->addObject(frame2);
		animFrames.pushBack(frame2);

		//auto frame3 = getCardSprite(_data[i],-1,true);
		//frame3->setScale(0.59f);
		//animFrames[i]->addObject(frame3);
		//animFrames.pushBack(frame3->displayFrame());

		animation[i] = Animation::createWithSpriteFrames(animFrames,0.2f);
		//spt[i]->runAction(Sequence::create(MoveTo::create(0.2f,ccp(pt.x+(i-1)*100,pt.y)),CCAnimate::create(animation[i]),CallFuncN::create(this,callfuncN_selector(DDZViewHandCard::showDipaiSpt)),DelayTime::create(1.0f),
			//CCSpawn::create(CCScaleTo::create(0.4f,0.4f),MoveTo::create(0.4f,ccp(30,size.height-30)),NULL),i==2?CallFuncN::create(this,callfuncN_selector(DDZViewHandCard::showDipaiEffect)):NULL,NULL));
		
		//animation[i]->setRestoreOriginalFrame(true);
		
		spt[i]->runAction(Sequence::create(
			MoveTo::create(0.2f,ccp(pt.x+(i-1)*100,pt.y)),
			CCAnimate::create(animation[i]),
			DelayTime::create(0.18f),
			CallFuncN::create(spt[i],callfuncN_selector(DDZViewHandCard::showDipaiSpt)),
			DelayTime::create(1.0f),
			CCSpawn::create(MoveTo::create(0.4f,ccp(spt[i]->getContentSize().width / 2 + 10 + i * CARD_SPACE * 0.59f,size.height - spt[i]->getContentSize().height / 2 - 10)),NULL)
			//,i==2?CallFuncN::create(this,callfuncN_selector(DDZViewHandCard::showDipaiEffect)):NULL
			,NULL));
		animFrames.clear();
	}
}

void DDZViewHandCard::showDipaiSpt(Node* _node)
{
	Sprite *spt=(Sprite*)_node;
	spt->setTexture(DDZtexture_name::s_paiBei0);
	Sprite * _sptcard=getCardSprite(((Integer*)(spt->getUserObject()))->getValue(),1,true);
	_sptcard->setScale(0.59f);
	_sptcard->setAnchorPoint(ccp(0.5f,0.5f));
	_sptcard->setPosition(ccp(spt->getContentSize().width*0.5,spt->getContentSize().height*0.5));
	spt->addChild(_sptcard);
}

void DDZViewHandCard::showDipaiEffect(Node* _node)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_diPaiNode->removeAllChildren();
	for (int i=0;i<3;i++)
	{
		Sprite *spt = getCardSprite(m_BackData[i],1,true);
		CC_BREAK_IF(!spt);
		spt->setAnchorPoint(ccp(0,1.0f));
		spt->setPosition(ccp(10.0f+i*CARD_SPACE,size.height-10.0f));
		m_diPaiNode->addChild(spt);
	}
	m_viewTask->setBackTaskView(m_BackData,3);
	m_viewTask->setLastTaskView(m_lastTaskKey);
}

void DDZViewHandCard::showRightHintCard(int _num)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_pDDZRightCardNum->setString(Value(_num).asString());
	m_pDDZRightCardNum->setVisible(true);
	if (_num <= 0)
	{
		m_pDDZRightCardNum->setVisible(false);
	}
	int _baseL = (_num-1)*5;
	m_hintRightCardNode->removeAllChildren();
	for (int i=0;i<_num;i++)
	{
		if(i>=20)
			break;
		Sprite * spt = Sprite::createWithSpriteFrame(spriteFrame(DDZtexture_name::s_hintCard));
		CC_BREAK_IF(!spt);
		spt->setPosition(ccp(-i*4.5 + m_rightCardNum[i], i*5));
		m_hintRightCardNode->addChild(spt, _num - i, 0);
	}
}

void DDZViewHandCard::showLeftHintCard(int _num)
{
	cocos2d::Size size = Director::getInstance()->getWinSize();

	m_pDDZLeftCardNum->setString(Value(_num).asString());
	m_pDDZLeftCardNum->setVisible(true);
	if (_num <= 0)
	{
		m_pDDZLeftCardNum->setVisible(false);
	}

	int _baseL = (_num-1)*5;
	m_hintLeftCardNode->removeAllChildren();
	for (int i=0;i<_num;i++)
	{
		Sprite * spt = Sprite::createWithSpriteFrame(spriteFrame(DDZtexture_name::s_hintCard));
		spt->setFlipX(true);
		CC_BREAK_IF(!spt);
		spt->setPosition(ccp(i*4.5 + m_leftCardNum[i], i*5));
		m_hintLeftCardNode->addChild(spt, _num - i, 0);
	}
}

int DDZViewHandCard::getCardData( BYTE* _data )
{

	WORD		tempCardNum=0;
	Object* pObj=NULL;
	//正向
	//Array_FOREACH(m_handCardNode->getChildren(),pObj)
	for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
	{
		//Sprite * pNode = (Sprite*)pObj;
		Sprite * pNode = (Sprite*)m_handCardNode->getChildren().at(i);
		CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
		if ( cardObj )
		{
			_data[tempCardNum++] = cardObj->getValue();
		}
	}
	return tempCardNum;
}

int DDZViewHandCard::getCardUpData( BYTE* _data )
{
	WORD		tempCardNum=0;
	Object* pObj=NULL;
	//正向
	//Array_FOREACH(m_handCardNode->getChildren(),pObj)
	for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
	{
		//Sprite * pNode = (Sprite*)pObj;
		Sprite * pNode = (Sprite*)m_handCardNode->getChildren().at(i);
		CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
		if ( cardObj && cardObj->m_isup)
		{
			_data[tempCardNum++] = cardObj->getValue();
		}
	}
	return tempCardNum;
}

void DDZViewHandCard::setUnallowTouch()
{
	m_unallowToucn = true;
}

void DDZViewHandCard::removeHintCard()
{
	m_hintLeftCardNode->setVisible(false);
	m_hintLeftCardNode->removeAllChildren();

	m_hintRightCardNode->setVisible(false);
	m_hintRightCardNode->removeAllChildren();
}

void DDZViewHandCard::setCardRecover()
{
	Object* pObj=NULL;
	//正向
	cocos2d::Size size=Director::getInstance()->getWinSize();
	//Array_FOREACH(m_handCardNode->getChildren(),pObj)
	for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
	{
		//Sprite * pNode = (Sprite*)pObj;
		Sprite * pNode = (Sprite*)m_handCardNode->getChildren().at(i);
		CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
		if ( cardObj && cardObj->m_isup)
		{
			cardObj->m_isup=!cardObj->m_isup;
			float _h = (cardObj->m_isup)?0.15f:0.12f;
			pNode->stopAllActions();
			pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
		}
	}
}

void DDZViewHandCard::setCardUp( BYTE * _data,int _num )
{
	cocos2d::Size size=Director::getInstance()->getWinSize();
	Object* pObj=NULL;
	//Array_FOREACH(m_handCardNode->getChildren(),pObj)
	for(int i = 0;i < m_handCardNode->getChildrenCount();i ++)
	{
		//Sprite * pNode = (Sprite*)pObj;
		Sprite * pNode = (Sprite*)m_handCardNode->getChildren().at(i);
		CCcardDate * cardObj = (CCcardDate*)pNode->getUserObject();
		if (cardObj==NULL)continue;
		for (int i =0;i<_num;i++)
		{
			if ( cardObj && cardObj->getValue()==_data[i])
			{
				cardObj->m_isup =true;

				float _h = (cardObj->m_isup)?0.15f:0.12f;
				pNode->stopAllActions();
				pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
				break;
			}
			else
			{
				cardObj->m_isup =false;
				float _h = (cardObj->m_isup)?0.15f:0.12f;
				pNode->stopAllActions();
				pNode->runAction(MoveTo::create(0.1f,ccp(pNode->getPositionX(),_h*size.height)));
			}
		}
	}
}


bool DDZViewHandCard::OnShunziWidom( BYTE cbCardData[], BYTE cbCardCount ,BYTE wisCard[], int& index )
{
	DDZDDZLogic m_gameLogic;
	index=0;
	int twoIndex=0;
	bool  isWisdom=true;
	BYTE cbCompareValueMax = m_gameLogic.getCardLogicValue(cbCardData[0]);//最大牌
	BYTE cbCompareValueMin = m_gameLogic.getCardLogicValue(cbCardData[cbCardCount-1]);//最小牌
	//判断是否是顺子
	memset(wisCard,0,20);
	isWisdom=true;
	if((cbCompareValueMax-cbCompareValueMin)<4 || (cbCompareValueMax-cbCompareValueMin)>12)return false;
	if(cbCompareValueMax>14)return false;
	for(int k=cbCompareValueMin;k<=cbCompareValueMax;k++)
	{
		twoIndex=0;
		for(int i=0;i<cbCardCount;i++)
		{
			if(m_gameLogic.getCardLogicValue(cbCardData[i])==k)
			{
				twoIndex++;
				wisCard[index++]=cbCardData[i];
				break;
			}
		}
		if(twoIndex<1)				
		{
			isWisdom=false;
			break;
		}
	}
	if (isWisdom)
	{
		setCardUp(wisCard,index);
		return true;
	}
	return false;
}

bool DDZViewHandCard::OnLianduiWidom( BYTE cbCardData[], BYTE cbCardCount,BYTE wisCard[], int& index )
{
	DDZDDZLogic m_gameLogic;
	index=0;
	int twoIndex=0;
	bool  isWisdom=true;
	BYTE cbCompareValueMax = m_gameLogic.getCardLogicValue(cbCardData[0]);//最大牌
	BYTE cbCompareValueMin = m_gameLogic.getCardLogicValue(cbCardData[cbCardCount-1]);//最小牌


	//判断选派是否为连对
	memset(wisCard,0,20);
	if(cbCompareValueMax>14)return false;
	if((cbCompareValueMax-cbCompareValueMin)<2)return false;
	for(int k=cbCompareValueMin;k<=cbCompareValueMax;k++){
		twoIndex=0;
		for (int i=0;i<cbCardCount;i++)
		{
			if(m_gameLogic.getCardLogicValue(cbCardData[i])==k){
				twoIndex++;
				wisCard[index++]=cbCardData[i];
			}
			if(twoIndex>=2)break;
		}
		if (twoIndex<2)
		{
			isWisdom=false;
			break;
		}
	}
	if (isWisdom)
	{
		setCardUp(wisCard,index);
		return true;
	}
	return false;
}

void DDZViewHandCard::newRound()
{
	m_outCardNum = 0;
}

void DDZViewHandCard::dealGameReady()
{
	newRound();
	m_unallowToucn = false;

	//	自家相关处理
	m_handCardNode->removeAllChildren();
	m_outCardNodeM->removeAllChildren();

	//	上家相关处理
	m_hintLeftCardNode->setVisible(true);
	m_hintLeftCardNode->removeAllChildren();

	m_leftCardNode->setVisible(false);
	m_leftCardNode->removeAllChildren();

	//	下家相关处理
	m_rightCardNode->setVisible(false);
	m_rightCardNode->removeAllChildren();

	m_hintRightCardNode->setVisible(true);
	m_hintRightCardNode->removeAllChildren();
}

void DDZViewHandCard::dealGameStart()
{
	m_pDDZLeftCardNum->setVisible(true);
	m_pDDZRightCardNum->setVisible(true);
}

void DDZViewHandCard::dealGameOver()
{
// 	m_outCardNodeM->removeAllChildren();
	m_outCardNodeL->removeAllChildren();
	m_outCardNodeR->removeAllChildren();
	m_diPaiNode->removeAllChildren();
	m_pDDZLeftCardNum->setVisible(false);
	m_pDDZRightCardNum->setVisible(false);
	m_leftCardNode->setVisible(true);
	m_rightCardNode->setVisible(true);
	setUnallowTouch();//游戏结束，设置扑克牌不能触摸
	removeHintCard();
	this->m_isLord = false;
}

void DDZViewHandCard::clearOutCard(DDZViewChair eChair)
{
	Node* pTmp = nullptr;
	switch (eChair)
	{
	case DDZViewChair_Last:
		pTmp = m_outCardNodeL;
		break;
	case DDZViewChair_Self:
		pTmp = m_outCardNodeM;
		break;
	case DDZViewChair_Next:
		pTmp = m_outCardNodeR;
		break;
	default:
		return;
	}
	pTmp->removeAllChildren();
}
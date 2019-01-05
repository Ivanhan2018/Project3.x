//  [3/25/2014 wucan]

#ifndef			__VIEW_HANDCARD__
#define			__VIEW_HANDCARD__
#include "cocos2d.h"
#include "ViewTask.h"
#include <cmath>
USING_NS_CC;
#define  CARD_SPACE  40
#define  SMALLCARD_SPACE 20

class ViewHandCard :public Layer
{
public:
	ViewHandCard();
	~ViewHandCard();
	CREATE_FUNC(ViewHandCard);
	virtual bool init();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);

public:
	void setHardCard(WORD _charID,BYTE* _data,int _num,bool _effect,bool _open);	
	void setOutCard(WORD _charID,BYTE* _data,int _num,WORD BankUserID=1000);
	bool UserOutCard(bool isOut);				//--用户出牌
	void UserAutoCard();																//--自动出牌
	void showDiPai(BYTE * _data,int _num,int _lastTask,bool isNew=true);										//--显示底牌
	void showDipaiEffect(Node* _node);
	void showDipaiSpt(Node* _node);
	bool setHintCard();																	//--设置提示牌
	bool isNoCardout();
	void setUnallowTouch();	//游戏结束设置扑克牌不能被触摸到
	void removeHintCard();	//移除盖章的扑克牌
	void setCardRecover();													//--卡牌复原
	void setCardUp(BYTE * _data,int _num);						//--卡牌弹起
	void newRound();
private:
	void showMeUserHardCard(BYTE* _data,int _num,bool _open);		//显示自己手中的扑克牌
	void showhardCardEffect(float deley);
	void showLeftUserHardCard(BYTE* _data,int _num);	//显示上家手牌
	void showRightUserHardCard(BYTE* _data,int _num);	//显示下家手牌
	void showLeftHintCard(int _num);					//显示上家扑克牌盖着的样子
	void showRightHintCard(int _num);					//显示下家扑克牌盖着的样子
	int m_leftCardNum[20];								//左边牌x位置
	int m_rightCardNum[20];								//右边牌x位置


public:
	Sprite* getCardSprite(char _data,int _type,bool isBigCard);
	int			getCardData(BYTE* _data);							//--得到手牌
	int			getCardUpData(BYTE* _data);						//--得到up标志的牌
	bool OnShunziWidom( BYTE cbCardData[], BYTE cbCardCount ,BYTE wisCard[], int& index );
	bool OnLianduiWidom( BYTE cbCardData[], BYTE cbCardCount,BYTE wisCard[], int& index  );
private:
	//用作选牌手势判断
	//通过距离的变化判断手势，如果m_distance>m_maxDistance,表示发散的手势，反之则为收缩手势
	float m_distance; //前一个点x到原点的距离
	float m_maxDistance; //后一个点x到原点的距离  
	float m_origin;	//原点x位置
	bool m_isRay;			//手势是否为发散
	bool m_isSelect;	//是否选中了扑克牌
// 	LabelAtlas * m_leftCardNumAtlas;//左边玩家扑克牌数量
// 	LabelAtlas * m_rightCardNumAtlas;//右边玩家扑克牌数量

	Node * m_handCardNode;		//自己手牌node					--SpriteBatchNode有写手机不支持太多
	SpriteBatchNode * m_leftCardNode;		//上家手牌node
	SpriteBatchNode * m_rightCardNode;		//下家手牌node
	Node * m_diPaiNode;			//地主牌（底牌）node
	SpriteBatchNode * m_hintLeftCardNode;	//上家盖着的扑克牌node
	SpriteBatchNode * m_hintRightCardNode; //下家盖着的扑克牌node
	int m_dt[20];					//扑克牌偏移量
	Node * m_outCardNodeL;		//上家出的牌
	Node * m_outCardNodeR;		//下家出的牌
	Node * m_outCardNodeM;		//自己出的牌
	ViewTask*				m_viewTask;
	//--最大出牌数据
	BYTE		m_outCardData[20];
	WORD	m_outCardNum;
	//--明牌效果相关
	int        m_effectNum;
	bool     m_isOpenCard;
	//--底牌数据
	BYTE   m_BackData[3];
	int	  m_lastTaskKey;
	bool m_unallowToucn;		//游戏结束，扑克牌不允许触摸
};

#endif
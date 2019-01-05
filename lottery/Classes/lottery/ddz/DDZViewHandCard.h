//  [3/25/2014 wucan]

#ifndef	DDZ___VIEW_HANDCARD__
#define	DDZ___VIEW_HANDCARD__
#include "cocos2d.h"
#include "DDZViewTask.h"
#include <cmath>
USING_NS_CC;
#define  CARD_SPACE  52
#define  SMALLCARD_SPACE 20

//	斗地主视图椅子
enum DDZViewChair
{
	DDZViewChair_Last = 0,	//	上家
	DDZViewChair_Self,		//	自家
	DDZViewChair_Next		//	下家
};

class DDZViewHandCard :public Layer
{
public:
	DDZViewHandCard();
	~DDZViewHandCard();
	CREATE_FUNC(DDZViewHandCard);
	virtual bool init();
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

public:
// 	void setHardCard(DDZCCsendCard* pCardData);	
	void setHardCard(WORD _charID,BYTE _data[],int _num,bool _effect,bool _open);			//设置玩家自己手牌
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

	//	游戏准备相关逻辑处理
	void dealGameReady();

	//	游戏开始相关逻辑处理
	void dealGameStart();

	//	游戏结束相关逻辑处理
	void dealGameOver();

	//	清除打出的牌
	void clearOutCard(DDZViewChair eChair);

	//add by zengxin 20160926  在地主的牌上显示地主角标
	void showLoadIconOnPokers(bool isLord);

private:
	void showMeUserHardCard(BYTE* _data,int _num,bool _open);		//显示自己手中的扑克牌
	void showhardCardEffect(float deley);
	void showLeftUserHardCard(BYTE* _data,int _num);	//显示上家手牌
	void showRightUserHardCard(BYTE* _data,int _num);	//显示下家手牌
	void showLeftHintCard(int _num);					//显示上家扑克牌盖着的样子
	void showRightHintCard(int _num);					//显示下家扑克牌盖着的样子

	

	int m_leftCardNum[20];								//左边牌x位置
	int m_rightCardNum[20];								//右边牌x位置
	bool m_unallowToucn;		//游戏结束，扑克牌不允许触摸
	Node * m_handCardNode;		//自己手牌node					--SpriteBatchNode有写手机不支持太多
	SpriteBatchNode * m_hintLeftCardNode;	//上家盖着的扑克牌node
	SpriteBatchNode * m_hintRightCardNode; //下家盖着的扑克牌node
	Node * m_outCardNodeM;		//自己出的牌
	Node * m_outCardNodeL;		//上家出的牌
	Node * m_outCardNodeR;		//下家出的牌
	Node * m_diPaiNode;			//地主牌（底牌）node
	//	上家手牌数量显示
	Label* m_pDDZLeftCardNum;

	//	下家手牌数量显示
	Label* m_pDDZRightCardNum;

	// add by zengxin 20160926   是否为地主标志
	bool m_isLord;

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

	SpriteBatchNode * m_leftCardNode;		//上家手牌node
	SpriteBatchNode * m_rightCardNode;		//下家手牌node
	int m_dt[20];					//扑克牌偏移量
	DDZViewTask*				m_viewTask;

	//--最大出牌数据
	BYTE		m_outCardData[20];
	WORD	m_outCardNum;
	//--明牌效果相关
	int        m_effectNum;
	bool     m_isOpenCard;
	//--底牌数据
	BYTE   m_BackData[3];
	int	  m_lastTaskKey;
};

#endif
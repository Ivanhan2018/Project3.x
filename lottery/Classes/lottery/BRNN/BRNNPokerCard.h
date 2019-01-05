#ifndef __BRNN_POKER_CARD_H__
#define __BRNN_POKER_CARD_H__

#include "cocos2d.h"
#include "Card.h"
#include "BRNNDefine.h"
#include "Define.h"

USING_NS_CC;

/*
**	@brief	百人牛牛——扑克牌
*/
class BRNNPokerCard: public Card
{
public:
	BRNNPokerCard();
	~BRNNPokerCard();
	CREATE_FUNC(BRNNPokerCard);

	//	初始化卡牌
	void initCard(const int nCardIdx);

private:
	bool init();

	/*
	**	@brief	获取卡牌纹理文件名
	**	@nCardIdx	卡牌索引；0为卡牌背面，1-54为正面
	*/
	string getCardFileName(const int nCardIdx);

};

/*
**	@brief	百人牛牛——手牌
*/
class BRNNHandsCard: public Node
{
public:
	BRNNHandsCard();
	~BRNNHandsCard();
	CREATE_FUNC(BRNNHandsCard);
	
	//	重置
	void resetHandsCard();

	//	初始化单张手牌
	void initHandsCard(int nIdx, const BYTE sCardData);

	//	初始化整副手牌
	void initHandsCard(const BYTE sCardData[]);

	//	展现手牌类型
	void showCardType(E_BRNN_CardType eCardType);

	//	醒目展示手牌
	void showCardMarkedness();

	//	扑克牌
	BRNNPokerCard* m_pPokerCard[BRNN_HANDS_CARD_COUNT];

	//	扑克牌数据
	BYTE m_cbCardData[BRNN_HANDS_CARD_COUNT];

	//	牌型
	CC_SYNTHESIZE(E_BRNN_CardType, m_eCardType, CardType);
private:
	bool init();
	void initUI();

	//	牌型底纹
	Sprite* m_pCardTypeFloor;

	//	牌型
	Sprite* m_pCardType;

};

#endif  // __BRNN_POKER_CARD_H__
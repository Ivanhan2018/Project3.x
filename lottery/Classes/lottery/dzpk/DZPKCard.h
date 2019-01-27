#ifndef __DZPK_CARD_H__
#define __DZPK_CARD_H__

#include "cocos2d.h"
#include "BJLRes.h"
#include "CMD_DZPK.h"

USING_NS_CC;

#define DZPK_CARD_OBVERSE_ANGLE_Z 270	//正面卡牌的起始Z轴角度
#define DZPK_CARD_OBVERSE_DELTA_Z 90	//正面卡牌旋转的Z轴角度差
#define DZPK_CARD_REVERSE_ANGLE_Z 0		//背面卡牌的起始Z轴角度
#define DZPK_CARD_REVERSE_DELTA_Z 90	//背面卡牌旋转的Z轴角度差

#define DZPK_CARD_ANI_TURN_DURATION 0.3f	//	翻转动画持续时间


//	玩家手牌——枚举
enum EDZPKCardID
{
	EDZPKCardID_1 = 0,	//	第一张
	EDZPKCardID_2,		//	第二张
	EDZPKCardID_3,		//	第三张
	EDZPKCardID_4,		//	第四张
	EDZPKCardID_5		//	第五张
};

class DZPKCard: public Node
{
public:
	DZPKCard();
	~DZPKCard();
	CREATE_FUNC(DZPKCard);
	virtual bool init();
	void initUI();

	//	设置卡牌正面图形
	void setCardObverse(unsigned char cCardData);

	/*
	**	@brief	根据卡牌数据获取卡牌图形
	**	@cCardData	卡牌数据
	**	@return	返回Sprite* 类型的卡牌
	*/
	Sprite* getCard(unsigned char cCardData);

	//	卡牌翻转动画
	void animationTurnCard();

	//	卡牌变灰暗
	void showCardDark();

	//	卡牌变高亮
	void showCardHighLight();

	//	卡牌重置状态
	void resetCard();

	//	获取卡牌数据
	char getCardData() const;

	//	设置默认的位置
	void setNorPos(Vec2 vPosition);

private:
	//	卡牌正面
	Sprite* m_pCardObverse;

	//	卡牌反面
	Sprite* m_pCardReverse;

	//	卡牌高亮描边
	Sprite* m_pCardHighLight;

	//	卡牌数据
	char m_cCardData;

	//	卡牌默认位置
	Vec2 m_vNorPosition;
};


//	德州扑克——玩家手牌
class DZPKHandCard: public Node
{
public:
	DZPKHandCard();
	~DZPKHandCard();
	CREATE_FUNC(DZPKHandCard);
	bool init();
	void initUI();

	//	设置手牌
	void setHandCardImg(int nCardID, unsigned char cCardData);

	//	翻牌动画
	void aniTurnCard(int nCardID);

	//	设置卡牌可见性
	void setCardVisible(int nCardID, bool bIsVisible);

	//	获取手牌数据
	char getHandCardData(int nCardID) const;

	//	重置手牌
	void resetHandCard();

	//	手牌
	DZPKCard* m_pImgHandCard[DZPK_HAND_CARD_COUNT];
private:

};

//	德州扑克——台面公共牌
class DZPKCommunityCards: public Node
{
public:
	DZPKCommunityCards();
	~DZPKCommunityCards();
	CREATE_FUNC(DZPKCommunityCards);
	bool init();
	void initUI();

	//	设置公共牌
	void setCommunityCardImg(int nCardID, unsigned char cCardData);

	//	重置公共牌
	void resetCommunityCards();

	//	获取公共牌图像
	DZPKCard* getCommunityCardImg(int nCardID) const;

	//	获取公共牌数据
	char* getComunityCardData();

	//	显示前三张公共牌
	void showFirstThreeCard(char cCardData[]);

	//	显示第四张公共牌
	void showFourthCard(char cCardData);

	//	显示第五张公共牌
	void showFifthCard(char cCardData);

	//	展现公共牌
	void showCommunityCard(char cCardData[]);

	//	公共牌
	DZPKCard* m_pImgComunityCard[MAX_CENTERCOUNT];
private:


	//	公共牌数据
	char m_cComunityCardData[MAX_CENTERCOUNT];
};


#endif	//	__DZPK_CARD_H__
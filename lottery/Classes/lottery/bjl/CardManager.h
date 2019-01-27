#ifndef __GAME_CARDMANAGER_H__
#define __GAME_CARDMANAGER_H__
#pragma once

/***************************************
 牌管理类
 游戏界面

 ***************************************/

#include "cocos2d.h"
#include "CardSprite.h"
#include "CMD_BJL.h"//by hxh
#include "Tools.h"//by hxh
USING_NS_CC;
using namespace cocos2d;

// add by hxh 20160806
//#define USE_PRINTTEST3

struct PlayerCard
{
	//玩家手中的三张牌
	CardSprite* cards[3];
};

class CardManager : public cocos2d::Layer
{
public:
	virtual void onExit();//add by zengxin 20160921
	//创建函数
	static CardManager *create();
	void showPlayerCardfive();
	//显示显示前两张牌
	void showPlayerCard(); 
	//显示第三张手牌
	void showPlayerCard1(BYTE m_cbTableCardArray[2][3],BYTE m_cbCardCount[2]);
	// 显示庄家第三张手牌<by hxh>
	void showBankThirdCard(BYTE m_cbTableCardArray[2][3]);

	//游戏结束刷新手牌
	void updatePlayerCard(int chairID,unsigned char bHandCardData[3]);
	//设置手牌
	void SetHandCardData(unsigned char bHandCardData[]);
	//设置手牌
	void SetHandCardData1(unsigned char bHandCardData1[]);

	//发牌动画
	//(闲家第一张)
	void SetAnmionPlayOneCard();
	//(庄家第一张)
	void SetAnmionBankoneCard(Object* obj);
    //(闲家第二张)
	void SetAnmionPlayTwoCard(Object* obj);
	//庄家第二张
	void SetAnmionBankTwoCard(Object* obj);
	//闲家第三张
	void  SendCardToPlayerRuntion(float loat);
	//庄家第三张
	void  SendCardToBankThirdCard(float loat);
	// 发牌音效<by hxh 20160729>
	void playSendCardSoundEffect();

	//////////////////////////////////////////////////////
	//游戏结束重置方法
	void resetManager();
	void HideCards();
	//设置卡片翻滚
	void SetAnimation(Sprite * Sprite,int Number);

	void afterCatchPalyerUsed(Object *obj);//闲家
	void afterCatchBankUsed(Object *obj);//庄家

	void SetAnimationdoule(float loat){}

	//显示百家乐开牌结果界面（不播放动画） <by zengxin>
	void showGameOverWithNoAnim( BYTE m_cbTableCardArray[2][3],BYTE m_cbCardCount[2]);

public:

	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//获取牌点
	BYTE GetCardPip(BYTE cbCardData);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	Sprite *GAMEoverSprite;                        //游戏结束

	LabelTTF* PlayhomeTTf[8];    //8个下注区域的标签<by hxh>
	LabelTTF* ScoreGameover;   //合计输赢<by hxh>
	LabelTTF* Xian;   //闲点数<by hxh>
	LabelTTF* Zhuang; //庄点数<by hxh>

	//设置游戏结束界面时间
	void showGameOver();
	void showGameOveAnmionTime(float dt);
	void remaggerDissGameOver();
	void SetScoreGameOver(LONGLONG lPlayScore,int Index, LONGLONG	lPlayAllScore);

private:
	//初始化函数
	bool initCardManager();
	//初始化所有玩家手牌
	void initPlayerCard();

	void SetPlayBankNumber();
public:
	PlayerCard        playerCard[8];  //玩家数组
	unsigned char     m_ucHandCardData[3];
	unsigned char     m_ucHandCardData1[3];
	int m_ZXP;//庄闲平结果<by hxh>
	//unsigned short    m_bPlayerStatus[8]; 

	Sprite *Player;  //闲家
	Sprite *Banker;  // 庄家
	Sprite *AndBureauSprite;  //和局
    Sprite *XianZhuangBg;  //闲庄背景<by hxh>
	// add by hxh 20160806
#ifdef USE_PRINTTEST3
	LabelTTF *Test;
	LabelTTF *Test2;
#endif
public:
	unsigned char     m_ucDeskCardData[5];
	bool FreehomeThirdCard;  //闲家是否发第三张牌 flase表示没有，true表示有
	cocos2d::Size Winsize;
	Sprite *m_pUserPlaySprite;  //闲家点数
	Sprite *m_pUserBankSprite;  //庄家点数

	BYTE cbBankerCount;	//闲家点数(两张牌的时候)
	BYTE cbPlayerTwoCardCount;	//庄家点数
	BYTE m_cbBankerCount;	//显示闲家第三张牌（三张牌的时候）

};

#endif  // __GAME_CARDMANAGER_H__
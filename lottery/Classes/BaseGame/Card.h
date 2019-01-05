#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define DZPK_CARD_OBVERSE_ANGLE_Z 270	//正面卡牌的起始Z轴角度
#define DZPK_CARD_OBVERSE_DELTA_Z 90	//正面卡牌旋转的Z轴角度差
#define DZPK_CARD_REVERSE_ANGLE_Z 0		//背面卡牌的起始Z轴角度
#define DZPK_CARD_REVERSE_DELTA_Z 90	//背面卡牌旋转的Z轴角度差

#define DZPK_CARD_ANI_TURN_DURATION 0.3f	//	翻转动画持续时间

/*
**	@brief	卡牌基础类；默认显示背面
*/
class Card: public Node
{
public:
	Card();
	virtual ~Card();
	CREATE_FUNC(Card);

	//	加载卡牌正面纹理
	void loadCardObverse(const string& sFileName);

	//	加载卡牌背面纹理
	void loadCardReverse(const string& sFileName);

	/*
	**	@brief	展示卡牌正面
	**	@isDoAni	true：动画效果	false：无动画效果
	*/
	void showCardObverse(bool isDoAni = false);

	//	卡牌重置状态
	virtual void reset();

protected:

	virtual bool init();

	virtual void initUI();

	//	卡牌翻转动画——展示卡牌正面
	void doAniOfObverse();

	//	卡牌正面
	Sprite* m_pCardObverse;

	//	卡牌背面
	Sprite* m_pCardReverse;

private:

	//	正面纹理文件名
	string m_sFileNameObverse;

	//	背面纹理文件名
	string m_sFileNameReverse;
};


#endif	//	__CARD_H__
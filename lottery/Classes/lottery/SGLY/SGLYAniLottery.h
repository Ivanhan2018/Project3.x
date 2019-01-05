#ifndef __SGLY_ANI_LOTTERY_H__
#define __SGLY_ANI_LOTTERY_H__

#include "cocos2d.h"

USING_NS_CC;

/*
**	@brief	水果乐园——轮转动画（正方向完整版）
*/
class SGLYAniLottery: public ActionInterval
{
public:
	SGLYAniLottery();
	~SGLYAniLottery();

	static SGLYAniLottery* create(float fDuration, int nIdx);

private:
	void initData(float fDuration, int nIdx);
	void showHighLight(int nIdx);
	void update(float time);

	int m_TargetIdx;	//	目标索引
	int m_nFinishSHift;	//	已走路程
	int m_nCurrIdx;	//	当前汽车logo索引
	bool m_bSegregate;	//	隔离

	int m_nS0;	//	加速段总位移
	int m_nS1;	//	匀速段总位移
	int m_nS2;	//	减速段总位移

	int m_nSSum;	//	总路程
	int m_nEndS0;	//	加速段终点
	int m_nEndS1;	//	匀速段终点
	int m_nEndS2;	//	减速段终点

	int m_nSNor0;	//	加速段单位路程
	int m_nSNor1;	//	匀速段单位路程
	int m_nSNor2;	//	减速段单位路程

	float m_fT0;	//	加速段持续时间
	float m_fT1;	//	匀速段持续时间
	float m_fT2;	//	减速段持续时间

	float m_fAveT1;	//	匀速段平均时间
	float m_fAveV1;	//	匀速段平均速度

	float m_fStartV0;	//	加速段初速度
	float m_fEndV0;	//	加速段末速度

	float m_fStartV2;	//	减速段初速度
	float m_fEndV2;	//	减速段末速度

	float m_fA0;	//	加速段加速度
	float m_fA2;	//	减速段加速度

	float m_fVT;	//	末速度
	float m_fPayT;	//	所需时间
};

/*
**	@brief	水果乐园——轮转动画（正或逆匀速版）
*/
class SGLYAniUniform: public ActionInterval
{
public:
	SGLYAniUniform();
	~SGLYAniUniform();

	/*
	**	@brief	创建水果乐园GoodLuck动画效果
	**	@fDuration	持续时间
	**	@nBeginIdx	开始位置索引
	**	@nEndIdx	结束位置索引
	**	@pMoveTarget	移动光标
	**	@nCount	动画圈数
	**	@bIsForward	是否正转
	*/
static SGLYAniUniform* create(float fDuration, int nBeginIdx, int nEndIdx, Node* pMoveTarget, bool bIsForward = true, int nCount = 1);

private:

	void initData(float fDuration, int nBeginIdx, int nEndIdx, Node* pMoveTarget, bool bIsForward, int nCount);
	void showHighLight(int nIdx);
	void update(float time);

	//	开始位置索引
	int m_nBeginIdx;

	//	结束位置索引
	int m_nEndIdx;

	//	是否正转
	bool m_bIsForward;

	//	总路程
	int m_nTotalS;

	//	平均速度
	float m_fAveV;

	//	已走路程
	int m_nRunS;

	//	当前位置索引
	int m_nCurrIdx;

	//	移动物体
	Node* m_pMoveTarget;

	//	只执行一次声音
	bool m_bOnce;
};

#endif  // __SGLY_ANI_LOTTERY_H__
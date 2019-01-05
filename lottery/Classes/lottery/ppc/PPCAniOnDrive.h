#ifndef __PPC_ANI_ON_DRIVE_H__
#define __PPC_ANI_ON_DRIVE_H__

#include "cocos2d.h"
#include "PPCGameLayer.h"

USING_NS_CC;

class PPCAniOnDrive: public ActionInterval
{
public:
	PPCAniOnDrive();
	~PPCAniOnDrive();

	static PPCAniOnDrive* create(float fDuration, int nIdx);

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

#endif  // __PPC_ANI_ON_DRIVE_H__
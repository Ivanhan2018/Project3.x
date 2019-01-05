#ifndef _DZPKOperaLayer_H_
#define _DZPKOperaLayer_H_
#pragma once

/***************************************
 操作面板
 游戏界面
 xxxx 2014.1.5
 ***************************************/

#include "cocos2d.h"
#include "CMD_DZPK.h"
#include "EntityMgr.h"
#include "RoomLayer.h"
#include "Tools.h"
#include "DZPKRaiseSlider.h"

USING_NS_CC;

// 用于测试<add by hxh 20161021>
//#define SHOW_BET_XZ_BTN

class DZPKLayer;//by hxh

class DZPKOperaLayer : public cocos2d::Layer
{
public:
	//创建函数
	static DZPKOperaLayer *create();  

	//按钮索引<by hxh>
	enum {
		SPRITE_NUM=9,
		CKPX_BTN = 0,			//	查看牌型说明按钮
		CKPX_PIC=1,				//	查看牌型图
		GRYZ_BTN=2,				//	跟任意注按钮
		CHECK_GP_BTN = 7,		//	过牌按钮
		CALL_GZ_BTN = 5,		//	跟注按钮
		RAISE_JZ_BTN = 6,		//	加注按钮
		BET_XZ_BTN = 8,			//	下注按钮
		FOLD_QP_BTN = 3,		//	弃牌按钮
		ALLIN_QX_BTN = 4		//	全下按钮
	};

	// add by hxh 20161009
	void showBtn(int idx,bool visible);	

	//显示右边所有点选按钮(最后两个按钮为跟注和加注)
	void showDXGZJZBtns(int iGZ1GP2);
	//隐藏右边所有点选按钮
	void dismissDXBtns();
	//显示加注条
	void showRaiseSlider();
	//隐藏加注条
	void dismissRaiseSlider();

	// add by hxh 20160928
	DZPKLayer* getDZPKLayer();

	//	加注滑动条
	DZPKRaiseSlider* m_pRaiseSlider;

private:

	//初始化函数
	bool initOperaLayer();
	//执行勾选的操作
	void doGXOpera(float t);
	// add by hxh 20160921
	void SendAddScoreReq(LONGLONG lScore,int idx);
	//点击事件的处理<by hxh>
	void onBtnBegan(int idx);
	void onBtnEnded(int idx);
	//点击事件的监听
	bool onTouchBegan(Touch *pTouch, Event *pEvent);  
	void onTouchEnded(Touch *pTouch, Event *pEvent);   

	// add by hxh 20160919
	static const char * SPRITE_NAME(int idx){
		const char * s_szArr[SPRITE_NUM]={"查看牌型","查看牌型图片","跟任何注","弃牌","全下","跟注","加注","过牌","下注"};
		const char * s=(idx>=0&&idx<SPRITE_NUM?s_szArr[idx]:"");
		return s;
	}

	//"1过","2跟注","3加注","4下注","5弃牌","6全下<add by hxh 20161009>
	static BYTE getJudgeAction(int idx){
		BYTE s_iArr[SPRITE_NUM]={0,0,2,5,6,2,3,1,4};
		BYTE i=(idx>=0&&idx<SPRITE_NUM?s_iArr[idx]:0);
		return i;
	}

	//按钮图片资源<by hxh>
	static const char * getBtnTexName(int idx);
private:

	//按钮<by hxh>
	Sprite *m_pBTN[SPRITE_NUM];
#if 1
	//查看牌型说明按钮
	Sprite *m_pCKPX_BTN;
	//查看牌型图
	Sprite *m_pCKPX_PIC;
	//跟任意注按钮
	Sprite *m_pGRYZ_BTN;
	//弃牌按钮
	Sprite *m_pQP_BTN;
	//跟注按钮
	Sprite *m_pGZ_BTN;
	//加注按钮
	Sprite *m_pJZ_BTN;
	//全下按钮
	Sprite *m_pQX_BTN;
	//过牌按钮
	Sprite *m_pGP_BTN;
	//下注按钮
	Sprite *m_pXZ_BTN;
#endif	

	//跟任意注按钮是否已按下
	bool m_bGRYZ;

	//取加注值或是下注值
	bool m_bGetRaiseInfo;

	//空消息
	CMD_C_NULL cNull;

	//下注消息
	CMD_C_AddScore m_addScore;
};

#endif  // __DZPKOperaLayer_H__
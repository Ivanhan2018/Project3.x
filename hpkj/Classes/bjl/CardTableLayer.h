#ifndef __CARDTABLE_LAYER__
#define __CARDTABLE_LAYER__

//#define USE_PRINTTEST
//#define USE_PRINTTEST2

#define MAX_RECORD_NUM (17)   // 显示记录数目

#include "cocos2d.h"
#include "BJLRes.h"
#include "CMD_BJL.h"
#include "MyTimer.h"
#include "User.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CardTableLayer : public Node
{
public:

	CardTableLayer();
	~CardTableLayer();
	CREATE_FUNC(CardTableLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void waitingMatching(bool isWaiting);			//等待排队

	void upHaiou(float ft);
	void haiouCall(Object* _obj);
	
	// 用户坐庄
	void setUserIsOnline(WORD wBankerUser,LONGLONG lBankerScore,LONGLONG lBankerWinScore, WORD	wBankerTime);
	// 设置庄家头像信息
	void setUserIcon(int userId,int gender,char* name,int gold,char* huafeidian );
	//设置庄家的昵称<>
	bool setBankerName(WORD wBankerUser);
	//设置庄家的金币
	void setBankerScore(LONGLONG Score);
	//设置庄家的成绩和局数
	void setBankUsedChengjiORrJuNumber(LONGLONG Chengji,LONGLONG JuNumber);

	// 设置玩家自己的昵称
	void setNickName(const char * nikeName);
	//设置玩家的分数成绩
	void setPlayAllScoreORchengji(LONGLONG Chengji);

	//用户下注更新积分
	void SetPlaceLandScore(Object *obj);

    //游戏记录新增
	void AddGameRecord(Object *obj);

#ifdef USE_PRINTTEST
	void PrintTest(Object *obj);
	void PrintTest_callback_once(float t);
#endif

#ifdef USE_PRINTTEST2
	void PrintTest2(Object *obj);
	void PrintTest2_callback_once(float t);
#endif

	void initScrollView(BYTE m_cbPlayerCount ,BYTE m_cbBankerCount);
	void AddWinLostResult(int ZXP);
	void SetWinLostResult(int i,int ZXP);
	void SetUpdate();
public:
	cocos2d::extension::ScrollView      *m_pScrollView;
	LONGLONG szText0;
	cocos2d::Size size;// 设计分辨率
private:
	CC_SYNTHESIZE(int,m_coinNumber,CoinNum); 	//金币数量
	CC_SYNTHESIZE(int,m_hhdNumber,HhdNumber);		//话费点数量
	CC_SYNTHESIZE(int,m_times,Times);			//倍率

	SpriteBatchNode * m_pWinLostRecordBatch;	// 输赢记录批结点
	SpriteBatchNode * m_cardTableBatchNode;		//牌桌batchnode
	SpriteBatchNode * m_pdNode;						//排队动画
	Sprite *SpriteGameRecord;             //游戏记录

	// 庄家UI对象改为私有成员变量
	Sprite *FinUsedSprite;
	LabelTTF* laberName;   //姓名
	LabelTTF* goldname;        //金币
	LabelTTF* ChengjiScore;  //地址
	LabelTTF *JiNumber;   //局数

	// 玩家自己UI对象
	LabelTTF * m_coinNumberAtlas;				//金币数字
	Sprite *SpriteMyUsed;                        //用户头像
	LabelTTF * m_nikeName;						//玩家昵称
	LabelTTF *m_Chengji;                          //用户成绩

public:
#ifdef USE_PRINTTEST
	LabelTTF *Test;
#endif

#ifdef USE_PRINTTEST2
	LabelTTF *Test2;
#endif

	bool hnnnnnnn;
	deque<int> m_vWinLostRecord;// 游戏记录
};

#endif

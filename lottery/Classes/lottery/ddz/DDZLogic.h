//
//  DDZLogic.h
//  斗地主逻辑类
//
//  Created by zhouwei on 13-6-20.
//
//

#ifndef DDZ___Game__DDZLogic__
#define DDZ___Game__DDZLogic__

#include "Define.h"

//
//#ifdef WIN32
//#include <windows.h>
//#else
//
//#endif

#include <iostream>



//////////////////////////////////////////////////////////////////////////
//功能：逻辑判断

//排序类型
#define ST_ORDER					0									//大小排序
#define ST_COUNT					1									//数目排序

//////////////////////////////////////////////////////////////////////////
//数目定义

#define MAX_COUNT					20									//最大数目
#define FULL_COUNT					54									//全牌数目
#define BACK_COUNT					3									//底牌数目
#define NORMAL_COUNT				17									//常规数目

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//扑克类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对牌类型
#define CT_THREE					3									//三条类型
#define CT_SINGLE_LINE				4									//单连类型
#define CT_DOUBLE_LINE				5									//对连类型
#define CT_THREE_LINE				6									//三连类型
#define CT_THREE_LINE_TAKE_ONE		7									//三带一单
#define CT_THREE_LINE_TAKE_TWO		8									//三带一对
#define CT_FOUR_LINE_TAKE_ONE		9									//四带两单
#define CT_FOUR_LINE_TAKE_TWO		10									//四带两对
#define CT_BOMB_CARD				11									//炸弹类型
#define CT_MISSILE_CARD				12									//火箭类型


//动画效果标志
#define IDA_WARN                        200                             //报警动画
#define IDA_SMOKE                       201                             //烟雾动画
#define IDA_GOLD                        202                             //金蛋动画
#define IDA_LAND_WIN                    203								//地主胜利
#define IDA_LAND_FAIL                   204								//地主失败
#define IDA_FAR_WIN                     205								//农民胜利
#define IDA_FAR_FAIL                    206								//农民失败
//////////////////////////////////////////////////////////////////////////

//分析结构
struct DDZtagAnalyseResult
{
	unsigned char 							cbFourCount;						//四张数目
	unsigned char 							cbThreeCount;						//三张数目
	unsigned char 							cbDoubleCount;						//两张数目
	unsigned char							cbSignedCount;						//单张数目
	unsigned char							cbFourCardData[MAX_COUNT];			//四张扑克
	unsigned char							cbThreeCardData[MAX_COUNT];			//三张扑克
	unsigned char							cbDoubleCardData[MAX_COUNT];		//两张扑克
	unsigned char							cbSignedCardData[MAX_COUNT];		//单张扑克
};

//出牌结果
struct DDZtagOutCardResult
{
	unsigned char							cbCardCount;						//扑克数目
	unsigned char							cbResultCard[MAX_COUNT];			//结果扑克
};

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class DDZDDZLogic
{
	//变量定义
protected:
	static const unsigned char				m_cbCardData[FULL_COUNT];			//扑克数据
    
	//函数定义
public:
	//构造函数
	DDZDDZLogic();
	//析构函数
	~DDZDDZLogic();
    
	//类型函数
public:
	
    //获取类型
	unsigned char getCardType(const unsigned char cbCardData[], unsigned char cbCardCount);
	
    //获取数值
	unsigned char getCardValue(unsigned char cbCardData) { return cbCardData & MASK_VALUE; }
	
    //获取花色
	unsigned char getCardColor(unsigned char cbCardData) { return cbCardData & MASK_COLOR; }
   
    //获取同牌数量
    unsigned char getSampleCount(unsigned char cbCardData[], unsigned char cbCardCount, unsigned char cbCard);
   
    //搜索同牌(手中牌，匹配牌，中心牌，返回牌)
    bool searchSampleCard(unsigned char cbHandCardData[], unsigned char cbHandCardCount,unsigned char cbCardData[], unsigned char cbCardCount,unsigned char cbCard,DDZtagOutCardResult & OutCardResult);
    
    unsigned char getCardCount(unsigned char cbCardData[], unsigned char cbCardCount,unsigned char cbCenterCard);
	//控制函数
public:
	
    //混乱扑克
	void randCardList(unsigned char cbCardBuffer[], unsigned char cbBufferCount);
	
    //排列扑克
	void sortCardList(unsigned char cbCardData[], unsigned char cbCardCount, unsigned char cbSortType);
	
    //删除扑克
	bool removeCard(const unsigned char cbRemoveCard[], unsigned char cbRemoveCount, unsigned char cbCardData[], unsigned char cbCardCount);
    
	//逻辑函数
public:
	//有效判断
	bool isValidCard(unsigned char cbCardData);
    
    //是否有连牌
    bool islinkCard(unsigned char cbCardData[], unsigned char cbCardCount,DDZtagOutCardResult& OutCardResult);
    
    //是否有双龙
    bool isDoublelinkCard(unsigned char cbCardData[], unsigned char cbCardCount,DDZtagAnalyseResult& AnalyseResult);
    
    //是否拥有中心牌
    unsigned char isHasCardData(unsigned char cbCenterCardData,unsigned char cbCardData[], unsigned char cbCardCount);
    
	//逻辑数值
	unsigned char getCardLogicValue(unsigned char cbCardData);
	
    //对比扑克
	bool compareCard(const unsigned char cbFirstCard[], const unsigned char cbNextCard[], unsigned char cbFirstCount, unsigned char cbNextCount);
	
    //出牌搜索
	bool searchOutCard(const unsigned char cbHandCardData[], unsigned char cbHandCardCount, const unsigned char cbTurnCardData[], unsigned char cbTurnCardCount, DDZtagOutCardResult & OutCardResult);
    
	//内部函数
public:
	//分析扑克
	void analysebCardData(const unsigned char cbCardData[], unsigned char cbCardCount, DDZtagAnalyseResult & AnalyseResult,bool bFlag = false);
	//获得最大分数
// 	long getMaxScore(const long bGameScore[]);
    long int getMaxScore(const long int* bGameScore);
		
	//  [4/1/2014 wucan]
public:
	unsigned char getBackbeiLv(unsigned char* _data,unsigned char _num);
	unsigned char getLastTask(unsigned char* _data,unsigned char _num,unsigned char _key);
	bool isWangZha(unsigned char cbCardData[], unsigned char cbCardCount);
};

//////////////////////////////////////////////////////////////////////////



#endif /* defined(__Game__DDZDDZLogic__) */

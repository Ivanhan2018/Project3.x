//
//  DDZDDZLogic.cpp
//  Game
//
//  Created by zhouwei on 13-6-20.
//
//

#include "DDZLogic.h"
#include "cocos2d.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

//扑克数据
const unsigned char	DDZDDZLogic::m_cbCardData[54]=
{
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
    0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
    0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
    0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
    0x4E,0x4F,
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------对应相应游戏逻辑值------------------
/// 14,15,3,4,5,6,7,8,9,10,11,12,13
/// 14,15,3,4,5,6,7,8,9,10,11,12,13
/// 14,15,3,4,5,6,7,8,9,10,11,12,13
/// 16,17
///////////////////////////////////////////////////////////////////////////////////////////////////

//游戏逻辑的实现
//构造函数
DDZDDZLogic::DDZDDZLogic()
{
    //for (int i = 0; i < 54; i++)
    //{
    //    int value = getCardLogicValue(m_cbCardData[i]);
    //    CCLOG("value = %d",value);
    //}
}

//析构函数
DDZDDZLogic::~DDZDDZLogic()
{
}
//排列扑克
void DDZDDZLogic::sortCardList(unsigned char cbCardData[], unsigned char cbCardCount, unsigned char cbSortType)
{
	//数目过虑
	if (cbCardCount==0) return;
    
	//转换数值
	unsigned char cbSortValue[MAX_COUNT];
	for (unsigned char i=0;i<cbCardCount;i++) cbSortValue[i]=getCardLogicValue(cbCardData[i]);
    
	//排序操作
	bool bSorted=true;
	unsigned char cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (unsigned char i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//交换位置
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}
		}
		cbLast--;
	} while(bSorted==false);
    
	//数目排序
	if (cbSortType==ST_COUNT)
	{
		//分析扑克
		unsigned char cbIndex=0;
		DDZtagAnalyseResult AnalyseResult;
		analysebCardData(cbCardData,cbCardCount,AnalyseResult);
        
		//拷贝四牌
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbFourCardData,sizeof(unsigned char)*AnalyseResult.cbFourCount*4);
		cbIndex+=AnalyseResult.cbFourCount*4;
        
		//拷贝三牌
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbThreeCardData,sizeof(unsigned char)*AnalyseResult.cbThreeCount*3);
		cbIndex+=AnalyseResult.cbThreeCount*3;
        
		//拷贝对牌
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbDoubleCardData,sizeof(unsigned char)*AnalyseResult.cbDoubleCount*2);
		cbIndex+=AnalyseResult.cbDoubleCount*2;
        
		//拷贝单牌
		memcpy(&cbCardData[cbIndex],AnalyseResult.cbSignedCardData,sizeof(unsigned char)*AnalyseResult.cbSignedCount);
		cbIndex+=AnalyseResult.cbSignedCount;
	}
    
	return;
}
//逻辑数值
unsigned char DDZDDZLogic::getCardLogicValue(unsigned char cbCardData)
{
	//扑克属性
	unsigned char cbCardColor=getCardColor(cbCardData);
	unsigned char cbCardValue=getCardValue(cbCardData);
    
	//转换数值
	if (cbCardColor==0x40) return cbCardValue+2;
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//获取同牌数量
unsigned char DDZDDZLogic::getSampleCount(unsigned char cbCardData[], unsigned char cbCardCount, unsigned char cbCard)
{
    unsigned char cbCount = 0;
    for (int i = 0; i < cbCardCount; i++)
    {
        if (getCardValue(cbCardData[i]) == cbCard)
        {
            cbCount++;
        }
    }
    return  cbCount;
}
//分析扑克
void DDZDDZLogic::analysebCardData(const unsigned char cbCardData[], unsigned char cbCardCount, DDZtagAnalyseResult & AnalyseResult,bool bFlag)
{
	//设置结果
	memset(&AnalyseResult,0,sizeof(AnalyseResult));
    
	//扑克分析
	for (unsigned char i=0;i<cbCardCount;i++)
	{
		//变量定义
		unsigned char cbSameCount=1,cbCardValueTemp=0;
		unsigned char cbLogicValue=getCardLogicValue(cbCardData[i]);
        
		//搜索同牌
		for (unsigned char j=i+1;j<cbCardCount;j++)
		{
			//获取扑克
			if (getCardLogicValue(cbCardData[j])!=cbLogicValue) break;
            
			//设置变量
			cbSameCount++;
		}
        
		//设置结果
		switch (cbSameCount)
		{
            case 1:		//单张
			{
				unsigned char cbIndex=AnalyseResult.cbSignedCount++;
				AnalyseResult.cbSignedCardData[cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
            case 2:		//两张
			{
				unsigned char cbIndex=AnalyseResult.cbDoubleCount++;
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
            case 3:		//三张
			{
                if (bFlag)
                {
                    unsigned char cbIndex=AnalyseResult.cbDoubleCount++;
                    AnalyseResult.cbDoubleCardData[cbIndex*2]=cbCardData[i];
                    AnalyseResult.cbDoubleCardData[cbIndex*2+1]=cbCardData[i+1];
                }else
                {
                    unsigned char cbIndex=AnalyseResult.cbThreeCount++;
                    AnalyseResult.cbThreeCardData[cbIndex*cbSameCount]=cbCardData[i];
                    AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
                    AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
                }
				break;
			}
            case 4:		//四张
			{
				unsigned char cbIndex=AnalyseResult.cbFourCount++;
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		}
        
		//设置索引
		i+=cbSameCount-1;
	}
    
	return;
}

//删除扑克
bool DDZDDZLogic::removeCard(const unsigned char cbRemoveCard[], unsigned char cbRemoveCount, unsigned char cbCardData[], unsigned char cbCardCount)
{
	//检验数据
	//assert(cbRemoveCount<=cbCardCount);
	//定义变量
	unsigned char cbDeleteCount = 0;
	unsigned char cbTempCardData[MAX_COUNT];
	//assert(cbCardCount<=CountArray(cbTempCardData));
	memcpy(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));
    
	//置零扑克
	for (unsigned char i=0;i<cbRemoveCount;i++)
	{
		for (unsigned char j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}
	if (cbDeleteCount!=cbRemoveCount)
	{
		//AfxMessageBox("删除错误190",MB_OK);
		return false;
	}
    
/*
	//清理扑克
	memset(cbCardData, 0, sizeof(cbCardData[0]) * MAX_COUNT);
	unsigned char cbCardPos=0;
	for (unsigned char i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}
*/

	unsigned char tmpData[MAX_COUNT] = "";
	for (unsigned char i = 0, j = 0; i < MAX_COUNT; i++)
	{
		if (cbTempCardData[i] != 0)
		{
// 			cbCardData[j++]=cbTempCardData[i];
			tmpData[j++] = cbTempCardData[i];
		}
	}
	memset(cbCardData, 0, sizeof(cbCardData[0]) * MAX_COUNT);
	memcpy(cbCardData, tmpData, sizeof(cbCardData[0]) * MAX_COUNT);
	return true;
}

//获取类型
unsigned char DDZDDZLogic::getCardType(const unsigned char cbCardData[], unsigned char cbCardCount)
{
	//简单牌型
	switch (cbCardCount)
	{
        case 0:	//空牌
		{
			return CT_ERROR;
		}
        case 1: //单牌
		{
			return CT_SINGLE;
		}
        case 2:	//对牌火箭
		{
			//牌型判断
			if ((cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E)||(cbCardData[0]==0x4E)&&(cbCardData[1]==0x4F)) return CT_MISSILE_CARD;
			if (getCardLogicValue(cbCardData[0])==getCardLogicValue(cbCardData[1])) return CT_DOUBLE;
            
			return CT_ERROR;
		}
	}
    
	//分析扑克
	DDZtagAnalyseResult AnalyseResult;
	memset(&AnalyseResult,0,sizeof(AnalyseResult));
	analysebCardData(cbCardData,cbCardCount,AnalyseResult);
    
	//四牌判断
	if (AnalyseResult.cbFourCount>0)
	{
		//牌型判断
		if ((AnalyseResult.cbFourCount==1)&&(cbCardCount==4)) return CT_BOMB_CARD;
		if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbSignedCount==2)&&(cbCardCount==6)) return CT_FOUR_LINE_TAKE_ONE;
		if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbDoubleCount==2)&&(cbCardCount==8)) return CT_FOUR_LINE_TAKE_TWO;
        
        //炸弹和三张组合成飞机带翅膀
		if (AnalyseResult.cbThreeCount==1&&AnalyseResult.cbSignedCount==1&&AnalyseResult.cbFourCount==1&&cbCardCount==8)
		{
			if (getCardLogicValue(AnalyseResult.cbFourCardData[0])-getCardLogicValue(AnalyseResult.cbThreeCardData[0])==1)
                return CT_THREE_LINE_TAKE_ONE;
			if (getCardLogicValue(AnalyseResult.cbThreeCardData[0])-getCardLogicValue(AnalyseResult.cbFourCardData[0])==1)
                return CT_THREE_LINE_TAKE_ONE;
		}
		//炸弹和两个三张组合
		if (AnalyseResult.cbThreeCount==2&&AnalyseResult.cbSignedCount==2&&AnalyseResult.cbFourCount==1&&cbCardCount==12)
		{
			if (getCardLogicValue(AnalyseResult.cbThreeCardData[0])-getCardLogicValue(AnalyseResult.cbThreeCardData[3])==1)
			{
				if (getCardLogicValue(AnalyseResult.cbFourCardData[0])-getCardLogicValue(AnalyseResult.cbThreeCardData[0])==1)
					return CT_THREE_LINE_TAKE_ONE;
				if (getCardLogicValue(AnalyseResult.cbThreeCardData[3])-getCardLogicValue(AnalyseResult.cbFourCardData[0])==1)
					return CT_THREE_LINE_TAKE_ONE;
                
			}
			if ((getCardLogicValue(AnalyseResult.cbThreeCardData[0])-getCardLogicValue(AnalyseResult.cbFourCardData[0])==1)&&
				(getCardLogicValue(AnalyseResult.cbFourCardData[0])-getCardLogicValue(AnalyseResult.cbThreeCardData[3])==1))
				return CT_THREE_LINE_TAKE_ONE;
		}
        
		return CT_ERROR;
	}
    
	//三牌判断
	if (AnalyseResult.cbThreeCount>0)
	{
		//三条类型
		if(AnalyseResult.cbThreeCount==1 && cbCardCount==3) return CT_THREE ;
        
		//连牌判断
		if (AnalyseResult.cbThreeCount>1)
		{
			//变量定义
			unsigned char cbCardData=AnalyseResult.cbThreeCardData[0];
			unsigned char cbFirstLogicValue=getCardLogicValue(cbCardData);
            
			//错误过虑
			if (cbFirstLogicValue>=15) return CT_ERROR;
            
			//连牌判断
			for (unsigned char i=1;i<AnalyseResult.cbThreeCount;i++)
			{
				unsigned char cbCardData=AnalyseResult.cbThreeCardData[i*3];
				if (cbFirstLogicValue!=(getCardLogicValue(cbCardData)+i)) return CT_ERROR;
			}
		}
        
		//牌形判断
		if (AnalyseResult.cbThreeCount*3==cbCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbThreeCount*4==cbCardCount) return CT_THREE_LINE_TAKE_ONE;
		if ((AnalyseResult.cbThreeCount*5==cbCardCount)&&(AnalyseResult.cbDoubleCount==AnalyseResult.cbThreeCount)) return CT_THREE_LINE_TAKE_TWO;
        
		return CT_ERROR;
	}
    
	//两张类型
	if (AnalyseResult.cbDoubleCount>=3)
	{
		//变量定义
		unsigned char cbCardData=AnalyseResult.cbDoubleCardData[0];
		unsigned char cbFirstLogicValue=getCardLogicValue(cbCardData);
        
		//错误过虑
		if (cbFirstLogicValue>=15) return CT_ERROR;
        
		//连牌判断
		for (unsigned char i=1;i<AnalyseResult.cbDoubleCount;i++)
		{
			unsigned char cbCardData=AnalyseResult.cbDoubleCardData[i*2];
			if (cbFirstLogicValue!=(getCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}
        
		//二连判断
		if ((AnalyseResult.cbDoubleCount*2)==cbCardCount) return CT_DOUBLE_LINE;
        
		return CT_ERROR;
	}
    
	//单张判断
	if ((AnalyseResult.cbSignedCount>=5)&&(AnalyseResult.cbSignedCount==cbCardCount))
	{
		//变量定义
		unsigned char cbCardData=AnalyseResult.cbSignedCardData[0];
		unsigned char cbFirstLogicValue=getCardLogicValue(cbCardData);
        
		//错误过虑
		if (cbFirstLogicValue>=15) return CT_ERROR;
        
		//连牌判断
		for (unsigned char i=1;i<AnalyseResult.cbSignedCount;i++)
		{
			unsigned char cbCardData=AnalyseResult.cbSignedCardData[i];
			if (cbFirstLogicValue!=(getCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}
        
		return CT_SINGLE_LINE;
	}
	return CT_ERROR;
}

//对比扑克
bool DDZDDZLogic::compareCard(const unsigned char cbFirstCard[], const unsigned char cbNextCard[], unsigned char cbFirstCount, unsigned char cbNextCount)
{
	//获取类型
	unsigned char cbNextType=getCardType(cbNextCard,cbNextCount);
	unsigned char cbFirstType=getCardType(cbFirstCard,cbFirstCount);
    
	//类型判断
	if (cbNextType==CT_ERROR) return false;
	//ASSERT(cbNextType!=CT_ERROR);
	if (cbNextType==CT_MISSILE_CARD) return true;
    
	//炸弹判断
	if ((cbFirstType!=CT_BOMB_CARD)&&(cbNextType==CT_BOMB_CARD)) return true;
	if ((cbFirstType==CT_BOMB_CARD)&&(cbNextType!=CT_BOMB_CARD)) return false;
    
	//规则判断
	if ((cbFirstType==cbNextType)&&(cbFirstCount!=cbNextCount)) return false;
    if (cbFirstType!=cbNextType) return false;
	//开始对比
	switch (cbNextType)
	{
        case CT_SINGLE:
        case CT_DOUBLE:
        case CT_THREE:
        case CT_SINGLE_LINE:
        case CT_DOUBLE_LINE:
        case CT_THREE_LINE:
        case CT_BOMB_CARD:
		{
			//获取数值
			unsigned char cbNextLogicValue=getCardLogicValue(cbNextCard[0]);
			unsigned char cbFirstLogicValue=getCardLogicValue(cbFirstCard[0]);
            
			//对比扑克
			return cbNextLogicValue>cbFirstLogicValue;
		}
        case CT_THREE_LINE_TAKE_ONE:
        case CT_THREE_LINE_TAKE_TWO:
		{
			//分析扑克
			DDZtagAnalyseResult NextResult;
			DDZtagAnalyseResult FirstResult;
			analysebCardData(cbNextCard,cbNextCount,NextResult);
			analysebCardData(cbFirstCard,cbFirstCount,FirstResult);
			
			if (NextResult.cbFourCount>0&&NextResult.cbThreeCount>0)
			{
				if (getCardLogicValue(NextResult.cbFourCardData[0])>getCardLogicValue(NextResult.cbThreeCardData[0]))
                    NextResult.cbThreeCardData[0]= NextResult.cbFourCardData[0];
			}
            
			if (FirstResult.cbFourCount>0&&FirstResult.cbThreeCount>0)
			{
				if (getCardLogicValue(FirstResult.cbFourCardData[0])>getCardLogicValue(FirstResult.cbThreeCardData[0]))
                    FirstResult.cbThreeCardData[0]= FirstResult.cbFourCardData[0];
			}
			////////////////////////////////
            
			//获取数值
			unsigned char cbNextLogicValue=getCardLogicValue(NextResult.cbThreeCardData[0]);
			unsigned char cbFirstLogicValue=getCardLogicValue(FirstResult.cbThreeCardData[0]);
            
			//对比扑克
			return cbNextLogicValue>cbFirstLogicValue;
		}
        case CT_FOUR_LINE_TAKE_ONE:
        case CT_FOUR_LINE_TAKE_TWO:
		{
			//分析扑克
			DDZtagAnalyseResult NextResult;
			DDZtagAnalyseResult FirstResult;
			analysebCardData(cbNextCard,cbNextCount,NextResult);
			analysebCardData(cbFirstCard,cbFirstCount,FirstResult);
            
			//获取数值
			unsigned char cbNextLogicValue=getCardLogicValue(NextResult.cbFourCardData[0]);
			unsigned char cbFirstLogicValue=getCardLogicValue(FirstResult.cbFourCardData[0]);
            
			//对比扑克
			return cbNextLogicValue>cbFirstLogicValue;
		}
	}
    
	return false;
}

unsigned char DDZDDZLogic::getCardCount(unsigned char cbCardData[], unsigned char cbCardCount,unsigned char cbCenterCard)
{
    unsigned char cbCount = 0;
    for (unsigned char i = 0; i < cbCardCount; i++)
    {
        if (getCardValue(cbCardData[i]) == getCardValue(cbCenterCard))
        {
            cbCount++;
        }
    }
    return cbCount;
}
//搜索同牌(手中牌，匹配牌，中心牌，返回牌)
bool DDZDDZLogic::searchSampleCard(unsigned char cbHandCardData[], unsigned char cbHandCardCount,unsigned char cbCardData[], unsigned char cbCardCount,unsigned char cbCard, DDZtagOutCardResult& OutCardResult)
{
    unsigned char cbTempCardData[20];
    unsigned char cbTempCardCount = cbCardCount;
    memset(cbTempCardData, 0, 20);
    
    memcpy(cbCardData,cbCardData, cbHandCardCount);
    
    bool ret = searchOutCard(cbHandCardData,cbHandCardCount,cbCardData,cbCardCount,OutCardResult);
    bool result = false;
    if (ret)
    {
        unsigned char cbValue = getCardValue(cbCard);
        for (int i = 0; i < OutCardResult.cbCardCount; i++)
        {
            if (cbValue == getCardValue(OutCardResult.cbResultCard[i]))
            {
                result = true;
                return true;
            }
        }
        
        //没有找到中心牌 递归搜索
        if (!result)
        {
            //memcpy(cbTempCardData, OutCardResult.cbResultCard, OutCardResult.cbCardCount);
            //重新根据中心牌搜索合适的牌
            //memset(OutCardResult.cbResultCard, 0, MAX_COUNT);
            //searchSampleCard(cbHandCardData, cbHandCardCount,cbTempCardData, cbCardCount,cbCard,OutCardResult);
            
            
            unsigned char cbHandCard[20];
            unsigned char cbHandCout = cbHandCardCount;
            memcpy(cbHandCard, cbHandCardData, sizeof(cbHandCard));
            
            removeCard(OutCardResult.cbResultCard, OutCardResult.cbCardCount, cbHandCard, cbHandCardCount);
            cbHandCout -= OutCardResult.cbCardCount;
            
            searchSampleCard(cbHandCard, cbHandCout,OutCardResult.cbResultCard, cbCardCount,cbCard,OutCardResult);
        }
    }
    
    return ret;
}

//出牌搜索
bool DDZDDZLogic::searchOutCard(const unsigned char cbHandCardData[], unsigned char cbHandCardCount, const unsigned char cbTurnCardData[], unsigned char cbTurnCardCount, DDZtagOutCardResult& OutCardResult)
{
	//设置结果
	memset(&OutCardResult, 0, sizeof(OutCardResult));
    
	//构造扑克
	unsigned char cbCardData[MAX_COUNT];
	unsigned char cbCardCount=cbHandCardCount;
	memcpy(cbCardData,cbHandCardData,sizeof(unsigned char)*cbHandCardCount);
    
	//排列扑克
	sortCardList(cbCardData,cbCardCount,ST_ORDER);
	//获取类型
	unsigned char cbTurnOutType=getCardType(cbTurnCardData,cbTurnCardCount);
    
	//出牌分析
	switch (cbTurnOutType)
	{
        case CT_ERROR:					//错误类型
		{
			return false;
		}
        case CT_SINGLE:					//单牌类型
		{
			//获取数值
			unsigned char cbLogicValue=getCardLogicValue(cbTurnCardData[0]);
			//分析扑克
			DDZtagAnalyseResult AnalyseResult;
			analysebCardData(cbCardData,cbCardCount,AnalyseResult);
			//寻找单牌
			for (unsigned char i=0;i<AnalyseResult.cbSignedCount;i++)
			{
				unsigned char cbIndex=AnalyseResult.cbSignedCount-i-1;
				if (getCardLogicValue(AnalyseResult.cbSignedCardData[cbIndex])>cbLogicValue)
				{
					//设置结果
					OutCardResult.cbCardCount=cbTurnCardCount;
					memcpy(OutCardResult.cbResultCard,&AnalyseResult.cbSignedCardData[cbIndex],sizeof(unsigned char)*cbTurnCardCount);
					return true;
				}
			}
			//全部查找
			for (WORD i=0;i<cbCardCount;i++)
			{
				if (getCardLogicValue(cbCardData[cbCardCount-i-1])>cbLogicValue)
				{
					memcpy(OutCardResult.cbResultCard,&cbCardData[cbCardCount-i-1],sizeof(unsigned char)*cbTurnCardCount);
                    OutCardResult.cbCardCount = cbTurnCardCount;
					return true;
				}
			}
			break;
		}
        case CT_DOUBLE:					//对牌类型
            
		{
			//获取数值
			unsigned char cbLogicValue=getCardLogicValue(cbTurnCardData[0]);
			//分析扑克
			DDZtagAnalyseResult AnalyseResult;
			analysebCardData(cbCardData,cbCardCount,AnalyseResult);
			//寻找对牌
			for (unsigned char i=0;i<AnalyseResult.cbDoubleCount;i++)
			{
				unsigned char cbIndex=(AnalyseResult.cbDoubleCount-i-1)*2;
				if (getCardLogicValue(AnalyseResult.cbDoubleCardData[cbIndex])>cbLogicValue)
				{
					//设置结果
					OutCardResult.cbCardCount=cbTurnCardCount;
					memcpy(OutCardResult.cbResultCard,&AnalyseResult.cbDoubleCardData[cbIndex],sizeof(unsigned char)*cbTurnCardCount);
					return true;
				}
			}
			//拆三张
			for (unsigned char i=0;i<AnalyseResult.cbThreeCount;i++)
			{
				unsigned char cbIndex=(AnalyseResult.cbThreeCount-i-1)*3;
				if (getCardLogicValue(AnalyseResult.cbThreeCardData[cbIndex])>cbLogicValue)
				{
					//设置结果
					OutCardResult.cbCardCount=cbTurnCardCount;
					memcpy(OutCardResult.cbResultCard,&AnalyseResult.cbThreeCardData[cbIndex],sizeof(unsigned char)*cbTurnCardCount);
                    
					return true;
				}
			}
			break;
		}
            
        case CT_THREE:					//三条类型
		{
			//获取数值
			unsigned char cbLogicValue=getCardLogicValue(cbTurnCardData[0]);
			//分析扑克
			DDZtagAnalyseResult AnalyseResult;
			analysebCardData(cbCardData,cbCardCount,AnalyseResult);
			for (unsigned char i=0;i<AnalyseResult.cbThreeCount;i++)
			{
				unsigned char cbIndex=(AnalyseResult.cbThreeCount-i-1)*3;
				if (getCardLogicValue(AnalyseResult.cbThreeCardData[cbIndex])>cbLogicValue)
				{
					//设置结果
					OutCardResult.cbCardCount=cbTurnCardCount;
					memcpy(OutCardResult.cbResultCard,&AnalyseResult.cbThreeCardData[cbIndex],sizeof(unsigned char)*cbTurnCardCount);
					return true;
				}
			}
			break;
		}
        case CT_SINGLE_LINE:		//单连类型
		{
			//长度判断
			if (cbCardCount<cbTurnCardCount) break;
            
			//获取数值
			unsigned char cbLogicValue=getCardLogicValue(cbTurnCardData[0]);
            
			//搜索连牌
			for (unsigned char i=(cbTurnCardCount-1);i<cbCardCount;i++)
			{
				//获取数值
				unsigned char cbHandLogicValue=getCardLogicValue(cbCardData[cbCardCount-i-1]);
                
				//构造判断
				if (cbHandLogicValue>=15) break;
				if (cbHandLogicValue<=cbLogicValue) continue;
                
				//搜索连牌
				unsigned char cbLineCount=0;
				for (unsigned char j=(cbCardCount-i-1);j<cbCardCount;j++)
				{
					if ((getCardLogicValue(cbCardData[j])+cbLineCount)==cbHandLogicValue)
					{
						//增加连数
						OutCardResult.cbResultCard[cbLineCount++]=cbCardData[j];
                        
						//完成判断
						if (cbLineCount==cbTurnCardCount)
						{
							OutCardResult.cbCardCount=cbTurnCardCount;
							return true;
						}
					}
				}
			}
            
			break;
		}
        case CT_DOUBLE_LINE:		//对连类型
		{
			//长度判断
			if (cbCardCount<cbTurnCardCount) break;
            
			//获取数值
			unsigned char cbLogicValue=getCardLogicValue(cbTurnCardData[0]);
            
			//搜索连牌
			for (unsigned char i=(cbTurnCardCount-1);i<cbCardCount;i++)
			{
				//获取数值
				unsigned char cbHandLogicValue=getCardLogicValue(cbCardData[cbCardCount-i-1]);
                
				//构造判断
				if (cbHandLogicValue<=cbLogicValue) continue;
				if ((cbTurnCardCount>1)&&(cbHandLogicValue>=15)) break;
                
				//搜索连牌
				unsigned char cbLineCount=0;
				for (unsigned char j=(cbCardCount-i-1);j<(cbCardCount-1);j++)
				{
					if (((getCardLogicValue(cbCardData[j])+cbLineCount)==cbHandLogicValue)
						&&((getCardLogicValue(cbCardData[j+1])+cbLineCount)==cbHandLogicValue))
					{
						//增加连数
						OutCardResult.cbResultCard[cbLineCount*2]=cbCardData[j];
						OutCardResult.cbResultCard[(cbLineCount++)*2+1]=cbCardData[j+1];
                        
						//完成判断
						if (cbLineCount*2==cbTurnCardCount)
						{
							OutCardResult.cbCardCount=cbTurnCardCount;
							return true;
						}
					}
				}
			}
            
			break;
		}
        case CT_THREE_LINE:				//三连类型
        case CT_THREE_LINE_TAKE_ONE:	//三带一单
        case CT_THREE_LINE_TAKE_TWO:	//三带一对
		{
			//长度判断
			if (cbCardCount<cbTurnCardCount) break;
            
			//获取数值
			unsigned char cbLogicValue=0;
			for (unsigned char i=0;i<cbTurnCardCount-2;i++)
			{
				cbLogicValue=getCardLogicValue(cbTurnCardData[i]);
				if (getCardLogicValue(cbTurnCardData[i+1])!=cbLogicValue) continue;
				if (getCardLogicValue(cbTurnCardData[i+2])!=cbLogicValue) continue;
				break;
			}
            
			//属性数值
			unsigned char cbTurnLineCount=0;
			if (cbTurnOutType==CT_THREE_LINE_TAKE_ONE) cbTurnLineCount=cbTurnCardCount/4;
			else if (cbTurnOutType==CT_THREE_LINE_TAKE_TWO) cbTurnLineCount=cbTurnCardCount/5;
			else cbTurnLineCount=cbTurnCardCount/3;
            
			//搜索连牌
			for (unsigned char i=cbTurnLineCount*3-1;i<cbCardCount;i++)
			{
				//获取数值
				unsigned char cbHandLogicValue=getCardLogicValue(cbCardData[cbCardCount-i-1]);
                
				//构造判断
				if (cbHandLogicValue<=cbLogicValue) continue;
				if ((cbTurnLineCount>1)&&(cbHandLogicValue>=15)) break;
                
				//搜索连牌
				unsigned char cbLineCount=0;
				for (unsigned char j=(cbCardCount-i-1);j<(cbCardCount-2);j++)
				{
					//设置变量
					OutCardResult.cbCardCount=0;
                    
					//三牌判断
					if ((getCardLogicValue(cbCardData[j])+cbLineCount)!=cbHandLogicValue) continue;
					if ((getCardLogicValue(cbCardData[j+1])+cbLineCount)!=cbHandLogicValue) continue;
					if ((getCardLogicValue(cbCardData[j+2])+cbLineCount)!=cbHandLogicValue) continue;
                    
					//增加连数
					OutCardResult.cbResultCard[cbLineCount*3]=cbCardData[j];
					OutCardResult.cbResultCard[cbLineCount*3+1]=cbCardData[j+1];
					OutCardResult.cbResultCard[(cbLineCount++)*3+2]=cbCardData[j+2];
                    
					//完成判断
					if (cbLineCount==cbTurnLineCount)
					{
						//连牌设置
						OutCardResult.cbCardCount=cbLineCount*3;
                        
						//构造扑克
						unsigned char cbLeftCardData[MAX_COUNT];
						unsigned char cbLeftCount=cbCardCount-OutCardResult.cbCardCount;
						memcpy(cbLeftCardData,cbCardData,sizeof(unsigned char)*cbCardCount);
						removeCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,cbLeftCardData,cbCardCount);
                        
						//分析扑克
						DDZtagAnalyseResult AnalyseResultLeft;
						analysebCardData(cbLeftCardData,cbLeftCount,AnalyseResultLeft);
                        
						//单牌处理
						if (cbTurnOutType==CT_THREE_LINE_TAKE_ONE)
						{
							//提取单牌
							for (unsigned char k=0;k<AnalyseResultLeft.cbSignedCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;
                                
								//设置扑克
								unsigned char cbIndex=AnalyseResultLeft.cbSignedCount-k-1;
								unsigned char cbSignedCard=AnalyseResultLeft.cbSignedCardData[cbIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							}
                            
							//提取对牌
							for (unsigned char k=0;k<AnalyseResultLeft.cbDoubleCount*2;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;
                                
								//设置扑克
								unsigned char cbIndex=(AnalyseResultLeft.cbDoubleCount*2-k-1);
								unsigned char cbSignedCard=AnalyseResultLeft.cbDoubleCardData[cbIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							}
                            
							//提取三牌
							for (unsigned char k=0;k<AnalyseResultLeft.cbThreeCount*3;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;
                                
								//设置扑克
								unsigned char cbIndex=(AnalyseResultLeft.cbThreeCount*3-k-1);
								unsigned char cbSignedCard=AnalyseResultLeft.cbThreeCardData[cbIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							}
                            
							//提取四牌
							//for (unsigned char k=0;k<AnalyseResultLeft.cbFourCount*4;k++)
							//{
							//	//中止判断
							//	if (OutCardResult.cbCardCount==cbTurnCardCount) break;
                            
							//	//设置扑克
							//	unsigned char cbIndex=(AnalyseResultLeft.cbFourCount*4-k-1);
							//	unsigned char cbSignedCard=AnalyseResultLeft.cbFourCardData[cbIndex];
							//	OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							//}
						}
                        
						//对牌处理
						if (cbTurnOutType==CT_THREE_LINE_TAKE_TWO)
						{
							//提取对牌
							for (unsigned char k=0;k<AnalyseResultLeft.cbDoubleCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;
                                
								//设置扑克
								unsigned char cbIndex=(AnalyseResultLeft.cbDoubleCount-k-1)*2;
								unsigned char cbCardData1=AnalyseResultLeft.cbDoubleCardData[cbIndex];
								unsigned char cbCardData2=AnalyseResultLeft.cbDoubleCardData[cbIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
							}
                            
							//提取三牌
							for (unsigned char k=0;k<AnalyseResultLeft.cbThreeCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;
                                
								//设置扑克
								unsigned char cbIndex=(AnalyseResultLeft.cbThreeCount-k-1)*3;
								unsigned char cbCardData1=AnalyseResultLeft.cbThreeCardData[cbIndex];
								unsigned char cbCardData2=AnalyseResultLeft.cbThreeCardData[cbIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
							}
                            
							//提取四牌
							for (unsigned char k=0;k<AnalyseResultLeft.cbFourCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;
                                
								//设置扑克
								unsigned char cbIndex=(AnalyseResultLeft.cbFourCount-k-1)*4;
								unsigned char cbCardData1=AnalyseResultLeft.cbFourCardData[cbIndex];
								unsigned char cbCardData2=AnalyseResultLeft.cbFourCardData[cbIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
							}
						}
                        
						//完成判断
						if (OutCardResult.cbCardCount==cbTurnCardCount) return true;
					}
				}
			}
            
			break;
		}
	}
	//搜索炸弹
	if ((cbCardCount>=4)&&(cbTurnOutType!=CT_MISSILE_CARD))
	{
		//变量定义
		unsigned char cbLogicValue=0;
		if (cbTurnOutType==CT_BOMB_CARD) cbLogicValue=getCardLogicValue(cbTurnCardData[0]);
        
		//搜索炸弹
		for (unsigned char i=3;i<cbCardCount;i++)
		{
			//获取数值
			unsigned char cbHandLogicValue=getCardLogicValue(cbCardData[cbCardCount-i-1]);
            
			//构造判断
			if (cbHandLogicValue<=cbLogicValue) continue;
            
			//炸弹判断
			unsigned char cbTempLogicValue=getCardLogicValue(cbCardData[cbCardCount-i-1]);
            
            unsigned char j = 1;
			for ( ; j<4; j++)
			{
				if (getCardLogicValue(cbCardData[cbCardCount+j-i-1])!=cbTempLogicValue) break;
			}
			if (j!=4) continue;
            
			//设置结果
			OutCardResult.cbCardCount=4;
			OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-i-1];
			OutCardResult.cbResultCard[1]=cbCardData[cbCardCount-i];
			OutCardResult.cbResultCard[2]=cbCardData[cbCardCount-i+1];
			OutCardResult.cbResultCard[3]=cbCardData[cbCardCount-i+2];
            
			return true;
		}
	}
    
	//搜索火箭
	if ((cbCardCount>=2)&&(cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E))
	{
		//设置结果
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=cbCardData[0];
		OutCardResult.cbResultCard[1]=cbCardData[1];
        
		return true;
	}
    
	return false;
}
//获得最大分数
// LONG DDZDDZLogic::getMaxScore(const LONG bGameScore[])
long int DDZDDZLogic::getMaxScore(const long int* bGameScore)
{
    
	LONG ReMaxScore = 0;
	ReMaxScore = (bGameScore[0]>bGameScore[1])?bGameScore[0]:bGameScore[1];
	ReMaxScore = (bGameScore[2]>ReMaxScore)?bGameScore[2]:ReMaxScore;
	return ReMaxScore;
}

//是否有连牌
bool DDZDDZLogic::islinkCard(unsigned char cbCardData[], unsigned char cbCardCount,DDZtagOutCardResult& OutCardResult)
{
    memset(&OutCardResult.cbResultCard, 0 , sizeof(OutCardResult.cbResultCard));
    OutCardResult.cbCardCount= 0;
    
    unsigned char cbCard = getCardLogicValue(cbCardData[0]);
    int j = 0;
    for (int i = 0; i < cbCardCount; i++)
    {
        unsigned char cbRetCard = isHasCardData(cbCard, cbCardData, cbCardCount);
        if (cbRetCard > 0&& cbCard<15)//&& cbCard<15 xiaohua
        {
            OutCardResult.cbResultCard[j] = cbRetCard;
            OutCardResult.cbCardCount++;
            
            j++;
            cbCard--;
        }else
        {
            break;
        }
    }

    if (OutCardResult.cbCardCount >= 5)
    {
        return  true;
    }
    return false;
}

//是否有双龙
bool DDZDDZLogic::isDoublelinkCard(unsigned char cbCardData[], unsigned char cbCardCount,DDZtagAnalyseResult& AnalyseResult)
{
    memset(&AnalyseResult, 0, sizeof(AnalyseResult));
    analysebCardData(cbCardData,cbCardCount,AnalyseResult,true);
    
    for (int i = 0; i < AnalyseResult.cbDoubleCount * 2; i++)
    {
        CCLOG("%d",AnalyseResult.cbDoubleCardData[i]);
    }
    if (AnalyseResult.cbDoubleCount >= 3 )
    {

        bool bRet = false;
        int nLink = 0;
        for (int i = 0; i < AnalyseResult.cbDoubleCount; i++)
        {
            unsigned char cbFirstCardData = getCardLogicValue(AnalyseResult.cbDoubleCardData[i*2]);
            unsigned char cbSecondCardData = getCardLogicValue(AnalyseResult.cbDoubleCardData[i*2+2]);
			if(cbFirstCardData>=15)break; //xiaohua
            if (cbFirstCardData == cbSecondCardData + 1)
            {
                AnalyseResult.cbDoubleCardData[nLink*2] =
                AnalyseResult.cbDoubleCardData[i*2];
                AnalyseResult.cbDoubleCardData[nLink*2+1] =
                AnalyseResult.cbDoubleCardData[i*2+1];
                nLink++;
            }else
            {
                //有三连队
                if (nLink >= 2)
                {
                    AnalyseResult.cbDoubleCardData[nLink*2] =
                    AnalyseResult.cbDoubleCardData[i*2];
                    AnalyseResult.cbDoubleCardData[nLink*2+1] =
                    AnalyseResult.cbDoubleCardData[i*2+1];
                    
                    AnalyseResult.cbDoubleCount = nLink + 1;
                    bRet = true;
                }else
                {
                  nLink = 0;
                }
            }
        }
        return bRet;
    }
    
    return false;
}

//是否拥有中心牌
unsigned char DDZDDZLogic::isHasCardData(unsigned char cbCenterCardData,unsigned char cbCardData[], unsigned char cbCardCount)
{
    for (int i = 0; i < cbCardCount; i++)
    {
        //CCLOG("%d",cbCardData[i]);
        if (cbCenterCardData == getCardLogicValue(cbCardData[i]))
        {
            return cbCardData[i];
        }
    }
    return  0;
}

unsigned char DDZDDZLogic::getBackbeiLv( unsigned char* _data,unsigned char _num )
{
	sortCardList(_data,_num,ST_ORDER);
	WORD _unmBeilv=1;
	do 
	{
		//--王炸
		if ((_data[0]==0x4F)&&(_data[1]==0x4E))
		{
			_unmBeilv *=4;break;
		}

		//--单个王
		if(_data[0]==0x4F || _data[0]==0x4E )
		{
			_unmBeilv *=2;break;
		}

		//--三条
		if(_data[0]==_data[1] &&  _data[1]==_data[2] )
		{
			_unmBeilv *=6;break;
		}

		//--同花
		BYTE type =getCardColor(_data[0]);
		if (type==getCardColor(_data[1]) && type==getCardColor(_data[2]))
		{
			_unmBeilv*=3;
		}

		//--顺子
		BYTE cbFirstLogicValue=getCardLogicValue(_data[0]);
		if(cbFirstLogicValue<=14)
		{
			for (int i=0;i<_num;i++)
			{
				if (cbFirstLogicValue!=(getCardLogicValue(_data[i])+i))
					break;
				else if (i==(_num-1)){
					_unmBeilv*=3;
				}
			}
		}

	} while (0);
	return _unmBeilv;
}

unsigned char DDZDDZLogic::getLastTask( unsigned char* _data,unsigned char _num,unsigned char _key )
{
	WORD type=getCardType(_data,_num);
	if (type == _key)
	{
		switch(_key)
		{
		case CT_SINGLE:
			return 2;
		case CT_DOUBLE:
			return 2;
		case CT_SINGLE_LINE:
			return 3;
		case CT_DOUBLE_LINE:
			return 3;
		case	CT_THREE_LINE:
			return 3;
		case CT_THREE_LINE_TAKE_ONE:
			return 3;
		case CT_THREE_LINE_TAKE_TWO:
			return 3;
		case CT_BOMB_CARD:
			return 4;
		case CT_MISSILE_CARD:
			return 5;
		}
	}
	return 1;
}

bool DDZDDZLogic::isWangZha(unsigned char cbCardData[], unsigned char cbCardCount)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////

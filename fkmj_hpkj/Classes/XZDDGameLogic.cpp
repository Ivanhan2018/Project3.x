#include "XZDDGameLogic.h"

//////////////////////////////////////////////////////////////////////////
FV_SINGLETON_STORAGE(XZDD::CGameLogic);
namespace XZDD
{
	//静态变量
	bool		CChiHuRight::m_bInit = false;
	dword		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

	//构造函数
	CChiHuRight::CChiHuRight()
	{
		zeromemory( m_dwRight,sizeof(m_dwRight) );

		if( !m_bInit )
		{
			m_bInit = true;
			for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
			{
				if( 0 == i )
					m_dwRightMask[i] = 0;
				else
					m_dwRightMask[i] = (dword(pow((float)2,(float)(i-1))))<<28;
			}
		}
	}

	//赋值符重载
	CChiHuRight & CChiHuRight::operator = ( dword dwRight )
	{
		dword dwOtherRight = 0;
		//验证权位
		if( !IsValidRight( dwRight ) )
		{
			//验证取反权位
			ASSERT( IsValidRight( ~dwRight ) );
			if( !IsValidRight( ~dwRight ) ) return *this;
			dwRight = ~dwRight;
			dwOtherRight = MASK_CHI_HU_RIGHT;
		}

		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
				m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
			else m_dwRight[i] = dwOtherRight;
		}

		return *this;
	}

	//与等于
	CChiHuRight & CChiHuRight::operator &= ( dword dwRight )
	{
		bool bNavigate = false;
		//验证权位
		if( !IsValidRight( dwRight ) )
		{
			//验证取反权位
			ASSERT( IsValidRight( ~dwRight ) );
			if( !IsValidRight( ~dwRight ) ) return *this;
			//调整权位
			dword dwHeadRight = (~dwRight)&0xF0000000;
			dword dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
			dwRight = dwHeadRight|dwTailRight;
			bNavigate = true;
		}

		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
			{
				m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
			}
			else if( !bNavigate )
				m_dwRight[i] = 0;
		}

		return *this;
	}

	//或等于
	CChiHuRight & CChiHuRight::operator |= ( dword dwRight )
	{
		//验证权位
		if( !IsValidRight( dwRight ) ) return *this;

		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
				m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
		}

		return *this;
	}

	//与
	CChiHuRight CChiHuRight::operator & ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//与
	CChiHuRight CChiHuRight::operator & ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//或
	CChiHuRight CChiHuRight::operator | ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//或
	CChiHuRight CChiHuRight::operator | ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//是否权位为空
	bool CChiHuRight::IsEmpty()
	{
		for( BYTE i = 0; i < CountArray(m_dwRight); i++ )
			if( m_dwRight[i] ) return false;
		return true;
	}

	//设置权位为空
	void CChiHuRight::SetEmpty()
	{
		zeromemory( m_dwRight,sizeof(m_dwRight) );
		return;
	}

	//获取权位数值
	BYTE CChiHuRight::GetRightData( dword dwRight[], BYTE cbMaxCount )
	{
		ASSERT( cbMaxCount >= CountArray(m_dwRight) );
		if( cbMaxCount < CountArray(m_dwRight) ) return 0;

		memcpy( dwRight,m_dwRight,sizeof(dword)*CountArray(m_dwRight) );
		return CountArray(m_dwRight);
	}

	//设置权位数值
	bool CChiHuRight::SetRightData( const dword dwRight[], BYTE cbRightCount )
	{
		ASSERT( cbRightCount <= CountArray(m_dwRight) );
		if( cbRightCount > CountArray(m_dwRight) ) return false;

		zeromemory( m_dwRight,sizeof(m_dwRight) );
		memcpy( m_dwRight,dwRight,sizeof(dword)*cbRightCount );

		return true;
	}

	//检查仅位是否正确
	bool CChiHuRight::IsValidRight( dword dwRight )
	{
		dword dwRightHead = dwRight & 0xF0000000;
		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
			if( m_dwRightMask[i] == dwRightHead ) return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	//静态变量

	//扑克数据
	const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	};

	//////////////////////////////////////////////////////////////////////////

	//构造函数
	CGameLogic::CGameLogic()
	{
		m_cbMagicIndex = MAX_INDEX;
	}

	//析构函数
	CGameLogic::~CGameLogic()
	{
	}

	//混乱扑克
	void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
	{
		//混乱准备
		BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
		memcpy(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

		//混乱扑克
		BYTE cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=rand()%(cbMaxCount-cbRandCount);
			cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);

		return;
	}

	//删除扑克
	bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

		//删除扑克
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
		if (cbCardIndex[cbRemoveIndex]>0)
		{
			cbCardIndex[cbRemoveIndex]--;
			return true;
		}

		//失败效验
		ASSERT(FALSE);

		return false;
	}

	//删除扑克
	bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
	{
		//删除扑克
		for (BYTE i=0;i<cbRemoveCount;i++)
		{
			//效验扑克
			ASSERT(IsValidCard(cbRemoveCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

			//删除扑克
			BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
			if (cbCardIndex[cbRemoveIndex]==0)
			{
				//错误断言
				ASSERT(FALSE);

				//还原删除
				for (BYTE j=0;j<i;j++) 
				{
					ASSERT(IsValidCard(cbRemoveCard[j]));
					cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
				}

				return false;
			}
			else 
			{
				//删除扑克
				--cbCardIndex[cbRemoveIndex];
			}
		}

		return true;
	}

	BYTE CGameLogic::RemoveValueCardAll(BYTE cbCardData[], BYTE cbCardCount,BYTE cbRemoveCard)
	{
		BYTE cbCardIndex[MAX_INDEX];			//手中扑克
		SwitchToCardIndex(cbCardData,cbCardCount,cbCardIndex);
		BYTE cbRemoveCardArray[MAX_INDEX];
		BYTE cbRemoveCout = cbCardIndex[SwitchToCardIndex(cbRemoveCard)];
		for (int i = 0;i<cbRemoveCout;i++)
		{
			cbRemoveCardArray[i] = cbRemoveCard;
		}
		RemoveValueCard(cbCardData,cbCardCount,cbRemoveCardArray,cbRemoveCout);
		return cbCardCount - cbRemoveCout;
	}
	bool CGameLogic::RemoveValueCardOne(BYTE cbCardData[], BYTE cbCardCount,BYTE cbRemoveCard)
	{
		BYTE cbRemoveCardArray[MAX_INDEX];
		cbRemoveCardArray[0] = cbRemoveCard;
		return RemoveValueCard(cbCardData,cbCardCount,cbRemoveCardArray,1);
	}
	//删除扑克
	bool CGameLogic::RemoveValueCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
	{
		//检验数据
		ASSERT(cbCardCount<=14);
		ASSERT(cbRemoveCount<=cbCardCount);

		//定义变量
		BYTE cbDeleteCount=0,cbTempCardData[14];
		if (cbCardCount>CountArray(cbTempCardData))
			return false;
		memcpy(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

		//置零扑克
		for (BYTE i=0;i<cbRemoveCount;i++)
		{
			for (BYTE j=0;j<cbCardCount;j++)
			{
				if (cbRemoveCard[i]==cbTempCardData[j])
				{
					cbDeleteCount++;
					cbTempCardData[j]=0;
					break;
				}
			}
		}

		//成功判断
		if (cbDeleteCount!=cbRemoveCount) 
		{
			ASSERT(FALSE);
			return false;
		}

		//清理扑克
		BYTE cbCardPos=0;
		for (BYTE i=0;i<cbCardCount;i++)
		{
			if (cbTempCardData[i]!=0) 
				cbCardData[cbCardPos++]=cbTempCardData[i];
		}

		return true;
	}

	bool CGameLogic::AddCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbAddCard[], BYTE cbAddCount)
	{

		//添加扑克
		for (BYTE i=0;i<cbAddCount;i++)
		{
			//效验扑克
			ASSERT(IsValidCard(cbAddCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbAddCard[i])]<4);

			//删除扑克
			BYTE cbAddIndex=SwitchToCardIndex(cbAddCard[i]);
			if (cbCardIndex[cbAddIndex]<4)
			{
				//删除扑克
				++cbCardIndex[cbAddIndex];
			}
			else 
			{
				//错误断言
				ASSERT(FALSE);
				//还原添加
				for (BYTE j=0;j<i;j++) 
				{
					ASSERT(IsValidCard(cbAddCard[j]));
					cbCardIndex[SwitchToCardIndex(cbAddCard[j])]--;
				}
				return false;
			}
		}
		return true;
	}

	bool CGameLogic::ReplaceCardData(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbAddCardData[], BYTE cbAddCount)
	{
		BYTE cbCardPos=cbCardCount-cbAddCount;
		for (BYTE i=0;i<cbAddCount;i++)
		{
			cbCardData[cbCardPos]=cbAddCardData[i];
			cbCardPos++;
		}
		return true;
	}

	//有效判断
	bool CGameLogic::IsValidCard(BYTE cbCardData)
	{
		BYTE cbValue=(cbCardData&MASK_VALUE);
		BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
		return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
	}

	//扑克数目
	BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
	{
		//数目统计
		BYTE cbCardCount=0;
		for (BYTE i=0;i<MAX_INDEX;i++) 
			cbCardCount+=cbCardIndex[i];

		return cbCardCount;
	}

	//获取组合
	BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
	{
		//组合扑克
		switch (cbWeaveKind)
		{
		case WIK_LEFT:		//上牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard+1;
				cbCardBuffer[2]=cbCenterCard+2;

				return 3;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard-2;

				return 3;
			}
		case WIK_CENTER:	//上牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard+1;

				return 3;
			}
		case WIK_PENG:		//碰牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard;
				cbCardBuffer[2]=cbCenterCard;

				return 3;
			}
		case WIK_GANG:		//杠牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard;
				cbCardBuffer[2]=cbCenterCard;
				cbCardBuffer[3]=cbCenterCard;

				return 4;
			}
		default:
			{
				ASSERT(FALSE);
			}
		}

		return 0;
	}

	//动作等级
	BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
	{
		//胡牌等级
		if (cbUserAction&WIK_CHI_HU) { return 4; }

		//杠牌等级
		if (cbUserAction&WIK_GANG) { return 3; }

		//碰牌等级
		if (cbUserAction&WIK_PENG) { return 2; }

		//上牌等级
		if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

		return 0;
	}

	//胡牌等级
	WORD CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
	{
		WORD wFanShu = 0;
		return wFanShu;
	}

	//吃牌判断
	BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
	{
		//参数效验
		ASSERT(IsValidCard(cbCurrentCard));

		//过滤判断
		if ( cbCurrentCard>=0x31 || IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//变量定义
		BYTE cbExcursion[3]={0,1,2};
		BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

		//吃牌判断
		BYTE cbEatKind=0,cbFirstIndex=0;
		BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
		for (BYTE i=0;i<CountArray(cbItemKind);i++)
		{
			BYTE cbValueIndex=cbCurrentIndex%9;
			if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
			{
				//吃牌判断
				cbFirstIndex=cbCurrentIndex-cbExcursion[i];

				//吃牌不能包含有王霸
				if( m_cbMagicIndex != MAX_INDEX &&
					m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;

				if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
					continue;

				//设置类型
				cbEatKind|=cbItemKind[i];
			}
		}

		return cbEatKind;
	}

	//碰牌判断
	BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
	{
		//参数效验
		ASSERT(IsValidCard(cbCurrentCard));

		//过滤判断
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//碰牌判断
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
	}

	//杠牌判断
	BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
	{
		//参数效验
		ASSERT(IsValidCard(cbCurrentCard));

		//过滤判断
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//杠牌判断
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
	}

	//杠牌分析
	BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
	{
		//设置变量
		BYTE cbActionMask=WIK_NULL;
		zeromemory(&GangCardResult,sizeof(GangCardResult));

		//手上杠牌
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]==4)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
			}
		}

		//组合杠牌
		for (BYTE i=0;i<cbWeaveCount;i++)
		{
			if (WeaveItem[i].cbWeaveKind==WIK_PENG)
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
				{
					cbActionMask|=WIK_GANG;
					GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
				}
			}
		}

		return cbActionMask;
	}
	 
	BYTE CGameLogic::AnalyseGangCard(const  BYTE cbCardIndex[MAX_INDEX], const XZDD_CMD_WeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult )
	{
		//设置变量
		BYTE cbActionMask=WIK_NULL;
		zeromemory(&GangCardResult,sizeof(GangCardResult));

		//手上杠牌
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]==4)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
			}
		}

		//组合杠牌
		for (BYTE i=0;i<cbWeaveCount;i++)
		{
			if (WeaveItem[i].cbWeaveKind==WIK_PENG)
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
				{
					cbActionMask|=WIK_GANG;
					GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
				}
			}
		}

		return cbActionMask;
	}


	//扑克转换
	BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
	{
		ASSERT(cbCardIndex<34);
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	}

	//扑克转换
	BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
	{
		ASSERT(IsValidCard(cbCardData));
		return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
	}

	//扑克转换
	BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
	{
		//转换扑克
		BYTE cbPosition=0;
		//钻牌
		if( m_cbMagicIndex != MAX_INDEX )
		{
			for( BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
				cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
		}
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]!=0)
			{
				for (BYTE j=0;j<cbCardIndex[i];j++)
				{
					ASSERT(cbPosition<MAX_COUNT);
					cbCardData[cbPosition++]=SwitchToCardData(i);
				}
			}
		}

		return cbPosition;
	}

	//扑克转换
	BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
	{
		//设置变量
		zeromemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

		//转换扑克
		for (BYTE i=0;i<cbCardCount;i++)
		{
			ASSERT(IsValidCard(cbCardData[i]));
			cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
		}

		return cbCardCount;
	}

	//钻牌
	bool CGameLogic::IsMagicCard( BYTE cbCardData )
	{
		if( m_cbMagicIndex != MAX_INDEX )
			return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
		return false;
	}

	//排序,根据牌值排序
	bool CGameLogic::SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount )
	{
		//数目过虑
		if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

		//排序操作
		bool bSorted=true;
		BYTE cbSwitchData=0,cbLast=cbCardCount-1;
		do
		{
			bSorted=true;
			for (BYTE i=0;i<cbLast;i++)
			{
				if (cbCardData[i]>cbCardData[i+1])
				{
					//设置标志
					bSorted=false;

					//扑克数据
					cbSwitchData=cbCardData[i];
					cbCardData[i]=cbCardData[i+1];
					cbCardData[i+1]=cbSwitchData;
				}	
			}
			cbLast--;
		} while(bSorted==false);

		return true;
	}

	void CGameLogic::GetCardWithColor(BYTE cbCardData[MAX_COUNT],BYTE cbCardCount,std::vector<BYTE>& CardList1,std::vector<BYTE>& CardList2,std::vector<BYTE>& CardList3)
	{
		//数目过虑
		if (cbCardCount==0||cbCardCount>MAX_COUNT) return;
		for(int i=0;i<cbCardCount;i++)
		{
			BYTE cbColor=(((BYTE)cbCardData[i]&MASK_COLOR)>>4)+1;
			if (cbColor== 1 )
			{
				CardList1.push_back(cbCardData[i]);
			}
			else if (cbColor == 2 )
			{
				CardList2.push_back(cbCardData[i]);
			}
			else if (cbColor == 3 )
			{
				CardList3.push_back(cbCardData[i]);
			}
		}
	}

	/*
	// 胡法分析函数
	*/

	//大对子
	bool CGameLogic::IsPengPeng( const tagAnalyseItem *pAnalyseItem )
	{
		for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
				return false;
		}
		return true;
	}

	//清一色牌
	bool CGameLogic::IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard)
	{
		//胡牌判断
		BYTE cbCardColor=0xFF;

		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if(i==m_cbMagicIndex) continue;
			if (cbCardIndex[i]!=0)
			{
				//花色判断
				if (cbCardColor!=0xFF)
					return false;

				//设置花色
				cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

				//设置索引
				i=(i/9+1)*9-1;
			}
		}

		//如果手上只有王霸
		if( cbCardColor == 0xFF )
		{
			ASSERT( m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0 );
			//检查组合
			ASSERT( cbItemCount > 0 );
			cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
		}

		if((cbCurrentCard&MASK_COLOR)!=cbCardColor && !IsMagicCard(cbCurrentCard) ) return false;

		//组合判断
		for (BYTE i=0;i<cbItemCount;i++)
		{
			BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
			if ((cbCenterCard&MASK_COLOR)!=cbCardColor)	return false;
		}

		return true;
	}

	//七小对牌
	bool CGameLogic::IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard)
	{
		//组合判断
		if (cbWeaveCount!=0) return false;

		//单牌数目
		BYTE cbReplaceCount = 0;

		//临时数据
		BYTE cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//插入数据
		BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[cbCurrentIndex]++;

		//计算单牌
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			BYTE cbCardCount=cbCardIndexTemp[i];

			//王牌过滤
			if( i == m_cbMagicIndex ) continue;

			//单牌统计
			if( cbCardCount == 1 || cbCardCount == 3 ) 	cbReplaceCount++;
		}

		//王牌不够
		if( m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
			m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0 )
			return false;

		return true;

	}

	//带幺
	bool CGameLogic::IsDaiYao( const tagAnalyseItem *pAnalyseItem )
	{
		//检查牌眼
		BYTE cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
		if( cbCardValue != 1 && cbCardValue != 9 ) return false;

		for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			{
				BYTE j = 0;
				for(; j < 3; j++ )
				{
					cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
					if( cbCardValue == 1 || cbCardValue == 9 ) break;
				}
				if( j == 3 ) return false;
			}
			else
			{
				cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
				if( cbCardValue != 1 && cbCardValue != 9 ) return false;
			}
		}
		return true;
	}

	//将对
	bool CGameLogic::IsJiangDui( const tagAnalyseItem *pAnalyseItem )
	{
		//是否大对子
		if( !IsPengPeng(pAnalyseItem) ) return false;

		//检查牌眼
		BYTE cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
		if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;

		for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			{
				BYTE j = 0;
				for(; j < 3; j++ )
				{
					cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
					if( cbCardValue == 2 || cbCardValue == 5 || cbCardValue == 8 ) break;
				}
				if( j == 3 ) return false;
			}
			else
			{
				cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
				if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;
			}
		}
		return true;
	}

	//是否花猪
	bool CGameLogic::IsHuaZhu( const BYTE cbCardIndex[], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
	{
		BYTE cbColor[3] = { 0,0,0 };
		for( BYTE i = 0; i < MAX_INDEX; i++ )
		{
			if( cbCardIndex[i] > 0 )
			{
				BYTE cbCardColor = SwitchToCardData(i)&MASK_COLOR;
				cbColor[cbCardColor>>4]++;

				i = (i/9+1)*9-1;
			}
		}
		for( BYTE i = 0; i < cbWeaveCount; i++ )
		{
			BYTE cbCardColor = WeaveItem[i].cbCenterCard&MASK_COLOR;
			cbColor[cbCardColor>>4]++;
		}
		//缺一门就不是花猪
		for( BYTE i = 0; i < CountArray(cbColor); i++ )
			if( cbColor[i] == 0 ) return false;

		return true;
	}

	BYTE CGameLogic::GetCardColor(BYTE cbCardDat)
	{
		ASSERT(IsValidCard(cbCardDat));
		return ((cbCardDat&MASK_COLOR)>>4)+1;
	}

	BYTE CGameLogic::GetCardValue(BYTE cbCardDat)
	{
		ASSERT(IsValidCard(cbCardDat));
		return (cbCardDat&MASK_VALUE);
	}
	void CGameLogic::GetCardInfoList(BYTE* pHandCardData, int nHandCardCount,std::vector<XZDD_Card_Info>& kCardInfoList )
	{
		for (int m=1;m<=CARD_COLOR_TIAO;m++)
		{	
			XZDD_Card_Info kInfo;
			kInfo.nColor = m;
			kInfo.nLen = 0;
			for(int i=0;i<nHandCardCount;i++)
			{
				BYTE cbColor= GetCardColor(pHandCardData[i]);
				if (cbColor== kInfo.nColor )
				{
					kInfo.kCardList.push_back(pHandCardData[i]);
					kInfo.nLen++;
				}
			}
			kCardInfoList.push_back(kInfo);
		}
		std::sort(kCardInfoList.begin(),kCardInfoList.end());
	}
}

//////////////////////////////////////////////////////////////////////////

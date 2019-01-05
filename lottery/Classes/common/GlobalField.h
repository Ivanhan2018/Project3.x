#ifndef GLOBAL_FIELD_HEAD_FILE
#define GLOBAL_FIELD_HEAD_FILE
#pragma once
#include "GlobalDef.h"
//////////////////////////////////////////////////////////////////////////

//????√????∑
/*struct tagDataDescribe
{
	WORD							wDataSize;						//????￥?–°
	WORD							wDataDescribe;					//????√???
    tagDataDescribe()
    {
        wDataSize = 0;
        wDataDescribe = 0;
    }
};*/

//////////////////////////////////////////////////////////////////////////

//?¨??–≈?￠
#define DTP_NULL					0								//??–?????

//a?￥°–≈?￠
//a?￥°–≈?￠
#define DTP_USER_ID					1								//”√a? I D
#define DTP_GAME_ID					2								//”√a? I D
#define	DTP_USER_ACCOUNTS			3								//”√a?’à∫≈
#define	DTP_USER_PASS				4								//”√a?√???
#define DTP_USER_GENDER				5								//”√a?–‘±?
#define DTP_USER_FACE				6								//”√a??∑?ò
#define DTP_USER_RIGHT				7								//”√a?????
#define DTP_MASTER_RIGHT			8								//π??ì????
#define	DTP_UNDER_WRITE				9								//∏?–‘??√?
#define	DTP_LOVE_LINESS				10								//”√a?????
#define	DTP_NICK_NAME				11								//”√a?í?≥?
#define	DTP_USER_SORT				12								//≈≈√?
//?￥?¨–≈?￠
#define DTP_USER_STATUS				100								//”√a??￥?¨
#define DTP_USER_TABLE				101								//”??∑??∫≈
#define DTP_USER_CHAIR				102								//“??”∫≈??
#define DTP_USER_PING				103								//?ˉ?á—”?±
#define DTP_USER_INDEX				104								//”√a?à?“?
#define DTP_USER_ROUND				105								//—≠a∑o???
#define DTP_LOOK_ON_USER			106								//≈‘π�?êo“

//a?∑÷–≈?￠
#define DTP_USER_WIN				200								//?§?÷≈???
#define DTP_USER_LOST				201								//?‰?÷≈???
#define DTP_USER_DRAW				202								//∫??÷≈???
#define DTP_USER_FLEE				203								//?”?÷≈???
#define DTP_WIN_RATE				204								//”√a??§??
#define DTP_LOST_RATE				205								//”√a??‰??
#define DTP_DRAW_RATE				206								//”√a?∫???
#define DTP_FLEE_RATE				207								//”√a??”??
#define DTP_USER_TAX				208								//”√a?à∞?’
#define DTP_INSURE_SCORE			209								//￥ê￥￠Ω?±“
#define DTP_GAME_GOLD				210								//”??∑Ω?±“
#define DTP_USER_SCORE				211								//”√a?∑÷??
#define DTP_USER_PLAY_COUNT			212								//???÷≈???
#define DTP_USER_EXPERIENCE			213								//”√a??≠—è
#define DTP_GAME_LEVEL				214								//”??∑μ?o?
#define DTP_COMMUNITY_LEVEL			215								//…á?ˉμ?o?

//??’π–≈?￠
#define DTP_USER_GROUP_ID			300								//…á?≈ I D
#define DTP_USER_GROUP_NAME			301								//…á?≈√??÷
#define DTP_USER_NOTE				302								//”√a?±∏?￠
#define DTP_USER_DESCRIBE			303								//”√a?√???
#define DTP_USER_COMPANION			304								//”√a?π??μ
#define DTP_USER_ZIP_CODE			305								//”√a?”?±? by 20100925 ’≈a?
#define DTP_USER_CITY				306								//≥??––≈?￠ by 20100925 ’≈a?
#define DTP_USER_VISITOR			307								//是否游客
#define DTP_USER_Keepdays			308								// 连登天数 by 20120401 ljl
#define DTP_USER_PlayCount			309								// 今天玩的局数
#define DTP_USER_GetPoint			310								// 今天获得点数
#define DTP_USER_PrizeInfo			311								// 今天领奖情况
#define DTP_USER_TotalCount			312								// 总局数
#define DTP_USER_RelieveCount		313								// 救济

//?μ?≥–≈?￠
#define DTP_COMPUTER_ID				1000							//a˙??–ú?–
#define DTP_STATION_PAGE			1001							//’?μ?“≥√ê
#define DTP_OPTION_BUFFER			1002							//≈‰÷√–≈?￠
#define DTP_GAME_KIND				1003							// 游戏类型

//?μ?≥???￠
#define DTP_MESSAGE_ID				2000							//???￠id
#define DTP_ADDRESSER				2001							//∑￠o??à
#define DTP_TITLE					2002							//±í??
#define DTP_CONTENT					2003							//????
#define DTP_MESSAGE_TIME			2004							//?±o‰
//////////////////////////////////////////////////////////////////////////


union UKeepLogonDays
{
	DWORD dwVaule[3];
	struct
	{
		DWORD	dwTodayScore;
		DWORD	dwTomorrowScore;
		DWORD	canGet:1;
		DWORD	days:31;
	};

	UKeepLogonDays()
	{
		memset(this, 0, sizeof(*this));
	}
};

//????∞?∏?÷˙??
class CSendPacketHelper
{
	//±‰????“?
protected:
	WORD								m_wDataSize;					//????￥?–°
	WORD								m_wBufferSize;					//a∫≥?￥?–°
	BYTE								* m_pcbBuffer;					//a∫≥?÷∏’?

	//∫?????“?
public:
	//ππ‘?∫???
	CSendPacketHelper(void * pcbBuffer, WORD wBufferSize)
	{
		m_wDataSize=0;
		m_wBufferSize=wBufferSize;
		m_pcbBuffer=(BYTE *)pcbBuffer;
	}
	//??ππ∫???
	virtual ~CSendPacketHelper() {}

	//π???∫???
public:
	//???ì????
	void CleanData() { m_wDataSize=0; }
	//aò?°￥?–°
	WORD GetDataSize() { return m_wDataSize; }
	//aò?°a∫≥?
	void * GetDataBuffer() { return m_pcbBuffer; }
	//≤???????
	bool AddPacket(void * pData, WORD wDataSize, WORD wDataType)
	{
		//–?—è￥?–°
		//ASSERT((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)<=m_wBufferSize);
		if ((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)>m_wBufferSize) return false;

		//≤???????
		//ASSERT(m_pcbBuffer!=NULL);
		tagDataDescribe * pDataDescribe=(tagDataDescribe *)(m_pcbBuffer+m_wDataSize);
		pDataDescribe->wDataSize=wDataSize;
		//pDataDescribe->wDataDescribe=wDataType;

		//≤???????
		if (wDataSize>0)
		{
			//ASSERT(pData!=NULL);
			memcpy(pDataDescribe+1,pData,wDataSize);
		}

		//…?÷√????
		m_wDataSize+=sizeof(tagDataDescribe)+wDataSize;
	
		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////

class CRecvPacketHelper
{

protected:
	WORD								m_wDataPos;						
	WORD								m_wDataSize;					
	BYTE								* m_pcbBuffer;				

public:
	CRecvPacketHelper(void* pcbBuffer, WORD wDataSize)
	{
		m_wDataPos=0;
		m_wDataSize=wDataSize;
		m_pcbBuffer=(BYTE *)pcbBuffer;
	}
	
	virtual ~CRecvPacketHelper() {}

	
public:
	void * GetData(tagDataDescribe & DataDescribe)
	{
		if (m_wDataPos>=m_wDataSize) 
		{
			assert(m_wDataPos==m_wDataSize);
			DataDescribe.wDataSize=0;
			//DataDescribe.wDataDescribe=DTP_NULL;
			return NULL;
		}

		assert((m_wDataPos+sizeof(tagDataDescribe))<=m_wDataSize);
		memcpy(&DataDescribe,m_pcbBuffer+m_wDataPos,sizeof(tagDataDescribe));
        
        //这里有问题
        //int a = m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize;
		//assert((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)<=m_wDataSize);

        if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize) > m_wDataSize)
        {
           // CCLOG("－－－－－－－－－－－－－－－－－数据解包异常－－－－－－－－－－－－");
        }
        
        

		if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)>m_wDataSize)
		{
			DataDescribe.wDataSize=0;
			//DataDescribe.wDataDescribe=DTP_NULL;
			return NULL;
		}

		void * pData=NULL;
		if (DataDescribe.wDataSize>0) pData=m_pcbBuffer+m_wDataPos+sizeof(tagDataDescribe);
		m_wDataPos+=sizeof(tagDataDescribe)+DataDescribe.wDataSize;
		return pData;
	};
};

//////////////////////////////////////////////////////////////////////////

#endif
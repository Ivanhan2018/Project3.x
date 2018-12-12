#ifndef GLOBAL_FIELD_HEAD_FILE
#define GLOBAL_FIELD_HEAD_FILE
#pragma once
#include "GlobalDef.h"
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

class CSendPacketHelper
{
protected:
	WORD								m_wDataSize;					
	WORD								m_wBufferSize;					
	BYTE								* m_pcbBuffer;					
public:
	CSendPacketHelper(void * pcbBuffer, WORD wBufferSize)
	{
		m_wDataSize=0;
		m_wBufferSize=wBufferSize;
		m_pcbBuffer=(BYTE *)pcbBuffer;
	}
	virtual ~CSendPacketHelper() {}

public:
	void CleanData() { m_wDataSize=0; }
	WORD GetDataSize() { return m_wDataSize; }
	void * GetDataBuffer() { return m_pcbBuffer; }
	bool AddPacket(void * pData, WORD wDataSize, WORD wDataType)
	{
		if ((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)>m_wBufferSize) return false;

		tagDataDescribe * pDataDescribe=(tagDataDescribe *)(m_pcbBuffer+m_wDataSize);
		pDataDescribe->wDataSize=wDataSize;

		if (wDataSize>0)
		{
			memcpy(pDataDescribe+1,pData,wDataSize);
		}

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
			return NULL;
		}

		assert((m_wDataPos+sizeof(tagDataDescribe))<=m_wDataSize);
		memcpy(&DataDescribe,m_pcbBuffer+m_wDataPos,sizeof(tagDataDescribe));
        
        if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize) > m_wDataSize)
        {
           CCLOG("－－－－－－－－－－－－－－－－－数据解包异常－－－－－－－－－－－－");
        }
        
		if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)>m_wDataSize)
		{
			DataDescribe.wDataSize=0;
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
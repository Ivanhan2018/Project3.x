
#include "GameRule.h"
#include "XDate.h" 
#include "stdio.h"
#include "stdlib.h"
#include <time.h> 

CGameRule::CGameRule(void)
{
	Node::create();
	theApp = EntityMgr::instance()->getDispatch();
	fenDanDuration = 0;
	isStopSell = 0;//不停售
}

CGameRule::~CGameRule(void)
{

}

int CGameRule::GetSecByHMS(int h,int m,int s)
{
	int sec=3600*h+60*m+s;
	return sec;
}

time_t CGameRule::GetMorningTime(time_t t)
{ 
    struct tm * tm=localtime(&t);  
    tm->tm_hour = 0;  
    tm->tm_min = 0;  
    tm->tm_sec = 0;  
    time_t ct0=mktime(tm);
	return ct0;
}

//离下次开奖时间还剩下的时间
long CGameRule::GetKjShjDiff()
{
	time_t current_t;
	//ct = time(NULL); 得到相差的时间
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}

//离下次开奖时间还剩下的时间-字符串描述
string CGameRule::GetKjShjDiffDesc()
{
	if(isStopSell) return "0:00";

	long secDiff = GetKjShjDiff();

	if(secDiff < 0) secDiff = 0;

	int minute = 0;
	int second = 0;
	int hour = 0; //增加小时的显示	
	
	if (secDiff > fenDanDuration)
	{
		secDiff -= fenDanDuration;
	}
	hour = secDiff / 3600;
	secDiff = secDiff % 3600;
	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;
	

	char str[255] = {0};
	if (hour != 0)
	{
		sprintf(str, "%02d:%02d:%02d",hour, minute, second);
	} else
	{
		sprintf(str, "%02d:%02d", minute, second);		
	}

	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CGameRule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetKjShjDiff() > fenDanDuration; 
}

//每期之间间隔时间，标准时间
long CGameRule::GetQiSpan()
{
	return 300;
}

//是否在封单时间内
bool CGameRule::IsFenDanDuration()
{
	return GetKjShjDiff() <= fenDanDuration;
}

//设置初始期号
void CGameRule::SetStartQiHao(int value)
{
	m_nStartQihao = value;
}
//设置初始时间
void CGameRule::SetStartTime(const char* value)
{
	//时间
	tm temp_tm;
	memset(&temp_tm,0,sizeof(tm));
	sscanf(value, "%04d-%02d-%02d %02d:%02d:%02d", &(temp_tm.tm_year), &(temp_tm.tm_mon), &(temp_tm.tm_mday),
		&(temp_tm.tm_hour), &(temp_tm.tm_min), &(temp_tm.tm_sec));
	temp_tm.tm_year -= 1900;
	temp_tm.tm_mon -= 1;
	m_tStartTime = mktime(&temp_tm);
}

CChqSSCRule::CChqSSCRule(void)
{
	fenDanDuration = 35;
}

CChqSSCRule::~CChqSSCRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CChqSSCRule::GetQiSpan()
{
	return 1200;
}

int CChqSSCRule::GetQiShu(int sec)
{
	int qishu = 0;
	if (sec < 1800||sec>=85800) //001期没开奖
	{				
		qishu = 1;
	}
	else if (sec >= 1800 && sec<11400) //001期开奖——009期没开奖
	{				
		long total = sec - 1800;
		qishu = (int)(total / 1200 + 2);
	}
	else if (sec >= 11400 && sec<27000) //010期没开奖
	{				
		qishu = 10;
	}
	else if (sec >= 27000 && sec < 85800) //010期开奖——059期没开奖
	{
		long total = sec - 27000;
		qishu = (int)(total / 1200+11);
	}
	return qishu;
}

int CChqSSCRule::GetKjShj(int qishu)
{
	//前9期,等差数列求通项公式
	if(qishu>=1 && qishu<=9)
	{
	   int iKjShj=1800+1200*(qishu-1);
	   return iKjShj;
	}
	//后50期,等差数列求通项公式
	else if(qishu>=10 && qishu<=59)
	{
	   int iKjShj=27000+1200*(qishu-10);
	   return iKjShj;
	}
	return 1800;
}

//下期期号
string CChqSSCRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct;
	theApp->GetTime(ct);
	struct tm *my_tm = localtime(&ct);
    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	int qishu=GetQiShu(sec);
	//做出调整
	qishu += nDelta;
 
	tm *tmLocal=my_tm;
	if(sec>=85800)//期号算到第二天的第一期
	{
	    time_t ct1=GetMorningTime(ct+86400);
	    tmLocal = localtime(&ct1);
	}
	char temp[64] = {0};
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal);

	char last[64] = {0};
	sprintf(last, "%s%03d", temp, qishu);
	return last;
}

//下期重庆彩开奖时间
time_t CChqSSCRule::GetNextKjShj()
{
	time_t ct;
	theApp->GetTime(ct);
	struct tm *my_tm = localtime(&ct);

    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	
	int qishu=GetQiShu(sec);
    int kjshj=GetKjShj(qishu);
	time_t ct0=GetMorningTime(sec>=85800?ct+86400:ct);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}

//是否可撤单-离开奖时间大于两分钟
bool CChqSSCRule::IsCanCancel(string qihao)
{
	//比下期期号还要早，允许撤单

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetKjShjDiff() > fenDanDuration; 
}

//////////////韩国1.5分彩/////////////////////////////////
CHgydwfcRule::CHgydwfcRule(void)
	: m_t1_start(0)
	, m_t1_end(24)//5
	, m_t2_start(7)
	, m_t2_end(24)	
	, m_t3_start(22)
	, m_t3_end(24)
	, timespan_kj_shj(90)
	, timespan_ye_kj_shj(300)
	,nQihaocha(0)
{
	m_nStartQihao = 1972421;  //
	m_tStartTime=1490025600; //2017-03-21 上午00:00:00

	fenDanDuration = 0;

}
CHgydwfcRule::~CHgydwfcRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CHgydwfcRule::GetQiSpan()
{
	return timespan_kj_shj;
}

//下期期号
string CHgydwfcRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	//当前小时
	int hour = tmLocal->tm_hour;
	//重置时间
	tmLocal->tm_hour = 0;
	tmLocal->tm_min = 0;
	tmLocal->tm_sec = 5;

	time_t t1 = mktime(tmLocal);

	long total = (long)difftime(ct,m_tStartTime);

	int nDays = total / 86400;
	int nQihao = m_nStartQihao + (nDays*960); //960是总期数

	if (hour<m_t1_end)
	{	
		total = (long)difftime(ct, t1);
		nQihao += (total / 90 + 1);
	}
	else if (hour>=m_t1_end&&hour<m_t2_start)  //5点到7点不卖
	{
		tm *tmLocalSec = localtime(&ct);
		tmLocalSec->tm_hour = 7;
		tmLocalSec->tm_min = 0;
		tmLocalSec->tm_sec = 0;

		time_t t2 = mktime(tmLocalSec);

		total = (long)difftime(t2, t1);
		nQihao += ((total / 90) + 1);		
	}
	else
	{
		total = (long)difftime(ct, t1);		
		nQihao += ((total / 90) + 1);	
		nQihao -= 80;	
	}   

	//做出调整	
	string rQh;
	rQh.append(String::createWithFormat("%d",nQihao + nQihaocha)->getCString());
	return rQh;
}

//下期开奖时间
time_t CHgydwfcRule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	//当前小时
	int hour = tmLocal->tm_hour;
	//重置时间
	tmLocal->tm_hour = 0;
	tmLocal->tm_min = 0;
	tmLocal->tm_sec = 5;

	time_t t1 = mktime(tmLocal);

	long total = (long)difftime(ct,t1);

	int nQihao = total / 90 + 1;

	if (hour>=m_t1_end&&hour<m_t2_start)  //5点到7点不卖
	{
		tmLocal->tm_hour = 7;
		tmLocal->tm_min = 0;
		tmLocal->tm_sec = 0;

		t1 = mktime(tmLocal);	
	}
	else
	{		
		t1 += nQihao * timespan_kj_shj;	
	}   

	return t1;
}
void CHgydwfcRule::SetQihaocha(int nQihao)
{
	if(nQihaocha == 0||nQihaocha!=nQihao)
	{
		nQihaocha = nQihao;
		//m_nStartQihao += nQihaocha;		
	}

}

void CHgydwfcRule::SetStartQihao(const char* nQihao,const char* chTime)
{
	if(strlen(nQihao) == 7)
	{
		//开始期号
		stringstream strValue;
		strValue << nQihao;
		strValue>>m_nStartQihao;
		//时间
		tm temp_tm;
		memset(&temp_tm,0,sizeof(tm));
		sscanf(chTime, "%04d-%02d-%02d %02d:%02d:%02d", &(temp_tm.tm_year), &(temp_tm.tm_mon), &(temp_tm.tm_mday),
			&(temp_tm.tm_hour), &(temp_tm.tm_min), &(temp_tm.tm_sec));
		temp_tm.tm_year -= 1900;
		temp_tm.tm_mon -= 1;
		m_tStartTime = mktime(&temp_tm);
	}
}

//是否可撤单-离开奖时间大于两分钟
bool CHgydwfcRule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetKjShjDiff() > fenDanDuration; 
}

//////////////加拿大3.5分彩/////////////////////////////////
char CCanadaSDWFRule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CCanadaSDWFRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CCanadaSDWFRule::CCanadaSDWFRule(void)
	: timespan_kj_shj(24*60*60)
	, timespan_ye_kj_shj(24*60*60)
{
	fenDanDuration = 45;
}

CCanadaSDWFRule::~CCanadaSDWFRule(void)
{

}
//long CCanadaSDWFRule::GetFdShjDiff()
//{
//	if (strlen(m_lastExpect) == 0)
//	{
//		return 0;
//	}
//	time_t current_t;
//	theApp->GetTime(current_t);
//	time_t next_t = GetNextKjShj();
//	double total = difftime(next_t, current_t);
//
//	return (long)total;
//}

////是否可撤单-离开奖时间大于两分钟
//bool CCanadaSDWFRule::IsCanCancel(string qihao)
//{
//	//比下期旗号还要早，允许撤单
//	if (qihao > GetNextExpect())
//		return true;
//
//	if (qihao != GetNextExpect())
//	{
//		return false;
//	}
//
//	return GetFdShjDiff() > fenDanDuration;
//}

//下期期号
string CCanadaSDWFRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct_now;
	theApp->GetTime(ct_now);

	//最后的期号
	int nQihao = atoi(m_lastExpect);
	//LONGLONG nQihao = 0;

	//stringstream strValue;
	//strValue << m_lastExpect;
	//strValue>>nQihao;	

	if(nQihao)
	{
		tm temp_tm;
		memset(&temp_tm,0,sizeof(tm));
		sscanf(m_lastKjShj, "%04d-%02d-%02d %02d:%02d:%02d", &(temp_tm.tm_year), &(temp_tm.tm_mon), &(temp_tm.tm_mday),
			&(temp_tm.tm_hour), &(temp_tm.tm_min), &(temp_tm.tm_sec));

		temp_tm.tm_year -= 1900;
		temp_tm.tm_mon -= 1;
		
		time_t ct_last = mktime(&temp_tm);

		int delta = ct_now - ct_last;

		nQihao += delta / 210 + 1;	
	}
	
	return String::createWithFormat("%07d", nQihao)->getCString();
}

//下期开奖时间
time_t CCanadaSDWFRule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);

	//int nQihao = atoi(m_lastExpect);
	LONGLONG nQihao = 0;

	stringstream strValue;
	strValue << m_lastExpect;
	strValue>>nQihao;

	time_t ct_last = ct_now;

	if(nQihao)
	{	
		tm temp_tm;
		memset(&temp_tm,0,sizeof(tm));
		sscanf(m_lastKjShj, "%04d-%02d-%02d %02d:%02d:%02d", &(temp_tm.tm_year), &(temp_tm.tm_mon), &(temp_tm.tm_mday),
			&(temp_tm.tm_hour), &(temp_tm.tm_min), &(temp_tm.tm_sec));

		temp_tm.tm_year -= 1900;
		temp_tm.tm_mon -= 1;

		//封单时间跟期号无关
		ct_last = mktime(&temp_tm);	
	
		int delta = ct_now - ct_last;
		delta = delta / 210 + 1;	
		ct_last += delta * 210;	

		return ct_last;
	} else
	{
		ct_last = 0;
	}

	return ct_last;
}

///////////////////////天津彩//////////////////////////////////////////////////////////
CJxSSCRule::CJxSSCRule(void)
{
	m_iKjShjFirst=33600;//第一期001期开奖时间为9：20
	m_iKjShjLast=82800;//最后一期042期开奖时间为23：00
	m_qishu=42;//每日开奖42期
	m_timespan=1200;//开奖频率20分钟
	fenDanDuration = 45; //封单时间
}

CJxSSCRule::CJxSSCRule(int iKjShjFirst,int iKjShjLast,int qishu,int timespan,int fdtimespan)
{
	m_iKjShjFirst=iKjShjFirst;
	m_iKjShjLast=iKjShjLast;
	m_qishu=qishu;
	m_timespan=timespan;
	fenDanDuration = fdtimespan;
}

CJxSSCRule::~CJxSSCRule(void)
{

}

//下期期号
string CJxSSCRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct;
	theApp->GetTime(ct);
	struct tm *my_tm = localtime(&ct);
    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	int qishu=GetQiShu(sec);
	////做出调整
	//qishu += nDelta;
 
	tm *tmLocal=my_tm;
	if(sec>=m_iKjShjLast)//期号算到第二天的第一期
	{
	    time_t ct1=GetMorningTime(ct+86400);
	    tmLocal = localtime(&ct1);
	}
	char temp[64] = {0};
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal);

	char last[64] = {0};
	sprintf(last, "%s%03d", temp, qishu);
	return last;
}

time_t CJxSSCRule::GetNextKjShj()
{
	time_t ct;
	theApp->GetTime(ct);
	struct tm *my_tm = localtime(&ct);

    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	
	int qishu=GetQiShu(sec);
    int kjshj=GetKjShj(qishu);
	time_t ct0=GetMorningTime(sec>=m_iKjShjLast?ct+86400:ct);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}

int CJxSSCRule::GetQiShu(int sec)
{
	int qishu = 0;
	if (sec < m_iKjShjFirst||sec>=m_iKjShjLast) //001期没开奖
	{				
		qishu = 1;
	}
	else if (sec >= m_iKjShjFirst && sec < m_iKjShjLast) //001期开奖——m_qishu期没开奖
	{
		long total = sec - m_iKjShjFirst;
		qishu = (int)(total / m_timespan+2);
	}
	return qishu;
}

int CJxSSCRule::GetKjShj(int qishu)
{
	//等差数列求通项公式
	if(qishu>=1 && qishu<=m_qishu)
	{
	   int iKjShj=m_iKjShjFirst+m_timespan*(qishu-1);
	   return iKjShj;
	}
	return m_iKjShjFirst;
}

//----------------------------------------------------------------
CXJSSCRule::CXJSSCRule(void)
{
	m_iKjShjFirst=37200;
	m_iKjShjLast=93600;//第二天的凌晨2时整
	m_qishu=48;
	m_timespan=1200;
	fenDanDuration = 60;//封单时间
	m_gameKind=CZXinJiangSSC;
	strcpy(m_para1,"%s%02d");
}

CXJSSCRule::CXJSSCRule(CaiZhong gameKind,const char *para1,int iKjShjFirst,int iKjShjLast,int qishu,int timespan,int fdtimespan)
{
	m_iKjShjFirst=iKjShjFirst;
	m_iKjShjLast=iKjShjLast;
	m_qishu=qishu;
	m_timespan=timespan;
	fenDanDuration = fdtimespan;
	m_gameKind=gameKind;
	strcpy(m_para1,para1);
}

CXJSSCRule::~CXJSSCRule(void)
{

}

//下期期号;queryGameRet default
string CXJSSCRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct;
	theApp->GetTime(ct);
	struct tm *my_tm = localtime(&ct);
    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	int qishu=GetQiShu(sec);
	////做出调整
	//qishu += nDelta;
 
	tm *tmLocal=my_tm;
	if(sec<m_iKjShjLast-86400)//期号算到前一天的后七期
	{
	    time_t ct1=GetMorningTime(ct-86400);
	    tmLocal = localtime(&ct1);
	}
	char temp[64] = {0};
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal);

	char last[64] = {0};
	sprintf(last, m_para1, temp, qishu);
	return last;
}

time_t CXJSSCRule::GetNextKjShj()
{
	time_t ct;
	theApp->GetTime(ct);
	struct tm *my_tm = localtime(&ct);

    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	
	int qishu=GetQiShu(sec);
    int kjshj=GetKjShj(qishu);
	time_t ct0=GetMorningTime(ct);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}

int CXJSSCRule::GetQiShu(int sec)
{
	int sc=m_iKjShjLast-86400;//时差为2小时
	int sec1=(sec+86400-sc)%86400;//乌鲁木齐时间sec1比北京时间sec晚两个小时
	int qishu = 0;
	if (sec1 < (m_iKjShjFirst-sc)) //001期没开奖
	{				
		qishu = 1;
	}
	else if (sec1 >= (m_iKjShjFirst-sc) && sec1 < (m_iKjShjLast-sc)) //001期开奖——m_qishu期没开奖
	{
		long total = sec1 - (m_iKjShjFirst-sc);
		qishu = (int)(total / m_timespan+2);
	}
	return qishu;
}

int CXJSSCRule::GetKjShj(int qishu)
{
	//等差数列求通项公式
	if(qishu>=1 && qishu<=m_qishu)
	{
	   int iKjShj=m_iKjShjFirst+m_timespan*(qishu-1);//取值范围为[m_iKjShjFirst, m_iKjShjLast]
	   return iKjShj;
	}
	return m_iKjShjFirst;
}
////////////////////////////////////////////

CFenFenCaiRule::CFenFenCaiRule(void)
	:timespan_kj_shj(60)

{
	fenDanDuration = 5;
}

CFenFenCaiRule::~CFenFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CFenFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
//long CFenFenCaiRule::GetFdShjDiff()
//{
//	time_t current_t;
//	//ct = time(NULL);
//	theApp->GetTime(current_t);
//	time_t next_t = GetNextKjShj();
//	double total = difftime(next_t,current_t);
//
//	return (long)total;
//}
//下期期号
string CFenFenCaiRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	int nQiHao =0;

	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);

	nQiHao = tmLocal->tm_hour * 60 + tmLocal->tm_min +1;

	nQiHao+=nDelta;


	char temp[64] = {0}; 
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal); 
	char last[64] = {0};
	sprintf(last, "%s%04ld", temp, nQiHao);
	string strQiHao = string(last);
	return strQiHao;

}
//是否可撤单-离开奖时间大于两分钟
//bool CFenFenCaiRule::IsCanCancel(string qihao)
//{
//	//比下期旗号还要早，允许撤单
//	if(qihao > GetNextExpect())
//		return true;
//
//	if(qihao != GetNextExpect())
//	{
//		return false;
//	}
//
//	return GetFdShjDiff() > fenDanDuration; 
//}

//下期开奖时间
time_t CFenFenCaiRule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	//时分秒清零
	struct tm *my_tm = localtime(&ct);
	my_tm->tm_hour = 0;
	my_tm->tm_min = 0;
	my_tm->tm_sec = 0;
	time_t tmp_ct = mktime(my_tm);
	long total = (long)difftime(ct,tmp_ct);
	int qishu = 0;

	{
		qishu = (int)(total / timespan_kj_shj)+1;
	}

	tmp_ct += qishu * timespan_kj_shj;

	return tmp_ct;
}
////////////////////////////////////////////

char CTentcentCaiRule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CTentcentCaiRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CTentcentCaiRule::CTentcentCaiRule(void)
{
	fenDanDuration = 0;
}

CTentcentCaiRule::~CTentcentCaiRule(void)
{

}

//下期期号
string CTentcentCaiRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	//现在的时间
	time_t ct;
	theApp->GetTime(ct);

	//计算网络获取的值
	//最后的期号
	LONGLONG nQihao = 0;

	stringstream strValue;
	strValue << m_lastExpect;
	strValue>>nQihao;

	int dayNet = 0;
	int dayCount = 0;

	if(nQihao)
	{
		tm temp_tm;
		memset(&temp_tm,0,sizeof(tm));
		sscanf(m_lastKjShj, "%04d-%02d-%02d %02d:%02d:%02d", &(temp_tm.tm_year), &(temp_tm.tm_mon), &(temp_tm.tm_mday),
			&(temp_tm.tm_hour), &(temp_tm.tm_min), &(temp_tm.tm_sec));

		dayNet = temp_tm.tm_mday;		
	} else
	{
		return "0";
	}

	//时分秒清零 计算应该的期号
	struct tm *my_tm = localtime(&ct);

	char temp[64] = {0}; 
	strftime(temp, sizeof(temp), "%Y%m%d",my_tm); 

	dayCount = my_tm->tm_mday;

	my_tm->tm_hour = 0;
	my_tm->tm_min = 0;
	my_tm->tm_sec = -10;
	time_t tmp_ct = mktime(my_tm);
	long total = (long)difftime(ct,tmp_ct);
	int qishu = 0;	

	qishu = (int)(total / 60)+1 + nDelta;

	if(qishu > 1440)
	{
		my_tm->tm_mday ++;
		qishu = 1;
	}

	char last[64] = {0};
	sprintf(last, "%s%04ld", temp, qishu);
	string strQiHao = string(last);

	int delta_qishu = qishu - nQihao % 10000;

	//超过两期
	if((delta_qishu > 562 || delta_qishu < -562) && dayCount != dayNet)
	{
		strQiHao = "0";
	}else
	if((delta_qishu > 2 || delta_qishu < -2) && dayCount == dayNet)
	{
		strQiHao = "0";
	}

	return strQiHao;
}

//下期开奖时间
time_t CTentcentCaiRule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);

	//时分秒清零
	struct tm *my_tm = localtime(&ct);
	my_tm->tm_hour = 0;
	my_tm->tm_min = 0;
	my_tm->tm_sec = -10;
	time_t tmp_ct = mktime(my_tm);
	long total = (long)difftime(ct,tmp_ct);
	int qishu = 0;

	qishu = (int)(total / 60)+1;


	tmp_ct += qishu * 60;

	return tmp_ct;
}
////////////////////////////////////////////
char CQQCaiRule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CQQCaiRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CQQCaiRule::CQQCaiRule(void)
{
	fenDanDuration = 0;
}

CQQCaiRule::~CQQCaiRule(void)
{

}

//下期期号
string CQQCaiRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	//现在的时间
	time_t ct;
	theApp->GetTime(ct);

	//计算网络获取的值
	//最后的期号
	LONGLONG nQihao = 0;

	stringstream strValue;
	strValue << m_lastExpect;
	strValue>>nQihao;

	int dayNet = 0;
	int dayCount = 0;

	if(nQihao)
	{
		tm temp_tm;
		memset(&temp_tm,0,sizeof(tm));
		sscanf(m_lastKjShj, "%04d-%02d-%02d %02d:%02d:%02d", &(temp_tm.tm_year), &(temp_tm.tm_mon), &(temp_tm.tm_mday),
			&(temp_tm.tm_hour), &(temp_tm.tm_min), &(temp_tm.tm_sec));		

		dayNet = temp_tm.tm_mday;	
	} else
	{
		return "0";
	}

	//时分秒清零 计算应该的期号
	struct tm *my_tm = localtime(&ct);

	dayCount = my_tm->tm_mday;

	int qishu = 0;

	if(my_tm->tm_hour == 0 && my_tm->tm_min == 0 && my_tm->tm_sec < 10)
	{
		my_tm->tm_mday --;
		qishu = 1440;
	} else
	{
		my_tm->tm_hour = 0;
		my_tm->tm_min = 0;
		my_tm->tm_sec = 10;
		time_t tmp_ct = mktime(my_tm);
		long total = (long)difftime(ct,tmp_ct);	

		qishu = (int)(total / 60)+1 + nDelta;
	}

	char temp[64] = {0}; 
	strftime(temp, sizeof(temp), "%Y%m%d",my_tm); 
	char last[64] = {0};
	sprintf(last, "%s%04ld", temp, qishu);
	string strQiHao = string(last);

	int delta_qishu = qishu - nQihao % 10000;

	//超过两期
	if((delta_qishu > 562 || delta_qishu < -562) && dayCount != dayNet)
	{
		strQiHao = "0";
	}else
		if((delta_qishu > 2 || delta_qishu < -2) && dayCount == dayNet)
		{
			strQiHao = "0";
		}

	return strQiHao;

}

//下期开奖时间
time_t CQQCaiRule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	//时分秒清零
	struct tm *my_tm = localtime(&ct);
	my_tm->tm_hour = 0;
	my_tm->tm_min = 0;
	my_tm->tm_sec = 10;
	time_t tmp_ct = mktime(my_tm);
	long total = (long)difftime(ct,tmp_ct);
	int qishu = 0;

	{
		qishu = (int)(total / 60)+1;
	}

	tmp_ct += qishu * 60;

	return tmp_ct;
}

CErFenCaiRule::CErFenCaiRule(void)
	: m_t1_start(0)
	, m_t1_end(2)
	, m_t2_start(10)
	, m_t2_end(22)	
	, m_t3_start(22)
	, m_t3_end(24)
	, timespan_kj_shj(120)
	, timespan_ye_kj_shj(300)
{
	fenDanDuration = 15;
}

CErFenCaiRule::~CErFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CErFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
//long CErFenCaiRule::GetFdShjDiff()
//{
//	time_t current_t;
//	//ct = time(NULL);
//	theApp->GetTime(current_t);
//	time_t next_t = GetNextKjShj();
//	double total = difftime(next_t,current_t);
//
//	return (long)total;
//}
//下期期号
string CErFenCaiRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	int nQiHao =0;

	time_t ct;
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);

	nQiHao = tmLocal->tm_hour * 60 + tmLocal->tm_min;

	nQiHao /= 2;

	nQiHao+=nDelta + 1;

	char temp[64] = {0}; 
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal); 
	char last[64] = {0};
	sprintf(last, "%s%03ld", temp, nQiHao);
	string strQiHao = string(last);
	return strQiHao;

}
//是否可撤单-离开奖时间大于两分钟
//bool CErFenCaiRule::IsCanCancel(string qihao)
//{
//	//比下期旗号还要早，允许撤单
//	if(qihao > GetNextExpect())
//		return true;
//
//	if(qihao != GetNextExpect())
//	{
//		return false;
//	}
//
//	return GetFdShjDiff() > fenDanDuration; 
//}

//下期开奖时间
time_t CErFenCaiRule::GetNextKjShj()
{
	time_t ct;
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	//时分秒清零
	struct tm *my_tm = localtime(&ct);
	my_tm->tm_hour = 0;
	my_tm->tm_min = 0;
	my_tm->tm_sec = 0;
	time_t tmp_ct = mktime(my_tm);
	long total = (long)difftime(ct,tmp_ct);
	int qishu = 0;

	{
		qishu = (int)(total / timespan_kj_shj)+1;
	}

	tmp_ct += qishu * timespan_kj_shj;

	return tmp_ct;
}
////////////////////////////////////////////

CWuFenCaiRule::CWuFenCaiRule(void)
	: m_t1_start(0)
	, m_t1_end(2)
	, m_t2_start(10)
	, m_t2_end(22)	
	, m_t3_start(22)
	, m_t3_end(24)
	, timespan_kj_shj(300)
	, timespan_ye_kj_shj(300)
{
	fenDanDuration = 45; //封单时间
}

CWuFenCaiRule::~CWuFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CWuFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
//long CWuFenCaiRule::GetFdShjDiff()
//{
//
//	time_t current_t;
//	//current_t = time(NULL);
//	theApp->GetTime(current_t);
//	time_t next_t = GetNextKjShj();
//	double total = difftime(next_t,current_t);
//
//	return (long)total;
//}
////是否可撤单-离开奖时间大于两分钟
//bool CWuFenCaiRule::IsCanCancel(string qihao)
//{
//	//比下期旗号还要早，允许撤单
//	if(qihao > GetNextExpect())
//		return true;
//
//	if(qihao != GetNextExpect())
//	{
//		return false;
//	}
//
//	return GetFdShjDiff() > fenDanDuration; 
//}

//下期期号
string CWuFenCaiRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);
	int qishu = 0;

	my_tm->tm_hour = 0;
	my_tm->tm_min = 0;
	my_tm->tm_sec = delay_wfc;
	time_t tmp_ct = mktime(my_tm);
	long total = (long)difftime(ct,tmp_ct);
	{
		qishu =(int)(total / timespan_kj_shj) + 1;
	}
	qishu += nDelta;


	char temp[64] = {0}; 
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal); 
	char last[64] = {0};
	sprintf(last, "%s%03d", temp, qishu);
	string rQh = string(last);
	return rQh;
}

//下期开奖时间
time_t CWuFenCaiRule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);

	struct tm *my_tm = localtime(&ct);
	int qishu = 0;

	my_tm->tm_hour = 0;
	my_tm->tm_min = 0;
	my_tm->tm_sec = delay_wfc;
	time_t tmp_ct = mktime(my_tm);
	long total = (long)difftime(ct,tmp_ct);

	{
		qishu = (int)(total / timespan_kj_shj)+1;
	}

	tmp_ct += qishu * timespan_kj_shj;

	return tmp_ct;
}


///////////////////////////////////////////////////
//广东11选5、山东11选5、江西11选5、黑龙江11选5
pair<CaiZhong,CGD11X5Rule::tagStr> pairArray[] = 
{
	make_pair(CZGD11Xuan5,CGD11X5Rule::tagStr()),
	make_pair(CZSD11Xuan5,CGD11X5Rule::tagStr()),
	make_pair(CZJX11Xuan5,CGD11X5Rule::tagStr()),
	make_pair(CZ_HLJ11Xuan5,CGD11X5Rule::tagStr())
};
map<CaiZhong,CGD11X5Rule::tagStr> CGD11X5Rule::m_mapStr(pairArray,pairArray+sizeof(pairArray)/sizeof(pairArray[0]));
CGD11X5Rule::CGD11X5Rule(void)
{
	m_iKjShjFirst=34200;
	m_iKjShjLast=83400;
	m_qishu=42;
	m_timespan=1200;
	fenDanDuration = 60; //封单时间
	m_gameKind=CZGD11Xuan5;
	strcpy(m_para1,"%s%02d");
}

CGD11X5Rule::CGD11X5Rule(CaiZhong gameKind,const char *para1,int iKjShjFirst,int iKjShjLast,int qishu,int timespan,int fdtimespan):CJxSSCRule(iKjShjFirst,iKjShjLast,qishu,timespan,fdtimespan)
{
	m_gameKind=gameKind;
	strcpy(m_para1,para1);
}

CGD11X5Rule::~CGD11X5Rule(void)
{

}

//下期期号
string CGD11X5Rule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	if (strlen(m_mapStr[m_gameKind].m_lastExpect) == 0)
	{
		return "---";
	}

	time_t ct;
	theApp->GetTime(ct);
	struct tm *my_tm = localtime(&ct);
    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	int qishu=GetQiShu(sec);
	////做出调整
	//qishu += nDelta;
 
	tm *tmLocal=my_tm;
	if(sec>=m_iKjShjLast)//期号算到第二天的第一期
	{
	    time_t ct1=GetMorningTime(ct+86400);
	    tmLocal = localtime(&ct1);
	}
	char temp[64] = {0};
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal);

	char last[64] = {0};
	sprintf(last, m_para1, temp, qishu);
	return last;
}

////////////////////////////////////////////
//重庆11选5：已经取消
char CCQ11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CCQ11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CCQ11X5Rule::CCQ11X5Rule(void)
	: timespan_kj_shj(600)
	, timespan_ye_kj_shj(300)
{
	fenDanDuration = 60;
}

CCQ11X5Rule::~CCQ11X5Rule(void)
{

}

//下期期号
string CCQ11X5Rule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	if (strlen(m_lastExpect) == 0)
	{
		return "---";
	}

	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);
	string rQh;
	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=10)||my_tm->tm_hour>=10)&& (my_tm->tm_hour <= 23) ){
		string qihao = CCQ11X5Rule::m_lastExpect;
		string qh1 = qihao.substr(0,8);
		string qh2 = qihao.substr(qihao.length()-2,2);

		int iQh = atoi(qh2.c_str());
		iQh ++;
		//做出调整
		iQh += nDelta;
		char tmp[120] = {0};
		sprintf(tmp, "%s%02d", qh1.c_str(), iQh);
		rQh = string(tmp);
	}
	else {
		if(my_tm->tm_hour >=23 && my_tm->tm_hour <=24)
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		sprintf(temp, "%d%02d%02d01", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
		rQh = string(temp);
	}

	return rQh;
}

//下期开奖时间
time_t CCQ11X5Rule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);
	string rQh;
	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=10)||my_tm->tm_hour>=10) && (my_tm->tm_hour <= 23) ){
		string lastKjShj = m_lastKjShj;
		
		struct tm *temp_tm = localtime(&ct);    
		sscanf(m_lastKjShj,"%04d-%02d-%02d %02d:%02d:%02d",&(temp_tm->tm_year),&(temp_tm->tm_mon),&(temp_tm->tm_mday),
			&(temp_tm->tm_hour),&(temp_tm->tm_min),&(temp_tm->tm_sec));  
		temp_tm->tm_year-=1900;
		temp_tm->tm_mon -= 1;
		time_t ctm = mktime(temp_tm);

		ctm += timespan_kj_shj;
		return ctm;
	}
	else {

		if(my_tm->tm_hour >=23 && my_tm->tm_hour <=24)
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 9;
		tmp_tm->tm_min = 10;
		tmp_tm->tm_sec = 0;
		return mktime(tmp_tm);
	}
}
//long CCQ11X5Rule::GetFdShjDiff()
//{
//	if (strlen(m_lastExpect) == 0)
//	{
//		return 0;
//	}
//
//	time_t current_t;
//	//current_t = time(NULL);
//	theApp->GetTime(current_t);
//	time_t next_t = GetNextKjShj();
//	double total = difftime(next_t,current_t);
//
//	return (long)total;
//}
////是否可撤单-离开奖时间大于两分钟
//bool CCQ11X5Rule::IsCanCancel(string qihao)
//{
//	//比下期旗号还要早，允许撤单
//	if(qihao > GetNextExpect())
//		return true;
//
//	if(qihao != GetNextExpect())
//	{
//		return false;
//	}
//
//	return GetFdShjDiff() > fenDanDuration; 
//}

//北京快车PK10

char CBJPK10Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CBJPK10Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CBJPK10Rule::CBJPK10Rule(void)
	: timespan_kj_shj(300)
	, timespan_ye_kj_shj(300)
{
	fenDanDuration = 30;
    //729920 07040209100508010306 2019-02-23 09:32:33 
#if 0
	tm tm_today;
	tm_today.tm_hour = 9;
	tm_today.tm_min = 30;
	tm_today.tm_sec = 0;
	tm_today.tm_year = 119;
	tm_today.tm_mon = 1;
	tm_today.tm_mday = 23;
	tm_today.tm_wday = 6;
	tm_today.tm_yday = 53;
	tm_today.tm_isdst = 0;
	m_tStartTime=mktime(&tm_today);
#else
    m_tStartTime = 1550885400;
#endif
	m_nStartQihao = 729920;
}

CBJPK10Rule::~CBJPK10Rule(void)
{

}

int CBJPK10Rule::GetQiShu0()
{
	time_t ct;
	theApp->GetTime(ct);
    time_t tStartTime=GetMorningTime(ct)+34200;//今天第一期的开奖时间
	int qishu0 = m_nStartQihao + (tStartTime - m_tStartTime )/ 86400 * 44;
	return qishu0;
}

int CBJPK10Rule::GetQiShu(int sec)
{
	int qishu = 0;
	if (sec < 34200||sec>=85800) //第1期没开奖
	{				
		qishu = 0;
	}
	else if (sec >= 34200 && sec<85800) //第1期开奖——第44期没开奖
	{				
		long total = sec - 34200;
		qishu = (int)(total / 1200+1);
	}
	else //第44期开奖
	{
		qishu = 0;
	}
	return qishu;
}

int CBJPK10Rule::GetKjShj(int qishu)
{
	//等差数列求通项公式
	if(qishu>=1 && qishu<=44)
	{
	   int iKjShj=34200+1200*(qishu-1);
	   return iKjShj;
	}
	return 34200;
}

//下期期号
string CBJPK10Rule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";
	
	//xDate	xD;
	//int iLunarYear=0;
	//int iLunarMon=0;
	//int iLunarDay=0;
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);
	//xD.GetLunarDate(1900+tmLocal->tm_year, tmLocal->tm_mon+1, tmLocal->tm_mday, iLunarYear, iLunarMon, iLunarDay);
	//int nRestDays = (1900+tmLocal->tm_year-iLunarYear)*7;
    int sec=GetSecByHMS(tmLocal->tm_hour,tmLocal->tm_min,tmLocal->tm_sec);
	int qishu=GetQiShu(sec);
	int qishu0 = GetQiShu0();
	int nQiHao = qishu0+qishu;
	//nQiHao -= nRestDays* 44;	//扣除休息日7天
	char sztmp[32];
	sprintf(sztmp, "%ld", nQiHao);
	return string(sztmp);
}

//下期开奖时间
time_t CBJPK10Rule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

    int sec=GetSecByHMS(tmLocal->tm_hour,tmLocal->tm_min,tmLocal->tm_sec);
	
	int qishu=GetQiShu(sec);
    int kjshj=GetKjShj(qishu+1);
	time_t ct0=GetMorningTime(sec>=85800?ct_now+86400:ct_now);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}




//福彩3D
char CFC3DRule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CFC3DRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CFC3DRule::CFC3DRule(void)
	: timespan_kj_shj(24*60*60)
	, timespan_ye_kj_shj(24*60*60)
{
	m_nStartQihao = 2017027;  //开奖期号
	m_tStartTime = 1486126800; //开奖时间

	fenDanDuration = 2400;
}

CFC3DRule::~CFC3DRule(void)
{

}

//下期期号  福彩3d现在封单时间9点  截止时间9.15  下期投注时间9.40
string CFC3DRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	xDate	xD;
	int iLunarYear=0;
	int iLunarMon=0;
	int iLunarDay=0;
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	xD.GetLunarDate(1900+tmLocal->tm_year, tmLocal->tm_mon+1, tmLocal->tm_mday, iLunarYear, iLunarMon, iLunarDay);
	int nRestDays = (iLunarYear - 2017)*7;

	long total = (long)difftime(ct_now, m_tStartTime);

	int nLastExpert = (1900+tmLocal->tm_year)*1000 + tmLocal->tm_yday; 
	//判断已经过了春节
	if(iLunarYear == (1900+tmLocal->tm_year))
	{
		if(iLunarMon == 1 && iLunarDay < 7)
			nLastExpert -= tmLocal->tm_yday;
		else
			nLastExpert -= 7;
	}
	//如果是年末 元旦前的一天，则第二天为新的一年的第一天
	if(tmLocal->tm_mday == 31 && tmLocal->tm_mon == 11 && (tmLocal->tm_hour>=21 && tmLocal->tm_min>10))
	{
		//21.15分过后点半后
		nLastExpert = (tmLocal->tm_year + 1900 +1) * 1000;
	}
	else if(tmLocal->tm_hour >= 22 || (tmLocal->tm_hour==21 && tmLocal->tm_min>10))
	{
		//21:10分以后的订单都算是第二天的
		nLastExpert ++;
	}

	char sztmp[32];
	sprintf(sztmp, "%ld", nLastExpert + 1);	

	return string(sztmp);
}

//下期开奖时间
time_t CFC3DRule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	if ((tmLocal->tm_hour < 21 || (tmLocal->tm_hour == 21 && tmLocal->tm_min < 15)))
	{
		fenDanDuration = 900;

		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday;
		tm_today.tm_hour = 21;
		tm_today.tm_min = 15;
		tm_today.tm_sec = 0;

		time_t t_todayFirst = mktime(&tm_today);		

		return t_todayFirst;
	} else
	if(tmLocal->tm_hour == 21 && tmLocal->tm_min >= 15 && tmLocal->tm_min < 40)
	{
		fenDanDuration = 1500;

		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday;
		tm_today.tm_hour = 21;
		tm_today.tm_min = 40;
		tm_today.tm_sec = 0;

		time_t t_todayFirst = mktime(&tm_today);		

		return t_todayFirst;
	}
	else 
	{
		fenDanDuration = 900;

		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday + 1;		//+1 day
		tm_today.tm_hour = 21;
		tm_today.tm_min = 15;
		tm_today.tm_sec = 0;
		return mktime(&tm_today);
	}

	return mktime(tmLocal);
}

//排列3
char CPL3Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CPL3Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CPL3Rule::CPL3Rule(void)
	: timespan_kj_shj(24*60*60)
	, timespan_ye_kj_shj(24*60*60)
{
	m_nStartQihao = 2017027;  //开奖期号
	m_tStartTime = 1486125000; //开奖时间

	fenDanDuration = 300;
}

CPL3Rule::~CPL3Rule(void)
{

}

//下期期号
string CPL3Rule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	xDate	xD;
	int iLunarYear=0;
	int iLunarMon=0;
	int iLunarDay=0;
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	xD.GetLunarDate(1900+tmLocal->tm_year, tmLocal->tm_mon+1, tmLocal->tm_mday, iLunarYear, iLunarMon, iLunarDay);
	int nRestDays = (iLunarYear - 2017)*7;

	long total = (long)difftime(ct_now, m_tStartTime);

	int nLastExpert = (1900+tmLocal->tm_year)*1000 + tmLocal->tm_yday; 
	//判断已经过了春节
	if(iLunarYear == (1900+tmLocal->tm_year))
	{
		if(iLunarMon == 1 && iLunarDay < 7)
			nLastExpert -= tmLocal->tm_yday;
		else
			nLastExpert -= 7;
	}
	//如果是年末 元旦前的一天
	if(tmLocal->tm_mday == 31 && tmLocal->tm_mon == 11 && (tmLocal->tm_hour>=20 && tmLocal->tm_min>25))
	{
		//20.25分过后点半后
		nLastExpert = (tmLocal->tm_year + 1900 +1) * 1000;
	}
	else	if(tmLocal->tm_hour == 20 && tmLocal->tm_min >= 25 || tmLocal->tm_hour > 20)
	{
		nLastExpert++;
	}

	char sztmp[32];
	sprintf(sztmp, "%ld", nLastExpert + 1);	

	return string(sztmp);

}

//下期开奖时间
time_t CPL3Rule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	if ((tmLocal->tm_hour < 20 || (tmLocal->tm_hour == 20 && tmLocal->tm_min < 30)))
	{
		fenDanDuration = 300;

		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday;
		tm_today.tm_hour = 20;
		tm_today.tm_min = 30;
		tm_today.tm_sec = 0;

		time_t t_todayFirst = mktime(&tm_today);		

		return t_todayFirst;
	}else
	if(tmLocal->tm_hour == 20 && tmLocal->tm_min >= 30 && tmLocal->tm_min < 40)
	{
		fenDanDuration = 600;

		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday;
		tm_today.tm_hour = 20;
		tm_today.tm_min = 40;
		tm_today.tm_sec = 0;

		time_t t_todayFirst = mktime(&tm_today);		

		return t_todayFirst;
	}
	else 
	{
		fenDanDuration = 300;

		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday + 1;		//+1 day
		tm_today.tm_hour = 20;
		tm_today.tm_min = 30;
		tm_today.tm_sec = 0;
		return mktime(&tm_today);
	}

	return mktime(tmLocal);
}

//七星彩
char CQiXingCaiRule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CQiXingCaiRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CQiXingCaiRule::CQiXingCaiRule(void)
	: timespan_kj_shj(24*60*60)
	, timespan_ye_kj_shj(24*60*60)
{
	fenDanDuration = 3600;
}

CQiXingCaiRule::~CQiXingCaiRule(void)
{

}
//long CQiXingCaiRule::GetFdShjDiff()
//{
//	if (strlen(m_lastExpect) == 0)
//	{
//		return 0;
//	}
//	time_t current_t;
//	theApp->GetTime(current_t);
//	time_t next_t = GetNextKjShj();
//	double total = difftime(next_t, current_t);
//
//	return (long)total;
//}
////是否可撤单-离开奖时间大于两分钟
//bool CQiXingCaiRule::IsCanCancel(string qihao)
//{
//	//比下期旗号还要早，允许撤单
//	if (qihao > GetNextExpect())
//		return true;
//
//	if (qihao != GetNextExpect())
//	{
//		return false;
//	}
//
//	return GetFdShjDiff() > fenDanDuration;
//}

//下期期号
string CQiXingCaiRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	//获得当前准确时间  七星彩是星期二，五，日  1，4，6这三天开奖

	//获得星期几
	int weekDay = tmLocal->tm_wday; //0-6  0是星期天
	int deltaDays = 0;
	//计算下次开奖的时间
	while(weekDay != 0 && weekDay != 2 && weekDay != 5)
	{
		deltaDays ++;
		weekDay ++;
		weekDay %= 7;
	}
	//最后期的期号
	int nLastExpert = atoi(m_lastExpect);	

	//检测是不是跨年了
	time_t ct_time = ct_now + 86400 * deltaDays;
	tm *tm_time = localtime(&ct_time);	
	int tempExpert = atoi(String::createWithFormat("%d000",tm_time->tm_year + 1900)->getCString());	

	if(tempExpert > nLastExpert) //取大的
	{
		nLastExpert = tempExpert;
	}
	
	char sztmp[32];
	sprintf(sztmp, "%ld", nLastExpert + 1);
	return string(sztmp);


}

//下期开奖时间
time_t CQiXingCaiRule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	//获得当前准确时间  七星彩是星期二，五，日  1，4，6这三天开奖

	//获得星期几
	int weekDay = tmLocal->tm_wday; //0-6  0是星期天
	int deltaDays = 0;
	//计算下次开奖的时间
	while(weekDay != 0 && weekDay != 2 && weekDay != 5)
	{
		deltaDays ++;
		weekDay ++;
		weekDay %= 7;
	}

	if ((tmLocal->tm_hour < 20 || (tmLocal->tm_hour == 20 && tmLocal->tm_min < 31)))
	{
		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday + deltaDays;
		tm_today.tm_hour = 20;
		tm_today.tm_min = 30;
		tm_today.tm_sec = 0;

		time_t t_todayFirst = mktime(&tm_today);		

		return t_todayFirst;
	}
	else 
	{
		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday + deltaDays + 1;		//+1 day
		tm_today.tm_hour = 20;
		tm_today.tm_min = 30;
		tm_today.tm_sec = 0;
		return mktime(&tm_today);
	}

	return mktime(tmLocal);
}

CKuaiLe8Rule::CKuaiLe8Rule(void)
	: timespan_kj_shj(300)
{
    //937517	0103091012212325273132384145535758686977	2019-02-23 09:05:16 
#if 0
	tm tm_today;
	tm_today.tm_hour = 9;
	tm_today.tm_min = 5;
	tm_today.tm_sec = 0;
	tm_today.tm_year = 119;
	tm_today.tm_mon = 1;
	tm_today.tm_mday = 23;
	tm_today.tm_wday = 6;
	tm_today.tm_yday = 53;
	tm_today.tm_isdst = 0;
	m_tStartTime=mktime(&tm_today);
#else
    m_tStartTime = 1550883900;
#endif
	m_nStartQihao = 937517;

	fenDanDuration = 60;
}

CKuaiLe8Rule::~CKuaiLe8Rule(void)
{
	
}

int CKuaiLe8Rule::GetQiShu0()
{
	time_t ct;
	theApp->GetTime(ct);
    time_t tStartTime=GetMorningTime(ct)+32700;//今天第一期的开奖时间
	int qishu0 = m_nStartQihao + (tStartTime - m_tStartTime )/ 86400 * 179;
	return qishu0;
}

int CKuaiLe8Rule::GetQiShu(int sec)
{
	int qishu = 0;
	if (sec < 32700||sec>=86100) //第1期没开奖
	{				
		qishu = 0;
	}
	else if (sec >= 32700 && sec<86100) //第1期开奖——第179期没开奖
	{				
		long total = sec - 32700;
		qishu = (int)(total / 300+1);
	}
	else //第179期开奖
	{
		qishu = 0;
	}
	return qishu;
}

int CKuaiLe8Rule::GetKjShj(int qishu)
{
	//等差数列求通项公式
	if(qishu>=1 && qishu<=179)
	{
	   int iKjShj=32700+300*(qishu-1);
	   return iKjShj;
	}
	return 32700;
}

//下期期号
string CKuaiLe8Rule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);
    int sec=GetSecByHMS(tmLocal->tm_hour,tmLocal->tm_min,tmLocal->tm_sec);
	int qishu=GetQiShu(sec);
	int qishu0 = GetQiShu0();
	int nQiHao = qishu0+qishu;
	char sztmp[32];
	sprintf(sztmp, "%ld", nQiHao);
	return string(sztmp);
}

//下期开奖时间
time_t CKuaiLe8Rule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);	

	int TodayQihao = 1;
	if ((tmLocal->tm_hour >= 9)&&(tmLocal->tm_hour < 23)  || (tmLocal->tm_hour == 23)&&(tmLocal->tm_min < 55))
	{		
		tm *tmToday = localtime(&ct_now);
		tmToday->tm_hour = 9;
		tmToday->tm_min = 5;
		tmToday->tm_sec = 0;		

		time_t tCha = mktime(tmToday);

		long deltaToday = ct_now - tCha;

		TodayQihao = deltaToday / 300 + 1;

		time_t NextKj = tCha + TodayQihao * 300;		

		return NextKj;
	}
	else 
	{
		time_t ct_today = ct_now;

		if((tmLocal->tm_hour == 23)&&(tmLocal->tm_min > 55)) 
		{
			ct_today += 86400;
		}	
		tm *tmToday = localtime(&ct_today);
		tmToday->tm_hour = 9;
		tmToday->tm_min = 5;
		tmToday->tm_sec = 0;

		time_t NextKj = mktime(tmToday);

		return NextKj; 
	}
}

//六合彩
int LiuHeCaiRule::m_nextExpect = 0;
time_t LiuHeCaiRule::m_nextKjShj = 0;
LiuHeCaiRule::LiuHeCaiRule(void)
{
	fenDanDuration = 0;
}

LiuHeCaiRule::~LiuHeCaiRule(void)
{

}
//long LiuHeCaiRule::GetFdShjDiff()
//{	
//	time_t current_t;
//	theApp->GetTime(current_t);
//	time_t next_t = GetNextKjShj();
//	double total = difftime(next_t, current_t);
//
//	return (long)total;
//}
////是否可撤单-离开奖时间大于两分钟
//bool LiuHeCaiRule::IsCanCancel(string qihao)
//{
//	//比下期旗号还要早，允许撤单
//	if (qihao > GetNextExpect())
//		return true;
//
//	if (qihao != GetNextExpect())
//	{
//		return false;
//	}
//
//	return GetFdShjDiff() > fenDanDuration;
//}

//下期期号
string LiuHeCaiRule::GetNextExpect(int nDelta)
{
	if(getIsStopSell()) return "0";

	std::string result = "";

	result.append(String::createWithFormat("%07d",m_nextExpect + nDelta)->getCString());

	return result;
}

//下期开奖时间
time_t LiuHeCaiRule::GetNextKjShj()
{
	return m_nextKjShj;
}

//设置下期开奖时间
void LiuHeCaiRule::SetNextKjShj(int wYear,int wMonth,int wDay,int wHour,int wMinute,int wSecond,int wMilliseconds)
{
	tm tm_nextKjShij;
	memset(&tm_nextKjShij,0,sizeof(tm));

	if(wYear + wMonth + wDay + wHour + wMinute + wSecond == 0)
	{
		LiuHeCaiRule::m_nextKjShj = 0;
	} else
	{
		tm_nextKjShij.tm_year = wYear - 1900;
		tm_nextKjShij.tm_mon = wMonth - 1;
		tm_nextKjShij.tm_mday = wDay;
		tm_nextKjShij.tm_hour = wHour;
		tm_nextKjShij.tm_min = wMinute;
		tm_nextKjShij.tm_sec = wSecond;	

		LiuHeCaiRule::m_nextKjShj = mktime(&tm_nextKjShij);
	}
}

//CBJ5FenRule
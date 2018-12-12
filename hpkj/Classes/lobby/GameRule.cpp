
#include "GameRule.h"
//#include "time.h" 
#include "stdio.h"
#include "stdlib.h"
#include <time.h> 

CGameRule::CGameRule(void)
{
	Node::create();
	theApp = pMsgDispatch;
}

CGameRule::~CGameRule(void)
{

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
string CGameRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	if(nSecond==10&&secDiff <= nSecond)
	{
		secDiff+=10;
	}
	else if(nSecond == 30)
	{
		if(secDiff <= nSecond)
			secDiff += 30;
	}

	if (secDiff <= nSecond && nSecond != 5 )		//|| secDiff > 540
	{
		return "00:00";
	}

	int minute = 0;
	int second = 0;
	int hour = 0; //增加小时的显示

	if(nSecond==10)
	{
		minute = 0;
		second = secDiff % 60-10;
		if(second < 0)
		{
			second = 0;
		}
	}
	else if(nSecond == 30)
	{
		secDiff-=nSecond;

		minute = secDiff / 60;
		second = secDiff % 60;

	}
	else if (nSecond == 20)
	{
		if (secDiff > nSecond)
		{
			secDiff -= 20;
		}
		minute = secDiff / 60 /*- 1*/;
		second = secDiff % 60;
	}
	else //时间长的一项
	{
		hour = secDiff / 3600;
		minute = (secDiff - hour * 3600) / 60 - 1;
		second = secDiff % 60;
		if(minute < 0) 
		{
			hour = hour - 1;
			minute = 60 + minute;
		}

	}
	char str[255] = {0};
	if (hour != 0)
	{
		std::sprintf(str, "%02d:%02d:%02d",hour, minute, second);
	} else
	{
		std::sprintf(str, "%02d:%02d", minute, second);		
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

	return GetKjShjDiff() > 60; 
}

//每期之间间隔时间，标准时间
long CGameRule::GetQiSpan()
{
	return 300;
}




CChqSSCRule::CChqSSCRule(void)
	: m_t1_start(0)
	, m_t1_end(2)
	, m_t2_start(10)
	, m_t2_end(22)	
	, m_t3_start(22)
	, m_t3_end(24)
	, timespan_kj_shj(600)
	, timespan_ye_kj_shj(300)
{

}

CChqSSCRule::~CChqSSCRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CChqSSCRule::GetQiSpan()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);

	int hour = tmLocal->tm_hour;
	if (hour >= m_t2_start && hour < m_t2_end) //早上10点到晚上10点
		return timespan_kj_shj;
	else
		return timespan_ye_kj_shj;
}

//下期期号
string CChqSSCRule::GetNextExpect(int nDelta)
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);


	int hour = tmLocal->tm_hour;
	int qishu = 0;
	if (hour < m_t1_end) //0 - 2点  86400
	{				
		my_tm->tm_hour = m_t1_start;
		my_tm->tm_min = 0;
		my_tm->tm_sec = delay_chqssc;
		time_t t1;
		t1 = mktime(my_tm);  
		long total = (long)difftime(ct,t1);
		if (total % timespan_ye_kj_shj >= 280)
		{
			qishu = (int)(total / timespan_ye_kj_shj) + 2;
		}
		else
		{
			qishu = (int)(total / timespan_ye_kj_shj + 1);
		}
		if(qishu > 24) qishu = 24;
	}
	else if (hour >= m_t2_start && hour < m_t2_end) //早10点到晚上10点
	{
		my_tm->tm_hour = m_t2_start;
		my_tm->tm_min = 0;
		my_tm->tm_sec = delay_chqssc;
		time_t t1;
		t1 = mktime(my_tm);
		long total = (long)difftime(ct,t1);
		if (total % timespan_kj_shj >= 580)
		{
			qishu = 24 + (int)(total / timespan_kj_shj) + 2;
		}
		else
		{
			qishu = 24 + (int)(total / timespan_kj_shj) + 1;
		}
	}
	else if (hour >= m_t3_start) //22到24点
	{
		my_tm->tm_hour = m_t3_start;
		my_tm->tm_min = 0;
		my_tm->tm_sec = delay_chqssc;
		time_t t1;
		t1 = mktime(my_tm);
		long total = (long)difftime(ct,t1);
		if (total % timespan_ye_kj_shj >= 280)
		{
			qishu = 96 + (int)(total / timespan_ye_kj_shj)+2;
		}
		else
		{
			qishu = 96 + (int)(total / timespan_ye_kj_shj) + 1;
		}
		if (qishu > 120)		//处理期数超过120期
		{
			my_tm->tm_sec = 20;		//later 20 seconds
			t1 = mktime(my_tm);
			total = (long)difftime(ct, t1);
			if (total % timespan_ye_kj_shj >= 280)
			{
				qishu = 96 + (int)(total / timespan_ye_kj_shj) + 2;
			}
			else
			{
				qishu = 96 + (int)(total / timespan_ye_kj_shj) + 1;
			}
		}
	}
	else
	{
		qishu = 24;
	}
	//做出调整
	qishu += nDelta;

	char temp[64] = {0}; 
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal); 
	char last[64] = {0};
	std::sprintf(last, "%s%03d", temp, qishu);
	return last;
}

//下期重庆彩开奖时间
time_t CChqSSCRule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);

	int hour = tmLocal->tm_hour;
	int qishu = 0;
	if (hour < m_t1_end) //
	{		
		my_tm->tm_hour = m_t1_start;
		my_tm->tm_min = 0;
		my_tm->tm_sec = delay_chqssc; 
		time_t t1;
		t1 = mktime(my_tm);
		long total = (long)difftime(ct,t1);
		int qishu = 0;
		if (total % timespan_ye_kj_shj  >= 280)
		{
			qishu = (int)(total / timespan_ye_kj_shj)+2;
		}
		else
		{
			qishu = (int)(total / timespan_ye_kj_shj) + 1;
		}
		
		t1 += qishu * timespan_ye_kj_shj;
		if(qishu == 24)
		{
			my_tm->tm_hour = m_t2_start-1;
			my_tm->tm_min = 50;
			my_tm->tm_sec = delay_chqssc;
			time_t t2;
			t2 = mktime(my_tm);
			long total = (long)difftime(ct,t2);
			t2 += timespan_kj_shj;
			t1 = t2;
		}
		return t1;
	}
	else if (hour >= m_t2_start && hour < m_t2_end)
	{
		my_tm->tm_hour = m_t2_start;
		my_tm->tm_min = 0;
		my_tm->tm_sec = delay_chqssc;
		time_t t1;
		t1 = mktime(my_tm);
		long total = (long)difftime(ct,t1);
		int qishu = 0;
		if (total % timespan_kj_shj >= 580)
		{
			qishu = (int)(total / timespan_kj_shj)+2;
		}
		else
		{
			qishu = (int)(total / timespan_kj_shj) + 1;
		}
		t1 += qishu * timespan_kj_shj;
		return t1;
	}
	else if (hour >= m_t3_start)
	{
		my_tm->tm_hour = m_t3_start;
		my_tm->tm_min = 0;
		my_tm->tm_sec = delay_chqssc; 
		time_t t1;
		t1 = mktime(my_tm);
		long total = (long)difftime(ct,t1);
		int qishu = 0;
		if (total % timespan_ye_kj_shj >= 280)
		{
			qishu = (int)(total / timespan_ye_kj_shj)+2;
		}
		else
		{
			qishu = (int)(total / timespan_ye_kj_shj) + 1;
		}
		t1 += qishu * timespan_ye_kj_shj;		
		return t1;
	}
	else
	{
		my_tm->tm_hour = m_t2_start-1;
		my_tm->tm_min = 50;
		my_tm->tm_sec = delay_chqssc;
		time_t t1;
		t1 = mktime(my_tm);
		long total = (long)difftime(ct,t1);
		t1 += timespan_kj_shj;
		return t1;
	}
}

//是否可撤单-离开奖时间大于两分钟
bool CChqSSCRule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetKjShjDiff() > 20; 
}

/////////////////////////////////////////////////////////////////////////////////
char CJxSSCRule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CJxSSCRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CJxSSCRule::CJxSSCRule(void)
	: timespan_fd_shj(60*9+15)
{

}

CJxSSCRule::~CJxSSCRule(void)
{

}

//下期期号
string CJxSSCRule::GetNextExpect(int nDelta)
{

	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);

	string rQh;
	if (
		((tmLocal->tm_hour == 9 && tmLocal->tm_min >= 10) || tmLocal->tm_hour > 9)
		&& ((tmLocal->tm_hour == 23 && tmLocal->tm_min <= 10) || tmLocal->tm_hour < 23)
		){
			string qihao = CJxSSCRule::m_lastExpect;
			string qh1 = qihao.substr(0,8);
			string qh2 = qihao.substr(qihao.length()-3,3);

			int iQh = atoi(qh2.c_str());
			iQh ++;
			//做出调整
			iQh += nDelta;
			char temp[KJ_QIHAO_LENGTH] = {0};
			std::sprintf(temp, "%s%03d", qh1.c_str(), iQh);
			rQh = string(temp);
	}
	else if (tmLocal->tm_hour >= 23) {
		ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		std::sprintf(temp, "%d%02d%02d001", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
		rQh = string(temp);
	}
	else {
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		std::sprintf(temp, "%d%02d%02d001", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
		rQh = string(temp);
	}

	return rQh;
}

time_t CJxSSCRule::GetNextFdShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	string rQh;
	if (
		((tmLocal->tm_hour == 9 && tmLocal->tm_min >= 10) || tmLocal->tm_hour > 9)
		&& ((tmLocal->tm_hour == 23 && tmLocal->tm_min <= 20) || tmLocal->tm_hour < 23)
		){
			string lastKjShj = m_lastKjShj;
			if(lastKjShj.empty())
				return ct;

			//COleVariant vtime(lastKjShj);
			//vtime.ChangeType(VT_DATE);
			//COleDateTime time4 = vtime;

			//SYSTEMTIME systime;
			//VariantTimeToSystemTime(time4, &systime);
			//time_t ctm(systime);
			
		struct tm *temp_tm = tmLocal;    
		sscanf(m_lastKjShj,"%d-%d-%d %d:%d:%d",&(temp_tm->tm_year),&(temp_tm->tm_mon),&(temp_tm->tm_mday),
			&(temp_tm->tm_hour),&(temp_tm->tm_min),&(temp_tm->tm_sec));  
		temp_tm->tm_year-=1900;
		temp_tm->tm_mon -= 1;
		time_t ctm = mktime(temp_tm);

			ctm += timespan_fd_shj;
			return ctm;
	}
	else if (tmLocal->tm_hour >= 23) {
		ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 9;
		tmp_tm->tm_min = 10;
		tmp_tm->tm_sec = 0;
		time_t t1;
		t1 = mktime(tmp_tm);
		return t1;
	}
	else {
		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 9;
		tmp_tm->tm_min = 10;
		tmp_tm->tm_sec = 0;
		time_t t1;
		t1 = mktime(tmp_tm);
		return t1;
	}
}

long CJxSSCRule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}

	time_t current_t;
	//ct = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextFdShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}

string CJxSSCRule::GetFdShjDiffDesc()
{
	if (strlen(m_lastExpect) == 0)
	{
		return "00:00";
	}

	long secDiff = GetFdShjDiff();
	if (secDiff <= 0 || secDiff > 480)
	{
		return "00:00";
	}

	int minute = secDiff / 60;
	int second = secDiff % 60;

	char temp[120] = {0};
	std::sprintf(temp, "%02d:%02d", minute, second);
	string str = string(temp);
	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CJxSSCRule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}
//----------------------------------------------------------------
char CXJSSCRule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CXJSSCRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CXJSSCRule::CXJSSCRule(void)
	: timespan_fd_shj(600)
{

}

CXJSSCRule::~CXJSSCRule(void)
{

}

//下期期号;queryGameRet default
string CXJSSCRule::GetNextExpect(int nDelta)
{
	if (strlen(m_lastExpect) == 0)
	{
		return "---";
	}

	time_t ct;
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	string rQh;
	if ((tmLocal->tm_hour == 10 && tmLocal->tm_min >= 10) || (tmLocal->tm_hour > 10 && tmLocal->tm_hour <=24))
	{
		tmLocal->tm_hour = 10;
		tmLocal->tm_min = 0;
		tmLocal->tm_sec = delay_xjssc;
		time_t t1;
		t1 = mktime(tmLocal);
		long total = (long)difftime(ct, t1);
		int qishu = 0;
		if (total %timespan_fd_shj >= 540)
		{
			qishu = (int)(total / timespan_fd_shj) + 2;
		}
		else
		{
			qishu = (int)(total / timespan_fd_shj) + 1;
		}

		char temp[64] = { 0 };
		strftime(temp, sizeof(temp), "%Y%m%d", tmLocal);
		char szlast[64] = { 0 };
		std::sprintf(szlast, "%s%02d", temp, qishu);
		return szlast;
	}
	else if (tmLocal->tm_hour >= 0 && tmLocal->tm_hour <= 2)
	{
		tmLocal->tm_hour = 0;
		tmLocal->tm_min = 0;
		tmLocal->tm_sec = delay_xjssc;
		time_t t1;
		t1 = mktime(tmLocal);
		long total = (long)difftime(ct, t1);
		int qishu = 0;
		if (total %timespan_fd_shj >= 540)
		{
			qishu = (int)(total / timespan_fd_shj) + 2;
		}
		else
		{
			qishu = (int)(total / timespan_fd_shj) + 1;
		}

		qishu += 84;

		if(qishu > 144) qishu -= 144;

		char temp[64] = { 0 };
		strftime(temp, sizeof(temp), "%Y%m%d", tmLocal);
		char szlast[64] = { 0 };
		std::sprintf(szlast, "%s%02d", temp, qishu);
		return szlast;
	}
	else 
	{
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		std::sprintf(temp, "%d%02d%02d01", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
		rQh = string(temp);
	}

	return rQh;
}

time_t CXJSSCRule::GetNextFdShj()
{
	time_t ct;
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	string rQh;
	if ((tmLocal->tm_hour == 10 && tmLocal->tm_min >= 10) || (tmLocal->tm_hour > 10 && tmLocal->tm_hour <= 24))
	{
		tmLocal->tm_hour = 10;
		tmLocal->tm_min = 0;
		tmLocal->tm_sec = delay_xjssc; 
		time_t t1;
		t1 = mktime(tmLocal);
		long total = (long)difftime(ct, t1);
		int qishu = 0;
		if (total %timespan_fd_shj >= 540)
		{
			qishu = (int)(total / timespan_fd_shj) + 2;
		}
		else
		{
			qishu = (int)(total / timespan_fd_shj) + 1;
		}
		t1 += qishu * timespan_fd_shj;
		return t1;
	}
	else if ((tmLocal->tm_hour >= 10 && tmLocal->tm_hour <= 24) || (tmLocal->tm_hour >= 0 && tmLocal->tm_hour <= 2))
	{
		tmLocal->tm_hour = 0;
		tmLocal->tm_min = 0;
		tmLocal->tm_sec = delay_xjssc; 
		time_t t1;
		t1 = mktime(tmLocal);
		long total = (long)difftime(ct, t1);
		int qishu = 0;
		if (total %timespan_fd_shj >= 540)
		{
			qishu = (int)(total / timespan_fd_shj) + 2;
		}
		else
		{
			qishu = (int)(total / timespan_fd_shj) + 1;
		}

		//qishu += 84;
		t1 += qishu * timespan_fd_shj;
		return t1;
	}
	else
	{
		tmLocal->tm_hour = 10;
		tmLocal->tm_min = 10;
		tmLocal->tm_sec = delay_xjssc;
		time_t t1;
		t1 = mktime(tmLocal);
		return t1;
	}

}

long CXJSSCRule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}

	time_t current_t;
	//ct = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextFdShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}

string CXJSSCRule::GetFdShjDiffDesc()
{
	if (strlen(m_lastExpect) == 0)
	{
		return "00:00";
	}

	long secDiff = GetFdShjDiff();
	if (secDiff <= 60 || secDiff > 540)
	{
		return "00:00";
	}

	int minute = secDiff / 60 - 1;
	int second = secDiff % 60;

	char temp[120] = {0};
	std::sprintf(temp, "%02d:%02d", minute, second);
	string str = string(temp);

	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CXJSSCRule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

////////////////////////////////////////////

CFenFenCaiRule::CFenFenCaiRule(void)
	: m_t1_start(0)
	, m_t1_end(2)
	, m_t2_start(10)
	, m_t2_end(22)	
	, m_t3_start(22)
	, m_t3_end(24)
	, timespan_kj_shj(60)
	, timespan_ye_kj_shj(300)
{

}

CFenFenCaiRule::~CFenFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CFenFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
long CFenFenCaiRule::GetFdShjDiff()
{
	time_t current_t;
	//ct = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}
//下期期号
string CFenFenCaiRule::GetNextExpect(int nDelta)
{
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
	std::sprintf(last, "%s%04ld", temp, nQiHao);
	string strQiHao = string(last);
	return strQiHao;

}
//是否可撤单-离开奖时间大于两分钟
bool CFenFenCaiRule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 10; 
}

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

}

CWuFenCaiRule::~CWuFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CWuFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
long CWuFenCaiRule::GetFdShjDiff()
{

	time_t current_t;
	//current_t = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}
//是否可撤单-离开奖时间大于两分钟
bool CWuFenCaiRule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 30; 
}

//下期期号
string CWuFenCaiRule::GetNextExpect(int nDelta)
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
		qishu =(int)(total / timespan_kj_shj) + 1;
	}
	qishu += nDelta;


	char temp[64] = {0}; 
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal); 
	char last[64] = {0};
	std::sprintf(last, "%s%03d", temp, qishu);
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
char CGD11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CGD11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CGD11X5Rule::CGD11X5Rule(void)
	: timespan_kj_shj(600)
	, timespan_ye_kj_shj(300)
{

}

CGD11X5Rule::~CGD11X5Rule(void)
{

}

//下期期号
string CGD11X5Rule::GetNextExpect(int nDelta)
{
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
	int nQh = 1;
	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=10)||my_tm->tm_hour > 9) && (my_tm->tm_hour < 23 ||(my_tm->tm_hour == 23&& my_tm->tm_min<1)) ){
				
		struct tm *tm_start = localtime(&ct);    		
		tm_start->tm_hour = 9;
		tm_start->tm_min = 0;
		tm_start->tm_sec = 0;
		time_t ctm = mktime(tm_start);	
	
		long total = (long)difftime(ct,ctm);

		if(total % timespan_kj_shj >= 540)
			nQh = total / timespan_kj_shj + 2;
		else
			nQh = total / timespan_kj_shj + 1;		

		char tmp[120] = {0};
		std::sprintf(tmp, "%d%02d%02d%02d", tmLocal->tm_year + 1900,tmLocal->tm_mon + 1, tmLocal->tm_mday, nQh);
		rQh = string(tmp);
	}
	else {
		if((my_tm->tm_hour == 22&& my_tm->tm_min>55) || my_tm->tm_hour >=23)
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		std::sprintf(temp, "%d%02d%02d01", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
		rQh = string(temp);
	}

	return rQh;
}

//下期开奖时间
time_t CGD11X5Rule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);
	string rQh;
	//if (((my_tm->tm_hour == 9 && my_tm->tm_min>=12)||my_tm->tm_hour > 9) && (my_tm->tm_hour < 22 ||(my_tm->tm_hour == 22&& my_tm->tm_min<55)) ){
	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=10)||my_tm->tm_hour > 9) && (my_tm->tm_hour < 23 ||(my_tm->tm_hour == 23&& my_tm->tm_min<1)) ){

		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 9;
		tmp_tm->tm_min = 0;
		tmp_tm->tm_sec = 0;
		time_t temp_start = mktime(tmp_tm);

		//算出和第一期相差的时间
		long total = (long)difftime(ct, temp_start); 
		int nQihao = 1;

		if (total % timespan_kj_shj >= 540)
		{
			nQihao = total / timespan_kj_shj+2;
		} else
		{
			nQihao = total / timespan_kj_shj+1;
		}

		temp_start += nQihao * timespan_kj_shj;

		return temp_start;

		//string lastKjShj = m_lastKjShj;

		//struct tm *temp_tm = localtime(&ct);    
		//sscanf(m_lastKjShj,"%d-%d-%d %d:%d:%d",&(temp_tm->tm_year),&(temp_tm->tm_mon),&(temp_tm->tm_mday),
		//	&(temp_tm->tm_hour),&(temp_tm->tm_min),&(temp_tm->tm_sec));  
		//temp_tm->tm_year-=1900;
		//temp_tm->tm_mon -= 1;
		//time_t ctm = mktime(temp_tm);
		//ctm -= 90L;

		//ctm += timespan_kj_shj;

		//time_t tmp_ct = mktime(my_tm);
		//long total = (long)difftime(ctm,ct);
		//if(total<0)
		//{
		//	ctm += timespan_kj_shj;
		//}

		//return ctm;
	}
	else {
		if((my_tm->tm_hour == 22&& my_tm->tm_min>55) || my_tm->tm_hour >=23) //加1整天
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 9;
		tmp_tm->tm_min = 10;
		tmp_tm->tm_sec = 0;
		return mktime(tmp_tm);
	}
}
long CGD11X5Rule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}
	time_t current_t;
	//current_t = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}
//是否可撤单-离开奖时间大于两分钟
bool CGD11X5Rule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}


////////////////////////////////////////////

char CCQ11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CCQ11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CCQ11X5Rule::CCQ11X5Rule(void)
	: timespan_kj_shj(600)
	, timespan_ye_kj_shj(300)
{

}

CCQ11X5Rule::~CCQ11X5Rule(void)
{

}

//下期期号
string CCQ11X5Rule::GetNextExpect(int nDelta)
{
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
		std::sprintf(tmp, "%s%02d", qh1.c_str(), iQh);
		rQh = string(tmp);
	}
	else {
		if(my_tm->tm_hour >=23 && my_tm->tm_hour <=24)
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		std::sprintf(temp, "%d%02d%02d01", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
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
		sscanf(m_lastKjShj,"%d-%d-%d %d:%d:%d",&(temp_tm->tm_year),&(temp_tm->tm_mon),&(temp_tm->tm_mday),
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
long CCQ11X5Rule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}

	time_t current_t;
	//current_t = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}
//是否可撤单-离开奖时间大于两分钟
bool CCQ11X5Rule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

////////////////////////////////////////////

char CJX11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CJX11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CJX11X5Rule::CJX11X5Rule(void)
	: timespan_kj_shj(600)
	, timespan_ye_kj_shj(300)
{

}

CJX11X5Rule::~CJX11X5Rule(void)
{

}
long CJX11X5Rule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}
	time_t current_t;
	//current_t = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}
//是否可撤单-离开奖时间大于两分钟
bool CJX11X5Rule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

//下期期号
string CJX11X5Rule::GetNextExpect(int nDelta)
{
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

	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=10)||my_tm->tm_hour>=10) && (my_tm->tm_hour < 22) ){
		string qihao = CJX11X5Rule::m_lastExpect;
		string qh1 = qihao.substr(0,8);
		string qh2 = qihao.substr(qihao.length()-2,2);

		int iQh = atoi(qh2.c_str());
		iQh ++;
		//做出调整
		iQh += nDelta;
		string lastKjShj = m_lastKjShj;

		//COleVariant vtime(lastKjShj);
		//vtime.ChangeType(VT_DATE);
		//COleDateTime time4 = vtime;

		//SYSTEMTIME systime;
		//VariantTimeToSystemTime(time4, &systime);
		//time_t ctm(systime);

		
		struct tm *temp_tm = localtime(&ct);    
		sscanf(m_lastKjShj,"%d-%d-%d %d:%d:%d",&(temp_tm->tm_year),&(temp_tm->tm_mon),&(temp_tm->tm_mday),
			&(temp_tm->tm_hour),&(temp_tm->tm_min),&(temp_tm->tm_sec));  
		temp_tm->tm_year-=1900;
		temp_tm->tm_mon -= 1;
		time_t ctm = mktime(temp_tm);

		ctm -= 197;

		ctm += timespan_kj_shj;

		time_t tmp_ct = mktime(my_tm);
		long lSecond = (long)difftime(ctm,ct);
		if(lSecond<0)
		{
			iQh ++;
		}

		char tmp[120] = {0};
		std::sprintf(tmp, "%s%02d", qh1.c_str(), iQh);
		rQh = string(tmp);
	}
	else {
		if(my_tm->tm_hour >=22 && my_tm->tm_hour <=24)
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		std::sprintf(temp, "%d%02d%02d01", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
		rQh = string(temp);
	}

	return rQh;
}

//下期开奖时间
time_t CJX11X5Rule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);
	string rQh;

	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=10)||my_tm->tm_hour>=10) && (my_tm->tm_hour < 22) ){
		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 8;
		tmp_tm->tm_min = 59;
		tmp_tm->tm_sec = 30;
		time_t temp_start = mktime(tmp_tm);

		//算出和第一期相差的时间
		long total = (long)difftime(ct, temp_start); 
		int nQihao = 1;

		if (total % timespan_kj_shj >= 540)
		{
			nQihao = total / timespan_kj_shj+2;
		} else
		{
			nQihao = total / timespan_kj_shj+1;
		}

		temp_start += nQihao * timespan_kj_shj;

		return temp_start;
	}
	else {
		if(my_tm->tm_hour >=22 && my_tm->tm_hour <=24)
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 9;
		tmp_tm->tm_min = 10;
		tmp_tm->tm_sec = 0;
		return mktime(tmp_tm);
	}
}

////////////////////////////////////////////

char CSD11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CSD11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CSD11X5Rule::CSD11X5Rule(void)
	: timespan_kj_shj(600)
	, timespan_ye_kj_shj(300)
{

}

CSD11X5Rule::~CSD11X5Rule(void)
{

}
long CSD11X5Rule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}

	time_t current_t;
	//current_t = time(NULL);
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t,current_t);

	return (long)total;
}
//是否可撤单-离开奖时间大于两分钟
bool CSD11X5Rule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}
//下期期号
string CSD11X5Rule::GetNextExpect(int nDelta)
{
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
	int nQh = 1;
	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=5)||my_tm->tm_hour>9) && (my_tm->tm_hour < 21 || (my_tm->tm_hour == 21 && my_tm->tm_min<55))){
		struct tm *tm_start = localtime(&ct);    		
		tm_start->tm_hour = 8;
		tm_start->tm_min = 55;
		tm_start->tm_sec = 0;
		time_t ctm = mktime(tm_start);	

		long total = (long)difftime(ct,ctm);

		if(total % timespan_kj_shj >= 540)
			nQh = total / timespan_kj_shj + 2;
		else
			nQh = total / timespan_kj_shj + 1;		

		char tmp[120] = {0};
		std::sprintf(tmp, "%d%02d%02d%02d", tmLocal->tm_year + 1900,tmLocal->tm_mon + 1, tmLocal->tm_mday, nQh);
		rQh = string(tmp);
	}
	else {
		if(my_tm->tm_hour >= 22 ||((my_tm->tm_hour == 21 && my_tm->tm_min>55)))
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		char temp[KJ_QIHAO_LENGTH] = {0};
		std::sprintf(temp, "%d%02d%02d01", tmp_tm->tm_year+1900, tmp_tm->tm_mon+1, tmp_tm->tm_mday);
		rQh = string(temp);
	}

	return rQh;
}

//下期开奖时间
time_t CSD11X5Rule::GetNextKjShj()
{
	time_t ct;
	//ct = time(NULL);
	theApp->GetTime(ct);
	tm *tmLocal = localtime(&ct);
	struct tm *my_tm = localtime(&ct);
	string rQh;
	if (((my_tm->tm_hour == 9 && my_tm->tm_min>=5)||my_tm->tm_hour>9) && (my_tm->tm_hour < 21 || (my_tm->tm_hour == 21 && my_tm->tm_min<55)) ){
	struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 8;
		tmp_tm->tm_min = 55;
		tmp_tm->tm_sec = 0;
		time_t temp_start = mktime(tmp_tm);

		//算出和第一期相差的时间
		long total = (long)difftime(ct, temp_start); 
		int nQihao = 1;

		if (total % timespan_kj_shj >= 540)
		{
			nQihao = total / timespan_kj_shj+2;
		} else
		{
			nQihao = total / timespan_kj_shj+1;
		}

		temp_start += nQihao * timespan_kj_shj;

		return temp_start;
	}
	else {
		if(my_tm->tm_hour >= 22 ||((my_tm->tm_hour == 21 && my_tm->tm_min>55)))
			ct += 86400;
		struct tm *tmp_tm = localtime(&ct);
		tmp_tm->tm_hour = 9;
		tmp_tm->tm_min = 5;
		tmp_tm->tm_sec = 0;
		return mktime(tmp_tm);
	}
}


//北京快车PK10

char CBJPK10Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CBJPK10Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CBJPK10Rule::CBJPK10Rule(void)
	: timespan_kj_shj(300)
	, timespan_ye_kj_shj(300)
{

}

CBJPK10Rule::~CBJPK10Rule(void)
{

}
long CBJPK10Rule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}

	time_t current_t;
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t, current_t);

	return (long)total; 
}
//是否可撤单-离开奖时间大于两分钟
bool CBJPK10Rule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if (qihao > GetNextExpect())
		return true;

	if (qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60;
}
//下期期号
string CBJPK10Rule::GetNextExpect(int nDelta)
{
	tm t_start;
	memset(&t_start, 0, sizeof(tm));
	t_start.tm_year = 2016 - 1900;
	t_start.tm_mon = 3-1;
	t_start.tm_mday = 3;
	t_start.tm_hour = 0;
	t_start.tm_min = 0;
	t_start.tm_sec = 0;

	time_t startTime = mktime(&t_start);
	int startqihao = 540746;
	int nQiHao = 0;

	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	int TodayQihao = 0;
	if ((tmLocal->tm_hour >9) && (tmLocal->tm_hour<23) || (tmLocal->tm_hour == 23) && (tmLocal->tm_min<57) || (tmLocal->tm_hour == 9) && (tmLocal->tm_min >= 7))
	{
		tm tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon =  tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday;
		tm_today.tm_hour = 9;
		tm_today.tm_min = 7;
		tm_today.tm_sec = 35;

		time_t t_todayFirst = mktime(&tm_today);
		
		//先看今天到了多少期
		double totalSecond = difftime(ct_now, t_todayFirst);

		//5分钟=300秒
		if ((int)totalSecond % 300 > 240)
			TodayQihao = totalSecond / 300 + 2;
		else
			TodayQihao = totalSecond / 300 + 1;
		struct tm *temp_tm = localtime(&ct_now);
		sscanf(m_lastKjShj, "%d-%d-%d %d:%d:%d", &(temp_tm->tm_year), &(temp_tm->tm_mon), &(temp_tm->tm_mday),
			&(temp_tm->tm_hour), &(temp_tm->tm_min), &(temp_tm->tm_sec));
		//sscanf(m_lastKjShj, "%ld", startqihao + (ct_now / 24 / 60 / 60) * 179 + TodayQihao);
	}
	else if ((tmLocal->tm_hour == 23) && (tmLocal->tm_min >= 57))
	{
		//TodayQihao += 1;		//秒数?????????
		tmLocal->tm_mday ++;//天数加1
	}
	else
	{
		startqihao -= 1;
	}

	nQiHao = startqihao + (ct_now - startTime )/ 24 / 60 / 60 * 179 + TodayQihao;
	nQiHao += nDelta;
	char sztmp[32];
	std::sprintf(sztmp, "%ld", nQiHao);
	return string(sztmp);
}

//下期开奖时间
time_t CBJPK10Rule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	if ((tmLocal->tm_hour >9) && (tmLocal->tm_hour<23) || (tmLocal->tm_hour == 23) && (tmLocal->tm_min<57) || (tmLocal->tm_hour == 9) && (tmLocal->tm_min >= 7))
	{
		int TodayQihao = 1;

		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday;
		tm_today.tm_hour = 9;
		tm_today.tm_min = 7;
		tm_today.tm_sec = 35;

		time_t t_todayFirst = mktime(&tm_today);
		//先看今天到了多少期
		double totalSecond = difftime(ct_now, t_todayFirst);

		//5分钟=300秒
		if ((int)totalSecond % 300 > 240)
			TodayQihao = (int)(totalSecond/300) +  2;
		else
			TodayQihao = (int)(totalSecond/300) + 1;	

		t_todayFirst += TodayQihao * 300;

		return t_todayFirst;

	}
	else if ((tmLocal->tm_hour<9) || (tmLocal->tm_hour == 9 && tmLocal->tm_min<7))
	{
		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday; //不加1
		tm_today.tm_hour = 9;
		tm_today.tm_min = 6;
		tm_today.tm_sec = 30;
		return mktime(&tm_today);
	}
	else
	{
		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday+1;		//+1 day
		tm_today.tm_hour = 9;
		tm_today.tm_min = 6;
		tm_today.tm_sec = 30;
		return mktime(&tm_today);
	}

	return mktime(tmLocal);
}




//福彩3D和排列3
char CFC3DPL3Rule::m_lastExpect[KJ_QIHAO_LENGTH] = "";
char CFC3DPL3Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = "";

CFC3DPL3Rule::CFC3DPL3Rule(void)
	: timespan_kj_shj(24*60*60)
	, timespan_ye_kj_shj(24*60*60)
{

}

CFC3DPL3Rule::~CFC3DPL3Rule(void)
{

}
long CFC3DPL3Rule::GetFdShjDiff()
{
	if (strlen(m_lastExpect) == 0)
	{
		return 0;
	}
	time_t current_t;
	theApp->GetTime(current_t);
	time_t next_t = GetNextKjShj();
	double total = difftime(next_t, current_t);

	return (long)total;
}
//是否可撤单-离开奖时间大于两分钟
bool CFC3DPL3Rule::IsCanCancel(string qihao)
{
	//比下期旗号还要早，允许撤单
	if (qihao > GetNextExpect())
		return true;

	if (qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 30*60;
}
//下期期号
string CFC3DPL3Rule::GetNextExpect(int nDelta)
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	int nLastExpert = atoi(m_lastExpect);
	if (tmLocal->tm_hour ==20 && tmLocal->tm_hour < 35 )
	{
		char sztmp[32];
		std::sprintf(sztmp, "%ld", nLastExpert + 2);
		return string(sztmp);
	}
	else 
	{
		char sztmp[32];
		std::sprintf(sztmp, "%ld", nLastExpert + 1);
		return string(sztmp);
	}


	return string("");
}

//下期开奖时间
time_t CFC3DPL3Rule::GetNextKjShj()
{
	time_t ct_now;
	theApp->GetTime(ct_now);
	tm *tmLocal = localtime(&ct_now);

	if ((tmLocal->tm_hour < 20 || (tmLocal->tm_hour == 20 && tmLocal->tm_min < 21)))
	{
		tm   tm_today;
		memset(&tm_today,0,sizeof(tm));//增加分配内存操作
		tm_today.tm_year = tmLocal->tm_year;
		tm_today.tm_mon = tmLocal->tm_mon;
		tm_today.tm_mday = tmLocal->tm_mday;
		tm_today.tm_hour = 20;
		tm_today.tm_min = 21;
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
		tm_today.tm_mday = tmLocal->tm_mday + 1;		//+1 day
		tm_today.tm_hour = 20;
		tm_today.tm_min = 21;
		tm_today.tm_sec = 0;
		return mktime(&tm_today);
	}

	return mktime(tmLocal);
}

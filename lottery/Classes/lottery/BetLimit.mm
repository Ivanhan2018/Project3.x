#include "BetLimit.h"
#include "MyNSString.h"
#include "ConfigMgr.h"
#include "comm.h"
#include "EntityMgr.h"

int BetLimit::GetMaxBetFromId(int gameKind,int wanfaId)
{
	int result = 100000;
	int kindid = 0;

	switch (gameKind)
	{
	case CZChongQingSSC: //重庆彩
	case CZXinJiangSSC:
	case CZ_FENFEN_CAI:
	case CZ_WUFEN_CAI:
	case CZ_KOREA_FEN_CAI://韩国1.5
	case CZ_TIANJIN_SSC:	//天津时时彩
	case CZ_BEIJING_5FEN_CAI: //北京5分彩		
	case CZ_CANADA_FEN_CAI: //加拿大3.5分彩
	case CZ_JILI_2FEN_CAI://二分彩
	case CZ_TENCENT_FEN_CAI:	//腾讯分分彩
	case CZ_QQ_FEN_CAI:	//QQ 分分彩
	case CZ_TENCENT_2FC://腾讯二分彩
	case CZ_QQ_2FEN_CAI://QQ二分彩
		kindid = 0;
		break;
	case CZGD11Xuan5:	//11选5
	case CZSD11Xuan5:
	case CZJX11Xuan5:
	case CZ_HLJ11Xuan5:
		kindid = 1;
		break;
	case CZ_BJPK10:	//PK10
		kindid = 2;
		break;
	case CZ3D:		//福彩
	case CZPailie3:
		kindid = 3;
		break;
	case CZ_SEVENSTAR:
		kindid = 4;
		break;
	case CZ_LUCKY_28: //幸运28
		kindid = 5;
		break;
	case CZKUAILE8://快乐8
		kindid = 6;
		break;
	case CZ_HK6HeCai:
		kindid = 7;
		break;
	default:
		kindid = -1;
		break;
	}

	if(kindid == -1 || kindid > 3) return result;

	vector <int> dataList;

	dataList.clear();

	//玩法的id肯定是从1开始的，不同玩法的注数限制不一样
	switch (wanfaId)
	{
	case 1:
		dataList.push_back(1000);dataList.push_back(55);dataList.push_back(10);dataList.push_back(1000);
		break;
	case 2:		
		dataList.push_back(1000);dataList.push_back(165);dataList.push_back(90);dataList.push_back(100000);
		break;
	case 3:		
		dataList.push_back(1000);dataList.push_back(330);dataList.push_back(10);dataList.push_back(100000);
		break;
	case 4:
		dataList.push_back(1000);dataList.push_back(462);dataList.push_back(720);dataList.push_back(100000);
		break;
	case 5:
		dataList.push_back(100000);dataList.push_back(462);dataList.push_back(10);dataList.push_back(100000);
		break;
	case 6:
		dataList.push_back(100000);dataList.push_back(330);dataList.push_back(5040);dataList.push_back(100000);
		break;
	case 7:
		dataList.push_back(100000);dataList.push_back(165);dataList.push_back(10);dataList.push_back(10);
		break;
	case 8:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(30240);dataList.push_back(100);
		break;
	case 9:
		dataList.push_back(100000);dataList.push_back(110);dataList.push_back(10);dataList.push_back(100000);
		break;
	case 10:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 11:
		dataList.push_back(100000);dataList.push_back(990);dataList.push_back(100000);dataList.push_back(10);
		break;
	case 12:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100);
		break;
	case 13:
		dataList.push_back(100);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 14:
		dataList.push_back(100);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 15:
		dataList.push_back(100);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 16:
		dataList.push_back(100);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 17:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 18:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 19:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 20:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 21:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 22:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 23:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 24:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 25:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 26:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 27:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 28:
		dataList.push_back(225);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 29:
		dataList.push_back(225);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 30:
		dataList.push_back(225);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 31:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 32:
		dataList.push_back(10000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 33:
		dataList.push_back(10000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 34:
		dataList.push_back(1000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 35:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 36:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 37:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 38:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 39:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 40:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 41:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 42:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 43:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 44:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 45:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 46:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 47:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 48:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 49:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 50:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 51:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 52:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 53:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 54:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 55:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 56:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 57:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 58:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 59:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 60:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 61:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;		
	case 62:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 63:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 64:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 65:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 66:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 67:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 68:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 69:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 70:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 71:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 72:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 73:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 74:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 75:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 76:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 77:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 78:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 79:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 80: 
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 81:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 82:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 83:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 84:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 85:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 86:
		dataList.push_back(10000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 87:
		dataList.push_back(1000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 88:
		dataList.push_back(100);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 89:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 90:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 91:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 92:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 93:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 94:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 95:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 96:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 97:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 98:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 99:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	case 100:
		dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);dataList.push_back(100000);
		break;
	default:
		break;
	}

	return dataList.at(kindid) * 4 / 5;
}

int getIdByWanFaId(const int wanFaId, const int type)
{
	int i = 0;
	switch (type)
	{
	case 0://重庆彩		
		for (int var = 0;var < 100;var ++)
		{
			if(wanFaMap[var] == wanFaId) break;
			i++;
		}		
		break;
	case 1://11选5	
		for (int var = 0;var < 100;var ++)
		{
			if(GuangDongwanFaMap[var] == wanFaId) break;
			i++;
		}	
		break;
	case 2:	//3d		
		for (int var = 0;var < 100;var ++)
		{
			if(treeNumbersMap[var] == wanFaId) break;
			i++;
		}		
		break;
	case 3: //pk10	
		for (int var = 0;var < 100;var ++)
		{
			if(PK10WanfaMap[var] == wanFaId) break;
			i++;
		}		
		break;
	case 4:	//七星彩
		for (int var = 0;var < 50;var ++)
		{
			if(tagQiXingCai[var] == wanFaId) break;
			i++;
		}
		break;
	case 5://快8
		for (int var = 0;var < 50;var ++)
		{
			if(tagKuai8[var] == wanFaId) break;
			i++;
		}
		break;
	default:
		break;
	}

	return i;
}

std::string BetLimit::GetWanFaNameFromId(const int gameId,const int wanfaId)
{
	std::string result = "";

	switch (gameId)
	{
	case CZChongQingSSC: //重庆彩
	case CZXinJiangSSC:
	case CZ_FENFEN_CAI:
	case CZ_WUFEN_CAI:
	case CZ_KOREA_FEN_CAI://韩国1.5
	case CZ_TIANJIN_SSC:	//天津时时彩
	case CZ_BEIJING_5FEN_CAI: //北京5分彩		
	case CZ_CANADA_FEN_CAI: //加拿大3.5分彩
	case CZ_JILI_2FEN_CAI://二分彩
	case CZ_TENCENT_FEN_CAI:	//腾讯分分彩
	case CZ_QQ_FEN_CAI:	//QQ 分分彩
	case CZ_TENCENT_2FC://腾讯二分彩
	case CZ_QQ_2FEN_CAI://QQ二分彩
		result = ConfigMgr::instance()->text("display_chongqing.xml", String::createWithFormat("t%d", 20 + wanfaId)->getCString());
		break;
	case CZGD11Xuan5:	//11选5
	case CZSD11Xuan5:
	case CZJX11Xuan5:
	case CZ_HLJ11Xuan5:
		result = ConfigMgr::instance()->text("display_11xuan5.xml", String::createWithFormat("t%d", 20 + wanfaId)->getCString());
		break;
	case CZ_BJPK10:	//PK10
		result = ConfigMgr::instance()->text("display_pk10.xml", String::createWithFormat("t%d", 20 + wanfaId)->getCString());
		break;
	case CZ3D:		//福彩
	case CZPailie3:
		result = ConfigMgr::instance()->text("display_3D.xml", String::createWithFormat("t%d", 20 + wanfaId,2)->getCString());
		break;
	case CZ_SEVENSTAR:
		result = ConfigMgr::instance()->text("display_7xing.xml", String::createWithFormat("t%d", 20 + wanfaId)->getCString());
		break;
	case CZ_LUCKY_28: //幸运28  没有菜单
		result = ConfigMgr::instance()->text("display_DuangDong.xml", String::createWithFormat("t%d", 799 + wanfaId)->getCString());
		break;
	case CZKUAILE8://快乐8		
		result = ConfigMgr::instance()->text("display_kuaile8.xml", String::createWithFormat("t%d", 20 + wanfaId)->getCString());
		break;
	case CZ_HK6HeCai:
		result = ConfigMgr::instance()->text("display_6he.xml", String::createWithFormat("t%d", 20 + wanfaId)->getCString());
		break;
	default:
		break;
	}	

	return  result;
}

bool BetLimit::CheckDingWeiDanStr(int wanfaId,const char* betNumber)
{
	if (24 == wanfaId) //定位胆
	{
		int len = String::create(betNumber)->length();
		int perCount = 0;

		for(int i = 0;i < len;i ++)
		{
			if(perCount > 8) return false;	
			if(betNumber[i] == ','||betNumber[i] == '|') {perCount = 0; continue;}
			perCount ++;
		}
	}

	return true;
}

int getNumStr(std::string &str, const int max)
{
	if(str.find("-") != string::npos) 
	{
		str.clear();
		str.append("01");
		return 0;
	}
	int tempInt = atoi(str.c_str());	

	if(tempInt > max)
	{
		tempInt = atoi(str.substr(0,1).c_str());
		str.clear();
		str.append(String::createWithFormat("%02d", tempInt)->getCString());
		return 1;
	} else
	{
		str.clear();
		str.append(String::createWithFormat("%02d", tempInt)->getCString());
		return 0;
	}
}

std::string BetLimit::AjustDateStr(const char* dateString)
{	
	std::string temp = dateString;
	int start = 0;
	std::string result = temp.substr(start,4);
	start = 4; //当前位置
	result.append("-");
	if(temp.at(start) == '-') start++;
	int len = 1; //日期长度
	if(temp.at(start+1) != '-') len = 2;
	//月
	std::string tempData = temp.substr(start, len);
	int tempLen = getNumStr(tempData, 12);
	result.append(tempData);
	start += len - tempLen;
	//日
	result.append("-");
	if(temp.at(start) == '-') start++;
	tempData = temp.substr(start, 2);
	tempLen = getNumStr(tempData, 31);
	result.append(tempData);	

	return result.c_str();
}

bool BetLimit::CheckDateStr(const char* dateString)
{
	bool result = true;

	int len = String::create(dateString)->length();
	for(int i = 0;i < len;i ++)
	{
		if ((dateString[i] < '0' || dateString[i] > '9') && dateString[i] != '-')
		{
			result = false;
			break;
		}
	}

	return result;
}

//type 0为龙虎和 1为万百千 3为大小单双 4为幸运28 5为北京快8 6为六合彩
const char* getStrFromId(const int id,const int type)
{
	switch (id)
	{
	case 0:		
		return ConfigMgr::instance()->text("display_text.xml","t4400"); //万

	case 1:
		if(type == 3 || type == 4 || type == 6) return ConfigMgr::instance()->text("display_text.xml","t4409"); //小
		return ConfigMgr::instance()->text("display_text.xml","t4401");//千

	case 2:
		if(type == 3 || type == 4 || type == 6) return ConfigMgr::instance()->text("display_text.xml","t4411");//双
		return ConfigMgr::instance()->text("display_text.xml","t4402");//百

	case 3:
		if(type == 3 || type == 4 || type == 6) return ConfigMgr::instance()->text("display_text.xml","t4410");//单
		return ConfigMgr::instance()->text("display_text.xml","t4403");//十

	case 4:
		if(type == 1) return ConfigMgr::instance()->text("display_text.xml","t4404");//个
		return ConfigMgr::instance()->text("display_text.xml","t4406");//虎
		
	case 5:
		return ConfigMgr::instance()->text("display_text.xml","t4407");//和
		
	case 6:
		return ConfigMgr::instance()->text("display_text.xml","t4405");//龙

	case 7:
		return ConfigMgr::instance()->text("display_text.xml","t4420"); //半顺

	case 8:				
		return ConfigMgr::instance()->text("display_text.xml","t4419"); //杂六

	case 9:
		return ConfigMgr::instance()->text("display_text.xml","t4408");//大
		
	case 10:
		return ConfigMgr::instance()->text("display_text.xml","t4415");//小单

	case 11:
		return ConfigMgr::instance()->text("display_text.xml","t4416");//小双

	case 12:
		return ConfigMgr::instance()->text("display_text.xml","t4413");//大单

	case 13:
		return ConfigMgr::instance()->text("display_text.xml","t4414");//大双

	case 14:
		return ConfigMgr::instance()->text("display_text.xml","t4421");//对子

	case 15:
		return ConfigMgr::instance()->text("display_text.xml","t4422");//顺子

	case 16:
		return ConfigMgr::instance()->text("display_text.xml","t4423");//豹子

	case 17:
		return ConfigMgr::instance()->text("display_text.xml","t4418");//极值小,0-5

	case 18:
		return ConfigMgr::instance()->text("display_text.xml","t4417");//极值大：22-27

	case 20:
		return ConfigMgr::instance()->text("display_text.xml","t4424");//上

	case 21:
		return ConfigMgr::instance()->text("display_text.xml","t4425");//中

	case 22:
		return ConfigMgr::instance()->text("display_text.xml","t4426");//下

	case 23:
		return ConfigMgr::instance()->text("display_text.xml","t4427");//奇

	case 24:
		return ConfigMgr::instance()->text("display_text.xml","t4428");//和

	case 25:
		return ConfigMgr::instance()->text("display_text.xml","t4429");//偶

	case 26:
		return ConfigMgr::instance()->text("display_DuangDong.xml","t30");//红波

	case 27:
		return ConfigMgr::instance()->text("display_DuangDong.xml","t31");//蓝波

	case 28:
		return ConfigMgr::instance()->text("display_DuangDong.xml","t32");//绿波
		
	default:
		return ConfigMgr::instance()->text("display_text.xml","t4412"); //无
	} 
}

std::string BetLimit::GetLongHuHeStr(const char* touzhunumber)
{
	std::string tempStr = touzhunumber;
	std::string result = "";

	if(tempStr == "") return result;
	//不判断是否合法

	int tag1 = atoi(tempStr.substr(0,1).c_str());

	//龙虎
	result.append(getStrFromId(tag1, 0));
	result.append(",");
	//万百
	std::string tag2 = tempStr.substr(2,5);	
	int pos = tag2.find_first_of("1",0);
	result.append(getStrFromId(pos, 1));
	pos = tag2.find_last_of("1");
	result.append(getStrFromId(pos, 1));
	result.append("|");

	return result;
}

std::string BetLimit::GetKuai8Str(const char* touzhunumber,const int delta)
{
	std::string result = "";
	std::string tempStr = touzhunumber;

	int posStart = 0;

	for(int posInt = 1;posInt < tempStr.size();posInt ++)
	{		
		if(tempStr.at(posInt) < '0' || tempStr.at(posInt) > '9')
		{	
			int tempInt = atoi(tempStr.substr(posStart,posInt - posStart).c_str());
			result.append(getStrFromId(tempInt + 20 + delta,5));		

			posStart = posInt + 1;
		}
	}

	return result;
}

//大小单双 波色
std::string BetLimit::GetLiuHeCaiStr(const char* touzhunumber, const int delta)
{
	std::string result = "";
	std::string tempStr = touzhunumber;

	int posStart = 0;

	for(int posInt = 1;posInt < tempStr.size();posInt ++)
	{		
		if(tempStr.at(posInt) < '0' || tempStr.at(posInt) > '9')
		{	
			int tempInt = atoi(tempStr.substr(posStart,posInt - posStart).c_str());
			result.append(getStrFromId(tempInt + delta,6));		

			break;
		}
	}

	return result;
}

std::string BetLimit::GetLiuHeShengXiaoStr(const char* touzhunumber)
{
	//找出号码
	std::string result = "";
	std::string tempStr = touzhunumber;

	int number = 0;

	for(int posInt = 1;posInt < tempStr.size();posInt ++)
	{		
		if(tempStr.at(posInt) < '0' || tempStr.at(posInt) > '9')
		{	
			number = atoi(tempStr.substr(0,posInt).c_str());	
			break;
		}
	}
	bool isChunJie = false;
	time_t ct_now;
	EntityMgr::instance()->getDispatch()->GetTime(ct_now);
	if(ct_now > 1485532800) //春节换图
	{
		isChunJie = true;
	}
	//生肖
	for(int i = 0;i < 12;i ++)
	{
		int nowPos = i;
		if(isChunJie)
		{
			nowPos = (nowPos + 11) % 12;
		}
		const char* tempStr = ConfigMgr::instance()->text("liuhecai_config.xml",String::createWithFormat("t%d", nowPos + 1)->getCString());
		if(number == atoi(tempStr))
		{
			result.append(ConfigMgr::instance()->text("liuhecai_config.xml",String::createWithFormat("t%d", i + 14)->getCString()));
			break;
		}
	}

	return result;
}

std::string BetLimit::GetDXDSHStr(const char* touzhunumber)
{
	std::string result = "";
	std::string tempStr = touzhunumber;

	for(int i = 0;i < tempStr.length();i ++)
	{
		if(tempStr.at(i) > '0' && tempStr.at(i) <= '9')
		{
			int temp = tempStr.at(i) - '0';
			result.append(getStrFromId(temp,3));
		} else
		{
			result.append(tempStr.substr(i,1));
		}
	}

	return result;
}

std::string BetLimit::GetGSBQWStr(const char* touzhunumber)
{
	std::string result = "";
	std::string tempStr = touzhunumber;

	//寻找逗号
	int posInt = tempStr.find(",");
	CCLOG("---------------------------------------------------------this , is at position: %d", posInt);
	while(posInt >= 0 && posInt != string::npos)
	{
		result.append(tempStr.substr(0,posInt + 1));

		for(int i = 0;i < 5;i ++)
		{
			if(posInt + i + 1 < tempStr.size()  && tempStr.at(posInt + i + 1) == '1')
			{
				result.append(getStrFromId(i,1));
			}
		}
		
		if(posInt + 5 + 1 +1 > tempStr.size())
			break;
		else
			result.append("|");

		tempStr = tempStr.substr(posInt + 5 + 1 + 1, tempStr.size() - posInt - 5);		
		int posInt = tempStr.find(",");
	}

	return result;
}

std::string BetLimit::GetXingYunStr(const int wanfa,const char* touzhunumber)
{
	if(wanfa < 29) return touzhunumber; //玩法1-28   不需要翻译

	std::string result = "";
	std::string tempStr = touzhunumber;
	//寻找最后一个标志
	int posInt = tempStr.find("|");
	tempStr = tempStr.substr(0,posInt);

	result.append(getStrFromId(atoi(tempStr.c_str()),4));
	result.append("|");

	return result;
}

std::string BetLimit::GetXingYunResultStr(const char* resultStr)
{
	std::string result = " "; 
	std::string tempStr = resultStr;//已经是排好序的了

	if(tempStr.size() < 40) return tempStr;	
	
	int total = 0;
	int totalAll = 0;
	for(int i = 0;i < 20;i ++)
	{		
		total += atoi(tempStr.substr(i * 2,2).c_str());
		
		if((i + 1) % 6 == 0)
		{			
			if(result.size() > 1) result.append(" + ");
			total = total % 10;
			result.append(String::createWithFormat("%d",total)->getCString());
			totalAll += total;
			total = 0;
		}
	}	

	result.append(" = ");
	result.append(String::createWithFormat("%d", totalAll)->getCString());

	return result;
}

std::string BetLimit::GetKuai8ResultStr(const char* resultStr)
{
	std::string result = resultStr;

	if(result.size() < 10) return result;

	for(int i = 1;i <= 5;i ++)
	{
		result.insert(i * 3 - 1,",");
	}

	result = result.substr(0,12).append("...");

	return result;
}

std::string BetLimit::GetNameWithXing(const char* name)
{
	std::string result = name;
	return result.substr(0,3).append("**").c_str();
}

std::string BetLimit::GetBankCardWithXing(const char* name)
{
	std::string result = name;
	int pos1 = result.find_last_not_of(" ") + 1;	
	result.erase(pos1);//去掉空格	
	return result.substr(0,4).append("****").append(result.substr(result.length() - 4 > 0 ? result.length() - 4 : 0));	
}

//判断是不是数字
bool CheckIsNum(const std::string str_num,const int max_num)
{
	bool result = true;
	for(int i = 0;i < str_num.size();i ++)
	{
		if(str_num.at(i) < '0' || str_num.at(i) > '9') 
		{
			result = false;
			break;
		}
	}
	//判断是否超出范围
	if(result)
	{
		int tempInt = atoi(str_num.c_str());
		result = tempInt <= max_num;
	}

	return result;
}

bool CheckIsRepeat(const std::string perStr,const int id,const int numCount,bool isHasSerp)
{
	if (id == 0) return true;
	//默认为失败
	bool result = false;
	int tempSerp = isHasSerp ? 1 : 0;  //有没有逗号隔开 有就要加1
	if (id == 1)
	{
		bool isFind = false; 
		bool isSame = true;
		string count = perStr.substr(0,numCount);
		for(int i = 0;i < perStr.size();i += numCount + tempSerp)
		{
			if(count.compare(perStr.substr(i,numCount)) != 0) isSame = false;
			if(isFind) continue;
			if(perStr.find(perStr.substr(i,numCount)) != perStr.rfind(perStr.substr(i,numCount))) isFind = true;
		}
		//找到相同的，并且不全相同
		return isFind && !isSame;		
	}
	if(id == 2)
	{
		bool isFind = false;
		for (int i = 0;i < perStr.size();i += numCount+tempSerp)
		{
			if(perStr.find(perStr.substr(i,numCount)) != perStr.rfind(perStr.substr(i,numCount))) 
			{
				isFind = true;
				break;
			}
		}
		//找到相同的则返回true
		return !isFind;
	}
	if(id == 3)
	{
		bool isSame = true; string count = perStr.substr(0,numCount);
		for(int i = 0;i < perStr.size();i += numCount + tempSerp)
		{
			if(count.compare(perStr.substr(i,numCount)) != 0)
			{
				isSame = false;
				break;
			}
		}
		//全都相同则返回false
		return !isSame;
	}
	return false;
}

//组三的必须有个重复的，组六则必须不能有重复 isRepeat 0是没有要求，1是必须重复而且只重复一次，2是必须不重复 3是不能全相同
/***
*** number:几个数字 numCount每个数字是多少位 maxNum 每个数字最大值是多少
***/
std::string BetLimit::GetDanShiStr(const char* touzhunumber,const int number,const int isRepeat,const int maxNum,const int numCount,const bool isPK)
{
	if (number == 0) return "0";

	std::string result = "";
	std::string tempStr = touzhunumber;
	std::string perStr = "";	
	tempStr.append("|");
	int count = tempStr.size(); //字符串的count

	//if(count % useNum != 0 && count % useNum  != useNum - 1) return "0";
	//if(count % useNum != 0) tempStr.append("|");	
	//重设大小
	//count = tempStr.size();	

	int tempCount = 0;//实际的count
	perStr.clear();

	for(int i = 0;i < count;)
	{	
		if (tempCount == number)
		{
			if (CheckIsNum(tempStr.substr(i,1), maxNum)) return "0";		
			else i ++; //继续

			bool isHasSerp = !(number == 5 && numCount == 1);

			if(isRepeat == 1 && !CheckIsRepeat(perStr,1,numCount, isHasSerp))
			{
				//return "1";
				continue;
			}else
				if(isRepeat == 2 && !CheckIsRepeat(perStr, 2,numCount, isHasSerp))
				{
					//return "2";
					continue;
				} else
					if(isRepeat == 3 && !CheckIsRepeat(perStr, 3,numCount, isHasSerp))
					{
						//return "3";
						continue;
					}
					result.append(perStr);
					result.append("|");
					perStr.clear();
					tempCount = 0;
					continue;
		}
		//数字则加进来
		if(CheckIsNum(tempStr.substr(i,numCount),maxNum)) 
		{			
			if(tempCount % number != 0 && !(number == 5 && numCount == 1)) perStr.append(",");
			perStr.append(tempStr.substr(i, numCount));	

			tempCount ++;
			i += numCount;		
		} else
		{
			if (tempCount != 0 && !isPK) return "0";
			i++;
		}
	}

	return result;
}

//加逗号
std::string addDouHao(std::string& dealStr)
{
	std::string result = "";

	for(int i = 0;i < dealStr.size();i ++)
	{
		if (i != 0) result.append(",");	
		
		if(dealStr.at(i) != ',')
		result.append(dealStr.substr(i, 1));
	}

	return result;
}

//扩展注数，用于任选单式
std::string ExposeString(const char* segNumbers,const char* tailStr,int needInsert, int beishu)
{
	//初始化
	std::string result = "";
	std::string tail = tailStr;
	//每个位置能给的值
	std::string number = "";
	number.append(",");
	number.append(segNumbers);
	number.append(",");

	int count = number.size() - 2;//最小2  最大4

	//数字数组
	char segResult[6];
	memset(segResult,0,6);

	std::string sega = number.substr(0,2);
	if(tailStr[0] == '0'){ sega.clear();sega.append(",");}
	for(int xa = 0;xa < sega.size();xa ++)//万位
	{
		//第一个数
		segResult[0] = sega.at(xa);

		std::string segb = number.substr(0, 3);
		if(tailStr[1] == '0'){ segb.clear();segb.append(",");}
		for(int xb = 0;xb < segb.size();xb ++)//千位
		{
			//第二个数
			segResult[1] = segb.at(xb);

			std::string segc = number.substr(0, 4);
			if(tailStr[2] == '0'){ segc.clear();segc.append(",");}
			for(int xc = 0;xc < segc.size();xc ++)//百位
			{
				//第三个数
				segResult[2] = segc.at(xc);

				std::string segd = number.substr(count - 1, 3);
				if(tailStr[3] == '0'){ segd.clear();segd.append(",");}
				for(int xd = 0;xd < segd.size();xd ++)//十位
				{
					//第四个数
					segResult[3] = segd.at(xd);

					std::string sege = number.substr(count, 2);
					if(tailStr[4] == '0'){ sege.clear();sege.append(",");}
					for(int xe = 0;xe < sege.size();xe ++)//个位
					{
						//第五个数
						segResult[4] = sege.at(xe);

						int isPassed = 0;int numResult = 0;
						for(int i = 0,j = 0;i < 5;i ++)
						{
							if(segResult[i] != ',') numResult ++;						
							if(segResult[i] == ',') continue;

							if(j < count)
							{
								if(segResult[i] == segNumbers[j])
								{
									isPassed ++;
									j ++;
								}
							}
						}
						if(isPassed == count && count == numResult)
						{
							std::string segnumstr = segResult;
							segnumstr = addDouHao(segnumstr);
							if(result.find(segnumstr) == result.npos)
							{
								result.append(segnumstr);
								result.append("|");
							}							
						}
					}
				}
			}
		}
	}

	return result;
}

std::string BetLimit::GetRenXuanDanShiStr(const char* touzhunumber,const int number,const int isRepeat,const int maxNum,const char* tailStr,int beishu,bool isZhiXuan)
{
	if (number == 0) return "0";
	std::string result = tailStr;
	int myCount = 0;
	for(int counts = 0;counts < result.size();counts ++)
	{
		if(tailStr[counts] == '1') myCount ++;
	}
	//位数小于需要的个数
	if(myCount < number) return "4";

	result.clear();
	std::string tempStr = touzhunumber;
	std::string perStr = "";	
	tempStr.append("|");
	int count = tempStr.size(); //字符串的count

	int tempCount = 0;//实际的count
	perStr.clear();

	for(int i = 0;i < count;)
	{	
		if (tempCount == number)
		{
			if (CheckIsNum(tempStr.substr(i,1), maxNum)) return "0";		
			else i ++; //继续

			if(isRepeat == 1 && !CheckIsRepeat(perStr,1,1,false))
			{
				return "1";
			}else
				if(isRepeat == 2 && !CheckIsRepeat(perStr, 2,1,false))
				{
					return "2";
				} else
					if(isRepeat == 3 && !CheckIsRepeat(perStr, 3,1,false))
					{
						return "3";
					}

			if(!isZhiXuan)
			{
				result.append(perStr);
				result.append(",");
				result.append(tailStr);
				result.append("|");
			} else
			{
				result.append(ExposeString(perStr.c_str(), tailStr, 5 - number,beishu));
				//result.append("|");
			}
			perStr.clear();
			tempCount = 0;
			continue;
		}
		//数字则加进来
		if(CheckIsNum(tempStr.substr(i,1),maxNum)) 
		{		
			//if(tempCount % number != 0 && isZhiXuan) perStr.append(",");
			perStr.append(tempStr.substr(i, 1));	

			tempCount ++;
			i += 1;		
		} else
		{
			if (tempCount != 0) return "0";
			i++;
		}
	}

	return result;
}

int BetLimit::GetWanfaIdFromId(const int wafaId,const int typeId)
{
	int result = wafaId;
	switch (wafaId)
	{
	case 	1:

		break;
	case 	2:
		if(typeId == 0) result = 1;
		//if(typeId == 3) result = 1;
		break;
	case 	3:

		break;
	case 	4:
		if(typeId == 0) result = 3;
		//if(typeId == 3) result = 3;
		break;
	case 	5:

		break;
	case 	6:
		if(typeId == 0) result = 5;
		//if(typeId == 3) result = 5;
		break;
	case 	7:

		break;
	case 	8:
		if(typeId == 0) result = 7;
		break;
	case 	9:
		//if(typeId == 3) result = 8;
		break;
	case 	10:
		if(typeId == 0) result = 9;
		break;
	case 	11:

		break;
	case 	12:
		if(typeId == 0) result = 11;
		break;
	case 	13:
		//if(typeId == 3) result = 12;		
		break;
	case 	14:
		if(typeId == 0) result = 13;
		
		break;
	case 	15:		
		break;
	case 	16:
		if(typeId == 0) result = 15;
		
		break;
	case 	17:		
		break;
	case 	18:
		if(typeId == 0) result = 17;
		
		break;
	case 	19:		
		break;
	case 	20:
		if(typeId == 0) result = 19;		
		
		break;
	case 	21:		
		if(typeId == 1) result = 9;
		
		break;
	case 	22:
		if(typeId == 1) result = 11;
		
		break;
	case 	23:
		if(typeId == 1) result = 20;		
		if(typeId == 2) result = 2;		
		break;
	case 	24:
		if(typeId == 1) result = 1;
		if(typeId == 2) result = 4;		
		break;
	case 	25:
		if(typeId == 1) result = 2;
		if(typeId == 2) result = 6;		
		break;
	case 	26:
		if(typeId == 1) result = 3;
		if(typeId == 2) result = 8;		
		break;
	case 	27:
		if(typeId == 1) result = 4;		
		break;
	case 	28:
		if(typeId == 1) result = 5;
		
		break;
	case 	29:
		if(typeId == 1) result = 6;
		break;
	case 	30:
		if(typeId == 1) result = 7;
		break;
	case 	31:

		break;
	case 	32:

		break;
	case 	33:

		break;
	case 	34:

		break;
	case 	35:

		break;
	case 	36:

		break;
	case 	37:

		break;
	case 	38:

		break;
	case 	39:

		break;
	case 	40:

		break;
	case 	41:

		break;
	case 	42:

		break;
	case 	43:

		break;
	case 	44:

		break;
	case 	45:

		break;
	case 	46:

		break;
	case 	47:

		break;
	case 	48:

		break;
	case 	49:

		break;
	case 	50:

		break;
	case 	51:

		break;
	case 	52:

		break;
	case 	53:

		break;
	case 	54:

		break;
	case 	55:

		break;
	case 	56:

		break;
	case 	57:

		break;
	case 	58:

		break;
	case 	59:

		break;
	case 	60:

		break;
	case 	61:

		break;
	case 	62:

		break;
	case 	63:

		break;
	case 	64:

		break;
	case 	65:

		break;
	case 	66:

		break;
	case 	67:

		break;
	case 	68:

		break;
	case 	69:

		break;
	case 	70:
		if(typeId == 0) result = 69;
		break;
	case 71:
		break;
	case 72:
		break;
	case 73:
		break;
	case 74:
		break;
	case 75:
		break;
	case 76:
		break;
	case 77:
		break;
	case 78:
		break;
	case 79:
		break;
	case 	80:
		if(typeId == 0) result = 31;
		break;
	case 	81:
		if(typeId == 0) result = 32;
		break;
	case 	82:
		if(typeId == 0) result = 33;
		break;
	case 	83:
		if(typeId == 0) result = 34;
		break;
	case 	84:
		if(typeId == 0) result = 36;
		break;
	case 	85:
		if(typeId == 0) result = 35;
		break;
	case 	86:
		if(typeId == 0) result = 27;
		break;
	case 	87:
		if(typeId == 0) result = 26;
		break;
	case 	88:
		if(typeId == 0) result = 25;
		break;
	case 89:
		if(typeId == 0) result = 66;
		break;
	case 90:
		if(typeId == 0) result = 67;
		break;
	case 91:
		break;

	default:
		break;
	}

	return result;
}

void BetLimit::SpanStringByLen(std::string &spanStr,const int len)
{
	for(int i = len;i < spanStr.size();i += len)
	{
		if (CheckIsNum(spanStr.substr(i, 1), 9)) {i = i - len + 1; continue;} 
		spanStr.insert(i," \n ");
	}
}

bool BetLimit::CheckStrSpace(const char* str)
{
	bool result = false; //没有空格
	int count = strlen(str);
	for(int i = 0;i < count;i ++)
	{
		if(str[i] == ' ') 
		{
			result = true; //有空格
			break;
		}
	}

	return result;
}

//true则为非法
bool BetLimit::ValidateStr(const char* str)
{
	bool result = false;
	int count = strlen(str);

	for(int i = 0;i < count;i ++)
	{
		int tempInt = str[i];
		//ascii表和拓展表
		if(tempInt >=0 && tempInt < 48 || (tempInt > 57 && tempInt < 65) || (tempInt > 90 && tempInt < 97) || (tempInt > 122 && tempInt < 256))
		{
			result = true;
			break;
		}
	}
	return result;
}

long long BetLimit::getRealQihao(long long iQihao,int gameKind, long long GDselectedItemTagNumber)
{
	long long result = iQihao;

	if (result < GDselectedItemTagNumber) return result;

	int caiTypeValue = 0;
	int caiWei = 1000;

	if(gameKind == CZChongQingSSC) //重庆时时彩
	{
		caiTypeValue = 120;
		caiWei = 1000;
	}else 
		if(gameKind == CZXinJiangSSC) //新疆时时彩
		{
			caiTypeValue = 96;
			caiWei = 100;
		}else 
			if(gameKind == CZ_FENFEN_CAI) //吉利分分彩
			{
				caiTypeValue = 1440;
				caiWei = 10000;
			}else 
			if(gameKind == CZ_JILI_2FEN_CAI) //吉利两分彩
			{
				caiTypeValue = 720;
				caiWei = 1000;
			}else 
				if(gameKind == CZ_STOCK_FINANCE)	//猜涨跌
				{
					caiTypeValue = 0;
					caiWei = 1;
				}else
					if(gameKind == CZ_TENCENT_FEN_CAI)		//腾讯彩
					{
						caiTypeValue = 1440;
						caiWei = 10000;
					}else
						if(gameKind == CZ_QQ_FEN_CAI)	//QQ彩
						{
							caiTypeValue = 1440;
							caiWei = 10000;
						}
				if(gameKind == CZ_WUFEN_CAI) //吉利五分彩
				{ 
					caiTypeValue = 288;
					caiWei = 1000;
				}
				else
					if (gameKind == CZ_TIANJIN_SSC) //天津时时彩
					{
						caiTypeValue= 84; //中间多个0
						caiWei = 1000;
					}
					else if(gameKind == CZ_KOREA_FEN_CAI)  //韩国1.5分彩
					{
						caiTypeValue = 0;
						caiWei = 1;
					}
					else if(gameKind == CZ_CANADA_FEN_CAI)
					{
						caiTypeValue = 0;
						caiWei = 1;
					}
					else if (gameKind == CZGD11Xuan5)//广东11选5
					{
						caiTypeValue = 84;
						caiWei = 100;
					} 
					else if (gameKind == CZSD11Xuan5)//山东11选5
					{
						caiTypeValue = 87;
						caiWei = 100;
					} 
					else if( gameKind == CZJX11Xuan5)
					{
						caiTypeValue = 84;
						caiWei = 100;
					}
					else if( gameKind == CZ_HLJ11Xuan5)
					{
						caiTypeValue = 88;
						caiWei = 100;
					}		 
					else if (gameKind == CZ_BJPK10)//北京
					{
						caiTypeValue = 0;
						caiWei = 1;
					} 
					else if (gameKind == CZ3D)//福彩3D
					{
						caiTypeValue = 0;
						caiWei = 1;
					}  
					else if (gameKind == CZPailie3)//排列3
					{
						caiTypeValue = 0;
						caiWei = 1;
					}
					else if(gameKind == CZ_SEVENSTAR)//七星彩
					{
						caiWei = 3;
						caiTypeValue = 156;
					}
					else if(gameKind == CZKUAILE8 || gameKind == CZ_BEIJING_5FEN_CAI)//北京快8
					{
						caiTypeValue = 0;
						caiWei = 1;
					}

		if (caiTypeValue == 0 ) return result;

		//间距
		int idis = result - GDselectedItemTagNumber;
		//实际值										
		int temp = idis + GDselectedItemTagNumber % caiWei;
										
		long days = (temp - 1) / caiTypeValue;//减1再取余 确保120也不会7计算 小于120为0天

		if (days > 0)
		{
			time_t today;
			today = time(NULL);
			long second = 24 * 3600 * days;
			today += second; //后N天

			struct tm *p = localtime(&today);

			char szBuf2[12] = {0};  
			if (caiWei == 10000)
			{
				sprintf(szBuf2, "%4d%02d%02d%04d", 1900 + p->tm_year, p->tm_mon + 1,p->tm_mday,temp % caiTypeValue);
			} else
				if (caiWei == 1000)
				{
					if(gameKind == CZ_SEVENSTAR)
					{
						sprintf(szBuf2, "%4d%03d", 1900 + p->tm_year + temp / caiTypeValue, temp % caiTypeValue); //七星彩比较特殊只有年份
					} else													
					{
						sprintf(szBuf2, "%4d%02d%02d%03d", 1900 + p->tm_year, p->tm_mon + 1,p->tm_mday,temp % caiTypeValue);
					}
													
				} else
					if (caiWei == 100)
					{
						sprintf(szBuf2, "%4d%02d%02d%02d", 1900 + p->tm_year, p->tm_mon + 1,p->tm_mday,temp % caiTypeValue);
					} else
						if (caiWei == 10)
						{
							sprintf(szBuf2, "%4d%02d%02d%d", 1900 + p->tm_year, p->tm_mon + 1,p->tm_mday,temp % caiTypeValue);
						} else
						{
							sprintf(szBuf2, "%4d%02d%02d", 1900 + p->tm_year, p->tm_mon + 1,p->tm_mday);
						}

						result = atof(szBuf2);
		}

		return result;
}


int BetLimit::GetStrFromSplit(std::string &splitStr, const int num,const int maxCount)
{
	std::string result = splitStr;
	splitStr.clear();

	if(result.size() < 1) return 0;

	std::string segStr[5] = {"","","","",""};

	//拆分字符串
	int posStart = 0;
	int posEnd = 0;
	for(int i = 0;i < 5;i ++)
	{
		posEnd = result.find(',', posStart);
		if(posEnd < 0)
		{
			posEnd = result.size();
		}
		segStr[i].clear();
		segStr[i].append(result.substr(posStart,posEnd - posStart));				

		posStart = posEnd + 1;
	}

	//组合数

	//字符串
	std::string tempStr = "";
	for(int k = 0;k < num;k ++) tempStr.append("1");	
		
	tempStr = ExposeString(tempStr.c_str(),"11111",5 - num, 1);

	int count = 0;
	int zhushu = 0;int tempZhushu = 1;
	for(int j = 0;j < tempStr.size();j ++)
	{
		if(tempStr.at(j) == '1')
		{
			splitStr.append(segStr[count]);
			tempZhushu *= segStr[count].size();
		}else
		if(tempStr.at(j) == ',')
		{
			count ++;
			splitStr.append(",");			
		}else
		if(tempStr.at(j) == '|')
		{
			//错误的符号
			count ++;
			splitStr.append("|");

			//对5取余			
			count = count % 5;
			if(tempZhushu > maxCount)
			{
				//单注超过限定
				splitStr.clear();
				return 0;
			}else
			{
				zhushu += tempZhushu;
				tempZhushu = 1;	
			}
		}
	}
	
	return zhushu;
}

int BetLimit::GetLastNumberFromStr(const char* str)
{
	std::string tempStr = str;
	int pos = tempStr.find('=');

	if(pos < 0) return 0;

	int result = atoi(tempStr.substr(pos + 2, 2).c_str());

	return result;
}

std::string BetLimit::GetStrWithoutSpace(const char* str)
{
	std::string result = str;

	int start = result.find_first_not_of(" ");
	int end = result.find_last_not_of(" ");

	return result.substr(start,end - start + 1);
}

std::string BetLimit::GetStrWithoutHtml(const char* str)
{
	std::string result = str;
	//result.replace(pos,count,"");
	int posHead = -1;
	int posEnd = -1;
	int posCount = -1;
	//检测标签
	do
	{
		posHead = result.find_first_of("<");
		posEnd = result.find_first_of(">");
		posCount = posEnd - posHead + 1;

		if(posCount <= 2) break;

		result.replace(posHead, posCount, "   ");
	}
	while(posHead != result.npos && posEnd != result.npos && posHead >= 0 && posEnd >= 0);	
	//检测空格
	do 
	{
		posHead = result.find("&");
		if(posHead >= 0)
		{
			result.replace(posHead, 6, " ");
		}
	} while (posHead != result.npos && posHead >= 0);
	

	return result;
}
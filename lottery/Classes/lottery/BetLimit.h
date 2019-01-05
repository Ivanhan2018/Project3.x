#ifndef _BETLIMIT_H_
#define _BETLIMIT_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class BetLimit
{
public:
	BetLimit(){}
	~BetLimit(){}
	//是否超过注数限制
	static int GetMaxBetFromId(int gameKind,int wanfaId);
	//得到玩法的名字
	static std::string GetWanFaNameFromId(const int gameId,const int wanfaId);
	//检测定位胆是否合法投注  其它玩法也使用此函数
	static bool CheckDingWeiDanStr(int wanfaId,const char* betNumber);
	//校正日期字符串
	static std::string AjustDateStr(const char* dateString);
	//检测日期字符串
	static bool CheckDateStr(const char* dateString);
	//龙虎和翻译
	static std::string GetLongHuHeStr(const char* touzhunumber);
	//上中下奇和偶翻译
	static std::string GetKuai8Str(const char* touzhunumber,const int delta = 0);
	//六合彩翻译
	static std::string GetLiuHeCaiStr(const char* touzhunumber, const int delta = 0);
	//生肖翻译
	static std::string GetLiuHeShengXiaoStr(const char* touzhunumber);
	//大小单双翻译
	static std::string GetDXDSHStr(const char* touzhunumber);
	//个十百千万翻译
	static std::string GetGSBQWStr(const char* touzhunumber);
	//幸运28翻译
	static std::string GetXingYunStr(const int wanfa,const char* touzhunumber);
	//幸运28开奖结果
	static std::string GetXingYunResultStr(const char* resultStr);
	//快8字符处理
	static std::string GetKuai8ResultStr(const char* resultStr);
	//姓名隐藏***
	static std::string GetNameWithXing(const char* name);
	//银行卡号隐藏
	static std::string GetBankCardWithXing(const char* name);
	//检测输入字符串有效性 需全部为数字
	static std::string GetDanShiStr(const char* touzhunumber,const int number,const int isRepeat,const int maxNum,const int numCount = 1,const bool isPK = false);
	//任选单式
	static std::string GetRenXuanDanShiStr(const char* touzhunumber,const int number,const int isRepeat,const int maxNum,const char* tailStr,int beishu,bool isZhiXuan);
	//重定位玩法id typeId 0, 1, 2, 3, 4, 5分别代表重庆彩, 广东11选5, PK10, 3D 七星彩 快8
	static int GetWanfaIdFromId(const int wafaId,const int typeId);
	//截字符串
	static void SpanStringByLen(std::string &spanStr,const int len = 35);
	//检查字符串是否有空格
	static bool CheckStrSpace(const char* str);
	//检查字符串非法字符
	static bool ValidateStr(const char* str);
	//追号期数
	static long long getRealQihao(long long iQihao,int gameKind, long long GDselectedItemTagNumber);
	//重新组合字符串  默认位数为5，暂时不考虑其它位数 返回注数
	static int GetStrFromSplit(std::string &splitStr, const int num,const int maxCount);
	//获取开奖结果
	static int GetLastNumberFromStr(const char* str);
	//自动去掉空格
	static std::string GetStrWithoutSpace(const char* str);
	//去掉html标签
	static std::string GetStrWithoutHtml(const char* str);
};

#endif
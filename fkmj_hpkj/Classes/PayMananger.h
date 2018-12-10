#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "CGPShopInfoMission.h"

struct WeiXinPayInfo
{
	std::string appid;
	std::string attach;
	std::string body;
	std::string mch_id;
	std::string nonce_str;
	std::string notify_url;
	std::string out_trade_no;
	std::string spbill_create_ip;
	std::string total_fee;
	std::string trade_type;

	std::string key;
};
struct HuiPayInfo
{
	std::string version;
	std::string pay_type;
	std::string agent_id;
	std::string agent_bill_id;
	std::string pay_amt;
	std::string return_url;
	std::string notify_url;
	std::string user_ip;
	std::string agent_bill_time;
	std::string goods_name;
	std::string goods_num;
	std::string remark;
	std::string goods_note;

	std::string key;
};
class PayMananger
	:public cocos2d::Ref
	,public CGShopInfoMissionSink
	,public FvSingleton<PayMananger> 
{
public:
	PayMananger();
	~PayMananger();
public:
	void clearItem();
	bool isInShopping();
	void test(int iShopID);
	void startWeiXinPay(WeiXinPayInfo& kWeiXinPayInfo,int iItemID);
	void CB_StartWeiXinPay(std::string kStr);
	void jni_WeiXinResoult(std::string kErrorCode);

	void test1(int iShopID);
	void startHuiPay(HuiPayInfo& kHuiPayInfo,int iItemID);
	void CB_StartHuiPay(std::string kStr);
	void jni_HuiResoult(std::string kErrorCode);
public:
	std::string m_kWeiXinPrepay_id;
	WeiXinPayInfo m_kWeiXinPayInfo;
public:
	std::string m_kHuiToken_id;
	HuiPayInfo m_kHuiPayInfo;
public:
	int m_iItemID;
	CGPShopInfoMission	m_kShopInfoMission;
};
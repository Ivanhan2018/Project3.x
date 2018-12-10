#include "PayMananger.h"
#include "GameLib.h"
#include "SliderMenu.h"
#include "external/tinyxml2/tinyxml2.h"
#include "MCWebReq.h"
#include "MD5.h"
#include "JniFun.h"

FV_SINGLETON_STORAGE(PayMananger);

static tinyxml2::XMLElement* getPayXMLNodeForKey(const char* pKey, tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc,std::string xmlBuffer)
{
	tinyxml2::XMLElement* curNode = nullptr;

	// check the key value
	if (! pKey)
	{
		return nullptr;
	}

	do 
	{
		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
		*doc = xmlDoc;

		if (xmlBuffer.empty())
		{
			CCLOG("can not read xml file");
			break;
		}
		xmlDoc->Parse(xmlBuffer.c_str(), xmlBuffer.size());

		// get root node
		*rootNode = xmlDoc->RootElement();
		if (nullptr == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
		// find the node
		curNode = (*rootNode)->FirstChildElement();
		while (nullptr != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, pKey))
			{
				break;
			}

			curNode = curNode->NextSiblingElement();
		}
	} while (0);

	return curNode;
}

static std::string setPayValueForKey(std::string pKey, std::string pValue,std::string xmlBuffer)
{
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	// check the params
	if (pKey=="")
	{
		return xmlBuffer;
	}
	// find the node
	node = getPayXMLNodeForKey(pKey.c_str(), &rootNode, &doc,xmlBuffer);
	// if node exist, change the content
	if (node)
	{
		if (node->FirstChild())
		{
			node->FirstChild()->SetValue(pValue.c_str());
		}
		else
		{
			tinyxml2::XMLText* content = doc->NewText(pValue.c_str());
			node->LinkEndChild(content);
		}
	}
	else
	{
		if (rootNode)
		{
			tinyxml2::XMLElement* tmpNode = doc->NewElement(pKey.c_str());//new tinyxml2::XMLElement(pKey);
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* content = doc->NewText(pValue.c_str());//new tinyxml2::XMLText(pValue);
			tmpNode->LinkEndChild(content);
		}    
	}

	// save file and free doc
	if (doc)
	{
		tinyxml2::XMLPrinter stream;
		doc->Print( &stream );
		return stream.CStr();
	}
	return xmlBuffer;
}

std::string getPayForKey(const char* pKey, std::string xmlBuffer)
{
	const char* value = nullptr;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getPayXMLNodeForKey(pKey, &rootNode, &doc,xmlBuffer);
	// find the node
	if (node && node->FirstChild())
	{
		return (const char*)(node->FirstChild()->Value());
	}
	return "";
}
PayMananger::PayMananger()
	:m_iItemID(-1)
	,m_kShopInfoMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	m_kShopInfoMission.setMissionSink(this);
}
PayMananger::~PayMananger()
{

}
void PayMananger::clearItem()
{
	m_iItemID = -1;
}
bool PayMananger::isInShopping()
{
	return m_iItemID != -1;
}
void PayMananger::test(int iShopID)
{
	std::string kTimeStr = utility::toString(time(NULL));
	WeiXinPayInfo kWeiXinPayInfo;
	kWeiXinPayInfo.appid = "wx26ebc7a3307ea22a";
	kWeiXinPayInfo.attach = "attach";//附加数据
	kWeiXinPayInfo.body = "APPzhifu";//商品描述
	kWeiXinPayInfo.mch_id = "1374879202";//商户号
	kWeiXinPayInfo.nonce_str = kTimeStr;//随机字符串
	kWeiXinPayInfo.notify_url = "182.110.11.40";
	kWeiXinPayInfo.out_trade_no = kTimeStr;//商户订单号
	kWeiXinPayInfo.spbill_create_ip = "182.110.11.40";
	kWeiXinPayInfo.total_fee = "1";//总金额
	kWeiXinPayInfo.trade_type = "APP";
	kWeiXinPayInfo.key = "bosengame12345679123456789123456";//微信商户密钥设置
	startWeiXinPay(kWeiXinPayInfo,1);
}
void PayMananger::startWeiXinPay(WeiXinPayInfo& kWeiXinPayInfo,int iItemID)
{
	if (isInShopping())
	{
		return;
	}
	m_iItemID = iItemID;
	m_kWeiXinPayInfo = kWeiXinPayInfo;

	if (kWeiXinPayInfo.attach == "")
	{
		kWeiXinPayInfo.attach = "attach";
	}
	if (kWeiXinPayInfo.body == "")
	{
		kWeiXinPayInfo.body = "body";
	}

	std::string kStrXML="<xml></xml>"; 
	kStrXML = setPayValueForKey("appid",kWeiXinPayInfo.appid,kStrXML);
	kStrXML = setPayValueForKey("attach",kWeiXinPayInfo.attach,kStrXML);
	kStrXML = setPayValueForKey("body",kWeiXinPayInfo.body,kStrXML);
	kStrXML = setPayValueForKey("mch_id",kWeiXinPayInfo.mch_id,kStrXML);
	kStrXML = setPayValueForKey("nonce_str",kWeiXinPayInfo.nonce_str,kStrXML);
	kStrXML = setPayValueForKey("notify_url",kWeiXinPayInfo.notify_url,kStrXML);
	kStrXML = setPayValueForKey("out_trade_no",kWeiXinPayInfo.out_trade_no,kStrXML);
	kStrXML = setPayValueForKey("spbill_create_ip",kWeiXinPayInfo.spbill_create_ip,kStrXML);
	kStrXML = setPayValueForKey("total_fee",kWeiXinPayInfo.total_fee,kStrXML);
	kStrXML = setPayValueForKey("trade_type",kWeiXinPayInfo.trade_type,kStrXML);

	std::string kStr1;
	MCWebReq::pushValue(kStr1,"appid",kWeiXinPayInfo.appid);
	MCWebReq::pushValue(kStr1,"attach",kWeiXinPayInfo.attach);
	MCWebReq::pushValue(kStr1,"body",kWeiXinPayInfo.body);
	MCWebReq::pushValue(kStr1,"mch_id",kWeiXinPayInfo.mch_id);
	MCWebReq::pushValue(kStr1,"nonce_str",kWeiXinPayInfo.nonce_str);
	MCWebReq::pushValue(kStr1,"notify_url",kWeiXinPayInfo.notify_url);
	MCWebReq::pushValue(kStr1,"out_trade_no",kWeiXinPayInfo.out_trade_no);
	MCWebReq::pushValue(kStr1,"spbill_create_ip",kWeiXinPayInfo.spbill_create_ip);
	MCWebReq::pushValue(kStr1,"total_fee",kWeiXinPayInfo.total_fee);
	MCWebReq::pushValue(kStr1,"trade_type",kWeiXinPayInfo.trade_type);
	MCWebReq::pushValue(kStr1,"key",kWeiXinPayInfo.key);

	std::string kStrMD5 = md5(kStr1);
	kStrXML = setPayValueForKey("sign",kStrMD5,kStrXML);

	MCWebReq::instance().sendRequestStr("https://api.mch.weixin.qq.com/pay/unifiedorder",CC_CALLBACK_1(PayMananger::CB_StartWeiXinPay,this),kStrXML);
}
void PayMananger::CB_StartWeiXinPay(std::string kStr)
{
	std::string kErrorMsg = getPayForKey("return_msg",kStr);
	if (kErrorMsg == "OK")
	{
		NoticeMsg::Instance().ShowTopMsg(kErrorMsg);

		std::string appId = m_kWeiXinPayInfo.appid;
		std::string nonceStr = m_kWeiXinPayInfo.nonce_str;
		std::string packageValue = "Sign=WXPay";
		std::string partnerId = m_kWeiXinPayInfo.mch_id;
		m_kWeiXinPrepay_id = getPayForKey("prepay_id",kStr);
		std::string timeStamp = m_kWeiXinPayInfo.nonce_str;

		std::string kStr1;
		MCWebReq::pushValue(kStr1,"appid",appId);
		MCWebReq::pushValue(kStr1,"noncestr",nonceStr);
		MCWebReq::pushValue(kStr1,"package",packageValue);
		MCWebReq::pushValue(kStr1,"partnerid",partnerId);
		MCWebReq::pushValue(kStr1,"prepayid",m_kWeiXinPrepay_id);
		MCWebReq::pushValue(kStr1,"timestamp",timeStamp);
		MCWebReq::pushValue(kStr1,"key",m_kWeiXinPayInfo.key);
		std::string kStrMD5 = md5(kStr1);

		std::string kSendStr = "";
		kSendStr += appId;
		kSendStr += "|";
		kSendStr += partnerId;
		kSendStr += "|";
		kSendStr += m_kWeiXinPrepay_id;
		kSendStr += "|";
		kSendStr += nonceStr;
		kSendStr += "|";
		kSendStr += timeStamp;
		kSendStr += "|";
		kSendStr += packageValue;
		kSendStr += "|";
		kSendStr += kStrMD5;

		JniFun::startWeiXinPay(kSendStr);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		jni_WeiXinResoult("0");
#endif
	}
	else
	{
		NoticeMsg::Instance().ShowTopMsg(utility::toString("WeiXinOder Error = ",kErrorMsg));
		clearItem();
	}
}
void PayMananger::jni_WeiXinResoult(std::string kErrorCode)
{
	NoticeMsg::Instance().ShowTopMsg(kErrorCode);
	if (kErrorCode == "0")
	{
		CMD_GP_UpShopOder kUpShoperID;
		kUpShoperID.appid = m_kWeiXinPayInfo.appid;
		kUpShoperID.dwItemID = m_iItemID;
		kUpShoperID.dwUserID = UserInfo::Instance().getUserID();
		kUpShoperID.mch_id = m_kWeiXinPayInfo.mch_id;
		kUpShoperID.prepay_id = m_kWeiXinPrepay_id;
		kUpShoperID.total_fee = utility::parseInt(m_kWeiXinPayInfo.total_fee);
		m_kShopInfoMission.UpShopOder(kUpShoperID);
	}
	clearItem();
}
void PayMananger::test1(int iShopID)
{

	std::string kTimeStr = utility::toString(time(NULL));
	HuiPayInfo kHuiPayInfo;
	kHuiPayInfo.version = "1";
	kHuiPayInfo.pay_type = "30";
	kHuiPayInfo.agent_id = "2071108";
	kHuiPayInfo.agent_bill_id = kTimeStr;
	kHuiPayInfo.pay_amt = "3.00";
	kHuiPayInfo.return_url="http://www.baidu.com";
	kHuiPayInfo.notify_url="http://www.baidu.com";
	kHuiPayInfo.user_ip="127.0.0.1";
	kHuiPayInfo.agent_bill_time = utility::getTimeStr();
	kHuiPayInfo.goods_name = "goods_name";
	kHuiPayInfo.goods_num = "1";
	kHuiPayInfo.remark = "1";
	kHuiPayInfo.goods_note = "1";
	kHuiPayInfo.key = "00C3D2DD21D84B48A6DFA178";
	
	startHuiPay(kHuiPayInfo,1);
}
void PayMananger::startHuiPay(HuiPayInfo& kHuiPayInfo,int iItemID)
{
	if (isInShopping())
	{
		return;
	}
	m_iItemID = iItemID;
	m_kHuiPayInfo = kHuiPayInfo;
	std::string kStrUrl = "https://pay.heepay.com/Phone/SDK/PayInit.aspx?";
	MCWebReq::pushValue(kStrUrl,"version",kHuiPayInfo.version);
	MCWebReq::pushValue(kStrUrl,"pay_type",kHuiPayInfo.pay_type);
	MCWebReq::pushValue(kStrUrl,"agent_id",kHuiPayInfo.agent_id);
	MCWebReq::pushValue(kStrUrl,"agent_bill_id",kHuiPayInfo.agent_bill_id);
	MCWebReq::pushValue(kStrUrl,"pay_amt",kHuiPayInfo.pay_amt);
	MCWebReq::pushValue(kStrUrl,"return_url",kHuiPayInfo.return_url);
	MCWebReq::pushValue(kStrUrl,"notify_url",kHuiPayInfo.notify_url);
	MCWebReq::pushValue(kStrUrl,"user_ip",kHuiPayInfo.user_ip);
	MCWebReq::pushValue(kStrUrl,"agent_bill_time",kHuiPayInfo.agent_bill_time);
	MCWebReq::pushValue(kStrUrl,"goods_name",kHuiPayInfo.goods_name);
	MCWebReq::pushValue(kStrUrl,"goods_num",kHuiPayInfo.goods_num);
	MCWebReq::pushValue(kStrUrl,"remark",kHuiPayInfo.remark);
	MCWebReq::pushValue(kStrUrl,"goods_note",kHuiPayInfo.goods_note);

	std::string kStr1;
	MCWebReq::pushValue(kStr1,"version",kHuiPayInfo.version);
	MCWebReq::pushValue(kStr1,"agent_id",kHuiPayInfo.agent_id);
	MCWebReq::pushValue(kStr1,"agent_bill_id",kHuiPayInfo.agent_bill_id);
	MCWebReq::pushValue(kStr1,"agent_bill_time",kHuiPayInfo.agent_bill_time);
	MCWebReq::pushValue(kStr1,"pay_type",kHuiPayInfo.pay_type);
	MCWebReq::pushValue(kStr1,"pay_amt",kHuiPayInfo.pay_amt);
	MCWebReq::pushValue(kStr1,"notify_url",kHuiPayInfo.notify_url);
	MCWebReq::pushValue(kStr1,"user_ip",kHuiPayInfo.user_ip);
	MCWebReq::pushValue(kStr1,"key",kHuiPayInfo.key);
	std::string kStrMD5 = md5(kStr1);

	MCWebReq::pushValue(kStrUrl,"sign",kStrMD5);

	MCWebReq::instance().sendRequestStr(kStrUrl,CC_CALLBACK_1(PayMananger::CB_StartHuiPay,this),"");
}
void PayMananger::CB_StartHuiPay(std::string kStr)
{
	if (m_kHuiToken_id != "")
	{
		return;
	}
	std::vector<std::string> kTemp1 = utility::split(kStr,"<token_id>");
	if (kTemp1.size() < 2)
	{
		clearItem();
		NoticeMsg::Instance().ShowTopMsg(kStr);
		return;
	}
	std::vector<std::string> kTemp2 = utility::split(kTemp1[1],"</token_id>");
	if (kTemp2.size() == 0)
	{
		clearItem();
		NoticeMsg::Instance().ShowTopMsg(kStr);
		return;
	}

	m_kHuiToken_id = kTemp2[0];//getPayForKey("token_id",kStr);
	if (m_kHuiToken_id == "")
	{
		clearItem();
		NoticeMsg::Instance().ShowTopMsg(kStr);
		return;
	}
	std::string kSendStr = "";
	kSendStr += m_kHuiToken_id;
	kSendStr += "|";
	kSendStr += m_kHuiPayInfo.agent_id;
	kSendStr += "|";
	kSendStr += m_kHuiPayInfo.agent_bill_id;
	kSendStr += "|";
	kSendStr += "30";
	////if (utility::parseInt(m_kHuiPayInfo.pay_amt) > 4)
	////{
	////	jni_HuiResoult("01");
	////}
	////else
	{
		JniFun::startHuiPay(kSendStr);
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	jni_HuiResoult("01");
#endif
}
void PayMananger::jni_HuiResoult(std::string kErrorCode)
{
	NoticeMsg::Instance().ShowTopMsg(kErrorCode);
	if (kErrorCode == "01" && m_kHuiToken_id != "")
	{
		CMD_GP_UpShopOder kUpShoperID;
		kUpShoperID.appid = m_kHuiToken_id;
		kUpShoperID.dwItemID = m_iItemID;
		kUpShoperID.dwUserID = UserInfo::Instance().getUserID();
		kUpShoperID.mch_id = m_kHuiPayInfo.agent_id;
		kUpShoperID.prepay_id = m_kHuiPayInfo.agent_bill_id;
		kUpShoperID.total_fee = utility::parseInt(m_kWeiXinPayInfo.total_fee);
		m_kShopInfoMission.UpShopOder(kUpShoperID);
	}
	m_kHuiToken_id = "";
	clearItem();
}
#pragma once

#include "HttpClientEx.h"
#include "cocos2d.h"
#include "utility.h"
#include "external/json/document.h"

typedef std::map<std::string,std::string> QYWebReqInfo;

struct SendStructInfo
{
	SendStructInfo()
	{
		pRequest = NULL;
		m_fStrResCallBack = nullptr;
		m_fJsonResCallBack = nullptr;
	}
	cocos2d::network::HttpRequestEx* pRequest;
	std::function<void(std::string)> m_fStrResCallBack;
	std::function<void(rapidjson::Document*)> m_fJsonResCallBack;
	std::string kImagicUrlName;
};
typedef std::vector<SendStructInfo> SendStructInfoList;

class MCWebReq
	:public cocos2d::Ref
{
public:
	static MCWebReq& instance(){
		static MCWebReq _instance;
		return _instance;
	}
	static MCWebReq& getInstance(){
		return instance();
	}

public:
	void sendRequestStr(std::string kUrl
		,std::function<void(std::string)> _callBack1);
	void sendRequestStr(std::string kUrl
		,std::function<void(std::string)> _callBack1,std::string kData = "");
	void sendRequestDocument(std::string kUrl
		,std::function<void(rapidjson::Document*)> _callBack1);
	void sendRequestDocumentUrl(std::string kUrl
		,std::function<void(rapidjson::Document*)> _callBack1
		,std::function<void(std::string)> _callBack2,std::string kData = "");
	
public: //request
	void CB_IP(rapidjson::Document* pDoc);
	
	std::string getPostData(std::map<std::string,std::string>& dataMap);
	std::string joinMapToData(std::map<std::string, std::string> &paramenters);

	std::string parseResData(cocos2d::network::HttpResponseEx *response);
	void sendFile(std::string kUrl, std::string &fileName,const cocos2d::network::ccHttpRequestCallback &callback);
	void sendFileURLHeadImgaic(std::string kUrl, std::string &fileName,
		std::function<void(rapidjson::Document*)> _callBack);
	cocos2d::network::HttpRequestEx* getFile(std::string kUrl,const cocos2d::network::ccHttpRequestCallback &callback);
	cocos2d::network::HttpRequestEx* getFileEx(std::string kUrl,const cocos2d::network::ccHttpRequestCallback &callback);
protected://resqunse
	void resCallBack(cocos2d::network::HttpClientEx* client,cocos2d::network::HttpResponseEx* response);

	bool parseResByJson(cocos2d::network::HttpResponseEx *response,rapidjson::Document &doc);
	bool parseDataByJson(const std::string& data, rapidjson::Document &doc);
public:
	template<typename T>
	static void pushValue(std::string& kUrl,std::string kKey,T kValue)
	{
		pushValue(kUrl,kKey,utility::toString(kValue));
	}
	static void pushValue(std::string& kUrl,std::string kKey,std::string kValue);
	template<typename T>
	static std::string getDataValueStr(T* kValue,std::string kName)
	{
		if ((*kValue).HasMember(kName.c_str())&&((*kValue)[kName.c_str()].IsString()))
		{
			return (*kValue)[kName.c_str()].GetString();
		}
		return "";
	}
	template<typename T>
	static int getDataValueInt(T* kValue,std::string kName)
	{
		if ((*kValue).HasMember(kName.c_str())&&((*kValue)[kName.c_str()].IsInt()))
		{
			return (*kValue)[kName.c_str()].GetInt();
		}
		return 0;
    }
	template<typename T>
	static int getDataValueDouble(T* kValue,std::string kName)
	{
		if ((*kValue).HasMember(kName.c_str())&&((*kValue)[kName.c_str()].IsDouble()))
		{
			return (*kValue)[kName.c_str()].GetDouble();
		}
		return 0;
	}
	

    void init();
protected:
private:
	MCWebReq();
	std::string m_kWebIP;
	std::string m_kSocketIP;
	SendStructInfoList m_kCBList;
};

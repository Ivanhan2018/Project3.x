#pragma once
#include "FvSingleton.h"
#include "cocos2d.h"
#include "MCWebReq.h"

struct ImagicDownInfo
{
	ImagicDownInfo()
	{
		fActTime = 0;
		pRequest = NULL;
		pButtonSprite = NULL;
		bSameSize = false;
	}
	cocos2d::Sprite* pSprite;
	cocos2d::Menu* pButtonSprite;
	cocos2d::network::HttpRequestEx* pRequest;
	std::string kUrl;
	std::string kImagicName;
	float fActTime;
	bool bSameSize;
};

typedef std::vector<ImagicDownInfo> ImagicDownInfoList;

class ImagicDownManager
	:public FvSingleton<ImagicDownManager> 
	,public cocos2d::Ref
{
public:
	ImagicDownManager();
	~ImagicDownManager();
public:
	void OnImagic();
	void GetImagic(cocos2d::network::HttpClientEx *sender, cocos2d::network::HttpResponseEx *response);
public:
	cocos2d::Texture2D* getDownTexture(std::string kFile);
	void addDown(cocos2d::Node* pNode,std::string kUrl,int iUserID,bool bSameSize = true);
	void addDown(cocos2d::Node* pNode,std::string kUrl,std::string kFileName,bool bSameSize = true);
	void upTime(float fTime);
public:
	int m_iIdexCout;
	ImagicDownInfoList m_pDownList;
	std::vector<cocos2d::network::HttpResponseEx*> m_pGetList;
};


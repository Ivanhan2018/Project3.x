#include "ImagicDownManager.h"
#include "ScriptData.h"
#include "WidgetFun.h"
#include "utility.h"

FV_SINGLETON_STORAGE(ImagicDownManager);

ImagicDownManager::ImagicDownManager()
	:m_iIdexCout(0)
{
	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(ImagicDownManager::upTime),this,0.0f,false);
}
ImagicDownManager::~ImagicDownManager(void)
{
	cocos2d::Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(ImagicDownManager::upTime),this);
}
std::string DownImagicGetFileName(std::string kName)
{
	std::string::size_type iStart = 0;
	while (true)
	{
		std::string::size_type iTempPos = kName.find("/",iStart);
		if (iTempPos == std::string::npos)
		{
			break;
		}
		iStart = iTempPos;
		iStart++;
	}
	size_t iEnd = kName.size();
	return kName.substr(iStart,iEnd-iStart);
}
void ImagicDownManager::GetImagic(cocos2d::network::HttpClientEx *sender, cocos2d::network::HttpResponseEx *response)
{
	response->retain();
	m_pGetList.push_back(response);
}
void ImagicDownManager::OnImagic()
{
	if (m_pGetList.size() == 0)
	{
		return;
	}
	cocos2d::network::HttpResponseEx *response = m_pGetList[0];
	m_pGetList.erase(m_pGetList.begin());
	std::string kUrl = response->getHttpRequest()->getUrl();
	std::string kImagicName = cocos2d::FileUtils::getInstance()->getWritablePath()+"test.png";

	ImagicDownInfoList::iterator itor1 = m_pDownList.begin();
	while(itor1 != m_pDownList.end())
	{
		ImagicDownInfo& kInfo = *itor1;
		if (kInfo.kUrl.find(kUrl.c_str()) != std::string::npos)
		{
			kImagicName = kInfo.kImagicName;
			break;
		}
		itor1++;
	}
	std::vector<char> *buffer = response->getResponseData();
	if (buffer->size() == 0)
	{
		return;
	}
	cocos2d::CCImage* img = new cocos2d::CCImage;
	img->initWithImageData((unsigned char*)buffer->data(),buffer->size());
	bool bSucess = false;
	if (img->getData())
	{
		img->saveToFile(kImagicName,false);
		bSucess = true;
	}
	delete img;

	ImagicDownInfoList::iterator itor2 = m_pDownList.begin();
	while(itor2 != m_pDownList.end())
	{
		ImagicDownInfo& kInfo = *itor2;
        if (kInfo.kUrl.find(kUrl.c_str()) != std::string::npos)
		{
			if (kInfo.pSprite)
			{
				if (bSucess)
				{
					WidgetFun::setImagic(kInfo.pSprite,kImagicName,kInfo.bSameSize);
				}
				kInfo.pSprite->release();
			}
			if (kInfo.pButtonSprite)
			{
				if (bSucess)
				{
					//WidgetFun::setButtonImagic(kInfo.pButtonSprite,kImagicName,kImagicName,kImagicName);
				}
				kInfo.pButtonSprite->release();
			}
			itor2 = m_pDownList.erase(itor2);
		}
		else
		{
			itor2++;
		}
	}
	response->release();
}
cocos2d::Texture2D* ImagicDownManager::getDownTexture(std::string kFile)
{
	cocos2d::Texture2D* pTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(kFile.c_str());
	if (pTexture)
	{
		return pTexture;
	}
	pTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(utility::toString("head/",kFile.c_str()));
	if (pTexture)
	{
		return pTexture;
	}
	
	std::string kImagicName = DownImagicGetFileName(kFile);
	kImagicName = cocos2d::FileUtils::getInstance()->getWritablePath()+kImagicName;
	std::string StrSavePath = kImagicName;
	pTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(StrSavePath.c_str());
	if (pTexture)
	{
		return pTexture;
	}
	return NULL;
}
void ImagicDownManager::addDown(cocos2d::Node* pNode,std::string kUrl,int iUserID,bool bSameSize )
{
	int iIdex = 0;
	for (int i = 0;i<kUrl.size();i++)
	{
		iIdex += kUrl[i];
	}
	std::string strSrc = "/0";
	std::string strDes = "/132";
	std::string::size_type pos = 0;  
	std::string::size_type srcLen = strSrc.size();  
	std::string::size_type desLen = strDes.size();  
	pos=kUrl.find(strSrc, pos);   
	while (pos != std::string::npos)
	{  
		if (kUrl.find(strSrc, (pos+desLen)) == std::string::npos)
		{
			kUrl.replace(pos, srcLen, strDes);
			break;
		}
		pos = kUrl.find(strSrc, (pos+desLen));
	}  
	std::string kFileName = utility::toString(iUserID,"Idex",iIdex);
	addDown(pNode,kUrl,kFileName,bSameSize);
}
void ImagicDownManager::addDown(cocos2d::Node* pNode,std::string kUrl,std::string kFileName,bool bSameSize )
{
	if (kUrl == "")
	{
		return;
	}
	cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pNode);
	cocos2d::Menu* pButtonSprite = dynamic_cast<cocos2d::Menu*>(pNode);
	if (!pSprite && !pButtonSprite)
	{
		CCASSERT(false,"");
		return;
	}
	kFileName = cocos2d::FileUtils::getInstance()->getWritablePath()+kFileName+".png";
	std::string StrSavePath = kFileName;
	if (cocos2d::CCTextureCache::sharedTextureCache()->addImage(StrSavePath.c_str()))
	{
		if (pSprite)
		{
			WidgetFun::setImagic(pSprite,StrSavePath,bSameSize);
		}
		if (pButtonSprite)
		{
			//WidgetFun::setButtonImagic(pButtonSprite,StrSavePath,StrSavePath,StrSavePath);
		}
		return;
	}
	bool bHaveGet = false;
	for (int i = 0;i<(int)m_pDownList.size();i++)
	{
		ImagicDownInfo& kInfo = m_pDownList[i];
		if (kInfo.kImagicName == kFileName && kInfo.pSprite == pSprite)
		{
			return;
		}
		if (kInfo.kImagicName == kFileName)
		{
			bHaveGet = true;
		}
		
	}
	ImagicDownInfo kInfo;
	kInfo.pSprite = pSprite;
	kInfo.pButtonSprite = pButtonSprite;
	kInfo.kImagicName = kFileName;
	kInfo.kUrl = kUrl;
	kInfo.bSameSize = bSameSize;
	if (pSprite)
	{
		pSprite->retain();
	}
	if (pButtonSprite)
	{
		pButtonSprite->retain();
	}
	if (!bHaveGet)
	{
		cocos2d::network::HttpRequestEx* pRequest = MCWebReq::instance().getFileEx(kUrl,
			 CC_CALLBACK_2(ImagicDownManager::GetImagic, this));
		kInfo.pRequest = pRequest;
	}
	m_pDownList.push_back(kInfo);
}
void ImagicDownManager::upTime(float fTime)
{
	OnImagic();
	ImagicDownInfoList::iterator itor = m_pDownList.begin();
	while(itor != m_pDownList.end())
	{
		ImagicDownInfo& kInfo = *itor;
		kInfo.fActTime += fTime;
		if (kInfo.fActTime > 20.0f)
		{
			if (kInfo.pSprite)
			{
				kInfo.pSprite->release();
			}
			if (kInfo.pButtonSprite)
			{
				kInfo.pButtonSprite->release();
			}
			itor = m_pDownList.erase(itor);
		}
		else
		{
			itor++;
		}
	}
}
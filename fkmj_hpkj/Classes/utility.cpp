#include "utility.h"
#include "ScriptData.h"
#include "WidgetFun.h"
#include "Convert.h"


//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include "external\win32-specific\icon\include\iconv.h"
//#pragma comment(lib,"libiconv.lib")
//#else
////#include <iconv.h>
//#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "iconv.h"//F:\cocos2d-x-2.2.5\cocos2dx\platform\third_party\win32\iconv\
#pragma comment(lib,"libiconv.lib")
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "iconv.h"//F:/cocos2d-x-2.2.5/iconv/include/
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iconv.h"
#endif

namespace utility
{
	static std::string s_kDebugFileName = "Debug";
	static std::string s_kTime = "";
	//==============中文编码转换===============================================================

	std::string getScriptString(std::string kName,bool bAssert)
	{
		if (bAssert)
		{
			ScriptData<std::string> kTxt(kName);
			return kTxt.Value();
		}
		else
		{
			if (ScriptDataManager::haveValue<std::string>(kName))
			{
				ScriptData<std::string> kTxt(kName);
				return kTxt.Value();
			}
		}
		return "";
	}

	void setPaseTime(std::string kTime)
	{
		s_kTime = kTime;
	}
	void setDebugFileName(std::string kName)
	{
		s_kDebugFileName = kName+".txt";
	}
	std::string getDebugFileName()
	{
		return utility::toString(cocos2d::FileUtils::getInstance()->getWritablePath(),s_kDebugFileName);
	}
	void log(const char * format, ...)
	{
		va_list args;
		va_start(args, format); 

		const int iMaxLenght = 1024;
		char buf[iMaxLenght];
		vsnprintf(buf, iMaxLenght-3, format, args);
		strcat(buf, "\n");	
		strcat(buf, "\0");

		std::string kStr = utility::toString(s_kTime," ",buf);
		if (s_kDebugFileName != "")
		{
			std::string kPathTxt = getDebugFileName();
			FILE* file = fopen(kPathTxt.c_str(), "rb+"); 
			if (!file)
			{
				file = fopen(kPathTxt.c_str(), "wb+");   
			}
			if (file) {   
				fseek(file, 0L, SEEK_END);
				fwrite(kStr.c_str(), sizeof(char), strlen(kStr.c_str()), file);  
				fclose(file);    
			}

		}
		cocos2d::log("%s",buf);
		
		va_end(args);
	}
	void saveFilePath(std::string kStr)
	{
		kStr += "|";
		static std::vector<std::string> kStrV;
		for (int i = 0;i<(int)kStrV.size();i++)
		{
			if (kStrV[i] == kStr)
			{
				return;
			}
		}
		kStrV.push_back(kStr);
		std::string kPathTxt = "FilePath";
		FILE* file = fopen(kPathTxt.c_str(), "rb+"); 
		if (!file)
		{
			file = fopen(kPathTxt.c_str(), "wb+");   
		}
		if (file) {   
			fseek(file, 0L, SEEK_END);
			fwrite(kStr.c_str(), sizeof(char), strlen(kStr.c_str()), file);  
			fclose(file);    
		}
	}
	std::vector<std::string> getFilePath()
	{
		std::string kPathTxt = "FilePath";
		ssize_t iSize = 0;
		char* pChar = (char*)cocos2d::FileUtils::getInstance()->getFileData(
			cocos2d::FileUtils::getInstance()->fullPathForFilename(kPathTxt),"rb",&iSize);
		if (!pChar)
		{
			return std::vector<std::string>();
		}
		std::string kStr = pChar;
		free(pChar);
		return utility::split(kStr,"|");
	}
	std::string FullPath(std::string kName)
	{
		return cocos2d::FileUtils::getInstance()->fullPathForFilename(kName.c_str());
	}
	std::string getFileName(std::string kName)
	{
		std::string::size_type pos = kName.find_last_of('/');
		if (pos != std::string::npos)
		{
			return kName.substr(pos + 1);
		}
		return "";
	}
	cocos2d::Vec2 parsePoint(std::string kValue)
	{
		cocos2d::Vec2 kPoint;
		std::vector<std::string> kStr = split(kValue," ");
		if(kStr.size() == 2)
		{
			kPoint.x = parseFloat(kStr[0]);
			kPoint.y = parseFloat(kStr[1]);
		}
		return kPoint;
	}
	cocos2d::Rect parseRect(std::string kValue)
	{
		cocos2d::Rect kRect;
		std::vector<std::string> kStr = split(kValue," ");
		if(kStr.size() == 4)
		{
			kRect.origin.x = parseFloat(kStr[0]);
			kRect.origin.y = parseFloat(kStr[1]);
			kRect.size.width = parseFloat(kStr[2]);
			kRect.size.height = parseFloat(kStr[3]);
		}
		else
		{
			//	utility::CCAssertWithDes(false,kValue," parseRect Error");
		}
		return kRect;
	}
	RandFloat parseRandFloat(std::string kStr)
	{
		RandFloat kValue;
		std::vector<std::string> kVec = utility::split(kStr,":");
		if (kVec.size() && kVec[0] == "Key")
		{
			kValue.kUserKey = kVec[1];
		}
		else if (kVec.size() == 1)
		{
			float fTemp = utility::parseFloat(kVec[0]);
			kValue.fMin = fTemp;
			kValue.fMax = fTemp;
		}
		else if(kVec.size() == 2)
		{
			kValue.fMin = utility::parseFloat(kVec[0]);
			kValue.fMax = utility::parseFloat(kVec[1]);
		}
		else
		{
			CCASSERT(false,"");
		}
		return kValue;
	}
	RandPos parseRandPos(std::string kStr)
	{
		RandPos kValue;
		std::vector<std::string> kVec = utility::split(kStr,":");
		if (kVec.size() && kVec[0] == "Key")
		{
			kValue.kUserKey = kVec[1];
		}
		else if (kVec.size() == 1)
		{
			cocos2d::Vec2 kPos = utility::parsePoint(kVec[0]);
			kValue.kRandX.fMin = kPos.x;
			kValue.kRandX.fMax = kPos.x;
			kValue.kRandY.fMin = kPos.y;
			kValue.kRandY.fMax = kPos.y;
		}
		else if(kVec.size() == 2)
		{
			cocos2d::Vec2 kPos1 = utility::parsePoint(kVec[0]);
			cocos2d::Vec2 kPos2 = utility::parsePoint(kVec[1]);
			kValue.kRandX.fMin = kPos1.x;
			kValue.kRandX.fMax = kPos2.x;
			kValue.kRandY.fMin = kPos1.y;
			kValue.kRandY.fMax = kPos2.y;
		}
		else
		{
			CCASSERT(false,"");
		}
		return kValue;
	}
	cocos2d::Size parseSize(std::string kValue)
	{
		cocos2d::Size kSize;
		std::vector<std::string> kStr = split(kValue," ");
		if(kStr.size() == 2)
		{
			kSize.width = parseFloat(kStr[0]);
			kSize.height = parseFloat(kStr[1]);
		}
		else
		{
		}
		return kSize;
	}
	cocos2d::Color3B parseColour(std::string kValue)
	{
		cocos2d::Color3B kColour = cocos2d::Color3B(255,255,255);
		cocos2d::Size kSize;
		std::vector<std::string> kStr = split(kValue," ");
		if(kStr.size() == 3)
		{
			kColour.r = parseFloat(kStr[0]);
			kColour.g = parseFloat(kStr[1]);
			kColour.b = parseFloat(kStr[2]);
		}
		else
		{
			utility::CCAssertWithDes(false,kValue," parseColour Error");
		}
		return kColour;
	}
	cocos2d::Color4B parseColour4(std::string kValue)
	{
		cocos2d::Color4B kColour = cocos2d::Color4B(255,255,255,255);
		cocos2d::Size kSize;
		std::vector<std::string> kStr = split(kValue," ");
		if(kStr.size() == 4)
		{
			kColour.r = parseFloat(kStr[0]);
			kColour.g = parseFloat(kStr[1]);
			kColour.b = parseFloat(kStr[2]);
			kColour.a = parseFloat(kStr[3]);
		}
		else
		{
			utility::CCAssertWithDes(false,kValue," parseColour Error");
		}
		return kColour;
	}

	UserString valueForUserString( const char *key, std::map<std::string, std::string>& dict )
	{
		return parseUserString(valueForKey(key,dict));
	}

	UserString parseUserString(std::string kStr)
	{
		UserString kValue;
		std::vector<std::string> kVec = utility::split(kStr,":");
		if (kVec.size() && kVec[0] == "Key")
		{
			kValue.kUserKey = kVec[1];
		}
		else if (kVec.size() == 1)
		{
			kValue.kUserValue = kVec[0];
		}
		else
		{
			CCASSERT(false,"");
		}
		return kValue;
	}
	std::string getUserString(const UserString& kUserString,cocos2d::Node* pNode)
	{
		if (kUserString.kUserKey !="")
		{
			return WidgetFun::getWidgetUserInfo(pNode,kUserString.kUserKey);
		}
		return kUserString.kUserValue;
	}
	float fAbs(const float& fValue)
	{
		if(fValue < 0.f)
		{
			return -fValue;
		}
		return fValue;
	}
	float fMin(const float& fValue1,const float& fValue2)
	{
		if(fValue1 < fValue2)
		{
			return fValue1;
		}
		return fValue2;
	}
	float fMax(const float& fValue1,const float& fValue2)
	{
		if(fValue1 < fValue2)
		{
			return fValue2;
		}
		return fValue1;
	}
	float toDest(const float& fSart,const float& fDest,const float& fAdd)
	{
		float fReturn = fSart;
		if(fSart < fDest)
		{
			fReturn = fSart + fAbs(fAdd);
			fReturn = fMin(fDest,fReturn);
		}
		else
		{
			fReturn = fSart - fAbs(fAdd);
			fReturn = fMax(fDest,fReturn);
		}
		return fReturn;
	}

	bool CCPtInRect(const cocos2d::Rect& rect,const cocos2d::Vec2& point)
	{
		if (point.x > rect.origin.x - rect.size.width/2 && point.x < rect.origin.x + rect.size.width/2 &&
			point.y > rect.origin.y - rect.size.height/2 && point.y < rect.origin.y + rect.size.height/2)
		{
			return true;
		}
		return false;
	}
	cocos2d::Texture2D* addTexture(std::string kTextureName)
	{
		return cocos2d::TextureCache::getInstance()->addImage(kTextureName.c_str());
	}

	void createAttributeDict(std::map<std::string, std::string>& attributeDict,const char **atts)
	{
		if(atts && atts[0])
		{
			for(int i = 0; atts[i]; i += 2) 
			{
				std::string key = (char*)atts[i];
				std::string value = (char*)atts[i+1];
				attributeDict.insert(std::pair<std::string, std::string>(key, value));
			}
		}
	}
	std::string  valueForKey(const char *key, std::map<std::string, std::string>& dict)
	{
		std::map<std::string, std::string>::iterator it = dict.find(key);
		return it!=dict.end() ? it->second.c_str() : "";
	}
	int  valueForInt(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseInt(valueForKey(key,dict));
	}
	float  valueForFloat(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseFloat(valueForKey(key,dict));
	}
	bool  valueForBool(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseBool(valueForKey(key,dict));
	}
	cocos2d::Vec2 valueForPoint(const char *key, std::map<std::string, std::string>& dict)
	{
		return parsePoint(valueForKey(key,dict));
	}
	cocos2d::Size valueForSize(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseSize(valueForKey(key,dict));
	}
	cocos2d::Rect valueForRect(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseRect(valueForKey(key,dict));
	}
	RandFloat valueForRandFloat(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseRandFloat(valueForKey(key,dict));
	}
	RandPos valueForRandPos(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseRandPos(valueForKey(key,dict));
	}
	void valueForFloat4(float* fValue,const char *key, std::map<std::string, std::string>& dict)
	{
		std::string kStr = valueForKey(key,dict);
		std::vector<std::string> kVec = utility::split(kStr,":");
		float fTempValue = 0;
		int iIdex = 0;
		for (iIdex = 0;iIdex<4 && iIdex<(int)kVec.size();iIdex++)
		{
			fValue[iIdex] = parseInt(kVec[iIdex]);
			fTempValue = fValue[iIdex];
		}
		for (;iIdex<4;iIdex++)
		{
			fValue[iIdex] = fTempValue;
		}
	}
	void valueForInt4(int* iValue,const char *key, std::map<std::string, std::string>& dict)
	{
		std::string kStr = valueForKey(key,dict);
		std::vector<std::string> kVec = utility::split(kStr,":");
		CCAssert(kVec.size() != 0,"");
		int iTempValue = 0;
		int iIdex = 0;
		for (iIdex = 0;iIdex<4 && iIdex<(int)kVec.size();iIdex++)
		{
			iValue[iIdex] = parseInt(kVec[iIdex]);
			iTempValue = iValue[iIdex];
		}
		for (;iIdex<4;iIdex++)
		{
			iValue[iIdex] = iTempValue;
		}
	}


	void setSpriteScaleBySize(cocos2d::Node* pSprite,cocos2d::Size kSize)
	{
		if (!pSprite)
		{
			return;
		}
		float fScaleX = 0;
		float fScaleY = 0;
		fScaleX = kSize.width/(float)pSprite->getContentSize().width;
		fScaleY = kSize.height/(float)pSprite->getContentSize().height;
		pSprite->setScaleX(fScaleX);
		pSprite->setScaleY(fScaleY);
	}
	void StringReplace(std::string &strBase, std::string strSrc, std::string strDes,bool bReplaceAll)  
	{  
		std::string::size_type pos = 0;  
		std::string::size_type srcLen = strSrc.size();  
		std::string::size_type desLen = strDes.size();  
		pos=strBase.find(strSrc, pos);   
		while (pos != std::string::npos)
		{  
			strBase.replace(pos, srcLen, strDes);  
			pos=strBase.find(strSrc, (pos+desLen)); 

			if (!bReplaceAll)
			{
				break;
			}
		}  
	}
	void CCAssertWithDes(bool bAssert,std::string kDes1,std::string kDes2)
	{
		if (bAssert)
		{
			return;
		}
		std::string kDes = kDes1 + kDes2;
		CCAssert(bAssert,kDes.c_str());
	}
	float getMoveTime(cocos2d::Vec2 kSrc,cocos2d::Vec2 kDest,float fSpd)
	{
		if (fSpd <= 0.00f)
		{
			return 0.f;
		}
		return kSrc.distance(kDest)/fSpd;
	}

	float getMoveTime(float fSrc,float fDest,float fSpd)
	{
		if (fSpd <= 0.00f)
		{
			return 0.f;
		}
		return fAbs(fSrc-fDest)/fSpd;
	}
	cocos2d::Vec2 getDistancePos(cocos2d::Vec2 kSrc,cocos2d::Vec2 kDest,float fDistance)
	{
		if (kSrc.x == kDest.x && kSrc.y == kDest.y)
		{
			return kSrc;
		}
		if (fDistance < 0.001f)
		{
			return kSrc;
		}
		cocos2d::Vec2 kDistance = (kDest - kSrc);
		float fTempDistacne = kDistance.getLength();
		if (fTempDistacne < 0.001f)
		{
			return kSrc;
		}
		cocos2d::Vec2 kReturn = kSrc;
		kReturn.x += kDistance.x/fTempDistacne*fDistance;
		kReturn.y += kDistance.y/fTempDistacne*fDistance;

		return kReturn;
	}
	bool MoveTo(float fSrc,float fDest,float& fkOut,float fDistance)
	{
		float fDis = fAbs(fSrc - fDest);
		if (fDis < 0.001f)
		{
			fkOut = fDest;
			return true;
		}
		float fPercent = fDistance/fDis;
		if (fPercent>=1.0f)
		{
			fkOut = fDest;
			return false;
		}
		fkOut = fSrc + (fDest - fSrc)*fPercent;
		return false;
	}
	bool MoveTo(cocos2d::Vec2 kSrc,cocos2d::Vec2 kDest,cocos2d::Vec2& kOut,float fDistance)
	{
		float fDis = kSrc.getDistance(kDest);
		if (fDis < 0.0001f)
		{
			kOut = kDest;
			return true;
		}
		float fPercent = fDistance/fDis;
		if (fPercent>=1.0f)
		{
			kOut = kDest;
			return true;
		}
		kOut.x = kSrc.x + (kDest.x - kSrc.x)*fPercent;
		kOut.y = kSrc.y + (kDest.y - kSrc.y)*fPercent;
		return false;
	}
	cocos2d::Vec2 getAnglePos(cocos2d::Vec2 kSrc,cocos2d::Vec2 kDest,float fAngle)
	{
		if (kSrc.x == kDest.x && kSrc.y == kDest.y)
		{
			return kSrc;
		}
		cocos2d::Vec2 kDistance = (kDest - kSrc);
		float fDistance = kDistance.getLength();
		float fTempAngle = kDistance.getAngle() - fAngle;
		cocos2d::Vec2 kReturn = kSrc;
		kReturn.x += fDistance*cos(fTempAngle);
		kReturn.y += fDistance*sin(fTempAngle);
		return kReturn;
	}
	cocos2d::Vec2 getPosByDistanceAngle(float fDistance,float fRoata)
	{
		cocos2d::Vec2 kReturn;
		kReturn.x = fDistance*cos((90 - fRoata)/180.f*3.14f);
		kReturn.y = fDistance*sin((90 - fRoata)/180.f*3.14f);
		return kReturn;
	}
	cocos2d::Vec2 getPosByAngle(cocos2d::Vec2 kDest,float fRoata)
	{
		float fDistance = kDest.getLength();
		float fAngle = kDest.getAngle();
		cocos2d::Vec2 kReturn;
		kReturn.x = fDistance*cos((90 - fRoata)/180.f*3.14f + fAngle - 3.14/2.0f);
		kReturn.y = fDistance*sin((90 - fRoata)/180.f*3.14f + fAngle - 3.14/2.0f);
		return kReturn;
	}
	bool haveInVector(const std::vector<int>& kList,int iIdex)
	{
		std::vector<int>::const_iterator itor = kList.begin();
		for (;itor != kList.end();itor++)
		{
			if (*itor == iIdex)
			{
				return true;
			}
		}
		return false;
	}
	void saveFile(const char *pContent, std::string pFileName, int length){    
		std::string path = pFileName;    
		FILE* file = fopen(path.c_str(), "wb");   

		if (file) {   
			fwrite(pContent, sizeof(char), length, file);  
			fclose(file);    
		}     
	}  
	std::string AddFuHao(int iNum)
	{
		if (iNum >= 0)
		{
			return utility::toString("+",iNum);
		}
		return utility::toString(iNum);

	}
	void DelIntVectorByIdex(int iIdex,std::vector<int>& kVectors)
	{
		if (iIdex >= (int)kVectors.size())
		{
			CCAssert(false,"");
			return;
		}
		std::vector<int>::iterator itor = kVectors.begin();
		for (;itor != kVectors.end();itor++)
		{
			if (iIdex == 0)
			{
				kVectors.erase(itor);
				break;
			}
			iIdex--;
		}
	}
	void DelIntVectorByValue(int iValue,std::vector<int>& kVectors)
	{
		std::vector<int>::iterator itor = kVectors.begin();
		while (itor != kVectors.end())
		{
			if (iValue == *itor)
			{
				itor = kVectors.erase(itor);
			}
			else
			{
				itor++;
			}
		}
	}

	float getOffectByLR(bool bLeft,float fDistance)
	{
		if (bLeft)
		{
			return -fDistance;
		}
		return fDistance;
	}

	std::string EncodeURL(const std::string &URL )
	{
		std::string src = URL;
		char hex[] = "0123456789ABCDEF";
		std::string dst;


		for (size_t i = 0; i < src.size(); ++i)
		{
			unsigned char cc = src[i];
			if ( cc >= 'A' && cc <= 'Z' 
				|| cc >='a' && cc <= 'z'
				|| cc >='0' && cc <= '9'
				|| cc == '.'
				|| cc == '_'
				|| cc == '-'
				|| cc == '*')
			{
				if (cc == ' ')
				{
					dst += "+";
				}
				else
					dst += cc;
			}
			else
			{
				unsigned char c = static_cast<unsigned char>(src[i]);
				dst += '%';
				dst += hex[c / 16];
				dst += hex[c % 16];
			}
		}
		return dst;

	}
	std::string DecodeURL(const std::string &URL)
	{
		std::string result;  
		int hex = 0;  
		for (size_t i = 0; i < URL.length(); ++i)  
		{  
			switch (URL[i])  
			{  
			case '+':  
				result += ' ';  
				break;  
			case '%':  
				if (isxdigit(URL[i + 1]) && isxdigit(URL[i + 2]))  
				{  
					std::string hexStr = URL.substr(i + 1, 2);  
					hex = strtol(hexStr.c_str(), 0, 16);  
					//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]  
					//可以不经过编码直接用于URL  
					if (!((hex >= 48 && hex <= 57) || //0-9  
						(hex >=97 && hex <= 122) ||   //a-z  
						(hex >=65 && hex <= 90) ||    //A-Z  
						//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
						hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29  
						|| hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f  
						|| hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f  
						))  
					{  
						result += char(hex);  
						i += 2;  
					}  
					else result += '%';  
				}else {  
					result += '%';  
				}  
				break;  
			default:  
				result += URL[i];  
				break;  
			}  
		}  
		return result;  
	}
	int getVectorIdex(std::vector<int>& kVector,int iValue)
	{
		for (int i = 0;i<(int)kVector.size();i++)
		{
			if (kVector[i] == iValue)
			{
				return i;
			}
		}
		return -1;
	}

	std::string lineFeeds( const std::string &str, int iCurrentIndex )
	{
		if (typeid(str) == typeid(std::string) && str.length() > 0 &&iCurrentIndex > 0) {
			std::stringstream result;
			std::vector<std::string> temp;
			temp = parseUTF8(str); 
			int iStringLength = temp.size();
			for (int i = 0;i < iStringLength;i++) {
				result<<temp[i];
				if (iCurrentIndex == i)
					result<<"\n";
			}
			return result.str();
		} else {
			return "";
		}
	}

	std::vector<std::string> parseUTF8( const std::string &str )
	{
		int l = str.length();
		std::vector<std::string> ret;
		ret.clear();
		for(int p = 0; p < l; ) {
			int size = 1;
			unsigned char ch = str[p]; 
			if (ch >=0x00 && ch <=0x7F) 
			{  
				size = 1;  
			}
			else if(ch >=0xC0 && ch <=0xDF)  
			{  
				size =2; 
			}  
			else if(ch >=0xE0 && ch <=0xEF)  
			{  
				size =3;  
			}  
			else if(ch >=0xF0 && ch <=0xF7)  
			{  
				size =4;  
			}  
			else if(ch >=0xF8 && ch <=0xFB)  
			{  
				size =5;   
			}  
			else if(ch >=0xFC && ch <=0xFD)  
			{  
				size =6;  
			}
			else
			{
				size =1;
			}
			std::string temp = "";
			temp = str.substr(p, size);
			ret.push_back(temp);
			p += size;
		}
		return ret;
	}

	bool isNumAZ( const std::string& str)
	{
		int l = str.length();
		for(int p = 0; p < l;p++) 
		{
			if ((str[p] >= '0' && str[p] <= '9') ||
				(str[p] >= 'a' && str[p] <= 'z') ||
				(str[p] >= 'A' && str[p] <= 'Z'))
			{
				continue;
			}
			return false;
		}
		return true;
	}
	std::string subUTF8( const std::string &str,int from, int to )
	{
		if(from > to) return "";
		std::vector<std::string> test = parseUTF8(str);
		if ((int)test.size() < to) return str;
		std::vector<std::string>::iterator iter = test.begin();
		std::string res;
		std::string result;
		for(iter=(test.begin() + from); iter != (test.begin() + to); iter++)
		{
			res += *iter;

		}
		return res;
	}

	std::string getSameSizeValue10(int iValue)
	{
		if (iValue < 10)
		{
			return utility::toString("0",iValue);
		}
		return utility::toString(iValue);
	}
	cocos2d::Vec2 getRandPos(const RandPos& kRand,cocos2d::Node* pNode)
	{
		if (kRand.kUserKey != "")
		{
			return utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,kRand.kUserKey));
		}
		float fValueX = getRandFloat(kRand.kRandX,pNode);
		float fValueY = getRandFloat(kRand.kRandY,pNode);
		return cocos2d::Vec2(fValueX,fValueY);
	}
	float getRandFloat(const RandFloat& kRand,cocos2d::Node* pNode)
	{
		if (kRand.kUserKey != "")
		{
			return utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,kRand.kUserKey));
		}
		int iRand = (int)((kRand.fMax-kRand.fMin)*10);
		if (iRand == 0)
		{
			return kRand.fMin;
		}
		if (iRand < 0)
		{
			return kRand.fMax+(float)(rand()%(-iRand)/10.0f);
		}
		
		return kRand.fMin+(float)(rand()%(iRand)/10.0f);
	}

	float getRotation(cocos2d::Vec2 kSrcPos,cocos2d::Vec2 kDestPoint)
	{
		cocos2d::Vec2 kDestTance = kDestPoint - kSrcPos;
		float fAngle = kDestTance.getAngle();
		return (-fAngle*180.0f/3.14f);
	}
	cocos2d::Vec2 getOffectParentPos(cocos2d::Node* pNode,cocos2d::Vec2 kOffect)
	{
		cocos2d::Mat4 tmp = pNode->getNodeToParentTransform();
		tmp.m[12] = 0;
		tmp.m[13] = 0;
		cocos2d::Vec3 vec3(kOffect.x, kOffect.y, 0);
		cocos2d::Vec3 ret;
		tmp.transformPoint(vec3,&ret);
		return cocos2d::Vec2(ret.x, ret.y);

	}

	void schedule(cocos2d::SEL_SCHEDULE selector,cocos2d::Ref* pTarget)
	{
		cocos2d::Director::getInstance()->getScheduler()->schedule(
			selector,pTarget,0.0f,false);
	}
	void unschedule(cocos2d::SEL_SCHEDULE selector,cocos2d::Ref* pTarget)
	{
		cocos2d::Director::getInstance()->getScheduler()->unschedule(selector,pTarget);
	}
	void scheduleOnce(cocos2d::SEL_SCHEDULE selector,cocos2d::Ref* pTarget,float interval)
	{
		cocos2d::Director::getInstance()->getScheduler()->schedule(selector, pTarget, interval, 0, 0.0f, false);
	}
	std::string getScriptReplaceValueStr(std::string kSrc,std::string kValue1,std::string kValue2,std::string kValue3)
	{
		std::string kScriptStr = ScriptData<std::string>(kSrc.c_str()).Value();
		if (kValue1 != "")
		{
			utility::StringReplace(kScriptStr,"#",kValue1);
		}
		if (kValue2 != "")
		{
			utility::StringReplace(kScriptStr,"#",kValue2);
		}
		if (kValue3 != "")
		{
			utility::StringReplace(kScriptStr,"#",kValue3);
		}
		return kScriptStr;
	}
	std::string getShortGoldValue(int iGold)
	{
		std::string kFuHao = "";
		if (iGold < 0)
		{
			iGold = -iGold;
			kFuHao = "-";
		}
		std::string kTempTxt;
		std::string kGoldStr = utility::toString(iGold);
		int iCout = 0;
		int iTempGold = iGold;
		int iLastValue = 0;
		while (iTempGold >= 10)
		{
			iTempGold = iTempGold/10;
			iCout ++;
		}
		if (iCout > 7)
		{
			iTempGold = iGold/100000000;
			iLastValue = (iGold/10000)%10000;
			static ScriptData<std::string> s_kTxt("PlayerGoldTxt2");
			kTempTxt = s_kTxt.Value();
		}
		else if (iCout > 3)
		{
			iTempGold = iGold/10000;
			iLastValue = iGold%10000;
			static ScriptData<std::string> s_kTxt("PlayerGoldTxt3");
			kTempTxt = s_kTxt.Value();
		}
		else
		{
			return utility::toString(kFuHao,iGold);
		}
		std::string kTxt1 = utility::toString(iTempGold);
		std::string kTxt2 = utility::toString(iLastValue);
		if (iLastValue == 0)
		{
			kTxt1.push_back('.');
			kTxt1.push_back('0');
		}
		else
		{
			int iSize = kTxt2.size();
			for (int i = 0;i<4-iSize;i++)
			{
				kTxt2 = utility::toString("0",kTxt2);
			}
			int iTempCout1 = kTxt1.size();
			if (iTempCout1 < 4)
			{
				kTxt1.push_back('.');
				for (int i = 0;i<4-iTempCout1 && i < (int)kTxt2.size();i++)
				{
					kTxt1.push_back(kTxt2[i]);
				}
			}
		}
		return utility::toString(kFuHao,kTxt1,kTempTxt);
	}
	void StringToChar(std::string kStringValue,char* pChar,int iSize)
	{
		CCASSERT(kStringValue.size() < iSize,"");
		strcpy(pChar,kStringValue.c_str());
	}
	void CharToString(char* pChar,std::string& kStringValue)
	{
		kStringValue = std::string(pChar);
	}
	std::string stringReplaceEnter(std::string kStr,std::string kReplace)
	{
		char kTemp = '\n';
		std::string kStrTemp;
		kStrTemp.push_back(kTemp);
		utility::StringReplace(kStr,kReplace,kStrTemp,true);;
		return kStr;
	}
	std::string paseFloat(float fValue,int iCout)
	{
		char kTxt[12];
		std::string kTemp = utility::toString("%.",iCout,"f");
		sprintf(kTxt,kTemp.c_str(),fValue);
		return kTxt;
	}
	std::string paseInt(int iValue,int iCout)
	{
		char kTxt[12];
		std::string kTemp = utility::toString("%0",iCout,"d");
		sprintf(kTxt,kTemp.c_str(),iValue);
		return kTxt;
	}

	bool IsSpriteInTouch(cocos2d::Vec2 kWorldPos, cocos2d::Sprite *pSrite)
	{
		kWorldPos = pSrite->convertToNodeSpace(kWorldPos);
		cocos2d::Size elementSize = pSrite->getContentSize();  
		cocos2d::Rect elementRect;

		cocos2d::Vec2 kAnchor = pSrite->getAnchorPoint();

		elementRect = cocos2d::Rect(
			0,0,
			elementSize.width,        
			elementSize.height
			);

		return elementRect.containsPoint(kWorldPos); 
	}

	std::string NumToStringMyriabit( int nNum,int nPointNum /*= 1*/ )
	{
		static ScriptData<std::string> s_kTxt("Wan");
		std::string kStr;
		if (nNum<10000)
		{
			kStr = utility::toString(nNum);
		}
		else 
		{
			int nWan = nNum/10000;
			int nYu = nNum%10000;
			int nQian = nYu/(10000/pow(10,nPointNum));

			if (nQian == 0)
			{
				kStr = utility::toString(nWan,s_kTxt.Value());
			}
			else
			{
				kStr = utility::toString(nWan,".",nQian,s_kTxt.Value());
			}

		}
		return (kStr);
	}

	bool isAllVisible(cocos2d::Node* pNode)
	{
		while (pNode)
		{
			if (!pNode->isVisible())
			{
				return false;
			}
			pNode = pNode->getParent();
		}
		return true;
	}
	void runPaoMaDeng(cocos2d::Node* pRootNode,std::string kTxtNode,std::string kLayoutNode,std::string kTxt,cocos2d::FiniteTimeAction* pNetAction)
	{
		cocos2d::Node* pTxt = WidgetFun::getChildWidget(pRootNode,kTxtNode);
		cocos2d::Node* pLayoutNode = WidgetFun::getChildWidget(pRootNode,kLayoutNode);
		WidgetFun::setText(pTxt,kTxt);
		float fMaxHeight = pLayoutNode->getContentSize().height;
		float fMaxWidth = pLayoutNode->getContentSize().width;
		float fTxtWidth = pTxt->getContentSize().width;
		pTxt->setPosition(0,-fMaxHeight);
		pTxt->stopAllActions();
		pTxt->setVisible(true);
		if (fMaxWidth > fTxtWidth)
		{
			cocos2d::CCAction * seq1 = cocos2d::CCSequence::create(
				cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,0)),
				cocos2d::CCDelayTime::create(6.0f), 
				cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,fMaxHeight)),
				cocos2d::CCVisibleAction::create(0.2f,false),
				pNetAction,
				NULL);
			pTxt->runAction(seq1);
		}
		else
		{
			float fPosX = fMaxWidth-fTxtWidth-10;
			cocos2d::CCAction * seq1 = cocos2d::CCSequence::create( 
				cocos2d::MoveTo::create(0.5f,cocos2d::Point(0,0)),
				cocos2d::CCDelayTime::create(3.0f), 
				cocos2d::MoveTo::create(2.0f,cocos2d::Point(fPosX,0)),
				cocos2d::CCDelayTime::create(3.0f), 
				cocos2d::MoveTo::create(0.5f,cocos2d::Point(fPosX,fMaxHeight)),
				cocos2d::CCVisibleAction::create(0.2f,false),
				pNetAction,
				NULL);
			pTxt->runAction(seq1);
		}
	}
	std::string getTimeStr()
	{
		time_t t = time(0); 
		char tmp[64]; 
		strftime( tmp, sizeof(tmp), "%Y/%m/%d %H:%M:%S",localtime(&t) ); 
		return tmp;
	}
	std::string getPayTimeStr()
	{
		time_t t = time(0); 
		char tmp[64]; 
		strftime( tmp, sizeof(tmp), "%Y%m%d%H%M%S",localtime(&t) ); 
		return tmp;
	}
	void getGPSValue(std::string kStrValue,int& iValue1,int& iValue2,int& iValue3)
	{
		float fValue = utility::parseFloat(kStrValue);
		iValue1 = fValue;
		iValue2 = (int)(fValue*100)%100;
		iValue3 = (int)(fValue*10000)%100;
	}
	bool isGPS100In(std::string kStrValue1,std::string kStrValue2)
	{
		std::vector<std::string> kVector1 = utility::split(kStrValue1,":");
		std::vector<std::string> kVector2 = utility::split(kStrValue2,":");
		if (kVector1.size() != 2 || kVector2.size() != 2)
		{
			return false;
		}
		cocos2d::Point kPos1,kPos2;
		kPos1.x = utility::parseFloat(kVector1[0]);
		kPos1.y = utility::parseFloat(kVector1[1]);

		kPos2.x = utility::parseFloat(kVector2[0]);
		kPos2.y = utility::parseFloat(kVector2[1]);

		if (kPos1.getDistance(kPos2) < 100.f/24.f/10000.f)
		{
			return true;
		}
		return false;
	}
} 

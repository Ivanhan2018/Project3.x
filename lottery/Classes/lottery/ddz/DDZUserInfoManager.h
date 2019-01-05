//  [3/19/2014 wucan]

#ifndef __DDZ__DDZUserInfoManager__
#define __DDZ__DDZUserInfoManager__

#include "cocos2d.h"
#include "Define.h"
USING_NS_CC;

class DDZUserInfoManager
{

public:
	virtual ~DDZUserInfoManager();
	static DDZUserInfoManager* getInstance();

	void setUserId(DWORD userId);
	void setGender(int gender);
	void setUserName(const std::string &name);
	void setUserGold(LONGLONG gold);

	WORD m_gender;

	DWORD m_userId;

	std::string m_UserName;

	LONG m_UserGold;
private:
	DDZUserInfoManager();
	static DDZUserInfoManager* instance;					// ∂‘œÛ÷∏’Î


};



#endif

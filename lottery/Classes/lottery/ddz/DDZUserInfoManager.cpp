#include "DDZUserInfoManager.h"

DDZUserInfoManager* DDZUserInfoManager::instance = nullptr;					//¶ÔÏóÖ¸Õë

DDZUserInfoManager* DDZUserInfoManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new DDZUserInfoManager();
	}
	return instance;
}

DDZUserInfoManager::DDZUserInfoManager():
	m_gender(0),
	m_UserGold(0),
	m_userId(0),
	m_UserName("")
{

}

DDZUserInfoManager::~DDZUserInfoManager()
{

}

void DDZUserInfoManager::setUserId(DWORD userId)
{
	this->m_userId = userId;
}
void DDZUserInfoManager::setGender(int gender)
{
	this->m_gender = gender;
}
void DDZUserInfoManager::setUserName(const std::string &name)
{
	this->m_UserName = name;
}
void DDZUserInfoManager::setUserGold(LONGLONG gold)
{
	this->m_UserGold = gold;
}
//
//  Login.h
//  Game
//
//  Created by zhouwei on 13-6-17.
//
//

#ifndef __Game__Login__
#define __Game__Login__

#include "BaseObject.h"
#include "GBEvent.h"
#include "GBEventIDs.h"
#include "GlobalDef.h"
#include "cocos2d.h"
#include <iostream>
#include <string>
using namespace std;
using namespace cocos2d;

class Login : public BaseObject,Object
{
public:
    Login();
    ~Login();
    
    void onSocketLogin(GBEventConstArg& arg);
    
    //用户登陆
    void setUserLogin(string account,string password);

    //用户注册
    void setUserRegister(const char* szAccount,const char* szPassword,int nfaceId,int nGender, int visiter);
    
    bool onEventLoginRead(CMD_Command* pCommand,void* pBuffer,WORD wDataSize);
    
    string getAccout() { return m_szAccount; }
private:
    string m_szAccount;
    string m_szPassword;
    
};
#endif /* defined(__Game__Login__) */

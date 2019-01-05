#ifndef __TexasPoker__MessageModel__
#define __TexasPoker__MessageModel__

#include "Inc.h"

class MessageModel
{
public:
    MessageModel();
    ~MessageModel();
    
public:
    unsigned int mid;
    unsigned int type;
    unsigned int date;
    std::string title;
    std::string content;
};

class HronMsgModel:public CCObject
{
public:
    HronMsgModel()
    {
        who="系统:";
		sex=0;
		msg="欢迎来到汇智德州扑克!";
		sysColor = ccc3(255,210,0);
        manColor = ccc3(0,240,255);
        wanColor = ccc3(255,139,232);
    }
    
    ~HronMsgModel()
    {}
    
public:
    std::string        who;
    unsigned int       sex;
    std::string        msg;
    unsigned int       uid;
    ccColor3B          sysColor;
    ccColor3B          manColor;
    ccColor3B          wanColor;
};

class ActiveModel
{
   /*
    status: 1,
    a_type: 2,
    t_type: 1,
    name: "首充活动",
    id: 2,
    starttime: 1385827200,
    endtime: 1391270399,
    info: "活动期间，只要首次充值即可获得相应的奖励，如：首次充值100元，则获得100元的相应奖励；机不可失失不再来！把握机会成就土豪！"
    */    
public:
    unsigned int           status;
    unsigned int           a_type;
    unsigned int           t_type;
    std::string       name;
    unsigned int           id;
    unsigned int          starttime;
    unsigned int          endtime;
    std::string       info;
    
public:
    ActiveModel(){status=0;a_type=0;t_type=0;name="";id=0;starttime=0;endtime=0;info="";}
    ~ActiveModel(){}
};

#endif

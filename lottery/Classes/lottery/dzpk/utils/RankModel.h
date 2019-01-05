#ifndef __TexasPoker__RankModel__
#define __TexasPoker__RankModel__

#include "Inc.h"

class RankModel
{
    
public:
    RankModel();
    RankModel(unsigned int id,unsigned int type,unsigned int uid,std::string name,unsigned int sex,unsigned int no,unsigned int amount);
    ~RankModel();
    
public:
    unsigned int         id;
    unsigned int         type;
    unsigned int         uid;
    std::string          name;
    unsigned int         sex;
    unsigned int         no;
    unsigned int         amount;
};

#endif

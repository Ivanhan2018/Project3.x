#ifndef __TexasPoker__Venue__
#define __TexasPoker__Venue__

#include "Inc.h"

class Venue
{
    
/*
 id: 3,
 ip: "210.14.156.54",
 port: 7703,
 sblind: 5,
 bblind: 10,
 min_money: 100,
 max_money: 2000,
 seats: 9,
 max_member: 15,
 limit: 250000,
 table_id: 39,
 cur_member: 0
*/
    
public:
    Venue();
    ~Venue();
    
public:
    UNS_INT           room_id;
    UNS_INT           table_id;
    std::string       ip;
    UNS_INT           port;
    UNS_INT           sblind;
    UNS_INT           bblind;
    long int          min_money;
    UNS_INT           max_money;
    UNS_INT           seats;
    UNS_INT           max_member;
    UNS_INT           cur_member;
    long int          limit;
    //列表上显示的房间id号，是由上面的 room_id + table_id组合成的
    UNS_INT           showVenueId;
    //快速加注40，50，100，200
    std::string       quickRefuelItems;
    
};

#endif

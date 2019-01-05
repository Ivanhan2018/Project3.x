//
//  Players.h
//  gamePoker
//
//  Created by xavier on 13-11-30.
//
//用于保存进入房间的玩家信息

#ifndef gamePoker_Players_h
#define gamePoker_Players_h

#include <string>
#include <stdint.h>
using namespace std;

class Players
{
public:
    Players();
    virtual ~Players();
   
    static Players *getInstance();
    static int getSeatidByUid(int u_id);
    static int convertToStatus(int status);
    const char* convertToChipNum(int count);
    static string getNameByUid(int u_id);
    int getOthersMostChip(int chip);
    int init();
    int reset();

public:
    bool                    is_player;
    string                  avatar;
    string                  birthday;
    int                     coin;
    int                     max_chip;
    string                  contact;
    int                     exp;
    int                     money;
    string                  name;
    int                     pcount;
    string                  ps;
    int                     rmb;
    int                     seatid;
    int                     sex;
    int                     uid;
    int                     total_board;
    int                     total_win;
    int                     vlevel;
    int                     vtime;
    string                  zone;
    
    string                  best_cards[5];
    int raise[5];
};



#endif

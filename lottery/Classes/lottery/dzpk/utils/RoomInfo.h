//
//  RoomInfo.h
//  gamePoker
//
//  Created by xavier on 13-12-9.
//
//

#ifndef gamePoker_RoomInfo_h
#define gamePoker_RoomInfo_h

#include <string>
#include <stdint.h>
using namespace std;

//用于保存坐在桌子上的玩家
class RoomInfo
{
public:
    static RoomInfo *getInstance();
    void init ();
    
    int bb;
    string comm_card[5];
    int cur_players;
    int dealer;
    string my_cards[2];
    int round_state;
    int sb;
    
    int s_bet;
    int b_bet;
    
    int min_take;
    int max_take;
    
    int port;
    
    string ip;
    
    int tid;
    
    int tableid;
    
    int roomNum;
    
    int Wifi;
    
    int max_limit;
    
    int play_count;
    
    int rmid;
    
    int stake;
    
    string time;
    
    string deskType;
    
    bool isGamming;
    
    int quick_addBet[5];
};

#endif

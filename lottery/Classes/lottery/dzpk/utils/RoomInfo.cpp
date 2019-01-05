//
//  RoomInfo.cpp
//  gamePoker
//
//  Created by xavier on 13-12-9.
//
//

#include "RoomInfo.h"

RoomInfo *roomInfo;

RoomInfo * RoomInfo::getInstance()
{
    if (roomInfo==NULL)
    {
        roomInfo = new RoomInfo();
        roomInfo->init();
    }
    
    return roomInfo;
}


void RoomInfo::init()
{
    bb=0;              
    comm_card[0]="00";
    comm_card[1]="00";
    comm_card[2]="00";
    comm_card[3]="00";
    comm_card[4]="00";
    
    cur_players=0;      
    dealer=-1;
    my_cards[0]="00";
    my_cards[1]="00";
    
    round_state=0;     
    sb=0;
    
    s_bet=50;
    stake=0;
    
    max_limit=0;
    roomNum=0;
    play_count=0;
    
    isGamming=false;
    
    tableid=0;
    
    time = "00:00";
    
    Wifi = 1;
    
    deskType=1;
    
    quick_addBet[0] = 100;
    quick_addBet[1] = 200;
    quick_addBet[2] = 400;
    quick_addBet[3] = 800;
    quick_addBet[4] = 1000;
}


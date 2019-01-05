//
//  Players.cpp
//  gamePoker
//
//  Created by xavier on 13-11-30.
//
//

#include "Players.h"
#include "cocos2d.h"
//#include "RoomScene.h"

using namespace cocos2d;


static Players players[50];

Players::Players()
{
    init();
}

Players::~Players()
{
    
}

Players* Players::getInstance()
{
    return players;
}

int Players::init()
{

    is_player=false;
    
    avatar="mm.png";
    birthday="00/00/00";
    max_chip=0;
    coin=0;                 
    contact="qq";              
    exp=0;                  
    money=0;                
    name="xavier";                 
    pcount=0;              
    ps="1234";                  
    rmb=0;                  
    seatid=-1;               //座位id
    sex=1;                  // 性别 1：女 2：男
    uid=0;                  // user_id
    total_board=0;          // 玩牌总次数
    total_win=0;            // 胜利总次数
    vlevel=0;               // vip等级
    vtime=0;                // vip过期时间
    zone="sz";              //地区
    
    
    raise[0]=0;
    raise[1]=0;
    raise[2]=0;
    raise[3]=0;
    raise[4]=0;
    
    best_cards[0]="00";
    best_cards[1]="00";
    best_cards[2]="00";
    best_cards[3]="00";
    best_cards[4]="00";
    
    return 0;
}

int Players::getSeatidByUid(int u_id)
{
    for (int i=0;i<50;i++)
    {
        if (players[i].uid == u_id)
        {
            return players[i].seatid;
            break;
        }
    }
    return -1;
}

string Players::getNameByUid(int u_id)
{
    for (int i=0;i<50;i++)
    {
        if (players[i].uid == u_id)
        {
            return players[i].name;
            break;
        }
    }
    return "0";
}

int Players::convertToStatus(int status)
{
    switch (status) {
        case 0:
            break;
        case 1:
            return 3;
            break;
        case 2:
            return 1;
            break;
        case 3:
            return 2;
            break;
        case 4:
            return 11;
            break;
        case 5:
            return 10;
            break;
        case 6:
            return 4;
            break;
        case 7:
            return 5;
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            return 12;
            break;
        case 11:
            return 13;
            break;
        default:
            break;
    }
    return 0;
}

const char* Players::convertToChipNum(int count)
{
    
    if (count<1000000)
        return CCString::createWithFormat("%d",count)->getCString();
    
    
    return CCString::createWithFormat("%.1fK",(float)count/1000)->getCString();
}

int Players::getOthersMostChip(int chip)
{
    int maxChip=0;
    for (int i=0;i!=9;++i){
        //if (i!=RoomScene::getMySeatID() && RoomScene::getRff()[i].own_gold+RoomScene::getRff()[i].bet>maxChip){
        //    maxChip = RoomScene::getRff()[i].own_gold+RoomScene::getRff()[i].bet;
        //    CCLog("maxChip=%d,i=%d",maxChip,i);
        //}
    }

    return MIN(chip, maxChip);
}






#include "RoomPlayer.h"



RoomPlayer::RoomPlayer()
{
    reset();
}


void RoomPlayer::reset()
{
	is_start = false;
	user_id = -1 ;
	seatID = -1;
	is_banker = false;
    left_time = 0;

    bborsb=0;
    is_player = 0;
    player_status = 12;
    auto_cmd = 0;
    
    own_gold = 0;
    total_gold=0;
	name = "xavier" ;      

	current_round = 1 ; 
	min_bet = 0;
	min_raise = 0;
	s_bet = 0;
    bet=0;

	card_one = "00";
	card_two = "00";
	
    openCard_times = 1;
    
    winCards[0]="00";
    winCards[1]="00";
    winCards[2]="00";
    winCards[3]="00";
    winCards[4]="00";
    
    winCardType=0;
    
    sex=1;
}

void RoomPlayer::setStatus(int status)
{
	player_status=status;
}

void RoomPlayer::setGold(int num)
{
	own_gold=num;
}

void RoomPlayer::setRound(int round)
{
	current_round = round;
}
//1.看牌 2.跟注 3.弃牌 4.加注 5.全下 6.自动弃牌 7.看或弃牌 8.跟任何注 , 10.下注中, 11.初始化, 12.表示没人 , 13.等待下一轮
string RoomPlayer::getTitleByStatus(int status , string userName)
{
	switch (status)
	{
		case 1:return "看牌";
		case 2:return "跟注";
		case 3:return "弃牌";
		case 4:return "加注";
		case 5:return "全下";
		case 6:return "自动弃牌";
		case 7:return "看或弃牌";
		case 8:return "跟任何注";
        case 10:return "下注中";
        case 11:
            /*
            if (strlen(userName.c_str())>5){
                userName = userName.substr(0,4)+"..";
            }*/
            return userName;
        case 13:return "等待下一轮";
	}
	return "";
}

string RoomPlayer::getCardType(int cardType)
{
   	switch (cardType)
	{
        case 0:return "高牌";
		case 1:return "一对";
		case 2:return "两对";
		case 3:return "三条";
		case 4:return "顺子";
		case 5:return "同花";
		case 6:return "葫芦";
		case 7:return "四条";
        case 8:return "同花顺";
        default:
            break;
	}
	return "";
}
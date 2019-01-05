#ifndef __RoomPlayer_H__
#define __RoomPlayer_H__

#include <string>
#include <stdint.h>
using namespace std;

//用于保存坐在桌子上的玩家
class RoomPlayer 
{
	public:
	bool                is_start; //是否在游戏中，false则游戏重新发牌开始。
	int					user_id;    
	int					seatID;   //从0到8
	bool                is_banker;  //庄家
    int                 bborsb; //小盲注或者大盲注 0:不是 1小盲注 2大盲注
    
    int                 is_player; //1有人0没人
    int                 player_status; // 1.看牌 2.跟注 3.弃牌 4.加注 5.全下 6.自动弃牌 7.看或弃牌 8.跟任何注 , 10.操作中, 11.初始化, 12.表示没人 , 13.等待下一轮
    
    int                 left_time; //剩余操作时间

    int                 auto_cmd;
    int                 own_gold; //当前携带筹码
    int                 total_gold; //玩家拥有的总筹码
    string              name;        //玩家名

	int					current_round; //当前回合数，若回合是0，只能跟注
	int                 min_bet;  //至少跟注
	int					min_raise; //最小加注
	int                 s_bet; //初始化房间时，玩家下在桌面上的注
    int                 bet;//本次玩家已经下注数量
	
	string                 card_one; //玩家拿到的第一张牌
	string                 card_two;
    
    int                 openCard_times; //第几次翻牌
    
    string              winCards[5];
    
    int                 winCardType;
    
    int                 sex;

public:
	RoomPlayer();

	static string getTitleByStatus(int status, string userName);
    
    static string getCardType(int cardType);

	void setStatus(int status);

	void setGold(int num);

	void setChipin(int count);

	void setRound(int round);

	void reset();
};

#endif // __RoomPlayer_H__

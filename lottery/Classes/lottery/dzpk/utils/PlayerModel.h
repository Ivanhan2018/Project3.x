#ifndef __Game_PlayerModel_H__
#define __Game_PlayerModel_H__

#include "Inc.h"

class Venue;
class AppModel;
class RankModel;
class MessageModel;
class HronMsgModel;
class ActiveModel;
class RechargeModel;
class GoodsModel;

class PlayerModel : public cocos2d::CCObject
{

public:
	/*
	字段解释
	user：账户
	name：昵称
	sex：性别
	birthday: 生日
	zone: 地区
	contact: 联系方式
	ps: 个性签名
	avatar: 头像URL
	money：筹码
	rmb：人民币
	coin：元宝
	skey：登陆通行证
	login_days：连续登陆天数
	is_get: 是否领取连续登陆
	total_board：总玩牌次数
	total_win：总赢牌次数
	best_cards: 最好5张牌
	exp：经验值
	vtime: V标志过期时间
	vleve: V标志等级
	uid：用户id
	password: 密码
    max_ship: 最高获得财富
    solt_num: 老虎机，是否有免费的摇杆机会，0表示没有，1表示有
    cur_time: 当前服务器时间
    broke_num:破产次数
    auto_register:0 1为自动注册账号，0为正式账号
	*/
	UNS_INT                 uid;
    std::string             user;
    std::string             best_cards[5];
    std::string             pwd;
	std::string			    name;
	UNS_INT			        sex;
	std::string			    avatar;
	std::string			    birthday;
	std::string			    zone;
	UNS_LONG			    exp;
	UNS_LONG			    rmb;
	long int                money;
    std::string             skey;
	UNS_INT				    total_board;
	UNS_INT				    total_win;
	UNS_INT				    pcount;
	UNS_LONG			    vtime;
	UNS_INT				    vlevel;
	UNS_INT                 login_days;
	bool                    is_get_login_packer;
    UNS_INT                 first_recharge_coin;
	bool                    is_login_succ;
    //1.资产榜
    //排名/玩家/拥有筹码
    //2.赢取榜
    //排名/玩家/昨日赢筹码
    //3.充值榜
    //排名 /玩家/昨日充钻石
    std::vector<RankModel>  rankList_1;
    std::vector<RankModel>  rankList_2;
    std::vector<RankModel>  rankList_3;
    //房间列表,1新手场 2初级场 3中级场 4高级级场
    std::vector<Venue>      venueList_1;
    std::vector<Venue>      venueList_2;
    std::vector<Venue>      venueList_3;
    std::vector<Venue>      venueList_4;
    std::vector<Venue>      venueList_5;
    //是否要刷新房间列表，跟服务器返回的数据进行比较，相同就不刷新，不相同就刷新
    int                     servVenueStatus;
    int                     localVenueStatus;
    //0系统消息,1用户消息
    std::vector<MessageModel> msgList_0;
    std::vector<MessageModel> msgList_1;
    //活动列表
    std::vector<ActiveModel>  activeList;
    //大喇叭信息列表
    CCArray*                hronMsgList;
    //购买钻石（人民币）列表
    std::vector<GoodsModel> rmbGoodsList;
    //商品列表
    std::vector<GoodsModel> goodsList;
    //限购商品列表
    std::vector<GoodsModel> limitGoodsList;
    //当前购买限购商品的数量
    int                     currBuyLimitGoodsCount;
    //app信息
    AppModel*               appModel;
    //0表示没更新，1表示有更新
    UNS_INT                 isUpdate;
    UNS_INT                 max_ship;
    UNS_INT                 broke_num;
    UNS_INT                 solt_num;
    double                  cur_time;
    double                  cur_time_s;
    //兑奖卷数量
    UNS_INT                 reward_card_count;
    //音乐，音效，震动开关，音量
    bool                    vocieSwitch;
    bool                    vocieEffectSwitch;
    bool                    vocieSnakeSwitch;
    float                   sound;
    //0手动注册用户(正式用户)，1自动注册用户(游客)
    UNS_INT                 auto_register;
    //平台id
    UNS_INT                 chanel_id;
    //是否是购买的限购商品，默认不是
    bool                    isPayLimitGoods;

private:
	PlayerModel();

public:
	static PlayerModel* getInstance();
	~PlayerModel();
    
    int getLevelByExp(int exp);
    float getCurrExpPercentage(int& nextExp,int currExp);
    
    void getBrokeChip();
    
    void getBrokeChipCallBack(float dt);
    
    void reset();
    
};

#endif
#include "PlayerModel.h"
#include "Venue.h"
#include "AppModel.h"
#include "RankModel.h"
#include "MessageModel.h"
#include "GoodsModel.h"
#include "DzConfig.h"

//exp array
static int expArray[100] = {
    25,61,112,185,290,440,655,963,1403,2033,
    2935,4226,6074,8720,12507,17927,25685,36790,48889,62071,
    76433,92081,109130,127705,147943,169993,194017,220192,248710,279781,
    313634,350518,390704,434488,482191,534165,590792,652489,719709,792947,
    872742,959681,1054403,1157605,1270047,1392555,1526031,1671456,1829900,2002529,
    2190613,2395535,2618803,2862059,3127093,3415855,3730468,4073247,4446714,4853615,
    5296945,5775280,6288660,6837085,7420555,8039070,8692630,9381235,10104885,10863580,
    11657320,12486105,13349935,14248810,15182730,16151695,17155705,18194760,19268860,20378005,
    21522195,22701430,23915710,25165035,26449405,27768820,29123280,30512785,31937335,33396930,
    34891570,36421255,37985985,39585760,41220580,42890445,44595355,46335310,48110310,49920355
};

static PlayerModel* instance = NULL;

PlayerModel::PlayerModel():
uid(0),
user(""),
pwd(""),
name(""),
sex(0),
avatar(""),
birthday(""),
zone(""),
exp(0),
rmb(0),
money(0),
skey(""),
total_board(0),
total_win(0),
pcount(0),
vtime(0),
vlevel(0),
login_days(1),
is_get_login_packer(false),
first_recharge_coin(0),
is_login_succ(false),
sound(0.5f),
appModel(NULL),
isUpdate(0),
max_ship(0),
solt_num(0),
cur_time(0),
cur_time_s(0),
reward_card_count(0),
vocieSwitch(true),
vocieEffectSwitch(true),
vocieSnakeSwitch(false),
currBuyLimitGoodsCount(0),
broke_num(0),
auto_register(1),
servVenueStatus(-1),
localVenueStatus(-2),
chanel_id(0),
isPayLimitGoods(false)
{
    for (int i=0; i<5; ++i)
    {
        best_cards[i]="3c";
    }
    
    hronMsgList = CCArray::create();
    CC_SAFE_RETAIN(hronMsgList);
    hronMsgList->addObject(new HronMsgModel);
}

PlayerModel::~PlayerModel()
{    
	if (NULL != instance)
	{
		delete instance;
        instance = NULL;
	}
    
	if (NULL != appModel)
	{
		delete appModel;
        appModel = NULL;
	}
    
    CC_SAFE_RELEASE_NULL(hronMsgList);
}

PlayerModel* PlayerModel::getInstance()
{
   if (NULL == instance)
   {
	   instance = new PlayerModel();
   }
   return instance;
}

void PlayerModel::reset()
{
    rankList_1.clear();
    rankList_2.clear();
    rankList_3.clear();
    
    //0系统消息,1用户消息
    msgList_0.clear();
    msgList_1.clear();
    
    //活动列表
    activeList.clear();
    
    rmbGoodsList.clear();
    goodsList.clear();
    limitGoodsList.clear();
    
    uid = 0;
    user = "";
    pwd = "";
    name = "";
    sex = 0;
    avatar = "";
    birthday = "";
    zone = "";
    exp = 0;
    rmb = 0;
    money = 0;
    skey = "";
    total_board = 0;
    total_win = 0;
    pcount = 0;
    vtime = 0;
    vlevel = 0;
    login_days = 1;
    is_get_login_packer = false;
    first_recharge_coin = 0;
    is_login_succ = false;
    //sound = 0.5f;
    appModel = NULL;
    //isUpdate = 0;
    max_ship = 0;
    solt_num = 0;
    cur_time = 0;
    cur_time_s = 0;
    reward_card_count = 0;
    //vocieSwitch = true;
    //vocieEffectSwitch = true;
    //vocieSnakeSwitch = false;
    currBuyLimitGoodsCount = 0;
    broke_num = 0;
    auto_register = 1;
    chanel_id = 0;
    isPayLimitGoods = false;
    
    for (int i=0; i<5; ++i)
    {
        best_cards[i]="3c";
    }
    
    hronMsgList->removeAllObjects();
    hronMsgList->addObject(new HronMsgModel);
}

int PlayerModel::getLevelByExp(int exp)
{
    int level = 0;
    for (int i=0; i<100; ++i)
    {
        if (exp<=expArray[i])
        {
            level = i+1;
            break;
        }
    }
    if (exp>=expArray[99])
    {
        level = 100;
    }
    return level;
}

float PlayerModel::getCurrExpPercentage(int& nextExp,int currExp)
{
    for (int i=0; i<100; ++i)
    {
        if (currExp>expArray[i])
        {
            continue;
        }
        nextExp = expArray[i];
        float scale = (float)currExp/(float)expArray[i];
        if(scale == 0.0f)
        {
            scale = 0.001f;
        }
        else if(scale >= 1.0f)
        {
            scale = 1.0f;
        }
        return scale;
    }
    return 1.0f;
}

void PlayerModel::getBrokeChip()
{
    if (money>=DzConfig::getInstance()->brokeMoney || broke_num<=0)
    {
        return;
    }
    money += DzConfig::getInstance()->brokeGiveMoney;
    //CCHttpAgent::getInstance()->doRewardChip();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PlayerModel::getBrokeChipCallBack),this, 0.1f, kRepeatForever, 0, false);
}

void PlayerModel::getBrokeChipCallBack(float dt)
{
    //CCHttpPacket& getChipPacket = CCHttpAgent::getInstance()->packets[atoi(string(CMD_REWARD_CHIP).c_str()) - 1000];
    //
    //if (getChipPacket.status != 0)
    //{
    //    if (getChipPacket.status != 3)
    //    {
    //        getChipPacket.status = 0;
    //        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PlayerModel::getBrokeChipCallBack), this);
    //        CCMessageBox("错误提示", "网络异常");
    //        money -= DzConfig::getInstance()->brokeGiveMoney;
    //        return;
    //    }
    //    
    //    int ret = getChipPacket.recvVal["ret"].asInt();
    //    if (ret == 100)
    //    {
    //        getChipPacket.status = 0;
    //        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PlayerModel::getBrokeChipCallBack), this);
    //        CCMessageBox("错误提示", "领取次数已经用完");
    //        money -= DzConfig::getInstance()->brokeGiveMoney;
    //        return;
    //    }
    //    else if (ret == 101)
    //    {
    //        getChipPacket.status = 0;
    //        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PlayerModel::getBrokeChipCallBack), this);
    //        CCMessageBox("错误提示", "不满足领取条件!");
    //        money -= DzConfig::getInstance()->brokeGiveMoney;
    //        return;
    //    }
    //}
    
    //PlayerModel::getInstance()->broke_num = getChipPacket.recvVal["data"]["broke_num"].asInt();
    //PlayerModel::getInstance()->money = getChipPacket.recvVal["data"]["money"].asInt();
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PlayerModel::getBrokeChipCallBack), this);
    
    string str = CCString::createWithFormat("很遗憾你已经破产了，系统将赠送你%d筹码，加油！",DzConfig::getInstance()->brokeGiveMoney)->getCString();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    showToast(str.c_str());
    #else
    //CCMessageBox(str.c_str(),"提示");
    #endif
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVNET_ROOM_UPDATE,NULL);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFI_UPDATE_RICHINFO_KEY);
    
}



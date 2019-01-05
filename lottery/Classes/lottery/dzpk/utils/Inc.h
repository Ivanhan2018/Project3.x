#ifndef __Game__Inc__
#define __Game__Inc__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

//#include "json.h"

//#include "Resources.h"

//#include "Strength.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

typedef unsigned int UNS_INT;

typedef unsigned long UNS_LONG;

typedef void (CCObject::*SEL_COMMON)(int);

typedef enum
{
	HALL_CONTRL_TAG,
	QUICK_GAME_CONTRL_TAG,
	USER_INFO_NODE_TAG,
	USER_INFO_FRAME_TAG,
	VENUE_LIST_TAG,
	ONLINE_TEXT_TAG,
    LEFT_BOTTOM_NODE_TAG,
	MINI_GAME_CONTRL_TAG,
	RECHARGE_CONTRL_TAG,
	PLAYERINFO_CONTRL_TAG,
	GAME_ACTIVITY_CONTRL_TAG,
	RANKING_CONTRL_TAG,
	SETTINGS_CONTRL_TAG,
	MSG_LIST_CONTRL_TAG,
	SEND_MSG_CONTRL_TAG,
	EMAIL_CONTRL_TAG,
	PLAYER_HEADER_TAG,
	PLAYER_NAME_TAG,
    BACK_PREV_SCENE_TAG,
	PLAYERINFO_NODE_TAG,
    
	NEW_HAND_SPACE_TAG,
	ELEMENTARY_HAND_SPACE_TAG,
	INTERMEDIATE_SPACE_TAG,
	ADVANCED_SPACE_TAG,
	PRIVATE_SPACE_TAG,
    
    MODIFY_NAME_EDITBOX_TAG,
    CHANGE_USER_HEADER_TAG,
    TIGER_ROCKING_CONTAINER_TAG,
    USER_MAN_HEADER_TAG,
    USER_WMAN_HEADER_TAG,
    FINISH_ACCOUNT_TAG,
    ACTIVE_FIRST_RECHARGE_TAG,
    ACTIVE_XIANGOU_TAG,
    ACTIVE_DUSHENG_TAG,
    ACTIVE_DUSHENG_REWARD_TAG,
	SEARCH_ROOM_CONTRL_TAG,
	CREATE_ROOM_CONTRL_TAG,
	HALL_BACK_CONTRL_TAG,
    
    POPUP_CTRLBTN_OK_TAG,
	POPUP_CTRLBTN_CANCEL_TAG,
	POPUP_CTRLBTN_FIRST_RECHARGE_TAG,
	POPUP_PLAYER_MAN_CONTRL_TAG,
	POPUP_PLAYER_WMAN_CONTRL_TAG,
	POPUP_DAILY_LOGIN_CONTRL_TAG,
    POPUP_NETWORK_UNUSUAL_OK_TAG,
    POPUP_NETWORK_UNUSUAL_CANCEL_TAG,
    SETTINGS_VOLUME_SWITCH_TAG,
    SETTINGS_VOLUME_EFFECT_SWITCH_TAG,
    SETTINGS_VOLUME_SNAKE_SWITCH_TAG,
    SETTINGS_HELP_TAG,
    SETTINGS_FEEDBACK_TAG,
    CHECK_VERSION_TAG,
    POPUP_MODIFY_NAME_TAG,
    POPUP_ALLWAY_TAG,
    POPUP_CONTENT_TAG,
    POPUP_TIGER_MACHINE_TAG,
    POPUP_GUAGUALE_MACHINE_TAG,
    CLOSE_TIGER_MACHINE_TAG,
    CLOSE_GUAGUALE_MACHINE_TAG,
    CLOSE_BOUNS_TABLE_TAG,
    POPUP_SHOPPING_TAG,
    BOUNS_TABLE_TAG,
    BOUNS_OPTION_0_TAG,
    BOUNS_OPTION_1_TAG,
    BOUNS_OPTION_2_TAG,
    POPUP_ANIM_LOADING_TAG,
    ADD_VOCIE_TAG,
    SUB_VOCIE_TAG,
    CHANGE_ACCOUNT_TAG,
    ACCOUNT_LOGIN_TAG,
    ACCOUNT_REGISTER_TAG,
    CANCEL_LONG_REGISTER_TAG,
    RECHARGE_RMB_TAG,
    REPEAT_REQ_GUAGUALE_TAG,
    TIGER_MACHINE_LAYER_TAG,
    GUAGUALE_MACHINE_LAYER_TAG,
    WEIXIN_SHARE_LAYER_TAG,
    RMB_RECHARGE_LAYER_TAG,
    SHARE_TO_FRIEND,
    SHARE_TO_FRIEND_CIRCLE,
    SHARE_BTN_TAG,
    SHARE_CANCEL_BTN_TAG
}CONTRL_TAG;

typedef enum
{
    SMALL_TO_BIG_ORDER,
    BIG_TO_SMALL_ORDER
}ORDER;

typedef enum
{
    POPUP_LAYER_TOUCH_PRIORITY = -100,
    CTRL_BTN_TOUCH_PRIORITY = -102,
    TOP_POPUP_TOUCH_PRIORITY = -102,
}TOUCH_PRIORITY;

typedef enum
{
    ORDER_BY_HOUSE_ID,
    ORDER_BY_BIG_OR_SMALL_BLIND,
    ORDER_BY_MAX_AND_MIN_CARRY,
    ORDER_BY_PLAYER_COUNT_OF_HOUSE
}ORDER_BY;

typedef enum
{
    SETTING_SCENE,
    UNKNOWN_SCENE
}SCENE;

typedef enum
{
    WEIXIN_CLIENT_TIP,
    NETWORK_UNUSUAL_TIP,
    GAME_SAFEGUARD_TIP,
    FORCE_UPDATE_TIP,
    LOGIN_ERROR_TIP,
    NO_MONEY_TIP,
    NO_LIMIT_GOODS_TIP,
    EXCHANGE_GOODS_TIP,
    ERROR_INFO_TIP,
    GET_VENUELIST_ERROR_TIP,
    GET_GOODSLIST_ERROR_TIP,
    GET_RANKLIST_ERROR_TIP,
    GET_ACTIVELIST_ERROR_TIP,
    GET_DUSHEN_REWARD_ERROR_TIP,
    GET_MAILLIST_ERROR_TIP,
    MODIFY_USER_INFO_TIP,
    MODIFY_USER_ERROR_TIP,
    CHANGE_ACCOUNT_TIP,
    UPDATE_VERSION_TIP,
    NO_ACTION,
    INPUT_ERROR_TIP,
    REGISTER_ERROR_TIP,
    GUAGUALE_LIST_TIP,
    GUAGUALE_REWARD_TIP,
    DEDUCT_GUAGUALE_SHIP_TIP,
    CHONGFU_GET_REWARD_TIP,
    NO_GUAGUA_FINISH_REWARD_TIP
}TIP_TAG;

typedef enum
{
    TIGER_ADD_SPEED_STATUS,
    TIGER_SUB_SPEED_STATUS,
    TIGER_STOPING_STATUS,
    TIGER_STOPED_STATUS
}TIGER_STATUS;

enum server_command
{
	SERVER_LOGIN_SUCC       		 = 4001,
    SERVER_LOGIN_ERROR               = 4002,
    SERVER_LOGIN_SUCC_RE             = 4003,
    SERVER_LOGIN_ERROR_RE            = 4004,
    SERVER_TABLE_INFO         	     = 4005,
	SERVER_MY_INFO				     = 4006,
	SERVER_SIT_DOWN			         = 4007,
    SERVER_SIT_ERROR                 = 4008,
	SERVER_STAND_UP      			 = 4009,
    SERVER_STAND_ERROR               = 4010,
    SERVER_LOGOUT_SUCC               = 4011,
    SERVER_LOGOUT_ERROR              = 4012,
	SERVER_GAME_START			     = 4013,
    SERVER_GAME_END			         = 4014,
	SERVER_MY_CARD			         = 4015,
	SERVER_SEE_CARD_FIRST			 = 4016,
	SERVER_SEE_CARD_SECOND           = 4017,
	SERVER_SEE_CARD_THIRD			 = 4018,
    SERVER_SHOWCARDS                 = 4019,
    SERVER_PLAYER_ACTION             = 4020,
    SERVER_PLAYER_ACTION_ERR         = 4021,
    SERVER_VIEWERS                   = 4022, 
	SERVER_SHOW_SIDE_POTS			 = 4023,
	SERVER_NEXT_BET_PLAYER			 = 4024,
    SEVER_CHAT_SUCC                  = 4026,
    SERVER_FACE                      = 4027,
    SERVER_LOGIN_AND_SIT_SUCC	     = 4028,
    SERVER_LOGIN_AND_SIT_ERR	     = 4029,
    SERVER_UPDATE_ROOM               = 4032,
    SERVER_UPDATE_ROOM_ERR           = 4033,
};

enum UIReturnKeyType{
    kUIReturnKeyDefault = 0,
    kUIReturnKeyGo,
    kUIReturnKeyGoogle,
    kUIReturnKeyJoin,
    kUIReturnKeyNext,
    kUIReturnKeyRoute,
    kUIReturnKeySearch,
    kUIReturnKeySend,
    kUIReturnKeyYahoo,
    kUIReturnKeyDone,
    kUIReturnKeyEmergencyCall,
};

//大喇叭请求
enum HRON_CMD
{
    HRON_USER_LOGIN_CMD = 1001,
    HRON_USER_SAY_CMD = 1002,
    
    HRON_USER_SAY_SUCC_CMD = 4004,
    HRON_CONNENCT_SUCC_CMD = 4001,
    HRON_USER_CHAT_CMD = 4003,
    HRON_SYS_CHAT_CMD = 4006,
    PAY_SUCCESS_CMD = 4007
};

class CCInt : public CCObject
{
public:
    CCInt(int i){this->i = i;}
    int getInt()const{return i;}
private:
    int i;
};

//http server address
#define HTTP_SERVER_ADDRESS                  "http://dz.boxwan.cn/api.php?ver=1.1.0&"
#define HTTP_IMAGE_SERVER_ADDRESS            "http://dzimages.boxwan.cn"
#define GAME_PAY_ADDRESS                     "http://dz.boxwan.cn/api.php?action=Ihuizhipay&callbackinfo="
#define WEIXIN_SHARE_ADDRESS                 "http://zjhboss.boxwan.cn/cron/index.php/share?uid="

//music
#define HALL_BG_MUSIC                        "sound/bg_music.mp3"

//heart beat Interval
#define HEART_BEAT_INTERVAL                  1.0f

//hron msg
#define MAX_GRON_MSG_COUNT                   20

//ccuserdefault key
#define PLAYER_ACOUNT_KEY                    "player_acount_key"
#define PLAYER_PASSWORD_KEY                  "player_password_key"
//#define PLAYER_SEX_KEY                      "player_sex_key"
//#define PLAYER_NICK_NAME_KEY                "player_nick_name_key"
#define SYSTEM_VOLUME_SWITCH_KEY             "system_volume_switch_key"
#define SYSTEM_VOLUME_VALUE_KEY              "system_volume_value_key"
#define SYSTEM_VOLUME_EFFECT_SWITCH_KEY      "system_volume_effect_switch_key"
#define SYSTEM_VOLUME_SNAKE_SWITCH_KEY       "system_volume_snake_switch_key"

//notifi
#define NOTIFI_TIGER_ROCKING_KEY             "notifi_tiger_rocking_key"
#define NOTIFI_UPDATE_RICHINFO_KEY           "notifi_update_richinfo_key"
#define NOTIFI_DAILY_SHOW_TIGER_KEY          "notifi_daily_show_tiger_key"
#define NOTIFI_GUAGUALE_REWARD_KEY           "notifi_guaguale_reward_key"
#define NOTIFI_DEDUCT_GUAGUALE_SHIP_KEY      "notifi_deduct_guaguale_ship_key"
#define NOTIFI_RECHARGE_KEY                  "notifi_recharge_key"
#define NOTIFI_VENUE_PERSON_MAX_KEY          "notifi_venue_person_max_key"
#define NOTIFI_REFRESH_VENUE_LIST_KEY        "notifi_refresh_venue_list_key"
#define NOTIFI_SCROLLING_HORN_MSG_KEY        "notifi_scroll_horrn_msg_key"
#define NOTIFI_NO_RMB_KEY                    "notifi_no_rmb_key"
#define NOTIFI_NO_MONEY_KEY                  "notifi_no_money_key"
#define NOTIFI_PAY_LIMIT_GOODS_KEY           "notifi_pay_limit_goods_key"

#define EVNET_ROOM_BACKGROUND    "event_come_to_room_background"
#define EVNET_ROOM_FOREGROUND    "event_come_to_room_foreground"
#define EVNET_ROOM_UPDATE        "event_uptate_room_myinfo"

#define common_selector(_SELECTOR) (SEL_COMMON)(&_SELECTOR)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "Jni/MiscHelper.h"
#endif

#define CMD_ANNOUNCEMENT                    "1000"
#define CMD_AUTOREGISTER                    "1001"
#define CMD_AWARDLIST                       "1002"
#define CMD_BROKEGET                        "1003"
#define CMD_BROKENUM                        "1004"
#define CMD_EXCHANGEAWARD                   "1005"
#define CMD_EXCHANGEMONEY                   "1006"
#define CMD_FEEDBACK                        "1007"
#define CMD_GOODSLIST                       "1008"
#define CMD_HEARTBEAT                       "1009"
#define CMD_LOGIN                           "1010"
#define CMD_LOGINREWARD                     "1011"
#define CMD_LOTTERY                         "1012"
#define CMD_LOTTERYINFO                     "1013"
#define CMD_MARQUEE                         "1014"
#define CMD_MESSAGELIST                     "1015"
#define CMD_MODIFYINFO                      "1016"
#define CMD_ONPLAYINFO                      "1017"
#define CMD_PAYRECORD                       "1018"
#define CMD_PLAYNREWARD                     "1019"
#define CMD_RANKLIST                        "1020"
#define CMD_REGISTER                        "1021"
#define CMD_UPDATE                          "1022"
#define CMD_USERINO                         "1023"
#define CMD_AWARDLOG                        "1025"
#define CMD_LOGINREWARD_INFO                "1026"
#define CMD_TIGER_MACHINE                   "1027"
#define CMD_REWARD_CHIP                     "1028"
#define CMD_CONFIG                          "1029"
#define CMD_ACTIVE                          "1030"
#define CMD_GET_GUAGUALE_LIST               "1031"
#define CMD_GET_GUAGUALE_REWARD             "1032"
#define CMD_DEDUCT_GUAGUALE_SHIP            "1033"
#define CMD_DESHEN_REWARD                   "1034"

#define CMD_VENUELIST_1                     "1035"
#define CMD_VENUELIST_2                     "1036"
#define CMD_VENUELIST_3                     "1037"
#define CMD_VENUELIST_4                     "1038"
#define CMD_VENUELIST_5                     "1039"

//房间请求
#define CMD_LOGIN_ROOM                      1001
#define CMD_TABLE_INFO                      1002
#define CMD_SIT_REQUEST                     1003
#define CMD_STAND_REQUEST                   1004
#define CMD_LOGOUT_REQUEST                  1005
#define CMD_PLAYER_ACTION                   1006
#define CMD_CHAT                            1009
#define CMD_UPDATE_ROOM                     1012

#define DEFAULT_AVATAR                      "Icon.png"


////net
//#include "Net/HttpAgent.h"
//#include "Net/JPacket.h"
//#include "Net/Queue.h"
//#include "Net/Network.h"
//#include "Net/JPacket.h"
//#include "Net/TcpClient.h"
//
////model
//#include "model/PlayerModel.h"
//#include "model/Venue.h"
//#include "model/AppModel.h"
//#include "model/RankModel.h"
//#include "model/MessageModel.h"
//#include "model/GoodsModel.h"
//#include "model/DzConfig.h"
//
////utils
//#include "utils/PlistReader.h"
//#include "utils/StringFormat.h"
//#include "utils/DLog.h"
//
////room
//#include "ui/rooms/RoomScene.h"

#ifdef __cplusplus
extern "C" {
#endif
    double getCurrentTime();
    
    double getServerTime();
    
    double getDeltaT();
    
    void setDeltaT(double t);
    
    //获取玩家头像
    void getOtherHeader(int uid , int seatid);
#ifdef __cplusplus
}
#endif

#endif
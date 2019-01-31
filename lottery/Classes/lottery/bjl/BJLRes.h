//  [3/18/2014 wucan]

#ifndef		__BJL_RES__
#define		__BJL_RES__
#include "Define.h"
//--Notification message
#define MESSAGE_JNICONTROL				"message_jnicontrol"
#define MESSAGE_TOVIEW						"message_toview"
#define MESSAGE_TOLOGIC					"message_tologic"
#define MESSAGE_MENUCLICKED			"message_menuclicked"		//按钮按下

//--jnicontrol message
enum
{
	MSG_JNI_Share=1,
	MSG_JNI_Task,
	MSG_JNI_Start,
	MSG_JNI_Recharge,
};

////--game Scene
//enum
//{
//	SCENE_start=1,			//大厅
//	SCENE_game,
//	SCENE_endAgain
//};

namespace texture_name
{
	//所有图片资源
	const static char s_cardTable1[] = "cardtable.png";//<by hxh>
	const static char s_cardTable[] = "myCardtable.png";
	const static char s_background[]="BJL_01.png";//背景pic
	const static char s_cardtable[] = "cardtable.png";//<by hxh>
	const static char s_usericon[] = "manicon.png";//男玩家头像pic
	const static char s_womanIcon[] = "womanicon.png";//女玩家头像
	const static char s_bujiao[] = "btn_bujiao.png";//不叫地主btn
	const static char s_buqiang[] = "btn_buqiang.png";//不抢地主btn
	const static char s_CCteefTag[] = "BJL_15.png";//空闲时间 pic
	const static char s_PlaceScore[] = "BJL_14.png";//下注时间 pic
	const static char s_CardTalayer[] = "BJL_16.png";//开牌时间 pic
	const static char s_GameState0[] = "BJL_23_1.png";//空闲时间 pic
	const static char s_GameState1[] = "BJL_23_2.png";//下注时间 pic
	const static char s_GameState2[] = "BJL_23_3.png";//开牌时间 pic
	const static char s_CallScoreTip[] = "BJL_04.png";//可以下注提示 pic
	const static char s_BankerCallScoreTip[] = "BJL_04_2.png";//可以下注提示 pic
	const static char s_timer[] = "BJL_19.png";//计时器 pic<by hxh 20160630>
	const static char s_timerNumber[] = "timernumber_1.png";//计时器数字 pic
	const static char s_ChipMagerNumber[] = "SCORE_NUMBER.png";//总注数字 pic
	const static char s_TIME_NUMBERScore[] = "TIME_NUMBER.png";//下注分数 pic
	const static char s_ChangeBankerTip1[] = "BJL_11.png";//由您做庄 pic
	const static char s_ChangeBankerTip2[] = "BJL_12.png";//轮换庄家 pic
	const static char s_ChangeBankerTip3[] = "BJL_13.png";//无人做庄 pic
	const static char s_SpriteNunber[] = "SCORE_BACK.png";//总注 pic
	const static char s_MySpriteNunber[] = "ME_SCORE_BACK.png";//自己下注 pic
	const static char s_Light[]= "cm_15.png";//筹码按钮光圈
	const static char s_coinNumber1[] = "coinnumber1.png";
	const static char s_GameRecord[] = "BJL_06.png";//游戏记录 pic
	const static char s_ZXPGou[] = "BJL_24_1.png";//√ pic
	const static char s_BankerUser[]= "BJL_05.png";//庄家用户 pic
	const static char s_Gender1[] = "bjl_face_man.png";//男 pic<by hxh 20160919>
	const static char s_Gender0[] = "bjl_face_woman.png";//女 pic<by hxh 20160919>
	const static char s_Player[] = "BJL_20_2.png";//闲 pic<by hxh 20160721>
	const static char s_Banker[] = "BJL_20_1.png";//庄 pic<by hxh 20160721>
	const static char s_BankerPic1[] = "bjl_face_man_2.png";//庄家头像 pic<by hxh 20160919>
	const static char s_settlementbg[] = "settlement_bg.png";//结算背景
	const static char s_BankerPic2[] = "bjl_face_man_2.png";//庄家头像 pic<by hxh 20160919>
	const static char s_PlayerWin[] = "BJL_20_5.png";//闲赢 pic<by hxh 20160811>
	const static char s_BankerWin[] = "BJL_20_4.png";//庄赢 pic<by hxh 20160811>
	const static char s_AndBureau[] = "BJL_20_3.png";//和 pic<by hxh 20160811>
	const static char s_GameEnd[] = "GAME_END.png";//游戏结束 pic<by hxh 20160811>
	const static char s_font_jiaodizhu[] = "font_jiaodizhu.png";//叫地主字
	const static char s_font_pass[] = "font_pass.png";//不出字
	const static char s_font_qiangdizhu[] = "font_qiangdizhu.png";//抢地主字
	const static char s_font_visiblecard[] = "font_visiblecard.png";//明牌字
	const static char s_font_jiabei[] = "font_jiabei.png";//加倍字
	const static char s_font_hfd[] = "font_hfd.png";
	const static char s_x[] = "x.png";//X
	const static char s_menuBarBackground[] = "bg_menubar.png";//菜单栏背景
	const static char s_back[] = "btn_back.png";//返回按钮
	const static char s_options[] = "btn_options.png";//设置按钮
	const static char s_pull[] = "btn_pull.png";//菜单栏拉环
	const static char s_robot[] = "btn_robot.png";//托管按钮
	const static char s_talk[] = "btn_talk.png";//聊天按钮
	const static char s_task[] = "btn_task.png";//任务按钮
	const static char s_smallCard[] = "smallcard.png";//小扑克牌图片
	const static char s_hintCard[] = "hintcard.png";//盖着的扑克图片
	const static char s_settlementDetial[] = "settlementdetial.png";//查看明细btn
	const static char s_jiaBeiBar[] = "jbb_bj.png";//底牌加倍处的提示背景图片

	const static char s_music[] = "music.png";//背景音乐图片
	const static char s_soundEffect[] = "soundeffect.png";//音效图片
	const static char s_off[] = "off.png";//关背景图片
	const static char s_on[] = "on.png";//开背景图片
	const static char s_on_off[] = "on_off.png";//开关图片
	const static char s_progress_bj[] = "progressbar0.png";//进度条背景
	const static char s_progress[] = "progressbar1.png";//进度条
	const static char s_setup_bg[] = "task_k.png";//设置底框
	const static char s_expression[] = "expression.png";//表情文字
	const static char s_voice[] = "voice.png";//语音文字

	const static char s_confirm[] = "btn_confirm.png";//确定按钮
	const static char s_confirm1[] = "btn_confirm1.png";//确定按钮<by hxh>
	const static char s_faceBg[] = "face_bg.png";//表情框背景
	//const static char s_xlt[] = "xlt.png";//下拉条
	const static char s_yyk[] = "yyk.png";//语音框
	const static char s_banshou[] = "banshou.png";//机器人扳手
	const static char s_cancleRobot[] = "btn_cancelrobot.png";//取消托管
	const static char s_eye[]= "eye.png";//机器人眼睛
	const static char s_robot_pic[] = "robot.png";//机器人图片
	const static char s_paiBei0[] = "paibei0.png";//牌背0
	const static char s_paiBei1[] = "paibei1.png";//牌背1

	const static char s_dizhu_man[] = "dizhu_man.png";//男地主
	const static char s_farmer_man[] = "farmer_man.png";//男农民
	const static char s_dizhu_woman[] = "dizhu_woman.png";//女地主
	const static char s_farmer_woman[] = "farmer_woman.png";//女农民
	const static char s_btn_cancle[] = "btn_cancle.png";//取消按钮
	const static char s_btn_cancle1[] = "btn_cancle1.png";//取消按钮<by hxh>
	const static char s_huojian[] = "huojian.png";//火箭
	const static char s_huoyan[] = "huoyan.png";//火焰
	const static char s_paizi[] = "paizi.png";//搜人动画底图
	const static char s_font_deng[] = "font_deng.png";//等
	const static char s_font_dai[] = "font_dai.png";//待
	const static char s_font_qi[] = "font_qi.png";//其
	const static char s_font_ta[] = "font_ta.png";//他
	const static char s_font_wan[] = "font_wan.png";//玩
	const static char s_font_jia[] = "font_jia.png";//家
	const static char s_font_zhong[] = "font_zhong.png";//中
	const static char s_A_8[] = "A_8.png";//。。。<by hxh>
	const static char s_loser[] = "loser.png";//失败动画
	const static char s_winner[] = "winner.png";//胜利动画
	const static char s_notify[] = "notify.png";//游戏最上面提示信息背景
	const static char s_task_d[] = "task_d.png";//任务文字低
	const static char s_task_k[] = "task_k.png";//任务框
	const static char s_bankerlist[] = "BJL_25_2.png";//上庄列表<by hxh>

	//dzpk资源图片<by hxh 20160826>
	const static char s_dzpklayerBG[] = "dzpk_backgroud.png";//德州扑克桌子背景
	const static char s_ready[] = "Ap_35.png";//准备
	const static char s_tx_boy[] = "tx_boy%d.png";//男
	const static char s_tx_girl[] = "tx_girl%d.png";//女
	const static char s_ol_btn_ckpx_pressed[] = "ol_btn_ckpx_pressed.png";//查看牌型按下
	const static char s_ol_btn_gryz_pressed[] = "ol_btn_gryz_pressed.png";//跟任何注按下
	const static char s_table_level[] = "table_level%d.png";
	const static char s_room_light[] = "room_light.png";//灯光 by hxh 20161014
	//img_game_operalayer
	const static char s_win[] = "GaneSuccent.png";
	const static char s_lose[] = "GameWaring.png";
	const static char s_draw[] = "GameNumBer.png";
	const static char s_cd_bg_other[] = "cd_bg_other.png";//倒计时 by hxh 20161012
	const static char s_dl_btn_cancel[] = "dl_btn_cancel.png";//否
	const static char s_dl_btn_confirm[] = "strGame_button1.png";//"dl_btn_confirm.png";//是
	const static char s_dl_bg_wxts[] = "dl_bg_wxts.png";//温馨提示
	const static char s_ol_bg_bccm[] = "ol_bg_bccm.png";//VipLagerBack
	const static char s_strGame_button1[] = "strGame_button1.png";//确定
	const static char s_strGame_button2[] = "strGame_button2.png";//确定
	const static char s_ol_btn_qp[] = "ol_btn_qp.png";//放弃
	const static char s_ol_btn_qx[] = "ol_btn_qx.png";//梭哈
	const static char s_gp_btn[] = "Acard.png";//过牌
	const static char s_jz_btn[] = "ol_btn_jz.png";//加注
	const static char s_gz_btn[] = "ol_btn_gz.png";//跟注
	const static char s_xz_btn[] = "ol_btn_xz.png";//下注
	const static char s_ol_bg_raise[] = "ol_bg_raise.png";
	const static char s_ol_track_raise[] = "ol_track_raise.png";//滑动条背景图
	const static char s_ol_progress_raise[] = "ol_progress_raise.png";//划过区域图
	const static char s_ol_thumb_raise[] = "ol_thumb_raise.png";//滑块图
	const static char s_ol_btn_ckpx_normal[] = "ol_btn_ckpx_normal.png";//查看牌型
	const static char s_ol_bg_ckpx[] = "ol_bg_ckpx.png";//牌型
	const static char s_ol_btn_gryz_normal[] = "ol_btn_gryz_normal.png";//跟任何注
	const static char s_start_button1[] = "StartButton1.png";//开始
	const static char s_start_button2[] = "StartButton2.png";//开始
	const static char s_exit_button1[] = "exitbutton1.png";//离开
	const static char s_exit_button2[] = "exitbutton2.png";//离开
	const static char s_exit_1[] = "exit_1.png";//退出
	const static char s_exit_2[] = "exit_2.png";//退出
	const static char s_card_big_baiban[] = "card_big_baiban.png";//扑克牌（大）白板
	const static char s_card_small_baiban[] = "card_small_baiban.png";//白板（小）
	const static char s_card_color[] = "card_color.png";//扑克牌花色（黑红方梅）
	const static char s_card_num_black[] = "card_num_black.png";//扑克牌字黑
	const static char s_card_num_red[] = "card_num_red.png";//扑克牌红
	const static char s_joker[] = "joker.png";	//图片大小为 pic

	const static char s_dzpk_raise_slider_floor[] = "dzpk_raise_slider_floor.png";			//加注条——地板
	const static char s_dzpk_raise_slider_bg[] = "dzpk_raise_slider_bg.png";				//加注条——背景
	const static char s_dzpk_raise_slider_progress[] = "dzpk_raise_slider_progress.png";	//加注条——进度
	const static char s_dzpk_raise_slider_thumb[] = "dzpk_raise_slider_thumb.png";			//加注条——滑动块
	const static char s_dzpk_raise_slider_allin[] = "dzpk_raise_slider_allin.png";			//	加注条文本——全下

	const static char s_card[] ="card.png";//扑克牌
	//DZPKImage
	const static char s_mk_bg_makers[] = "mk_bg_makers.png";//D
	const static char s_rf_back_light[] = "RF_back_light.png";//
	const static char s_rf_a[] = "RF_a.png";//A
	const static char s_rf_k[] = "RF_k.png";//K
	const static char s_rf_q[] = "RF_q.png";//Q
	const static char s_rf_j[] = "RF_j.png";//J
	const static char s_rf_10[] = "RF_10.png";//10
	const static char s_rf_back_circle[] = "RF_back_circle.png";//
	const static char s_rf_zi[] = "RF_zi.png";//皇家同花顺
	const static char s_ts_bg_lock[] = "ts_bg_lock.png";//锁
	const static char s_ts_bg_circle1[] = "ts_bg_circle1.png";//
	const static char s_ts_bg_arrow[] = "ts_bg_arrow.png";//
	const static char s_action_think[] = "action_think.png";//思考
	const static char s_small_blind[] = "small_blind.png";//小盲位
	const static char s_big_blind[] = "big_blind.png";//大盲位
	const static char s_action_check[] = "action_check.png";//过
	const static char s_action_call[] = "action_call.png";//跟注
	const static char s_action_raise[] = "action_raise.png";//加注
	const static char s_action_bet[] = "action_bet.png";//下注
	const static char s_action_fold[] = "action_fold.png";//弃牌
	const static char s_action_allin[] = "action_allin.png";//全下
	const static char s_cardtype_gaopai[] = "cardtype_gaopai.png";//高牌
	const static char s_cardtype_hulu[] = "cardtype_hulu.png";//葫芦
	const static char s_cardtype_liangdui[] = "cardtype_liangdui.png";//两对
	const static char s_cardtype_santiao[] = "cardtype_santiao.png";//三条
	const static char s_cardtype_shunzi[] = "cardtype_shunzi.png";//顺子
	const static char s_cardtype_sitiao[] = "cardtype_sitiao.png";//四条
	const static char s_cardtype_tonghua[] = "cardtype_tonghua.png";//同花
	const static char s_cardtype_tonghuashun[] = "cardtype_tonghuashun.png";//同花顺
	const static char s_cardtype_yidui[] = "cardtype_yidui.png";//一对
	const static char s_room_win_label[] = "room_win_label.png";//胜利啦!
	const static char s_cardtype_gaopai_2[] = "cardtype_gaopai_2.png";//高牌
	const static char s_cardtype_hulu_2[] = "cardtype_hulu_2.png";//葫芦
	const static char s_cardtype_liangdui_2[] = "cardtype_liangdui_2.png";//两对
	const static char s_cardtype_santiao_2[] = "cardtype_santiao_2.png";//三条
	const static char s_cardtype_shunzi_2[] = "cardtype_shunzi_2.png";//顺子
	const static char s_cardtype_sitiao_2[] = "cardtype_sitiao_2.png";//四条
	const static char s_cardtype_tonghua_2[] = "cardtype_tonghua_2.png";//同花
	const static char s_cardtype_tonghuashun_2[] = "cardtype_tonghuashun_2.png";//同花顺
	const static char s_cardtype_yidui_2[] = "cardtype_yidui_2.png";//一对
	const static char s_cm_bg_redchip[] = "cm_bg_redchip.png";
	const static char s_cm_bg_bluechip[] = "cm_bg_bluechip.png";
	const static char s_cm_bg_purplechip[] = "cm_bg_purplechip.png";
	const static char s_desk_num_frame[] ="desk_num_frame.png";
	const static char s_dzchip[] = "dzchip_%d.png";//筹码 by hxh 20161015
	const static char s_room_card_frame[] = "room_card_frame.png";
	//dzpk
	const static char s_nullHead[] = "pi_bg_nullplayer.png";//空头像 by hxh 20161012
	const static char s_info_bg_mask[] = "pi_bg_mask.png";//弃牌或旁观时遮罩 by hxh 20161012
	const static char s_info_bg_chipvalue[] = "pi_bg_chipvalue.png";//筹码值背景 by hxh 20161012
	const static char s_next_round[] = "next_round.png";//等待下一局 by hxh 20161012
	const static char s_offline[] = "offline.png";//掉线 by hxh 20161012
	
	//DZPKImageAdd
	const static char s_SendCardMachine0[] = "SendCardMachine0.png";
	const static char s_CardBK[] = "CardBK.png";
	const static char s_jingbao5[] = "jingbao5.png";
	//pic_show
	const static char s_tx_boy6[] = "tx_boy6.png";//头像
	//pic_game
	const static char s_vip_level[] = "table_level1.png";//会员等级

	//玩家小头像
	const static char s_man_head_icon[] = "man_head_icon.png";
	const static char s_woman_head_icon[] = "woman_head_icon.png";
	const static char s_userMessage[] = "usermessage.png";//用户信息框
}
#endif
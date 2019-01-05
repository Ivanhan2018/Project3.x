//  [3/18/2014 wucan]

#ifndef		DDZ___DDZ_RES__
#define		DDZ___DDZ_RES__
#include "Define.h"
//--Notification message
#define MESSAGE_JNICONTROL				"message_jnicontrol"
#define MESSAGE_TOVIEW						"message_toview"
#define MESSAGE_TOLOGIC					"message_tologic"
#define MESSAGE_MENUCLICKED			"message_menuclicked"		//按钮按下

////--jnicontrol message
//enum
//{
//	MSG_JNI_Share=1,
//	MSG_JNI_Task,
//	MSG_JNI_Start,
//	MSG_JNI_Recharge,
//};
//
////--game Scene
//enum
//{
//	SCENE_start=1,			//大厅
//	SCENE_game,
//	SCENE_endAgain
//};


namespace DDZtexture_name
{
	//所有图片资源
	//const static char s_cardTable[] = "cardtable.png";
	const static char s_cardTable[] = "ddzCardtable.png";

	//	结算界面
	const static char s_ddzBGSummary[] = "ddz_popups_summary_bg_summary.png";						//	结算界面——背景
	const static char s_ddzTextLose[] = "ddz_popups_summary_text_lose.png";							//	结算界面——标识文字——失败
	const static char s_ddzTextWin[] = "ddz_popups_summary_text_win.png";							//	结算界面——标识文字——胜利
	const static char s_ddzTitleLose[] = "ddz_popups_summary_title_lose.png";						//	结算界面——标题——失败
	const static char s_ddzTitleWin[] = "ddz_popups_summary_title_win.png";							//	结算界面——标题——胜利
	const static char s_ddzTitleSpring[] = "ddz_popups_summary_title_spring.png";					//	结算界面——标题——春天
	const static char s_ddzPlayerLandlordMan[] = "ddz_popups_summary_player_landlord_man.png";		//	结算界面——标题——地主男
	const static char s_ddzPlayerLandlordWoman[] = "ddz_popups_summary_player_landlord_woman.png";	//	结算界面——标题——地主女
	const static char s_ddzPlayerPeasantMan[] = "ddz_popups_summary_player_peasant_man.png";		//	结算界面——标题——农民男
	const static char s_ddzPlayerPeasantWoman[] = "ddz_popups_summary_player_peasant_woman.png";	//	结算界面——标题——农民女


	//	字体
	const static char s_fntDDZGoldNum[] ="ddz_fnt_gold_num.fnt";	//	斗地主——字体——金币数字
	const static char s_fntDDZTimeNum[] ="ddz_fnt_time_num.fnt";	//	斗地主——字体——计时器数字

	//	背景
	const static char s_smallCardFront[] ="ddz_bg_small_card_front.png";//底牌正面
	const static char s_card[] ="card.png";//扑克牌
	const static char s_chatBarBg[] = "ddz_bg_chat.png";//聊天框背景
	const static char s_faceBg[] = "ddz_bg_face.png";//表情框背景
	const static char s_menuBarBackground[] = "ddz_bg_menu.png";//菜单栏背景
	const static char s_background[]="ddz_bg_table.png";//背景——牌桌
	const static char s_timer[] = "ddz_bg_time.png";//计时器 pic
	const static char s_popups_bg[] = "ddz_bg_popups.png";//弹窗背景
	const static char s_chatBarBG[] = "ddz_bg_chat_bar.png";//聊天框


	const static char s_notify[] = "notify.png";//游戏最上面提示信息背景
	const static char s_jiaBeiBar[] = "jbb_bj.png";//底牌加倍处的提示背景图片
	const static char s_showSelf[] = "showself.png";//结算，显示自己栏目时候的底图
	const static char s_hintCard[] = "hintcard.png";//盖着的扑克图片
	const static char s_task_k[] = "task_k.png";//任务框

	//	按钮
	const static char s_buJiaoNormal[] = "ddz_btn_bujiao_normal.png";								//	斗地主——按钮——不叫——正常
	const static char s_buJiaoPressed[] = "ddz_btn_bujiao_selected.png";							//	斗地主——按钮——不叫——按下
	const static char s_againNormal[] = "btn_again.png";											//	斗地主——按钮——再来一局——正常
	const static char s_againPressed[] = "btn_again.png";											//	斗地主——按钮——再来一局——按下
	const static char s_buqiangNormal[] = "ddz_btn_buqiang_normal.png";								//	斗地主——按钮——不抢——正常
	const static char s_buqiangPressed[] = "ddz_btn_buqiang_selected.png";							//	斗地主——按钮——不抢——按下
	const static char s_rechargeNormal[] = "ddz_btn_buy_normal.png";								//	斗地主——按钮——充值——正常
	const static char s_rechargePressed[] = "ddz_btn_buy_selected.png";								//	斗地主——按钮——充值——按下
	const static char s_btnCancelNormal[] = "ddz_btn_cancel_normal.png";							//	斗地主——按钮——取消——正常
	const static char s_btnCancelPressed[] = "ddz_btn_cancel_selected.png";							//	斗地主——按钮——取消——按下
	const static char s_changeTableNormal[] = "ddz_btn_change_table_normal.png";					//	斗地主——按钮——换桌——正常
	const static char s_changeTablePressed[] = "ddz_btn_change_table_selected.png";					//	斗地主——按钮——换桌——按下
	const static char s_detailNormal[] = "ddz_btn_detail_normal.png";								//	斗地主——按钮——查看结算明细——正常
	const static char s_detailPressed[] = "ddz_btn_detail_selected.png";							//	斗地主——按钮——查看结算明细——按下
	const static char s_menuPullTabFrontNormal[] = "ddz_btn_pull_tab_front_normal.png";				//	斗地主——按钮——菜单栏拉环——正常
	const static char s_menuPullTabBackNormal[] = "ddz_btn_pull_tab_back_normal.png";				//	斗地主——按钮——菜单栏拉环——正常
	const static char s_jiaoDiZhuNormal[] = "ddz_btn_jiaodizhu_normal.png";							//	斗地主——按钮——叫地主——正常
	const static char s_jiaoDiZhuPressed[] = "ddz_btn_jiaodizhu_selected.png";						//	斗地主——按钮——叫地主——按下
	const static char s_notRedoubleNormal[] = "ddz_btn_not_redouble_normal.png";					//	斗地主——按钮——不加倍——正常
	const static char s_notRedoublePressed[] = "ddz_btn_not_redouble_selected.png";					//	斗地主——按钮——不加倍——按下
	const static char s_btnOKNormal[] = "ddz_btn_ok_normal.png";									//	斗地主——按钮——确定——正常
	const static char s_btnOKPressed[] = "ddz_btn_ok_selected.png";									//	斗地主——按钮——确定——按下
	const static char s_openCardNormal[] = "ddz_btn_open_card_normal.png";							//	斗地主——按钮——明牌X2——正常
	const static char s_openCardPressed[] = "ddz_btn_open_card_selected.png";						//	斗地主——按钮——明牌X2——按下
	const static char s_openStartNormal[] = "ddz_btn_open_start_normal.png";						//	斗地主——按钮——明牌开始——正常
	const static char s_openStartPressed[] = "ddz_btn_open_start_selected.png";						//	斗地主——按钮——明牌开始——按下
	const static char s_outCardNormal[] = "ddz_btn_out_card_normal.png";							//	斗地主——按钮——出牌——正常
	const static char s_outCardPressed[] = "ddz_btn_out_card_selected.png";							//	斗地主——按钮——出牌——按下
	const static char s_passNormal[] = "ddz_btn_pass_normal.png";									//	斗地主——按钮——过牌——正常
	const static char s_passPressed[] = "ddz_btn_pass_selected.png";								//	斗地主——按钮——过牌——按下
	const static char s_qiangDiZhuNormal[] = "ddz_btn_qiangdizhu_normal.png";						//	斗地主——按钮——抢地主——正常
	const static char s_qiangDiZhuPressed[] = "ddz_btn_qiangdizhu_selected.png";					//	斗地主——按钮——抢地主——按下
	const static char s_redoubleNormal[] = "ddz_btn_redouble_normal.png";							//	斗地主——按钮——加倍——正常
	const static char s_redoublePressed[] = "ddz_btn_redouble_selected.png";						//	斗地主——按钮——加倍——按下
	const static char s_startNormal[] = "ddz_btn_start_normal.png";									//	斗地主——按钮——开始游戏——正常
	const static char s_startPressed[] = "ddz_btn_start_selected.png";								//	斗地主——按钮——开始游戏——按下
	const static char s_tipsNormal[] = "ddz_btn_tips_normal.png";									//	斗地主——按钮——提示——正常
	const static char s_tipsPressed[] = "ddz_btn_tips_selected.png";								//	斗地主——按钮——提示——按下
	const static char s_cancelRobotNormal[] = "ddz_btn_cancel_robot_normal.png";					//	斗地主——按钮——取消托管——正常
	const static char s_cancelRobotPressed[] = "ddz_btn_cancel_robot_selected.png";					//	斗地主——按钮——取消托管——按下

	const static char s_talk[] = "ddz_btn_chat_normal.png";//聊天按钮
	const static char s_back[] = "ddz_btn_quit_normal.png";//返回按钮
	const static char s_robot[] = "ddz_btn_robot_normal.png";//托管按钮
	const static char s_options[] = "ddz_btn_setting_normal.png";//设置按钮
	const static char s_task[] = "ddz_btn_task_normal.png";//任务按钮

	//	文字
	const static char s_text_multiple[] = "ddz_text_beishu.png";					//	斗地主——文字——倍数
	const static char s_text_difen[] = "ddz_text_difen.png";						//	斗地主——文字——底分
	const static char s_diZhuTag[] = "dizhutag.png";								//	地主标签

	const static char s_font_jiaodizhu[] = "ddz_img_text_jiaodizhu.png";			//	叫地主字
	const static char s_font_qiangdizhu[] = "ddz_img_text_qiangdizhu.png";			//	抢地主字
	const static char s_font_buqiang[] = "ddz_img_text_buqiang.png";				//	不抢字
	const static char s_font_bujiao[] = "ddz_img_text_bujiao.png";					//	不叫字
	const static char s_font_jiabei[] = "ddz_img_text_jiabei.png";					//	加倍字
	const static char s_font_pass[] = "ddz_img_text_pass.png";						//	不出字
	const static char s_font_visiblecard[] = "ddz_img_text_mingpai.png";			//	明牌字
	const static char s_ctDoubleLine[] = "ddz_img_text_liandui.png";				//	连对
	const static char s_ctSingleLine[] = "ddz_img_text_shunzi.png";					//	顺子
	const static char s_ctThreeLine[] = "ddz_img_text_sanshun.png";					//	三顺
	const static char s_ddz_img_text_ready[] = "ddz_img_text_ready.png";			//	准备

	//	玩家相关
	const static char s_userMessage[] = "ddz_bg_player_data.png";//用户信息框
	const static char s_usericon[] = "manicon.png";//男玩家头像pic
	const static char s_womanIcon[] = "womanicon.png";//女玩家头像

	//	其他


	//	old
	const static char s_one[] = "one.png";//倍数1 pic
	const static char s_two[] = "two.png";//2 pic
	const static char s_three[] = "three.png";//3 pic
	const static char s_four[] = "four.png";//4 pic
	const static char s_five[] = "five.png";//5 pic
	//const static char s_exchange[] = "btn_exchange.png";//兑换 btn
	const static char s_coinNumber[]= "coinnumber.png";//金币字
	const static char s_coinNumber1[] = "coinnumber1.png";
	const static char s_twice[] = "twice.png";//二倍
	const static char s_thrice[] = "thrice.png";//三倍
	const static char s_fourfold[] = "fourfold.png";//四倍
	const static char s_fivefold[] = "fivefold.png";//五倍
	const static char s_nonuple[] = "nonuple.png";//九倍
	const static char s_sixfold[] = "sixfold.png";//六倍
	const static char s_jiaBeiTag[] = "jiabeitag.png";//结算时的加倍标志
	const static char s_music[] = "music.png";//背景音乐图片
	const static char s_soundEffect[] = "soundeffect.png";//音效图片
	const static char s_off[] = "off.png";//关背景图片
	const static char s_on[] = "on.png";//开背景图片
	const static char s_on_off[] = "on_off.png";//开关图片
	const static char s_progress_bj[] = "progressbar0.png";//进度条背景
	const static char s_progress[] = "progressbar1.png";//进度条
	const static char s_expression[] = "expression.png";//表情文字
	const static char s_voice[] = "voice.png";//语音文字
	//表情
	const static char s_faceCool[] = "face_0.png";//酷
	const static char s_faceHan[] = "face_1.png";//汗
	const static char s_faceHanlen[] = "face_2.png";//寒冷
	const static char s_faceKu[] = "face_3.png";//哭
	const static char s_faceNu[] = "face_4.png";//怒
	const static char s_faceNue[] = "face_5.png";//虐
	const static char s_faceQiu[] = "face_6.png";//糗
	const static char s_faceTu[] = "face_7.png";//吐
	const static char s_faceXiao[] = "face_8.png";//笑
	const static char s_faceYun[] = "face_9.png";//晕

	//const static char s_xlt[] = "xlt.png";//下拉条
	const static char s_yyk[] = "yyk.png";//语音框
	const static char s_banshou[] = "banshou.png";//机器人扳手
	const static char s_eye[]= "eye.png";//机器人眼睛
	const static char s_robot_pic[] = "robot.png";//机器人图片
	const static char s_paiBei0[] = "paibei0.png";//牌背0
	const static char s_paiBei1[] = "paibei1.png";//牌背1
	const static char s_plane[] = "plane.png";//飞机
	const static char s_lxj0[] = "lxj0.png";//螺旋桨0
	const static char s_lxj1[] = "lxj1.png";//螺旋桨1
	const static char s_planeyy[] = "planeyy.png";//飞机下面的阴影
	const static char s_bomb[] = "bomb.png";//炸弹坑
	const static char s_bomb0[] = "bomb0.png";//炸弹火焰0
	const static char s_bomb1[] = "bomb1.png";//炸弹火焰1
	const static char s_bomb2[] = "bomb2.png";//炸弹火焰2
	const static char s_bomb3[] = "bomb3.png";//炸弹火焰3
	const static char s_bomb4[] = "bomb4.png";//炸弹火焰4
	const static char s_bomb5[] = "bomb5.png";//炸弹火焰5
	const static char s_bomb6[] = "bomb6.png";//炸弹火焰6
	const static char s_bomb7[] = "bomb7.png";//炸弹火焰7
	const static char s_yanwu0[] = "yanwu0.png";//人物形象变换烟雾
	const static char s_yanwu1[] = "yanwu1.png";
	const static char s_yanwu2[] = "yanwu2.png";
	const static char s_yanwu3[] = "yanwu3.png";
	const static char s_yanwu4[] = "yanwu4.png";
	const static char s_dizhu_man[] = "dizhu_man.png";//男地主
	const static char s_farmer_man[] = "farmer_man.png";//男农民
	const static char s_dizhu_woman[] = "dizhu_woman.png";//女地主
	const static char s_farmer_woman[] = "farmer_woman.png";//女农民
	const static char s_huojian[] = "huojian.png";//火箭
	const static char s_huoyan[] = "huoyan.png";//火焰
	const static char s_yun0[] = "yun0.png";//火箭云
	const static char s_yun1[] = "yun1.png";
	const static char s_yun2[] = "yun2.png";
	const static char s_yun3[] = "yun3.png";
	const static char s_paizi[] = "paizi.png";//搜人动画底图
	const static char s_font_deng[] = "font_deng.png";//等
	const static char s_font_dai[] = "font_dai.png";//待
	const static char s_font_qi[] = "font_qi.png";//其
	const static char s_font_ta[] = "font_ta.png";//他
	const static char s_font_wan[] = "font_wan.png";//玩
	const static char s_font_jia[] = "font_jia.png";//家
	const static char s_font_zhong[] = "font_zhong.png";//中
	const static char s_zawu[] = "zawu.png";//杂物
	const static char s_jingbao0[] = "jingbao0.png";//警报动画
	const static char s_jingbao1[] = "jingbao1.png";
	const static char s_jingbao2[] = "jingbao2.png";
	const static char s_jingbao3[] = "jingbao3.png";
	const static char s_jingbao4[] = "jingbao4.png";
	const static char s_jingbao5[] = "jingbao5.png";
	const static char s_loser[] = "loser.png";//失败动画
	const static char s_winner[] = "winner.png";//胜利动画
	const static char s_task_d[] = "task_d.png";//任务文字低

	//玩家小头像
	const static char s_man_head_icon[] = "man_head_icon.png";
	const static char s_woman_head_icon[] = "woman_head_icon.png";
	const static char s_dizhuCardTag[] = "dizhucardtag.png";//地主最后一张卡片右上角图标

	const static char s_showCardTag[] = "showcardtag.png";//明牌标志
	const static char s_difenTimesNum[] = "difentimesnum.png";//底分倍率数字
	//眨眼图片
	const static char s_dz_m_eye0[] = "dz_m_eye0.png";//男地主眨眼
	const static char s_dz_m_eye1[] = "dz_m_eye1.png";
	const static char s_dz_m_eye2[] = "dz_m_eye2.png";
	const static char s_dz_wm_eye0[] = "dz_wm_eye0.png";//女地主眨眼
	const static char s_dz_wm_eye1[] = "dz_wm_eye1.png";
	const static char s_dz_wm_eye2[] = "dz_wm_eye2.png";
	const static char s_fm_m_eye0[] = "fm_m_eye0.png";//男农民眨眼
	const static char s_fm_m_eye1[] = "fm_m_eye1.png";
	const static char s_fm_m_eye2[] = "fm_m_eye2.png";
	const static char s_fm_wm_eye0[] = "fm_wm_eye0.png";//女农民眨眼
	const static char s_fm_wm_eye1[] = "fm_wm_eye1.png";
	const static char s_fm_wm_eye2[] = "fm_wm_eye2.png";
	const static char s_m_eye0[] = "m_eye0.png";//男孩眨眼
	const static char s_m_eye1[] = "m_eye1.png";
	const static char s_m_eye2[] = "m_eye2.png";
	const static char s_wm_eye0[] = "wm_eye0.png";//女孩子眨眼
	const static char s_wm_eye1[] = "wm_eye1.png";
	const static char s_wm_eye2[] = "wm_eye2.png";


	const static char s_back_fen0[] = "back_fen0.png";//风筝
	const static char s_back_fen1[] = "back_fen1.png";
	const static char s_back_fen2[] = "back_fen2.png";

	const static char s_card_color[] = "card_color.png";//扑克牌花色（黑红方梅）
	const static char s_card_num_black[] = "card_num_black.png";//扑克牌字黑
	const static char s_card_num_red[] = "card_num_red.png";//扑克牌红
	const static char s_joker[] = "joker.png";	//图片大小为 pic
	//气泡资源
	const static char s_font_back[] = "font_back.png";//返回
	const static char s_font_chat[] = "font_chat.png";//聊天
	const static char s_font_robot[] = "font_robot.png";//托管
	const static char s_font_setup[] = "font_setup.png";//设置
	const static char s_font_task[] = "font_task.png";//任务
	const static char s_bubble[] = "bubble.png";//气泡

	//新的斗地主扑克牌图片
	const static char s_ddz_poker_back[] = "ddz_poker_back.png";//牌底图
	const static char s_ddz_poker_black[] = "ddz_poker_black.png";//小王
	const static char s_ddz_poker_lord_icon[] = "ddz_poker_lord_icon.png";//地主角标
	const static char s_ddz_poker_red[] = "ddz_poker_red.png";//大王
	const static char s_ddz_pokers_color[] = "ddz_pokers_color.png";//牌的花色
	const static char s_ddz_pokers_num[] = "ddz_pokers_num.png";//牌的数值
}
#endif
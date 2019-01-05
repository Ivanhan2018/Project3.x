//  [4/9/2014 xieyi]
#ifndef __SOUNDCONTROL__
#define __SOUNDCONTROL__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

//所有音效资源
namespace sound_name
{
	const static char s_bgMusic[] = "xyxbg.mp3";//背景音乐
// 	//男声
// 	const static char s_man_wav0[] = "man/wav0.wav";//大侠手下留情
// 	const static char s_man_wav1[] = "man/wav1.wav";//多打牌少发呆。。。
// 	const static char s_man_wav2[] = "man/wav2.wav";//坑爹呢
// 	const static char s_man_wav3[] = "man/wav3.wav";//牌不在好。。。
// 	const static char s_man_wav4[] = "man/wav4.wav";//我要开始发威了。。。
// 	const static char s_man_wav5[] = "man/wav5.wav";//西边的太阳。。。
// 	const static char s_man_bujiabei[] = "man/bujiabei.wav";//不加倍
// 	const static char s_man_bujiao[] = "man/bujiao.wav";//不叫
// 	const static char s_man_buqiang[] = "man/buqiang.wav";//不抢
// 	const static char s_man_buyao[] = "man/buyao.wav";//不要
// 	const static char s_man_dani[] = "man/dani.wav";//大你
// 	const static char s_man_feiji[] = "man/feiji.wav";//飞机
// 	const static char s_man_guo[] = "man/guo.wav";//过
// 	const static char s_man_huojian[] = "man/huojian.wav";//huojian
// 	const static char s_man_jiabei[] = "man/jiabei.wav";//加倍
// 	const static char s_man_jiaodizhu[] = "man/jiaodizhu.wav";//叫地主
// 	const static char s_man_liandui[] = "man/liandui.wav";//连队
// 	const static char s_man_mingpai[] = "man/mingpai.wav";//明牌
// 	const static char s_man_qiangdizhu[] = "man/qiangdizhu.wav";//抢地主
// 	const static char s_man_sandaiyi[] = "man/sandaiyi.wav";//三带一
// 	const static char s_man_sandaier[] = "man/sandaier.wav";//三带一对
// 	const static char s_man_sanshun[] = "man/sanshun.wav";//三顺
// 	const static char s_man_sanzhang[] = "man/sanzhang.wav";//三张
// 	const static char s_man_shunzi[] = "man/shunzi.wav";//顺子
// 	const static char s_man_sidaier[] = "man/sidaier.wav";//四代二
// 	const static char s_man_yasi[] = "man/yasi.wav";//压死
// 	const static char s_man_yaobuqi[] = "man/yaobuqi.wav";//要不起
// 	const static char s_man_yidui[] = "man/yidui.wav";//一对
// 	const static char s_man_yizhang[] = "man/yizhang.wav";//一张
// 	const static char s_man_zhadan[] = "man/zhadan.wav";//炸弹

	//女声

// 	const static char s_woman_wav0[] = "woman/wav0.wav";//大侠手下留情
// 	const static char s_woman_wav1[] = "woman/wav1.wav";//多打牌少发呆。。。
// 	const static char s_woman_wav2[] = "woman/wav2.wav";//坑爹呢
// 	const static char s_woman_wav3[] = "woman/wav3.wav";//牌不在好。。。
// 	const static char s_woman_wav4[] = "woman/wav4.wav";//我要开始发威了。。。
// 	const static char s_woman_wav5[] = "woman/wav5.wav";//西边的太阳。。。
// 	const static char s_woman_bujiabei[] = "woman/bujiabei.wav";//不加倍
// 	const static char s_woman_bujiao[] = "woman/bujiao.wav";//不叫
// 	const static char s_woman_buqiang[] = "woman/buqiang.wav";//不抢
// 	const static char s_woman_buyao[] = "woman/buyao.wav";//不要
// 	const static char s_woman_dani[] = "woman/dani.wav";//大你
// 	const static char s_woman_feiji[] = "woman/feiji.wav";//飞机
// 	const static char s_woman_guo[] = "woman/guo.wav";//过
// 	const static char s_woman_huojian[] = "woman/huojian.wav";//huojian
// 	const static char s_woman_jiabei[] = "woman/jiabei.wav";//加倍
// 	const static char s_woman_jiaodizhu[] = "woman/jiaodizhu.wav";//叫地主
// 	const static char s_woman_liandui[] = "woman/liandui.wav";//连队
// 	const static char s_woman_mingpai[] = "woman/mingpai.wav";//明牌
// 	const static char s_woman_qiangdizhu[] = "woman/qiangdizhu.wav";//抢地主
// 	const static char s_woman_sandaiyi[] = "woman/sandaiyi.wav";//三带一
// 	const static char s_woman_sandaier[] = "woman/sandaier.wav";//三带一对
// 	const static char s_woman_sanshun[] = "woman/sanshun.wav";//三顺
// 	const static char s_woman_sanzhang[] = "woman/sanzhang.wav";//三张
// 	const static char s_woman_shunzi[] = "woman/shunzi.wav";//顺子
// 	const static char s_woman_sidaier[] = "woman/sidaier.wav";//四代二
// 	const static char s_woman_yasi[] = "woman/yasi.wav";//压死
// 	const static char s_woman_yaobuqi[] = "woman/yaobuqi.wav";//要不起
// 	const static char s_woman_yidui[] = "woman/yidui.wav";//一对
// 	const static char s_woman_yizhang[] = "woman/yizhang.wav";//一张
// 	const static char s_woman_zhadan[] = "woman/zhadan.wav";//炸弹
	
	// 百家乐音效<by hxh 20160729>
	const static char s_add_gold[] = "ADD_GOLD.wav";//下注
	const static char s_dispatch_card[] = "DISPATCH_CARD.wav";//发牌
	const static char s_end_lost[] = "END_LOST.wav";//输
	const static char s_end_win[] = "END_WIN.wav";//赢
	const static char s_time_warimg[] = "TIME_WARIMG.wav";//倒计时提醒

	// 德州扑克音效<by hxh 20160912>
	const static char s_dzpk_b_ready[] = "dzpk_b_ready.mp3";//牌局开始
	const static char s_dzpk_man_giveover[] = "dzpk_man_demand_type_0_6_0.mp3";//放弃
	const static char s_dzpk_women_giveover[] = "dzpk_women_demand_type_2_6_0.mp3";//放弃
	const static char s_dzpk_man_qx[] = "dzpk_man_demand_type_0_5_0.mp3";//全压
	const static char s_dzpk_women_qx[] = "dzpk_women_demand_type_2_5_0.mp3";//全压
	const static char s_dzpk_man_jz[] = "dzpk_man_demand_type_0_2_0.mp3";//加注
	const static char s_dzpk_women_jz[] = "dzpk_women_demand_type_2_2_0.mp3";//加注
	const static char s_dzpk_man_gz[] = "dzpk_man_demand_type_0_3_0.mp3";//跟注
	const static char s_dzpk_women_gz[] = "dzpk_women_demand_type_2_3_0.mp3";//跟注
	const static char s_dzpk_man_guo[] = "dzpk_man_demand_type_0_4_0.mp3";//过——男
	const static char s_dzpk_women_guo[] = "dzpk_women_demand_type_2_4_0.mp3";//过——女
	const static char s_dzpk_man_xz[] = "dzpk_man_demand_type_0_1_0.mp3";//下注
	const static char s_dzpk_women_xz[] = "dzpk_women_demand_type_2_1_0.mp3";//下注
	const static char s_dzpk_win[] = "sound_guzhang.mp3";//赢了 by hxh
	const static char s_dzpk_lose[] = "sound_shibai.mp3";//输了 by hxh
	const static char s_dzpk_chupai[] = "sound_chupai.mp3";//轮到我方行动 by hxh
    const static char s_dzpk_fapai[] = "sound_fapai.mp3";//玩牌时—开始的发牌 by hxh
	const static char s_dzpk_gangjinru[] = "sound_gangjinru.mp3";//玩家刚加入玩牌时的音效（包括换桌，坐下，快速玩牌） by hxh
	const static char s_dzpk_dianji[] = "sound_dianji.mp3";//点击任意按钮 by hxh
	const static char s_dzpk_card_type_1[] = "dzpk_card_type_1.mp3";//散牌
	const static char s_dzpk_card_type_2[] = "dzpk_card_type_2.mp3";//对子
	const static char s_dzpk_card_type_3[] = "dzpk_card_type_3.mp3";//两对
	const static char s_dzpk_card_type_4[] = "dzpk_card_type_4.mp3";//三条
	const static char s_dzpk_card_type_5[] = "dzpk_card_type_5.mp3";//顺子
	const static char s_dzpk_card_type_6[] = "dzpk_card_type_6.mp3";//同花
	const static char s_dzpk_card_type_7[] = "dzpk_card_type_7.mp3";//葫芦
	const static char s_dzpk_card_type_8[] = "dzpk_card_type_8.mp3";//四条
	const static char s_dzpk_card_type_9[] = "dzpk_card_type_9.mp3";//同花顺
	const static char s_dzpk_card_type_10[] = "dzpk_card_type_10.mp3";//同花大顺
	const static char s_dzpk_sound_base_card[] = "dzpk_sound_base_card.mp3";//
	const static char s_dzpk_chouma[] = "sound_chouma.mp3";//玩牌时有玩家选择加注、跟注的音效 by hxh
	const static char s_dzpk_dachouma[] = "sound_dachouma.mp3";//胜利的玩家筹码移动 by hxh
	const static char s_dzpk_sound_chip[] = "dzpk_sound_chip.mp3";//单个筹码
	const static char s_dzpk_sound_chips[] = "dzpk_sound_chips.mp3";//多个筹码
	const static char s_dzpk_sound_clock[] = "dzpk_sound_clock.mp3";//时钟
	const static char s_dzpk_daojishi[] = "sound_daojishi.mp3";//玩牌时还剩5秒时播放的音效 by hxh
	const static char s_dzpk_sound_deal[] = "dzpk_sound_deal.mp3";//
	const static char s_dzpk_sound_pass[] = "dzpk_sound_pass.mp3";//
	const static char s_dzpk_sound_show_hand[] = "dzpk_sound_show_hand.mp3";//
	const static char s_dzpk_sound_take_chips[] = "dzpk_sound_take_chips.mp3";//
	const static char s_ct_sanshun[] = "ct_sanshun.mp3";//三顺
	const static char s_dzpk_b_show_card[] = "dzpk_b_show_card.mp3";//请亮牌
}

class SoundControl
{
public:
	SoundControl();
	~SoundControl();
	static SoundControl * sharedSoundControl();		//获得播放声音的单例
	void playSoundEffect(const char * soundName);	//播放音效
	void playBackGroundMusic(const char * soundName,bool loop);	//播放背景音乐
	void coloseBackGroundMusic();
	void setVoiceIsOpen(bool var);					//声音开关
	void setSoundEffVol(float vol);					//音效音量
	void setMusicVol(float vol);						//背景音乐音量

private:
	bool m_voiceIsOpen;		//true表示关闭声音
	float m_soundEffVol;
	float m_musicVol;
};

struct Tag 
{
	unsigned int userId;//用户id
	unsigned int gender;//性别
};

class SoundEffect
{
public:
	SoundEffect();
	~SoundEffect();
	static SoundEffect * sharedSoundEffect();
	void playEffect(const char * soundName,int userId);
	void playPassEffect(int userId);
	void playDaPaiEffect(int userId);
	void initLeftTag(Tag left);
	void initRightTag(Tag right);
	void initSelfTag(Tag self);

public:
	Tag m_leftUser;//上家
	Tag m_self;//自己
	Tag m_rightUser;//下家
};

#endif
//  [4/9/2014 xieyi]
#ifndef DDZ___SOUNDCONTROL__
#define DDZ___SOUNDCONTROL__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

//所有音效资源
namespace DDZsound_name
{
	const static char s_bgMusic[] = "roomback.mp3";//背景音乐
// 	//男声
// 	const static char s_man_wav0[] = "soundeffect/man/wav0.wav";//大侠手下留情
// 	const static char s_man_wav1[] = "soundeffect/man/wav1.wav";//多打牌少发呆。。。
// 	const static char s_man_wav2[] = "soundeffect/man/wav2.wav";//坑爹呢
// 	const static char s_man_wav3[] = "soundeffect/man/wav3.wav";//牌不在好。。。
// 	const static char s_man_wav4[] = "soundeffect/man/wav4.wav";//我要开始发威了。。。
// 	const static char s_man_wav5[] = "soundeffect/man/wav5.wav";//西边的太阳。。。
// 	const static char s_man_bujiabei[] = "soundeffect/man/bujiabei.wav";//不加倍
// 	const static char s_man_bujiao[] = "soundeffect/man/bujiao.wav";//不叫
// 	const static char s_man_buqiang[] = "soundeffect/man/buqiang.wav";//不抢
// 	const static char s_man_buyao[] = "soundeffect/man/buyao.wav";//不要
// 	const static char s_man_dani[] = "soundeffect/man/dani.wav";//大你
// 	const static char s_man_feiji[] = "soundeffect/man/feiji.wav";//飞机
// 	const static char s_man_guo[] = "soundeffect/man/guo.wav";//过
// 	const static char s_man_huojian[] = "soundeffect/man/huojian.wav";//huojian
// 	const static char s_man_jiabei[] = "soundeffect/man/jiabei.wav";//加倍
// 	const static char s_man_jiaodizhu[] = "soundeffect/man/jiaodizhu.wav";//叫地主
// 	const static char s_man_liandui[] = "soundeffect/man/liandui.wav";//连队
// 	const static char s_man_mingpai[] = "soundeffect/man/mingpai.wav";//明牌
// 	const static char s_man_qiangdizhu[] = "soundeffect/man/qiangdizhu.wav";//抢地主
// 	const static char s_man_sandaiyi[] = "soundeffect/man/sandaiyi.wav";//三带一
// 	const static char s_man_sandaier[] = "soundeffect/man/sandaier.wav";//三带一对
// 	const static char s_man_sanshun[] = "soundeffect/man/sanshun.wav";//三顺
// 	const static char s_man_sanzhang[] = "soundeffect/man/sanzhang.wav";//三张
// 	const static char s_man_shunzi[] = "soundeffect/man/shunzi.wav";//顺子
// 	const static char s_man_sidaier[] = "soundeffect/man/sidaier.wav";//四代二
// 	const static char s_man_yasi[] = "soundeffect/man/yasi.wav";//压死
// 	const static char s_man_yaobuqi[] = "soundeffect/man/yaobuqi.wav";//要不起
// 	const static char s_man_yidui[] = "soundeffect/man/yidui.wav";//一对
// 	const static char s_man_yizhang[] = "soundeffect/man/yizhang.wav";//一张
// 	const static char s_man_zhadan[] = "soundeffect/man/zhadan.wav";//炸弹

	//女声

// 	const static char s_woman_wav0[] = "soundeffect/woman/wav0.wav";//大侠手下留情
// 	const static char s_woman_wav1[] = "soundeffect/woman/wav1.wav";//多打牌少发呆。。。
// 	const static char s_woman_wav2[] = "soundeffect/woman/wav2.wav";//坑爹呢
// 	const static char s_woman_wav3[] = "soundeffect/woman/wav3.wav";//牌不在好。。。
// 	const static char s_woman_wav4[] = "soundeffect/woman/wav4.wav";//我要开始发威了。。。
// 	const static char s_woman_wav5[] = "soundeffect/woman/wav5.wav";//西边的太阳。。。
// 	const static char s_woman_bujiabei[] = "soundeffect/woman/bujiabei.wav";//不加倍
// 	const static char s_woman_bujiao[] = "soundeffect/woman/bujiao.wav";//不叫
// 	const static char s_woman_buqiang[] = "soundeffect/woman/buqiang.wav";//不抢
// 	const static char s_woman_buyao[] = "soundeffect/woman/buyao.wav";//不要
// 	const static char s_woman_dani[] = "soundeffect/woman/dani.wav";//大你
// 	const static char s_woman_feiji[] = "soundeffect/woman/feiji.wav";//飞机
// 	const static char s_woman_guo[] = "soundeffect/woman/guo.wav";//过
// 	const static char s_woman_huojian[] = "soundeffect/woman/huojian.wav";//huojian
// 	const static char s_woman_jiabei[] = "soundeffect/woman/jiabei.wav";//加倍
// 	const static char s_woman_jiaodizhu[] = "soundeffect/woman/jiaodizhu.wav";//叫地主
// 	const static char s_woman_liandui[] = "soundeffect/woman/liandui.wav";//连队
// 	const static char s_woman_mingpai[] = "soundeffect/woman/mingpai.wav";//明牌
// 	const static char s_woman_qiangdizhu[] = "soundeffect/woman/qiangdizhu.wav";//抢地主
// 	const static char s_woman_sandaiyi[] = "soundeffect/woman/sandaiyi.wav";//三带一
// 	const static char s_woman_sandaier[] = "soundeffect/woman/sandaier.wav";//三带一对
// 	const static char s_woman_sanshun[] = "soundeffect/woman/sanshun.wav";//三顺
// 	const static char s_woman_sanzhang[] = "soundeffect/woman/sanzhang.wav";//三张
// 	const static char s_woman_shunzi[] = "soundeffect/woman/shunzi.wav";//顺子
// 	const static char s_woman_sidaier[] = "soundeffect/woman/sidaier.wav";//四代二
// 	const static char s_woman_yasi[] = "soundeffect/woman/yasi.wav";//压死
// 	const static char s_woman_yaobuqi[] = "soundeffect/woman/yaobuqi.wav";//要不起
// 	const static char s_woman_yidui[] = "soundeffect/woman/yidui.wav";//一对
// 	const static char s_woman_yizhang[] = "soundeffect/woman/yizhang.wav";//一张
// 	const static char s_woman_zhadan[] = "soundeffect/woman/zhadan.wav";//炸弹

	const static char s_wav0[] = "wav0.mp3";//大侠手下留情
	const static char s_wav1[] = "wav1.mp3";//多打牌少发呆。。。
	const static char s_wav2[] = "wav2.mp3";//坑爹呢
	const static char s_wav3[] = "wav3.mp3";//牌不在好。。。
	const static char s_wav4[] = "wav4.mp3";//我要开始发威了。。。
	const static char s_wav5[] = "wav5.mp3";//西边的太阳。。。
	const static char s_bujiabei[] = "bujiabei.mp3";//不加倍
	const static char s_bujiao[] = "bujiao.mp3";//不叫
	const static char s_buqiang[] = "buqiang.mp3";//不抢
	const static char s_buyao[] = "buyao.mp3";//不要
	const static char s_dani[] = "dani.mp3";//大你
	const static char s_feiji[] = "feiji.mp3";//飞机
	const static char s_guo[] = "guo.mp3";//过
	const static char s_huojian[] = "huojian.mp3";//huojian
	const static char s_jiabei[] = "jiabei.mp3";//加倍
	const static char s_jiaodizhu[] = "jiaodizhu.mp3";//叫地主
	const static char s_liandui[] = "liandui.mp3";//连队
	const static char s_mingpai[] = "mingpai.mp3";//明牌
	const static char s_qiangdizhu[] = "qiangdizhu.mp3";//抢地主
	const static char s_sandaiyi[] = "sandaiyi.mp3";//三带一
	const static char s_sandaier[] = "sandaier.mp3";//三带一对
	const static char s_sanshun[] = "sanshun.mp3";//三顺
	const static char s_sanzhang[] = "sanzhang.mp3";//三张
	const static char s_shunzi[] = "shunzi.mp3";//顺子
	const static char s_sidaier[] = "sidaier.mp3";//四代二
	const static char s_yasi[] = "yasi.mp3";//压死
	const static char s_yaobuqi[] = "yaobuqi.mp3";//要不起
	const static char s_yidui[] = "yidui.mp3";//一对
	const static char s_yizhang[] = "yizhang.mp3";//一张
	const static char s_zhadan[] = "zhadan.mp3";//炸弹

	//牌型音效
	const static char s_ct_fapai[] = "soundeffect/ct_fapai.mp3";//发牌
	const static char s_ct_feiji[] = "soundeffect/ct_feiji.mp3";//飞机
	const static char s_ct_huojian[] = "soundeffect/ct_huojian.mp3";//火箭
	const static char s_ct_liandui[] = "soundeffect/ct_liandui.mp3";//连对
	const static char s_ct_sanshun[] = "soundeffect/ct_sanshun.mp3";//三顺
	const static char s_ct_zhadan[] = "soundeffect/ct_zhadan.mp3";//炸弹
	const static char s_ct_shunzi[] = "soundeffect/ct_shunzi.mp3";//顺子
	//结算音效
	const static char s_winner[] = "soundeffect/winner.mp3";//胜利
	const static char s_loser[] = "soundeffect/loser.mp3";//失败
	//警报音效
	const static char s_warning[] = "soundeffect/warning.mp3";//警报

}

class DDZSoundControl
{
public:
	DDZSoundControl();
	~DDZSoundControl();
	static DDZSoundControl * sharedDDZSoundControl();		//获得播放声音的单例
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

struct DDZTag 
{
	unsigned int userId;//用户id
	unsigned int gender;//性别
};

class DDZSoundEffect
{
public:
	DDZSoundEffect();
	~DDZSoundEffect();
	static DDZSoundEffect * sharedSoundEffect();
	void playEffect(const char * soundName,int userId);
	void playPassEffect(int userId);
	void playDaPaiEffect(int userId);
	void initLeftTag(DDZTag left);
	void initRightTag(DDZTag right);
	void initSelfTag(DDZTag self);

public:
	DDZTag m_leftUser;//上家
	DDZTag m_self;//自己
	DDZTag m_rightUser;//下家
};

#endif
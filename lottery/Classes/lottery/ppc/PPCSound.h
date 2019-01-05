#ifndef _PPC_SOUND_H_
#define _PPC_SOUND_H_

//声音引擎
#include "SimpleAudioEngine.h"
#include "PPCEnum.h"

#define PPC_MUSICE_SWITCH  "ppc_musice_switch";
#define PPC_EFFECT_SWITCH  "ppc_effect_switch";

//把用到的音乐做为枚举类型
enum PPC_MUSIC_TYPE
{
	//游戏的背景音乐 music_ground.mp3
	MUSIC_TYPE_BACKGROUND1 = 50,					//下注状态背景音乐
	MUSIC_TYPE_BACKGROUND2,
	MUSIC_TYPE_BACKGROUND3,
	MUSIC_TYPE_BACKGROUND4,
	MUSIC_TYPE_BACKGROUND5,
	MUSIC_TYPE_BACKGROUND6,
	MUSIC_TYPE_BACKGROUND7,
	MUSIC_TYPE_BACKGROUND8,
	MUSIC_TYPE_BACKGROUND9,
	MUSIC_TYPE_BACKGROUND_GAME_FREE,				//空闲状态背景音乐
	EFFECT_TYPE_KAICHEYINYUE						//开车过程中音乐
};

enum PPC_EFFECT
{
	//游戏中的声音
	EFFECT_TYPE_ADD_GOLD = 100,		//下注
	EFFECT_TYPE_ADD_GOLD_EX,     //下注 + 惊叹声
	EFFECT_TYPE_CHEER1,				//欢呼
	EFFECT_TYPE_CHEER2,
	EFFECT_TYPE_CHEER3,
	EFFECT_TYPE_DISPATCH_CARD,
	EFFECT_TYPE_END_DRAW,
	EFFECT_TYPE_END_LOST,			//失败
	EFFECT_TYPE_END_WIN,			//胜利
	EFFECT_TYPE_GAME_START,			//车开始跑动
	EFFECT_TYPE_IDC_SND,			//刹车声
	//EFFECT_TYPE_KAICHEYINYUE,		//开车过程中音乐
	EFFECT_TYPE_PLEASE_BET,				//请下注
	EFFECT_TYPE_PLEASE_SPEAK,			//请说话
	EFFECT_TYPE_TIME_WARIMG			//时间快到时，警报
};

//声音处理类
class PPCSound
{
public:
	static PPCSound * getInstance();
	static void freeInstance();
	const char * getMusic(PPC_MUSIC_TYPE type);//捕鱼背景音乐

private:
	static PPCSound * m_sound;
	PPCSound();
	//根据不同的音乐和音效类型获得相应平台下的文件	
	const char * getEffect(PPC_EFFECT type);//捕鱼音效

	EPPCProcessType m_type;

public:
	//声音的播放和停止

	void playBgMusic(PPC_MUSIC_TYPE type);
	void playEffect(PPC_EFFECT type);

	void  setBgMusicVolume(float volume); //设置背景音乐音量
	float getBgMusicVolume();             //获取背景音乐音量
	void  setEffectsVolume(float volume); //设置音效音量
	float getEffectsVolume(); //获取音效音量

	bool getEffectStatue();
	bool getMusicStatue();
public:
	void StopMusic();
	void StopEffect();

	//随机播放背景音乐
	void playBgMusicRandom();

	//播放游戏开始音效
	void playGameStartEffects();

	//游戏结束时的音效播放
	void playGameEndEffects();

	//动画停止时音效播放
	void playEffectShowResult(bool win);

	//游戏空闲状态播放的音乐
	void playMusiceOnGameFree();

	//根据当前游戏阶段，播放对应的背景音乐
	void playMusicByState();
};
#endif // _SOUND_H_

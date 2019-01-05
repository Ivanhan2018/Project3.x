#ifndef _SOUND_H_
#define _SOUND_H_

//声音引擎
#include "SimpleAudioEngine.h"
//把用到的音乐做为枚举类型
enum MUSIC_TYPE
{
	//游戏的背景音乐 music_ground.mp3
	MUSIC_TYPE_SceneBg,
	MUSIC_TYPE_BACKGROUND1 = 50,
	MUSIC_TYPE_BACKGROUND2,
	MUSIC_TYPE_BACKGROUND3,
	MUSIC_TYPE_BACKGROUND4,
};

enum EFFECT_FOUR_FISH_TYPE
{
	//游戏中的声音
	EFFECT_TYPE_BINGO = 100,		//bingo
	EFFECT_TYPE_CANNON_SWITCH,     //切换炮
	EFFECT_TYPE_CATCH,				//抓住
	EFFECT_TYPE_FIRE,				//开火
	EFFECT_TYPE_FISH10_1,
	EFFECT_TYPE_FISH10_2,
	EFFECT_TYPE_FISH11_1,
	EFFECT_TYPE_FISH11_2,
	EFFECT_TYPE_FISH12_1,
	EFFECT_TYPE_FISH12_2,
	EFFECT_TYPE_FISH13_1,
	EFFECT_TYPE_FISH13_2,
	EFFECT_TYPE_FISH14_1,
	EFFECT_TYPE_FISH14_2,
	EFFECT_TYPE_FISH15_1,
	EFFECT_TYPE_FISH15_2,
	EFFECT_TYPE_FISH16_1,
	EFFECT_TYPE_FISH16_2,
	EFFECT_TYPE_FISH17_1,
	EFFECT_TYPE_FISH17_2,
	EFFECT_TYPE_GOLD,
	EFFECT_TYPE_INSERT,
	EFFECT_TYPE_ION_CATCH,
	EFFECT_TYPE_ION_FIRE,
	EFFECT_TYPE_LOCK,
	EFFECT_TYPE_WAVE,
	EFFECT_TYPE_coinsky,
	EFFECT_TYPE_Unknow,
	EFFECT_TYPE_SILVER			//上下分音效
};

//声音处理类
class Sound
{
public:
	static Sound * getInstance();
	static void freeInstance();
	const char * getMusic(MUSIC_TYPE type);//捕鱼背景音乐

private:
	static Sound * m_sound;
	Sound();
	//根据不同的音乐和音效类型获得相应平台下的文件	
	const char * getEffect(EFFECT_FOUR_FISH_TYPE type);//捕鱼音效

public:
	//声音的播放和停止

	void playBgMusic(MUSIC_TYPE type);
	void playEffect(EFFECT_FOUR_FISH_TYPE type);

	void  setBgMusicVolume(float volume); //设置背景音乐音量
	float getBgMusicVolume();             //获取背景音乐音量
	void  setEffectsVolume(float volume); //设置音效音量
	float getEffectsVolume(); //获取音效音量

	bool getEffectStatue();
	bool getMusicStatue();
public:
	void StopMusic();
	void StopEffect();

};
#endif // _SOUND_H_

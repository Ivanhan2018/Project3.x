#ifndef __SGLY_MANAGE_AUDIO_H__
#define __SGLY_MANAGE_AUDIO_H__

#include "cocos2d.h"
#include "SGLYDefine.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

/*
**	@brief	水果乐园——音频管理
*/
class SGLYManageAudio
{
public:
	SGLYManageAudio();
	~SGLYManageAudio();

	//	单例
	static SGLYManageAudio* getInstance();

	//	预加载音频文件
	void preLoadAudio();

	//	清除缓存
	void clear();

	//	播放背景音乐
	void playBGMusic(bool bLoop = false);

	//	停止播放背景音乐
	void stopBGMusic();

	//	播放音效
	void playSoundEffect(E_SGLY_Audio eSound, bool bLoop = false);

	//	停止播放所有音效
	void stopAllSoundEffect();

	//	设置背景音乐音量
	void setBGMusicVolume(float fVolume);

	//	设置音效音量
	void setSoundEffectVolume(float fVolume);

	//	设置是否开启背景音乐
	void setOnBGMusic(bool bIsOn);

	//	获得当前是否开启背景音乐
	bool getOnBGMusic() const;

	//	设置是否开启音效
	void setOnSoundEffect(bool bIsOn);

	//	获得当前是否开启音效
	bool getOnSoundEffect() const;

private:
	bool init();

	//	是否开启背景音乐
	bool m_bOnBGMusic;

	//	是否开启音效
	bool m_bOnSoundEffect;

	//	音频文件名
	string m_sAudioFile[E_SGLY_Audio_Null];
};


#endif  // __SGLY_MANAGE_AUDIO_H__
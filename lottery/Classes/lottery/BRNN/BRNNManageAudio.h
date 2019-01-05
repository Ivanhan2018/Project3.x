#ifndef __BRNN_MANAGE_AUDIO_H__
#define __BRNN_MANAGE_AUDIO_H__

#include "cocos2d.h"
#include "BRNNDefine.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

/*
**	@brief	百人牛牛——音频管理
*/
class BRNNManageAudio
{
public:
	BRNNManageAudio();
	~BRNNManageAudio();

	//	单例
	static BRNNManageAudio* getInstance();

	//	预加载音频文件
	void preLoadAudio();

	//	清除缓存
	void clear();

	//	播放背景音乐
	void playBGMusic(bool bLoop = false);

	//	停止播放背景音乐
	void stopBGMusic();

	//	播放音效
	void playSoundEffect(E_BRNNAudio eSound, bool bLoop = false);

	//	停止播放所有音效
	void stopAllSoundEffect();

	//	设置背景音乐音量
	void setBGMusicVolume(float fVolume);

	//	设置音效音量
	void setSoundEffectVolume(float fVolume);

	//	设置是否打开背景音乐
	CC_SYNTHESIZE(bool, m_bOnBGMusic, OnBGMusic);

	//	设置是否打开音效
	CC_SYNTHESIZE(bool, m_bOnSoundEffect,OnSoundEffect);

private:
	bool init();

	//	音频文件名
	char* m_sAudioFile[E_BRNNAudio_Null];
};


#endif  // __BRNN_MANAGE_AUDIO_H__
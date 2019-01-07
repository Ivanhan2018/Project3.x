#include "BRNNManageAudio.h"

//	“Ù∆µπ‹¿Ì¿‡µ•¿˝∂‘œÛ
static BRNNManageAudio* m_sSharedManageAudio = nullptr;

BRNNManageAudio::BRNNManageAudio()
{
}

BRNNManageAudio::~BRNNManageAudio()
{
}

BRNNManageAudio* BRNNManageAudio::getInstance()
{
	if (!m_sSharedManageAudio)
	{
		m_sSharedManageAudio = new (std::nothrow)BRNNManageAudio();
		CCASSERT(m_sSharedManageAudio, "FATAL: Not enough memory");
		m_sSharedManageAudio->init();
	}

	return m_sSharedManageAudio;
}

//	‘§º”‘ÿ“Ù∆µŒƒº˛
void BRNNManageAudio::preLoadAudio()
{
	//	±≥æ∞“Ù¿÷
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(m_sAudioFile[E_BRNNAudio_BGMusic]);

	//	“Ù–ß
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_Change_Banker]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_Countdown]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_Game_Start]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_Bet]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_Lose]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_Win]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_0]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_1]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_2]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_3]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_4]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_5]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_6]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_7]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_8]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_9]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_10]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_11]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_12]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_13]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_14]);
	SimpleAudioEngine::getInstance()->preloadEffect(m_sAudioFile[E_BRNNAudio_NN_15]);
}

//	«Â≥˝ª∫¥Ê
void BRNNManageAudio::clear()
{
	SimpleAudioEngine::getInstance()->end();
}

//	≤•∑≈±≥æ∞“Ù¿÷
void BRNNManageAudio::playBGMusic(bool bLoop)
{
	if (!getOnBGMusic())
	{
		//	Ω˚”√“Ù¿÷
		return;
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic(m_sAudioFile[E_BRNNAudio_BGMusic], bLoop);
}

//	Õ£÷π≤•∑≈±≥æ∞“Ù¿÷
void BRNNManageAudio::stopBGMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

//	≤•∑≈“Ù–ß
void BRNNManageAudio::playSoundEffect(E_BRNNAudio eSound, bool bLoop)
{
	if (eSound < E_BRNNAudio_BGMusic && eSound > E_BRNNAudio_Null)
	{
		return;
	}
	if (!getOnSoundEffect())
	{
		//	Ω˚”√“Ù–ß
		return;
	}
	SimpleAudioEngine::getInstance()->playEffect(m_sAudioFile[eSound], bLoop);
}

//	Õ£÷π≤•∑≈À˘”–“Ù–ß
void BRNNManageAudio::stopAllSoundEffect()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

//	…Ë÷√±≥æ∞“Ù¿÷“Ù¡ø
void BRNNManageAudio::setBGMusicVolume(float fVolume)
{
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(fVolume);
}

//	…Ë÷√“Ù–ß“Ù¡ø
void BRNNManageAudio::setSoundEffectVolume(float fVolume)
{
	SimpleAudioEngine::getInstance()->setEffectsVolume(fVolume);
}

bool BRNNManageAudio::init()
{
	//	“Ù∆µŒƒº˛

	m_sAudioFile[E_BRNNAudio_BGMusic] = "brnn_bgm.wav";
	m_sAudioFile[E_BRNNAudio_Change_Banker] = "brnn_change_banker.wav";
	m_sAudioFile[E_BRNNAudio_Countdown] = "brnn_count_down.wav";
	m_sAudioFile[E_BRNNAudio_Game_Start] = "brnn_game_start.wav";
	m_sAudioFile[E_BRNNAudio_Bet] = "brnn_bet.wav";
	m_sAudioFile[E_BRNNAudio_Lose] = "brnn_lose.wav";
	m_sAudioFile[E_BRNNAudio_Win] = "brnn_win.wav";
	m_sAudioFile[E_BRNNAudio_NN_0] = "OX_0_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_1] = "OX_1_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_2] = "OX_2_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_3] = "OX_3_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_4] = "OX_4_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_5] = "OX_5_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_6] = "OX_6_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_7] = "OX_7_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_8] = "OX_8_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_9] = "OX_9_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_10] = "OX_10_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_11] = "OX_10_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_12] = "OX_10_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_13] = "OX_10_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_14] = "OX_14_BANKER_SOUND.mp3";
	m_sAudioFile[E_BRNNAudio_NN_15] = "OX_15_BANKER_SOUND.mp3";

	bool bBanBGM = UserDefault::getInstance()->getBoolForKey(BRNN_DATA_KEY_AUDIO_ON_BGM, true);
	bool bBanSoundEffect = UserDefault::getInstance()->getBoolForKey(BRNN_DATA_KEY_AUDIO_ON_SOUND_EFFECT, true);
	setOnBGMusic(bBanBGM);
	setOnSoundEffect(bBanSoundEffect);

	return true;
}
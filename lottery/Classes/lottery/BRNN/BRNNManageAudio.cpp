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

	bool bBanBGM = UserDefault::getInstance()->getBoolForKey(BRNN_DATA_KEY_AUDIO_ON_BGM, true);
	bool bBanSoundEffect = UserDefault::getInstance()->getBoolForKey(BRNN_DATA_KEY_AUDIO_ON_SOUND_EFFECT, true);
	setOnBGMusic(bBanBGM);
	setOnSoundEffect(bBanSoundEffect);

	return true;
}
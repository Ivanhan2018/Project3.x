//
//  Sound.cpp
//  TowerDefense
//
//  Created by peniel on 12/19/13.
//
//

#include "Sound.h"
#include "PlayerModel.h"
static Sound* instance = NULL;


Sound::Sound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(HALL_BG_MUSIC);
    
    /*
     sound_chouma      玩牌时有玩家选择加注，跟注的音效（所有人都可听）
     sound_chuangkou   点击任意窗口展开
     sound_chupai      轮到我方行动
     sound_chuxian     老虎机摇中一个图案
     sound_dachouma    胜利的玩家筹码移动
     sound_daojishi    玩牌时还剩5秒时播放的音效
     sound_dianji      点击任意按钮
     sound_fapai       玩牌时一开始的发牌
     sound_gangjinru   玩家刚加入玩牌时的音效（包括换桌，坐下，快速玩牌）
     sound_guagua      刮刮乐的音效
     sound_guanbi      关闭窗口的音效
     sound_guzhang     玩家自己胜利的音效（仅自己可听）
     sound_jieguo      老虎机所有图案出现后的结果音效
     sound_kanpai      玩牌时有玩家选择看牌（所有人都可听）
     sound_lagan       老虎机拉缸一瞬间的音效
     sound_qipai       玩牌时有玩家选择弃牌（所有人都可听）
     sound_quanya      玩牌时有玩家选择全押（所有人都可听）
     sound_shibai      玩家自己失败的音效（仅自己可听）
     sound_zhuandong   老虎机转动过程的声音（截止到三个结果出现，否则始终循环播放）
     */
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_chouma.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_chuangkou.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_chupai.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_chuxian.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_dachouma.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_daojishi.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_dianji.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_fapai.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_gangjinru.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_guagua.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_guanbi.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_guzhang.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_jieguo.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_kanpai.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_lagan.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_qipai.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_quanya.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_shibai.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/sound_zhuandong.ogg");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(3.0f);
}

Sound::~Sound()
{
    
}

Sound* Sound::getInstance()
{
    if (instance == NULL) {
        instance = new Sound();
    }
    
    return instance;
}

void Sound::PlayBgSound()
{
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    
    PlayerModel::getInstance()->vocieSwitch = pUserDefault->getBoolForKey(SYSTEM_VOLUME_SWITCH_KEY,true);
    PlayerModel::getInstance()->vocieEffectSwitch = pUserDefault->getBoolForKey(SYSTEM_VOLUME_EFFECT_SWITCH_KEY,true);
    //PlayerModel::getInstance()->vocieSnakeSwitch = pUserDefault->getBoolForKey(SYSTEM_VOLUME_SNAKE_SWITCH_KEY,false);
    PlayerModel::getInstance()->sound = pUserDefault->getFloatForKey(SYSTEM_VOLUME_VALUE_KEY,0.5f);
    
    // set default volume
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(PlayerModel::getInstance()->sound);
    
    if (true == PlayerModel::getInstance()->vocieSwitch)
    {
        if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(HALL_BG_MUSIC,true);
        }
    }
}

void Sound::StopBgSound()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

UNS_INT Sound::playEffect(const char* fileName, bool isLoop)
{
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    PlayerModel::getInstance()->vocieEffectSwitch = pUserDefault->getBoolForKey(SYSTEM_VOLUME_EFFECT_SWITCH_KEY,true);
    
    if (true == PlayerModel::getInstance()->vocieEffectSwitch)
    {
        return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(fileName, isLoop);
    }
    return 0;
}

void Sound::stopEffect(unsigned int nSoundId)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
}

void Sound::stopAllEffect()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void Sound::resumeAllEffect()
{
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    PlayerModel::getInstance()->vocieEffectSwitch = pUserDefault->getBoolForKey(SYSTEM_VOLUME_EFFECT_SWITCH_KEY,true);
    
    if (true == PlayerModel::getInstance()->vocieEffectSwitch)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
}














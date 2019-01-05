//
//  Sound.h
//  TowerDefense
//
//  Created by peniel on 12/19/13.
//
//

#ifndef __TowerDefense__Sound__
#define __TowerDefense__Sound__

#include "Inc.h"

class Sound : public CCObject
{
    
private:
	Sound();
    
public:
	~Sound();
   
public:
    static Sound* getInstance();
    
    void PlayBgSound();
    void StopBgSound();
    
    UNS_INT playEffect(const char* fileName, bool isLoop=false);
    void stopEffect(unsigned int nSoundId);
    
    void stopAllEffect();
    void resumeAllEffect();
    
};

#endif /* defined(__TowerDefense__Sound__) */

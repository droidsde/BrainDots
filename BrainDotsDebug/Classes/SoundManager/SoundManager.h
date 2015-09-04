//
//  SoundManager.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/04.
//
//

#ifndef __BrainDotsDebug__SoundManager__
#define __BrainDotsDebug__SoundManager__

#include <cocos2d.h>
#include "SimpleAudioEngine.h"
#include "ManageSoundDelegate.h"

USING_NS_CC;

class SoundManager : public ManageSoundDelegate
{
public:
    SoundManager();
    ~SoundManager();
    
    static SoundManager* getInstance();
    
    CC_SYNTHESIZE(bool, music, MusicOn);
    CC_SYNTHESIZE(bool, sound, SoundOn);
    
    virtual void playBackgroundMusic() override;
    
    virtual void resumeBackgroundMusic() override;
    
    virtual void pauseBackgroundMusic() override;
    
    virtual void playClickSound() override;
    
    virtual void playMoveSound() override;
    
    virtual void playAddScoreSound() override;
    
    virtual bool isMusicOn() override;
    
    virtual bool isSoundOn() override;
    
    void preLoad();
    
    void unLoad(std::string);
    
private:
    
    static SoundManager* _soundManager;
    CocosDenshion::SimpleAudioEngine* audioEngine;
    
};

#endif /* defined(__BrainDotsDebug__SoundManager__) */

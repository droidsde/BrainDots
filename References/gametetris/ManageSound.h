//
//  ManageSound.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef __GameTetris__ManageSound__
#define __GameTetris__ManageSound__

#include <cocos2d.h>
#include "SimpleAudioEngine.h"
#include "ManageSoundDelegate.h"

USING_NS_CC;

class ManageSound: public ManageSoundDelegate
{
    
public:
    
    ManageSound();
    
    ~ManageSound();
    
    static ManageSound* getInstance();
    
    CC_SYNTHESIZE(bool, music, MusicOn);
    
    CC_SYNTHESIZE(bool, sound, SoundOn);
    
    virtual void playBackgroundMusic() override;
    
    virtual void resumeBackgroundMusic() override;
    
    virtual void pauseBackgroundMusic() override;
    
    virtual void playClickSound() override;
    
    virtual void playMoveSound() override;
    
    virtual void playMoveLeftRightSound() override;
    
    virtual void playHardDropSound() override;
    
    virtual void playRotateSound() override;
    
    virtual void playLockOnSound() override;
    
    virtual void playClearLineSound() override;
    
    virtual void playAddScoreSound() override;
    
    virtual bool isMusicOn() override;
    
    virtual bool isSoundOn() override;
    
    void preLoad();
    
    void unLoad(std::string);
    
private:
    
    static ManageSound* _manageSound;
    CocosDenshion::SimpleAudioEngine* audioEngine;
};

#endif /* defined(__GameTetris__ManageSound__) */

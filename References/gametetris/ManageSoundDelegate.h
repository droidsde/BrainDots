//
//  ManageSoundDelegate.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef GameTetris_ManageSoundDelegate_h
#define GameTetris_ManageSoundDelegate_h

#pragma once

class ManageSoundDelegate
{
public:
    
    virtual void playBackgroundMusic() = 0;
    
    virtual void resumeBackgroundMusic() = 0;
    
    virtual void pauseBackgroundMusic() = 0;
    
    virtual void playClickSound() = 0;
    
    virtual void playMoveSound() = 0;
    
    virtual void playMoveLeftRightSound() = 0;
    
    virtual void playHardDropSound() = 0;
    
    virtual void playRotateSound() = 0;
    
    virtual void playLockOnSound() = 0;
    
    virtual void playClearLineSound() = 0;
    
    virtual void playAddScoreSound() = 0;
    
    virtual bool isMusicOn() = 0;
    
    virtual bool isSoundOn() = 0;
};

#endif

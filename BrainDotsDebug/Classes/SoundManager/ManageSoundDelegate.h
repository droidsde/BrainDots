//
//  ManageSoundDelegate.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/04.
//
//

#ifndef BrainDotsDebug_ManageSoundDelegate_h
#define BrainDotsDebug_ManageSoundDelegate_h

#pragma once

class ManageSoundDelegate
{
public:
    
    virtual void playBackgroundMusic() = 0;
    
    virtual void resumeBackgroundMusic() = 0;
    
    virtual void pauseBackgroundMusic() = 0;
    
    virtual void playClickSound() = 0;
    
    virtual void playMoveSound() = 0;
    
    virtual void playAddScoreSound() = 0;
    
    virtual bool isMusicOn() = 0;
    
    virtual bool isSoundOn() = 0;
    
};

#endif

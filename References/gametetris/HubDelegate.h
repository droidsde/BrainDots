//
//  HubDelegate.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/23.
//
//

#pragma once

class HubDelegate
{
public:
    virtual void addLineHub(int) = 0;
    
    virtual void addScoreHub() = 0;
    
    virtual void addSpeedHub() = 0;
    
    virtual void addComboHub() = 0;
    
    virtual void onHoldCallback() = 0;
    
    virtual void onPauseCallback() = 0;
    
    virtual void onResumeCallback() = 0;
    
    virtual void removeTouch() = 0;
};

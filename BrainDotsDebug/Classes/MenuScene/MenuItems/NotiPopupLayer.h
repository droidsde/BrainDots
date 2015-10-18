//
//  NotiPopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/16.
//
//

#ifndef __BrainDotsDebug__NotiPopupLayer__
#define __BrainDotsDebug__NotiPopupLayer__

#include <cocos2d.h>
#include "SceneManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Network/ClientRequest.h"
#include "MenuScene/BasePopupLayer.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ClientRequest.h"
#include "BasePopupLayer.h"
#endif

class NotiPopupLayer : public BasePopupLayer
{
public:
    NotiPopupLayer();
    ~NotiPopupLayer();
    
    static NotiPopupLayer* create();
    bool init();
    
    void onEnter();
    void onExit();
    
private:
    Size layoutSize;
};

#endif /* defined(__BrainDotsDebug__NotiPopupLayer__) */

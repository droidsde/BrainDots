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
#include "BasePopupLayer.h"
#include "SceneManager.h"
#include "ClientRequest.h"

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

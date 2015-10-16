//
//  InfoPopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/16.
//
//

#ifndef __BrainDotsDebug__InfoPopupLayer__
#define __BrainDotsDebug__InfoPopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"
#include "SceneManager.h"
#include "ClientRequest.h"

class InfoPopupLayer : public BasePopupLayer
{
public:
    InfoPopupLayer();
    ~InfoPopupLayer();
    
    static InfoPopupLayer* create();
    bool init();
    
    void onExit();
    
    void textFieldEvent(Ref *pSender, TextField::EventType type);
    
private:
    Size layoutSize;
    Size listviewSize;
    
    void createLayoutItem(Vec2 pos, std::string iconFile, std::string text);
};

#endif /* defined(__BrainDotsDebug__InfoPopupLayer__) */

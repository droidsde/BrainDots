//
//  PaperSprite.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/28.
//
//

#ifndef __BrainDotsDebug__PaperSprite__
#define __BrainDotsDebug__PaperSprite__

#include <cocos2d.h>

USING_NS_CC;

class PaperSprite : public Sprite
{
    
public:
    PaperSprite(bool isScale);
    ~PaperSprite();
    
    static PaperSprite* create(std::string filename, bool isScale);
    
    void initOptions();
    
    void addEvents();
    
    void touchEvent(Touch* touch);
    
private:
    bool isScale;
};
#endif /* defined(__BrainDotsDebug__PaperSprite__) */

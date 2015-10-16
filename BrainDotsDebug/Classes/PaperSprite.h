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
    PaperSprite(bool isScale, bool isCallbackFunction);
    ~PaperSprite();
    
    static PaperSprite* create(std::string filename, bool isFromPlist, bool isScale = false, bool isCallbackFunction = false);
    
    CC_SYNTHESIZE(bool, enableTouch, EnableTouch);
    void initOptions();
    
    void addEvents();
    
    void touchEvent(Touch* touch);
    
    void setCallback(Sequence* callback);
    
    void setCallbackFunction(std::function<void(void)> callback);
    
    // function gray
    void setShaderGray();
    
    void changeColorGray();
    
private:
    bool isScale;
    Sequence* _callback;
    std::function<void(void)> _callbackFunction;
    bool isCallbackFunction;
    
    // gray color
    GLProgram* m_Shader;
    float m_grayRate;
};
#endif /* defined(__BrainDotsDebug__PaperSprite__) */

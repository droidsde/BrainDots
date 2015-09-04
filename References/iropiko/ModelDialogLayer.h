//
//  ModelDialogLayer.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/17.
//
//

#ifndef __PoolExample__ModelDialogLayer__
#define __PoolExample__ModelDialogLayer__

#include <cocos2d.h>
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;

#define ModelDialogLayerTagMenu     101
#define ModelDialogLayerTagOK       102
#define ModelDialogLayerTagNG       103
#define ModelDialogLayerTagBG       110


class ModelDialogLayer : public LayerColor
{
public:
    static ModelDialogLayer* create(const char* message, Ref* target, SEL_CallFuncN okSelector, SEL_CallFuncN ngSelector);
    static ModelDialogLayer* create(const char* message, Ref* target, SEL_CallFuncN ngSelector);
    static ModelDialogLayer* create(const char* message, int timeLife);
    
    virtual bool init();
    void setStyle(const char* message, Ref* target, SEL_CallFuncN okSelector, SEL_CallFuncN ngSelector, int timeLife);
    
    void menuAlertOK(Ref* pSender, Widget::TouchEventType type);
    void menuAlertClose(Ref* pSender, Widget::TouchEventType type);
    
    CREATE_FUNC(ModelDialogLayer);

private:
    Ref* target;
    SEL_CallFuncN okSelector;
    SEL_CallFuncN ngSelector;
    Vec2 origin;
    cocos2d::Size visibleSize;
    cocos2d::Size bgSize;
    Sprite* frame;
    int timeLife;
};

#endif /* defined(__PoolExample__ModelDialogLayer__) */

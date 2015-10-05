//
//  TestButton.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/05.
//
//

#include "BlendFuncButton.h"

BlendFuncButton::BlendFuncButton()
{
    
}

BlendFuncButton::~BlendFuncButton()
{
    
}

BlendFuncButton* BlendFuncButton::create(std::string fileNormal)
{
    BlendFuncButton* button = new BlendFuncButton();
    if (button->init(fileNormal)){
        button->autorelease();
        return button;
    }
    else {
        return nullptr;
    }
}

void BlendFuncButton::setBlendFunc(const BlendFunc &blendFunc)
{
    this->_buttonNormalRenderer->setBlendFunc(blendFunc);
}
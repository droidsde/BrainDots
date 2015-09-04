//
//  GLViewUtil.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/07.
//
//

#ifndef PoolExample_GLViewUtil_h
#define PoolExample_GLViewUtil_h

#include "base/CCDirector.h"
#include "platform/CCGLView.h"

namespace GLViewUtil {
    
    enum class FitType {
        WIDTH,
        HEIGHT,
    };
    
    cocos2d::Size calcFitDesignResolutionSize(float width, float height, FitType fitType, float scale = 1.0f)
    {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        
        float baseScaleWidth  = width * scale;
        float baseScaleHeight = height * scale;
        
        int widthDiff = abs((int)visibleSize.width - (int)width);
        int heightDiff = abs((int)visibleSize.height - (int)height);
        
        if (widthDiff == 0 && heightDiff == 0) {
            return cocos2d::Size(baseScaleWidth, baseScaleHeight);
        }
        
        float divX = visibleSize.width / baseScaleWidth;
        float divY = visibleSize.height / baseScaleHeight;
        float fixWidth = widthDiff / divY;
        float fixHeight = heightDiff / divX;
        if (fitType == FitType::HEIGHT) {
            // priority width
            float addWidth = (heightDiff / divY * (width / height));
            return cocos2d::Size(baseScaleWidth + fixWidth - addWidth, baseScaleHeight);
        } else {
            // priority height
            float addheight = (widthDiff / divX * (height / width));
            return cocos2d::Size(baseScaleWidth, baseScaleHeight + fixHeight - addheight);
        }
    }
    
    cocos2d::Size getFitDesignResolutionSize(float width, float height, FitType fitType, float scale = 1.0f)
    {
        cocos2d::Size fitSize = GLViewUtil::calcFitDesignResolutionSize(width, height, FitType::WIDTH, scale);
        
        float baseScaleWidth  = width * scale;
        float baseScaleHeight = height * scale;
        
        if (fitType == FitType::HEIGHT && fitSize.width < baseScaleWidth) {
            return GLViewUtil::calcFitDesignResolutionSize(width, height, FitType::WIDTH, scale);
        } else if (fitType == FitType::WIDTH && fitSize.height < baseScaleHeight) {
            return GLViewUtil::calcFitDesignResolutionSize(width, height, FitType::HEIGHT, scale);
        }
        return fitSize;
    }
}


#endif

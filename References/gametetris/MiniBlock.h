//
//  MiniBlock.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#ifndef __GameTetris__Block__
#define __GameTetris__Block__

#include <cocos2d.h>
#include "Constants.h"
#include "BoardInterface.h"

USING_NS_CC;

class MiniBlock: public Sprite
{
  
public:
    
    virtual bool init() override;
    
    cocos2d::Point toBoardCoordinate();
    
    void switchShowing();
    
    void switchDarkShowing();
    
protected:
    
    BoardInterface* getGameBoard();
    
    virtual std::string getBlockFileName() = 0;
    
    CC_SYNTHESIZE_PASS_BY_REF(BlockDef, blockDef, BlockDefinition);
    
    void createByDef(const BlockDef&);
    
    cocos2d::Point parentCenter;
    
    cocos2d::Point getParentCenter();
    
    void  setParentCenter();
    
    bool isMeEmpty();
    
    bool turnLeftAble();
    
    bool turnRightAble();
    
    bool turnLeftAbleStep(int);
    
    bool turnRightAbleStep(int);

    bool turnDownAble();
    
    bool turnRotateAble(int = 0, int = 0);
    
    void resetOccupySituation();
    
    void resetShowingSituation();
};

#endif /* defined(__GameTetris__Block__) */

//
//  BlockImage.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/20.
//
//

#ifndef __GameTetris__BlockImage__
#define __GameTetris__BlockImage__

#include <cocos2d.h>
#include "MiniBlock.h"

USING_NS_CC;

class BlockImage : public MiniBlock
{
public:
    static BlockImage* create(BlockDef&);
    
    virtual bool init() override;
    
    BlockImage(BlockDef&);
    
    ~BlockImage();
    
    void reShowing(BlockDef&);
    
    void lockOn();
    
private:
    
    virtual std::string getBlockFileName() override;
    
};

#endif /* defined(__GameTetris__BlockImage__) */

//
//  BlockGhost.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/24.
//
//

#ifndef __GameTetris__BlockGhost__
#define __GameTetris__BlockGhost__

#include <cocos2d.h>
#include "MiniBlock.h"

USING_NS_CC;

class BlockGhost : public MiniBlock
{
public:
    
    static BlockGhost* create(BlockDef&);
    
    virtual bool init() override;
    
    BlockGhost(BlockDef&);
    
    ~BlockGhost();
    
private:
    
    virtual std::string getBlockFileName() override;
};

#endif /* defined(__GameTetris__BlockGhost__) */

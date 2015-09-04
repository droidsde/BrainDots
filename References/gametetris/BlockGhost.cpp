//
//  BlockGhost.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/24.
//
//

#include "BlockGhost.h"

BlockGhost* BlockGhost::create(BlockDef &def)
{
    BlockGhost *block = new BlockGhost(def);
    if(block && block->init())
    {
        block->autorelease();
        return block;
    }
    else{
        CC_SAFE_DELETE(block);
        block = NULL;
        return NULL;
    }
}

BlockGhost::BlockGhost(BlockDef& def)
{
    this->setBlockDefinition(def);
}

BlockGhost::~BlockGhost()
{
    
}

bool BlockGhost::init()
{
    if(!MiniBlock::init())
    {
        return false;
    }
    createByDef(getBlockDefinition());
    return true;
}

std::string BlockGhost::getBlockFileName()
{
    std::string file_name;
    if(getBlockDefinition()._isOccupOrEmpty == EMPTY)
    {
        file_name = "block_empty.png";
    }
    else
    {
        file_name = "block_ghost.png";
    }
    return file_name;
}
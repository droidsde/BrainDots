//
//  BlockImage.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/20.
//
//

#include "BlockImage.h"

BlockImage* BlockImage::create(BlockDef& def)
{
    BlockImage *block = new BlockImage(def);
    if (block && block->init())
    {
        block->autorelease();
        return block;
    }
    else
    {
        CC_SAFE_DELETE(block);
        block = NULL;
        return NULL; 
    } 
    
}

BlockImage::BlockImage(BlockDef& def)
{
    this->setBlockDefinition(def);
}

BlockImage::~BlockImage()
{
    
}

bool BlockImage::init()
{
    if (!MiniBlock::init())
    {
        return false;
    }
    
    createByDef(getBlockDefinition());
    
    return true;
}

std::string BlockImage::getBlockFileName()
{
    TETROMINO_TYPE type = this->getBlockDefinition()._tetroType;
    std::string file_name;
    
    if (getBlockDefinition()._isOccupOrEmpty == EMPTY) {
        file_name = "block_empty.png";
//        file_name = "block_ghost.png";
    }
    else{
        switch (type) {
            case TETROMINO_TYPE::O :
                file_name = "block_yellow.png";
                break;
            
            case TETROMINO_TYPE::I :
                file_name = "block_gray.png";
                break;
            
            case TETROMINO_TYPE::T :
                file_name = "block_pink.png";
                break;
            
            case TETROMINO_TYPE::Z :
                file_name = "block_red.png";
                break;
            
            case TETROMINO_TYPE::S :
                file_name = "block_green.png";
                break;
            
            case TETROMINO_TYPE::L :
                file_name = "block_orange.png";
                break;
            
            case TETROMINO_TYPE::J :
                file_name = "block_blue.png";
                break;
            
            default:
                break;
        }
    }
    
    return file_name;
}

void BlockImage::reShowing(BlockDef& def)
{
    this->setBlockDefinition(def);
    std::string file_name = this->getBlockFileName();
    auto newFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(file_name);
    this->setSpriteFrame(newFrame);
}

void BlockImage::lockOn()
{
    Point point = this->toBoardCoordinate();
    
    // inside board
    if (point.x >= FIELD_LEFT_BOARD && point.x <= FIELD_RIGHT_BOARD && point.y >= FIELD_BOTTOM && point.y <= FIELD_TOP)
    {
        this->getGameBoard()->switchShowingForCell(point);
        if (point.y > this->getGameBoard()->getCurTop() && point.y <= FIELD_TOP)
        {
            this->getGameBoard()->setCurTop(point.y);
            this->getGameBoard()->checkClear();
        }
    }
    
    if (point.y > FIELD_TOP)
    {
        this->getGameBoard()->setCurTop(point.y);
        
        return;
    }
}

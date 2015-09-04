//
//  RandomTetro.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/24.
//
//

#ifndef __GameTetris__RandomTetro__
#define __GameTetris__RandomTetro__

#include <cocos2d.h>

USING_NS_CC;

class RandomTetro
{
public:
    
    RandomTetro();
    
    ~RandomTetro();
    
    static RandomTetro* getInstance();
    
    int getItemInList();
    
    cocos2d::Point getRandomPosition();
    
    int getRandomFromRange(int,int);
    
    float getRandomFromRange(float,float);
    
private:
    
    std::random_device randomDevice;
    
    std::mt19937* mt;
    
    std::vector<int> listRandom;
    
    static RandomTetro* _randomTetro;
    
    int itemIndex;
    
    void autoFillList();
    
    int myRandom(int index);
};
#endif /* defined(__GameTetris__RandomTetro__) */

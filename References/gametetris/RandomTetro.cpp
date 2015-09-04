//
//  RandomTetro.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/24.
//
//

#include "RandomTetro.h"

RandomTetro* RandomTetro::_randomTetro = nullptr;

RandomTetro* RandomTetro::getInstance()
{
    if (_randomTetro == nullptr) {
        _randomTetro = new RandomTetro();
        return _randomTetro;
    }
    else
        return _randomTetro;
}

RandomTetro::RandomTetro()
{
    itemIndex = 0;
    mt = new std::mt19937(randomDevice());
    
    // init list
    this->listRandom.resize(7);
    for (int i = 0; i < 7; i++) {
        listRandom[i] = i;
    }
}


RandomTetro::~RandomTetro()
{
    CC_SAFE_DELETE(mt);
    CC_SAFE_DELETE(_randomTetro);
}

void RandomTetro::autoFillList()
{
    random_shuffle(listRandom.begin(), listRandom.end(), CC_CALLBACK_1(RandomTetro::myRandom, this));
}

int RandomTetro::myRandom(int index)
{
    std::uniform_int_distribution<int> dist(1,7);
    return dist(*mt) % index;
}

int RandomTetro::getItemInList()
{
    if (itemIndex == 0) {
        this->autoFillList();
    }
    int type = listRandom[itemIndex];
    itemIndex++;
    if (itemIndex == 7)
    {
        itemIndex = 0;
    }
    return type;
}

int RandomTetro::getRandomFromRange(int, int)
{
    return 0;
}
float RandomTetro::getRandomFromRange(float, float)
{
    return 0.0;
}

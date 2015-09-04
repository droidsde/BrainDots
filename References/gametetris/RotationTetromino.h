//
//  RotationTetromino.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/24.
//
//

#ifndef __GameTetris__RotationTetromino__
#define __GameTetris__RotationTetromino__

#include <cocos2d.h>
#include "Tetromino.h"

USING_NS_CC;

class RotationTetromino
{
private:
    
    static RotationTetromino* _rotatetetro;
    
public:
    static RotationTetromino* getInstance();
    
    CC_SYNTHESIZE(class Tetromino*, currentTetro, CurrentTetro);
    
    RotationTetromino();
    
    ~RotationTetromino();
    
    bool doRotation(Tetromino*);
    
    bool rotateByTypeTetro(TETROMINO_TYPE);
    
    bool rotateTest( int x, int y);
    void setNewPositionAndRotate(int x, int y);
    
    bool rotateType_I();
    
    bool rotateType_Other();
    
    // only for type I
    bool rotateTypeI_0toR();
    bool rotateTypeI_Rto2();
    bool rotateTypeI_2toL();
    bool rotateTypeI_Lto0();
    
    // for another type
    bool rotateTypeOther_0toR();
    bool rotateTypeOther_Rto2();
    bool rotateTypeOther_2toL();
    bool rotateTypeOther_Lto0();
};

#endif /* defined(__GameTetris__RotationTetromino__) */

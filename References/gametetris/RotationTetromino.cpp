//
//  RotationTetromino.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/24.
//
//

#include "RotationTetromino.h"

RotationTetromino* RotationTetromino::_rotatetetro = nullptr;

RotationTetromino* RotationTetromino::getInstance()
{
    if (_rotatetetro == nullptr) {
        _rotatetetro = new RotationTetromino();
        return _rotatetetro;
    }
    else
        return _rotatetetro;
}

RotationTetromino::RotationTetromino()
{
    currentTetro = nullptr;
}

RotationTetromino::~RotationTetromino()
{
    CC_SAFE_DELETE(currentTetro);
}

// implement rotation
bool RotationTetromino::doRotation(Tetromino *tetro)
{
    setCurrentTetro(tetro);
    TETROMINO_TYPE type = tetro->getType();
    bool is_rotate = this->rotateByTypeTetro(type);
    return is_rotate;
}

// rotation tetro by type
bool RotationTetromino::rotateByTypeTetro(TETROMINO_TYPE type)
{
    // rotate type I
    if (type == TETROMINO_TYPE::I) {
        return rotateType_I();
    }
    // must not rotate
    else if (type == TETROMINO_TYPE::O)
    {
        return false;
    }
    else
    {
        // rotate other type
        return rotateType_Other();
    }
}

bool RotationTetromino::rotateTest(int x, int y)
{
    int tempShape = getCurrentTetro()->curShape + 1;
    // if shape 0->R->2-L comeback
    if (tempShape == 4)
    {
        tempShape = 0;
    }
    char16_t mask = 0x8000;
    for (int i = PIECE_SIZE - 1; i >= 0; i--)
    {
        for (int j = 0; j < PIECE_SIZE; j++)
        {
            if (getCurrentTetro()->shapeMatrix[i][j] == (bool) (mask & getCurrentTetro()->shapesVector[tempShape]))
            {
                mask = mask >> 1;
                continue;
            }
            else
            {
                bool is_rotateable = getCurrentTetro()->blocksVector[i][j]->turnRotateAble(x, y);
                if (!is_rotateable) {
                    return false;
                }
            }
            mask = mask >> 1;
        }
    }
    return true;
}

void RotationTetromino::setNewPositionAndRotate(int x, int y)
{
    Point tetroCurPos = getCurrentTetro()->getPosition();
    getCurrentTetro()->setPositionX(tetroCurPos.x + x * BLOCK_WIDTH);
    getCurrentTetro()->setPositionY(tetroCurPos.y + y * BLOCK_WIDTH);
    getCurrentTetro()->doRotation();
}

bool RotationTetromino::rotateType_I()
{
    SHAPE_STATE curShape = (SHAPE_STATE) (getCurrentTetro()->curShape);
    if (curShape == SHAPE_STATE::SHAPE_0) {
        return rotateTypeI_0toR();
    }
    else if (curShape == SHAPE_STATE::SHAPE_R)
    {
        return rotateTypeI_Rto2();
    }
    else if (curShape == SHAPE_STATE::SHAPE_2)
    {
        return rotateTypeI_2toL();
    }
    else if(curShape == SHAPE_STATE::SHAPE_L)
    {
        return rotateTypeI_Lto0();
    }
    else return false;
}

//0->R  (0, 0)	(-2, 0)	(+1, 0)	(-2, -1)	(+1, +2)
//R->0	(0, 0)	(+2, 0)	(-1, 0)	(+2, +1)	(-1, -2)
bool RotationTetromino::rotateTypeI_0toR()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(-2, 0))
    {
        setNewPositionAndRotate(-2, 0);
        return true;
    }
    else if (rotateTest(1, 0))
    {
        setNewPositionAndRotate(1, 0);
        return true;
    }
    else if (rotateTest(-2, -1))
    {
        setNewPositionAndRotate(-2, -1);
        return true;
    }
    else if (rotateTest(1, 2))
    {
        setNewPositionAndRotate(1, 2);
        return true;
    }
    else
        return false;
}

//R->2	(0, 0)	(-1, 0)	(+2, 0)	(-1, +2)	(+2, -1)
//2->R  (0, 0)	(+1, 0)	(-2, 0)	(+1, -2)	(-2, +1)
bool RotationTetromino::rotateTypeI_Rto2()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(-1, 0))
    {
        setNewPositionAndRotate(-1, 0);
        return true;
    }
    else if (rotateTest(2, 0))
    {
        setNewPositionAndRotate(2, 0);
        return true;
    }
    else if (rotateTest(-1, 2))
    {
        setNewPositionAndRotate(-1, 2);
        return true;
    }
    else if (rotateTest(2, -1))
    {
        setNewPositionAndRotate(2, -1);
        return true;
    }
    else
        return false;
}

//2->L  (0, 0)	(+2, 0)	(-1, 0)	(+2, +1)	(-1, -2)
//L->2	(0, 0)	(-2, 0)	(+1, 0)	(-2, -1)	(+1, +2)

bool RotationTetromino::rotateTypeI_2toL()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(2, 0))
    {
        setNewPositionAndRotate(2, 0);
        return true;
    }
    else if (rotateTest(-1, 0))
    {
        setNewPositionAndRotate(-1, 0);
        return true;
    }
    else if (rotateTest(2, 1))
    {
        setNewPositionAndRotate(2, 1);
        return true;
    }
    else if (rotateTest(-1, -2))
    {
        setNewPositionAndRotate(-1, -2);
        return true;
    }
    else
        return false;

}

//L->0	(0, 0)	(+1, 0)	(-2, 0)	(+1, -2)	(-2, +1)
//0->L  (0, 0)	(-1, 0)	(+2, 0)	(-1, +2)	(+2, -1)
bool RotationTetromino::rotateTypeI_Lto0()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(1, 0))
    {
        setNewPositionAndRotate(1, 0);
        return true;
    }
    else if (rotateTest(-2, 0))
    {
        setNewPositionAndRotate(-2, 0);
        return true;
    }
    else if (rotateTest(1, -2))
    {
        setNewPositionAndRotate(1, -2);
        return true;
    }
    else if (rotateTest(-2, 1))
    {
        setNewPositionAndRotate(-2, 1);
        return true;
    }
    else
        return false;

}

//J, L, S, T, Z Tetromino Wall Kick Data
bool RotationTetromino::rotateType_Other()
{
    SHAPE_STATE curShape = (SHAPE_STATE) (getCurrentTetro()->curShape);
    if (curShape == SHAPE_STATE::SHAPE_0) {
        return rotateTypeOther_0toR();
    }
    else if (curShape == SHAPE_STATE::SHAPE_R)
    {
        return rotateTypeOther_Rto2();
    }
    else if (curShape == SHAPE_STATE::SHAPE_2)
    {
        return rotateTypeOther_2toL();
    }
    else if(curShape == SHAPE_STATE::SHAPE_L)
    {
        return rotateTypeOther_Lto0();
    }
    else return false;

}

//0->R	  (0, 0)	(-1, 0)	(-1, +1)	(0, -2)	(-1, -2)
//R->0	  (0, 0)	(+1, 0)	(+1, -1)	(0, +2)	(+1, +2)
bool RotationTetromino::rotateTypeOther_0toR()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(-1, 0))
    {
        setNewPositionAndRotate(-1, 0);
        return true;
    }
    else if (rotateTest(-1, 1))
    {
        setNewPositionAndRotate(-1, 1);
        return true;
    }
    else if (rotateTest(0, -2))
    {
        setNewPositionAndRotate(0, -2);
        return true;
    }
    else if (rotateTest(-1, -2))
    {
        setNewPositionAndRotate(-1, -2);
        return true;
    }
    else
        return false;

}

//R->2	  (0, 0)	(+1, 0)	(+1, -1)	(0, +2)	(+1, +2)
//2->R    (0, 0)	(-1, 0)	(-1, +1)	(0, -2)	(-1, -2)
bool RotationTetromino::rotateTypeOther_Rto2()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(1, 0))
    {
        setNewPositionAndRotate(1, 0);
        return true;
    }
    else if (rotateTest(1, -1))
    {
        setNewPositionAndRotate(1, -1);
        return true;
    }
    else if (rotateTest(0, 2))
    {
        setNewPositionAndRotate(0, 2);
        return true;
    }
    else if (rotateTest(1, 2))
    {
        setNewPositionAndRotate(1, 2);
        return true;
    }
    else
        return false;

}

//2->L    (0, 0)	(+1, 0)	(+1, +1)	(0, -2)	(+1, -2)
//L->2	  (0, 0)	(-1, 0)	(-1, -1)	(0, +2)	(-1, +2)
bool RotationTetromino::rotateTypeOther_2toL()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(1, 0))
    {
        setNewPositionAndRotate(1, 0);
        return true;
    }
    else if (rotateTest(1, 1))
    {
        setNewPositionAndRotate(1, 1);
        return true;
    }
    else if (rotateTest(0, -2))
    {
        setNewPositionAndRotate(0, -2);
        return true;
    }
    else if (rotateTest(1, -2))
    {
        setNewPositionAndRotate(1, -2);
        return true;
    }
    else
        return false;

}

//L->0	  (0, 0)	(-1, 0)	(-1, -1)	(0, +2)	(-1, +2)
//0->L    (0, 0)	(+1, 0)	(+1, +1)	(0, -2)	(+1, -2)
bool RotationTetromino::rotateTypeOther_Lto0()
{
    if (rotateTest(0,0)) {
        setNewPositionAndRotate(0,0);
        return true;
    }
    else if (rotateTest(-1, 0))
    {
        setNewPositionAndRotate(-1, 0);
        return true;
    }
    else if (rotateTest(-1, -1))
    {
        setNewPositionAndRotate(-1, -1);
        return true;
    }
    else if (rotateTest(0, 2))
    {
        setNewPositionAndRotate(0, 2);
        return true;
    }
    else if (rotateTest(-1, 2))
    {
        setNewPositionAndRotate(-1, 2);
        return true;
    }
    else
        return false;

}

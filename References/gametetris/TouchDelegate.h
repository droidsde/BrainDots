//
//  TouchDelegate.h
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/25.
//
//

#ifndef GameTetris_TouchDelegate_h
#define GameTetris_TouchDelegate_h

class TouchDelegate
{
public:
    virtual void moveLeft(int) = 0;
    
    virtual void moveRight(int) = 0;
    
    virtual void moveDown(int) = 0;
    
    virtual void moveRotate() = 0;
    
    virtual void moveHardDrop() = 0;
    
    virtual void moveSoftDropStart() = 0;
    
    virtual void moveSoftDropStop() = 0;
    
    virtual void onHold() = 0;
    
    virtual void onPause() = 0;
    
};
#endif

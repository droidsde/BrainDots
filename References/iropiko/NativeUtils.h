//
//  NativeUtils.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/05.
//
//

#ifndef __PoolExample__NativeUtils__
#define __PoolExample__NativeUtils__

#include "cocos2d.h"


class NativeUtils
{
public:
    // record sound
    static void startRecording(const char* filename);
    static void stopRecording();
};

#endif /* defined(__PoolExample__NativeUtils__) */

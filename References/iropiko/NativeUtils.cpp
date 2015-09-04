//
//  NativeUtils.cpp
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/05.
//
//

#include "NativeUtils.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define CLASS_NAME "org/cocos2dx/cpp/AppActivity"
#include "JniHelpers.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "SimpleAudioRecordEngine.h"
#endif
using namespace cocos2d;

void NativeUtils::startRecording(const char *filename)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelpers::jniCommonVoidCall("startRecording", CLASS_NAME, filename);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCLOG("filename= %s",filename);
    CocosDenshion::SimpleAudioRecordEngine::getInstance()->initRecord(filename);
    CocosDenshion::SimpleAudioRecordEngine::getInstance()->startRecord();
#endif
}

void NativeUtils::stopRecording()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelpers::jniCommonVoidCall("stopRecording", CLASS_NAME);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CocosDenshion::SimpleAudioRecordEngine::getInstance()->stopRecord();
#endif
}
//
//  SimpleAudioRecordEngine.mm
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/10.
//
//

#include "SimpleAudioRecordEngine.h"
#include "SimpleAudioRecordEnigine_objc.h"
#include "cocos2d.h"

USING_NS_CC;

static bool static_checkMic()
{
    return [[SimpleAudioRecordEngine sharedEngine] checkMic];
}

static bool static_isRecording()
{
    return [[SimpleAudioRecordEngine sharedEngine] isRecording];
}

static void static_initRecord(const char *fileName)
{
    [[SimpleAudioRecordEngine sharedEngine] initRecord:[NSString stringWithUTF8String:fileName]];
}

static void static_startRecord()
{
    [[SimpleAudioRecordEngine sharedEngine] startRecord];
}

static void static_stopRecord()
{
    [[SimpleAudioRecordEngine sharedEngine] stopRecord];
}

namespace CocosDenshion
{
    
    static SimpleAudioRecordEngine *s_pEngine;
    
    SimpleAudioRecordEngine::SimpleAudioRecordEngine()
    {
        
    }
    
    SimpleAudioRecordEngine::~SimpleAudioRecordEngine()
    {
        
    }
    
    SimpleAudioRecordEngine *SimpleAudioRecordEngine::getInstance()
    {
        if (!s_pEngine)
        {
            s_pEngine = new SimpleAudioRecordEngine();
        }
        
        return s_pEngine;
    }
    
    bool SimpleAudioRecordEngine::checkMic()
    {
        return static_checkMic();
    }
    
    bool SimpleAudioRecordEngine::isRecording()
    {
        return static_isRecording();
    }
    
    void SimpleAudioRecordEngine::initRecord(const char *fileName)
    {
//        std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
        static_initRecord(fileName);
    }
    
    void SimpleAudioRecordEngine::startRecord()
    {
        static_startRecord();
    }
    
    void SimpleAudioRecordEngine::stopRecord()
    {
        static_stopRecord();
    }
    
}

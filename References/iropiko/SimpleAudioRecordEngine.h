//
//  SimpleAudioRecordEngine.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/05.
//
//

#ifndef PoolExample_SimpleAudioRecordEngine_h
#define PoolExample_SimpleAudioRecordEngine_h

#include "Export.h"

namespace CocosDenshion
{
    
    class EXPORT_DLL SimpleAudioRecordEngine
    {
    public:
        static SimpleAudioRecordEngine *getInstance();
        
    protected:
        SimpleAudioRecordEngine();
        virtual ~SimpleAudioRecordEngine();
        
    public:
        virtual bool checkMic();
        virtual bool isRecording();
        virtual void initRecord(const char *fileName);
        virtual void startRecord();
        virtual void stopRecord();
    };
    
}

#endif

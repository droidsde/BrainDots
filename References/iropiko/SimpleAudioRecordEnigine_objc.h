//
//  SimpleAudioRecordEnigine_objc.h
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/10.
//
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface SimpleAudioRecordEngine : NSObject <AVAudioRecorderDelegate, AVAudioPlayerDelegate>
{
    AVAudioRecorder *_recorder;
    AVAudioSession *_audioSession;
    AVAudioPlayer *_player;
    
    NSString *_documentsPath;
}

@property (nonatomic, retain) AVAudioRecorder *recorder;

+ (SimpleAudioRecordEngine *) sharedEngine;

- (BOOL)checkMic;
- (BOOL)isRecording;

- (void)initRecord:(NSString *)fileName;
- (void)startRecord;
- (void)stopRecord;

- (NSString *)documentsPath;

@end
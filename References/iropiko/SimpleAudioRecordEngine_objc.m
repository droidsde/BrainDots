//
//  SimpleAudioRecordEngine_objc.m
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/10.
//
//

#import "SimpleAudioRecordEnigine_objc.h"
#import "audio/ios/SimpleAudioEngine_objc.h"

@implementation SimpleAudioRecordEngine

@synthesize recorder = _recorder;


static SimpleAudioRecordEngine *sharedEngine = nil;


#pragma mark -
#pragma mark Life Cycle Methods

+ (SimpleAudioRecordEngine *)sharedEngine
{
    @synchronized(self)
    {
        if (!sharedEngine)
            sharedEngine = [[SimpleAudioRecordEngine alloc] init];
    }
    return sharedEngine;
}

+ (id)alloc
{
    @synchronized(self)
    {
        NSAssert(sharedEngine == nil, @"Attempted to allocate a second instance of a singleton");
        return [super alloc];
    }
    return nil;
}

- (id)init
{
    if ((self = [super init]))
    {
        NSError *error = nil;
        
        _audioSession = [AVAudioSession sharedInstance];
        [_audioSession setCategory:AVAudioSessionCategoryPlayAndRecord error:&error];
        
        if (error)
        {
            NSLog(@"1: error: %@", [error localizedDescription]);
        }
        
        [_audioSession setActive:YES error:&error];
        
        if (error)
        {
            NSLog(@"2: error: %@", [error localizedDescription]);
        }
        UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;
        AudioSessionSetProperty (kAudioSessionProperty_OverrideAudioRoute,sizeof (audioRouteOverride),&audioRouteOverride);
    }
    
    return self;
}

- (void)dealloc
{
    [_recorder stop];
    [_recorder release];
    [_documentsPath release];
    [super dealloc];
}


#pragma mark -
#pragma mark Recording Methods

- (BOOL)checkMic
{
    NSLog(@"check mic");
    if (_audioSession)
        return _audioSession.inputIsAvailable;
    
    return NO;
}

- (BOOL)isRecording
{
    if (!_recorder)
        return NO;
    
    return _recorder.isRecording;
}

- (void)initRecord:(NSString *)fileName
{
     NSLog(@"init record");
    if (_recorder)
    {
        [_recorder stop];
        [_recorder release];
        _recorder = nil;
    }
    
    NSLog(@"fileName ios %@",fileName);
    // Set the audio file
    NSArray *pathComponents = [NSArray arrayWithObjects:
                               [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject],fileName,nil];
    NSURL *outputFileURL = [NSURL fileURLWithPathComponents:pathComponents];
//    NSURL *outputFileURL = [NSURL fileURLWithPath:fileName];
    NSLog(@"%@",[outputFileURL absoluteString]);
//    NSURL *fileURL = [NSURL fileURLWithPath:[[self documentsPath] stringByAppendingPathComponent:fileName]];
    
    NSDictionary *settings = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithFloat: 44100.0],                      AVSampleRateKey,
                              [NSNumber numberWithInt:   kAudioFormatAppleLossless],    AVFormatIDKey,
                              [NSNumber numberWithInt:   16],                           AVEncoderBitRateKey,
                              [NSNumber numberWithInt:   2],                            AVNumberOfChannelsKey,
                              [NSNumber numberWithInt:   AVAudioQualityHigh],         AVEncoderAudioQualityKey,
                              nil];
    
    NSError *error = nil;
    
    _recorder = [[AVAudioRecorder alloc] initWithURL:outputFileURL settings:settings error:nil];
    _recorder.delegate = self;
    _recorder.meteringEnabled = YES;
    [_recorder prepareToRecord];
    if (error)
    {
        NSLog(@"3: error: %@", [error localizedDescription]);
    }
}

- (void)startRecord
{
     NSLog(@"start record");
    if (_recorder)
    {
        if (!_recorder.recording) {
            // start recording
            [_recorder record];
            NSLog(@"Success recording!");
        }
//        if ([_recorder record])
//        {
//            NSLog(@"Success recording!");
//        }
//        else
//        {
//            [_recorder pause];
//            NSLog(@"Fail recording!");
//        }
    }
}

- (void)stopRecord
{
    [_recorder stop];
    NSLog(@"stop record");
}

- (NSString *)documentsPath
{
    if (!_documentsPath)
    {
        NSArray *searchPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        _documentsPath = [searchPath objectAtIndex:0];
        [_documentsPath retain];
    }
    
    return _documentsPath;
}


#pragma mark -
#pragma mark Audio Recording Delegate Methods

- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag
{
//    _player = [[AVAudioPlayer alloc] initWithContentsOfURL:_recorder.url error:nil];
//    [_player setDelegate:self];
//    [_player play];
}

@end

//
//  ManageSound.cpp
//  GameTetris
//
//  Created by Nguyen Minh Hoang on 2015/03/19.
//
//

#include "ManageSound.h"

ManageSound* ManageSound::_manageSound = nullptr;

ManageSound::ManageSound()
{
    audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    music = true;
    sound = true;
}

ManageSound* ManageSound::getInstance()
{
    if (!_manageSound) {
        _manageSound = new ManageSound();
        return _manageSound;
    }
    else
        return _manageSound;
}

ManageSound::~ManageSound()
{
    CC_SAFE_DELETE(_manageSound);
}

void ManageSound::preLoad()
{
//    audioEngine->preloadBackgroundMusic("Tetris.wav");
    audioEngine->preloadBackgroundMusic("Tetris.mp3");
    audioEngine->preloadEffect("buttons.wav");
    audioEngine->preloadEffect("deletechips.wav");
    audioEngine->preloadEffect("figurestop.wav");
    audioEngine->preloadEffect("PickedCoinEcho.wav");
    
    // move
    audioEngine->preloadEffect("SFX_PieceMoveLR.wav");
    audioEngine->preloadEffect("SFX_SpecialLineClearSingle.wav");
    audioEngine->preloadEffect("SFX_PieceRotateLR.wav");
    audioEngine->preloadEffect("SFX_PieceHardDrop.wav");
}

void ManageSound::unLoad(std::string file)
{
    audioEngine->unloadEffect(file.c_str());
}

bool ManageSound::isMusicOn()
{
    return getMusicOn();
}

bool ManageSound::isSoundOn()
{
    return getSoundOn();
}

void ManageSound::playBackgroundMusic()
{
    if (music)
    {
        audioEngine->playBackgroundMusic("Tetris.mp3", true);
    }
}

void ManageSound::pauseBackgroundMusic()
{
    if (audioEngine != nullptr && music)
    {
        if (audioEngine->isBackgroundMusicPlaying())
        {
            audioEngine->pauseBackgroundMusic();
        }
        else
            audioEngine->stopBackgroundMusic();
    }
}

void ManageSound::resumeBackgroundMusic()
{
    if (audioEngine != nullptr && music)
    {
        audioEngine->resumeBackgroundMusic();
    }
}

void ManageSound::playClickSound()
{
    if (sound)
    {
        audioEngine->playEffect("buttons.wav");
    }
}

void ManageSound::playMoveSound()
{
    if (sound)
    {
       audioEngine->playEffect("buttons.wav");
    }
}

void ManageSound::playMoveLeftRightSound()
{
    if (sound)
    {
        audioEngine->playEffect("SFX_PieceMoveLR.wav");
    }
}

void ManageSound::playLockOnSound()
{
    if (sound)
    {
        audioEngine->playEffect("figurestop.wav");
    }
}

void ManageSound::playClearLineSound()
{
    if (sound)
    {
//        audioEngine->playEffect("deletechips.wav");
        audioEngine->playEffect("SFX_SpecialLineClearSingle.wav");
    }
}

void ManageSound::playAddScoreSound()
{
    if (sound)
    {
        audioEngine->playEffect("PickedCoinEcho.wav");
    }
}

void ManageSound::playHardDropSound()
{
    if (sound)
    {
        audioEngine->playEffect("SFX_PieceHardDrop.wav");
    }
}

void ManageSound::playRotateSound()
{
    if (sound)
    {
        audioEngine->playEffect("SFX_PieceRotateLR.wav");
    }
}


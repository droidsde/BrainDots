//
//  SoundManager.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/04.
//
//

#include "SoundManager.h"

SoundManager* SoundManager::_soundManager = nullptr;

SoundManager::SoundManager()
{
    audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    music = true;
    sound = true;
}

SoundManager* SoundManager::getInstance()
{
    if (!_soundManager) {
        _soundManager = new SoundManager();
        return _soundManager;
    }
    else
        return _soundManager;
}

SoundManager::~SoundManager()
{
    CC_SAFE_DELETE(_soundManager);
}

void SoundManager::preLoad()
{
    // load background music
//    audioEngine->preloadBackgroundMusic("Tetris.mp3");
    
    // load effects
//    audioEngine->preloadEffect("SFX_PieceMoveLR.wav");
//    audioEngine->preloadEffect("SFX_SpecialLineClearSingle.wav");
//    audioEngine->preloadEffect("SFX_PieceRotateLR.wav");
//    audioEngine->preloadEffect("SFX_PieceHardDrop.wav");
}

void SoundManager::unLoad(std::string file)
{
    audioEngine->unloadEffect(file.c_str());
}

bool SoundManager::isMusicOn()
{
    return getMusicOn();
}

bool SoundManager::isSoundOn()
{
    return getSoundOn();
}

void SoundManager::playBackgroundMusic()
{
    if (music)
    {
//        audioEngine->playBackgroundMusic("Tetris.mp3", true);
    }
}

void SoundManager::pauseBackgroundMusic()
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

void SoundManager::resumeBackgroundMusic()
{
    if (audioEngine != nullptr && music)
    {
        audioEngine->resumeBackgroundMusic();
    }
}

void SoundManager::playClickSound()
{
    if (sound)
    {
//        audioEngine->playEffect("buttons.wav");
    }
}

void SoundManager::playMoveSound()
{
    if (sound)
    {
//        audioEngine->playEffect("buttons.wav");
    }
}

void SoundManager::playAddScoreSound()
{
    if (sound)
    {
//        audioEngine->playEffect("PickedCoinEcho.wav");
    }
}
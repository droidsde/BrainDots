//
//  ManageScene.cpp
//  PoolExample
//
//  Created by Nguyen Minh Hoang on 2015/08/07.
//
//

#include "ManageScene.h"

ManageScene* ManageScene::_manageScene = nullptr;

ManageScene::ManageScene()
{
    curLanguage = "jp.json";
    showNameNumber = true;
    showNumber = true;
    playSound = true;
}

ManageScene::~ManageScene()
{
    CC_SAFE_DELETE(_manageScene);
}

ManageScene* ManageScene::getInstance()
{
    
    if (_manageScene == nullptr)
    {
        _manageScene = new ManageScene();
        return _manageScene;
    }else
        return _manageScene;
    
}

void ManageScene::changeState(GAME_STATE state)
{
    
    this->setCurState(state);
    
    switch (state) {
            
        case GAME_STATE::LOGO :
            moveSplashScene();
            break;
            
        case GAME_STATE::MENU :
            moveMenuScene();
            break;
            
        case GAME_STATE::GAME :
            moveGameScene();
            break;
            
        case GAME_STATE::RECORD :
            moveManageRecord();
            break;
            
        default:
            break;
    }
}

void ManageScene::nextScene(Scene* scene)
{
    this->setCurScene(scene);
    TransitionScene* transition = TransitionFade::create(TIME_TRANSITION_SCENE, scene);
    Director::getInstance()->replaceScene(transition);
}

void ManageScene::moveSplashScene()
{
    auto scene = SplashScreen::createScene();
    this->nextScene(scene);
}

void ManageScene::moveMenuScene()
{
    auto scene = MenuScene::createScene();
    this->nextScene(scene);
}

void ManageScene::moveManageRecord()
{
    auto scene = AudioManagerScene::createScene();
    this->nextScene(scene);
}

void ManageScene::moveGameScene()
{
    auto scene = HelloWorld::createScene();
    this->nextScene(scene);
}

void ManageScene::loadResouceJson(std::string jsonFile)
{
    if (jsonFile == "ja.json") {
        curLanguage = "ja";
    } else if(jsonFile == "en.json"){
        curLanguage = "en";
    }
    //below we open, read and parse language data file with rapidjson library
    std::string clearContent = FileUtils::getInstance()->getStringFromFile(jsonFile);
    
    document.Parse<0>(clearContent.c_str());
    if(document.HasParseError())
    {
        CCLOG("Language file parsing error!");
        return;
    }
}

std::string ManageScene::getStringForKey(std::string key)
{
    return document[key.c_str()].GetString();
}

std::string  ManageScene::getString(std::string key)
{
    return getInstance()->getStringForKey(key);
}

bool ManageScene::checkFileSoundExist(std::string filename)
{
    if (FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath()+filename))
    {
        CCLOG("file %s exist", filename.c_str());
        return true;
    }
    else
    {
        CCLOG("file %s non exist", filename.c_str());
        return false;
    }
}

std::string ManageScene::getFileNameFollowPlatform(std::string filename)
{
    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        filename += ".3gp";
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        filename += ".m4a";
    }
    return filename;
}


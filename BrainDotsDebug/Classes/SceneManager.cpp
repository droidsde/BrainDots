//
//  SceneManager.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/03.
//
//

#include "SceneManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define FILE_FORMAT ("/mnt/sdcard/loading4.gif") //On the Android platform, the resources are compressed in the asset directory. Therefore, the resources must be files on the sd card
#else
#define FILE_FORMAT ("loading4.gif")
#endif

SceneManager* SceneManager::_sceneManager = nullptr;

SceneManager::SceneManager()
{
    gameLevel = UserDefault::getInstance()->getIntegerForKey("level", 0);
    curMaxLevel = UserDefault::getInstance()->getIntegerForKey("level", 0);
    curLanguage = UserDefault::getInstance()->getStringForKey("language", "English");
    curUnlockPencil = UserDefault::getInstance()->getIntegerForKey("unlockpencil", PENCIL_DEFAULT_UNLOCK);
    curUsePencil = UserDefault::getInstance()->getIntegerForKey("usepencil", 0);
    curCoin = UserDefault::getInstance()->getIntegerForKey("coin", DEFAULT_COIN);
    gif = NULL;
}

SceneManager::~SceneManager()
{
    CC_SAFE_DELETE(_sceneManager);
}

SceneManager* SceneManager::getInstance()
{
    if (_sceneManager == nullptr) {
        _sceneManager = new SceneManager();
    }
    
    return _sceneManager;
}

void SceneManager::changeState(GAME_STATE state)
{
    this->setCurState(state);
    
    switch (state) {
        case GAME_STATE::LOGO :
            moveLogoScene();
            break;
            
        case GAME_STATE::SPLASH_SCREEN :
            moveSplashScreen();
            break;
            
        case GAME_STATE::MENU :
            moveMenuScene();
            break;
        
        case GAME_STATE::SETTING :
            moveSettingLayer();
            break;
            
        case GAME_STATE::GAME :
            moveGameScene();
            break;
            
        case GAME_STATE::OVER :
            moveGameOverScene();
            break;
            
        default:
            break;
    }
}

void SceneManager::nextScene(cocos2d::Scene * scene)
{
    log("#SCENEMANAGER %s", __FUNCTION__);
    this->setCurScene(scene);
    TransitionScene* transition = TransitionFade::create(TIME_TRANSITION_SCENE, scene);
    Director::getInstance()->replaceScene(transition);
}

void SceneManager::loadingScene(Layer* scene)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    std::string name = FILE_FORMAT;
    name = FileUtils::getInstance()->fullPathForFilename(name.c_str());

//    gif = InstantGif::create(name.c_str());//InstantGif ：While playing, while parsing
//    if(gif == NULL)
//    {
//        log("%s","create gif failed");
//        return ;
//    }
//    gif->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    gif->setPosition(visibleSize.width/2 , visibleSize.height/2);
//    //    gif->setScale(visibleSize.width/ gif->getContentSize().width, visibleSize.height/gif->getContentSize().height);
//    scene->addChild(gif);
}

void SceneManager::saveLevel(int level)
{
    int oldLevel = UserDefault::getInstance()->getIntegerForKey("level", 0);
    if (level > oldLevel) {
        UserDefault::getInstance()->setIntegerForKey("level", level);
        setCurMaxLevel(level);
    }
}

void SceneManager::saveLanguage()
{
    UserDefault::getInstance()->setStringForKey("language", getCurLanguage());
}

void SceneManager::saveUnlockPencil(int index)
{
    int oldPencil = UserDefault::getInstance()->getIntegerForKey("unlockpencil", PENCIL_DEFAULT_UNLOCK);
    if (index > oldPencil) {
        UserDefault::getInstance()->setIntegerForKey("unlockpencil", index);
        setCurUnlockPencil(index);
    }
}

void SceneManager::saveUsePencil(int index)
{
    UserDefault::getInstance()->setIntegerForKey("usepencil", index);
    setCurUsePencil(index);
}

void SceneManager::saveCoin(int coin)
{
    UserDefault::getInstance()->setIntegerForKey("coin", coin);
    setCurCoin(coin);
}

void SceneManager::loadListLanguage(std::string jsonFile)
{
    //below we open, read and parse language data file with rapidjson library
    std::string clearContent = FileUtils::getInstance()->getStringFromFile(jsonFile);
    
    document.Parse<0>(clearContent.c_str());
    if(document.HasParseError())
    {
        CCLOG("Language file parsing error!");
        return;
    }
}

std::string SceneManager::getStringForKey(std::string key)
{
    std::string result = "";
    if (document.IsObject()) {
        if (document.HasMember(key.c_str())) {
            return document[key.c_str()].GetString();
        }
        else return result;
    }
    else return result;
}

int SceneManager::getIntForKey(std::string key)
{
    int result = 0;
    if (document.IsObject()) {
        if (document.HasMember(key.c_str())) {
            result = document[key.c_str()].GetInt();
        }
    }
    return result;
}

std::vector<std::string> SceneManager::getListData(std::string root, std::string key)
{
    std::vector<std::string> list;
    if (document.IsObject()) {
        if (document.HasMember(root.c_str()) && document[root.c_str()].IsArray()) {
            const rapidjson::Value& arr = document[root.c_str()];
            for (int i = 0; i < arr.Size(); i++) {
                if (arr[i].HasMember(key.c_str())) {
                    auto keyText = arr[i][key.c_str()].GetString();
                    list.push_back(keyText);
                }
            }
        }
    }
    return list;
}

std::map<std::string, std::string> SceneManager::getMapData(std::string root, std::string key, std::string value)
{
    std::map<std::string, std::string> map;
    if (document.IsObject()) {
        if (document.HasMember(root.c_str()) && document[root.c_str()].IsArray()) {
            const rapidjson::Value& arr = document[root.c_str()];
            for (int i = 0; i < arr.Size(); i++) {
                std::string keyText, valueText;
                if (arr[i].HasMember(key.c_str()) && arr[i].HasMember(value.c_str())) {
                    keyText = arr[i][key.c_str()].GetString();
                    valueText = arr[i][value.c_str()].GetString();
                    map.insert(std::map<std::string, std::string>::value_type(keyText, valueText));
                }
            }
        }
    }
    return map;
}

void SceneManager::moveLogoScene()
{
    auto scene = LogoScene::createScene();
    this->setCurScene(scene);
    Director::getInstance()->replaceScene(scene);
}

void SceneManager::moveSplashScreen()
{
    
}

void SceneManager::moveMenuScene()
{
    auto scene = MenuScene::createScene();
    this->nextScene(scene);
}

void SceneManager::moveSettingLayer()
{
    
}

void SceneManager::moveGameScene()
{
    auto scene = GameScene::createScene();
    this->nextScene(scene);
}

void SceneManager::moveGameOverScene()
{
    
}

bool SceneManager::checkFileExist(std::string filename)
{
    if (FileUtils::getInstance()->isFileExist(filename)) {
        return true;
    }
    else return false;
}

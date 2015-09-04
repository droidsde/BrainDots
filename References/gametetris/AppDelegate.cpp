#include "AppDelegate.h"
#include "GLViewUtil.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize path resource
    std::vector<std::string> paths;
    paths.push_back("fonts");
    paths.push_back("images");
    paths.push_back("sounds");
    paths.push_back("ui");
    FileUtils::getInstance()->setSearchPaths(paths);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Game Tetris");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);
    
    auto fitSize = GLViewUtil::calcFitDesignResolutionSize(640, 960, GLViewUtil::FitType::WIDTH);
    glview->setDesignResolutionSize(fitSize.width, fitSize.height, ResolutionPolicy::SHOW_ALL);

//    Size frameSize = glview->getFrameSize();
//    float widthDesign = 640.0f;
//    float heightDesign = widthDesign * frameSize.height / frameSize.width;
//    glview->setDesignResolutionSize(widthDesign, heightDesign, ResolutionPolicy::SHOW_ALL);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // first go to splash screen
    auto manageScene = ManageScene::getInstance();
    manageScene->changeState(GAME_STATE::LOGO);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    NotificationCenter::getInstance()->postNotification(PAUSE_GAME);
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

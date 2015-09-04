#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <ui/CocosGUI.h>
#include "SimpleAudioEngine.h"
#include "ManageScene.h"

USING_NS_CC;

class HelloWorld : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    void fillColorTriangle(Vec2 p0, Vec2 p1, Vec2 p2);
    void fillColorBezierQuad(Vec2 p0, Vec2 p1, Vec2 p2, Color3B color, STATE_MOVE state);
    void fillColorBezierCubic(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Color3B color, STATE_MOVE state);
    
    void preLoadEffect();
    void playEffect(int number);
    
    void drawCircle(Vec2 position, float delay);
    void drawCircle(Vec2 position, float delay, cocos2d::CallFunc *callFuncOnCompletion);
    void drawCounterCircles(int number);
    
    void initLayout(cocos2d::ui::Layout* layout, Color3B color, cocos2d::Size size, Vec2 position);
private:
    DrawNode* draw;
    Vector<DrawNode*> circles;
    cocos2d::Size winsize;
    Vec2 firstTouch, movingTouch, endTouch;
    Vector<cocos2d::ui::Layout*> layoutList;
    float widthLayout;
    STATE_MOVE state;
    Color3B colorBefore, colorAfter;
    Label* colorName;
    Label* numberCount;
    int colorIndex;
    CocosDenshion::SimpleAudioEngine* audio;
    int arrayNumber[SUMCOLOR];
    std::string numberName[SUMCOLOR];
    cocos2d::ui::Layout* layoutA;
    cocos2d::ui::Layout* layout[5];
    float currentXOfA;
    
    void shuffleArray(int* arr, size_t n);
    void removeAndClearListDrawNode();
    void clearListDrawNodeAndShowColorName();
    void animationColorName();
    void animationNumbers();
    bool checkDrawNodeRunning();
};

#endif // __HELLOWORLD_SCENE_H__

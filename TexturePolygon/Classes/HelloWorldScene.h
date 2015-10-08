#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

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
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    
    void initPhysics();
    void addRectangleBetweenPointsToBody(b2Body* body, Vec2 start, Vec2 end);
    Rect getBodyRectangle(b2Body* body);
    
    void update(float dt);
    
    Node *createRandomPolygon(int size, std::string filename);
    
    void getRectsFromBody(b2Body* body);
    
    void createBodyByRender();
    void createBodyByTextPoly();
    
private:
    // size and pos screen
    Size visibleSize;
    Vec2 origin;
    bool isOver = false;
    
    // physics variable
    b2World* world;
    b2Body* currentPlatformBody;
    b2Fixture* _wallFixture[4];
    b2PolygonShape boxShape;
    b2FixtureDef boxFixtureDef;
    b2Body* groundBody;
    
    std::vector<Vec2> platformPoints;
    RenderTexture *target;
    Sprite *brush;
    std::vector<Point> upArray, downArray;

};

#endif // __HELLOWORLD_SCENE_H__

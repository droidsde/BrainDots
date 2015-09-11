#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <ui/CocosGUI.h>
#include "BallContactListener.h"
#include "TiledBodyCreator.h"
#include "GLESDebugDraw.h"
#include "SceneManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

class GameScene : public cocos2d::LayerColor
{
public:
    
    GameScene();
    ~GameScene();
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    
    CREATE_FUNC(GameScene);
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    
    void initPhysics();
    void initMapLevel(int level);
    void initPhysicObjects();
    void initWall(b2Body* body, b2Fixture* _wallFixture[],float outside, uint16 categorybits, uint16 maskbits);
    void initBalls();
    
    // adds a new sprite at a given coordinate
    void update(float dt);
    void addRectangleBetweenPointsToBody(b2Body* body, Vec2 start, Vec2 end);
    Rect getBodyRectangle(b2Body* body);
    Vec2 checkBodyWeighOnSomebody(Vec2 start, Vec2 end);
    std::vector<Vec2> getListPointsIn2Point(Vec2 start, Vec2 end);
    void backMenu();
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    void endGame();
    
private:
    // size and pos screen
    Size visibleSize;
    Vec2 origin;
    
    // map variable
    TMXTiledMap* map;
    
    // test draw node
    DrawNode* drawnode;
    
    // list rect hex grid
    std::vector<Layer*> listGirdLayer;
    
    // control game
    bool gameOver = false;
    bool m_bClearBox;
    
    // physics variable
    b2World* world;
    GLESDebugDraw* debugDraw;
    b2Body* currentPlatformBody;
    b2Body* ballA;
    b2Body* ballB;
    b2Fixture* _ballAFixture;
    b2Fixture* _ballBFixture;
    b2Fixture* _wallFixture1[4];
    b2Fixture* _wallFixture2[4];
    b2Vec2 vertices[b2_maxPolygonVertices];
    
    // contacts physics
    BallContactListener* _ballContactListener;
    Vec2 collisionPoint = Vec2::ZERO;
    Vec2 posballA, posballB;
    
    // touch variable
    Vec2 posErrorDraw;
    bool isErrorDraw = false;
    std::vector<Vec2> platformPoints;
    RenderTexture *target, *captureScreen;
    Sprite *brush;
};

#endif // __HELLOWORLD_SCENE_H__

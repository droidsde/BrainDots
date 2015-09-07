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
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
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
    bool checkBodyWeighOnSomebody(Vec2 start, Vec2 end, float distance);
    void backMenu();
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    
private:
    
    Size visibleSize;
    Vec2 origin;
    
    bool gameOver = false;
    bool m_bClearBox;
    b2World* world;
    GLESDebugDraw* debugDraw;
    b2Body* currentPlatformBody;
    b2Body* ballA;
    b2Body* ballB;
    b2Fixture* _ballAFixture;
    b2Fixture* _ballBFixture;
    b2Fixture* _wallFixture1[4];
    b2Fixture* _wallFixture2[4];
    
    Vec2 posballA, posballB;
    b2Vec2 vertices[b2_maxPolygonVertices];
    int vindex = 0;
    
    Vec2 previousLocation;
    std::vector<Vec2> platformPoints;
    
    RenderTexture *target;
    Sprite *brush;
    std::vector<Sprite*> _brushs;
    
    // contacts
    BallContactListener* _ballContactListener;
    Vec2 collisionPoint = Vec2::ZERO;
    
    // test draw node
    DrawNode* drawnode;
    
    // list rect hex grid
    std::vector<Layer*> listGirdLayer;

};

#endif // __HELLOWORLD_SCENE_H__

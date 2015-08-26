#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLESDebugDraw.h"
#include "BallContactListener.h"

USING_NS_CC;

class HelloWorld: public cocos2d::Layer {
public:
    HelloWorld();
    virtual ~HelloWorld();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
    
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
    
    void initPhysics();
    void initBalls();
	
	// adds a new sprite at a given coordinate
	void update(float dt);
	void addRectangleBetweenPointsToBody(b2Body* body, Vec2 start, Vec2 end);
	Rect getBodyRectangle(b2Body* body);
    
    void clearScreen(Ref* pSender);
    
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

	Vec2 previousLocation;
	std::vector<Vec2> platformPoints;

	RenderTexture *target;
	Sprite *brush;
    std::vector<Sprite*> _brushs;
    
    // contacts
    BallContactListener* _ballContactListener;
    Vec2 collisionPoint = Vec2::ZERO;
};

#endif // __HELLOWORLD_SCENE_H__

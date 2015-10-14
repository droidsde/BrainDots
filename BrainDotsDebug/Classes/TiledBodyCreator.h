//
//  TiledBodyCreator.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/02.
//
//

#ifndef __BrainDotsDebug__TiledBodyCreator__
#define __BrainDotsDebug__TiledBodyCreator__

#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include "SceneManager.h"
#include "TexturePolygon.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PhysicObject/SwitchObject.h"
#include "PhysicObject/ElectricityObject.h"
#else
#include "SwitchObject.h"
#include "ElectricityObject.h"
#endif

USING_NS_CC;
class GameScene;
class FixtureDef
{
public:
    FixtureDef()
    : next(nullptr) {}
    
    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }
    
    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};

class BodyDef
{
public:
    BodyDef()
    : fixtures(nullptr) {}
    
    ~BodyDef() {
        if (fixtures)
            delete fixtures;
    }
    
    FixtureDef *fixtures;
    Point anchorPoint;
};

class Joint2Body
{
public:
    std::string nameBodyA, nameBodyB;
    b2Body* bodyA, *bodyB;
    bool collideConnected;
    bool enableLimit;
    bool enableMotor;
    b2JointType jointType;
    b2Vec2 localAnchorA;
    b2Vec2 localAnchorB;
    float lowerAngle;
    float maxMotorTorque;
    float motorSpeed;
    float upperAngle;
    float referenceAngle;
};

class TiledBodyCreator
{
public:
    
    TiledBodyCreator(TMXTiledMap* map, b2World* world, std::string layerName);
    ~TiledBodyCreator();
    
    // create map level
    static void createMapLevel(TMXTiledMap* map, b2World* world, std::string layerName);
    
    // init data
    void initData();
    
    // init object list
    void initObjectList(ValueVector objectList);
    //init joint list
    void initJointList(ValueVector jointList);
    // create joints
    void createJoint(Joint2Body* joint);
    
    // create fixture from tmx file
    FixtureDef* createFixture(ValueMap object);
    FixtureDef* createPolyline(ValueMap object);
    FixtureDef* createCircle(ValueMap object);
    FixtureDef* createRect(ValueMap object);
    
    PointVector initPolygonTexture(ValueMap object);
    void createPolygonTexture(ValueMap object, std::string fileName, b2Body* body);
    
    // get position body
    b2Vec2 getPositionBody(ValueMap object);
    
    // static body
    void createStaticBodies(ValueVector staticBodyList);
    
    // dynamic body & kinematic body
    void createDynamicBodies(ValueVector dynamicBodyList, b2BodyType type);
    
    void createSpriteBody(b2Body* body, FixtureDef* fixtureShape, std::string spriteName,  b2Shape::Type type);
    
    BARRIER_TYPE getBarrierType(cocos2d::Value objectValue);
    static std::vector<ConveyorBelt> getListConveyorBelt();
    static b2Fixture* getElectricityFixture();
    static b2Fixture* getSwitchFixture();
    
private:
    
    TMXTiledMap* _map;
    b2World* _world;
    std::string _layerName;
    std::map<std::string , b2Body*> mapBodyList;
};


#endif /* defined(__BrainDotsDebug__TiledBodyCreator__) */

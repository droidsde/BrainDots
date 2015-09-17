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

USING_NS_CC;

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

    // init map
    static void initMapLevel(TMXTiledMap* map, b2World* world, std::string layerName, uint16 categorybits, uint16 maskbits);
    
    // execute 2 lists
    static void executeObjectList(cocos2d::TMXTiledMap *map, b2World *world, ValueVector objectList, uint16 categorybits, uint16 maskbits);
    static void executeJointList(b2World *world, ValueVector jointList);
    
    // create fixture from tmx file
    static FixtureDef* createFixture(ValueMap object);
    static FixtureDef* createPolygon(ValueMap object);
    static FixtureDef* createPolyline(ValueMap object);
    static FixtureDef* createCircle(ValueMap object);
    static FixtureDef* createRect(ValueMap object);
    
    // get position body
    static b2Vec2 getPositionBody(ValueMap object);
    
    // static body
    static void createStaticBodies(b2World *world, ValueVector staticBodyList, uint16 categorybits, uint16 maskbits);
    
    // dynamic body
    static void createDynamicBodies(cocos2d::TMXTiledMap *map, b2World *world, ValueVector dynamicBodyList, uint16 categorybits, uint16 maskbits);
    
    // create joints
    static void createJoint(b2World* world, Joint2Body* joint);
    
    // get list hex objects
    static std::vector<Rect> getRectListObjects(TMXTiledMap* map,std::string objectsName, std::string layerName);
    
    // get list objects
    static ValueVector getListObjects(TMXTiledMap* map, std::string layerName);
    // get list object by type
    static ValueVector getListObjectByType(TMXTiledMap* map, std::string layerName, std::string type);
    // get list physics by bodyType
    static ValueVector getListPhysicsByBodytype(TMXTiledMap* map, std::string layerName, std::string type, std::string bodytype);
    
    // get list conveyor belts
    static std::vector<ConveyorBelt> getListConveyorBelt();
};


#endif /* defined(__BrainDotsDebug__TiledBodyCreator__) */

//
//  TiledBodyCreator.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/02.
//
//

#include "TiledBodyCreator.h"

USING_NS_CC;

std::map<std::string , b2Body*> mapBodyList;
std::vector<ConveyorBelt> listConveyorBelt;

typedef enum {
    POLYGON_FIXTURE,
    POLYLINE_FIXTURE,
    RECT_FIXTURE,
    CIRCLE_FIXTURE,
    UNKNOWN_FIXTURE
} fixtureTypes;

TiledBodyCreator::TiledBodyCreator()
{
    
}

TiledBodyCreator::~TiledBodyCreator()
{
    mapBodyList.clear();
    listConveyorBelt.clear();
}

void TiledBodyCreator::initMapLevel(cocos2d::TMXTiledMap *map, b2World *world, std::string layerName,
                                    uint16 categorybits, uint16 maskbits)
{
    auto layerGroup = map->getObjectGroup(layerName);
    cocos2d::ValueVector physicObjects = layerGroup->getObjects();
    
    // static bodies
    ValueVector objectsList;
    ValueVector jointList;
    
    for (cocos2d::Value objectValue : physicObjects) {
        auto object = objectValue.asValueMap();
        if (object["type"].asString() == "object") {
            objectsList.push_back(objectValue);
        } else if (object["type"].asString() == "joint") {
            jointList.push_back(objectValue);
        }
    }
    
    // execute object list
    executeObjectList(map, world, objectsList, categorybits, maskbits);
    
    // execute joint list
    executeJointList(world, jointList);
    
    // clear
    objectsList.clear();
    jointList.clear();
}

void TiledBodyCreator::executeObjectList(cocos2d::TMXTiledMap *map, b2World *world, ValueVector objectList, uint16 categorybits, uint16 maskbits)
{
    if (objectList.size() <= 0) {
        return;
    }
    
    ValueVector staticBodyList;
    ValueVector dynamicBodyList;
    ValueVector kinematicBodyList;
    
    // check static or dynamic
    for (cocos2d::Value objectValue : objectList) {
        auto object = objectValue.asValueMap();
        // only for auto create body
        if (object["autocreate"].asBool()) {
            if (object["bodyType"].asString() == "DynamicBody") {
                dynamicBodyList.push_back(objectValue);
            } else if (object["bodyType"].asString() == "StaticBody" || object["bodyType"].asString() == "") {
                staticBodyList.push_back(objectValue);
            } else if (object["bodyType"].asString() == "KinematicBody") {
                kinematicBodyList.push_back(objectValue);
            }
        }
    }
    
    // create auto staticbodies
    createStaticBodies(map, world, staticBodyList, categorybits, maskbits);
    
    // create auto dynamic bodies
    createDynamicBodies(map, world, dynamicBodyList, categorybits, maskbits, b2_dynamicBody);
    
    // create auto kinematic bodies
    createDynamicBodies(map, world, kinematicBodyList, categorybits, maskbits, b2_kinematicBody);
}

void TiledBodyCreator::executeJointList(b2World *world, ValueVector jointList)
{
    if (jointList.size() <= 0) {
        return;
    }
    std::vector<Joint2Body*> listJoint2body;
    bool enableLimit;
    bool enableMotor;

    for (cocos2d::Value objectValue : jointList)
    {
        Joint2Body* joint = new Joint2Body();
        
        auto object = objectValue.asValueMap();
        joint->nameBodyA = object["bodyA"].asString();
        joint->nameBodyB = object["bodyB"].asString();
        joint->collideConnected = object["collideConnected"].asBool();
        joint->referenceAngle = object["referenceAngle"].asFloat();
        
        if (object["jointType"].asString() == "RevoluteJoint") {
            joint->jointType = e_revoluteJoint;
        } else if (object["jointType"].asString() == "WeldJoint") {
            joint->jointType = e_weldJoint;
        }
        
        // local anchor A,B
        joint->localAnchorA = b2Vec2(object["localAnchorAX"].asFloat() / PTM_RATIO, object["localAnchorAY"].asFloat() / PTM_RATIO);
        joint->localAnchorB = b2Vec2(object["localAnchorBX"].asFloat() / PTM_RATIO, object["localAnchorBY"].asFloat() / PTM_RATIO);
        
        enableLimit = object["enableLimit"].asBool();
        enableMotor = object["enableMotor"].asBool();
        joint->enableLimit = enableLimit;
        joint->enableMotor = enableMotor;
        if (enableLimit) {
            joint->lowerAngle = object["lowerAngle"].asFloat();
            joint->upperAngle = object["upperAngle"].asFloat();
        }
        
        if (enableMotor) {
            joint->motorSpeed = object["motorSpeed"].asFloat();
            joint->maxMotorTorque = object["maxMotorTorque"].asFloat();
        }
        listJoint2body.push_back(joint);
    }
    
    // create joint
    std::map<std::string, b2Body*>::iterator itr1;
    std::map<std::string, b2Body*>::iterator itr2;
    for (int i=0; i<listJoint2body.size(); i++) {
        Joint2Body* joint = listJoint2body[i];
        itr1 = mapBodyList.find(joint->nameBodyA);
        if (itr1 != mapBodyList.end()) {
            itr2 = mapBodyList.find(joint->nameBodyB);
            if (itr2 != mapBodyList.end()) {
                CCLOG("Joint bodyA: %s vs bodyB: %s", joint->nameBodyA.c_str(), joint->nameBodyB.c_str());
                joint->bodyA = (b2Body*)itr1->second;
                joint->bodyB = (b2Body*)itr2->second;
                // find out 2 body to joint
                createJoint(world, joint);
            }
        }
    }
    
    mapBodyList.clear();
}

void TiledBodyCreator::createJoint(b2World *world, Joint2Body *joint)
{
    switch (joint->jointType) {
        case  e_revoluteJoint:
        {
            b2RevoluteJointDef revoluteJointDef;
            revoluteJointDef.bodyA = joint->bodyA;
            revoluteJointDef.bodyB = joint->bodyB;
            revoluteJointDef.collideConnected = joint->collideConnected;
            revoluteJointDef.localAnchorA = joint->localAnchorA;
            revoluteJointDef.localAnchorB = joint->localAnchorB;
            revoluteJointDef.referenceAngle = joint->referenceAngle;
            
            revoluteJointDef.enableLimit = joint->enableLimit;
            if (joint->enableLimit) {
                revoluteJointDef.lowerAngle = joint->lowerAngle;
                revoluteJointDef.upperAngle = joint->upperAngle;
            }
            
            revoluteJointDef.enableMotor = joint->enableMotor;
            if (joint->enableMotor) {
                revoluteJointDef.maxMotorTorque = joint->maxMotorTorque;
                revoluteJointDef.motorSpeed = joint->motorSpeed;
            }
            world->CreateJoint(&revoluteJointDef);
            
            break;
        }
            
        case e_weldJoint :
        {
            b2WeldJointDef weldJointDef;
            weldJointDef.referenceAngle = CC_DEGREES_TO_RADIANS(joint->referenceAngle);
            weldJointDef.bodyA = joint->bodyA;
            weldJointDef.bodyB = joint->bodyB;
            weldJointDef.localAnchorA = joint->localAnchorA;
            weldJointDef.localAnchorB = joint->localAnchorB;
            weldJointDef.collideConnected = joint->collideConnected;
            
            world->CreateJoint(&weldJointDef);
            break;
        }
        default:
            break;
    }
}

std::vector<ConveyorBelt> TiledBodyCreator::getListConveyorBelt()
{
    return listConveyorBelt;
}

void TiledBodyCreator::createStaticBodies(cocos2d::TMXTiledMap *map, b2World *world,  ValueVector staticBodyList, uint16 categorybits, uint16 maskbits)
{
    listConveyorBelt.clear();
    if (staticBodyList.size() <= 0) {
        return;
    }
    
    for(cocos2d::Value objectValue : staticBodyList)
    {
        b2Fixture* platform;
        bool isConveyorBelts = objectValue.asValueMap()["isConveyorBelt"].asBool();
        float conveyorSpeed = objectValue.asValueMap()["conveyorBeltSpeed"].asFloat();
        float friction = objectValue.asValueMap()["friction"].asFloat();
        std::string spriteName = objectValue.asValueMap()["spriteName"].asString();
        
        // create fixture shape
        auto fixtureShape = createFixture(objectValue.asValueMap());
        if(fixtureShape != NULL) {
            // create bodedef
            b2BodyDef bd;
            bd.type = b2_staticBody;
            b2Vec2 pos = getPositionBody(objectValue.asValueMap());
            if (pos.x != 0 && pos.y != 0) {
                bd.position.Set(pos.x, pos.y);
            }
            // create body
            b2Body* staticBody = world->CreateBody(&bd);
            fixtureShape->fixture.filter.categoryBits = categorybits;
            fixtureShape->fixture.filter.maskBits = maskbits;
            platform = staticBody->CreateFixture(&fixtureShape->fixture);
            if (isConveyorBelts) {
                ConveyorBelt cb;
                cb.fixture = platform;
                cb.tangentSpeed = conveyorSpeed;
                cb.friction = friction;
                listConveyorBelt.push_back(cb);
            }
            
            // create sprite for polygon
            if (spriteName != "") {
                createSpriteBody(map, staticBody, fixtureShape, spriteName, fixtureShape->fixture.shape->GetType());
            }
            
            // insert to map
            mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), staticBody));
        } else {
            CCLOG("Error when get objects");
        }
    }
    staticBodyList.clear();
}

void TiledBodyCreator::createDynamicBodies(cocos2d::TMXTiledMap *map, b2World *world,  ValueVector dynamicBodyList, uint16 categorybits, uint16 maskbits, b2BodyType type)
{
    if (dynamicBodyList.size() <= 0) {
        return;
    }
    
    for (cocos2d::Value objectValue : dynamicBodyList) {
        
        // create fixture shape
        auto fixtureShape = createFixture(objectValue.asValueMap());
        float density = objectValue.asValueMap()["density"].asFloat();
        float angle = objectValue.asValueMap()["angle"].asFloat();
        float angularVelocity = objectValue.asValueMap()["angularVelocity"].asFloat();
        std::string spriteName = objectValue.asValueMap()["spriteName"].asString();

        if (fixtureShape != NULL) {
            
            // create body def
            b2BodyDef bd;
            bd.type = type;
            b2Vec2 pos = getPositionBody(objectValue.asValueMap());
            if (pos.x != 0 && pos.y != 0) {
                bd.position.Set(pos.x, pos.y);
            }
            bd.angle = CC_DEGREES_TO_RADIANS( angle );
            b2Body* dynamicBody = world->CreateBody(&bd);
            
            // fixturedef
            fixtureShape->fixture.filter.categoryBits = categorybits;
            fixtureShape->fixture.filter.maskBits = maskbits;
            fixtureShape->fixture.density = density;
            dynamicBody->CreateFixture(&fixtureShape->fixture);
            dynamicBody->SetAngularVelocity(angularVelocity);
            
            // create sprite for polygon, circle, rect
            createSpriteBody(map, dynamicBody, fixtureShape, spriteName, fixtureShape->fixture.shape->GetType());
            
            // insert to map
            mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), dynamicBody));
        } else {
            CCLOG("Error when get objects");
        }
    }
    dynamicBodyList.clear();
}

void TiledBodyCreator::createSpriteBody(cocos2d::TMXTiledMap *map, b2Body *body, FixtureDef* fixtureShape, std::string spriteName, b2Shape::Type type)
{
    switch (type) {
        case b2Shape::e_polygon:
        {
            Rect bodyRectangle = ExecuteShapePhysic::getBodyRectangle(map->getMapSize(), body);
            float anchorX = body->GetPosition().x * PTM_RATIO - bodyRectangle.origin.x;
            float anchorY = bodyRectangle.size.height - (map->getMapSize().height - bodyRectangle.origin.y - body->GetPosition().y * PTM_RATIO);
            Vec2 anchorPoint = Vec2(anchorX / bodyRectangle.size.width, anchorY / bodyRectangle.size.height);
            auto dynamicSprite = Sprite::create(spriteName , bodyRectangle);
            dynamicSprite->setAnchorPoint(anchorPoint);
            body->SetUserData(dynamicSprite);
            map->addChild(dynamicSprite);
            break;
        }
        case b2Shape::e_circle:
        {
            Vec2 position = Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO);
            float radius = fixtureShape->fixture.shape->m_radius * PTM_RATIO;
            Rect bodyRectangle = Rect(position.x, position.y, radius*2, radius*2);
            auto dynamicSprite = Sprite::create(spriteName);
            dynamicSprite->setPosition(Vec2(position));
            dynamicSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            body->SetUserData(dynamicSprite);
            map->addChild(dynamicSprite);
            break;
        }
        case b2Shape::e_chain:
        {
            break;
        }
        case b2Shape::e_edge:
        {
            break;
        }
        case b2Shape::e_typeCount:
        {
            break;
        }
        default:
            break;
    }
}

b2Vec2 TiledBodyCreator::getPositionBody(ValueMap object)
{
    auto position = Vec2(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    float width = object["width"].asFloat() / PTM_RATIO;
    
    int fixtureType = RECT_FIXTURE;
    for(auto propObj : object)
    {
        if(propObj.first == "points") {
            fixtureType = POLYGON_FIXTURE;
        } else if(propObj.first == "polylinePoints") {
            fixtureType = POLYLINE_FIXTURE;
        }
    }
    if(object["shape"].asString() == "circle") {
        fixtureType = CIRCLE_FIXTURE;
    }
    
    if(fixtureType == CIRCLE_FIXTURE) {
        return b2Vec2(position.x + width/2, position.y + width/2);
    } else if(fixtureType == RECT_FIXTURE) {
        float height = object["height"].asFloat() / PTM_RATIO;
        return b2Vec2(position.x + width/2, position.y + height/2);
    } else if (fixtureType == POLYGON_FIXTURE) {
        ValueVector pointsVector = object["points"].asValueVector();
        return b2Vec2(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    } else if (fixtureType == POLYLINE_FIXTURE) {
        ValueVector pointsVector = object["polylinePoints"].asValueVector();
        return b2Vec2(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    }
    else return b2Vec2(0, 0);
}

FixtureDef* TiledBodyCreator::createFixture(ValueMap object)
{
    if (object["autocreate"].asBool() == false) {
        return nullptr;
    }
    
    int fixtureType = RECT_FIXTURE;
    for(auto propObj : object)
    {
        if(propObj.first == "points") {
            fixtureType = POLYGON_FIXTURE;
        } else if(propObj.first == "polylinePoints") {
            fixtureType = POLYLINE_FIXTURE;
        }
    }
    if(object["shape"].asString() == "circle") {
        fixtureType = CIRCLE_FIXTURE;
    }
    
    if(fixtureType == POLYGON_FIXTURE) {
        return createPolygon(object);
    } else if(fixtureType == POLYLINE_FIXTURE) {
        return createPolyline(object);
    } else if(fixtureType == CIRCLE_FIXTURE) {
        return createCircle(object);
    } else if(fixtureType == RECT_FIXTURE) {
        return createRect(object);
    }
    else return nullptr;
}

FixtureDef* TiledBodyCreator::createPolygon(ValueMap object)
{
    ValueVector pointsVector = object["points"].asValueVector();
    auto position = Vec2(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    
    b2PolygonShape *polyshape = new b2PolygonShape();
    b2Vec2 vertices[b2_maxPolygonVertices];
    int vindex = 0;
    
    if(pointsVector.size() > b2_maxPolygonVertices) {
        CCLOG("Skipping TMX polygon at x=%d,y=%d for exceeding %d vertices", object["x"].asInt(), object["y"].asInt(), b2_maxPolygonVertices);
        return NULL;
    }
    
    auto fix = new FixtureDef();
    
    for(Value point : pointsVector) {
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / PTM_RATIO );
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / PTM_RATIO );
        vindex++;
    }
    
    polyshape->Set(vertices, vindex);
    fix->fixture.shape = polyshape;
    
    return fix;
}


FixtureDef* TiledBodyCreator::createPolyline(ValueMap object)
{
    ValueVector pointsVector = object["polylinePoints"].asValueVector();
    auto position = Vec2(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    
    b2ChainShape *polylineshape = new b2ChainShape();
//    float verticesSize = pointsVector.size()+ 1;
    b2Vec2 vertices[30];
    int vindex = 0;
    
    auto fix = new FixtureDef();
    
    for(Value point : pointsVector) {
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / PTM_RATIO );
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / PTM_RATIO );
        vindex++;
    }
    
    polylineshape->CreateChain(vertices, vindex);
    fix->fixture.shape = polylineshape;
    
    return fix;
}

FixtureDef* TiledBodyCreator::createCircle(ValueMap object)
{
    auto position = Vec2(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    float radius = object["width"].asFloat()/2 / PTM_RATIO;

    b2CircleShape *circleshape = new b2CircleShape();
    circleshape->m_radius = radius;
//    circleshape->m_p.Set(position.x , position.y);
    
    auto fix = new FixtureDef();
    fix->fixture.shape = circleshape;

    return fix;
}

FixtureDef* TiledBodyCreator::createRect(ValueMap object)
{
    auto position = Vec2(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    float width = object["width"].asFloat() / PTM_RATIO;
    float height = object["height"].asFloat() / PTM_RATIO;
    
    b2PolygonShape *rectshape = new b2PolygonShape();
    auto fix = new FixtureDef();
    rectshape->SetAsBox(width/2, height/2);
    fix->fixture.shape = rectshape;
    
    return fix;
}

std::vector<Rect> TiledBodyCreator::getRectListObjects(cocos2d::TMXTiledMap *map, std::string objectsName, std::string layerName)
{
    std::vector<Rect> listRect;
    auto layerGrid = map->getLayer(layerName);
    if (layerGrid) {
        int zorder = layerGrid->getProperty("zorder").asInt();
        map->reorderChild(layerGrid, zorder>0?zorder:100);
    }
    auto layerGroup = map->getObjectGroup(objectsName);
    cocos2d::ValueVector collisionObjects = layerGroup->getObjects();
    
    for (cocos2d::Value objectValue : collisionObjects) {
        auto object = objectValue.asValueMap();
        Rect pos = Rect(object["x"].asFloat(), object["y"].asFloat(), object["width"].asFloat(), object["height"].asFloat());
        listRect.push_back(pos);
    }
    return listRect;
}

ValueVector TiledBodyCreator::getListObjects(cocos2d::TMXTiledMap *map, std::string layerName)
{
    // get list objects
    auto layerGroup = map->getObjectGroup(layerName);
    cocos2d::ValueVector physicObjects = layerGroup->getObjects();
    return physicObjects;
}

ValueVector TiledBodyCreator::getListObjectByType(cocos2d::TMXTiledMap *map, std::string layerName, std::string type)
{
    ValueVector objectsByType;
    
    // get list object physics
    auto physicObjects = getListObjects(map, layerName);
    
    // get list objects by type
    for (cocos2d::Value objectValue : physicObjects) {
        auto object = objectValue.asValueMap();
        if (object["type"].asString() == type) {
            objectsByType.push_back(objectValue);
        }
    }
    
    return objectsByType;
}

ValueVector TiledBodyCreator::getListPhysicsByBodytype(cocos2d::TMXTiledMap *map, std::string layerName, std::string type, std::string bodytype)
{
    ValueVector listPhysicsByBodytype;
    auto listObjectByType = getListObjectByType(map, layerName, type);
    
    for (cocos2d::Value objectValue : listObjectByType) {
        auto object = objectValue.asValueMap();
        // only for auto create body
        if (object["autocreate"].asBool()) {
            if (object["bodyType"].asString() == bodytype) {
                listPhysicsByBodytype.push_back(objectValue);
            }
        }
    }
    
    return listPhysicsByBodytype;
}

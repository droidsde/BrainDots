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
typedef enum {
    POLYGON_FIXTURE,
    POLYLINE_FIXTURE,
    RECT_FIXTURE,
    CIRCLE_FIXTURE,
    UNKNOWN_FIXTURE
} fixtureTypes;

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
    
    // static bodies
    ValueVector staticBodyList;
    ValueVector dynamicBodyList;
    
    // check static or dynamic
    for (cocos2d::Value objectValue : objectList) {
        auto object = objectValue.asValueMap();
        // only for auto create body
        if (object["autocreate"].asBool()) {
            if (object["bodyType"].asString() == "DynamicBody") {
                dynamicBodyList.push_back(objectValue);
            } else if (object["bodyType"].asString() == "StaticBody" || object["bodyType"].asString() == "") {
                staticBodyList.push_back(objectValue);
            }
        }
    }
    
    // create auto staticbodies
    createStaticBodies(world, staticBodyList, categorybits, maskbits);
    
    // create auto dynamic bodies
    createDynamicBodies(map, world, dynamicBodyList, categorybits, maskbits);
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
        
        if (object["jointType"].asString() == "RevoluteJoint") {
            joint->jointType = e_revoluteJoint;
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
            joint->maxMotorTorque = object["maxMotorTorque"].asFloat();
            joint->motorSpeed = object["motorSpeed"].asFloat();
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
                CCLOG("bodyA: %s bodyB: %s joint", joint->nameBodyA.c_str(), joint->nameBodyB.c_str());
                joint->bodyA = (b2Body*)itr1->second;
                joint->bodyB = (b2Body*)itr2->second;
                CCLOG("bodyA posX,posY: %f %f", joint->bodyA->GetPosition().x, joint->bodyA->GetPosition().y);
                CCLOG("bodyB posX,posY: %f %f", joint->bodyB->GetPosition().x, joint->bodyB->GetPosition().y);
                // remove itr
                mapBodyList.erase(itr1);
                mapBodyList.erase(itr2);
                // find out 2 body to joint
                createJoint(world, joint);
            }
        }
    }
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
            revoluteJointDef.localAnchorA.Set(joint->localAnchorA.x, joint->localAnchorA.y);
            revoluteJointDef.localAnchorB.Set(joint->localAnchorB.x, joint->localAnchorB.y);
            
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
        default:
            break;
    }
}

void TiledBodyCreator::createStaticBodies(b2World *world,  ValueVector staticBodyList, uint16 categorybits, uint16 maskbits)
{
    if (staticBodyList.size() <= 0) {
        return;
    }
    
    
    for(cocos2d::Value objectValue : staticBodyList)
    {
        b2BodyDef bd;
        auto fixtureShape = createFixture(objectValue.asValueMap(), bd);
        b2Body* staticBody = world->CreateBody(&bd);
        if(fixtureShape != NULL) {
            fixtureShape->fixture.filter.categoryBits = categorybits;
            fixtureShape->fixture.filter.maskBits = maskbits;
            staticBody->CreateFixture(&fixtureShape->fixture);

            // insert to map
            mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), staticBody));
        } else {
            CCLOG("Error when get objects");
        }
    }
}

void TiledBodyCreator::createDynamicBodies(cocos2d::TMXTiledMap *map, b2World *world,  ValueVector dynamicBodyList, uint16 categorybits, uint16 maskbits)
{
    if (dynamicBodyList.size() <= 0) {
        return;
    }
    
    for (cocos2d::Value objectValue : dynamicBodyList) {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        auto fixtureShape = createFixture(objectValue.asValueMap(), bd);
        float density = objectValue.asValueMap()["density"].asFloat();
        std::string spriteName = objectValue.asValueMap()["spriteName"].asString();
        
        if (fixtureShape != NULL) {

            b2Body* dynamicBody = world->CreateBody(&bd);
            // fixturedef
            fixtureShape->fixture.filter.categoryBits = categorybits;
            fixtureShape->fixture.filter.maskBits = maskbits;
            fixtureShape->fixture.density = density;
            dynamicBody->CreateFixture(&fixtureShape->fixture);
            
            // create sprite for polygon
            Rect bodyRectangle = ExecuteShapePhysic::getBodyRectangle(map->getMapSize(), dynamicBody);
            CCLOG("posx posy %f %f size %f %f", bodyRectangle.origin.x, bodyRectangle.origin.y, bodyRectangle.size.width, bodyRectangle.size.height);
            float anchorX = dynamicBody->GetPosition().x * PTM_RATIO - bodyRectangle.origin.x;
            float anchorY = bodyRectangle.size.height - (map->getMapSize().height - bodyRectangle.origin.y - dynamicBody->GetPosition().y * PTM_RATIO);
            Vec2 anchorPoint = Vec2(anchorX / bodyRectangle.size.width, anchorY / bodyRectangle.size.height);
            auto dynamicSprite = Sprite::create(spriteName , bodyRectangle);
            dynamicSprite->setAnchorPoint(anchorPoint);
            dynamicBody->SetUserData(dynamicSprite);
            map->addChild(dynamicSprite);
            
            // insert to map
            mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), dynamicBody));
        } else {
            CCLOG("Error when get objects");
        }
    }
}

FixtureDef* TiledBodyCreator::createFixture(ValueMap object, b2BodyDef bd)
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
        return createPolygon(object, bd);
    } else if(fixtureType == POLYLINE_FIXTURE) {
        return createPolyline(object, bd);
    } else if(fixtureType == CIRCLE_FIXTURE) {
        return createCircle(object, bd);
    } else if(fixtureType == RECT_FIXTURE) {
        return createRect(object, bd);
    }
    else return nullptr;
}

FixtureDef* TiledBodyCreator::createPolygon(ValueMap object, b2BodyDef bd)
{
    ValueVector pointsVector = object["points"].asValueVector();
    auto position = Point(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    
    b2PolygonShape *polyshape = new b2PolygonShape();
    b2Vec2 vertices[b2_maxPolygonVertices];
    int vindex = 0;
    
    if(pointsVector.size() > b2_maxPolygonVertices) {
        CCLOG("Skipping TMX polygon at x=%d,y=%d for exceeding %d vertices", object["x"].asInt(), object["y"].asInt(), b2_maxPolygonVertices);
        return NULL;
    }
    
    auto fix = new FixtureDef();
    
    for(Value point : pointsVector) {
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / PTM_RATIO + position.x);
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / PTM_RATIO + position.y);
        vindex++;
    }
    
    polyshape->Set(vertices, vindex);
    fix->fixture.shape = polyshape;
    
    return fix;
}


FixtureDef* TiledBodyCreator::createPolyline(ValueMap object, b2BodyDef bd)
{
    ValueVector pointsVector = object["polylinePoints"].asValueVector();
    auto position = Point(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    
    b2ChainShape *polylineshape = new b2ChainShape();
//    float verticesSize = pointsVector.size()+ 1;
    b2Vec2 vertices[30];
    int vindex = 0;
    
    auto fix = new FixtureDef();
    
    for(Value point : pointsVector) {
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / PTM_RATIO + position.x);
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / PTM_RATIO + position.y);
        vindex++;
    }
    
    polylineshape->CreateChain(vertices, vindex);
    fix->fixture.shape = polylineshape;
    
    return fix;
}

FixtureDef* TiledBodyCreator::createCircle(ValueMap object, b2BodyDef bd)
{
    auto position = Point(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    float radius = object["width"].asFloat()/2 / PTM_RATIO;
    
    b2CircleShape *circleshape = new b2CircleShape();
    circleshape->m_radius = radius;
    circleshape->m_p.Set(position.x + radius, position.y + radius);
    
    auto fix = new FixtureDef();
    fix->fixture.shape = circleshape;
    
    bd.position.Set(position.x + radius, position.y + radius);
    return fix;
}

FixtureDef* TiledBodyCreator::createRect(ValueMap object, b2BodyDef bd)
{
    auto position = Point(object["x"].asFloat() / PTM_RATIO, object["y"].asFloat() / PTM_RATIO);
    float width = object["width"].asFloat() / PTM_RATIO;
    float height = object["height"].asFloat() / PTM_RATIO;
    
    b2PolygonShape *rectshape = new b2PolygonShape();
    b2Vec2 vertices[4];
    int vindex = 4;
    
    vertices[0].x = position.x + 0.0f;
    vertices[0].y = position.y + 0.0f;
    
    vertices[1].x = position.x + 0.0f;
    vertices[1].y = position.y + height;
    
    vertices[2].x = position.x + width;
    vertices[2].y = position.y + height;
    
    vertices[3].x = position.x + width;
    vertices[3].y = position.y + 0.0f;
    
    auto fix = new FixtureDef();
    rectshape->Set(vertices, vindex);
    fix->fixture.shape = rectshape;
    
    return fix;
}

std::vector<Rect> TiledBodyCreator::getRectListObjects(cocos2d::TMXTiledMap *map, std::string objectsName, std::string layerName)
{
    std::vector<Rect> listRect;
    auto layerGrid = map->getLayer(layerName);
    if (layerGrid) {
        CCLOG("found layer grid");
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
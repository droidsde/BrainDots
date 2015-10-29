//
//  TiledBodyCreator.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/02.
//
//

#include "TiledBodyCreator.h"

USING_NS_CC;

std::vector<ConveyorBelt> listConveyorBelt;
b2Fixture* electricityFixture;
b2Fixture* switchFixture;

typedef enum {
    POLYGON_FIXTURE,
    POLYLINE_FIXTURE,
    RECT_FIXTURE,
    CIRCLE_FIXTURE,
    UNKNOWN_FIXTURE
} fixtureTypes;

TiledBodyCreator::TiledBodyCreator(TMXTiledMap* map, b2World* world, std::string layerName)
{
    this->_map = map;
    this->_world = world;
    this->_layerName = layerName;
}

TiledBodyCreator::~TiledBodyCreator()
{
    mapBodyList.clear();
    listConveyorBelt.clear();
    _map = nullptr;
    delete _world;
    _world = nullptr;
}

void TiledBodyCreator::createMapLevel(cocos2d::TMXTiledMap *map, b2World *world, std::string layerName)
{
    auto maplevel = new TiledBodyCreator(map, world, layerName);
    maplevel->initData();
}

void TiledBodyCreator::initData()
{
    auto layerGroup = _map->getObjectGroup(_layerName);
    cocos2d::ValueVector physicObjects = layerGroup->getObjects();
    
    // static bodies
    ValueVector objectsList;
    ValueVector jointList;
    
    for (cocos2d::Value objectValue : physicObjects) {
        auto object = objectValue.asValueMap();
        
        // object in tiledmap
        if (object["type"].asString() == "object")
        {
            objectsList.push_back(objectValue);
        }
        
        // joint in tiledmap
        else if (object["type"].asString() == "joint")
        {
            jointList.push_back(objectValue);
        }
    }
    
    // execute object list
    initObjectList(objectsList);
    
    // execute joint list
    initJointList(jointList);
    
    // clear
    objectsList.clear();
    jointList.clear();
}

void TiledBodyCreator::initObjectList(ValueVector objectList)
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
            
            // dynamic body
            if (object["bodyType"].asString() == "DynamicBody")
            {
                dynamicBodyList.push_back(objectValue);
            }
            
            // static body
            else if (object["bodyType"].asString() == "StaticBody" || object["bodyType"].asString() == "")
            {
                staticBodyList.push_back(objectValue);
            }
            
            // kinematic body
            else if (object["bodyType"].asString() == "KinematicBody")
            {
                kinematicBodyList.push_back(objectValue);
            }
        }
    }
    
    // create auto staticbodies
    createStaticBodies(staticBodyList);
    
    // create auto dynamic bodies
    createDynamicBodies(dynamicBodyList, b2_dynamicBody);
    
    // create auto kinematic bodies
    createDynamicBodies(kinematicBodyList, b2_kinematicBody);
}

void TiledBodyCreator::initJointList(ValueVector jointList)
{
    if (jointList.size() <= 0) {
        return;
    }
    std::vector<Joint2Body*> listJoint2body;
    bool enableLimit;
    bool enableMotor;

    // read option of joint
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
                createJoint(joint);
            }
        }
    }
    
    mapBodyList.clear();
}

void TiledBodyCreator::createJoint(Joint2Body *joint)
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
            _world->CreateJoint(&revoluteJointDef);
            
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
            
            _world->CreateJoint(&weldJointDef);
            break;
        }
        default:
            break;
    }
}

void TiledBodyCreator::createStaticBodies(ValueVector staticBodyList)
{
    listConveyorBelt.clear();
    if (staticBodyList.size() <= 0) {
        return;
    }
    
    for(cocos2d::Value objectValue : staticBodyList)
    {
        b2Fixture* barrierStatic;
        std::string spriteName = objectValue.asValueMap()["spriteName"].asString();
        // create bodedef
        b2BodyDef bd;
        bd.type = b2_staticBody;
        b2Vec2 pos = getPositionBody(objectValue.asValueMap());
        if (pos.x != 0 && pos.y != 0) {
            bd.position.Set(pos.x, pos.y);
        }
        // create body
        b2Body* staticBody = _world->CreateBody(&bd);
        
        // create fixture shape
        auto fixtureShape = createFixture(objectValue.asValueMap());
        if(fixtureShape != NULL) {
            //---------------//
            //  BARRIER TYPE //
            //---------------//
            switch (getBarrierType(objectValue)) {
                
                case BARRIER_TYPE::ELECTRICITY :
                {
                    CCLOG("ELECTRICITY");
                    fixtureShape->fixture.filter.categoryBits = CATEGORY_BARRIER_ELECTRICITY;
                    fixtureShape->fixture.filter.maskBits = MASK_BARRIER_ELECTRICITY;
                    break;
                }
                    
                case BARRIER_TYPE::HEXGRID :
                {
                    CCLOG("HEXGRID");
                    fixtureShape->fixture.filter.categoryBits = CATEGORY_BARRIER;
                    fixtureShape->fixture.filter.maskBits = MASK_HEXGRID;
                    break;
                }
                
                case BARRIER_TYPE::UNKNOWN :
                {
                    fixtureShape->fixture.filter.categoryBits = CATEGORY_BARRIER;
                    fixtureShape->fixture.filter.maskBits = MASK_BARRIER;
                    break;
                }
                default:
                    break;
            }
            // create fixture for body
            barrierStatic = staticBody->CreateFixture(&fixtureShape->fixture);
            
            switch (getBarrierType(objectValue)) {
                    
                case BARRIER_TYPE::CONVEYOR :
                {
                    // conveyor option
                    float conveyorSpeed = objectValue.asValueMap()["conveyorBeltSpeed"].asFloat();
                    float friction = objectValue.asValueMap()["friction"].asFloat();
                    ConveyorBelt cb;
                    cb.fixture = barrierStatic;
                    cb.tangentSpeed = conveyorSpeed;
                    cb.friction = friction;
                    listConveyorBelt.push_back(cb);
                    break;
                }
                    
                case BARRIER_TYPE::ELECTRICITY :
                {
                    electricityFixture = barrierStatic;
                    
                    // electricity
                    Rect rect = Rect(objectValue.asValueMap()["x"].asFloat(), objectValue.asValueMap()["y"].asFloat(), objectValue.asValueMap()["width"].asFloat(), objectValue.asValueMap()["height"].asFloat());
                    std::string name = "electricity.gif";
                    name = FileUtils::getInstance()->fullPathForFilename(name.c_str());
                    GifBase *gif = InstantGif::create(name.c_str());//InstantGif ：While playing, while parsing
                    if(gif == NULL)
                    {
                        CCLOG("%s","create gif failed");
                        break ;
                    } else {
                        gif->setScale(rect.size.width / gif->getContentSize().width, rect.size.height / gif->getContentSize().height);
                        gif->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        gif->setPosition(rect.origin);
                        _map->addChild(gif);
                        staticBody->SetUserData(gif);
                    }

                    break;
                }
                default:
                    break;
            }
            
            // create sprite for polygon
            if (spriteName != "") {
                createSpriteBody(staticBody, barrierStatic, spriteName, objectValue);
            }
            
            // insert to map
            mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), staticBody));
        } else {
            CCLOG("Error when get objects");
            if (spriteName != "")
            {
                switch (this->getBarrierType(objectValue)) {
                    case BARRIER_TYPE::PHYSICSEDITOR:
                    {
                        this->createPhysicseditor(staticBody, spriteName);
                    }
                    break;
                        
                    default:
                        this->createPolygonTexture(objectValue.asValueMap(), spriteName, staticBody);
                        break;
                }
                // insert to map
                mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), staticBody));
            }
        }
    }
    staticBodyList.clear();
}

void TiledBodyCreator::createDynamicBodies(ValueVector dynamicBodyList, b2BodyType type)
{
    if (dynamicBodyList.size() <= 0) {
        return;
    }
    
    for (cocos2d::Value objectValue : dynamicBodyList) {
        
        // create fixture shape
        b2Fixture* barrierDynamic;
        float density = objectValue.asValueMap()["density"].asFloat();
        float angle = objectValue.asValueMap()["angle"].asFloat();
        float angularVelocity = objectValue.asValueMap()["angularVelocity"].asFloat();
        std::string spriteName = objectValue.asValueMap()["spriteName"].asString();
        
        // create body def
        b2BodyDef bd;
        bd.type = type;
        b2Vec2 pos = getPositionBody(objectValue.asValueMap());
        if (pos.x != 0 && pos.y != 0) {
            bd.position.Set(pos.x, pos.y);
        }
        bd.angle = CC_DEGREES_TO_RADIANS( angle );
        b2Body* dynamicBody = _world->CreateBody(&bd);
        
        auto fixtureShape = createFixture(objectValue.asValueMap());
        if (fixtureShape != NULL) {
            
            // fixturedef
            fixtureShape->fixture.filter.categoryBits = CATEGORY_BARRIER;
            fixtureShape->fixture.filter.maskBits = MASK_BARRIER;
            fixtureShape->fixture.density = density;
            barrierDynamic = dynamicBody->CreateFixture(&fixtureShape->fixture);
            dynamicBody->SetAngularVelocity(angularVelocity);

            // create sprite for polygon, circle, rect
            if (spriteName != "") {
                createSpriteBody(dynamicBody, barrierDynamic, spriteName, objectValue);
            }
            
            // insert to map
            mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), dynamicBody));
        } else {
            CCLOG("Error when get objects");
            if (spriteName != "")
            {
                switch (this->getBarrierType(objectValue)) {
                    case BARRIER_TYPE::PHYSICSEDITOR:
                    {
                        this->createPhysicseditor(dynamicBody, spriteName);
                    }
                        break;
                        
                    default:
                        this->createPolygonTexture(objectValue.asValueMap(), spriteName, dynamicBody);
                        break;
                }
                // insert to map
                mapBodyList.insert(std::pair<std::string, b2Body*>(objectValue.asValueMap()["name"].asString(), dynamicBody));
            }
        }
    }
    dynamicBodyList.clear();
}

void TiledBodyCreator::createSpriteBody(b2Body *body, b2Fixture* fixture, std::string spriteName, Value objectValue)
{
    
    switch (fixture->GetType()) {
        case b2Shape::e_polygon:
        {
            Rect bodyRectangle = ExecuteShapePhysic::getBodyRectangle(_map->getMapSize(), body);
            
            switch (getBarrierType(objectValue)) {
                case BARRIER_TYPE::SWITCH :
                {
                    switchFixture = fixture;
                    // switch object
                    SwitchObject* dynamicSprite = SwitchObject::create(spriteName, false, body);
                    dynamicSprite->setScale(bodyRectangle.size.width / dynamicSprite->getContentSize().width, bodyRectangle.size.height / dynamicSprite->getContentSize().height);
                    dynamicSprite->setPosition(Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
                    body->SetUserData(dynamicSprite);
                    _map->addChild(dynamicSprite);
                    break;
                }
                    
                default:
                {
                    auto dynamicSprite = Sprite::create(spriteName);
                    dynamicSprite->setScale(bodyRectangle.size.width / dynamicSprite->getContentSize().width, bodyRectangle.size.height / dynamicSprite->getContentSize().height);
                    dynamicSprite->setPosition(Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
                    body->SetUserData(dynamicSprite);
                    _map->addChild(dynamicSprite);
                    break;
                }
            }
            
            break;
        }
            
        case b2Shape::e_circle:
        {
            Vec2 position = Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO);
            float radius = fixture->GetShape()->m_radius * PTM_RATIO;
            auto dynamicSprite = Sprite::create(spriteName);
            dynamicSprite->setPosition(Vec2(position));
            dynamicSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            dynamicSprite->setScale(radius*2/dynamicSprite->getContentSize().width);
            body->SetUserData(dynamicSprite);
            _map->addChild(dynamicSprite);
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

void TiledBodyCreator::createPhysicseditor(b2Body *body, std::string shapeName)
{
    Sprite *sprite = Sprite::create(shapeName + ".png");
    sprite->setPosition(Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
    _map->addChild(sprite);
    
    body->SetUserData(sprite);
    
    // add the fixture definitions to the body
    GB2ShapeCache *sc = GB2ShapeCache::getInstance();
    sc->addFixturesToBody(body, shapeName);
    sprite->setAnchorPoint(sc->getAnchorPointForShape(shapeName));
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
    
    // physicseditor
    if (object["barrierType"].asString() == "physicseditor") {
        return nullptr;
    }
    
    int fixtureType = RECT_FIXTURE;
    for(auto propObj : object)
    {
        if(propObj.first == "points") {
            fixtureType = POLYGON_FIXTURE;
        }
        else if(propObj.first == "polylinePoints") {
            fixtureType = POLYLINE_FIXTURE;
        }
    }
    if(object["shape"].asString() == "circle") {
        fixtureType = CIRCLE_FIXTURE;
    }
    
    if (fixtureType == POLYGON_FIXTURE) {
        return nullptr;
    } else if(fixtureType == POLYLINE_FIXTURE) {
        return createPolyline(object);
    } else if(fixtureType == CIRCLE_FIXTURE) {
        return createCircle(object);
    } else if(fixtureType == RECT_FIXTURE) {
        return createRect(object);
    }
    else return nullptr;
}

PointVector TiledBodyCreator::initPolygonTexture(ValueMap object)
{
    PointVector listPoint;
    ValueVector pointsVector = object["points"].asValueVector();
    
    if (pointsVector.size() <= 0) {
        return listPoint;
    }
    
    for(Value point : pointsVector) {
        float x = point.asValueMap()["x"].asFloat();
        float y = -point.asValueMap()["y"].asFloat();
        listPoint.push_back(Point(x, y));
    }
    
    return listPoint;
}

void TiledBodyCreator::createPolygonTexture(ValueMap object, std::string fileName, b2Body* body)
{
    if (object["autocreate"].asBool() == false) {
        return;
    }
    
    int fixtureType = UNKNOWN_FIXTURE;
    for(auto propObj : object)
    {
        if(propObj.first == "points") {
            fixtureType = POLYGON_FIXTURE;
        }
    }
    if (fixtureType == UNKNOWN_FIXTURE) {
        return;
    }
    else if (fixtureType == POLYGON_FIXTURE)
    {
        PointVector listPoint = this->initPolygonTexture(object);
        if (listPoint.size() > 0) {
            for (int i=0; i < listPoint.size(); i++) {
                log("%f %f", listPoint[i].x, listPoint[i].y);
            }
            log("fileName %s", fileName.c_str());
            TexturePolygon* tp = TexturePolygon::create(listPoint, fileName, body);
            if (tp != NULL) {
                _map->addChild(tp);
            }
        }
    }
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

BARRIER_TYPE TiledBodyCreator::getBarrierType(cocos2d::Value objectValue)
{
    BARRIER_TYPE type;

    std::string nameType = objectValue.asValueMap()["barrierType"].asString();

    // hex grid: no for touch
    if ( nameType == "hexgrid" )
    {
        type = BARRIER_TYPE::HEXGRID;
    }
    
    // conveyor belt
    else if ( nameType == "conveyor" )
    {
        type = BARRIER_TYPE::CONVEYOR;
    }
    
    // electricity
    else if ( nameType == "electricity" )
    {
        type = BARRIER_TYPE::ELECTRICITY;
    }
    
    // switch
    else if ( nameType == "switch" )
    {
        type = BARRIER_TYPE::SWITCH;
    }
    
    else if ( nameType == "physicseditor" )
    {
        type = BARRIER_TYPE::PHYSICSEDITOR;
    }
    
    else
    {
        type = BARRIER_TYPE::UNKNOWN;
    }
    return type;
}

std::vector<ConveyorBelt> TiledBodyCreator::getListConveyorBelt()
{
    return listConveyorBelt;
}

b2Fixture* TiledBodyCreator::getElectricityFixture()
{
    return electricityFixture;
}

b2Fixture* TiledBodyCreator::getSwitchFixture()
{
    return switchFixture;
}

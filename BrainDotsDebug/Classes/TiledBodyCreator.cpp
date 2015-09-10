//
//  TiledBodyCreator.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/02.
//
//

#include "TiledBodyCreator.h"

USING_NS_CC;

#define PTMRATIO 32

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
    cocos2d::ValueVector collisionObjects = layerGroup->getObjects();
    
    b2BodyDef bd;
    auto collisionBody = world->CreateBody(&bd);
    
    for(cocos2d::Value objectValue : collisionObjects)
    {
        auto fixtureShape = createFixture(objectValue.asValueMap());
        if(fixtureShape != NULL) {
            fixtureShape->fixture.filter.categoryBits = categorybits;
            fixtureShape->fixture.filter.maskBits = maskbits;
            collisionBody->CreateFixture(&fixtureShape->fixture);
        } else {
            CCLOG("Error when get objects");
        }
    }
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
    if(object["type"].asString() == "Circle") {
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
    auto position = Point(object["x"].asFloat() / PTMRATIO, object["y"].asFloat() / PTMRATIO);
    CCLOG("polygon position %f %f", object["x"].asFloat(), object["y"].asFloat());
    
    b2PolygonShape *polyshape = new b2PolygonShape();
    b2Vec2 vertices[b2_maxPolygonVertices];
    int vindex = 0;
    
    if(pointsVector.size() > b2_maxPolygonVertices) {
        CCLOG("Skipping TMX polygon at x=%d,y=%d for exceeding %d vertices", object["x"].asInt(), object["y"].asInt(), b2_maxPolygonVertices);
        return NULL;
    }
    
    auto fix = new FixtureDef();
    
    for(Value point : pointsVector) {
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / PTMRATIO + position.x);
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / PTMRATIO + position.y);
        CCLOG("polygon %f %f", point.asValueMap()["x"].asFloat(), point.asValueMap()["y"].asFloat());
        vindex++;
    }
    
    polyshape->Set(vertices, vindex);
    fix->fixture.shape = polyshape;
    
    return fix;
}


FixtureDef* TiledBodyCreator::createPolyline(ValueMap object)
{
    ValueVector pointsVector = object["polylinePoints"].asValueVector();
    auto position = Point(object["x"].asFloat() / PTMRATIO, object["y"].asFloat() / PTMRATIO);
    
    b2ChainShape *polylineshape = new b2ChainShape();
    float verticesSize = pointsVector.size()+ 1;
    b2Vec2 vertices[30];
    int vindex = 0;
    
    auto fix = new FixtureDef();
    
    for(Value point : pointsVector) {
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / PTMRATIO + position.x);
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / PTMRATIO + position.y);
        vindex++;
    }
    
    polylineshape->CreateChain(vertices, vindex);
    fix->fixture.shape = polylineshape;
    
    return fix;
}

FixtureDef* TiledBodyCreator::createCircle(ValueMap object)
{
    auto position = Point(object["x"].asFloat() / PTMRATIO, object["y"].asFloat() / PTMRATIO);
    float radius = object["width"].asFloat()/2 / PTMRATIO;
    
    b2CircleShape *circleshape = new b2CircleShape();
    circleshape->m_radius = radius;
    circleshape->m_p.Set(position.x + radius, position.y + radius);
    
    auto fix = new FixtureDef();
    fix->fixture.shape = circleshape;
    
    return fix;
}

FixtureDef* TiledBodyCreator::createRect(ValueMap object)
{
    auto position = Point(object["x"].asFloat() / PTMRATIO, object["y"].asFloat() / PTMRATIO);
    float width = object["width"].asFloat() / PTMRATIO;
    float height = object["height"].asFloat() / PTMRATIO;
    
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

std::vector<Rect> TiledBodyCreator::getRectListObjects(cocos2d::TMXTiledMap *map, std::string objectsName,std::string layerName)
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
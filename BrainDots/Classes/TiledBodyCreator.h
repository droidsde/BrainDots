//
//  TiledBodyCreator.h
//  BrainDots
//
//  Created by Nguyen Minh Hoang on 2015/09/01.
//
//

#ifndef __BrainDots__TiledBodyCreator__
#define __BrainDots__TiledBodyCreator__

#include <cocos2d.h>
#include <Box2D/Box2D.h>

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

class TiledBodyCreator
{
public:
    static void initMapLevel(TMXTiledMap* map, b2World* world, std::string layerName, uint16 categorybits, uint16 maskbits);
    static FixtureDef* createFixture(ValueMap object);
    
    static FixtureDef* createPolygon(ValueMap object);
    static FixtureDef* createPolyline(ValueMap object);
    static FixtureDef* createCircle(ValueMap object);
    static FixtureDef* createRect(ValueMap object);
    
    static std::vector<Rect> getRectListObjects(TMXTiledMap* map, std::string layerName);
};

#endif /* defined(__BrainDots__TiledBodyCreator__) */

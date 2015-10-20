//
//  GB2ShapeCache-x.cpp
//
//  Loads physics sprites created with https://www.codeandweb.com/physicseditor
//  To be used with cocos2d-x
//
//  Generic Shape Cache for box2d
//
//  Created by Thomas Broquist
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include "GB2ShapeCache-x.h"

static GB2ShapeCache *_sharedGB2ShapeCache = NULL;

GB2ShapeCache* GB2ShapeCache::getInstance(void) {
	if (!_sharedGB2ShapeCache) {
		_sharedGB2ShapeCache = new GB2ShapeCache();
        _sharedGB2ShapeCache->init();
	}

	return _sharedGB2ShapeCache;
}

bool GB2ShapeCache::init() {
	return true;
}

void GB2ShapeCache::reset() {
	std::map<std::string, BodyDef *>::iterator iter;
	for (iter = shapeObjects.begin() ; iter != shapeObjects.end() ; ++iter) {
		delete iter->second;
	}
	shapeObjects.clear();
}

void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape) {

	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
	assert(pos != shapeObjects.end());

	BodyDef *so = (*pos).second;

	FixtureDef *fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}

Vec2 GB2ShapeCache::getAnchorPointForShape(const std::string &shape) {
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
//	assert(pos != shapeObjects.end());
    if (pos == shapeObjects.end()) {
        CCLOG("not find");
    }

	BodyDef *bd = (*pos).second;
	return bd->anchorPoint;
}

void GB2ShapeCache::addShapesWithFile(const std::string &plist)
{
    std::string fullName = FileUtils::getInstance()->fullPathForFilename(plist);
    ValueMap dict =  FileUtils::getInstance()->getValueMapFromFile(fullName);
    
    if (dict.empty())
    {
        // plist file not found
        CCLOG("plist file empty or not existing");
        return;
    }
    
    ValueMap metadataDict = dict["metadata"].asValueMap();
    int format = Value(metadataDict["format"]).asInt();
    CCAssert(format == 1, "Format not supported");
    
    ValueMap bodyDict = dict["bodies"].asValueMap();
    
    b2Vec2 vertices[b2_maxPolygonVertices];
    
    for (auto pElement : bodyDict)
    {
        ValueMap bodyData = pElement.second.asValueMap();
        std::string bodyName = pElement.first;
        
        BodyDef *bodyDef = new BodyDef();
        std::string content = Value(bodyData["anchorpoint"]).asString();
        bodyDef->anchorPoint = PointFromString(content);
        
        ValueVector fixtureList = bodyData["fixtures"].asValueVector();
        FixtureDef **nextFixtureDef = &(bodyDef->fixtures);
        
        
        for (auto pObj : fixtureList)
        {
            ValueMap fixtureData = pObj.asValueMap();
            b2FixtureDef basicData;
            
            basicData.filter.categoryBits = Value(fixtureData["filter_categoryBits"]).asInt();
            basicData.filter.maskBits = Value(fixtureData["filter_maskBits"]).asInt();
            basicData.filter.groupIndex = Value(fixtureData["filter_groupIndex"]).asInt();
            basicData.friction = Value(fixtureData["friction"]).asFloat();
            basicData.density = Value(fixtureData["density"]).asFloat();
            basicData.restitution = Value(fixtureData["restitution"]).asFloat();
            basicData.isSensor = (bool)Value(fixtureData["isSensor"]).asBool();
            
            Value cb = Value(fixtureData["userdataCbValue"]);
            
            int callbackData = 0;
            
            if (!cb.isNull())
                callbackData = cb.asInt();
            
            std::string fixtureType = Value(fixtureData["fixture_type"]).asString();
            
            
            if (fixtureType == "POLYGON")
            {
                ValueVector polygonsArray = fixtureData["polygons"].asValueVector();
                
                
                for (auto pObjPolygons : polygonsArray)
                {
                    FixtureDef *fix = new FixtureDef();
                    fix->fixture = basicData; // copy basic data
                    fix->callbackData = callbackData;
                    
                    b2PolygonShape *polyshape = new b2PolygonShape();
                    int vindex = 0;
                    ValueVector polygonArray = pObjPolygons.asValueVector();
                    
                    
                    assert(polygonArray.size() <= b2_maxPolygonVertices);
                    
                    
                    for (auto pObjPolygon : polygonArray)
                    {
                        
                        std::string polygonValue = Value(pObjPolygon).asString();
                        Vec2 offset = PointFromString(polygonValue);
                        vertices[vindex].x = (offset.x / PTM_RATIO) ;
                        vertices[vindex].y = (offset.y / PTM_RATIO) ;
                        vindex++;
                    }
                    
                    polyshape->Set(vertices, vindex);
                    fix->fixture.shape = polyshape;
                    
                    // create a list
                    *nextFixtureDef = fix;
                    nextFixtureDef = &(fix->next);
                }
                
            }
            else if (fixtureType == "CIRCLE")
            {
                FixtureDef *fix = new FixtureDef();
                fix->fixture = basicData; // copy basic data
                fix->callbackData = callbackData;
                
                ValueMap circleData = fixtureData["circle"].asValueMap();
                
                b2CircleShape *circleShape = new b2CircleShape();
                
                circleShape->m_radius = Value(circleData["radius"]).asFloat() / PTM_RATIO;
                Vec2 p = PointFromString(Value(circleData["position"]).asString());
                circleShape->m_p = b2Vec2(p.x / PTM_RATIO, p.y / PTM_RATIO);
                fix->fixture.shape = circleShape;
                
                // create a list
                *nextFixtureDef = fix;
                nextFixtureDef = &(fix->next);
                
            }
            else
            {
                CCAssert(0, "Unknown fixtureType");
            }
            
            // add the body element to the hash
            shapeObjects[bodyName] = bodyDef;
        }
    }
}

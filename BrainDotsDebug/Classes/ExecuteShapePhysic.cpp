//
//  ExecuteShapePhysic.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/15.
//
//

#include "ExecuteShapePhysic.h"

Rect ExecuteShapePhysic::getBodyRectangle(Size visibleSize, b2Body* body) {
    float minX2 = visibleSize.width;
    float maxX2 = 0;
    float minY2 = visibleSize.height;
    float maxY2 = 0;
    
    const b2Transform& xf = body->GetTransform();
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
        
        b2PolygonShape* poly = (b2PolygonShape*) f->GetShape();
        int32 vertexCount = poly->m_count;
        b2Assert(vertexCount <= b2_maxPolygonVertices);
        
        for (int32 i = 0; i < vertexCount; ++i) {
            b2Vec2 vertex = b2Mul(xf, poly->m_vertices[i]);
            
            if (vertex.x < minX2) {
                minX2 = vertex.x;
            }
            
            if (vertex.x > maxX2) {
                maxX2 = vertex.x;
            }
            
            if (vertex.y < minY2) {
                minY2 = vertex.y;
            }
            
            if (vertex.y > maxY2) {
                maxY2 = vertex.y;
            }
        }
    }
    
    maxX2 *= PTM_RATIO;
    minX2 *= PTM_RATIO;
    maxY2 *= PTM_RATIO;
    minY2 *= PTM_RATIO;
    
    float width2 = maxX2 - minX2;
    float height2 = maxY2 - minY2;
    
    float remY2 = visibleSize.height - maxY2;
    
    return Rect(minX2, remY2, width2, height2);
}

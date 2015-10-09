//
//  textpoly.h
//  TexturePolygon
//
//  Created by Nguyen Minh Hoang on 2015/10/06.
//
//

#ifndef __TexturePolygon__textpoly__
#define __TexturePolygon__textpoly__

#include <cocos2d.h>
#include "Box2D/Box2D.h"

USING_NS_CC;

typedef std::vector<Point> CCPointVector;

class TexPoly : public Node {
public:
    TexPoly();
    virtual ~TexPoly();
    /**
     * @brief Create a physic texture polygon.
     * @param points    The polygon vertices. Must be a simple polygon (http://en.wikipedia.org/wiki/Simple_polygon) and points must not repeat.
     * @param texture   The texture filename. Must be a Power Of Two (POT) texture
     * @param world     Pointer to the box2d physic world
     */
    static TexPoly *create(const CCPointVector &points, const std::string &filename, b2World *world);
    static TexPoly *create(const CCPointVector &points, const CCPointVector &hole , const std::string &filename, b2World *world);
    virtual bool init(const CCPointVector &points, const CCPointVector &hole, const std::string &filename, b2World *world);
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    void setColor(const Color4F &color);
    
protected:
    template <class T> void freeContainer(T &cntr);
    
protected:
    std::vector<Vec3> mVertexPos;
    std::vector<Tex2F> mTexCoords;
    GLint mColorLocation;
    Color4F mColor;
    Texture2D *mTexture;
    BlendFunc _blendFunc;
    CustomCommand _customCommand;
};

#endif /* defined(__TexturePolygon__textpoly__) */

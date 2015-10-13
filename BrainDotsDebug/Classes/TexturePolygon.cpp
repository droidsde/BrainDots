//
//  textpoly.cpp
//  TexturePolygon
//
//  Created by Nguyen Minh Hoang on 2015/10/06.
//
//

#include "TexturePolygon.h"
#include "poly2tri.h"
#define PTM_RATIO 32

TexturePolygon::TexturePolygon()
: mTexture(nullptr)
{
    mColor = Color4F(1.f, 1.f, 1.f, 1.f);
}

TexturePolygon::~TexturePolygon()
{
    CC_SAFE_RELEASE(mTexture);
}

TexturePolygon *TexturePolygon::create(const PointVector &points, const std::string &filename, b2Body* body)
{
    PointVector empty;
    return create(points, empty, filename, body);
}

TexturePolygon *TexturePolygon::create(const PointVector &points, const PointVector &hole, const std::string &filename, b2Body* body)
{
    TexturePolygon *ptr = new TexturePolygon();
    if (ptr && ptr->init(points, hole, filename, body)) {
        ptr->autorelease();
        return ptr;
    }
    delete ptr;
    return nullptr;
}

bool TexturePolygon::init(const PointVector &points, const PointVector &hole, const std::string &filename, b2Body* body)
{
    if (filename.length() == 0) {
        CCLOGERROR("ERROR: Invalid texture filename (empty string)");
        return false;
    }
    
    /* STEP 1: Load texture into cache (if not already loaded) */
    mTexture = Director::getInstance()->getTextureCache()->addImage(filename.c_str());
    if (! mTexture) {
        CCLOGERROR("ERROR: Can't load the texture '%s'", filename.c_str());
        return false;
    }
    
    /**
     * STEP 2: convertion from CCPoint to p2t::Point
     */
    std::vector<p2t::Point*> polyline;
    for (const Point& p : points)
        polyline.push_back(new p2t::Point(p.x, p.y));
    
    std::vector<p2t::Point*> holePolyline;
    for (const Point& p : hole)
        holePolyline.push_back(new p2t::Point(p.x, p.y));
    
    /**
     * STEP 3: Create CDT and add primary polyline
     * NOTE: polyline must be a simple polygon. The polyline's points
     * constitute constrained edges. No repeat points!!!
     */
    p2t::CDT cdt(polyline);
    
    /// Add the hole
    if (holePolyline.size())
        cdt.AddHole(holePolyline);
    
    /**
     * STEP 4: Triangulate!
     */
    cdt.Triangulate();
    
    std::vector<p2t::Triangle*> triangles;
    triangles = cdt.GetTriangles();
    
    /**
     * STEP 5: create the Box2D body
     */
//    b2BodyDef bd;
//    bd.type = b2_dynamicBody;
//    bd.position = b2Vec2_zero;
//    bd.userData = this;
//    b2Body *body = world->CreateBody(&bd);
    body->SetUserData(this);
    
    /**
     * STEP 6: Create the fixtures
     */
    /// The box2d polygon shape
    b2PolygonShape shape;
    /// The fixture definition
    b2FixtureDef fd;
    fd.density = 6.f;
    fd.restitution = .2f;
    fd.shape = &shape;
    /// The polygon shape's vertices (actually a triangle)
    b2Vec2 v[3];
    /// Texture size in pixels
    Size texSize = mTexture->getContentSizeInPixels();
    
    /// Iterate over all triangles
    for (p2t::Triangle *tri : triangles) {
        /// Iterate over the 3 triangle's points
        for (int i = 0; i < 3; ++i) {
            const p2t::Point &p = *tri->GetPoint(i);
            /// Convert from pixels to meters
            v[i].Set(p.x / PTM_RATIO, p.y / PTM_RATIO);
            /// Populate the vertices position
            mVertexPos.push_back(Vec3(p.x, p.y, .0f));   /// x y z
            /// Populate the texture coordinates
            mTexCoords.push_back(Tex2F(p.x / texSize.width, 1 - p.y / texSize.height));
        }
        /// Create the fixture
        shape.Set(v, 3);
        body->CreateFixture(&fd);
    }
    
    /**
     * STEP 7: Configure the shader program
     */
    /// Enable texture repeat
    cocos2d::Texture2D::TexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    mTexture->setTexParameters(params);
    if ( !mTexture || !mTexture->hasPremultipliedAlpha() ) {
        _blendFunc.src = GL_SRC_ALPHA;
        _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    } else {
        _blendFunc.src = CC_BLEND_SRC;
        _blendFunc.dst = CC_BLEND_DST;
    }
    mTexture->retain();
    
    /// Create the shader program
    GLProgram *program = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR);
    mColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
    setGLProgram(program);
    
    /**
     * STEP 8: cleanup
     */
    freeContainer(polyline);
    freeContainer(holePolyline);
    
    return true; /// Success
}


void TexturePolygon::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    if (! isVisible()) return;
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(TexturePolygon::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    
}

void TexturePolygon::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
    /// Setup the OpenGL shader
    CC_NODE_DRAW_SETUP();
    /// Bind the 2D texture
    GL::bindTexture2D(mTexture->getName());
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    /// Enable shader attributes
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    
    /// Color
    getGLProgram()->setUniformLocationWith4f(mColorLocation, mColor.r, mColor.g, mColor.b, mColor.a);
    /// Vertex
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)&mVertexPos[0]);
    /// TexCoords
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Tex2F), (void*)&mTexCoords[0]);
    /// Available mode: GL_TRIANGLES, GL_TRIANGLES_STRIP and GL_TRIANGLE_FAN
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mVertexPos.size());
    
    CHECK_GL_ERROR_DEBUG();
    
    CC_INCREMENT_GL_DRAWS(1);
}

void TexturePolygon::setColor(const Color4F &color)
{
    mColor = color;
}

template <class T> void TexturePolygon::freeContainer(T &cntr)
{
    for (typename T::iterator it = cntr.begin(); it != cntr.end(); ++it)
        delete * it;
    cntr.clear();
}

#include "HelloWorldScene.h"

USING_NS_CC;

#define PTM_RATIO   32
#include "clipper.hpp"
#include "textpoly.h"
#include "B2DebugDrawLayer.h"
#include "PRFilledPolygon.h"

#define RADIUS      8
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setTag(10);
    this->addChild(menu, 1);

    // init physics
    this->initPhysics();
    
    // Add the box2d debug draw layer
    addChild(B2DebugDrawLayer::create(world, 32),INT_MAX);
    
    // init rendertexture and sprite draw
    target = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8_OES);
    target->retain();
    target->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(target);
    
    // brush
    brush = Sprite::create("brush.png");
    brush->retain();
    scheduleUpdate();
    
    // add touch
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

     /// Example of a concave polygon with hole
     int p[] = {80,200,112,120,160,72,256,88,336,120,368,248,352,296,272,312,256,248,192,216,160,232,112,280,64,248};
     CCPointVector path;
     for (int i = 0; i < 24; i += 2)
     path.push_back(Point(p[i],p[i+1]));
     
     int h[] = {144,168,192,120,272,136,320,200,288,232,240,168,192,184};
     CCPointVector hole;
     for (int i = 0; i < 14; i += 2)
     hole.push_back(Point(h[i],h[i+1]));
     
     /// Create the polygon and add it to the layer
//     addChild(TexPoly::create(path, hole, "pattern.png", world));

    CCPointVector test;
    test.push_back(Point(500, 500));
    test.push_back(Point(500, 200));
    test.push_back(Point(800, 200));
    test.push_back(Point(800, 500));
    test.push_back(Point(700, 500));
    test.push_back(Point(700, 400));
    test.push_back(Point(600, 400));
    test.push_back(Point(600, 500));
//    addChild(TexPoly::create(test, "pattern.png", world));
    
//    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("pattern.png");
//    FilledPolygon* filledPolygon = FilledPolygon::create(texture, test);
//    addChild(filledPolygon);
    return true;
}

void HelloWorld::initPhysics()
{
    // create physics world box2d
    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    
    //Define the ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    // create body
    groundBody = world->CreateBody(&groundBodyDef);
    
    //Define the ground box shape
    b2EdgeShape groundBox;
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundBox;
    groundFixtureDef.density = 0;
    
    // define 4 b2Vec2
    b2Vec2 lowerLeftCorner = b2Vec2(0, 0);
    b2Vec2 lowerRightCorner = b2Vec2(visibleSize.width/PTM_RATIO, 0);
    b2Vec2 upperLeftCorner = b2Vec2(0, visibleSize.height / PTM_RATIO);
    b2Vec2 upperRightCorner = b2Vec2(visibleSize.width/PTM_RATIO, (visibleSize.height)/PTM_RATIO);
    
    //ground
    groundBox.Set(lowerLeftCorner, lowerRightCorner);
    _wallFixture[0] = groundBody->CreateFixture(&groundFixtureDef);
    
    // top
    groundBox.Set(upperLeftCorner, upperRightCorner);
    _wallFixture[1] = groundBody->CreateFixture(&groundFixtureDef);
    
    // left
    groundBox.Set(lowerLeftCorner, upperLeftCorner);
    _wallFixture[2] = groundBody->CreateFixture(&groundFixtureDef);
    
    // right
    groundBox.Set(lowerRightCorner, upperRightCorner);
    _wallFixture[3] = groundBody->CreateFixture(&groundFixtureDef);
    
    // create for platform
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 6.0;
    boxFixtureDef.friction = 0.1;
}

void HelloWorld::update(float dt)
{
    if (!isOver) {
        
    int positionIterations = 8;
    int velocityIterations = 1;
    world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body *body = world->GetBodyList(); body != NULL; body =
         body->GetNext()) {
        if (body->GetUserData()) {
            Sprite *sprite = (Sprite *) body->GetUserData();
            sprite->setPosition(
                                Point(body->GetPosition().x * PTM_RATIO,
                                      body->GetPosition().y * PTM_RATIO));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
        }
    }
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
    
    platformPoints.clear();
    Vec2 location = touch->getLocation();
//    platformPoints.push_back(location);
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody;
    myBodyDef.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);
    currentPlatformBody = world->CreateBody(&myBodyDef);
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event) {
    
    Vec2 start = touch->getPreviousLocation();
    Vec2 end = touch->getLocation();
    if (start != end && start.getDistance(end) > 2) {
        platformPoints.push_back(end);
    }
    
    target->begin();
    float distance = start.getDistance(end);
        
    for (int i = 0; i < distance; i++) {
        float difX = end.x - start.x;
        float difY = end.y - start.y;
        float delta = (float) i / distance;
        Sprite * sprite = Sprite::createWithTexture(brush->getTexture());
        sprite->setPosition(Vec2(start.x + (difX * delta), start.y + (difY * delta)));
        sprite->visit();
    }
    target->end();
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
//    Vec2 location = touch->getLocation();
//    platformPoints.push_back(location);
    
    createBodyByRender();
    
//    createBodyByTextPoly();
    
    // remove and retain
    target->clear(0, 0, 0, 0);
//    removeChild(target, true);
//    target->release();
//    target = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8_OES);
//    target->retain();
//    target->setPosition(visibleSize.width / 2, visibleSize.height / 2);
//    addChild(target);
}

void HelloWorld::createBodyByRender()
{
    if (platformPoints.size() > 1) {
        //Add a new body/atlas sprite at the touched location
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
        myBodyDef.position.Set(currentPlatformBody->GetPosition().x,
                               currentPlatformBody->GetPosition().y); //set the starting position
        world->DestroyBody(currentPlatformBody);
        b2Body* newBody = world->CreateBody(&myBodyDef);
        CCLOG("size of list fixture %zd", platformPoints.size());
        for (int i = 0; i < platformPoints.size() - 1; i++) {
            Vec2 start = platformPoints.at(i);
            Vec2 end = platformPoints.at(i+1);
            this->addRectangleBetweenPointsToBody(newBody, start, end);
        }
        
        Rect bodyRectangle = this->getBodyRectangle(newBody);
        float anchorX = newBody->GetPosition().x * PTM_RATIO - bodyRectangle.origin.x;
        float anchorY = bodyRectangle.size.height - (visibleSize.height - bodyRectangle.origin.y - newBody->GetPosition().y * PTM_RATIO);
        
        Vec2 anchorPoint = Vec2(anchorX / bodyRectangle.size.width, anchorY / bodyRectangle.size.height);
        
        // draw sprite use render texture
//        auto sprite = Sprite::create();
//        sprite->createWithTexture(target->getSprite()->getTexture(), bodyRectangle);
//        sprite->setAnchorPoint(anchorPoint);
//        addChild(sprite);
        
        auto _image = target->newImage();
        CCLOG("size image %d %d", _image->getWidth(), _image->getHeight());
        auto _key = to_string((int) time(NULL));
        auto _texture2D = Director::getInstance()->getTextureCache()->addImage(_image, _key);
        CC_SAFE_DELETE(_image);
        auto texture2D = Sprite::createWithTexture(_texture2D, bodyRectangle);
        CCLOG("size of texture2D %f %f", texture2D->getContentSize().width, texture2D->getContentSize().height);
        texture2D->setAnchorPoint(anchorPoint);
        this->addChild(texture2D);
        newBody->SetUserData(texture2D);
        Director::getInstance()->getTextureCache()->removeTexture(_texture2D);
    }
}

void HelloWorld::createBodyByTextPoly()
{
    if (platformPoints.size() > 1) {
        //Add a new body/atlas sprite at the touched location
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
        myBodyDef.position.Set(currentPlatformBody->GetPosition().x,
                               currentPlatformBody->GetPosition().y); //set the starting position
        world->DestroyBody(currentPlatformBody);
        b2Body* newBody = world->CreateBody(&myBodyDef);
        CCLOG("size of list fixture %zd", platformPoints.size());
        for (int i = 0; i < platformPoints.size() - 1; i++) {
            Vec2 start = platformPoints.at(i);
            Vec2 end = platformPoints.at(i+1);
            this->addRectangleBetweenPointsToBody(newBody, start, end);
        }
        // test
        getRectsFromBody(newBody);
        world->DestroyBody(newBody);
        
        std::reverse(upArray.begin(), upArray.end());
        CCPointVector test;
        for (int i = 0 ; i < platformPoints.size(); i++) {
            CCLOG("draw %f %f", platformPoints[i].x, platformPoints[i].y);
        }
        
        for (int i = 0; i < upArray.size(); i++) {
            test.push_back(upArray[i]);
        }
        for (int i = 0; i < downArray.size(); i++) {
            test.push_back(downArray[i]);
        }
        
        for (int i = 0 ; i < test.size(); i++) {
            CCLOG("test %f %f", test[i].x, test[i].y);
        }
        
        auto draw = TexPoly::create(test, "pattern.png", world);
        addChild(draw);
    }

}

void HelloWorld::getRectsFromBody(b2Body* body)
{
    upArray.clear();
    downArray.clear();
    
    const b2Transform& xf = body->GetTransform();
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
        
        b2PolygonShape* poly = (b2PolygonShape*) f->GetShape();
        int32 vertexCount = poly->m_count;
        b2Assert(vertexCount <= b2_maxPolygonVertices);
        
        for (int32 i = 0; i < vertexCount; ++i) {
            b2Vec2 vertex = b2Mul(xf, poly->m_vertices[i]);
            CCLOG("x = %f y = %f", vertex.x * PTM_RATIO, vertex.y * PTM_RATIO);
        }
        b2Vec2 vertex0 = b2Mul(xf, poly->m_vertices[0]);
        b2Vec2 vertex1 = b2Mul(xf, poly->m_vertices[1]);
        b2Vec2 vertex2 = b2Mul(xf, poly->m_vertices[2]);
        b2Vec2 vertex3 = b2Mul(xf, poly->m_vertices[3]);
        upArray.push_back(Point(vertex0.x * PTM_RATIO, vertex0.y * PTM_RATIO));
//        if (f->GetNext() == nullptr) {
//            upArray.push_back(Point(vertex1.x * PTM_RATIO, vertex1.y * PTM_RATIO));
//            downArray.push_back(Point(vertex2.x * PTM_RATIO, vertex2.y * PTM_RATIO));
//        }
        downArray.push_back(Point(vertex3.x * PTM_RATIO, vertex3.y * PTM_RATIO));
    }
}

void HelloWorld::addRectangleBetweenPointsToBody(b2Body* body, Vec2 start,
                                                Vec2 end) {
    
    float minW = brush->boundingBox().size.width / PTM_RATIO ;
    float minH = brush->boundingBox().size.height / PTM_RATIO;
    
    float sx = start.x;
    float sy = start.y;
    float ex = end.x;
    float ey = end.y;
    float dist_x = sx - ex;
    float dist_y = sy - ey;
    float angle = atan2(dist_y, dist_x);
    
    float px = (sx + ex) / 2 / PTM_RATIO - body->GetPosition().x;
    float py = (sy + ey) / 2 / PTM_RATIO - body->GetPosition().y;
    
    float height = brush->boundingBox().size.height / PTM_RATIO;
    if (dist_x < minW) {
        dist_x = start.distance(end);
    } else {
        if (abs(dist_y) > minH) {
            dist_x = start.distance(end);
        }
    }
    float width = MAX(abs(dist_x) / PTM_RATIO, minW);
    
    boxShape.SetAsBox(width / 2, height / 2, b2Vec2(px, py), angle);
    
    body->CreateFixture(&boxFixtureDef);
}

Rect HelloWorld::getBodyRectangle(b2Body* body) {
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


void HelloWorld::menuCloseCallback(Ref* pSender)
{
//    Director::getInstance()->end();
    CCLOG("size child %zd", this->getChildrenCount());
    isOver = true;
    for(long i = this->getChildrenCount()-1; i > 0; i--){
        Node* child = this->getChildren().at(i);
        if (child->getTag() != 10) {
            this->removeChild(child);
        }
    }
    
    for (b2Body *body = world->GetBodyList(); body != NULL; body =
         body->GetNext()) {
        if (body != groundBody) {
            world->DestroyBody(body);
        }
    }
    isOver = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
#endif
}

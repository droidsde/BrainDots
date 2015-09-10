#include "GameScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

#define PTM_RATIO 32.0 // 32px = 1m in Box2D
const float BALL_RADIUS = 25.0f;
const float OUTSIDE = 500;

// category definiti
const short CATEGORY_BALL = 0x0001;
const short CATEGORY_PLATFORM = 0x0002;
const short CATEGORY_BARRAGE = 0x0004;
const short CATEGORY_WALL1 = 0x0008;
const short CATEGORY_WALL2 = 0x0016;

// maskbit definiti
const short MASK_BALL = -1;
const short MASK_PLATFORM = CATEGORY_BALL | CATEGORY_BARRAGE | CATEGORY_PLATFORM | CATEGORY_WALL2;
const short MASK_BARRAGE = -1;
const short MASK_WALL1 = CATEGORY_BALL | CATEGORY_BARRAGE;
const short MASK_WALL2 = CATEGORY_BALL | CATEGORY_BARRAGE | CATEGORY_PLATFORM;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    CCLOG("size screen %f %f", visibleSize.width, visibleSize.height);
    
    // button back
    auto backButton = Button::create("back.png");
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(PADDING, visibleSize.height - PADDING));
    backButton->setTouchEnabled(true);
    backButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchButtonEvent, this));
    backButton->setTag(TAG_GAME::TAG_BUTTON_BACK);
    addChild(backButton, ZORDER_GAME::ZORDER_BUTTON_BACK);
    
    // button replay
    auto replayButton = Button::create("replay.png");
    replayButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    replayButton->setPosition(Vec2(visibleSize.width - PADDING, visibleSize.height - PADDING));
    replayButton->setTouchEnabled(true);
    replayButton->setTag(TAG_GAME::TAG_BUTTON_REPLAY);
    replayButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchButtonEvent, this));
    addChild(replayButton, ZORDER_GAME::ZORDER_BUTTON_REPLAY);
    
    // draw node
    drawnode = DrawNode::create();
    addChild(drawnode);
    
    // init physics
    this->initPhysics();
    
    // init map level
    this->initMapLevel(SceneManager::getInstance()->getLevelGame());
    
    // init physics object
    this->initPhysicObjects();
    this->initBalls();
    
    // init rendertexture and sprite draw
    target = RenderTexture::create(visibleSize.width, visibleSize.height,
                                   Texture2D::PixelFormat::RGBA8888);
    target->retain();
    target->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(target);
    
    // init rendertexture capture screen
    captureScreen = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
    captureScreen->retain();
    captureScreen->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(captureScreen);
    
    brush = Sprite::create("brush.png");
    brush->retain();
    this->schedule(schedule_selector(GameScene::update));
    
    // add touch
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void GameScene::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated) {
    Layer::draw(renderer, transform, transformUpdated);
    Director* director = Director::getInstance();
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    world->DrawDebugData();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GameScene::initPhysics()
{
    // create physics world box2d
    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    
    // add debug draw
    this->debugDraw = new GLESDebugDraw( PTM_RATIO );
    this->world->SetDebugDraw(debugDraw);
    
    // add contact
    _ballContactListener = new BallContactListener();
    world->SetContactListener(_ballContactListener);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    this->debugDraw->SetFlags(flags);
}

void GameScene::initMapLevel(int level)
{
    //reading in a tiled map
    std::string nameLevel = "level" + to_string(level) + ".tmx";
    map = TMXTiledMap::create(nameLevel);
    if (map==nullptr) {
        CCLOG("file not found");
        
    } else {
        CCLOG("size of map level %f %f", map->getContentSize().width, map->getContentSize().height);
        addChild(map, ZORDER_GAME::ZORDER_MAPLEVEL, 1);
        
        // auto create physics objects
        TiledBodyCreator::initMapLevel(map, world, "braindots", CATEGORY_BARRAGE, MASK_BARRAGE);
        
        // get ball group
        auto group = map->getObjectGroup("braindots");
        CCASSERT(NULL != group, "braindots group not found");
        
        // ball A
        auto ballA_map = group->getObject("ballA");
        CCASSERT(!ballA_map.empty(), "ball A not found");
        float xA = ballA_map["x"].asFloat();
        float yA = ballA_map["y"].asFloat();
        posballA = Vec2(xA, yA);
        CCLOG("%f %f",xA,yA);
        
        // ball B
        auto ballB_map = group->getObject("ballB");
        CCASSERT(!ballB_map.empty(), "ball B not found");
        float xB = ballB_map["x"].asFloat();
        float yB = ballB_map["y"].asFloat();
        posballB = Vec2(xB, yB);
        CCLOG("%f %f",xB,yB);
        
        // hex grid
        std::vector<Rect> listRectGrid;
        auto gridGroup = map->getObjectGroup("hexgridobjects");
        if (gridGroup==nullptr) {
            CCLOG("hexgridobjects group not found");
        } else {
            listGirdLayer.clear();
            listRectGrid = TiledBodyCreator::getRectListObjects(map, "hexgridobjects", "hexgridlayer");
            if (listRectGrid.size() > 0) {
                for (int i = 0; i < listRectGrid.size(); i++) {
                    Rect rect = listRectGrid[i];
                    auto layer = LayerColor::create();
                    layer->setContentSize(Size(rect.size));
                    layer->setPosition(Vec2(rect.origin));
                    CCLOG("layer %f %f %f %f", layer->getPositionX(), layer->getPositionY(), layer->getContentSize().width, layer->getContentSize().height);
                    addChild(layer, 100);
                    listGirdLayer.push_back(layer);
                    
                    // add touch
                    auto listener = EventListenerTouchOneByOne::create();
                    listener->setSwallowTouches(true);
                    listener->onTouchBegan = [&](Touch* touch, Event* event)
                    {
                        auto bounds = event->getCurrentTarget()->getBoundingBox();
                        if (bounds.containsPoint(touch->getLocation())) {
                            CCLOG("layer touch begin");
                            return true;
                        }
                        else return false;
                    };
                    
                    listener->onTouchMoved = [=](Touch* touch, Event* event){
                    };
                    
                    listener->onTouchEnded = [=](Touch* touch, Event* event){
                        
                    };
                    
                    // Add listener
                    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
                }
            }
        }
    }
}

void GameScene::initPhysicObjects()
{
    //Define the ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    // create body
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    
    // create 2 box wall
    initWall(groundBody, _wallFixture1, BALL_RADIUS, CATEGORY_WALL1, MASK_WALL1);
    initWall(groundBody, _wallFixture2, OUTSIDE, CATEGORY_WALL2, MASK_WALL2);
}


void GameScene::initWall(b2Body *body, b2Fixture* _wallFixture[], float outside, uint16 categorybits, uint16 maskbits)
{
    //Define the ground box shape
    b2EdgeShape groundBox;
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundBox;
    groundFixtureDef.density = 0;
    groundFixtureDef.filter.categoryBits = categorybits;
    groundFixtureDef.filter.maskBits = maskbits;
    
    // define 4 b2Vec2
    b2Vec2 lowerLeftCorner = b2Vec2(-outside/PTM_RATIO, -outside/PTM_RATIO);
    b2Vec2 lowerRightCorner = b2Vec2((visibleSize.width+outside)/PTM_RATIO, -outside/PTM_RATIO);
    b2Vec2 upperLeftCorner = b2Vec2(-outside/PTM_RATIO, (visibleSize.height+outside)/PTM_RATIO);
    b2Vec2 upperRightCorner = b2Vec2((visibleSize.width+outside)/PTM_RATIO, (visibleSize.height+outside)/PTM_RATIO);
    
    //ground
    groundBox.Set(lowerLeftCorner, lowerRightCorner);
    _wallFixture[0] = body->CreateFixture(&groundFixtureDef);
    
    // top
    groundBox.Set(upperLeftCorner, upperRightCorner);
    _wallFixture[1] = body->CreateFixture(&groundFixtureDef);
    
    // left
    groundBox.Set(lowerLeftCorner, upperLeftCorner);
    _wallFixture[2] = body->CreateFixture(&groundFixtureDef);
    
    // right
    groundBox.Set(lowerRightCorner, upperRightCorner);
    _wallFixture[3] = body->CreateFixture(&groundFixtureDef);
}

void GameScene::initBalls()
{
    auto ballASprite = Sprite::create("ball_red.png");
    map->addChild(ballASprite);
//        ballASprite->setPosition(Vec2(visibleSize.width/4, visibleSize.height/2));
    ballASprite->setPosition(posballA);
    
    auto ballBSprite = Sprite::create("ball_blue.png");
    map->addChild(ballBSprite);
//        ballBSprite->setPosition(Vec2(visibleSize.width*3/4, visibleSize.height/2));
    ballBSprite->setPosition(posballB);
    
    // shape of body
    b2CircleShape circleShape;
    circleShape.m_radius = BALL_RADIUS / PTM_RATIO;
    
    // fixturedef
    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &circleShape;
    ballFixtureDef.density = 10.0f;
    ballFixtureDef.friction = 0;
    ballFixtureDef.restitution = 0;
    ballFixtureDef.filter.categoryBits = CATEGORY_BALL;
    ballFixtureDef.filter.maskBits = MASK_BALL;
    
    // body definition
    b2BodyDef mBallDefA;
    mBallDefA.position.Set(ballASprite->getPositionX() / PTM_RATIO, ballASprite->getPositionY()/ PTM_RATIO);
    mBallDefA.type = b2_staticBody;
    
    b2BodyDef mBallDefB;
    mBallDefB.position.Set(ballBSprite->getPositionX() / PTM_RATIO, ballBSprite->getPositionY()/ PTM_RATIO);
    mBallDefB.type = b2_staticBody;
    
    ballA = world->CreateBody(&mBallDefA);
    _ballAFixture = ballA->CreateFixture(&ballFixtureDef);
    ballA->SetUserData(ballASprite);
    
    ballB = world->CreateBody(&mBallDefB);
    _ballBFixture = ballB->CreateFixture(&ballFixtureDef);
    ballB->SetUserData(ballBSprite);
    
}

void GameScene::update(float dt) {
    
    int positionIterations = 8;
    int velocityIterations = 1;
    if (!gameOver) {
        world->Step(dt, velocityIterations, positionIterations);
    } else {
        CCLOG("endgames %f %f", collisionPoint.x, collisionPoint.y);
        ParticleSystemQuad* starParticle = ParticleSystemQuad::create("star_particle.plist");
        starParticle->setPosition(collisionPoint);
        starParticle->setAutoRemoveOnFinish(true);
        starParticle->retain();
        
        ParticleSystemQuad* ringParticle = ParticleSystemQuad::create("ring_particle.plist");
        ringParticle->setPosition(collisionPoint);
        ringParticle->setAutoRemoveOnFinish(true);
        ringParticle->retain();
        
        auto call1 = CallFunc::create([=]()
                                      {
                                          this->addChild(ringParticle);
                                      });
        auto call2 = CallFunc::create([=]()
                                      {
                                          this->addChild(starParticle);
                                      });
        
        this->runAction(Sequence::create(call1, DelayTime::create(1.0f), call2, DelayTime::create(2.0f), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)),  NULL));
        
        
        this->unschedule(schedule_selector(GameScene::update));
        return;
    }
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
    
    std::vector<b2Body *>toStatic;
    std::vector<b2Body *>toDestroy;
    std::vector<BallContact>::iterator pos;
    for (pos = _ballContactListener->_contacts.begin(); pos != _ballContactListener->_contacts.end(); ++pos)
    {
        BallContact contact = *pos;
        b2Body* bodyA = contact.fixtureA->GetBody();
        b2Body* bodyB = contact.fixtureB->GetBody();
        
        // contact ball with wall1
        for (int i=0; i<4; i++) {
            if((contact.fixtureA == _ballAFixture && contact.fixtureB == _wallFixture1[i]) ||
               (contact.fixtureA == _wallFixture1[i] && contact.fixtureB == _ballAFixture) ||
               (contact.fixtureA == _ballBFixture && contact.fixtureB == _wallFixture1[i]) ||
               (contact.fixtureA == _wallFixture1[i] && contact.fixtureB == _ballBFixture))
            {
                collisionPoint = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
                gameOver = true;
            }
        }
        
        // contact platform with wall2
        for (int i=0; i<4; i++) {
            if (contact.fixtureA == _wallFixture2[i])
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end()) {
                    toDestroy.push_back(bodyB);
                }
            }
            else if( contact.fixtureB == _wallFixture2[i])
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end()) {
                    toDestroy.push_back(bodyA);
                }
            }
        }
        
        // contact 2 ball
        
        if (bodyA && bodyB && ballA && ballB && bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
            if ((bodyA == ballA && bodyB == ballB) || (bodyA == ballB && bodyB == ballA)) {
                if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody) {
                    toStatic.push_back(bodyA);
                    toStatic.push_back(bodyB);
                    collisionPoint = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
                    gameOver = true;
                }
            }
        }
    }
    
    std::vector<b2Body *>::iterator pos2;
    for(pos2 = toStatic.begin(); pos2 != toStatic.end(); ++pos2) {
        b2Body *body = *pos2;
        if (body->GetType() == b2_dynamicBody) {
            body->SetType(b2_staticBody);
        }
    }
    
    // remove body run out
    std::vector<b2Body *>::iterator pos3;
    for(pos3 = toDestroy.begin(); pos3 != toDestroy.end(); ++pos3) {
        b2Body *body = *pos3;
        if (body->GetUserData() != NULL) {
            Sprite *sprite = (Sprite *) body->GetUserData();
            this->removeChild(sprite, true);
        }
        world->DestroyBody(body);
        CCLOG("remove a body");
    }
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    drawnode->clear();
    if (gameOver) {
        return false;
    }
    // touch in grid layer
    if (listGirdLayer.size() > 0) {
        for (int i = 0; i<listGirdLayer.size(); i++) {
            if (listGirdLayer[i]->getBoundingBox().containsPoint(touch->getLocation())) {
                return false;
            }
        }
    }
    
    // touch in any physic body
    if (checkBodyWeighOnSomebody(touch->getLocation(), touch->getLocation()) != Vec2::ZERO)
    {
        return false;
    }
    
    srand((int)time(NULL));
    int r = rand() % 255;
    int b = rand() % 255;
    int g = rand() % 255;
    brush->setColor(Color3B(r, b, g));
    
    platformPoints.clear();
    Vec2 location = touch->getLocation();
    platformPoints.push_back(location);
    previousLocation = location;
    
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody;
    myBodyDef.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);
    currentPlatformBody = world->CreateBody(&myBodyDef);
    return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
    
    Vec2 start = touch->getLocation();
    Vec2 end = touch->getPreviousLocation();
    Vec2 collision = checkBodyWeighOnSomebody(start, end);
    
    if (listGirdLayer.size() > 0) {
        for (int i = 0; i<listGirdLayer.size(); i++) {
            if (listGirdLayer[i]->getBoundingBox().containsPoint(start)) {
                return;
            }
        }
    }
    if (collision == Vec2::ZERO) {
        platformPoints.push_back(start);
        if (isErrorDraw) {
            isErrorDraw = false;
        }
    }
    else {
        if (!isErrorDraw) {
            if (posErrorDraw != collision) {
                posErrorDraw = collision;
            }
        }
        isErrorDraw = true;
        start = collision;
    }
    
    // if draw no problem
    if (!isErrorDraw)
    {
        target->begin();
        float distance = start.getDistance(end);
        
        for (int i = 0; i < distance; i++) {
            float difX = end.x - start.x;
            float difY = end.y - start.y;
            float delta = (float) i / distance;
            Sprite * sprite = Sprite::createWithTexture(brush->getTexture());
            sprite->setPosition(Vec2(start.x + (difX * delta), start.y + (difY * delta)));
            sprite->setColor(brush->getColor());
            sprite->visit();
        }
        target->end();
    }
    
    previousLocation = start;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
    
    if (ballA && ballB) {
        if (ballA->GetType() == b2_staticBody) {
            ballA->SetType(b2_dynamicBody);
        }
        if (ballB->GetType() == b2_staticBody) {
            ballB->SetType(b2_dynamicBody);
        }
    }
    if (platformPoints.size() > 1) {
        //Add a new body/atlas sprite at the touched location
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
        myBodyDef.position.Set(currentPlatformBody->GetPosition().x,
                               currentPlatformBody->GetPosition().y); //set the starting position
        world->DestroyBody(currentPlatformBody);
        b2Body* newBody = world->CreateBody(&myBodyDef);
        for (int i = 0; i < platformPoints.size() - 1; i++) {
            Vec2 start = platformPoints[i];
            Vec2 end = platformPoints[i + 1];
            this->addRectangleBetweenPointsToBody(newBody, start, end);
        }
        Rect bodyRectangle = getBodyRectangle(newBody);
        float anchorX = newBody->GetPosition().x * PTM_RATIO
        - bodyRectangle.origin.x;
        float anchorY = bodyRectangle.size.height
        - (visibleSize.height - bodyRectangle.origin.y
           - newBody->GetPosition().y * PTM_RATIO);
        
        Vec2 anchorPoint = Vec2(anchorX / bodyRectangle.size.width,
                                anchorY / bodyRectangle.size.height);
        
        // draw sprite use render texture
        Director::getInstance()->getRenderer()->render();
        auto _image = target->newImage();
        auto _key = to_string((int) time(NULL));
        auto _texture2D = Director::getInstance()->getTextureCache()->addImage(
                                                                               _image, _key);
        CC_SAFE_DELETE(_image);
        auto texture2D = Sprite::createWithTexture(_texture2D, bodyRectangle);
        texture2D->setAnchorPoint(anchorPoint);
        map->addChild(texture2D);
        newBody->SetUserData(texture2D);
    }
    removeChild(target, true);
    //	target->release();
    target = RenderTexture::create(visibleSize.width, visibleSize.height,
                                   Texture2D::PixelFormat::RGBA8888);
    target->retain();
    target->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    addChild(target);
}

void GameScene::addRectangleBetweenPointsToBody(b2Body* body, Vec2 start,
                                                 Vec2 end) {
    drawnode->drawDot(start, 1, Color4F::RED);
    drawnode->drawDot(end, 1, Color4F::GREEN);
    
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
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2, height / 2, b2Vec2(px, py), angle);
    
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 5;
    boxFixtureDef.filter.categoryBits = CATEGORY_PLATFORM;
    boxFixtureDef.filter.maskBits = MASK_PLATFORM;
    
    body->CreateFixture(&boxFixtureDef);
}

Rect GameScene::getBodyRectangle(b2Body* body) {
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

std::vector<Vec2> GameScene::getListPointsIn2Point(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
    std::vector<Vec2> list;
    float startX, endX;
    float A, B;
    
    float x1 = start.x;
    float y1 = start.y;
    float x2 = end.x;
    float y2 = end.y;
    
    if (x1 - x2 == 0) {
        A = 0;
    } else
    {
        A = (y1 - y2) / (x1 - x2);
    }
    B = y1 - x1 * A;
    
    if (x1 > x2) {
        startX = x2;
        endX = x1;
    } else {
        startX = x1;
        endX = x2;
    }
    for (float i=startX; i<=endX; i++) {
        Vec2 acceptPoint = Vec2(i, A * i + B);
        list.push_back(acceptPoint);
    }
    return list;
}

Vec2 GameScene::checkBodyWeighOnSomebody(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
    Vec2 result = Vec2::ZERO;
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        
        b2Fixture *f = body->GetFixtureList();
        std::vector<Vec2> listPoints;
        listPoints.clear();
        listPoints = getListPointsIn2Point(start, end);
        
        while(f)
        {
            for (int i=0; i<listPoints.size(); i++) {
                b2Vec2 checkPoint = b2Vec2(listPoints[i].x / PTM_RATIO, listPoints[i].y / PTM_RATIO);
                if(f -> TestPoint(checkPoint))
                {
                    result = listPoints[i];
                    CCLOG("You touched a body");
                    return result;
                }
            }
            f = f->GetNext();            
        }
    }
    return result;
}

void GameScene::backMenu() {
    this->m_bClearBox = true;
    if (m_bClearBox) {
        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
            world->DestroyBody(b);
        }
        this->removeChild(target, true);
        _brushs.clear();
        this->removeAllChildren();
        m_bClearBox = false;
    }
    SceneManager::getInstance()->changeState(GAME_STATE::MENU);
}

void GameScene::touchButtonEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    auto receiver = (Node*) sender;
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        switch (receiver->getTag()) {
            case TAG_GAME::TAG_BUTTON_BACK :
                backMenu();
                break;
            case TAG_GAME::TAG_BUTTON_REPLAY:
                SceneManager::getInstance()->changeState(GAME_STATE::GAME);
                break;
        }
    }
}

void GameScene::endGame()
{
    captureScreen->begin();
    Director::getInstance()->getRunningScene()->visit();
    captureScreen->end();
    
    Director::getInstance()->getRenderer()->render();
    auto _image = captureScreen->newImage();
    auto _key = to_string((int) time(NULL));
    auto _texture2D = Director::getInstance()->getTextureCache()->addImage(
                                                                           _image, _key);
    CC_SAFE_DELETE(_image);
    auto texture2D = Sprite::createWithTexture(_texture2D);
    texture2D->setPosition(visibleSize/2);
    texture2D->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(texture2D);
    removeChild(captureScreen, true);
    
    // create tick
    auto tick = Sprite::create("mini_tick.png");
    tick->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    tick->setPosition(Vec2(texture2D->getContentSize().width - PADDING, texture2D->getContentSize().height - PADDING));
    tick->retain();
    auto addtick = CallFunc::create([texture2D, tick] {
        texture2D->addChild(tick);
    });
    texture2D->runAction(Sequence::create(ScaleTo::create(0.5, 0.5f), DelayTime::create(0.5), addtick, nullptr));
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

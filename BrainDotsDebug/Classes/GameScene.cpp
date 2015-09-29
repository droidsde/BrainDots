#include "GameScene.h"

GameScene::GameScene()
{
    CCLOG("GameScene::GameScene()");
    map = nullptr;
    tiledmap = nullptr;
    drawnode = nullptr;
    world = nullptr;
    currentPlatformBody = nullptr;
    target = nullptr;
    brush = nullptr;
    _ballContactListener = nullptr;
}

GameScene::~GameScene()
{
    CCLOG("GameScene::~GameScene()");
    map = nullptr;
    drawnode = nullptr;
    delete tiledmap;
    tiledmap = nullptr;
    brush = nullptr;
    delete world;
    world = nullptr;
    
    delete _ballContactListener;
    CC_SAFE_RELEASE(target);
    delete this->debugDraw;
    this->debugDraw = nullptr;
}

Scene* GameScene::createScene()
{
    CCLOG("GameScene::createScene()");
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    CCLOG("GameScene::init()");
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    // draw grid
    this->drawGrids();
    
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
    
    // init physics
    this->initPhysics();
    
    // init map level
    this->initMapLevel(SceneManager::getInstance()->getLevelGame());
    
    // init physics object
    this->initPhysicObjects();
    this->initBalls();
    
//    this->revoluteJoint();
//    this->weldJoint();
    
    // init rendertexture and sprite draw
    target = RenderTexture::create(visibleSize.width, visibleSize.height,
                                   Texture2D::PixelFormat::RGBA8888);
    target->retain();
    target->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(target, ZORDER_GAME::ZORDER_DRAW_RENDER);
    
    // brush
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
    
    // set scale
    this->setScale(0.2);
    
    return true;
}

void GameScene::revoluteJoint()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    b2FixtureDef fixtureDef;
    fixtureDef.density = 10;
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(5, 1);
    b2CircleShape circleShape;
    circleShape.m_radius = 1;
    
    bodyDef.position.Set(600 / PTM_RATIO, 600 / PTM_RATIO);
    fixtureDef.shape = &boxShape;
    b2Body* m_bodyA = world->CreateBody(&bodyDef);
    m_bodyA->CreateFixture(&fixtureDef);
    
    bodyDef.position.Set(600 / PTM_RATIO, 600 / PTM_RATIO);
    bodyDef.type = b2_staticBody;
    fixtureDef.shape = &circleShape;
    b2Body* m_bodyB = world->CreateBody(&bodyDef);
    m_bodyB->CreateFixture(&fixtureDef);
    
    b2RevoluteJointDef revoluteJointDef;
    revoluteJointDef.bodyA = m_bodyA;
    revoluteJointDef.bodyB = m_bodyB;
    revoluteJointDef.collideConnected = false;
    revoluteJointDef.localAnchorA.Set(0,0);
    revoluteJointDef.localAnchorB.Set(0,0);
    
//    revoluteJointDef.enableLimit = true;
    //revoluteJointDef.lowerAngle = -90 * PI / 180;
    //revoluteJointDef.upperAngle =  90 * PI / 180;
    
    revoluteJointDef.enableMotor = true;
    revoluteJointDef.maxMotorTorque = 150;
    revoluteJointDef.motorSpeed = 100;
    
    world->CreateJoint(&revoluteJointDef );
    
}

void GameScene::weldJoint()
{
    b2Body* mBodyA, *mBodyB;
    
    float posX = visibleSize.width/2;
    
    // box A
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(posX / PTM_RATIO, 600 / PTM_RATIO);
        bodyDef.angle = 0;
        
        b2PolygonShape boxShape;
        boxShape.SetAsBox(5, 1);
        
        b2FixtureDef fixtureDef;
        fixtureDef.density = 10;
        fixtureDef.shape = &boxShape;
        
        mBodyA = world->CreateBody(&bodyDef);
        mBodyA->CreateFixture(&fixtureDef);
    }
    
    // box B
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(posX / PTM_RATIO, 600 / PTM_RATIO);
        bodyDef.angle = -90 * b2_pi / 180;
        
        b2PolygonShape boxShape;
        boxShape.SetAsBox(5, 1);
        
        b2FixtureDef fixtureDef;
        fixtureDef.density = 10;
        fixtureDef.shape = &boxShape;
        
        mBodyB = world->CreateBody(&bodyDef);
        mBodyB->CreateFixture(&fixtureDef);
    }
    
    b2WeldJointDef weldJointDef;
    weldJointDef.referenceAngle = -90 * b2_pi / 180;
    weldJointDef.bodyA = mBodyA;
    weldJointDef.bodyB = mBodyB;
    weldJointDef.localAnchorA.Set(0, 0);
    weldJointDef.localAnchorB.Set(0, 0);
    weldJointDef.collideConnected = false;
    
    world->CreateJoint(&weldJointDef);
}

void GameScene::conveyorBelts()
{
    // Platform
    {
        b2BodyDef bd;
        bd.position.Set(visibleSize.width/2 / PTM_RATIO, 5.0f);
        b2Body* body = world->CreateBody(&bd);
        
        b2PolygonShape shape;
        shape.SetAsBox(10.0f, 0.5f);
        
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.friction = 10.0f;
        mPlatform = body->CreateFixture(&fd);
        
        ConveyorBelt cb;
        cb.fixture = mPlatform;
        cb.friction = fd.friction;
        cb.tangentSpeed = 100;
        listConveyorBelt.push_back(cb);
    }
    
    // Boxes
    for (int32 i = 0; i < 5; ++i)
    {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(visibleSize.width/2 / PTM_RATIO + 2.0f * i, 7.0f);
        b2Body* body = world->CreateBody(&bd);
        
        b2CircleShape shape;
        shape.m_radius = 0.5f;
        body->CreateFixture(&shape, 20.0f);
        body->SetAngularVelocity(-50);
    }
}

void GameScene::drawGrids()
{
    DrawNode* draw = DrawNode::create();
    this->addChild(draw, ZORDER_GRID);
    
    float lineSize = 0.5;
    Color4F color = Color4F(0/255, 205/255, 1, 0.2);
    
    // draw col
    for (int x = 0; x < visibleSize.width; x++) {
        float xPoint = x * TILE_SIZE;
        draw->drawSegment(Vec2(xPoint, 0), Vec2(xPoint, visibleSize.height), lineSize, color);
    }
    
    // draw row
    for (int y = 0;  y < visibleSize.height; y++) {
        float yPoint = y * TILE_SIZE;
        draw->drawSegment(Vec2(0, yPoint), Vec2(visibleSize.width, yPoint), lineSize, color);
    }
}

//void GameScene::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated) {
//    Layer::draw(renderer, transform, transformUpdated);
//    Director* director = Director::getInstance();
//    
//    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
//    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
//    GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
//    if (this->isScheduled(schedule_selector(GameScene::update))) {
//        this->update(0.0f);
//    }
//    world->DrawDebugData();
//    CHECK_GL_ERROR_DEBUG();
//    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//}

void GameScene::initPhysics()
{
    CCLOG("GameScene::initPhysics()");
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
    
    uint32  flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
//    flags += b2Draw::e_aabbBit;
//    flags += b2Draw::e_pairBit;
//    flags += b2Draw::e_centerOfMassBit;
    this->debugDraw->SetFlags(flags);
}

void GameScene::initMapLevel(int level)
{
    CCLOG("GameScene::initMapLevel()");
    //reading in a tiled map
    std::string nameLevel = "level" + to_string(level) + ".tmx";
    map = TMXTiledMap::create(nameLevel);
    if (map==nullptr) {
        
        CCLOG("file not found");
        
    } else {
        addChild(map, ZORDER_GAME::ZORDER_MAPLEVEL, 1);
        
        // auto create physics objects
        tiledmap = new TiledBodyCreator();
        tiledmap->initMapLevel(map, world, "braindots", CATEGORY_BARRAGE, MASK_BARRAGE);
        listConveyorBelt = tiledmap->getListConveyorBelt();
//        this->conveyorBelts();
        _ballContactListener->setListConveyorBelt(listConveyorBelt);
        
        // get ball group
        auto group = map->getObjectGroup("braindots");
        CCASSERT(NULL != group, "braindots group not found");
        
        // ball A
        auto ballA_map = group->getObject("ballA");
        CCASSERT(!ballA_map.empty(), "ball A not found");
        float xA = ballA_map["x"].asFloat();
        float yA = ballA_map["y"].asFloat();
        posballA = Vec2(xA, yA);
        
        // ball B
        auto ballB_map = group->getObject("ballB");
        CCASSERT(!ballB_map.empty(), "ball B not found");
        float xB = ballB_map["x"].asFloat();
        float yB = ballB_map["y"].asFloat();
        posballB = Vec2(xB, yB);
        
        // hex grid
        std::vector<Rect> listRectGrid;
        auto gridGroup = map->getObjectGroup("hexgridobjects");
        if (gridGroup==nullptr) {
//            CCLOG("hexgridobjects group not found");
        } else {
            listGirdLayer.clear();
            listRectGrid = tiledmap->getRectListObjects(map, "hexgridobjects", "hexgridlayer");
            if (listRectGrid.size() > 0) {
                for (int i = 0; i < listRectGrid.size(); i++) {
                    Rect rect = listRectGrid.at(i);
                    auto layer = LayerColor::create();
                    layer->setContentSize(Size(rect.size));
                    layer->setPosition(Vec2(rect.origin));
                    CCLOG("layer %f %f %f %f", layer->getPositionX(), layer->getPositionY(), layer->getContentSize().width, layer->getContentSize().height);
                    addChild(layer, 100);
                    listGirdLayer.pushBack(layer);
                    
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
        
        // draw node
        drawnode = DrawNode::create();
        map->addChild(drawnode);
    }
}

void GameScene::initPhysicObjects()
{
    CCLOG("GameScene::initPhysicObjects()");
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
    CCLOG("GameScene::initBalls()");
    auto ballASprite = Sprite::create("ball_red.png");
    map->addChild(ballASprite);
    ballASprite->setPosition(posballA);
    ballASprite->setTag(TAG_BALLA);
    
    auto ballBSprite = Sprite::create("ball_blue.png");
    map->addChild(ballBSprite);
    ballBSprite->setPosition(posballB);
    ballBSprite->setTag(TAG_BALLB);
    
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
    mBallDefA.fixedRotation = true;
    mBallDefA.position.Set(ballASprite->getPositionX() / PTM_RATIO, ballASprite->getPositionY()/ PTM_RATIO);
    mBallDefA.type = b2_staticBody;
    
    b2BodyDef mBallDefB;
    mBallDefB.fixedRotation = true;
    mBallDefB.position.Set(ballBSprite->getPositionX() / PTM_RATIO, ballBSprite->getPositionY()/ PTM_RATIO);
    mBallDefB.type = b2_staticBody;
    
    ballA = world->CreateBody(&mBallDefA);
    _ballAFixture = ballA->CreateFixture(&ballFixtureDef);
    ballA->SetUserData(ballASprite);
    
    ballB = world->CreateBody(&mBallDefB);
    _ballBFixture = ballB->CreateFixture(&ballFixtureDef);
    ballB->SetUserData(ballBSprite);

    _ballContactListener->setFixtureForBall(_ballAFixture, _ballBFixture);
}

void GameScene::update(float dt) {
    
    int positionIterations = 8;
    int velocityIterations = 1;
    if (!isSuccess && !isFail) {
        world->Step(dt, velocityIterations, positionIterations);
        
    } else {
        if (isSuccess) {
//            ParticleSystemQuad* starParticle = ParticleSystemQuad::create("star_particle.plist");
//            starParticle->setPosition(collisionPoint);
//            starParticle->setAutoRemoveOnFinish(true);
//            starParticle->retain();
//            map->addChild(starParticle);
            
            this->animationSuccess(collisionPoint);
            // run animation ring
//            this->explosionRing("explosion_yellow_ring.png", collisionPoint);
            
            this->runAction(Sequence::create(DelayTime::create(2.5f), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)),  NULL));
        } else if (isFail) {
            if (collisionFailA != Vec2::ZERO && collisionFailB == Vec2::ZERO) {
                this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailA, "explosion_red")), DelayTime::create(2), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)), NULL));
            }
            else if (collisionFailB != Vec2::ZERO && collisionFailA == Vec2::ZERO) {
                this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailB, "explosion_blue")), DelayTime::create(2), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)), NULL));
            }
            else if (collisionFailA != Vec2::ZERO  && collisionFailB != Vec2::ZERO) {
                this->runAction(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailA, "explosion_red")));
                this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailB, "explosion_blue")), DelayTime::create(2), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)), NULL));
            }
        }
        
        // disbale update
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
//        world->DrawDebugData();
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
            if((contact.fixtureA == _ballAFixture && contact.fixtureB == _wallFixture1[i]))
            {
                collisionFailA = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end()) {
                    toDestroy.push_back(bodyA);
                }
                isFail = true;
            }
            else if (contact.fixtureA == _ballBFixture && contact.fixtureB == _wallFixture1[i])
            {
                collisionFailB = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end()) {
                    toDestroy.push_back(bodyA);
                }
                isFail = true;
            }
            else if  ((contact.fixtureA == _wallFixture1[i] && contact.fixtureB == _ballAFixture))
            {
                collisionFailA = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end()) {
                    toDestroy.push_back(bodyB);
                }
                isFail = true;
            }
            else if  (contact.fixtureA == _wallFixture1[i] && contact.fixtureB == _ballBFixture)
            {
                collisionFailB = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end()) {
                    toDestroy.push_back(bodyB);
                }
                isFail = true;
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
                    isSuccess = true;
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
            if (sprite->getTag() == TAG_BALLA || sprite->getTag() == TAG_BALLB) {
                map->removeChild(sprite, true);
            }
            else {
                this->removeChild(sprite, true);
            }
            CCLOG("remove a sprite");
        }
        world->DestroyBody(body);
        CCLOG("remove a body");
    }
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    drawnode->clear();
    isErrorDraw = false;
    posErrorDraw = Vec2::ZERO;
    if (isSuccess || isFail) {
        return false;
    }
    // touch in grid layer
    if (listGirdLayer.size() > 0) {
        for (int i = 0; i<listGirdLayer.size(); i++) {
            if (listGirdLayer.at(i)->getBoundingBox().containsPoint(touch->getLocation())) {
                return false;
            }
        }
    }
    
    // touch in any physic body
    if (checkBodyWeighOnSomebody(touch->getLocation(), touch->getLocation()) != Vec2::ZERO)
//    if (checkDrawingWithOtherBodies(touch->getLocation(), touch->getLocation()) != Vec2::ZERO)
    {
        return false;
    }
    
    platformPoints.clear();
    Vec2 location = touch->getLocation();
    platformPoints.push_back(location);
    
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody;
    myBodyDef.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);
    currentPlatformBody = world->CreateBody(&myBodyDef);
    return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
    drawnode->clear();
    
    Vec2 start = touch->getPreviousLocation();
    Vec2 end = touch->getLocation();
    
//    Vec2 collision = checkBodyWeighOnSomebody(start, end);
    Vec2 collision = checkDrawingWithOtherBodies(start, end);
    
    // if before error draw
    if (isErrorDraw) {
        if (checkBodyWeighOnSomebody(start, start)!= Vec2::ZERO) {
            Color4F color = Color4F(1, 1, 0, 0.5);
            drawnode->drawSegment(start, posErrorDraw, brush->getContentSize().width/2 , Color4F(color));
        } else {
//            Vec2 _collision = checkBodyWeighOnSomebody(posErrorDraw, end);
            Vec2 _collision = checkDrawingWithOtherBodies(start, posErrorDraw);
            // if error draw
            if (_collision != Vec2::ZERO) {
                Color4F color = Color4F(1, 1, 0, 0.5);
                drawnode->drawSegment(start, posErrorDraw, brush->getContentSize().width/2 , Color4F(color));
            }
            else {
                // draw from pos error -> poos now
                isErrorDraw = false;
                start = posErrorDraw;
            }
        }
    }
    
    // if draw ok
    if (collision == Vec2::ZERO) {
        if (!isErrorDraw) {
            platformPoints.push_back(start);
        }
    }
    else {
        // draw error
        if (!isErrorDraw) {
            if (posErrorDraw != collision) {
                posErrorDraw = collision;
            }
        }
        isErrorDraw = true;
        end = collision;
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
            sprite->visit();
        }
        target->end();
    }
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
    drawnode->clear();
    
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
            Vec2 start = platformPoints.at(i);
            Vec2 end = platformPoints.at(i+1);
            this->addRectangleBetweenPointsToBody(newBody, start, end);
        }
        Rect bodyRectangle = ExecuteShapePhysic::getBodyRectangle(visibleSize, newBody);
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
        
        Director::getInstance()->getTextureCache()->removeTextureForKey(_key);
    }
    removeChild(target, true);
    //	target->release();
    target = RenderTexture::create(visibleSize.width, visibleSize.height,
                                   Texture2D::PixelFormat::RGBA8888);
    target->retain();
    target->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    addChild(target, ZORDER_GAME::ZORDER_DRAW_RENDER);
}

Vec2 GameScene::checkDrawingWithOtherBodies(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
    Vec2 result = Vec2::ZERO;
    
    if (start.distance(end) <= 0) {
        return  result;
    }
    // use ray cast check bodies inside start-end
    b2Vec2 point1(start.x / PTM_RATIO , start.y / PTM_RATIO);
    b2Vec2 point2(end.x / PTM_RATIO , end.y / PTM_RATIO);
    
    // Loại 1
    RayCastClosestCallback callback;
    world->RayCast(&callback, point1, point2);
    
    if (callback.m_hit) {
        result = Vec2(callback.m_point.x * PTM_RATIO, callback.m_point.y * PTM_RATIO);
        CCLOG("ray cast collision %f %f", result.x, result.y);
    }
    
    return result;
}

void GameScene::addRectangleBetweenPointsToBody(b2Body* body, Vec2 start,
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
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2, height / 2, b2Vec2(px, py), angle);
    
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 6.0;
    boxFixtureDef.friction = 0.1;
    boxFixtureDef.filter.categoryBits = CATEGORY_PLATFORM;
    boxFixtureDef.filter.maskBits = MASK_PLATFORM;
    
    body->CreateFixture(&boxFixtureDef);
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
    std::vector<Vec2> listPoints = getListPointsIn2Point(start, end);
    
    // collision with hex grid layer
    if (listGirdLayer.size() > 0) {
        for (int i = 0; i<listGirdLayer.size(); i++) {
            for (int j = 0; j < listPoints.size(); j++) {
                if (listGirdLayer.at(i)->getBoundingBox().containsPoint(listPoints.at(j))) {
                    result = listPoints.at(j);
                    CCLOG("You touched a hex grid layer");
                    return result;
                }
            }
        }
    }
    
    // collision with body physics
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        
        b2Fixture *f = body->GetFixtureList();
        while(f)
        {
            for (int i=0; i<listPoints.size(); i++) {
                b2Vec2 checkPoint = b2Vec2(listPoints.at(i).x / PTM_RATIO, listPoints.at(i).y / PTM_RATIO);
                if(f -> TestPoint(checkPoint))
                {
                    result = listPoints.at(i);
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
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
        world->DestroyBody(b);
    }
    this->removeChild(target, true);
    this->removeAllChildrenWithCleanup(true);
    
    auto fadeout = CallFunc::create(CC_CALLBACK_0(Node::setOpacity, this, 0));
    auto loading = CallFunc::create(CC_CALLBACK_0(SceneManager::loadingScene, SceneManager::getInstance(), this));
    auto change = CallFunc::create(CC_CALLBACK_0(SceneManager::changeState, SceneManager::getInstance(), GAME_STATE::MENU));
    
    this->runAction(Sequence::create(Spawn::create(DelayTime::create(TIME_LOADING), fadeout, loading, NULL), change, NULL));
    
//    SceneManager::getInstance()->changeState(GAME_STATE::MENU);
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
                for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
                    world->DestroyBody(b);
                }
                SceneManager::getInstance()->changeState(GAME_STATE::GAME);
                break;
            case TAG_GAME::TAG_BUTTON_NEXT:
                SceneManager::getInstance()->setLevelGame(SceneManager::getInstance()->getLevelGame()+1);
                
                // check level
                SceneManager::getInstance()->changeState(GAME_STATE::GAME);
                break;
        }
    }
}

void GameScene::explosionBall(b2Body *ball)
{
    float m_blastPower = 1000;
    
    srand((int)time(NULL));
    if (ball) {
        b2Vec2 center = ball->GetPosition();
        for (int i = 0; i < NUM_EXPLOSION_CIRCLE; i++) {
            float angle = CC_DEGREES_TO_RADIANS((i/(float)NUM_EXPLOSION_CIRCLE) * 360);
            b2Vec2 rayDir (sinf(angle), cosf(angle) );
            
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.fixedRotation = true;
            bd.bullet = true;
            bd.linearDamping = 10 + rand()%10;
            bd.gravityScale = 0;
            bd.position = center;
            bd.linearVelocity = 0.125f * m_blastPower * rayDir;
            b2Body* body = world->CreateBody(&bd);
            
            b2CircleShape circleShape;
            //            CCLOG("radius %f", (rand()%10)/100);
            circleShape.m_radius = 0.3 + (float)(rand()%10)/50;
            
            b2FixtureDef fd;
            fd.shape = &circleShape;
            fd.density = 60 / (float)NUM_EXPLOSION_CIRCLE;
            fd.friction = 0;
            fd.restitution = 0.99f;
            fd.filter.categoryBits = CATEGORY_EXPLOSION;
            fd.filter.maskBits = MASK_EXPLOSION;
            body->CreateFixture(&fd);
            
        }
    }
}

void GameScene::animationSuccess(Vec2 point)
{
	srand((int)time(0));

	for (int i = 0; i < NUM_EXPLOSION_CIRCLE; ++i) {
		int rand_angle = rand() % 20;
		float rand_scale = (float)(rand()%10)/10;
		int rand_add_space = rand()%100;
		float rand_delay = (float)(rand()%10)/100;

		float rad = CC_DEGREES_TO_RADIANS((i/(float)NUM_EXPLOSION_CIRCLE) * 360 + rand_angle);
		auto starSprite = Sprite::create("explosion_yellow.png");
		starSprite->setScale(0);
		starSprite->setPosition(point);

		// animation
		Vec2 dest = Vec2(point.x + BASE_EXPLOSION_SPACE * sinf(rad) + rand_add_space, point.y + BASE_EXPLOSION_SPACE * cosf(rad) + rand_add_space);
		auto move = MoveTo::create(1.0f, dest);
		auto zoomOut = ScaleTo::create(1.0f, rand_scale);
		auto rotate = RotateBy::create(1.3, rand() % 180);
		auto spawn1 = Spawn::create(move, zoomOut, rotate, NULL);
		auto delay = DelayTime::create(0.3 + rand_delay);
		auto fadeout = FadeOut::create(0.3f);
		auto zoomIn = ScaleTo::create(0.3, 0);
		auto spawn2 = Spawn::create( fadeout, zoomIn, NULL);

		auto sequence = Sequence::create(spawn1, delay, spawn2, RemoveSelf::create(), NULL);
        this->addChild(starSprite, ZORDER_GAME::ZORDER_ANIMATION);
		starSprite->runAction(sequence);
	}

	this->explosionRing("explosion_yellow_ring.png", point);
}

void GameScene::animationFail(cocos2d::Vec2 point, std::string explosionName)
{
    srand((int)time(0));
    
    for (int i = 0; i < NUM_EXPLOSION_CIRCLE; i ++) {
        int rand_angle = rand() % 20;
        float rand_scale = (float)(rand()%10)/10;
        int rand_add_space = rand()%100;
        float rand_delay = (float)(rand()%10)/100;
        
        float rad = CC_DEGREES_TO_RADIANS((i/(float)NUM_EXPLOSION_CIRCLE) * 360 + rand_angle);
        auto explosionSprite = Sprite::create(explosionName + ".png");
        explosionSprite->setScale(rand_scale);
        explosionSprite->setPosition(point);
        
        auto fadeout = FadeOut::create(0.7f);
        auto zoomOut = ScaleTo::create(0.7f, 0);
        auto spawn = Spawn::create(fadeout, zoomOut, NULL);
        
        Vec2 dest = Vec2(point.x + BASE_EXPLOSION_SPACE * sinf(rad) + rand_add_space, point.y + BASE_EXPLOSION_SPACE * cosf(rad) + rand_add_space);
        auto move = MoveTo::create(0.7f, dest);
        auto delay = DelayTime::create(0.2 + rand_delay);
        auto sequence = Sequence::create(delay, move, delay, spawn, RemoveSelf::create(), NULL);
        this->addChild(explosionSprite, ZORDER_GAME::ZORDER_ANIMATION);
        explosionSprite->runAction(sequence);
    }
    
    this->explosionRing(explosionName + "_ring.png", point);
}

void GameScene::explosionRing(std::string name, Vec2 point)
{
    auto ring = Sprite::create(name);
    ring->setScale(100 / ring->getContentSize().width);
    ring->setPosition(point);
    this->addChild(ring, ZORDER_GAME::ZORDER_ANIMATION);
    auto zoomOut = ScaleTo::create(1.4f, 3000/ring->getContentSize().width);
    auto fadeOut = FadeOut::create(1.4f);
    auto spawn = Spawn::create(zoomOut, fadeOut, NULL);
    ring->runAction(Sequence::create(spawn, RemoveSelf::create() , NULL));
}

void GameScene::showShareLayer()
{
    auto baseLayer = SharePopupLayer::create();
    this->addChild(baseLayer);
}

void GameScene::afterCaptured(bool succeed, const std::string &outputFile)
{
    //remove all body physics
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        if (body->GetUserData()) {
            Sprite *sprite = (Sprite *) body->GetUserData();
            map->removeChild(sprite, true);
        }
        world->DestroyBody(body);
    }
    map->removeAllChildrenWithCleanup(true);
    this->removeChild(map,true);

    Sprite* captureSprite;
    if (succeed) {
        CCLOG("capture screen game level success!");
        captureSprite = Sprite::create(filenameCapture);
    } else {
        CCLOG("capture screen game fail");
        captureSprite = Sprite::create("share_image.png");
    }

    // create paper sprite
    PaperSprite* paperSprite = PaperSprite::create("paper3.png", true);
    paperSprite->setPosition(visibleSize/2);
    paperSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(paperSprite);
    captureSprite->setPosition(paperSprite->getContentSize()/2);
    captureSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    paperSprite->addChild(captureSprite);
    
    // create paper sprite small to share
    PaperSprite* paperSmall = PaperSprite::create("paper8.png", true);
    paperSmall->setPosition(Vec2(paperSprite->getPositionX() + paperSprite->getContentSize().width/2, paperSprite->getPositionY() - paperSprite->getContentSize().height/2 - PADDING));
    paperSmall->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperSmall->setTag(TAG_GAME::TAG_PAPER_MINI);
    
    // image
    auto captureSpriteMini = Sprite::createWithTexture(captureSprite->getTexture());
    captureSpriteMini->setScale(0.25f);
    captureSpriteMini->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    captureSpriteMini->setPosition(Vec2(paperSmall->getContentSize().width/2, paperSmall->getContentSize().height-PADDING));
    paperSmall->addChild(captureSpriteMini);
    
    //text
    auto text = Label::createWithTTF("Share image", "arial.ttf", 40);
    text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    text->setColor(Color3B::BLUE);
    text->setPosition(Vec2(captureSpriteMini->getPositionX(), PADDING));
    paperSmall->addChild(text);
    
    this->addChild(paperSmall);
    
    // create tick
    std::string tickName;
    if (isFail) {
        tickName = "mini_fail.png";
    } else if (isSuccess) {
        tickName = "mini_tick.png";
    }
    
    auto tick = Sprite::create(tickName);
    tick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tick->setPosition(Vec2(paperSprite->getContentSize().width - tick->getContentSize().width, paperSprite->getContentSize().height - tick->getContentSize().height));
    tick->retain();
    tick->setScale(3.0f);
    auto addtick = CallFunc::create([paperSprite, tick] {
        paperSprite->addChild(tick);
        tick->runAction(ScaleTo::create(0.5f, 1.0f));
    });
    captureSprite->runAction(Sequence::create(ScaleTo::create(0.5, 0.5f), DelayTime::create(0.5f), addtick, nullptr));
    
    // add next button
    if (isSuccess) {
        auto nextButton = Button::create("next_button.png");
        nextButton->setScale(0.5f);
        nextButton->setPosition(Vec2(visibleSize.width - PADDING, PADDING));
        nextButton->setTag(TAG_GAME::TAG_BUTTON_NEXT);
        nextButton->setZOrder(ZORDER_GAME::ZORDER_BUTTON_NEXT);
        this->addChild(nextButton);
        nextButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        nextButton->setTouchEnabled(true);
        nextButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchButtonEvent, this));
    }
}

void GameScene::endGame()
{
    // capture screen
    if (isSuccess) {
        filenameCapture = "capture_level_" + to_string(SceneManager::getInstance()->getLevelGame()) + "_success.png";
        // open game level
        SceneManager::getInstance()->saveLevel(SceneManager::getInstance()->getLevelGame()+1);
    } else if (isFail) {
        filenameCapture = "capture_level_" + to_string(SceneManager::getInstance()->getLevelGame()) + "_fail.png";
    }
    
    // remove cache and memory
    Director::getInstance()->getTextureCache()->removeTextureForKey(filenameCapture);
    // remove file screen shoot
    if(FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath() + filenameCapture))
    {
        CCLOG("fileRemove %s removed", filenameCapture.c_str());
    }
    else {
        CCLOG("fileRemove %s not exist or error when removing", filenameCapture.c_str());
    }

    // capturing
    utils::captureScreen(CC_CALLBACK_2(GameScene::afterCaptured, this), filenameCapture);
    
}

void GameScene::onEnterTransitionDidFinish()
{
    this->runAction(ScaleTo::create(0.5, 1.0f));
}

#include "GameScene.h"

//////////////////////
// CONTRUCTOR
//////////////////////
GameScene::GameScene()
{
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
    drawnode = nullptr;
    brush = nullptr;
    map = nullptr;
    delete tiledmap;
    tiledmap = nullptr;
    
    delete _ballContactListener;
    _ballContactListener = nullptr;
    delete world;
    world = nullptr;
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}


///////////////////////////
// INIT ALL COMPONENT
///////////////////////////
bool GameScene::init()
{
    log("##GAMESCENE %s", __FUNCTION__);
    // init background white color
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    
    // get size and pos screen
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    // draw grid
    this->drawGrids();
    
    log("##GAMESCENE %s :button back and replay start", __FUNCTION__);
    // button back
    auto backButton = Button::create("game_btn_back.png", "", "", TextureResType::PLIST);
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(PADDING, visibleSize.height - PADDING));
    backButton->setTouchEnabled(true);
    backButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchButtonEvent, this));
    backButton->setTag(TAG_GAME::TAG_BUTTON_BACK);
    addChild(backButton, ZORDER_GAME::ZORDER_BUTTON_BACK);
    
    // button replay
    auto replayButton = Button::create("game_btn_replay.png", "", "", TextureResType::PLIST);
    replayButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    replayButton->setPosition(Vec2(visibleSize.width - PADDING, visibleSize.height - PADDING));
    replayButton->setTouchEnabled(true);
    replayButton->setTag(TAG_GAME::TAG_BUTTON_REPLAY);
    replayButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchButtonEvent, this));
    addChild(replayButton, ZORDER_GAME::ZORDER_BUTTON_REPLAY);
    
    log("##GAMESCENE %s :button back and replay done", __FUNCTION__);
    // load physics shapes cache
    GB2ShapeCache::getInstance()->addShapesWithFile("physicshapes.plist");
    
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
    this->addChild(target, ZORDER_GAME::ZORDER_DRAW_RENDER);
    
    // brush
    brush = Sprite::create("game_brush_25x25.png");
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


///////////////////////////
// DRAW GRIDS
///////////////////////////
void GameScene::drawGrids()
{
    DrawNode* draw = DrawNode::create();
    this->addChild(draw, ZORDER_GRID);
    
    float lineSize = 0.5;
    Color4F color = Color4F(0/255, 205/255, 1, 0.3);
    
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


///////////////////////////
// PHYSIC COMPONENTS
///////////////////////////
void GameScene::initPhysics()
{
    // create physics world box2d
    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    
    // Add the box2d debug draw layer
//    addChild(B2DebugDrawLayer::create(world, PTM_RATIO),INT_MAX);
    
    // add contact
    _ballContactListener = new BallContactListener();
    world->SetContactListener(_ballContactListener);
}

void GameScene::initMapLevel(int level)
{
    //reading in a tiled map
    std::string nameLevel = "level" + to_string(level) + ".tmx";
    map = TMXTiledMap::create(nameLevel);
    if (map!=nullptr) {
        this->addChild(map, ZORDER_GAME::ZORDER_MAPLEVEL);
        
        // auto create physics objects
        tiledmap->createMapLevel(map, world, "braindots");
        // for conveyor
        if (tiledmap->getListConveyorBelt().size() > 0) {
            listConveyorBelt = tiledmap->getListConveyorBelt();
            _ballContactListener->setListConveyorBelt(listConveyorBelt);
        }
        // for electricity
        if (tiledmap->getElectricityFixture()) {
            electricictyFixture = tiledmap->getElectricityFixture();
        }
        // for switch
        if (tiledmap->getSwitchFixture()) {
            switchFixture = tiledmap->getSwitchFixture();
        }
        
        // get ball group
        auto group = map->getObjectGroup("braindots");
        if (group != nullptr) {
            // ball A
            auto ballA_map = group->getObject("ballA");
            if (!ballA_map.empty()) {
                float xA = ballA_map["x"].asFloat();
                float yA = ballA_map["y"].asFloat();
                posballA = Vec2(xA, yA);
            }
            else log("##GAMESCENE:%s :ball A not found", __FUNCTION__);
            
            // ball B
            auto ballB_map = group->getObject("ballB");
            if(!ballB_map.empty()) {
                float xB = ballB_map["x"].asFloat();
                float yB = ballB_map["y"].asFloat();
                posballB = Vec2(xB, yB);
            } else log("##GAMESCENE:%s :ball B not found", __FUNCTION__);
        }
        else
            log("##GAMESCENE:%s :braindots group not found", __FUNCTION__);
        
        // draw node
        drawnode = DrawNode::create();
        map->addChild(drawnode, 200);
    }
}

void GameScene::initPhysicObjects()
{
    // Define fixture platform
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 6.0;
    boxFixtureDef.friction = 0.1;
    boxFixtureDef.filter.categoryBits = CATEGORY_PLATFORM;
    boxFixtureDef.filter.maskBits = MASK_PLATFORM;
    
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
    auto ballASprite = Sprite::createWithSpriteFrameName("game_ball_red.png");
    map->addChild(ballASprite);
    ballASprite->setPosition(posballA);
    ballASprite->setTag(TAG_BALLA);
    
    auto ballBSprite = Sprite::createWithSpriteFrameName("game_ball_blue.png");
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


/////////////////////////////
// TOUCH COMPONENTS
/////////////////////////////
bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    
    isErrorDraw = false;
    posErrorDraw = Vec2::ZERO;
    if (isSuccess || isFail) {
        return false;
    }
    
    if (drawnode!=nullptr) {
        drawnode->clear();
    }

    // touch in any physic body
    if (checkInsideBox2d(touch->getLocation(), touch->getLocation()) != Vec2::ZERO)
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
    Vec2 collision = checkInsideBox2dByRayCats(start, end);
    
    // if before error draw
    if (isErrorDraw) {
        if (checkInsideBox2d(start, start)!= Vec2::ZERO) {
            Color4F color = Color4F(1, 1, 0, 0.5);
            drawnode->drawSegment(start, posErrorDraw, brush->getContentSize().width/2 , Color4F(color));
        } else {
//            Vec2 _collision = checkBodyWeighOnSomebody(posErrorDraw, end);
            Vec2 _collision = checkInsideBox2dByRayCats(start, posErrorDraw);
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
        if (!isErrorDraw && start.getDistance(end) > 2) {
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
        float anchorX = newBody->GetPosition().x * PTM_RATIO - bodyRectangle.origin.x;
        float anchorY = bodyRectangle.size.height - (visibleSize.height - bodyRectangle.origin.y - newBody->GetPosition().y * PTM_RATIO);
        Vec2 anchorPoint = Vec2(anchorX / bodyRectangle.size.width, anchorY / bodyRectangle.size.height);
        
        // draw sprite use render texture
        auto _image = target->newImage();
        auto _key = to_string((int) time(NULL));
        auto _texture2D = Director::getInstance()->getTextureCache()->addImage(_image, _key);
        CC_SAFE_DELETE(_image);
        auto texture2D = Sprite::createWithTexture(_texture2D, bodyRectangle);
        texture2D->setAnchorPoint(anchorPoint);
        map->addChild(texture2D);
        newBody->SetUserData(texture2D);
        
        Director::getInstance()->getTextureCache()->removeTextureForKey(_key);
        log("%s",Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
    }
    target->clear(0, 0, 0, 0);
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
                this->removeAllObjects();
                SceneManager::getInstance()->changeState(GAME_STATE::GAME);
                break;
                
            case TAG_GAME::TAG_BUTTON_NEXT:
                this->removeAllObjects();
                SceneManager::getInstance()->setLevelGame(SceneManager::getInstance()->getLevelGame()+1);
                // check level
                SceneManager::getInstance()->changeState(GAME_STATE::GAME);
                break;
        }
    }
}


/////////////////////////////
// EXECUTE TOUCH AND BODY
/////////////////////////////
Vec2 GameScene::checkInsideBox2dByRayCats(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
    Vec2 result = Vec2::ZERO;
    
    if (start.distance(end) <= 0) {
        return  result;
    }
    // use ray cast check bodies inside start-end
    b2Vec2 point1(start.x / PTM_RATIO , start.y / PTM_RATIO);
    b2Vec2 point2(end.x / PTM_RATIO , end.y / PTM_RATIO);
    
    RayCastClosestCallback callback;
    world->RayCast(&callback, point1, point2);
    
    if (callback.m_hit) {
        result = Vec2(callback.m_point.x * PTM_RATIO, callback.m_point.y * PTM_RATIO);
        log("##GAMESCENE:%s :ray cast collision %f %f", __FUNCTION__, result.x, result.y);
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
        if (std::abs(dist_y) > minH) {
            dist_x = start.distance(end);
        }
    }
    float width = MAX(std::abs(dist_x) / PTM_RATIO, minW);
    
    boxShape.SetAsBox(width / 2, height / 2, b2Vec2(px, py), angle);
    
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

Vec2 GameScene::checkInsideBox2d(cocos2d::Vec2 start, cocos2d::Vec2 end)
{
    Vec2 result = Vec2::ZERO;
    std::vector<Vec2> listPoints = getListPointsIn2Point(start, end);
    
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
                    return result;
                }
            }
            f = f->GetNext();            
        }
    }
    return result;
}


///////////////////////////////
// GAME COMPONENTS
///////////////////////////////
void GameScene::backMenu() {
    this->removeAllObjects();
    
    auto fadeout = CallFunc::create(CC_CALLBACK_0(Node::setOpacity, this, 0));
    auto loading = CallFunc::create(CC_CALLBACK_0(SceneManager::loadingScene, SceneManager::getInstance(), this));
    auto change = CallFunc::create(CC_CALLBACK_0(SceneManager::changeState, SceneManager::getInstance(), GAME_STATE::MENU));
    
    this->runAction(Sequence::create(Spawn::create(DelayTime::create(TIME_LOADING), fadeout, loading, NULL), change, NULL));
}

void GameScene::endGame()
{
    log("##GAMESCENE %s", __FUNCTION__);
    // capture screen
    if (isSuccess) {
        filenameCapture = "capture_level_" + to_string(SceneManager::getInstance()->getLevelGame()) + "_success.png";
        // open game level
        SceneManager::getInstance()->saveLevel(SceneManager::getInstance()->getLevelGame()+1);
    } else if (isFail) {
        filenameCapture = "capture_level_" + to_string(SceneManager::getInstance()->getLevelGame()) + "_fail.png";
    }
    
    // remove cache and memory
//    Director::getInstance()->getTextureCache()->removeTextureForKey(filenameCapture);
    
    // capturing
    utils::captureScreen(CC_CALLBACK_2(GameScene::afterCaptured, this), filenameCapture);
    
}

void GameScene::removeAllObjects()
{
    log("##GAMESCENE %s begin", __FUNCTION__);
    this->stopAllActions();
    removeChild(target, true);
    target->release();
    target = nullptr;
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        world->DestroyBody(body);
    }
    
    for(long i = this->getChildrenCount()-1; i > 0; i--){
        Node* child = this->getChildren().at(i);
        this->removeChild(child);
    }
    
    log("##GAMESCENE %s end", __FUNCTION__);
}


///////////////////////////////
// ANIMATION GAMEOVER
///////////////////////////////
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
            circleShape.m_radius = 0.3 + (float)(rand()%10)/50;
            
            b2FixtureDef fd;
            fd.shape = &circleShape;
            fd.density = 60 / (float)NUM_EXPLOSION_CIRCLE;
            fd.friction = 0;
            fd.restitution = 0.99f;
            fd.filter.categoryBits = CATEGORY_EXPLOSION;
            fd.filter.maskBits = MASK_HEXGRID;
            body->CreateFixture(&fd);
            
        }
    }
}

void GameScene::animationSuccess(Vec2 point)
{
	srand((int)time(0));

	for (int i = 0; i < NUM_EXPLOSION_CIRCLE; ++i) {
		int rand_angle = rand() % 3;
		float rand_scale = (float)(rand()%10)/10;
		int rand_add_space = rand()%100;
		float rand_delay = (float)(rand()%10)/100;

		float rad = CC_DEGREES_TO_RADIANS((i/(float)NUM_EXPLOSION_CIRCLE + rand_angle) * 360);
		auto starSprite = Sprite::createWithSpriteFrameName("game_explosion_yellow.png");
		starSprite->setScale(0);
		starSprite->setPosition(point);

		// animation
		Vec2 dest = Vec2(point.x + BASE_EXPLOSION_SPACE * sinf(rad) + rand_add_space, point.y + BASE_EXPLOSION_SPACE * cosf(rad) + rand_add_space);
		auto move = MoveTo::create(0.7f, dest);
		auto zoomOut = ScaleTo::create(0.8f, rand_scale);
		auto rotate = RotateBy::create(1.0, 360);
		auto spawn1 = Spawn::create(move, zoomOut, rotate, NULL);
        
		auto delay = DelayTime::create(0.3 + rand_delay);
		auto fadeout = FadeOut::create(0.3f);
		auto zoomIn = ScaleTo::create(0.3, 0);
		auto spawn2 = Spawn::create( fadeout, zoomIn, NULL);

		auto sequence = Sequence::create(spawn1, delay, spawn2, RemoveSelf::create(), NULL);
        this->addChild(starSprite, ZORDER_GAME::ZORDER_ANIMATION);
		starSprite->runAction(sequence);
	}

	this->explosionRing("game_explosion_yellow_ring.png", point);
}

void GameScene::animationFail(cocos2d::Vec2 point, std::string explosionName)
{
    srand((int)time(0));
    
    for (int i = 0; i < NUM_EXPLOSION_CIRCLE; i ++) {
        int rand_angle = rand() % 20;
        float rand_scale = (float)(rand()%10)/10;
        int rand_add_space = rand()%100;
        float rand_delay = (float)(rand()%10)/100;
        
        float rad = CC_DEGREES_TO_RADIANS((i/(float)NUM_EXPLOSION_CIRCLE + rand_angle) * 360);
        auto explosionSprite = Sprite::createWithSpriteFrameName(explosionName + ".png");
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
    auto ring = Sprite::createWithSpriteFrameName(name);
    ring->setScale(100 / ring->getContentSize().width);
    ring->setPosition(point);
    this->addChild(ring, ZORDER_GAME::ZORDER_ANIMATION);
    auto zoomOut = ScaleTo::create(1.4f, 3000/ring->getContentSize().width);
    auto fadeOut = FadeOut::create(1.4f);
    auto spawn = Spawn::create(zoomOut, fadeOut, NULL);
    ring->runAction(Sequence::create(spawn, RemoveSelf::create() , NULL));
}


////////////////////////////////
// CAPTURE AND SHARE
////////////////////////////////
void GameScene::showShareLayer(std::string filename)
{
    auto baseLayer = SharePopupLayer::create(filename);
    this->addChild(baseLayer);
}

void GameScene::afterCaptured(bool succeed, const std::string &outputFile)
{
    log("##GAMESCENE %s", __FUNCTION__);
    
    ((Button*)this->getChildByTag(TAG_GAME::TAG_BUTTON_BACK))->setTouchEnabled(true);
    ((Button*)this->getChildByTag(TAG_GAME::TAG_BUTTON_REPLAY))->setTouchEnabled(true);
    
    //remove all body physics
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
        if (body->GetUserData() != NULL) {
            Node *sprite = (Node *) body->GetUserData();
            map->removeChild(sprite);
            log("##GAMESCENE:%s :remove done", __FUNCTION__);
        }
        world->DestroyBody(body);
    }
    log("##GAMESCENE %s remove body and sprite done", __FUNCTION__);
    
    map->removeAllChildrenWithCleanup(true);
    map->removeFromParentAndCleanup(true);

    log("##GAMESCENE %s remove done", __FUNCTION__);
    
    Sprite* captureSprite;
    if (succeed) {
        captureSprite = Sprite::create(filenameCapture);
    } else {
        captureSprite = Sprite::createWithSpriteFrameName("image_share.png");
    }

    // create paper sprite
    PaperSprite* paperSprite = PaperSprite::create("menu_img_share_700x457.png", true, true, true);
    paperSprite->setPosition(visibleSize/2);
    paperSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(paperSprite);
    Size sizePaper = paperSprite->getContentSize();
    // add sprite capture
    captureSprite->setPosition(sizePaper/2);
    captureSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    paperSprite->addChild(captureSprite);
    
    // create paper sprite small to share
    PaperSprite* paperSmall = PaperSprite::create("menu_img_share_350x228.png", true, true, true);
    paperSmall->setCallbackFunction(CC_CALLBACK_0(GameScene::showShareLayer, this, filenameCapture));
    paperSmall->setPosition(Vec2(paperSprite->getPositionX() + sizePaper.width/2, paperSprite->getPositionY() - sizePaper.height/2 - PADDING));
    paperSmall->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    paperSmall->setTag(TAG_GAME::TAG_PAPER_MINI);
    Size sizeSmallPaper = paperSmall->getContentSize();
    
    // image
    auto captureSpriteMini = Sprite::createWithTexture(captureSprite->getTexture());
    float scaleX = sizeSmallPaper.width * 0.9 / visibleSize.width;
    float scaleY = sizeSmallPaper.height * 0.9 / visibleSize.height;
    captureSpriteMini->setScale(scaleX, scaleY);
    captureSpriteMini->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    captureSpriteMini->setPosition(Vec2(sizeSmallPaper.width/2, sizeSmallPaper.height-PADDING));
    paperSmall->addChild(captureSpriteMini);
    
    //text
    auto text = Label::createWithTTF("Share image", "fonts/keifont.ttf", 40);
    text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    text->setColor(Color3B::BLUE);
    text->setPosition(Vec2(captureSpriteMini->getPositionX(), PADDING));
    paperSmall->addChild(text);
    
    this->addChild(paperSmall);
    
    // create tick
    std::string tickName;
    if (isFail) {
        tickName = "image_fail_64x64.png";
    } else if (isSuccess) {
        tickName = "image_success_64x64.png";
    }
    
    auto tick = Sprite::createWithSpriteFrameName(tickName);
    tick->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tick->setPosition(Vec2(sizePaper.width - tick->getContentSize().width, sizePaper.height - tick->getContentSize().height));
    tick->retain();
    tick->setScale(3.0f);
    auto addtick = CallFunc::create([paperSprite, tick] {
        paperSprite->addChild(tick);
        tick->runAction(ScaleTo::create(0.5f, 1.0f));
    });
    captureSprite->runAction(Sequence::create(ScaleTo::create(0.5f, sizePaper.width*0.9/visibleSize.width), DelayTime::create(0.2f), addtick, nullptr));
    
    // show text success or fail
    auto titleText = Text::create("", "fonts/keifont.ttf", 80);
    titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    titleText->setPosition(Vec2(visibleSize.width/2, this->getChildByTag(TAG_GAME::TAG_BUTTON_BACK)->getPositionY()));
    this->addChild(titleText);
    
    if (isSuccess) {
        // add next button
        auto nextButton = Button::create("game_btn_next.png", "", "", TextureResType::PLIST);
        nextButton->setScale(0.5f);
        nextButton->setPosition(Vec2(visibleSize.width - PADDING, PADDING));
        nextButton->setTag(TAG_GAME::TAG_BUTTON_NEXT);
        nextButton->setZOrder(ZORDER_GAME::ZORDER_BUTTON_NEXT);
        this->addChild(nextButton);
        nextButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        nextButton->setTouchEnabled(true);
        nextButton->addTouchEventListener(CC_CALLBACK_2(GameScene::touchButtonEvent, this));
        
        // set string text
        titleText->setString("Stage Clear!");
        titleText->setColor(Color3B(26, 217, 244));
    } else {
        // set string text
        titleText->setString("Game Over!");
        titleText->setColor(Color3B(240,68,94));
    }
    
    log("##GAMESCENE:%s :done", __FUNCTION__);
}


////////////////////////
// UPADTE GAME
////////////////////////
void GameScene::update(float dt) {
    
    int positionIterations = 8;
    int velocityIterations = 1;
    if (!isSuccess && !isFail) {
        world->Step(dt, velocityIterations, positionIterations);
        
    } else {
        ((Button*)this->getChildByTag(TAG_GAME::TAG_BUTTON_BACK))->setTouchEnabled(false);
        ((Button*)this->getChildByTag(TAG_GAME::TAG_BUTTON_REPLAY))->setTouchEnabled(false);
        
        // result success
        if (isSuccess) {
            // animation success
            this->animationSuccess(collisionPoint);
            this->runAction(Sequence::create(DelayTime::create(TIME_DELAY_GAMEOVER), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)),  NULL));
            
        } else if (isFail) {
            // ballA was broken
            if (collisionFailA != Vec2::ZERO && collisionFailB == Vec2::ZERO) {
                this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailA, "game_explosion_red")), DelayTime::create(TIME_DELAY_GAMEOVER), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)), NULL));
            }
            // ball B was broken
            else if (collisionFailB != Vec2::ZERO && collisionFailA == Vec2::ZERO) {
                this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailB, "game_explosion_blue")), DelayTime::create(TIME_DELAY_GAMEOVER), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)), NULL));
            }
            // ball A and B together were broken
            else if (collisionFailA != Vec2::ZERO  && collisionFailB != Vec2::ZERO) {
                this->runAction(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailA, "game_explosion_red")));
                this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(GameScene::animationFail, this, collisionFailB, "game_explosion_blue")), DelayTime::create(TIME_DELAY_GAMEOVER), CallFunc::create( CC_CALLBACK_0(GameScene::endGame, this)), NULL));
            }
        }
        
        // disable update
        this->unschedule(schedule_selector(GameScene::update));
        return;
    }
    // update sprite follow body
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
    std::vector<b2Body *>toRemoveElectric;
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
        
        // contact with electricity
        if (contact.fixtureA == electricictyFixture && contact.fixtureB == _ballAFixture) {
            collisionFailA = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
            if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end()) {
                toDestroy.push_back(bodyB);
            }
            isFail = true;
        }
        else if (contact.fixtureA == electricictyFixture && contact.fixtureB == _ballBFixture) {
            collisionFailB = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
            if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end()) {
                toDestroy.push_back(bodyB);
            }
            isFail = true;
        }
        else if (contact.fixtureA == _ballAFixture && contact.fixtureB ==  electricictyFixture) {
            collisionFailA = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
            if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end()) {
                toDestroy.push_back(bodyA);
            }
            isFail = true;
        }
        else if (contact.fixtureA == _ballBFixture && contact.fixtureB ==  electricictyFixture) {
            collisionFailB = Vec2(contact.collisionPoint.x * PTM_RATIO, contact.collisionPoint.y * PTM_RATIO);
            if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end()) {
                toDestroy.push_back(bodyA);
            }
            isFail = true;
        }
        
        // collision switch
        if (contact.fixtureA == switchFixture && (contact.fixtureB == _ballAFixture || contact.fixtureB == _ballBFixture))
        {
            if (bodyA->GetUserData() != NULL)
            {
                if (std::find(toRemoveElectric.begin(), toRemoveElectric.end(), bodyA) == toRemoveElectric.end()) {
                    toRemoveElectric.push_back(bodyA);
                }
                if (electricictyFixture!=NULL) {
                    if (std::find(toDestroy.begin(), toDestroy.end(), electricictyFixture->GetBody()) == toDestroy.end()) {
                        toDestroy.push_back(electricictyFixture->GetBody());
                        log("##GAMESCENE:%s :remove electricity", __FUNCTION__);
                    }
                }
            }
        }
        else if (contact.fixtureB == switchFixture && (contact.fixtureA == _ballAFixture || contact.fixtureA == _ballBFixture))
        {
            if (bodyB->GetUserData() != NULL)
            {
                if (std::find(toRemoveElectric.begin(), toRemoveElectric.end(), bodyB) == toRemoveElectric.end()) {
                    toRemoveElectric.push_back(bodyB);
                }
                if (electricictyFixture!=NULL) {
                    if (std::find(toDestroy.begin(), toDestroy.end(), electricictyFixture->GetBody()) == toDestroy.end()) {
                        toDestroy.push_back(electricictyFixture->GetBody());
                        log("##GAMESCENE:%s :remove electricity", __FUNCTION__);
                    }
                }
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
    
    // move switch
    std::vector<b2Body *>::iterator pos4;
    for (pos4 = toRemoveElectric.begin(); pos4 != toRemoveElectric.end(); ++pos4) {
        b2Body* body = *pos4;
        if (body->GetUserData() != NULL) {
            SwitchObject* sprite = (SwitchObject*) body->GetUserData();
            if (!sprite->getTurnOff()) {
                sprite->moveDown(sprite->getContentSize().height);
                sprite->setTurnOff(true);
                
                ParticleSystemQuad* firework = ParticleSystemQuad::create("firework.plist");
                firework->setPosition(Vec2(electricictyFixture->GetBody()->GetPosition().x * PTM_RATIO, electricictyFixture->GetBody()->GetPosition().y * PTM_RATIO));
                firework->setAutoRemoveOnFinish(true);
                map->addChild(firework);
                electricictyFixture = NULL;
            }
        }
    }
    
    // stop 2 ball when collision
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
            auto sprite = (Node *) body->GetUserData();
            map->removeChild(sprite, true);
        }
        world->DestroyBody(body);
    }
    toDestroy.clear();
}


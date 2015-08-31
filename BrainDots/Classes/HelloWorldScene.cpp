#include "HelloWorldScene.h"
#include <sstream>

USING_NS_CC;
#define PTM_RATIO 32.0 // 32px = 1m in Box2D

enum TAG
{
    ballA = 1,
    ballB,
};

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}
HelloWorld::HelloWorld() :
		world(nullptr), currentPlatformBody(nullptr), target(nullptr), brush(
				nullptr), _brushs(), m_bClearBox(false), _ballContactListener(nullptr) {
}

HelloWorld::~HelloWorld() {
	delete world;
	world = NULL;
	CC_SAFE_RELEASE(target);
	std::vector<Vec2>().swap(platformPoints);
    delete _ballContactListener;
    delete this->debugDraw;
    this->debugDraw = NULL;
}

Scene* HelloWorld::createScene() {
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
bool HelloWorld::init() {
    
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
    CCLOG("size screen %f %f", visibleSize.width, visibleSize.height);
	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png",
			"CloseSelected.png", CC_CALLBACK_1(HelloWorld::clearScreen, this));

	closeItem->setPosition(
			Vec2(
					origin.x + visibleSize.width
							- closeItem->getContentSize().width / 2,
					origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
    
    // draw node
    drawnode = DrawNode::create();
    addChild(drawnode);
    
    //reading in a tiled map
    auto map = TMXTiledMap::create("test.tmx");
    addChild(map, 0, 99);
    
    // get ball group
    auto ballGroup = map->getObjectGroup("ball");
    CCASSERT(NULL != ballGroup, "Ball group not found");
    
    auto ballA_map = ballGroup->getObject("ballA");
    CCASSERT(!ballA_map.empty(), "ball A not found");
    float xA = ballA_map["x"].asFloat();
    float yA = ballA_map["y"].asFloat();
    posballA = Vec2(xA, yA);
    CCLOG("%f %f",xA,yA);
    
    auto ballB_map = ballGroup->getObject("ballB");
    CCASSERT(!ballB_map.empty(), "ball B not found");
    float xB = ballB_map["x"].asFloat();
    float yB = ballB_map["y"].asFloat();
    posballB = Vec2(xB, yB);
    CCLOG("%f %f",xB,yB);
    
    // get group ground
    auto groundGroup = map->getObjectGroup("ground");
    CCASSERT(groundGroup!=NULL, "Ground group not found");
    auto groundDown = groundGroup->getObject("groundDown");
    CCLOG("%s",groundDown["type"].asString().c_str());
    
    auto position = Vec2(groundDown["x"].asFloat() / PTM_RATIO, groundDown["y"].asFloat() / PTM_RATIO);
    auto pointsVector = groundDown["points"].asValueVector();
    if(pointsVector.size() > b2_maxPolygonVertices) {
        CCLOG("Skipping TMX polygon at x=%d,y=%d for exceeding %d vertices", groundDown["x"].asInt(), groundDown["y"].asInt(), b2_maxPolygonVertices);
        return NULL;
    }
    
    for(Value point : pointsVector) {
        vertices[vindex].x = (point.asValueMap()["x"].asFloat() / PTM_RATIO + position.x);
        vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / PTM_RATIO + position.y);
        vindex++;
    }
    
	// init physics
	this->initPhysics();
    this->initBalls();
    
	target = RenderTexture::create(visibleSize.width, visibleSize.height,
			Texture2D::PixelFormat::RGBA8888);
	target->retain();
	target->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(target);

	brush = CCSprite::create("brush.png");
	brush->retain();
    this->schedule(schedule_selector(HelloWorld::update));
    
    // add touch
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void HelloWorld::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformUpdated) {
    Layer::draw(renderer, transform, transformUpdated);
    Director* director = Director::getInstance();
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    world->DrawDebugData();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void HelloWorld::initPhysics() {
	b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
    world->SetAutoClearForces(true);
    
    // add debug draw
    this->debugDraw = new GLESDebugDraw( PTM_RATIO );
    this->world->SetDebugDraw(debugDraw);
    
    // add contact
    _ballContactListener = new BallContactListener();
    world->SetContactListener(_ballContactListener);
    
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
    this->debugDraw->SetFlags(flags);
    
	//Define the ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

	// create body
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	//Define the ground box shape
	b2EdgeShape groundBox;
    b2PolygonShape groundShape;

	//ground
//	groundBox.Set(b2Vec2(0, 0), b2Vec2(visibleSize.width / PTM_RATIO, 0));
//	groundBody->CreateFixture(&groundBox, 0);
    groundShape.Set(vertices, vindex);
    groundBody->CreateFixture(&groundShape, 0);

	// top
	groundBox.Set(b2Vec2(0, visibleSize.height / PTM_RATIO),
			b2Vec2(visibleSize.width / PTM_RATIO,
					visibleSize.height / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// left
//	groundBox.Set(b2Vec2(0, visibleSize.height / PTM_RATIO), b2Vec2(0, 0));
//	groundBody->CreateFixture(&groundBox, 0);

	// right
//	groundBox.Set(
//			b2Vec2(visibleSize.width / PTM_RATIO,
//					visibleSize.height / PTM_RATIO),
//			b2Vec2(visibleSize.width / PTM_RATIO, 0));
	groundBody->CreateFixture(&groundBox, 0);

}

void HelloWorld::initBalls()
{
    auto ballASprite = Sprite::create("ball_red.png");
    addChild(ballASprite);
//    ballASprite->setPosition(Vec2(visibleSize.width/4, visibleSize.height/2));
    ballASprite->setPosition(posballA);
    ballASprite->setTag(TAG::ballA);
    
    auto ballBSprite = Sprite::create("ball_blue.png");
    addChild(ballBSprite);
//    ballBSprite->setPosition(Vec2(visibleSize.width*3/4, visibleSize.height/2));
    ballBSprite->setPosition(posballB);
    ballBSprite->setTag(TAG::ballB);
    
    
    // shape of body
    b2CircleShape circleShape;
    circleShape.m_radius = 25/PTM_RATIO;
    
    // fixturedef
    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &circleShape;
    ballFixtureDef.density = 30.0f;
    
    // body definition
    b2BodyDef mBallDefA;
    mBallDefA.position.Set(ballASprite->getPositionX() / PTM_RATIO, ballASprite->getPositionY()/ PTM_RATIO);
    mBallDefA.type = b2_staticBody;
    
    b2BodyDef mBallDefB;
    mBallDefB.position.Set(ballBSprite->getPositionX() / PTM_RATIO, ballBSprite->getPositionY()/ PTM_RATIO);
    mBallDefB.type = b2_staticBody;
    
    ballA = world->CreateBody(&mBallDefA);
    ballA->CreateFixture(&ballFixtureDef);
    ballA->SetUserData(ballASprite);
//    ballA->SetGravityScale(10);
    
    ballB = world->CreateBody(&mBallDefB);
    ballB->CreateFixture(&ballFixtureDef);
    ballB->SetUserData(ballBSprite);
//    ballB->SetGravityScale(10);

}

void HelloWorld::update(float dt) {

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

        this->runAction(Sequence::create(call1, DelayTime::create(1.0f), call2,  NULL));

        
        this->unschedule(schedule_selector(HelloWorld::update));
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
//    for (b2Contact* contact = world->GetContactList(); contact; contact = contact->GetNext()) {
//        b2Fixture* a = contact->GetFixtureA();
//        b2Fixture* b = contact->GetFixtureB();
//        auto bodyA = a->GetBody();
//        auto bodyB = b->GetBody();
//        
//        if (bodyA && bodyB) {
//            if ((bodyA == ballA && bodyB == ballB) || (bodyA == ballB && bodyB == ballA)) {
//                if (bodyA->GetType() == b2_dynamicBody) {
//                    bodyA->SetType(b2_staticBody);
//                }
//                
//                if (bodyB->GetType() == b2_dynamicBody) {
//                    bodyB->SetType(b2_staticBody);
//                }
//                gameOver = true;
//            }
//        }
//    }
    
    std::vector<b2Body *>toStatic;
    std::vector<BallContact>::iterator pos;
    for (pos = _ballContactListener->_contacts.begin(); pos != _ballContactListener->_contacts.end(); ++pos)
    {
        BallContact contact = *pos;
        b2Body* bodyA = contact.fixtureA->GetBody();
        b2Body* bodyB = contact.fixtureB->GetBody();
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

	world->ClearForces();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
    drawnode->clear();
	int r = rand() % 128 + 128;
	int b = rand() % 128 + 128;
	int g = rand() % 128 + 128;
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

void HelloWorld::onTouchMoved(Touch* touch, Event* event) {

    Vec2 start = touch->getLocation();
    Vec2 end = touch->getPreviousLocation();

	target->begin();

	float distance = start.getDistance(end);

//    for (int i = 0; i < distance; i++) {
//        float difX = end.x - start.x;
//        float difY = end.y - start.y;
//        float delta = (float) i / distance;
//        brush->setPosition(Vec2(start.x + (difX * delta), start.y + (difY * delta)));
//        brush->visit();
//    }
	_brushs.clear();
	for (int i = 0; i < distance; ++i) {
		Sprite * sprite = Sprite::createWithTexture(brush->getTexture());
		_brushs.push_back(sprite);
	}
	for (int i = 0; i < distance; i++) {
		float difx = end.x - start.x;
		float dify = end.y - start.y;
		float delta = (float) i / distance;
		_brushs.at(i)->setPosition(
				Vec2(start.x + (difx * delta), start.y + (dify * delta)));
		_brushs.at(i)->visit();
	}
	target->end();
//    addRectangleBetweenPointsToBody(currentPlatformBody, start, end);
	platformPoints.push_back(start);
	previousLocation = start;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {

    if (ballA && ballB) {
        if (ballA->GetType() == b2_staticBody) {
            ballA->SetType(b2_dynamicBody);
        }
        if (ballB->GetType() == b2_staticBody) {
            ballB->SetType(b2_dynamicBody);
        }
    }
	if (platformPoints.size() > 1) {
        CCLOG("size of platformPoints %zd", platformPoints.size());
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
		auto _image = target->newImage();
		auto _key = to_string((int) time(NULL));
		auto _texture2D = Director::getInstance()->getTextureCache()->addImage(
				_image, _key);
		CC_SAFE_DELETE(_image);
        
		auto texture2D = Sprite::createWithTexture(_texture2D, bodyRectangle);
		texture2D->setAnchorPoint(anchorPoint);
		addChild(texture2D);
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

void HelloWorld::addRectangleBetweenPointsToBody(b2Body* body, Vec2 start,
		Vec2 end) {
//    CCLOG("start(%f,%f) end(%f,%f)", start.x, start.y, end.x, end.y);
    drawnode->drawDot(start, 1, Color4F::RED);
    drawnode->drawDot(end, 1, Color4F::GREEN);
    
//	float min = brush->getContentSize().width * brush->getScale() / PTM_RATIO;
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
        CCLOG("dist_x = %f", dist_x);
    } else {
        if (abs(dist_y) > minH) {
            dist_x = start.distance(end);
        }
    }
	float width = MAX(abs(dist_x) / PTM_RATIO, minW);
//	float height = MAX(abs(dist_y) / PTM_RATIO, minH);
    
    CCLOG("width=%f height=%f", width* PTM_RATIO, height* PTM_RATIO);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2, height / 2, b2Vec2(px, py), angle);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 5;

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

void HelloWorld::clearScreen(cocos2d::Ref *pSender) {
	this->m_bClearBox = true;
    if (m_bClearBox) {
        this->removeChild(target, true);
        _brushs.clear();
        this->removeAllChildren();
        m_bClearBox = false;
        
        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
            if (b->GetUserData() != NULL) {
                world->DestroyBody(b);
            }
        }
    }
    auto scene = HelloWorld::createScene();
    auto changeScene = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(changeScene);
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

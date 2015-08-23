#include "HelloWorldScene.h"

USING_NS_CC;
#define SCALE_RATIO 32.0

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
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(
			Vec2(
					origin.x + visibleSize.width
							- closeItem->getContentSize().width / 2,
					origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// init physics
	this->initPhysics();

	target = RenderTexture::create(visibleSize.width, visibleSize.height,
			Texture2D::PixelFormat::RGBA8888);
	target->retain();
	target->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(target);

	brush = CCSprite::create("largeBrush.png");
	brush->retain();
	scheduleUpdate();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void HelloWorld::initPhysics() {
	b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;

	addWall(visibleSize.width, 10, (visibleSize.width / 2), 0); // Floor
	addWall(10, visibleSize.height, 0, (visibleSize.height / 2)); //Left
	addWall(10, visibleSize.height, visibleSize.width,
			(visibleSize.height / 2)); // Right
//	bodyShape.m_radius = 45 / SCALE_RATIO;
//
//	//fixtureDef
//	fixtureDef.density = 10;
//	fixtureDef.friction = 0.8;
//	fixtureDef.restitution = 0.6;
//	fixtureDef.shape = &bodyShape;
//
//	//bodyDef
//	bodyDef.type = b2_dynamicBody;
//	bodyDef.userData = ball;
//	bodyDef.position.Set(brush->getPosition().x / SCALE_RATIO, brush->getPosition().y / SCALE_RATIO);
//
//	//ballBody
//	currentPlatformBody = world->CreateBody(&bodyDef);
//	currentPlatformBody->CreateFixture(&fixtureDef);
//	currentPlatformBody->SetGravityScale(10);
}

void HelloWorld::addWall(float w, float h, float px, float py) {

	b2PolygonShape floorShape;

	floorShape.SetAsBox(w / SCALE_RATIO, h / SCALE_RATIO);
	b2FixtureDef floorFixture;

	floorFixture.density = 0;
	floorFixture.friction = 10;
	floorFixture.restitution = 0.5;
	floorFixture.shape = &floorShape;

	b2BodyDef floorBodyDef;
	floorBodyDef.position.Set(px / SCALE_RATIO, py / SCALE_RATIO);

	b2Body *floorBody = world->CreateBody(&floorBodyDef);
	floorBody->CreateFixture(&floorFixture);

}

void HelloWorld::update(float dt) {
	int positionIterations = 8;
	int velocityIterations = 1;

	world->Step(dt, velocityIterations, positionIterations);
	for (b2Body *body = world->GetBodyList(); body != NULL; body =
			body->GetNext()) {
		if (body->GetUserData()) {
			Sprite *sprite = (Sprite *) body->GetUserData();
			sprite->setPosition(
					Point(body->GetPosition().x * SCALE_RATIO,
							body->GetPosition().y * SCALE_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}
	}
	world->ClearForces();
	world->DrawDebugData();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
	platformPoints.clear();
	Vec2 location = touch->getLocationInView();
	location = Director::getInstance()->convertToGL(location);
	platformPoints.push_back(location);
	previousLocation = location;
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(location.x / SCALE_RATIO, location.y / SCALE_RATIO);
	currentPlatformBody = world->CreateBody(&myBodyDef);
	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event) {
	Vec2 start = touch->getLocationInView();
	start = Director::getInstance()->convertToGL(start);
	Vec2 end = touch->getPreviousLocationInView();
	end = Director::getInstance()->convertToGL(end);

	target->begin();

	float distance = start.getDistance(end);

	for (int i = 0; i < distance; i++) {
		float difx = end.x - start.x;
		float dify = end.y - start.y;
		float delta = (float) i / distance;
		brush->setPosition(
				Vec2(start.x + (difx * delta), start.y + (dify * delta)));

		//brush->setOpacity(0.1);
		brush->visit();
	}
	target->end();

	addRectangleBetweenPointsToBody(currentPlatformBody, previousLocation,
			start);
	platformPoints.push_back(start);
	previousLocation = start;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
	if (platformPoints.size() > 1) {
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
		myBodyDef.position.Set(currentPlatformBody->GetPosition().x,
				currentPlatformBody->GetPosition().y); //set the starting position
		b2Body* newBody = world->CreateBody(&myBodyDef);

		for (int i = 0; i < platformPoints.size() - 1; i++) {
			Vec2 start = platformPoints[i];
			Vec2 end = platformPoints[i + 1];
			this->addRectangleBetweenPointsToBody(newBody, start, end);
		}

		world->DestroyBody(currentPlatformBody);
		Rect bodyRectangle = getBodyRectangle(newBody);

//	Image *pImage = target->newImage();
//	Texture2D *tex = TextureCache::sharedTextureCache()->addImage(pImage,
//			NULL);
//	CC_SAFE_DELETE(pImage);

		//CCSprite *sprite = CCSprite::create(tex, bodyRectangle);

		Sprite *sprite = Sprite::createWithTexture(brush->getTexture(),
				bodyRectangle, false);
		float anchorX = newBody->GetPosition().x * SCALE_RATIO
				- bodyRectangle.origin.x;
		float anchorY = bodyRectangle.size.height
				- (visibleSize.height - bodyRectangle.origin.y
						- newBody->GetPosition().y * SCALE_RATIO);

		sprite->setAnchorPoint(
				Vec2(anchorX / bodyRectangle.size.width,
						anchorY / bodyRectangle.size.height));
		newBody->SetUserData(sprite);
		addChild(sprite);

		removeChild(target, true);
		target->release();

		target = RenderTexture::create(visibleSize.width, visibleSize.height,
				Texture2D::PixelFormat::RGBA8888);
		target->retain();
		target->setPosition(
				Vec2(visibleSize.width / 2, visibleSize.height / 2));

		this->addChild(target, 5);
	}
}

void HelloWorld::addRectangleBetweenPointsToBody(b2Body* body, Vec2 start,
		Vec2 end) {
	float min = brush->getContentSize().width*brush->getScale()/SCALE_RATIO;
	float distance = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));

	float sx = start.x;
	float sy = start.y;
	float ex = end.x;
	float ey = end.y;
	float dist_x = sx - ex;
	float dist_y = sy - ey;
	float angle = atan2(dist_y, dist_x);

	float px = (sx + ex) / 2 / SCALE_RATIO - body->GetPosition().x;
	float py = (sy + ey) / 2 / SCALE_RATIO - body->GetPosition().y;

//	float width = abs(distance) / SCALE_RATIO;
//	float height = brush->boundingBox().size.height / SCALE_RATIO;
	float width = MAX(abs(dist_x) / SCALE_RATIO, min);
	float height = MAX(abs(dist_y) / SCALE_RATIO, min);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2, height / 2, b2Vec2(px, py), angle);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 5;

	boxFixtureDef.filter.categoryBits = 2;

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

	maxX2 *= SCALE_RATIO;
	minX2 *= SCALE_RATIO;
	maxY2 *= SCALE_RATIO;
	minY2 *= SCALE_RATIO;

	float width2 = maxX2 - minX2;
	float height2 = maxY2 - minY2;

	float remY2 = visibleSize.height - maxY2;

	return Rect(minX2, remY2, width2, height2);
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

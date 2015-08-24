#include "HelloWorldScene.h"

USING_NS_CC;
#define PTM_RATIO 32.0

HelloWorld::HelloWorld(): world(nullptr), currentPlatformBody(nullptr), target(nullptr)
{}

HelloWorld::~HelloWorld() {
    delete world;
    world = NULL;
    CC_SAFE_RELEASE(target);
    std::vector<Vec2>().swap(platformPoints);
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
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}
    m_bClearBox = false;
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(HelloWorld::clearScreen, this));

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
    
    //Define the ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    // create body
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    
    //Define the ground box shape
    b2EdgeShape groundBox;
    
    //ground
    groundBox.Set(b2Vec2(0,0), b2Vec2(visibleSize.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    
    // top
    groundBox.Set(b2Vec2(0,visibleSize.height/PTM_RATIO), b2Vec2(visibleSize.width/PTM_RATIO,visibleSize.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    
    // left
    groundBox.Set(b2Vec2(0,visibleSize.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);
    
    // right
    groundBox.Set(b2Vec2(visibleSize.width/PTM_RATIO,visibleSize.height/PTM_RATIO), b2Vec2(visibleSize.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

}


void HelloWorld::update(float dt) {
    
    if (m_bClearBox) {
        // ç”»é�¢ã‚¯ãƒªã‚¢å‡¦ç�†
        this->removeChild(target, true);
        target->release();
        
        this->removeAllChildren();
        m_bClearBox = false;
        
        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        {
            if (b->GetUserData() != NULL) {
                world->DestroyBody(b);
            }
        }
        
        target = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
        target->retain();
        target->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(target);
        
        return;
    }
    
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
	world->ClearForces();
	world->DrawDebugData();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
    int r = rand()%128+128;
    int b = rand()%128+128;
    int g = rand()%128+128;
    brush->setColor(Color3B(r, b, g));
    
    platformPoints.clear();
    Vec2 location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
    platformPoints.push_back(location);
    previousLocation = location;
    
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody;
    myBodyDef.position.Set(location.x/PTM_RATIO,location.y/PTM_RATIO);
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
    CCLOG("distance %f",distance);
    
//    for (int i = 0; i < distance; i++) {
//        float difX = end.x - start.x;
//        float difY = end.y - start.y;
//        float delta = (float) i / distance;
//        brush->setPosition(Vec2(start.x + (difX * delta), start.y + (difY * delta)));
//        brush->visit();
//    }
    _brushs.clear();
    for(int i = 0; i < distance; ++i)
    {
        Sprite * sprite = Sprite::create("largeBrush.png");
        _brushs.push_back(sprite);
    }
    for (int i = 0; i < distance; i++)
    {
        float difx = end.x - start.x;
        float dify = end.y - start.y;
        float delta = (float)i / distance;
        _brushs.at(i)->setPosition(Vec2(start.x + (difx * delta), start.y + (dify * delta)));
        _brushs.at(i)->setRotation(rand() % 360);
        _brushs.at(i)->visit();
    }
    target->end();
    
    platformPoints.push_back(start);
    previousLocation = start;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
    
    if (platformPoints.size()>1) {
        //Add a new body/atlas sprite at the touched location
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
        myBodyDef.position.Set(currentPlatformBody->GetPosition().x, currentPlatformBody->GetPosition().y); //set the starting position
        world->DestroyBody(currentPlatformBody);
        b2Body* newBody = world->CreateBody(&myBodyDef);
        for(int i=0; i < platformPoints.size()-1; i++)
        {
            Vec2 start = platformPoints[i];
            Vec2 end = platformPoints[i+1];
            this->addRectangleBetweenPointsToBody(newBody,start,end);
        }
        Rect bodyRectangle = getBodyRectangle(newBody);
        float anchorX = newBody->GetPosition().x * PTM_RATIO - bodyRectangle.origin.x;
        float anchorY = bodyRectangle.size.height - (visibleSize.height - bodyRectangle.origin.y - newBody->GetPosition().y * PTM_RATIO);
        
        Vec2 anchorPoint = Vec2(anchorX / bodyRectangle.size.width,  anchorY / bodyRectangle.size.height);
        
        // draw sprite use render texture
        auto _image = target->newImage();
        auto _key = std::to_string((int)time(NULL));
        auto _texture2D = Director::getInstance()->getTextureCache()->addImage(_image, _key);
        CC_SAFE_DELETE(_image);
        
        auto texture2D = Sprite::createWithTexture(_texture2D, bodyRectangle);
        texture2D->setAnchorPoint(anchorPoint);
        addChild(texture2D);
        newBody->SetUserData(texture2D);
    }
    removeChild(target, true);
    target->release();
    target = RenderTexture::create(visibleSize.width, visibleSize.height,Texture2D::PixelFormat::RGBA8888);
    target->retain();
    target->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    addChild(target);
}

void HelloWorld::addRectangleBetweenPointsToBody(b2Body* body, Vec2 start,
		Vec2 end) {
	float min = brush->getContentSize().width*brush->getScale()/PTM_RATIO;

	float sx = start.x;
	float sy = start.y;
	float ex = end.x;
	float ey = end.y;
	float dist_x = sx - ex;
	float dist_y = sy - ey;
	float angle = atan2(dist_y, dist_x);

	float px = (sx + ex) / 2 / PTM_RATIO - body->GetPosition().x;
	float py = (sy + ey) / 2 / PTM_RATIO - body->GetPosition().y;

//	float width = abs(distance) / PTM_RATIO;
//	float height = brush->boundingBox().size.height / PTM_RATIO;
	float width = MAX(abs(dist_x) / PTM_RATIO, min);
	float height = MAX(abs(dist_y) / PTM_RATIO, min);

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

	maxX2 *= PTM_RATIO;
	minX2 *= PTM_RATIO;
	maxY2 *= PTM_RATIO;
	minY2 *= PTM_RATIO;

	float width2 = maxX2 - minX2;
	float height2 = maxY2 - minY2;

	float remY2 = visibleSize.height - maxY2;

	return Rect(minX2, remY2, width2, height2);
}

void HelloWorld::clearScreen(cocos2d::Ref *pSender)
{
    this->m_bClearBox = true;
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

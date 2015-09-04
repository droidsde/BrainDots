#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

typedef struct ValueColor {
    std::string name;
    Color3B color;
} VALUE_COLOR;

const VALUE_COLOR listValueColor[] = {
    {"brown",       Color3B(66,3,3)},
    {"red",         Color3B(205, 0, 0)},
    {"orange",      Color3B(255,103,0)},
    {"pink",        Color3B(229,155,168)},
    {"purple",      Color3B(128,0,128)},
    {"black",       Color3B(22, 24, 3)},
    
    {"pale purple", Color3B(200,131,219)},
    {"dark blue",   Color3B(8,8,81)},
    {"gray",        Color3B(84,79,79)},
    {"greenish yellow", Color3B(129,212,11)},
    {"golden yellow",   Color3B(208,148,4)},
    {"blue",        Color3B(48,48,199)},
    {"green",       Color3B(3,154,3)},
    {"light blue",  Color3B(0,184,255)},
    {"yellow",      Color3B(208,208,4)},
    
    {"white",       Color3B(242,233,233)},
};

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
    if ( !LayerColor::initWithColor(Color4B(255,255,255,255)) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto close = Button::create("back.png");
    close->setTouchEnabled(true);
    close->setAnchorPoint(Vec2(0.5, 0.5));
    close->setPosition(Vec2(100, origin.y + visibleSize.height - 100));
    this->addChild(close, 50);
    close->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                ManageScene::getInstance()->changeState(GAME_STATE::MENU);
                break;
            }
            default:
                break;
        }
    });

    // load effect
    preLoadEffect();
    
    // shuffle generate array
    for (int i=0; i<SUMCOLOR; i++){
        arrayNumber[i] = i;
    }
    shuffleArray(arrayNumber, SUMCOLOR);
    // load name numbers follow current language
    for (int i=0; i<SUMCOLOR; i++) {
        CCLOG("%d", arrayNumber[i]);
        std::string number = "number_" + to_string(i+1);
        numberName[i] = ManageScene::getInstance()->getStringForKey(number).c_str();
    }
    
    
    /////////////////////////////
    // 3. add your codes below...
    
    winsize = Director::getInstance()->getWinSize();
    
    // init drawnode
    draw = DrawNode::create();
    addChild(draw, 10);
    
    // create layout color
    widthLayout = winsize.width-2*PADDING;
    
    // Layout A
    layoutA = Layout::create();
    layoutA->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layoutA->setBackGroundColor(listValueColor[15].color);
    layoutA->setSize(winsize);
    layoutA->setAnchorPoint(Vec2(0, 0));
    currentXOfA = -2 * widthLayout + PADDING;
    layoutA->setPosition(Vec2(currentXOfA,0));
    this->addChild(layoutA);
    
    // 5 layout first
    for (int i=0; i < 5; i++) {
        layout[i] = Layout::create();
        initLayout(layout[i], listValueColor[i].color, Size(widthLayout, winsize.height), Vec2(widthLayout*i, 0));
        layoutList.pushBack(layout[i]);
        layoutA->addChild(layout[i]);
    }
    
    // set index
    colorIndex = 2;
    
    // create name of number
    colorName = Label::createWithTTF(numberName[arrayNumber[colorIndex]].c_str(), "fonts/keifont.ttf", 150);
    colorName->setColor(Color3B::WHITE);
    colorName->setPosition(Vec2(winsize.width/2, winsize.height/3));
    colorName->setVerticalAlignment(TextVAlignment::CENTER);
    colorName->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(colorName,20);
    colorName->setOpacity(0.0f);
    
    // create number
    numberCount = Label::createWithTTF("", "fonts/keifont.ttf", 200);
    numberCount->setColor(Color3B::WHITE);
    numberCount->setPosition(Vec2(winsize.width/2, winsize.height*2/3));
    numberCount->setVerticalAlignment(TextVAlignment::CENTER);
    numberCount->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(numberCount,20);
    numberCount->setOpacity(0.0f);
    numberCount->setString(to_string(arrayNumber[colorIndex]+1));
    
    // run animation counter circles
    drawCounterCircles(arrayNumber[colorIndex]+1);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void HelloWorld::initLayout(cocos2d::ui::Layout* layout, Color3B color, Size size, Vec2 position)
{
    layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(color);
    layout->setSize(size);
    layout->setPosition(position);
}

void HelloWorld::preLoadEffect()
{
    audio = CocosDenshion::SimpleAudioEngine::getInstance();
    for (int i=0; i < SUMCOLOR; i++) {
        std::string filename = "number_" + to_string(i+1) + "_" + ManageScene::getInstance()->getCurrentLanguage();
        std::string temp = ManageScene::getInstance()->getFileNameFollowPlatform(filename);
        if (ManageScene::getInstance()->checkFileSoundExist(temp.c_str())) {
            filename = ManageScene::getInstance()->getFileNameFollowPlatform(filename);
        } else {
            filename += ".mp3";
        }
        audio->preloadEffect(filename.c_str());
    }
}

void HelloWorld::playEffect(int number)
{
    if (!ManageScene::getInstance()->getPLaySound()) {
        return;
    }
    std::string filename = "number_" + to_string(number) + "_" + ManageScene::getInstance()->getCurrentLanguage();
    std::string temp = ManageScene::getInstance()->getFileNameFollowPlatform(filename);
    if (ManageScene::getInstance()->checkFileSoundExist(temp.c_str())) {
        filename = ManageScene::getInstance()->getFileNameFollowPlatform(filename);
    } else {
        filename += ".mp3";
    }
    audio->playEffect(filename.c_str());
}

void HelloWorld::shuffleArray(int *arr, size_t n)
{
    if (n > 1)
    {
        size_t i;
        srand((int)time(NULL));
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = arr[j];
            arr[j] = arr[i];
            arr[i] = t;
        }
    }
}

bool HelloWorld::checkDrawNodeRunning()
{
    if (!circles.empty()) {
        for (int i=0; i<circles.size(); i++) {
            if (circles.at(i)->getNumberOfRunningActions()>0) {
                return false;
            }
        }
    }
    return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{

    firstTouch = touch->getLocation();
    
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    // setup variables for drawing bezier
    Vec2 origin,control1,control2,destination;
    draw->clear();
    movingTouch = touch->getLocation();
    float deltaX = movingTouch.x - firstTouch.x;
    int increase = 0;
    Color3B color;
    if (fabs(deltaX) < INCREASE) {
        increase = fabs(deltaX);
    } else {
        increase = INCREASE;
    }
    
    // get layout color
    colorAfter = layoutList.at(3)->getBackGroundColor();
    colorBefore = layoutList.at(1)->getBackGroundColor();
    
    // set state for moving left,right
    if ( deltaX > 0 ) {
        state = STATE_MOVE::RIGHT;
        color = colorBefore;
    } else if( deltaX < 0){
        state = STATE_MOVE::LEFT;
        color = colorAfter;
    }
    
    // prepare cordination of origin,control1,control2,destination each left, right
    switch (state) {
        case STATE_MOVE::RIGHT :
            origin = Vec2(PADDING, -movingTouch.x/6);
            control1 = Vec2(fabs(deltaX) + PADDING + increase, movingTouch.y);
            control2 = control1;
            destination = Vec2(PADDING, winsize.height + movingTouch.x/6);
            break;
            
        case STATE_MOVE::LEFT :
            origin = Vec2(winsize.width - PADDING, (movingTouch.x - winsize.width)/6);
            control1 = Vec2(winsize.width - fabs(deltaX) - PADDING - increase, movingTouch.y);
            control2 = control1;
            destination = Vec2(winsize.width - PADDING, winsize.height + (winsize.width - movingTouch.x)/6);
            break;
        default:
            break;
    }
    
    // draw and fill color
    if ( firstTouch.x > winsize.width * 3/4 )
    {
        control1.y -= SPACING_P1P2;
        control2.y += SPACING_P1P2;
        fillColorBezierCubic(origin, control1, control2, destination, color, state);
    }
    else if ( firstTouch.x <= winsize.width * 3/4 &&  firstTouch.x > winsize.width/2 )
    {
        fillColorBezierQuad(origin, control1, destination, color, state);

    }
    else if ( firstTouch.x <= winsize.width/2 && firstTouch.x > winsize.width/4 )
    {
        fillColorBezierQuad(origin, control1, destination, color, state);

    }
    else if ( firstTouch.x <= winsize.width/4 )
    {
        control1.y -= SPACING_P1P2;
        control2.y += SPACING_P1P2;
        fillColorBezierCubic(origin, control1, control2, destination, color, state);
    }
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    draw->clear();
    endTouch = touch->getLocation();
    float deltaX = endTouch.x - firstTouch.x;
    
    // case move layout
    if ( fabs(deltaX) > MIN_MOVING )
    {
        if ( deltaX > MIN_MOVING ) {
            state = STATE_MOVE::RIGHT;
        } else if( deltaX < -MIN_MOVING){
            state = STATE_MOVE::LEFT;
        }
        
        //animation
        switch (state) {
            case STATE_MOVE::RIGHT :
            {
                auto node1 = layoutList.at(0);
                auto node2 = layoutList.at(4);
                node2->setPositionX(node1->getPositionX() - widthLayout);
                layoutList.erase(4);
                layoutList.insert(0, node2);
                currentXOfA += widthLayout;
                    
                // change color for next of next layout
                colorIndex = (colorIndex-1)< 0 ? (SUMCOLOR-1) : (colorIndex-1);
                int _colorIndex = colorIndex - 2;
                if (_colorIndex == -1)
                    _colorIndex = SUMCOLOR - 1;
                else if (_colorIndex == -2)
                    _colorIndex = SUMCOLOR - 2;
                layoutList.at(0)->setBackGroundColor(listValueColor[_colorIndex].color);
                break;
            }
                
            case STATE_MOVE::LEFT :
            {
                auto node1 = layoutList.at(0);
                auto node2 = layoutList.at(4);
                node1->setPositionX(node2->getPositionX() + widthLayout);
                
                layoutList.erase(0);
                layoutList.pushBack(node1);
                currentXOfA -= widthLayout;
                    
                colorIndex = (colorIndex+1)>= SUMCOLOR ? 0 : (colorIndex+1);
                int _colorIndex = colorIndex + 2;
                if (_colorIndex == SUMCOLOR)
                    _colorIndex = 0;
                else if (_colorIndex == SUMCOLOR + 1)
                    _colorIndex = 1;
                layoutList.at(4)->setBackGroundColor(listValueColor[_colorIndex].color);
                break;
            }
            default:
                break;
        }
        
        // move layout
        auto moveTo = MoveTo::create(0.1, Vec2(currentXOfA, 0));
        auto move_ease = EaseElasticOut::create(moveTo);
        move_ease->setDuration(0.3);
        auto moveToEndX = moveTo->clone();
        layoutA->stopAllActions();
        layoutA->runAction(Sequence::create(move_ease, moveToEndX, NULL));
        
        // change name color
        colorName->setString(numberName[arrayNumber[colorIndex]].c_str());
        numberCount->setString(to_string(arrayNumber[colorIndex]+1));
        colorName->setOpacity(0.0f);
        numberCount->setOpacity(0.0f);
        // draw counter circles
        drawCounterCircles(arrayNumber[colorIndex]+1);
        return;
    }
    
    // run animation for colorname
    if (checkDrawNodeRunning()) {
        animationColorName();
        animationNumbers();
    }
}

void HelloWorld::animationColorName()
{
    // animation
    playEffect(arrayNumber[colorIndex]+1);
    
    if (!ManageScene::getInstance()->getShowNameNumber()) {
        return;
    }
    auto rotateIn = RotateTo::create(0.1, 5);
    auto rotateOut = RotateTo::create(0.1, 0);
    auto sequenceRotate = Sequence::create(rotateIn, rotateOut, NULL);
    
    if (colorName->getOpacity()==0) {
        colorName->setScale(0.0);
        auto fadeIn = FadeIn::create(0.1);
        auto zoomIn = ScaleTo::create(0.1, 1.0);
        
        auto spawn = Spawn::create(sequenceRotate, zoomIn, fadeIn, NULL);
        auto hideAnimation = Sequence::create(spawn, DelayTime::create(1.0), FadeOut::create(0.1), NULL);
        colorName->runAction(hideAnimation);
    }
    else {
        if (colorName->getNumberOfRunningActions()!=0) {
            colorName->stopAllActions();
            colorName->runAction(sequenceRotate);
        }
        else {
            auto sequence = Sequence::create(sequenceRotate, FadeOut::create(0.1), NULL);
            colorName->runAction(sequence);
        }
    }
}

void HelloWorld::animationNumbers()
{
    if (!ManageScene::getInstance()->getShowNumber()) {
        return;
    }
    auto rotateIn = RotateTo::create(0.1, 5);
    auto rotateOut = RotateTo::create(0.1, 0);
    auto sequenceRotate = Sequence::create(rotateIn, rotateOut, NULL);
    
    if (numberCount->getOpacity()==0) {
        numberCount->setScale(0.0);
        auto fadeIn = FadeIn::create(0.1);
        auto zoomIn = ScaleTo::create(0.1, 1.0);
        
        auto spawn = Spawn::create(sequenceRotate, zoomIn, fadeIn, NULL);
        auto hideAnimation = Sequence::create(spawn, DelayTime::create(1.0), FadeOut::create(0.1), NULL);
        numberCount->runAction(hideAnimation);
    }
    else {
        if (numberCount->getNumberOfRunningActions()!=0) {
            numberCount->stopAllActions();
            numberCount->runAction(sequenceRotate);
        }
        else {
            auto sequence = Sequence::create(sequenceRotate, FadeOut::create(0.1), NULL);
            numberCount->runAction(sequence);
        }
    }
}

void HelloWorld::fillColorTriangle(Vec2 p0, Vec2 p1, Vec2 p2)
{
    for (int i=0; i<(p0.x-p1.x); i++) {
        // path1: P0P1
        // y = ax + b
        // a = (p0.y - p1.y) / (p0.x - p1.x)
        // b = (p0.x * p1.y - p0.y * p1.x) / (p0.x - p1.x)
        float y1 = (p0.y - p1.y) / (p0.x - p1.x) * (p1.x+i) + (p0.x * p1.y - p0.y * p1.x) / (p0.x - p1.x);
        // path2: P1P2
        float y2 = (p2.y - p1.y) / (p2.x - p1.x) * (p1.x+i) + (p2.x * p1.y - p2.y * p1.x) / (p2.x - p1.x);
        draw->drawLine(Vec2(p1.x+i,y1), Vec2(p1.x+i,y2), Color4F(0.0, 1, 0.5, 1));
    }
}

void HelloWorld::fillColorBezierQuad(cocos2d::Vec2 p0, cocos2d::Vec2 p1, cocos2d::Vec2 p2, Color3B color, STATE_MOVE state)
{
    Vec2* vertices = new (std::nothrow) Vec2[SEGMENT + 1];
    float* test = new float[SEGMENT+1];
    float radius;
    if( ! vertices )
        return;
    
    float t = 0.0f;
    for(unsigned int i = 0; i < SEGMENT; i++)
    {
        vertices[i].x = powf(1 - t, 2) * p0.x + 2.0f * (1 - t) * t * p1.x + t * t * p2.x;
        vertices[i].y = powf(1 - t, 2) * p0.y + 2.0f * (1 - t) * t * p1.y + t * t * p2.y;
        test[i] = vertices[i].x;
        t += 1.0f / SEGMENT;
    }
    vertices[SEGMENT].x = p2.x;
    vertices[SEGMENT].y = p2.y;
    test[SEGMENT] = p2.x;
    
    switch (state) {
        case STATE_MOVE::LEFT :
        {
            float minX = *std::min_element(test, test+SEGMENT);
            radius = (p2.x - minX) *2 / SEGMENT;
            break;
        }
        case STATE_MOVE::RIGHT :
        {
            float maxX = *std::max_element(test, test+SEGMENT);
            radius = maxX *2 / SEGMENT;
            break;
        }
        default:
            break;
    }
    
    for (unsigned int i = 0; i <= SEGMENT/2; i++) {
        draw->drawSegment(Vec2(vertices[i].x,vertices[i].y), Vec2(vertices[SEGMENT-i].x,vertices[SEGMENT-i].y), radius, Color4F(color));
    }
    CC_SAFE_DELETE_ARRAY(vertices);
    CC_SAFE_DELETE_ARRAY(test);
}

void HelloWorld::fillColorBezierCubic(cocos2d::Vec2 p0, cocos2d::Vec2 p1, cocos2d::Vec2 p2, cocos2d::Vec2 p3, Color3B color, STATE_MOVE state)
{
    Vec2* vertices = new (std::nothrow) Vec2[SEGMENT + 1];
    float* test = new float[SEGMENT+1];
    float radius;
    if ( ! vertices)
        return;
    
    float t = 0;
    for (unsigned int i = 0; i < SEGMENT; i++) {
        vertices[i].x = powf(1-t, 3) * p0.x + 3.0f * powf(1-t, 2) * t * p1.x + 3.0f * (1-t) * t * t * p2.x +
        t * t * t * p3.x;
        vertices[i].y = powf(1-t, 3) * p0.y + 3.0f * powf(1-t, 2) * t * p1.y + 3.0f * (1-t) * t * t * p2.y +
        t * t * t * p3.y;
        test[i] = vertices[i].x;
        t += 1.0f / SEGMENT;
    }
    vertices[SEGMENT].x = p3.x;
    vertices[SEGMENT].y = p3.y;
    test[SEGMENT] = p3.x;
    
    switch (state) {
        case STATE_MOVE::LEFT :
        {
            float minX = *std::min_element(test, test+SEGMENT);
            radius = (p3.x - minX) *2 / SEGMENT;
            break;
        }
        case STATE_MOVE::RIGHT :
        {
            float maxX = *std::max_element(test, test+SEGMENT);
            radius = maxX *2 / SEGMENT;
            break;
        }
        default:
            break;
    }
    
    for (unsigned int i = 0; i <= SEGMENT/2; i++) {
        draw->drawSegment(Vec2(vertices[i].x,vertices[i].y), Vec2(vertices[SEGMENT-i].x,vertices[SEGMENT-i].y), radius, Color4F(color));
    }
    CC_SAFE_DELETE_ARRAY(vertices);
    CC_SAFE_DELETE_ARRAY(test);
}

void HelloWorld::clearListDrawNodeAndShowColorName()
{
    removeAndClearListDrawNode();
    animationColorName();
    animationNumbers();
}

void HelloWorld::removeAndClearListDrawNode()
{
    for (int i=0; i<circles.size(); i++) {
        if(circles.at(i)->getNumberOfRunningActions()>0)
            circles.at(i)->stopAllActions();
        circles.at(i)->clear();
        circles.at(i)->removeFromParentAndCleanup(true);
    }
    circles.clear();
}

void HelloWorld::drawCounterCircles(int number)
{
    // remove list
    removeAndClearListDrawNode();
    
    //draw a solid circle
    switch (number) {
        case 1:
            drawCircle(Vec2(winsize.width/2, winsize.height/2), 0, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 2:
            drawCircle(Vec2(winsize.width*2/3, winsize.height/2), 0);
            drawCircle(Vec2(winsize.width/3, winsize.height/2), TIME_CHANGE_ANIMATION * 1, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 3:
            drawCircle(Vec2(winsize.width/4, winsize.height/2), 0);
            drawCircle(Vec2(winsize.width/2, winsize.height/2), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/2), TIME_CHANGE_ANIMATION * 2, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 4:
            drawCircle(Vec2(winsize.width/4, winsize.height/4), 0);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/4), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 2);
            drawCircle(Vec2(winsize.width*3/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 3, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 5:
            drawCircle(Vec2(winsize.width/4, winsize.height/4), 0);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/4), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 2);
            drawCircle(Vec2(winsize.width*3/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 3);
            drawCircle(Vec2(winsize.width/2, winsize.height/2), TIME_CHANGE_ANIMATION * 4, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 6:
            drawCircle(Vec2(winsize.width/4, winsize.height/4), 0);
            drawCircle(Vec2(winsize.width/2, winsize.height/4), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/4), TIME_CHANGE_ANIMATION * 2);
            drawCircle(Vec2(winsize.width/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 3);
            drawCircle(Vec2(winsize.width/2, winsize.height*3/4), TIME_CHANGE_ANIMATION * 4);
            drawCircle(Vec2(winsize.width*3/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 5, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 7:
            drawCircle(Vec2(winsize.width/4, winsize.height/4), 0);
            drawCircle(Vec2(winsize.width/2, winsize.height/4), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/4), TIME_CHANGE_ANIMATION * 2);
            drawCircle(Vec2(winsize.width/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 3);
            drawCircle(Vec2(winsize.width/2, winsize.height*3/4), TIME_CHANGE_ANIMATION * 4);
            drawCircle(Vec2(winsize.width*3/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 5);
            drawCircle(Vec2(winsize.width/2, winsize.height/2), TIME_CHANGE_ANIMATION * 6, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 8:
            drawCircle(Vec2(winsize.width/4, winsize.height/4), 0);
            drawCircle(Vec2(winsize.width/2, winsize.height/4), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/4), TIME_CHANGE_ANIMATION * 2);
            drawCircle(Vec2(winsize.width/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 3);
            drawCircle(Vec2(winsize.width/2, winsize.height*3/4), TIME_CHANGE_ANIMATION * 4);
            drawCircle(Vec2(winsize.width*3/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 5);
            
            drawCircle(Vec2(winsize.width/4, winsize.height/2), TIME_CHANGE_ANIMATION * 6);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/2), TIME_CHANGE_ANIMATION * 7, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 9:
            drawCircle(Vec2(winsize.width/4, winsize.height/4), 0);
            drawCircle(Vec2(winsize.width/2, winsize.height/4), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/4), TIME_CHANGE_ANIMATION * 2);
            drawCircle(Vec2(winsize.width/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 3);
            drawCircle(Vec2(winsize.width/2, winsize.height*3/4), TIME_CHANGE_ANIMATION * 4);
            drawCircle(Vec2(winsize.width*3/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 5);
            
            drawCircle(Vec2(winsize.width/4, winsize.height/2), TIME_CHANGE_ANIMATION * 6);
            drawCircle(Vec2(winsize.width/2, winsize.height/2), TIME_CHANGE_ANIMATION * 7);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/2), TIME_CHANGE_ANIMATION * 8, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        case 10:
            drawCircle(Vec2(winsize.width/4, winsize.height/4), 0);
            drawCircle(Vec2(winsize.width/2, winsize.height/4), TIME_CHANGE_ANIMATION * 1);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/4), TIME_CHANGE_ANIMATION * 2);
            drawCircle(Vec2(winsize.width/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 3);
            drawCircle(Vec2(winsize.width/2, winsize.height*3/4), TIME_CHANGE_ANIMATION * 4);
            drawCircle(Vec2(winsize.width*3/4, winsize.height*3/4), TIME_CHANGE_ANIMATION * 5);
            
            drawCircle(Vec2(winsize.width/4, winsize.height/2 - 100), TIME_CHANGE_ANIMATION * 6);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/2 - 100), TIME_CHANGE_ANIMATION * 7);
            
            drawCircle(Vec2(winsize.width/4, winsize.height/2 + 100), TIME_CHANGE_ANIMATION * 8);
            drawCircle(Vec2(winsize.width*3/4, winsize.height/2 + 100), TIME_CHANGE_ANIMATION * 9, CallFunc::create(CC_CALLBACK_0(HelloWorld::clearListDrawNodeAndShowColorName, this)));
            break;
        default:
            break;
    }
}

void HelloWorld::drawCircle(Vec2 position, float delay)
{
    drawCircle(position, delay, nullptr);
}

void HelloWorld::drawCircle(cocos2d::Vec2 position, float delay, cocos2d::CallFunc *callFuncOnCompletion)
{
    Sequence* sequence;
    auto circle = DrawNode::create();
    addChild(circle, 40);
    circles.pushBack(circle);
    
    //    circle->drawDot(position, CIRCLE_RADIUS, Color4F(listValueColor[15].color));
    circle->drawSolidCircle(position, CIRCLE_RADIUS, CC_DEGREES_TO_RADIANS(90), SEGMENT, Color4F(listValueColor[15].color));
    circle->setScale(0.0);
    
    auto zoomIn = ScaleTo::create(TIME_CHANGE_ANIMATION, 1.0f);
    if (callFuncOnCompletion != nullptr) {
        sequence = Sequence::create(DelayTime::create(delay), zoomIn, DelayTime::create(1.0f), callFuncOnCompletion, NULL);
    }
    else sequence = Sequence::create(DelayTime::create(delay), zoomIn, NULL);

    circle->runAction(sequence);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

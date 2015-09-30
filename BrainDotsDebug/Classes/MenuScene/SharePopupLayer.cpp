//
//  SharePopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "SharePopupLayer.h"

SharePopupLayer::SharePopupLayer(std::string filename)
{
    imageShare = nullptr;
    this->filename = filename;
}

SharePopupLayer::~SharePopupLayer()
{
    layoutTable = nullptr;
    imageShare = nullptr;
}

SharePopupLayer* SharePopupLayer::create(std::string filename)
{
    SharePopupLayer* layer = new SharePopupLayer(filename);
    layer->init();
    layer->autorelease();
    return layer;
}

bool SharePopupLayer::init()
{
    BasePopupLayer::init();
    auto layoutSize = layoutTable->getContentSize();
    // title
    auto title = Text::create("Share to everybody", "arial.ttf", 60);
    title->setPosition(Vec2(layoutSize.width/2, layoutSize.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::RED);
    layoutTable->addChild(title);
    
    // image share
    // create paper sprite
    PaperSprite* paperSprite = PaperSprite::create("paper3.png", false, false, false);
    paperSprite->setPosition(layoutSize/2);
    paperSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layoutTable->addChild(paperSprite);
    
    // check file capture screen isexist
    if (SceneManager::getInstance()->checkFileExist(FileUtils::getInstance()->getWritablePath()+this->filename)) {
        imageShare = Sprite::create(FileUtils::getInstance()->getWritablePath()+this->filename);
    } else {
        imageShare = Sprite::create("share_image.png");
    }
    imageShare->setScale(0.5f);
    imageShare->setPosition(paperSprite->getContentSize()/2);
    imageShare->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    paperSprite->addChild(imageShare);
    
    float posX = layoutSize.width / 6;
    // button share
    // twitter button
    twitterShare = createButton("share_twitter.png", TAG_POPUP_SHARE::TAG_TWITTER, Vec2::ANCHOR_MIDDLE_BOTTOM);
    twitterShare->setPosition(Vec2(posX, PADDING));
    twitterShare->setScale(0.25);
    twitterShare->addTouchEventListener(CC_CALLBACK_2(SharePopupLayer::touchButtonEvent, this));
    layoutTable->addChild(twitterShare);
    
    // facebook button
    facebookShare = createButton("share_facebook.png", TAG_POPUP_SHARE::TAG_FACEBOOK, Vec2::ANCHOR_MIDDLE_BOTTOM);
    facebookShare->setPosition(Vec2(posX * 2, PADDING));
    facebookShare->setScale(0.25);
    facebookShare->addTouchEventListener(CC_CALLBACK_2(SharePopupLayer::touchButtonEvent, this));
    layoutTable->addChild(facebookShare);
    
    // line button
    lineShare = createButton("share_line.png", TAG_POPUP_SHARE::TAG_LINE, Vec2::ANCHOR_MIDDLE_BOTTOM);
    lineShare->setPosition(Vec2(posX * 3, PADDING));
    lineShare->setScale(0.25);
    lineShare->addTouchEventListener(CC_CALLBACK_2(SharePopupLayer::touchButtonEvent, this));
    layoutTable->addChild(lineShare);
    
    // ggplus button
    ggplusShare = createButton("share_googleplus.png", TAG_POPUP_SHARE::TAG_GGPLUS, Vec2::ANCHOR_MIDDLE_BOTTOM);
    ggplusShare->setPosition(Vec2(posX * 4, PADDING));
    ggplusShare->setScale(0.25);
    ggplusShare->addTouchEventListener(CC_CALLBACK_2(SharePopupLayer::touchButtonEvent, this));
    layoutTable->addChild(ggplusShare);
    
    // more button
    moreShare = createButton("share_more.png", TAG_POPUP_SHARE::TAG_MORESHARE, Vec2::ANCHOR_MIDDLE_BOTTOM);
    moreShare->setPosition(Vec2(posX * 5, PADDING));
    moreShare->setScale(0.5);
    moreShare->addTouchEventListener(CC_CALLBACK_2(SharePopupLayer::touchButtonEvent, this));
    layoutTable->addChild(moreShare);
    
    return true;
}

void SharePopupLayer::touchButtonEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    auto receiver = (Node*) sender;
    if (type == ui::Widget::TouchEventType::ENDED) {
        switch (receiver->getTag()) {
            case TAG_POPUP_SHARE::TAG_TWITTER :
            {
                break;
            }
            
            case TAG_POPUP_SHARE::TAG_FACEBOOK :
            {
                break;
            }
            
            case TAG_POPUP_SHARE::TAG_LINE :
            {
                break;
            }
            
            case TAG_POPUP_SHARE::TAG_GGPLUS :
            {
                break;
            }
            
            case TAG_POPUP_SHARE::TAG_MORESHARE :
            {
                break;
            }
            default:
                break;
        }
    }
}

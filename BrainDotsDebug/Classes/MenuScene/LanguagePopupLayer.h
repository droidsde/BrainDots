//
//  LanguagePopupLayer.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/30.
//
//

#ifndef __BrainDotsDebug__LanguagePopupLayer__
#define __BrainDotsDebug__LanguagePopupLayer__

#include <cocos2d.h>
#include "BasePopupLayer.h"
#include "SceneManager.h"
//#include "ScrollBarView.h"

USING_NS_CC;

using namespace cocos2d::ui;

class LanguagePopupLayer : public BasePopupLayer
{
public:
    
    LanguagePopupLayer();
    ~LanguagePopupLayer();
    static LanguagePopupLayer* create();
    virtual bool init();
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    void onExit();
    
private:
    
    Size layoutSize;
    Size listviewSize;
    int countLanguage;
    std::map<std::string, std::string> mapLanguage;
    std::vector<std::string> listLanguageName;
    
    std::string getValueFromMap(std::string key);
    
    ListView* listView;
    Vector<Button*> listButton;
    void reloadData();
};

#endif /* defined(__BrainDotsDebug__LanguagePopupLayer__) */

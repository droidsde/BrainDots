//
//  FBPlugin.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/16.
//
//

#ifndef __BrainDotsDebug__FBPlugin__
#define __BrainDotsDebug__FBPlugin__

#include <cocos2d.h>
#include "PluginFacebook/PluginFacebook.h"

USING_NS_CC;
using namespace sdkbox;

class FBPlugin : public sdkbox::FacebookListener
{
public:
    
    FBPlugin();
    ~FBPlugin();
    
    bool checkLogin();
    void fbLogin();
    void fbCheckStatus();
    void fbGetMyInfo();
    void fbGetMyFriends();
    void fbCaptureScreen();
    void fbShareLink(std::string link, std::string title, std::string text, std::string image);
    void fbSharePhoto(std::string fileName, std::string title);
    void fbDialogLink(std::string link, std::string title, std::string text, std::string image);
    void fbDialogPhoto(std::string fileName, std::string title);
    void fbRequestReadPermission();
    void fbRequestPublishPermission();
    
private:
    
    //Facebook callback
    void onLogin(bool isLogin, const std::string& msg);
    void onSharedSuccess(const std::string& message);
    void onSharedFailed(const std::string& message);
    void onSharedCancel();
    void onAPI(const std::string& key, const std::string& jsonData);
    void onPermission(bool isLogin, const std::string& msg);
    void onFetchFriends(bool ok, const std::string& msg);
};
#endif /* defined(__BrainDotsDebug__FBPlugin__) */

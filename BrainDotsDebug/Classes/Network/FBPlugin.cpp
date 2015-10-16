//
//  FBPlugin.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/16.
//
//

#include "FBPlugin.h"

static void checkFaceBookStatus()
{
    CCLOG("##FB> permission list: ");
    for (auto& permission : PluginFacebook::getPermissionList())
    {
        CCLOG("##FB>> permission %s", permission.data());
    }
    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
}

FBPlugin::FBPlugin()
{
    PluginFacebook::setListener(this);
}

FBPlugin::~FBPlugin()
{
    
}

bool FBPlugin::checkLogin()
{
    if (PluginFacebook::isLoggedIn())
    {
        return true;
    } else
        return false;
}

void FBPlugin::fbLogin()
{
    CCLOG("##FB %s", __FUNCTION__);
    if (!PluginFacebook::isLoggedIn())
    {
        PluginFacebook::login();
    }
}

void FBPlugin::fbCheckStatus()
{
    CCLOG("##FB %s", __FUNCTION__);
    checkFaceBookStatus();
}

void FBPlugin::fbGetMyInfo()
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        FBAPIParam params;
        PluginFacebook::api("me", "GET", params, "me");
    }
}

void FBPlugin::fbGetMyFriends()
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
//        PluginFacebook::fetchFriends();
    }
}

void FBPlugin::fbCaptureScreen()
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        
    }
}

void FBPlugin::fbShareLink(std::string link, std::string title, std::string text, std::string image)
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        FBShareInfo info;
        info.type  = FB_LINK;
        info.link  = "http://www.cocos2d-x.org";
        info.title = "cocos2d-x";
        info.text  = "Best Game Engine";
        info.image = "http://cocos2d-x.org/images/logo.png";
        PluginFacebook::share(info);
    }
}

void FBPlugin::fbSharePhoto(std::string fileName, std::string title)
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        if (!fileName.empty() && FileUtils::getInstance()->isFileExist(fileName))
        {
            CCLOG("##FB dialog photo: %s", fileName.c_str());
            
            FBShareInfo info;
            info.type  = FB_PHOTO;
            info.title = title;
            info.image = fileName;
            PluginFacebook::share(info);
        }
        else
        {
            CCLOG("##FB capture screen first");
        }
    }
}

void FBPlugin::fbDialogLink(std::string link, std::string title, std::string text, std::string image)
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        FBShareInfo info;
        info.type  = FB_LINK;
        info.link  = "http://www.cocos2d-x.org";
        info.title = "cocos2d-x";
        info.text  = "Best Game Engine";
        info.image = "http://cocos2d-x.org/images/logo.png";
        PluginFacebook::dialog(info);
    }
}

void FBPlugin::fbDialogPhoto(std::string fileName, std::string title)
{
    CCLOG("##FB %s", __FUNCTION__);
    if (PluginFacebook::isLoggedIn())
    {
        if (!fileName.empty() && FileUtils::getInstance()->isFileExist(fileName))
        {
            CCLOG("dialog photo: %s", fileName.c_str());
            FBShareInfo info;
            info.type  = FB_PHOTO;
            info.title = title;
            info.image = fileName;
            PluginFacebook::dialog(info);
        }
        else
        {
            CCLOG("##FB capture screen first");
        }
    }
}

void FBPlugin::fbRequestPublishPermission()
{
    CCLOG("##FB %s", __FUNCTION__);
    PluginFacebook::requestPublishPermissions({FB_PERM_PUBLISH_POST});
}

void FBPlugin::fbRequestReadPermission()
{
    CCLOG("##FB %s", __FUNCTION__);
    PluginFacebook::requestReadPermissions({FB_PERM_READ_USER_FRIENDS});
}

/*********************
 * Facebook callbacks
 *********************/
void FBPlugin::onLogin(bool isLogin, const std::string& error)
{
    CCLOG("##FB isLogin: %d, error: %s", isLogin, error.c_str());
    
    std::string title = "login ";
    title.append((isLogin ? "success" : "failed"));
    MessageBox(error.c_str(), title.c_str());
    
    if (isLogin) {
        this->fbRequestPublishPermission();
    }
}

void FBPlugin::onAPI(const std::string& tag, const std::string& jsonData)
{
    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
}

void FBPlugin::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
    
    MessageBox(message.c_str(), "share success");
}

void FBPlugin::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
    
    MessageBox(message.c_str(), "share failed");
    
    // request permission
    this->fbRequestPublishPermission();
}

void FBPlugin::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
    
    MessageBox("", "share cancel");
}

void FBPlugin::onPermission(bool isLogin, const std::string& error)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, error.c_str());
    
    std::string title = "permission ";
    title.append((isLogin ? "success" : "failed"));
    MessageBox(error.c_str(), title.c_str());
}

void FBPlugin::onFetchFriends(bool ok, const std::string& msg)
{
    CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
    
    const std::vector<sdkbox::FBGraphUser>& friends = PluginFacebook::getFriends();
    for (int i = 0; i < friends.size(); i++)
    {
        const sdkbox::FBGraphUser& user = friends.at(i);
        CCLOG("##FB> -------------------------------");
        CCLOG("##FB>> %s", user.uid.data());
        CCLOG("##FB>> %s", user.firstName.data());
        CCLOG("##FB>> %s", user.lastName.data());
        CCLOG("##FB>> %s", user.name.data());
        CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
        CCLOG("##FB");
    }
    
    MessageBox("", "fetch friends");
}

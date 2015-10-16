/****************************************************************************

 Copyright (c) 2014-2015 Chukong Technologies

 ****************************************************************************/
#ifndef _PLUGIN_FACKBOOK_H_
#define _PLUGIN_FACKBOOK_H_

#include <vector>
#include <map>
#include <string>

namespace sdkbox
{
    class Json;

    const std::string FB_PERM_READ_PUBLIC_PROFILE("public_profile");
    const std::string FB_PERM_READ_EMAIL("email");
    const std::string FB_PERM_READ_USER_FRIENDS("user_friends");
    const std::string FB_PERM_PUBLISH_POST("publish_actions");
    const std::string FB_API_ME_FRIENDS_TAG("__fb_me_friends__");
    const std::string FB_API_ME_FRIENDS_PATH("me/friends");


    const std::string kRI_ExcludeFromList("ExcludeFromListIds");
    const std::string kRI_PictureSize("PictureSize");
    const std::string kRI_PaginationLimit("PaginationLimit");
    const std::string kRI_ResponseFields("ResponseFields");


    enum FBShareType
    {
        FB_NONE  = 0,
        FB_LINK  = 1,
        FB_PHOTO = 2
    };

    typedef std::map<std::string, std::string> FBAPIParam;

    struct FBShareInfo
    {
        FBShareInfo()
        : type(FB_NONE), link(""), title("")
        , text(""), image("")
        {}

        FBShareType type;
        std::string link;
        std::string title;
        std::string text;
        std::string image;
    };

    struct FBGraphUser
    {
        FBGraphUser();

        std::string uid;
        std::string name;
        std::string firstName;
        std::string lastName;
        bool        isInstalled;
    };

    /*** INVITE-OUT
    struct FBInvitableUserInfo {
        std::string token;
        std::string name;
        bool picture_is_silhouete;
        std::string picture_url;

        FBInvitableUserInfo( const sdkbox::Json& json );
    };

    struct FBInvitableUsersCursor {
        std::string next_url;
        std::string prev_url;
        std::string next_cursor;
        std::string prev_cursor;

        FBInvitableUsersCursor( const sdkbox::Json& json );
        FBInvitableUsersCursor( );
    };
***/

    /**
     * FBInvitableFriendsInfo is returned as a result of a call to the FB plugin's method:
     * <code>requestInvitableFriends</code>.
     *
     * The idea is that with this information you can build a custom invitation dialog for your
     * FB contacts. Next to the name, there's an invitation token (FBInvitableUserInfo).
     * After the user makes the selection of people to invite, a call to the method
     * <code>inviteFriendsWithInviteIds</code> with an array of the desired invitation tokens
     * will invoke a FB dialog with invitation which will eventually send the invitations.
     *
     */
//    class FBInvitableFriendsInfo {
//
//    private:
//        std::vector<FBInvitableUserInfo>    _invitableUsersInfo;
//        FBInvitableUsersCursor              _cursorInfo;
//
//        // the originally returned string for the FB graph call.
//        std::string                         _originalJSONString;
//
//    public:
//        FBInvitableFriendsInfo( const std::string& json );
//        FBInvitableFriendsInfo* init();
//
//        typedef std::vector<FBInvitableUserInfo>::iterator iterator;
//        typedef std::vector<FBInvitableUserInfo>::const_iterator const_iterator;
//
//        size_t getNumInvitationTokens() const {
//            return _invitableUsersInfo.size();
//        }
//
//        iterator begin() { return _invitableUsersInfo.begin(); }
//        const_iterator begin() const { return _invitableUsersInfo.begin(); }
//        iterator end() { return _invitableUsersInfo.end(); }
//        const_iterator end() const { return _invitableUsersInfo.end(); }
//
//        const std::string& getOriginalString() const {
//            return _originalJSONString;
//        }
//
//        /**
//         * Request this url to get the next invitable friends document info.
//         * Maybe empty.
//         */
//        const std::string& getNextURL() const {
//            return _cursorInfo.next_url;
//        }
//
//        /**
//         * Request this url to get the prev invitable friends document info.
//         * Maybe empty;
//         */
//        const std::string& getPrevURL() const {
//            return _cursorInfo.prev_url;
//        }
//
//        /**
//         * Use this string to build the NextURL.
//         */
//        const std::string& getNextCursor() const {
//            return _cursorInfo.next_cursor;
//        }
//
//        /**
//         * Use this string to build the PrevURL.
//         */
//        const std::string& getPrevCursor() const {
//            return _cursorInfo.prev_cursor;
//        }
//    };

    class FacebookListener
    {
    public:
        virtual void onLogin(bool isLogin, const std::string& msg) = 0;
        virtual void onSharedSuccess(const std::string& message) = 0;
        virtual void onSharedFailed(const std::string& message) = 0;
        virtual void onSharedCancel() = 0;
        virtual void onAPI(const std::string& key, const std::string& jsonData) = 0;
        virtual void onPermission(bool isLogin, const std::string& msg) = 0;
        virtual void onFetchFriends(bool ok, const std::string& msg) = 0;

        /**
         * invitable_friends_and_pagination_json_as_string has the following format:
         *
         */
        /*** INVITE-OUT
        virtual void onRequestInvitableFriends( const FBInvitableFriendsInfo& invitable_friends_and_pagination_json_as_string ) = 0;
        virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& description )= 0;
        virtual void onInviteFriendsResult( bool result, const std::string& description )= 0;
         ***/
    };

    class PluginFacebook
    {
    public:

        /*!
         * initialize the plugin instance.
         */
        static void init();

        /**
         * Set listener to listen for facebook events
         */
        static void setListener(FacebookListener* listener);

        /**
         * Get the listener
         */
        static FacebookListener* getListener();

        /**
         * Remove the listener, and can't listen to events anymore
         */
        static void removeListener();

        /**
         * @brief log in
         */
        static void login();

        /**
         * @brief log in with specific read permissions, conflict with publish permissions
         * https://developers.facebook.com/docs/facebook-login/permissions
         *
         * @param read permissions
         */
        static void requestReadPermissions(const std::vector<std::string>& permissions);

        /**
         * @brief log in with specific public permissions
         * https://developers.facebook.com/docs/facebook-login/permissions
         *
         * @param publish permissions
         */
        static void requestPublishPermissions(const std::vector<std::string>& permissions);

        /**
         * @brief log out
         */
        static void logout();

        /**
         * @brief Check whether the user logined or not
         */
        static bool isLoggedIn();

        /**
         * @brief get UserID
         */
        static std::string getUserID();

        /**
         * @brief get AccessToken
         */
        static std::string getAccessToken();

        /**
         * @brief get permissoin list
         */
        static std::vector<std::string> getPermissionList();

        /**
         * @brief share
         *
         * @param info share information
         */
        static void share(const FBShareInfo& info);

        /**
         * @brief open a dialog of Facebook app or WebDialog (dialog with photo only avaible with native Facebook app)
         *
         * @param info share information
         */
        static void dialog(const FBShareInfo& info);

        /**
         * @brief return the version of Facebook SDK
         */
        static std::string getSDKVersion();

        /**
         * @brief use Facebook Open Graph api
         * https://developers.facebook.com/docs/ios/graph
         *
         * @param path path of Open Graph api
         * @param method HttpMethod ["GET","POST","DELETE"]
         * @param params request parameters
         * @param cb callback of request
         */
        static void api(const std::string& path, const std::string& method, const FBAPIParam& params, const std::string& tag);

        /**
         * @brief fetch friends data from Facebook
         */
        static void fetchFriends();

        /**
         * @brief get friends info
         */
        static std::vector<FBGraphUser> getFriends();

        /**
         * @brief check whether can present Facebook App
         */
        static bool canPresentWithFBApp(const FBShareInfo& info);


        /**
         * Get a vector of invitable friends info.
         */
        /*** INVITE-OUT
        static void requestInvitableFriends( const FBAPIParam& );
        static void inviteFriendsWithInviteIds( const std::vector<std::string>& invite_ids, const std::string& title, const std::string& invite_text );
        static void inviteFriends( const std::string& app_link_url, const std::string& preview_image_url );
         ***/
    };
}

#endif

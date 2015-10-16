//
//  ClientRequest.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/15.
//
//

#ifndef __BrainDotsDebug__ClientRequest__
#define __BrainDotsDebug__ClientRequest__

#include <cocos2d.h>
#include "network/HttpClient.h"
#include <iostream>
#include "cocos-ext.h"

USING_NS_CC;
using namespace network;

class ClientRequest {
public:
    ClientRequest();
    ~ClientRequest();

    void sendRequest(std::string url, std::string fileName);
    void onHttpRequestCompleted(HttpClient* sender, HttpResponse* response);
    
};
#endif /* defined(__BrainDotsDebug__ClientRequest__) */

//
//  ClientRequest.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/10/15.
//
//

#include "ClientRequest.h"

ClientRequest::ClientRequest()
{
    
}

ClientRequest::~ClientRequest()
{
    
}

void ClientRequest::sendRequest(std::string url, std::string fileName)
{
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(ClientRequest::onHttpRequestCompleted, this));
    request->setTag(fileName.c_str());
    
    HttpClient::getInstance()->setTimeoutForRead(10);
    HttpClient::getInstance()->send(request);
    request->release();
}

void ClientRequest::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (!response || !response->isSucceed()) {
        return;
    }
    
    // Dump the data
    std::vector<char>* buffer = response->getResponseData();
    
    // create file
    std::string fileName = response->getHttpRequest()->getTag();
    std::string filePath = FileUtils::getInstance()->getWritablePath() + fileName;
    
    //write file
//    Image* img = new Image();
//    img->initWithImageData((unsigned char*)&(buffer->front()), buffer->size());
//    img->saveToFile(filePath);
    
    FILE *fp = fopen(filePath.c_str(), "wb");
    
    if (! fp) {
        CCLOG("can not create file %s", filePath.c_str());
        return;
    }
    fwrite(buffer->data(), 1, buffer->size(), fp);
    fclose(fp);
}
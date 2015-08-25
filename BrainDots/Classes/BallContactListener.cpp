//
//  BallContact.cpp
//  BrainDots
//
//  Created by Nguyen Minh Hoang on 2015/08/25.
//
//

#include "BallContactListener.h"

BallContactListener::BallContactListener() : _contacts()
{
}

BallContactListener::~BallContactListener()
{
}

void BallContactListener::BeginContact(b2Contact *contact)
{
    BallContact mBallContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(mBallContact);
}

void BallContactListener::EndContact(b2Contact *contact)
{
    BallContact mBallContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<BallContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), mBallContact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }
}

void BallContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    
}

void BallContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    
}
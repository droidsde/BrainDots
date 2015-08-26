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
    const b2Manifold* manifold = contact->GetManifold();
    b2WorldManifold worldManifold;
    b2Vec2 collisionPoint;
    if (manifold->pointCount > 0) {
        contact->GetWorldManifold( &worldManifold );
        collisionPoint = b2Vec2( worldManifold.points[0].x, worldManifold.points[0].y );
    }
    BallContact mBallContact = { contact->GetFixtureA(), contact->GetFixtureB(), collisionPoint };
    _contacts.push_back(mBallContact);
}

void BallContactListener::EndContact(b2Contact *contact)
{
    const b2Manifold* manifold = contact->GetManifold();
    b2WorldManifold worldManifold;
    b2Vec2 collisionPoint;
    
    if (manifold->pointCount > 0) {
        contact->GetWorldManifold( &worldManifold );
        collisionPoint = b2Vec2( worldManifold.points[0].x, worldManifold.points[0].y );
    }
    BallContact mBallContact = { contact->GetFixtureA(), contact->GetFixtureB(), collisionPoint };
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
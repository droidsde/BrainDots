//
//  BallContactListener.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/02.
//
//

#include "BallContactListener.h"

BallContactListener::BallContactListener(b2Fixture* platform) : _contacts()
{
    this->mPlatform = platform;
}

BallContactListener::~BallContactListener()
{
}

void BallContactListener::setFixtureForBall(b2Fixture *fixBallA, b2Fixture *fixBallB)
{
    this->mFixBallA = fixBallA;
    this->mFixBallB = fixBallB;
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
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    if (fixtureA == mPlatform)
    {
        contact->SetTangentSpeed(50.0f);
    }
    
    if (fixtureB == mPlatform)
    {
        contact->SetTangentSpeed(-50.0f);
    }
}

void BallContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
//    b2Fixture* fixtureA = contact->GetFixtureA();
//    b2Fixture* fixtureB = contact->GetFixtureB();
//    
//    if ((fixtureA == mFixBallA || fixtureA == mFixBallB) && fixtureB != mPlatform) {
//        b2Vec2 velocity = fixtureA->GetBody()->GetLinearVelocity();
//        CCLOG("velocity %f %f", velocity.x, velocity.y);
//        fixtureA->GetBody()->ApplyLinearImpulse( b2Vec2(velocity.x+1, velocity.y), fixtureA->GetBody()->GetWorldCenter(), true);
//    }
//    else  if ((fixtureB == mFixBallA || fixtureB == mFixBallB) && fixtureA != mPlatform) {
//        b2Vec2 velocity = fixtureB->GetBody()->GetLinearVelocity();
//        CCLOG("velocity %f %f", velocity.x, velocity.y);
//        fixtureB->GetBody()->ApplyLinearImpulse( b2Vec2(velocity.x+1, velocity.y), fixtureB->GetBody()->GetWorldCenter(), true);
//    }
}
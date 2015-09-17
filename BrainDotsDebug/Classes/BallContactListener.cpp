//
//  BallContactListener.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/02.
//
//

#include "BallContactListener.h"

BallContactListener::BallContactListener() : _contacts()
{
}

BallContactListener::~BallContactListener()
{
}

void BallContactListener::setFixtureForBall(b2Fixture *fixBallA, b2Fixture *fixBallB)
{
    this->mFixBallA = fixBallA;
    this->mFixBallB = fixBallB;
}

void BallContactListener::setListConveyorBelt(std::vector<ConveyorBelt> list)
{
    this->mListConveyorBelt = list;
    CCLOG("size ConveyorBelt %zd", mListConveyorBelt.size());
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
    
    // contact with all Conveyor belt fixture
    for (int i = 0; i < mListConveyorBelt.size(); i++) {
        ConveyorBelt conveyorBelt = mListConveyorBelt[i];
        auto vect = b2Vec2(conveyorBelt.tangentSpeed, 0);
        
        if (fixtureA == conveyorBelt.fixture)
        {
//            contact->SetTangentSpeed(conveyorBelt.tangentSpeed);
            fixtureB->GetBody()->ApplyForceToCenter(vect, true);
        }
        else if (fixtureB == conveyorBelt.fixture)
        {
//            contact->SetTangentSpeed(conveyorBelt.tangentSpeed);
            fixtureA->GetBody()->ApplyForceToCenter(vect, true);
        }
    }
}

void BallContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
}
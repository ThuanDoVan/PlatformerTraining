#ifndef __COLLISION_LISTENER_H__
#define __COLLISION_LISTENER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

USING_NS_CC;
using namespace std;

class CollisionListener : public b2ContactListener
{
public:
	CollisionListener();
	~CollisionListener();

	void BeginContact(b2Contact *contact);

	//void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif // __COLLISION_LISTENER_H__

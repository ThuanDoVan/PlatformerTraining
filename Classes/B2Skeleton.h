#ifndef __B2_SKELETON_H__
#define __B2_SKELETON_H__

#include "Global.h"
#include <spine/spine-cocos2dx.h>

using namespace spine;

class B2Skeleton : public SkeletonAnimation
{
public:
	Sprite *boom;

	
	B2Skeleton(string jsonFile, string atlasFile, float scale);

	static B2Skeleton* create(string jsonFile, string atlasFile, float scale);

	// SCREEN_SIZE
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	b2Body *body;
	b2FixtureDef fixtureDef;

	int health;
	float move_vel;
	bool facingRight;

	virtual void initBoxPhysic(b2World *world, Point pos);
	virtual void initCirclePhysic(b2World *world, Point pos);
	virtual void die();
	virtual void changeBodyBitMask(uint16 mask);

};

#endif // __B2_SKELETON_H__
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "B2Skeleton.h"

USING_NS_CC;

using namespace spine;

class Enemy : public B2Skeleton
{
public:
	bool isDie;
	Enemy(string jsonFile, string atlasFile, float scale);
	static Enemy* create(string jsonFile, string atlasFile, float scale);
	Size sizeEnemy;

	virtual void getHit();
	virtual void move();
	virtual void shoot();
	virtual void shoot(Point posOfHuman);
	virtual void die();
	virtual void updateEnemy(float dt);
	virtual void initCirclePhysic(b2World *world, Point pos) override;

//protected:
	virtual bool checkOutScreen(Point posCamera);
	virtual void resetEnemy();
};

#endif // __ENEMY_H__

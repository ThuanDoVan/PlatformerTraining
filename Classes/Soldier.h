#ifndef __SOLDIER_H__
#define __SOLDIER_H__

#include "B2Skeleton.h"
#include "GB2ShapeCache-x.h"
#include "BulletOfHero.h"

USING_NS_CC;

using namespace spine;

enum BulletType {
	Slow,
	Fast,
	Circle,
	Super
};

enum State {
	IDLE,
	IDLE_SHOOT,
	IDLE_SHOOT_UP,
	JUMPING,

	LYING_SHOOT,

	RUNNING_SHOOT,
	RUNNING_SHOOT_DOWN,
	RUNNING_SHOOT_UP,

	DIE
};


class Soldier : public B2Skeleton
{
public:
	BulletType bulletType;

	int canShoot;
	int isNoDie;
	float angle; // goc de ban dan

	CCArray *bulletPool;
	int indexBullet;

	Soldier(string jsonFile, string atlasFile, float scale);
	static Soldier* create(string jsonFile, string atlasFile, float scale);
	bool onGround = false;
	bool isGetOriginX = false;
	bool isOnTheAir = false;
	
	int defense = 0;

	Size sizeSoldier;
	float jump_vel;

	State cur_state;
	State pre_state;

	void move();
	void moveFollow(Point joystickVel);
	void blinkTrans();

	virtual void die(Point posOfCammera);

	virtual void listener();

	virtual void initPhysic(b2World *world, Point pos);

	virtual void idle();
	virtual void idleShoot();
	virtual void idleShootUp();
	virtual void jumping();
	virtual void lyingShoot();
	virtual void runningShoot();
	virtual void runningShootUp();
	virtual void runningShootDown();
	virtual void createPool();
	virtual void shoot(float radian);
	virtual void createBullet(float radian, Point posGun);
	
	virtual void updateHero(float dt);
	void changeBodyBitMask(uint16 mask);

	virtual Point getGunLocation();
};
#endif // __SOLDIER_H__
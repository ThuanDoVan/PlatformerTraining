#ifndef __DYNAMIC_HUMAN_ENEMY_H__
#define __DYNAMIC_HUMAN_ENEMY_H__

#include "Enemy.h"
//#include "Utility.h"

USING_NS_CC;

using namespace spine;

class DynamicHumanEnemy : public Enemy
{
public:

	DynamicHumanEnemy(string jsonFile, string atlasFile, float scale);
	static DynamicHumanEnemy* create(float scale);
	virtual void move();
	virtual void die();
	virtual void updateEnemy(float dt, Point cameraPoint);
//protected:
	virtual bool checkOutScreen(Point posCamera);
	virtual void resetEnemy();
};

#endif // __ENEMY_H__

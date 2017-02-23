#ifndef __BULLET_H__
#define __BULLET_H__


#include "B2Sprite.h"

class Bullet : public B2Sprite
{
public:
	bool isDie;
	Bullet();
	~Bullet();


	static Bullet* create();
	virtual void update(float dt);
	virtual void initPhysic(b2World *world, Point pos);
	virtual void initPhysic(b2World *world, b2Vec2 pos);
	virtual void setAngel(float radian);
};

#endif // __B2_SPRITE_H__
#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "B2Sprite.h"



class Floor : public B2Sprite
{
public:

	static Floor* create();

	float rotate;

	void createLadderUp(Size sizeOfBox, Point pos);
	void createLadderDown(Size sizeOfBox, Point pos);
	void initPhysic(b2World *world, Point pos, b2BodyType type);

};

#endif // __FLOOR_H__

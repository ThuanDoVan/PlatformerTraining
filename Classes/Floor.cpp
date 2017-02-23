#include "Floor.h"




Floor * Floor::create()
{
	Floor *floor = new Floor();
	floor->initWithFile("box/lid.png");
	floor->setTag(TAG_FLOOR);
	return floor;
}

void Floor::createLadderUp(Size sizeOfBox, Point pos)
{
	
	rotate = (PI / 2 - atan2f(sizeOfBox.height, sizeOfBox.width));
	body->SetTransform(body->GetPosition(), PI / 2 - rotate);

	this->setRotation(90 + (rotate / PI) * 180);
}

void Floor::createLadderDown(Size sizeOfBox, Point pos)
{
	
	rotate = (PI - atan2f(sizeOfBox.height, sizeOfBox.width));
	body->SetTransform(body->GetPosition(), rotate);

	this->setRotation(180 - (rotate / PI) * 180);
}

void Floor::initPhysic(b2World * world, Point pos, b2BodyType type)
{
	B2Sprite::initPhysic(world, pos, type);
	b2Fixture *fixture = body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();

	filter.categoryBits = BITMASK_FLOOR;
	filter.maskBits = BITMASK_SOLDIER | BITMASK_ENEMY | BITMASK_ITEM | BITMASK_BOMB_HERO | BITMASK_BOMB_ENEMY | BITMASK_BLINK;
	fixture->SetFilterData(filter);
}



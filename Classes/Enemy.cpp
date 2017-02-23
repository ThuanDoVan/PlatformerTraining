#include "Enemy.h"

Enemy::Enemy(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
}

Enemy * Enemy::create(string jsonFile, string atlasFile, float scale)
{
	Enemy *e = new Enemy(jsonFile, atlasFile, scale);
	return e;
}


// run animation get hit by soldier
void Enemy::getHit()
{
}

// moving object
void Enemy::move()
{
	
}

void Enemy::shoot()
{
}

void Enemy::shoot(Point posOfHuman)
{
}

void Enemy::die()
{
}


void Enemy::updateEnemy(float dt)
{
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeEnemy.height / 2);
	}
	
}

void Enemy::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	b2Fixture *fixture = body->GetFixtureList();
	b2Filter filter= fixture->GetFilterData();

	filter.categoryBits = BITMASK_ENEMY;
	filter.maskBits = BITMASK_SOLDIER | BITMASK_BULLET_HERO | BITMASK_FLOOR | BITMASK_BOMB_HERO;

	fixture->SetFilterData(filter);
}

bool Enemy::checkOutScreen(Point posCamera)
{
	return false;
}

void Enemy::resetEnemy()
{
}

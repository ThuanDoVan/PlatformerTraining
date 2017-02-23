#include "BulletOfHero.h"



BulletOfHero::BulletOfHero()
{
}

BulletOfHero::~BulletOfHero()
{
}

BulletOfHero * BulletOfHero::create()
{
	BulletOfHero *bullet = new BulletOfHero();
	bullet->initWithSpriteFrameName("soldier-bullet.png");
	bullet->setTag(TAG_BULLET_HERO);
	bullet->alpha = PI;
	bullet->type = Type::normal;
	bullet->setScale(bullet->SCREEN_SIZE.height / 20.0f / bullet->getContentSize().height);
	bullet->isDie = false;
	bullet->damage = 1;
	bullet->setScale((Director::getInstance()->getVisibleSize().height / 20) / bullet->getContentSize().height);
	return bullet;
}


void BulletOfHero::update(float dt)
{
	Bullet::update(dt);
	// r = screensize.height , T = 2, 
	//if (body != nullptr) {
	//	//auto vLenght = PI * SCREEN_SIZE.height;
	//	if (type == Type::circle) {
	//		auto vLenght = SCREEN_SIZE.width / 2.25f * PI;
	//		auto tmp = PI / 8;
	//		alpha += tmp;
	//		auto vx = vLenght * cos(alpha) / PTM_RATIO;
	//		auto vy = -vLenght * sin(alpha) / PTM_RATIO *cos(radian);
	//		this->body->SetLinearVelocity(b2Vec2(vx, vy) + b2Vec2(SCREEN_SIZE.width / 1.5f / PTM_RATIO*cosf(radian), SCREEN_SIZE.width / 1.5f / PTM_RATIO*sinf(radian)));
	//		if (alpha > 2 * PI) alpha = 0;
	//	}
	//	
	//}
}


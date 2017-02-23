#include "DynamicHumanEnemy.h"

DynamicHumanEnemy::DynamicHumanEnemy(string jsonFile, string atlasFile, float scale) : Enemy(jsonFile, atlasFile, scale)
{
}



DynamicHumanEnemy * DynamicHumanEnemy::create(float scale)
{
	DynamicHumanEnemy *e = new DynamicHumanEnemy("enemy-soldier/soldier.json", "enemy-soldier/soldier.atlas", scale);
	e->setTag(TAG_ENEMY_SOLDIER);
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0,"running",true);
	e->isDie = false;
	return e;
}

void DynamicHumanEnemy::move()
{
	if (body != nullptr) 
		this->body->SetLinearVelocity(b2Vec2(-move_vel,this->body->GetLinearVelocity().y));
}

void DynamicHumanEnemy::die()
{
	this->setAnimation(0,"die",false);
}


void DynamicHumanEnemy::updateEnemy(float dt, Point cameraPoint)
{
	Enemy::updateEnemy(dt);
	//this->setPositionX(body->GetPosition().x * PTM_RATIO);
	//this->setPositionY(body->GetPosition().y * PTM_RATIO - sizeEnemy.height / 2);
	//if(this->body->Get)
	//if(!isDie)
	move();
	
	shoot();
	if (checkOutScreen(cameraPoint))
		if(body!= nullptr)
			resetEnemy();
	if (isDie) {
		resetEnemy();
		isDie = false;
	}
}

// private function
//20/12
bool DynamicHumanEnemy::checkOutScreen(Point posCamera)
{
	auto screenSize = Director::getInstance()->getVisibleSize();
	if (this->getPosition().y < 0 || this->getPosition().x < (posCamera.x - screenSize.width / 2)) {
		return true;
	}
	return false;
}

void DynamicHumanEnemy::resetEnemy()
{
	//this->setAnimation(0, "die", false);
	////this->body->SetLinearVelocity(b2Vec2(0, 0));
	//this->clearTracks();
	//this->setAnimation(0, "die", false);
	/*this->boom = Sprite::createWithSpriteFrameName("explosion-1.png");
	this->boom->setPosition(0,this->getBoundingBox().size.height/2);
	this->addChild(boom);
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(7);

	for (int i = 2; i < 8; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("explosion-%d.png", i));
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.075f);
	auto animate = Animate::create(animation);
	animate->retain();
	boom->runAction(animate);*/

	auto callFunc = CallFunc::create([&]() {
		this->addAnimation(0, "running", true);
		this->setPosition(INT_MAX, INT_MAX);
	});
	auto callFunc2 = CallFunc::create([&]() {
		this->die();
		if (this->body != nullptr) {
			auto world = this->body->GetWorld();
			//this->body->SetTransform(b2Vec2(INT_MAX / PTM_RATIO, INT_MIN / PTM_RATIO), 0);
			world->DestroyBody(this->body);
			this->body = nullptr;
		}
	});
	
	this->runAction(Spawn::createWithTwoActions(Sequence::create(DelayTime::create(0.5f), callFunc, nullptr),callFunc2));
	
	
	//this->body->SetType(b2_staticBody);
}
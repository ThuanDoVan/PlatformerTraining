#include "StaticHumanEnemy.h"
#include "BulletOfEnemy.h"
#include "AudioEngine.h"


StaticHumanEnemy::StaticHumanEnemy(string jsonFile, string atlasFile, float scale) : Enemy(jsonFile, atlasFile, scale)
{
	checkCanShoot = 110;
	periodShoot = 120;
}



StaticHumanEnemy * StaticHumanEnemy::create(float scale)
{
	StaticHumanEnemy *e = new StaticHumanEnemy("enemy-soldier/soldier.json", "enemy-soldier/soldier.atlas", scale);
	e->setTag(TAG_ENEMY_SOLDIER);
	e->isDie = false;
	e->update(0.0f);
	e->health = 1;
	e->sizeEnemy = e->getBoundingBox().size;
	//e->move_vel = e->SCREEN_SIZE.width / PTM_RATIO / 4.0f;
	e->setScaleX(-1);
	e->facingRight = false;
	e->setAnimation(0, "standing-shoot", false);
	//e->setTag(TAG_ENEMY);
	//e->setVisible(0);
	e->indexBullet = -1;
	return e;
}

void StaticHumanEnemy::shoot(Point posOfHero)
{
	/*auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEYSOUND);
	if (checkSound) {
		experimental::AudioEngine::play2d(SOUND_ENEMY_BULLET);
	}*/

	posOfHero = posOfHero - this->getParent()->getPosition();
	auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(indexBullet);
	bullet->isDie = false;

	
	bullet->initPhysic(this->body->GetWorld(), this->body->GetPosition());
	bullet->setVisible(true);
	indexBullet++;
	if (indexBullet == MAX_BULLET_SOLDIER_ENEMY_POOL) {
		indexBullet = 0;
	}
	if (!checkCanShoot) {
		auto thisToHero = posOfHero - this->getPosition();
		auto tmp = thisToHero.y / thisToHero.x;// vecto toi hero
		//log("Fabs this to hero: %f", fabs(tmp));

		if (fabs(tmp) < 0.5f) {
			if (thisToHero.x < 0) {
				this->setScaleX(-1);
				this->clearTracks();
				this->setAnimation(0, "standing-shoot", false);
				this->setToSetupPose();
				bullet->setAngel(PI);
			}
			else {
				this->setScaleX(1);
				this->clearTracks();
				this->setAnimation(0, "standing-shoot", false);
				this->setToSetupPose();
				bullet->setAngel(0);
			}
		}
		else if (fabs(tmp) >= 0.5f && fabs(tmp) <= 2.0f) {
			if (thisToHero.x < 0) {
				if (thisToHero.y < 0) {
					this->setScaleX(-1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45down", false);
					this->setToSetupPose();
					bullet->setAngel(PI * 5 / 4);
				}
				else {
					this->setScaleX(-1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45up", false);
					this->setToSetupPose();
					bullet->setAngel(PI * 3 / 4);
				}
			}
			else {
				if (thisToHero.y < 0) {
					this->setScaleX(1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45down", false);
					this->setToSetupPose();
					bullet->setAngel(-PI / 4);
				}
				else {
					this->setScaleX(1);
					this->clearTracks();
					this->setAnimation(0, "standing-shoot-45up", false);
					this->setToSetupPose();
					bullet->setAngel(PI / 4);
				}
			}
		}
		else if (fabs(tmp) > 2.0f) {
			if (thisToHero.y > 0) {
				this->clearTracks();
				this->setAnimation(0, "standing-shoot-up", false);
				this->setToSetupPose();
				bullet->setAngel(PI / 2);
			}
			else {
				this->clearTracks();
				this->setAnimation(0, "standing-shoot-down", false);
				this->setToSetupPose();
				bullet->setAngel(-PI / 2);
			}
		}
		/*else if (tmp < 0 && fabs(tmp) > 1 / 2 && fabs(tmp) < 2) {
			this->setScaleX(-1);
			this->clearTracks();
			this->setAnimation(0, "standing-shoot-45up", false);
			checkCanShoot = false;
		}
		else if(fabs(tmp) > 2){
			this->clearTracks();
			this->setAnimation(0, "standing-shoot-up", false);
			checkCanShoot = false;
		}
		else if (tmp > 0 && fabs(tmp) > 1 / 2 && fabs(tmp) < 2) {
			this->setScaleX(1);
			this->clearTracks();
			this->setAnimation(0, "standing-shoot-45up", false);
			checkCanShoot = false;
		}*/
	}

}


void StaticHumanEnemy::die()
{
	//Enemy::die();
	auto world = this->body->GetWorld();
	if (world->IsLocked()) return;
	world->DestroyBody(body);
	this->body = nullptr;
	this->clearTracks();
	this->setAnimation(0, "die", false);
	auto callFunc = CallFunc::create([&]() {
		this->setVisible(false);
		//removeFromParentAndCleanup(true);
	});

	this->runAction((Sequence::create(DelayTime::create(0.5f), callFunc, nullptr)));

}


void StaticHumanEnemy::updateEnemy(float dt, Point cameraPoint, Point posOfHero)
{
	//if (!this->checkOutScreen(cameraPoint)) {
	if (!isDie) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getParent()->getPositionY() - sizeEnemy.height / 2);
		checkCanShoot++;
		if (checkCanShoot == periodShoot) {
			checkCanShoot = 0;
			shoot(posOfHero);
		}
	}
	else {
		if (this->body != nullptr) {
			this->die();
		}
	}
	//}
	//updateBullet(cameraPoint);
}

void StaticHumanEnemy::updateBullet(Point cameraPoint)
{
	//log("Fuking bug in bullet update");
	if (this->indexBullet >= 0) {
		for (int i = 0; i < bulletPool->count(); i++) {
			auto bullet = (BulletOfEnemy*)bulletPool->getObjectAtIndex(i);
			if (bullet->checkOutOfScreen(cameraPoint)) {
				bullet->isDie = true;
				//log("Fk bug1");
			}
			if (bullet->isDie) {
				//log("Fk bug2");
				/*	bullet->body->SetTransform(b2Vec2(INT_MAX, INT_MAX), 0);
				bullet->isDie = false;*/
				auto world = bullet->body->GetWorld();
				world->DestroyBody(bullet->body);
				bullet->body = nullptr;
				bullet->setVisible(false);
				bullet->isDie = false;
			}
			bullet->update(0);
		}
	}
}
/*else {
	this->die();
}
}*/


void StaticHumanEnemy::createPool(int count)
{
	bulletPool = CCArray::createWithCapacity(count);
	bulletPool->retain();
	for (int i = 0; i < count; i++) {
		auto bullet = BulletOfEnemy::create();
		bullet->setPosition(INT_MAX, INT_MAX);
		//this->getParent()->addChild(bullet);
		this->getParent()->addChild(bullet, ZORDER_BULLET);
		bullet->fixtureDef.filter.categoryBits = BITMASK_BULLET_ENEMY;
		bullet->fixtureDef.filter.maskBits = BITMASK_SOLDIER;
		//bullet->initPhysic(this->body->GetWorld(), bullet->getPosition());
		bullet->body = nullptr;
		bulletPool->addObject(bullet);

	}
	indexBullet = 0;
}


// private function
//20/12
bool StaticHumanEnemy::checkOutScreen(Point posCamera)
{

	auto screenSize = Director::getInstance()->getVisibleSize();

	if ((this->getPosition().y < 0)) {
		//log("This fucking bug!");
	}

	if ((fabs((this->getPositionX() + this->getParent()->getPositionX()) - posCamera.x) > screenSize.width / 2)) {
		return true;
	}
	return false;
}

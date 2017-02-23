#include "B2Sprite.h"
#include "AudioEngine.h"

B2Sprite::B2Sprite()
{
}

B2Sprite::~B2Sprite()
{
}

B2Sprite * B2Sprite::create()
{
	return nullptr;
}


void B2Sprite::initPhysic(b2World *world, Point pos, b2BodyType type)
{
	//boom = nullptr;
	//------
	b2BodyDef bodyDef;	// defination of body like: shape,...// first
	b2PolygonShape shape;

	auto size = this->getBoundingBox().size;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	fixtureDef.density = 10.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	bodyDef.type = type;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

}

void B2Sprite::update(float dt)
{
	this->setPositionX(body->GetPosition().x * PTM_RATIO);
	this->setPositionY(body->GetPosition().y * PTM_RATIO);
	this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));	// rotate
}

void B2Sprite::explosion()
{
	// phai kiem tra boom = null moi tao vu no 
	//vi co the dan va cham cung luc voi hai body 
	//va tao hai vu no nhung chi giai phong duoc 1

	if (!boom) {
		if (this->getParent() != nullptr) {
			/*auto ref = UserDefault::getInstance()->sharedUserDefault();
			bool checkSound = ref->getBoolForKey(KEYSOUND);
			if (checkSound) {
				experimental::AudioEngine::play2d(SOUND_ENEMY_BOMB_EXPLOSION);
			}*/
			boom = Sprite::createWithSpriteFrameName("explosion-1.png");
			boom->setScale(SCREEN_SIZE.height / 6.0f / boom->getContentSize().height);
			//boom->setPosition(0, this->getBoundingBox().size.height / 2);
			//log("Boom-----------------------");
			boom->setPosition(this->getPosition());
			this->getParent()->addChild(boom, 100);
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
			boom->runAction(animate);

			auto callFunc2 = CallFunc::create([&]() {
				if (boom != nullptr) {
					boom->removeFromParentAndCleanup(true);
					//boom->release();
					boom = nullptr;
					//log("destroy Boom-----------------------");
				}
			});
			//this->runAction(Sequence::create(DelayTime::create(0.5f), callFunc2, nullptr));
			boom->runAction(Sequence::create(animate, callFunc2, nullptr));
		}
	}


}




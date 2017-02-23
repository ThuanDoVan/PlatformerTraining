#include "B2Skeleton.h"


B2Skeleton::B2Skeleton(string jsonFile, string atlasFile, float scale) : SkeletonAnimation(jsonFile, atlasFile, scale)
{

}

B2Skeleton * B2Skeleton::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}


void B2Skeleton::initBoxPhysic(b2World *world, Point pos)
{

	b2PolygonShape box_shape;
	auto size = this->getBoundingBox().size;
	box_shape.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &box_shape;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

}

void B2Skeleton::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set((pos.x) / PTM_RATIO, (pos.y) / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void B2Skeleton::die()
{

	/*boom = Sprite::createWithSpriteFrameName("explosion-1.png");
	boom->setPosition(0, this->getBoundingBox().size.height / 2);
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
	boom->runAction(animate);

	auto callFunc2 = CallFunc::create([&]() {
		boom->removeFromParentAndCleanup(true);
	});

	this->runAction(Sequence::create(DelayTime::create(0.5f), callFunc2, nullptr));*/
}

void B2Skeleton::changeBodyBitMask(uint16 mask)
{
	auto fixture = this->body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();

	filter.categoryBits = mask;
	//and set it back
	fixture->SetFilterData(filter);
}






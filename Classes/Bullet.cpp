#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

Bullet * Bullet::create()
{
	return nullptr;
}

void Bullet::update(float dt)
{
	if (body != nullptr) {
		this->setPosition(Point(this->body->GetPosition().x*PTM_RATIO, this->body->GetPosition().y*PTM_RATIO));
		//this->body->SetTransform(b2Vec2(0, 0), 0);
	}
}

void Bullet::initPhysic(b2World *world, Point pos)
{
	auto size = this->getContentSize();

	//b2PolygonShape shape;
	b2CircleShape circle;
	//shape.SetAsBox(size.width / 4 / PTM_RATIO, size.height/ 4 / PTM_RATIO);
	circle.m_radius = size.width / 4 / PTM_RATIO;

	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(0);
}

void Bullet::initPhysic(b2World * world, b2Vec2 pos)
{
	//b2PolygonShape shape;
	b2CircleShape circle;

	auto size = this->getContentSize();
	//shape.SetAsBox(size.width / 4 / PTM_RATIO, size.height / 4 / PTM_RATIO);
	circle.m_radius = size.width / 4 / PTM_RATIO;


	fixtureDef.density = 0.5f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle;
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData
	
	bodyDef.position.Set(pos.x, pos.y);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(0);
}

void Bullet::setAngel(float radian)
{
	float vx = SCREEN_SIZE.width / 2 / PTM_RATIO * cosf(radian);
	float vy = SCREEN_SIZE.width / 2 / PTM_RATIO * sinf(radian);
	this->body->SetLinearVelocity(b2Vec2(vx, vy));
}

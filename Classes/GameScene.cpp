#include "GameScene.h"
#include "DynamicHumanEnemy.h"
#include "StartScene.h"


USING_NS_CC;

Hud *hud;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	hud = Hud::create();
	auto layer = GameScene::create();
	layer->setTag(TAG_GAME);


	// add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(hud);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	setKeyboardEnabled(true);

	// world
	world = new b2World(b2Vec2(0, -visibleSize.height * 8.0f / 3.0f / PTM_RATIO));

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);


	posGenDEnemy = Point(INT_MAX, INT_MAX);

	follow = Node::create();
	follow->setPosition(origin + visibleSize / 2);
	addChild(follow);

	camera = Follow::create(follow);
	runAction(camera);

	//auto indexMap = UserDefault::getInstance()->sharedUserDefault()->getIntegerForKey(KEY_CHOICE);

	indexOfCurrentMap = 1;
	createBackground();
	createPool();
	originOfLastMap = Point(0, 0);
	createMap(tmxNextMap, originOfLastMap, layNextMap);

	createSoldier(Point(SCREEN_SIZE.width * 0.5f, SCREEN_SIZE.height * 0.75f));

	auto world_listener = new CollisionListener();
	world->SetContactListener(world_listener);

	this->schedule([&](float dt) {
		if (posGenDEnemy != Point(INT_MAX, INT_MAX)) {
			if (controlGenMoveEnemy < 3) {
				genDEnemy();
			}
			controlGenMoveEnemy++;
			if (controlGenMoveEnemy >= 10) {
				controlGenMoveEnemy = 0;
			}
			if ((soldier->getPositionX() + SCREEN_SIZE.width / 2) > posGenDEnemy.x) {
				posGenDEnemy = Point(INT_MAX, INT_MAX);
				controlGenMoveEnemy = 0;
			}
		}
	}, 0.5f, "timer");

	this->scheduleUpdate();
	return true;
}

void GameScene::update(float dt)
{
	int positionIterations = 3;		// position
	int velocityIterations = 8;		// velocity

	world->Step(dt, velocityIterations, positionIterations);

	updateSoldier(dt);
	updateStandMan(dt);

	for (int i = 0; i < dEnemyPool->count(); i++) {
		((DynamicHumanEnemy*)dEnemyPool->getObjectAtIndex(i))->updateEnemy(dt, follow->getPosition());
	}


	for (int i = 0; i < existedBullet.size(); i++) {
		auto bullet = (BulletOfHero*)existedBullet[i];
		bullet->update(dt);
		if (fabs(bullet->getPositionX() - follow->getPositionX()) > SCREEN_SIZE.width / 2 ||
			bullet->getPositionY() < 0 || bullet->getPositionY() > SCREEN_SIZE.height
			) {
			bullet->setVisible(false);
			listIndexExist.insert(i);
		}
	}

	if (listIndexExist.size() != 0 && listIndexExist.size() == existedBullet.size()) {
		for (auto i : existedBullet) {
			if (i->body != nullptr)
				world->DestroyBody(i->body);
			i->body = nullptr;

			i->removeFromParentAndCleanup(true);
		}

		existedBullet.clear();
		listIndexExist.clear();
	}

	for (int i = 0; i < soldier->bulletPool->count(); i++) {
		auto bullet = ((BulletOfHero*)soldier->bulletPool->getObjectAtIndex(i));
		bullet->update(dt);
		if (fabs(bullet->getPositionX() - follow->getPositionX()) > SCREEN_SIZE.width / 2) {
			bullet->isDie = true;
		}

		if (bullet->isDie) {
			if (bullet->body != nullptr) {
				//log("Destroy Bullet");
				world->DestroyBody(bullet->body);
				bullet->body = nullptr;
			}

			bullet->setVisible(false);
			bullet->isDie = false;
		}
	}

	
	//if (choiceControl == 0) {
	if (!isDoneGame) {
		controlSneakyJoystick();
		controlSneakyButtonJump();
		controlSneakyButtonFire();

		if (follow->getPositionX() <= soldier->getPositionX())
			follow->setPositionX(soldier->getPositionX());

		background->updatePosition();
	}


	if (isChangeControl) {
		changeControl();
		isChangeControl = false;
		log("Changed");
	}

	loadNextMap();
	freePassedMap();

	if (posGenDEnemy == Point(INT_MAX, INT_MAX)) {
		checkGenDEnemy();
	}


}

void GameScene::updateSoldier(float dt)
{
	if (soldier != nullptr)
		soldier->updateHero(dt);
	else
		return;

	if ((follow->getPositionX() - soldier->getPositionX()) > SCREEN_SIZE.width / 2 - soldier->sizeSoldier.width / 2) {
		soldier->isGetOriginX = true;
	}
	else {
		soldier->isGetOriginX = false;
	}

	if (soldier->getPosition().y < 0) {
		soldier->isNoDie = 0;
		soldier->cur_state = DIE;
	}

	if (soldier->cur_state == State::DIE) {
			soldier->health--;
			//if(soldier->health >= 0)
				soldier->die(follow->getPosition());


	}

}

void GameScene::updateStandMan(float dt)
{
	if (layCurrentMap != nullptr) {
		auto listObj = layCurrentMap->getChildren();
		for (auto e : listObj) {
			if (e->getTag() > 100) {
				auto tmp = (StaticHumanEnemy*)e;
				tmp->updateBullet(follow->getPosition());
				if (!tmp->checkOutScreen(follow->getPosition())) {
					tmp->updateEnemy(dt, follow->getPosition(), soldier->getPosition());
				}
				else {
					if (tmp->getPosition().x + tmp->getParent()->getPosition().x < follow->getPosition().x) {
						if (tmp->body != nullptr)
							tmp->die();
					}

				}
			}
		}
	}

	if (layNextMap != nullptr) {
		auto listObj = layNextMap->getChildren();
		for (auto e : listObj) {
			if (e->getTag() > 100) {
				auto tmp = (StaticHumanEnemy*)e;
				tmp->updateBullet(follow->getPosition());
				if (!tmp->checkOutScreen(follow->getPosition())) {
					tmp->updateEnemy(dt, follow->getPosition(), soldier->getPosition());
				}
				else {
					if (tmp->getPosition().x + tmp->getParent()->getPosition().x < follow->getPosition().x) {
						if (tmp->body != nullptr)
							tmp->die();
					}
				}
			}
		}
	}
}










/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createPool()
{
	// pool of dynamic enemy
	dEnemyPool = CCArray::createWithCapacity(8);
	dEnemyPool->retain();
	for (int i = 0; i < 8; i++) {
		auto enemy = DynamicHumanEnemy::create(SCREEN_SIZE.height / 11.0f / 242.0f);
		enemy->setPosition(INT_MAX, INT_MAX);
		//enemy->initCirclePhysic(world, Point(INT_MAX, INT_MAX));
		enemy->body = nullptr;
		this->addChild(enemy, ZORDER_ENEMY);
		dEnemyPool->addObject(enemy);
	}

	indexDEnemy = 0;
}



void GameScene::genDEnemy()
{
	auto enemy = (DynamicHumanEnemy*)dEnemyPool->objectAtIndex(indexDEnemy);
	enemy->health = 1;
	enemy->setPosition(posGenDEnemy);
	//enemy->body->SetTransform(b2Vec2(posGenDEnemy.x / PTM_RATIO, posGenDEnemy.y / PTM_RATIO), 0);
	//enemy->body->SetType(b2_dynamicBody);
	if (enemy->body != nullptr) {
		world->DestroyBody(enemy->body);
	}
	enemy->initCirclePhysic(world, posGenDEnemy);
	enemy->body->SetFixedRotation(false);
	auto fixture = enemy->body->GetFixtureList();
	fixture->SetFriction(0);

	indexDEnemy++;
	if (indexDEnemy == dEnemyPool->count()) {
		indexDEnemy = 0;
	}
}


void GameScene::checkGenDEnemy()
{
	if (tmxCurrentMap != nullptr) {
		auto groupOfGenemy1 = tmxCurrentMap->getObjectGroup("moveEnemy");

		for (auto e : groupOfGenemy1->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
			origin = layCurrentMap->getPosition() + origin + sizeOfBound;


			if (origin.x - follow->getPosition().x > SCREEN_SIZE.width / 2 && origin.x - follow->getPosition().x < SCREEN_SIZE.width) {
				posGenDEnemy = origin;
				return;
			}
		}
	}
	if (tmxNextMap != nullptr) {
		auto groupOfGenemy2 = tmxNextMap->getObjectGroup("moveEnemy");
		for (auto e : groupOfGenemy2->getObjects()) {
			auto mObject = e.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
			origin = layNextMap->getPosition() + origin + sizeOfBound;


			if (origin.x - follow->getPosition().x > SCREEN_SIZE.width / 2 && origin.x - follow->getPosition().x < SCREEN_SIZE.width) {
				posGenDEnemy = origin;
				return;
			}
		}
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createSoldier(Point pos)
{
	soldier = Soldier::create("soldier/soldier.json", "soldier/soldier.atlas", SCREEN_SIZE.height / 11.0f / 182.0f);
	soldier->listener();
	GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("soldier/soldierShape.plist", soldier->sizeSoldier.height / 251.0f);
	soldier->setPosition(pos);
	addChild(soldier, ZORDER_SOLDIER);
	// ko duoc thay doi thu tu cau lenh nay
	soldier->initPhysic(world, soldier->getPosition());
	soldier->body->SetFixedRotation(true);
	soldier->createPool();
}

void GameScene::createInfiniteNode()
{
	background = InfiniteParallaxNode::create();

	auto bg1_1 = Sprite::create("bg-1.jpg");
	//auto bg1_1 = Sprite::create("bg-4.png");
	bg1_1->setScaleX(SCREEN_SIZE.width / bg1_1->getContentSize().width);
	bg1_1->setScaleY(SCREEN_SIZE.height / bg1_1->getContentSize().height);
	bg1_1->setAnchorPoint(Point(0, 0.5f));

	auto bg1_2 = Sprite::create("bg-1.jpg");
	//auto bg1_2 = Sprite::create("bg-4.png");
	bg1_2->setScaleX(SCREEN_SIZE.width / bg1_2->getContentSize().width);
	bg1_2->setScaleY(SCREEN_SIZE.height / bg1_2->getContentSize().height);
	bg1_2->setAnchorPoint(Point(0, 0.5f));

	auto bg2_1 = Sprite::create("bg-2.png");
	bg2_1->setScaleX(SCREEN_SIZE.width / bg2_1->getContentSize().width);
	bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
	bg2_1->setAnchorPoint(Point(0, 0.5f));

	auto bg2_2 = Sprite::create("bg-2.png");
	bg2_2->setScaleX(SCREEN_SIZE.width / bg2_2->getContentSize().width);
	bg2_2->setScaleY(SCREEN_SIZE.height / bg2_2->getContentSize().height);
	bg2_2->setAnchorPoint(Point(0, 0.5f));

	auto bg3_1 = Sprite::create("bg-3.png");
	bg3_1->setScaleX(SCREEN_SIZE.width / bg3_1->getContentSize().width);
	bg3_1->setScaleY(SCREEN_SIZE.height / bg3_1->getContentSize().height);
	bg3_1->setAnchorPoint(Point(0, 0.5f));

	auto bg3_2 = Sprite::create("bg-3.png");
	bg3_2->setScaleX(SCREEN_SIZE.width / bg3_2->getContentSize().width);
	bg3_2->setScaleY(SCREEN_SIZE.height / bg3_2->getContentSize().height);
	bg3_2->setAnchorPoint(Point(0, 0.5f));


	background->addChild(bg1_1, 0, Vec2(0.1f, 1), Vec2(0, 0));
	background->addChild(bg1_2, 0, Vec2(0.1f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));
	background->addChild(bg2_1, 0, Vec2(0.5f, 1), Vec2(0, 0));
	background->addChild(bg2_2, 0, Vec2(0.5f, 1), Vec2(bg2_1->getBoundingBox().size.width, 0));
	background->addChild(bg3_1, 0, Vec2(1, 1), Vec2(0, 0));
	background->addChild(bg3_2, 0, Vec2(1, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	background->setPosition(Point(0, SCREEN_SIZE.height / 2));
	background->setAnchorPoint(Point(0, 0.5f));
	this->addChild(background, ZORDER_BG_1);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createBackground()
{
	createInfiniteNode();

	tmxNextMap = TMXTiledMap::create("map" + StringUtils::toString(indexOfCurrentMap) + ".tmx");
	tmxNextMap->setAnchorPoint(Vec2(0, 0));

	scaleOfMap = SCREEN_SIZE.height / tmxNextMap->getContentSize().height;
	tmxNextMap->setScale(scaleOfMap);
	layNextMap = Layer::create();
	layNextMap->setAnchorPoint(Point(0, 0));
	layNextMap->setContentSize(tmxNextMap->getContentSize()*scaleOfMap);
	this->setPosition(originOfLastMap);
	this->addChild(layNextMap);
	layNextMap->addChild(tmxNextMap);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::createMap(TMXTiledMap *map, Point origin, Layer *layer)
{
	buildFloor(map, layer, scaleOfMap);
	buildLadderUp(map, layer, scaleOfMap);
	buildLadderDown(map, layer, scaleOfMap);

	buildStandEnemy(map, layer, scaleOfMap);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::loadNextMap()
{

	if ((soldier->getPosition().x > (originOfLastMap.x + tmxNextMap->getBoundingBox().size.width - SCREEN_SIZE.width))) {
		
		if(indexOfCurrentMap < 2) {
			Point originOfNextmap = Point(originOfLastMap.x + tmxNextMap->getContentSize().width*scaleOfMap, 0);

			//freePassedMap(originOfLastMap);

			tmxCurrentMap = tmxNextMap;
			layCurrentMap = layNextMap;
			layNextMap = Layer::create();
			layNextMap->setPosition(originOfNextmap);
			this->addChild(layNextMap);
			string nameOfNextMap = "map" + StringUtils::toString(indexOfCurrentMap + 1) + ".tmx";
			tmxNextMap = TMXTiledMap::create(nameOfNextMap);
			tmxNextMap->setVisible(false);
			layNextMap->addChild(tmxNextMap, -100);
			layNextMap->setContentSize(tmxNextMap->getContentSize()*scaleOfMap);

			createMap(tmxNextMap, originOfNextmap, layNextMap);

			indexOfCurrentMap++;
			originOfLastMap = originOfNextmap;
			//log("next map: %d", indexOfCurrentMap);
		}

		
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::freePassedMap()
{
	if ((soldier->getPosition().x > (originOfLastMap.x + SCREEN_SIZE.width) && indexOfCurrentMap < 17)) {
		if (layCurrentMap != nullptr) {
			vector <b2Body*> toDestroy;

			for (auto body = world->GetBodyList(); body; body = body->GetNext()) {
				//log("POS BODY: %f", body->GetPosition().x*PTM_RATIO);
				//log("POS ORIGIN: %f", layNextMap->getPositionX());
				if (body->GetPosition().x*PTM_RATIO < layNextMap->getPositionX()) {
					toDestroy.push_back(body);
				}
			}
			for (auto a : toDestroy) {
				//log("Destroy layout");
				world->DestroyBody(a);
			}
			layCurrentMap->removeAllChildrenWithCleanup(true);
			this->removeChild(layCurrentMap, true);
			//layCurrentMap->removeFromParentAndCleanup(true);
			layCurrentMap = nullptr;
			tmxCurrentMap = nullptr;
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::buildFloor(TMXTiledMap *map, Layer* layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto groupFloor = map->getObjectGroup("floor");
	for (auto e : groupFloor->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		Point pos = Point(origin.x + sizeOfBound.width / 2,
			origin.y + sizeOfBound.height / 2 + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		auto floor = Floor::create();
		floor->setAnchorPoint(Vec2(0.5, 1));
		floor->setScaleX(sizeOfBound.width / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);
		floor->setPosition(pos);

		floor->initPhysic(world, pos + originThisMap, b2_staticBody);
		layer->addChild(floor);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::buildLadderUp(TMXTiledMap *map, Layer* layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto groupLadderUp = map->getObjectGroup("ladder");
	for (auto e : groupLadderUp->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto floor = Floor::create();
		floor->setAnchorPoint(Vec2(0.5, 0));
		floor->setScaleX(Vec2(sizeOfBound).length() / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);

		Point pos = Point(origin.x + sizeOfBound.width / 2,
			origin.y + (sizeOfBound.height - floor->getBoundingBox().size.height) / 2 + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		floor->initPhysic(world, pos + originThisMap, b2_staticBody);
		floor->createLadderUp(sizeOfBound, pos);

		floor->setPosition(pos);
		layer->addChild(floor);

	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::buildLadderDown(TMXTiledMap *map, Layer* layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto groupLadderDown = map->getObjectGroup("ladder2");
	for (auto e : groupLadderDown->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto floor = Floor::create();
		floor->setAnchorPoint(Vec2(0.5, 1));
		floor->setScaleX(Vec2(sizeOfBound).length() / floor->getContentSize().width);
		floor->setScaleY(SCREEN_SIZE.height / 30 / floor->getContentSize().height);

		Point pos = Point(origin.x + sizeOfBound.width / 2,
			origin.y + (sizeOfBound.height - floor->getBoundingBox().size.height) / 2 + SCREEN_SIZE.height / Y_INCREMENT_RATIO);

		floor->initPhysic(world, pos + originThisMap, b2_staticBody);
		floor->createLadderDown(sizeOfBound, pos);

		floor->setPosition(pos);
		layer->addChild(floor);

	}
}

void GameScene::buildStandEnemy(TMXTiledMap * map, Layer * layer, float scale)
{
	auto originThisMap = layer->getPosition();
	auto group = map->getObjectGroup("stand_man");
	for (auto e : group->getObjects()) {
		auto mObject = e.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);

		auto standMan = StaticHumanEnemy::create(SCREEN_SIZE.height / 11.0f / 242.0f);

		Point pos = Point(origin.x, origin.y + SCREEN_SIZE.height / Y_INCREMENT_RATIO);
		standMan->initCirclePhysic(world, pos + originThisMap);
		//and set it back
		//standMan->setTag(TAG_STANDMAN);
		standMan->setPosition(pos);

		layer->addChild(standMan, 3);
		standMan->createPool(MAX_BULLET_SOLDIER_ENEMY_POOL);
	}
}




/************************************************************************/
/* Control joystick                                                     */
/************************************************************************/
void GameScene::controlSneakyJoystick()
{
	float degree = hud->joystick->getDegrees();
	auto joystickVel = hud->joystick->getVelocity();
	if (soldier->isOnTheAir) {
		if (soldier->cur_state != IDLE)
			soldier->cur_state = IDLE;
		soldier->moveFollow(joystickVel);
		return;
	}

	if (hud->joystick->getVelocity() == Vec2::ZERO) {
		//
		if (soldier->onGround) {
			soldier->facingRight ? soldier->angle = 0 : soldier->angle = PI;
			if (soldier->body->GetLinearVelocity().x != 0.0f)
				soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			soldier->cur_state = IDLE;

		}
	}
	else if (degree < 30.0f || degree >= 330.0f) {
		soldier->move();
		soldier->angle = 0;  // goc 0
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 30.0f && degree < 70.0f) {
		soldier->move();
		soldier->angle = PI / 6;
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_UP;
	}
	else if (degree >= 70.0f && degree < 110.0f) {
		soldier->angle = PI / 2;
		if (soldier->body->GetLinearVelocity().x != 0.0f)
			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		if (soldier->onGround)
			soldier->cur_state = IDLE_SHOOT_UP;
	}
	else if (degree >= 110.0f && degree < 150.0f) {

		soldier->move();
		soldier->angle = PI * 5 / 6;
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_UP;
	}
	else if (degree >= 150.0f && degree < 210.0f) {
		soldier->move();
		soldier->angle = PI;
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT;
	}
	else if (degree >= 210.0f && degree < 250.0f) {
		soldier->move();
		soldier->angle = PI * 5 / 4;
		soldier->facingRight = false;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_DOWN;
	}
	else if (degree >= 250.0f && degree < 290.0f) {
		if (soldier->body->GetLinearVelocity().x != 0.0f)
			soldier->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		if (soldier->facingRight) {
			soldier->angle = 0;
		}
		else {
			soldier->angle = PI;
		}

		if (soldier->onGround)
			soldier->cur_state = LYING_SHOOT;
	}
	else if (degree >= 290.0f && degree < 330.0f) {
		soldier->move();
		soldier->angle = -PI / 4;
		soldier->facingRight = true;
		if (soldier->onGround)
			soldier->cur_state = RUNNING_SHOOT_DOWN;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void GameScene::controlSneakyButtonJump()
{
	if (hud->btnJump->getParent()->isVisible() && hud->btnJump->getIsActive() && soldier->onGround) {
		soldier->cur_state = JUMPING;
		soldier->onGround = false;

		soldier->body->SetLinearVelocity(b2Vec2(0.0f, soldier->jump_vel));
	}
}

void GameScene::controlSneakyButtonFire()
{
	if (hud->btnFire->getIsActive()) {
		if (soldier->cur_state == IDLE) {
			soldier->cur_state = IDLE_SHOOT;
		}

		soldier->shoot(soldier->angle);
	}
}
// change once when you hit button ok in ControlSetting
void GameScene::changeControl()
{
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	auto jtXRatio = ref->getFloatForKey(KEYJOYSTICK_X);
	auto jtYRatio = ref->getFloatForKey(KEYJOYSTICK_Y);

	auto jumpXRatio = ref->getFloatForKey(KEYBTNJUMP_X);
	auto jumpYRatio = ref->getFloatForKey(KEYBTNJUMP_Y);

	auto fireXRatio = ref->getFloatForKey(KEYBTNFIRE_X);
	auto fireYRatio = ref->getFloatForKey(KEYBTNFIRE_Y);

	hud->joystick->getParent()->setPosition(jtXRatio, jtYRatio);
	hud->joystick->updateVelocity(Point::ZERO);
	hud->btnJump->getParent()->setPosition(jumpXRatio, jumpYRatio);
	hud->btnFire->getParent()->setPosition(fireXRatio, fireYRatio);
}




void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Director::getInstance()->replaceScene(StartScene::createScene());
	}
}



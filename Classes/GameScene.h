#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__


#include "Hud.h"
#include "Soldier.h"

#include "Enemy.h"
#include "Floor.h"
#include "Item.h"
#include "CollisionListener.h"
#include "DynamicHumanEnemy.h"
#include "StaticHumanEnemy.h"
#include "InfiniteParallaxNode.h"


class GameScene : public cocos2d::Layer
{
public:
	int controlGenMoveEnemy=0;
    static cocos2d::Scene* createScene();

    virtual bool init();

	Mat4 _modelViewMV;
	CustomCommand _customCommand;
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	b2World *world;

	Follow *camera;
	Node* follow;
	InfiniteParallaxNode *background;

	TMXTiledMap *tmxCurrentMap;
	TMXTiledMap *tmxNextMap;
	Point originOfLastMap;
	Layer *layCurrentMap;
	Layer *layNextMap;
	float scaleOfMap;
	int indexOfCurrentMap;

	Soldier *soldier;
	CCArray *dEnemyPool;
	int indexDEnemy;
	Point posGenDEnemy;

	set<int> listIndexExist;
	vector<BulletOfHero *> existedBullet;
	//vector<Item*> items;

	bool isDoneGame = false;
	bool isChangeControl = false;
	int timeOut;

public:
	// main loop in game
	void update(float dt);
	void updateSoldier(float dt);
	void updateStandMan(float dt);

	

	// process background / map for game
	void createInfiniteNode();
	void createBackground();
	void createMap(TMXTiledMap *map, Point origin, Layer *layer);
	void loadNextMap();
	void freePassedMap();

	void buildFloor(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderUp(TMXTiledMap *map, Layer* layer, float scale);
	void buildLadderDown(TMXTiledMap *map, Layer* layer, float scale);
	void buildStandEnemy(TMXTiledMap *map, Layer* layer, float scale);


	// process hero for game
	void createSoldier(Point pos);

	// process enemy for game
	void createPool();
	
	void genDEnemy();
	void checkGenDEnemy();


	// process joystick
	void controlSneakyJoystick();
	void controlSneakyButtonJump();
	void controlSneakyButtonFire();

	/*virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw();*/

	void changeControl();



	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);


    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__

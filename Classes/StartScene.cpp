#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"



Scene* StartScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	experimental::AudioEngine::stopAll();

	// add Start sprite
	auto backGround = Sprite::create("background.png");

	/*log("contensize(%f,%f)", backGround->getContentSize().width, backGround->getContentSize().height);
	log("boundingbox(%f,%f)", backGround->getBoundingBox().size.width, backGround->getBoundingBox().size.height);*/

	backGround->setScaleX(visibleSize.width / backGround->getContentSize().width);
	backGround->setScaleY(visibleSize.height / backGround->getContentSize().height);
	backGround->setPosition(origin + visibleSize / 2);

	/*log("contensize(%f,%f)", backGround->getContentSize().width, backGround->getContentSize().height);
	log("boundingbox(%f,%f)", backGround->getBoundingBox().size.width, backGround->getBoundingBox().size.height);*/
	addChild(backGround);

	// cache frame
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map1.plist");
	Director::getInstance()->getTextureCache()->addImage("box/lid.png");

	ref = UserDefault::getInstance()->sharedUserDefault();
	ref->setIntegerForKey(KEY_CHOICE, 1); ref->flush();

	// sound and effect
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/boss_explosion.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/cannon shoot.mp3");		// touch me
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy bullet.mp3");			// attack
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy_bomb.mp3");		// on me
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy_bomb_explosion.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/F bullet.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/FormatFactoryplane_bomb.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/get item.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/helicopter.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/machine gun.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/missle.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/normal bullet.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/plane_bomb.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/plane_drop.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/S Bullet.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/sound_lose.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tank move.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tank shoot .mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/tank_explosion.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/theme_music.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/transform.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/transform_2.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Win.mp3");
	////
	//CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.75f);
	//CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.75f);


	experimental::AudioEngine::preload("sound/boss_explosion.mp3");
	experimental::AudioEngine::preload("sound/cannon shoot.mp3");		// touch me
	experimental::AudioEngine::preload("sound/enemy bullet.mp3");			// attack
	experimental::AudioEngine::preload("sound/enemy_bomb.mp3");		// on me
	experimental::AudioEngine::preload("sound/enemy_bomb_explosion.mp3");
	experimental::AudioEngine::preload("sound/F bullet.mp3");
	experimental::AudioEngine::preload("sound/FormatFactoryplane_bomb.mp3");
	experimental::AudioEngine::preload("sound/get item.mp3");
	experimental::AudioEngine::preload("sound/helicopter.mp3");
	experimental::AudioEngine::preload("sound/machine gun.mp3");
	experimental::AudioEngine::preload("sound/missle.mp3");
	experimental::AudioEngine::preload("sound/normal bullet.mp3");
	experimental::AudioEngine::preload("sound/plane_bomb.mp3");
	experimental::AudioEngine::preload("sound/plane_drop.mp3");
	experimental::AudioEngine::preload("sound/S Bullet.mp3");
	experimental::AudioEngine::preload("sound/sound_lose.mp3");
	experimental::AudioEngine::preload("sound/tank move.mp3");
	experimental::AudioEngine::preload("sound/tank shoot .mp3");
	experimental::AudioEngine::preload("sound/tank_explosion.mp3");
	experimental::AudioEngine::preload("sound/theme_music.mp3");
	experimental::AudioEngine::preload("sound/transform.mp3");
	experimental::AudioEngine::preload("sound/transform_2.mp3");
	experimental::AudioEngine::preload("sound/Win.mp3");


	auto labelVer = Label::createWithTTF("2017-02-17-Build 1", "fonts/Marker Felt.ttf", 200);
	labelVer->setScale(visibleSize.height / 24.0f / labelVer->getContentSize().height);
	labelVer->setPosition(origin.x + visibleSize.width * 0.90f, origin.y + visibleSize.height * 0.96f);
	addChild(labelVer);


	auto ground = Sprite::createWithSpriteFrameName("ground.png");
	ground->setAnchorPoint(Vec2(0.5, 0));
	ground->setPosition(origin.x + visibleSize.width / 2, origin.y);
	ground->setScale(visibleSize.width / ground->getContentSize().width);
	addChild(ground);

	auto rambo = Sprite::createWithSpriteFrameName("title-ramball.png");
	rambo->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.65f);
	rambo->setScale(visibleSize.width * 0.4f / rambo->getContentSize().width);
	addChild(rambo);

	auto skeleton = SkeletonAnimation::createWithFile("soldier/soldier.json", "soldier/soldier.atlas", visibleSize.height / 5.0f / 182.0f);
	skeleton->setPosition(origin.x + visibleSize.width / 2, origin.y + ground->getBoundingBox().size.height);
	skeleton->addAnimation(0, "running", true);
	addChild(skeleton);

	play = Sprite::createWithSpriteFrameName("btn-play.png");
	play->setScale(visibleSize.height / 5.0f / play->getContentSize().height);
	auto posX = visibleSize.width / 2 + rambo->getBoundingBox().size.width / 2 + play->getBoundingBox().size.width * 0.7f;
	play->setPosition(origin.x + posX, origin.y + visibleSize.height * 0.65f);
	addChild(play);

	/*setting = Sprite::create("send/btn-setting.png");
	setting->setScale(visibleSize.height / 9.0f / setting->getContentSize().height);
	setting->setPosition(origin.x + visibleSize.width * 0.1f, origin.y + visibleSize.height * 0.85f);
	addChild(setting);*/

	

	soundOn = Sprite::createWithSpriteFrameName("btn-sound-on.png");
	soundOff = Sprite::createWithSpriteFrameName("btn-sound-off.png");

	soundOn->setPosition(visibleSize.width * 0.85f, visibleSize.height / 10);
	soundOff->setPosition(visibleSize.width * 0.85f, visibleSize.height / 10);

	this->addChild(soundOn);
	this->addChild(soundOff);
	float scaleOfSound = (visibleSize.height / 11) / soundOn->getContentSize().height;
	soundOn->setScale(scaleOfSound);
	soundOff->setScale(scaleOfSound);

	bool checkSound = ref->getBoolForKey(KEYSOUND, true); 
	if (checkSound) {
		soundOff->setVisible(false);
		backgroudSoundID = experimental::AudioEngine::play2d(SOUND_BACKGROUND, true);
		if (ref->getIntegerForKey(KEY_ID_BG_MUSIC) == NULL) {
			ref->setIntegerForKey(KEY_ID_BG_MUSIC, backgroudSoundID);
		}
	}
	else {
		soundOn->setVisible(false);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StartScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool StartScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (play->getBoundingBox().containsPoint(touch->getLocation())) {
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	/*if (setting->getBoundingBox().containsPoint(touch->getLocation())) {
		Director::getInstance()->replaceScene(ControlSettingScene::createScene());
	}*/

	if (soundOn->getBoundingBox().containsPoint(touch->getLocation())) {
		//if (soundOn->isVisible()) {
		if (ref->getBoolForKey(KEYSOUND, true)) {
			soundOn->setVisible(false);
			soundOff->setVisible(true);
			ref->setBoolForKey(KEYSOUND, false);
			experimental::AudioEngine::stop(backgroudSoundID);
		}
		else {
			soundOff->setVisible(false);
			soundOn->setVisible(true);
			ref->setBoolForKey(KEYSOUND, true);
			backgroudSoundID = experimental::AudioEngine::play2d(SOUND_BACKGROUND, true);
		}
	}

	return false;
}

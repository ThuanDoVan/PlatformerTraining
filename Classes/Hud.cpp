#include "HUD.h"

bool Hud::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	auto ref = UserDefault::getInstance()->sharedUserDefault();
	/*int choice = ref->getIntegerForKey(KEY_SELECTION);*/

	//if (choice == 0) {
	
	auto jtXRatio = ref->getFloatForKey(KEYJOYSTICK_X);
	auto jtYRatio = ref->getFloatForKey(KEYJOYSTICK_Y);

	auto jumpXRatio = ref->getFloatForKey(KEYBTNJUMP_X);
	auto jumpYRatio = ref->getFloatForKey(KEYBTNJUMP_Y);

	auto fireXRatio = ref->getFloatForKey(KEYBTNFIRE_X);
	auto fireYRatio = ref->getFloatForKey(KEYBTNFIRE_Y);

	if (jtXRatio == NULL) {
		jtXRatio = 0.17f * winSize.width;
		jtYRatio = 0.20f * winSize.height;
		
		jumpXRatio = 0.73f * winSize.width;
		jumpYRatio = 0.19f * winSize.height;

		fireXRatio = 0.83f * winSize.width;
		fireYRatio = 0.38f * winSize.height;
	}

	addJoystick(jtXRatio, jtYRatio);
	addJoystickButtonJump(jumpXRatio, jumpYRatio);
	addJoystickButtonFire(fireXRatio, fireYRatio);


	/*} else
		addButton();*/

	return true;
}

void Hud::addJoystick(float px, float py)
{
	auto rect_size = SCREEN_SIZE.height / 2.4f;
	Rect joystickBaseDimensions = Rect(0, 0, rect_size, rect_size);

	SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
	joystickBase->init();
	joystickBase->setAnchorPoint(Vec2::ZERO);
	joystickBase->setPosition(px, py);

	auto joystick_thumb = Sprite::create("send/btn-move.png");
	auto joystick_bg = Sprite::create("send/move-area.png");
	joystickBase->setThumbSprite(joystick_thumb);
	joystickBase->setBackgroundSprite(joystick_bg);
	joystickBase->setScale(SCREEN_SIZE.height / 2.4f / joystick_bg->getBoundingBox().size.height);

	joystick = new SneakyJoystick();
	joystick->initWithRect(joystickBaseDimensions);
	joystick->autorelease();
	joystick->retain();

	joystickBase->setJoystick(joystick);
	addChild(joystickBase);
}

void Hud::addJoystickButtonJump(float px, float py)
{
	auto rect_size = SCREEN_SIZE.height / 6.0f;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/btn-jump.png"); b->setOpacity(140);

	joystickButtonBase->setAnchorPoint(Vec2::ZERO);
	joystickButtonBase->setPosition(px, py);
	joystickButtonBase->setDefaultSprite(b);

	joystickButtonBase->setActivatedSprite(Sprite::create("send/btn-jump.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/btn-jump2.png"));
	joystickButtonBase->setScale(SCREEN_SIZE.height / 6.0f / b->getContentSize().height);

	btnJump = new SneakyButton();
	btnJump->initWithRect(joystickButtonRect);
	btnJump->autorelease();
	btnJump->retain();


	joystickButtonBase->setButton(btnJump);
	addChild(joystickButtonBase);
}

void Hud::addJoystickButtonFire(float px, float py)
{
	auto rect_size = SCREEN_SIZE.height / 6.0f;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/btn-shoot.png"); b->setOpacity(140);

	joystickButtonBase->setAnchorPoint(Vec2::ZERO);
	joystickButtonBase->setPosition(px, py);
	joystickButtonBase->setDefaultSprite(b);

	joystickButtonBase->setActivatedSprite(Sprite::create("send/btn-shoot.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/btn-shoot2.png"));
	joystickButtonBase->setScale(SCREEN_SIZE.height / 6.0f / b->getContentSize().height);

	btnFire = new SneakyButton();
	btnFire->initWithRect(joystickButtonRect);
	btnFire->autorelease();
	btnFire->retain();


	joystickButtonBase->setButton(btnFire);
	addChild(joystickButtonBase);
}




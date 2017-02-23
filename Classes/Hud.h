#ifndef __HUD_H__
#define __HUD_H__

#include "Global.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystickSkinnedBase.h"
//#include "Button.h"



class Hud : public Layer
{
public:

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	SneakyJoystick *joystick;
	SneakyButton *btnJump;
	SneakyButton *btnFire;


	//Button *btnLeft;
	//Button *btnRight;

	void addJoystick(float px, float py);
	void addJoystickButtonJump(float px, float py);
	void addJoystickButtonFire(float px, float py);

	//void addButton();

	CREATE_FUNC(Hud);
};

#endif // __HUD_H__
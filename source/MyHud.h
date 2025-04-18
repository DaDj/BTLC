#pragma once

#include "CHud.h"
#include <CHudColours.h>
#include "plugin.h"
#include "CTxdStore.h"
#include "CFont.h"
#include "CWorld.h"
#include "CTheScripts.h"
#include "CCamera.h"

enum HUDSHOWSTATE 
{
	STATE_OFF,
	STATE_ON,
	STATE_FADE_IN,
	STATE_FADE_OUT,
	STATE_MOVE_Y //for Money stuff
};


class MyHud : CHud
{
public:
	static int prevMoney;
	static int MoneyDelta;
	static HUDSHOWSTATE MoneyState;
	static int MoneyAlpha;
	static HUDSHOWSTATE MoneyDeltaState;
	static int MoneyDeltaAlpha;
	static int MoneyShowTime;
	static int MoneyFadeTimer;
	static int MoneyMoveTimer;

	static CSprite2d Sprites[6];
	const static  char* SpritesNames[6];
public:
	static void Implement();
	static void Init();
	static void Shutdown();
	static void DrawPlayerInfo();
	static float DrawMoneyInfo();
	static void DrawRadar();
	static void DrawZoneText();
};


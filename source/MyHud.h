#pragma once

#include "CHud.h"
#include <CHudColours.h>
#include "plugin.h"
#include "CTxdStore.h"
#include "CFont.h"
#include "CWorld.h"
#include "CTheScripts.h"
#include "CCamera.h"
#include "CKeyGen.h"
#include "CSprite.h"
#include "math.h"
#include "CMenuManager.h"
#include "CRadar.h"

enum HUDSHOWSTATE 
{
	STATE_OFF,
	STATE_ON,
	STATE_FADE_IN,
	STATE_FADE_OUT,
	STATE_MOVE_Y, //for Money stuff
	STATE_SWITCH
};

constexpr auto NUM_STEPSCIRCLE = 72;


static class MyHud : CHud
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
	static HUDSHOWSTATE WeaponState;
	static int PrevAmmo;
	static int WeaponTimer;
	static HUDSHOWSTATE VehicleNameState;



	static float UnitCircleX[NUM_STEPSCIRCLE];
	static float UnitCircleY[NUM_STEPSCIRCLE];

	static float MoneyPosX;
	static float MoneyPosY;
	static float MoneySizeX;
	static float MoneySizeY;
	static float WeaponAmmoPosX;
	static float WeaponAmmoPosY;
	static float WeaponAmmoSizeX;
	static float WeaponAmmoSizeY;
	static float WeaponIconPosX;
	static float WeaponIconPosY;
	static float WeaponIconSize;
	static float RadarPosX;
	static float RadarPosY;
	static float RadarRadius;
	static float RadarRadiusOut;
	static float ZonePosX;
	static float ZonePosY;
	static float ZoneSizeX;
	static float ZoneSizeY;
	static float VehicleNamePosX;
	static float VehicleNamePosY;
	static float VehicleNameSizeX;
	static float VehicleNameSizeY;
	static float RadarRadiusSmall;;
	static float RadarRadiusBig;
	

	static CSprite2d Sprites[6];
	const static  char* SpritesNames[6];
public:
	static void Implement();
	static void Init();
	static void Shutdown();
	static void InitUnitCircle();
	static void DrawPlayerInfo();
	static float DrawMoneyInfo();
	static void DrawRadar();
	static void DrawZoneText();
	static void DrawVehicleName();
	static void DrawWeaponInfo(float Offset);
	static void DrawWeaponAmmo(float Alpha, float Offset);
	static void DrawWeaponIcon(float Alpha, float Offset);
	static void DrawHealthandArmor();

};


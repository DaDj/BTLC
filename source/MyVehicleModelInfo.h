#pragma once

#include "plugin.h"
#include "CVehicleModelInfo.h"
#include "MyCarFxRender.h"
//#include "MyVehicle.h"

using namespace plugin;

enum LightState
{

	LIGHT_OFF = 0,
	LIGHT_ON = 1,
	LIGHT_BROKEN = 2
};

struct VehicleLightData
{
	LightState HeadlightL = LIGHT_OFF;
	LightState HeadlightR = LIGHT_OFF;
	LightState IndicatorFL = LIGHT_OFF;
	LightState IndicatorFR = LIGHT_OFF;
	LightState IndicatorRL = LIGHT_OFF;
	LightState IndicatorRR = LIGHT_OFF;
	LightState TaillightL = LIGHT_OFF;
	LightState TaillightR = LIGHT_OFF;
	LightState ReverselightL = LIGHT_OFF;
	LightState ReverselightR = LIGHT_OFF;
	LightState BrakelightL = LIGHT_OFF;
	LightState BrakelightR = LIGHT_OFF;
	LightState FogLightFL = LIGHT_OFF;
	LightState FogLightFR = LIGHT_OFF;
	LightState FogLightRL = LIGHT_OFF;
	LightState FogLightRR = LIGHT_OFF;
	bool isIndicatorEmergency = false;
	int IndicatorBlinkTime = 500;
};

enum eVehicleDummy {
	DUMMY_LIGHT_FRONT_MAIN = 0,
	DUMMY_LIGHT_REAR_MAIN = 1,

	DUMMY_LIGHT_FRONT_SECONDARY = 2,
	DUMMY_LIGHT_REAR_SECONDARY = 3,

	DUMMY_SEAT_FRONT = 4,
	DUMMY_SEAT_REAR = 5,

	DUMMY_EXHAUST = 6,
	DUMMY_ENGINE = 7,
	DUMMY_GAS_CAP = 8,
	DUMMY_TRAILER_ATTACH = 9,
	DUMMY_HAND_REST = 10,
	DUMMY_EXHAUST_SECONDARY = 11,
	DUMMY_WING_AIR_TRAIL = 12,
	DUMMY_VEHICLE_GUN = 13,
};

struct tRestoreEntry 
{
	void* Address;
	int value;
};

class MyVehicleModelInfo : CVehicleModelInfo
{
public:

	static RwTexture* ms_pMyLightsTexture;
	static RwTexture* ms_pMyLightsOnTexture;
	static VehicleLightData VehLightStatus;
	static int CurrentDirtLevel;

	static void Implement();
	static RpMaterial* SetEditableMaterialsCB(RpMaterial* material, void* data);
	static void EmptyFindMats(CVehicleModelInfo* modelInfo);
	static void SetDirtShininess(RpMaterial* Material, int Dirtlevel,float MaxShininess, float MinShininess);
	static void SetDirtTextures(RpMaterial* Material, int Dirtlevel);
	//static void SetCarCustomPlate(CVehicleModelInfo* modelInfo, RpClump* clump);
	static void UpdateLightsTextures(CVehicleModelInfo* modelInfo);
	static void FindEditableMaterialList(CVehicleModelInfo* modelInfo, int Dirtlevel);
	static void RemapDirt(CVehicleModelInfo* modelInfo, int DirtLevel);
private:

};

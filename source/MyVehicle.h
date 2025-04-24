#include "plugin.h"
#include "CVehicle.h"
#include "extender/VehicleExtender.h"
using namespace plugin;


enum LightState
{

	LIGHT_OFF = 0,
	LIGHT_ON = 1,
	LIGHT_BROKEN = 2
};



struct VehicleLightData{
	LightState HeadlightL = LIGHT_ON;
	LightState HeadlightR = LIGHT_ON;
	LightState IndicatorFL = LIGHT_ON;
	LightState IndicatorFR = LIGHT_ON;
	LightState IndicatorRL = LIGHT_ON;
	LightState IndicatorRR = LIGHT_ON;
	LightState TaillightL = LIGHT_ON;
	LightState TaillightR= LIGHT_ON;
	LightState ReverselightL = LIGHT_ON;
	LightState ReverselightR = LIGHT_ON;
	LightState BrakelightL = LIGHT_ON;
	LightState BrakelightR = LIGHT_ON;
	LightState FogLightFL = LIGHT_ON;
	LightState FogLightFR = LIGHT_ON;
	LightState FogLightRL = LIGHT_ON;
	LightState FogLightRR = LIGHT_ON;
	bool isIndicatorEmergency = false;
	//unsigned char bDamagedFrontLeft : 1;
	//unsigned char bDamagedFrontRight : 1;
	//unsigned char bDamagedRearLeft : 1;
	//unsigned char bDamagedRearRight : 1;
};


class MyVehicle
{
public:
	VehicleLightData LightData;

	MyVehicle(CVehicle* vehicle)
	{
		LightData.HeadlightL = LIGHT_OFF;
		LightData.HeadlightR = LIGHT_OFF;
		LightData.IndicatorFL = LIGHT_OFF;
		LightData.IndicatorFR = LIGHT_OFF;
		LightData.IndicatorRL = LIGHT_OFF;
		LightData.IndicatorRR = LIGHT_OFF;
		LightData.TaillightL = LIGHT_OFF;
		LightData.TaillightR = LIGHT_OFF;
		LightData.ReverselightL = LIGHT_OFF;
		LightData.ReverselightR = LIGHT_OFF;
		LightData.BrakelightL = LIGHT_OFF;
		LightData.BrakelightR = LIGHT_OFF;
		LightData.FogLightFL = LIGHT_OFF;
		LightData.FogLightFR = LIGHT_OFF;
		LightData.FogLightRL = LIGHT_OFF;
		LightData.FogLightRR = LIGHT_OFF;

		LightData.isIndicatorEmergency = false;

	}

	static void Implement();
private:

};


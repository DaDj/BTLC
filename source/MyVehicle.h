#pragma once
#include "plugin.h"
#include "CVehicle.h"
#include "extender/VehicleExtender.h"
#include "CTxdStore.h"
#include "MyVehicleModelInfo.h"
#include "utils.h"
using namespace plugin;



class MyextVeh
{

public:
	VehicleLightData LightData;

	MyextVeh(CVehicle* vehicle)
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
		LightData.IndicatorBlinkTime = 450 + (rand() % 150);
	}


};




class MyVehicle : CVehicle
{
public:
	static void Implement();
	static void SetIndicatorState(CVehicle* TheVehicle);
	static void DoVehicleLights(CVehicle* TheVehicle);
	static void SetupRender(CVehicle* TheVehicle);

	static CVector2D GetCarPathLinkPosition(CCarPathLinkAddress& address);
	static float GetZAngleForPoint(CVector2D const& point);
private:

};


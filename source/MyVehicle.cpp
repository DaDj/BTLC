#include "MyVehicle.h"
#include "CCamera.h"
#include "CGeneral.h"
#include "MemoryMgr.h"

static VehicleExtendedData<MyextVeh> MyCustomVehicle;


void MyVehicle::Implement()
{
	//Memory::InjectHook(0x5532A9, &MyVehicle::SetupRender, Memory::HookType::Call);
	patch::RedirectCall(0x5532A9, MyVehicle::SetupRender);
	Events::vehicleRenderEvent.before.Add(MyVehicle::DoVehicleLights);
//	patch::RedirectCall(0x6ABCB9, MyVehicle::DoVehicleLights,true);
}

void MyVehicle::SetIndicatorState(CVehicle* TheVehicle)
{
	VehicleLightData& NewLightState = MyCustomVehicle.Get(TheVehicle).LightData;
	NewLightState.IndicatorFL = LIGHT_OFF;
	NewLightState.IndicatorFR = LIGHT_OFF;
	NewLightState.IndicatorRL = LIGHT_OFF;
	NewLightState.IndicatorRR = LIGHT_OFF;
	bool IndicatorLeft = false;
	bool IndicatorRight = false;


	if ((TheVehicle->m_nVehicleSubClass == VEHICLE_AUTOMOBILE || TheVehicle->m_nVehicleSubClass == VEHICLE_BIKE) &&
		(TheVehicle->GetVehicleAppearance() == VEHICLE_APPEARANCE_AUTOMOBILE || TheVehicle->GetVehicleAppearance() == VEHICLE_APPEARANCE_BIKE) &&
		TheVehicle->bEngineOn && TheVehicle->m_fHealth > 0 && !TheVehicle->bIsDrowning && !TheVehicle->m_pAttachedTo)
	{
		if (TheVehicle->m_pDriver && TheVehicle->m_pDriver == FindPlayerPed())
		{
			if (KeyJustPressed('Z'))
				NewLightState.isIndicatorEmergency = !NewLightState.isIndicatorEmergency;
				
		}
		else if (TheVehicle->m_pDriver)
		{
			CVector2D prevPoint = GetCarPathLinkPosition(TheVehicle->m_autoPilot.m_nPreviousPathNodeInfo);
			CVector2D currPoint = GetCarPathLinkPosition(TheVehicle->m_autoPilot.m_nCurrentPathNodeInfo);
			CVector2D nextPoint = GetCarPathLinkPosition(TheVehicle->m_autoPilot.m_nNextPathNodeInfo);
		   

			float angle = GetZAngleForPoint(nextPoint - currPoint) - GetZAngleForPoint(currPoint - prevPoint);
			while (angle < 0.0f) angle += 360.0f;
			while (angle > 360.0f) angle -= 360.0f;

			if (angle >= 30.0f && angle < 180.0f)
				IndicatorLeft = true;
			else if (angle <= 330.0f && angle > 180.0f)
				IndicatorRight = true;

			if (IndicatorLeft == false && IndicatorRight == false)
			{
				if (TheVehicle->m_autoPilot.m_nCurrentLane < TheVehicle->m_autoPilot.m_nCurrentLane)
					IndicatorRight = true;
				else if (TheVehicle->m_autoPilot.m_nCurrentLane < TheVehicle->m_autoPilot.m_nCurrentLane)
					IndicatorLeft = true;
			}
		}
		float RadiusMax = 150.0f;
		if (CTimer::m_snTimeInMilliseconds % (NewLightState.IndicatorBlinkTime *2) < NewLightState.IndicatorBlinkTime &&
			DistanceBetweenPoints(TheCamera.m_vecGameCamPos,TheVehicle->GetPosition()) < RadiusMax)
		{
			if (IndicatorLeft || NewLightState.isIndicatorEmergency)
			{
				NewLightState.IndicatorFL = LIGHT_ON;
				NewLightState.IndicatorRL = LIGHT_ON;
			}	

			if (IndicatorRight || NewLightState.isIndicatorEmergency)
			{
				NewLightState.IndicatorFR = LIGHT_ON;
				NewLightState.IndicatorRR = LIGHT_ON;
			}
		}

	}
}


void MyVehicle::DoVehicleLights(CVehicle* TheVehicle)
{
	//((void(__thiscall*)(CVehicle*, CMatrix&, unsigned int))0x6E1A60)(TheVehicle, matrix, Flags);

	VehicleLightData& NewLightState = MyCustomVehicle.Get(TheVehicle).LightData;
	float VehicleDistance = 0.0f;


	if (CVehicle::ms_forceVehicleLightsOff)
	{
		MyCustomVehicle.Get(TheVehicle).LightData = NewLightState;
		return;
	}

	bool bWantLight = TheVehicle->GetVehicleLightsStatus();

	//CVehicleModelInfo* MyModel = (CVehicleModelInfo*)CModelInfo::ms_modelInfoPtrs[TheVehicle->m_nModelIndex];


	bool bBombAlarm = 0; //noclue							  
	if (TheVehicle->m_nBombOnBoard == 2)    // 1 = Timed // 2 = On ignition// 
		bBombAlarm = 1;

	if (TheVehicle->m_nAlarmState && TheVehicle->m_nAlarmState != -1) //&& (TheVehicle->m_nStatus & 248) != 0x28 
	{
		if (CTimer::m_snTimeInMilliseconds & 256)
			bBombAlarm = 1;
		else
			bBombAlarm = 0;
	}


	bool isFrontLeftDamaged = ((CAutomobile*)TheVehicle)->m_damageManager.GetLightStatus(eLights::LIGHT_FRONT_LEFT);
	bool isFrontRightDamaged = ((CAutomobile*)TheVehicle)->m_damageManager.GetLightStatus(eLights::LIGHT_FRONT_RIGHT);
	bool isRearLeftDamaged = ((CAutomobile*)TheVehicle)->m_damageManager.GetLightStatus(eLights::LIGHT_REAR_LEFT);
	bool isRearRightDamaged = ((CAutomobile*)TheVehicle)->m_damageManager.GetLightStatus(eLights::LIGHT_REAR_RIGHT);


	//Copy standard Lights
	if (bWantLight)
	{
		if (TheVehicle->m_renderLights.m_bLeftFront)
			NewLightState.HeadlightL = LIGHT_ON;
		if (TheVehicle->m_renderLights.m_bRightFront)
			NewLightState.HeadlightR = LIGHT_ON;

		if (TheVehicle->m_renderLights.m_bLeftRear)
			NewLightState.TaillightL = LIGHT_ON;
		if (TheVehicle->m_renderLights.m_bRightRear)
			NewLightState.TaillightR = LIGHT_ON;
	}


	if (TheVehicle->m_pDriver && TheVehicle->m_fBreakPedal > 0.0 && !TheVehicle->bIsHandbrakeOn) //Is Braking?
	{
		NewLightState.BrakelightL = LIGHT_ON;
		NewLightState.BrakelightR = LIGHT_ON;

		if (bWantLight && (TheVehicle->m_renderLights.m_bLeftRear || TheVehicle->m_renderLights.m_bRightRear)) //overwrite the vanilla function which used the same light.
		{
			NewLightState.TaillightL = LIGHT_ON;
			NewLightState.TaillightR = LIGHT_ON;
		}
		else
		{
			NewLightState.TaillightL = LIGHT_OFF;
			NewLightState.TaillightR = LIGHT_OFF;
		}
	}
	else
	{
		NewLightState.BrakelightL = LIGHT_OFF;
		NewLightState.BrakelightR = LIGHT_OFF;
	}


	if (TheVehicle->m_fGasPedal < 0.0) // Is reversing
	{
		NewLightState.ReverselightL = LIGHT_ON;
		NewLightState.ReverselightR = LIGHT_ON;
	}
	else
	{
		NewLightState.ReverselightL = LIGHT_OFF;
		NewLightState.ReverselightR = LIGHT_OFF;
	}

	
	SetIndicatorState(TheVehicle);


	//Set Damage States
	if (isFrontLeftDamaged)
	{
		NewLightState.HeadlightL = LIGHT_BROKEN;
		NewLightState.IndicatorFL = LIGHT_BROKEN;
		NewLightState.FogLightFL = LIGHT_BROKEN;
	}

	if (isFrontRightDamaged)
	{
		NewLightState.HeadlightR = LIGHT_BROKEN;
		NewLightState.IndicatorFR = LIGHT_BROKEN;
		NewLightState.FogLightFR = LIGHT_BROKEN;
	}

	if (isRearLeftDamaged)
	{
		NewLightState.ReverselightL = LIGHT_BROKEN;	
		NewLightState.BrakelightL = LIGHT_BROKEN;
		NewLightState.TaillightL = LIGHT_BROKEN;
		NewLightState.IndicatorRL = LIGHT_BROKEN;
		NewLightState.FogLightRL = LIGHT_BROKEN;
	}
	if (isRearRightDamaged)
	{
		NewLightState.ReverselightR = LIGHT_BROKEN;
		NewLightState.BrakelightR = LIGHT_BROKEN;
		NewLightState.TaillightR = LIGHT_BROKEN;
		NewLightState.IndicatorRR = LIGHT_BROKEN;
		NewLightState.FogLightRR = LIGHT_BROKEN;
	}


	//MyModel->m_pVehicleStruct->m_avDummyPos[DUMMY_LIGHT_FRONT_MAIN]
//TheVehicle->m_renderLights.
}


void MyVehicle::SetupRender(CVehicle* TheVehicle)
{
	CVehicleModelInfo* MyModel = (CVehicleModelInfo*)CModelInfo::ms_modelInfoPtrs[TheVehicle->m_nModelIndex];
	MyVehicleModelInfo::UpdateLightsTextures(MyModel);
	//	MyVehicle::DoVehicleLights(TheVehicle);
	MyVehicleModelInfo::VehLightStatus = MyCustomVehicle.Get(TheVehicle).LightData;
	TheVehicle->SetupRender();
}


CVector2D MyVehicle::GetCarPathLinkPosition(CCarPathLinkAddress& address) {
	if (address.m_nAreaId != -1 && address.m_nCarPathLinkId != -1 && ThePaths.m_pNaviNodes[address.m_nAreaId])
	{
		return CVector2D(static_cast<float>(ThePaths.m_pNaviNodes[address.m_nAreaId][address.m_nCarPathLinkId].m_vecPosn.x) / 8.0f,
			static_cast<float>(ThePaths.m_pNaviNodes[address.m_nAreaId][address.m_nCarPathLinkId].m_vecPosn.y) / 8.0f);
	}
	return CVector2D(0.0f, 0.0f);
}

float MyVehicle::GetZAngleForPoint(CVector2D const& point)
{
	float angle = CGeneral::GetATanOfXY(point.x, point.y) * 57.295776f - 90.0f;
	while (angle < 0.0f) angle += 360.0f;
	return angle;
}




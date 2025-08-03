#include "MyEntity.h"
#include "MemoryMgr.h"

using namespace plugin;

void MyEntity::Implement()
{
	Memory::InjectHook(0x534540, &MyEntity::GetIsOnScreen, Memory::HookType::Jump);	
}

bool MyEntity::GetIsOnScreen()
{
	CVector dst = { 0.0f, 0.0f, 0.0f };
	dst = this->GetBoundCentre();
	
	float radius = this->GetColModel()->m_boundSphere.m_fRadius;


	//We should implement a check for the camerapos and distance to object
	//also better to check versus the camera instead of playerpos!

	if (FindPlayerPed() != nullptr)
	{
		int vehicleType = -1;
		float posZ = FindPlayerPed()->GetPosition().z;
		if (FindPlayerPed()->m_pVehicle != nullptr)
			vehicleType = FindPlayerPed()->m_pVehicle->m_nVehicleSubClass;

		if (posZ > 40.0f && (vehicleType == VEHICLE_HELI || vehicleType == VEHICLE_PLANE))
			radius = radius * 4.0;
		else if (posZ > 450.0f)
			radius = radius * 4.0;
		else
			radius = radius * 1.5;
	}



	if (TheCamera.IsSphereVisible(dst, radius))
	{
		return true;
	}
	else
	{
		return false;
	}

	
}
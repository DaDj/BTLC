#include "MyMisc.h"
using namespace plugin;

void MyMisc::Implement()
{
	Events::gameProcessEvent.after.Add(MyMisc::PatchShadowDrawDistances);

	
	//Allow running on metal surfaces
	patch::Set(0x55E870, 0xC2C03366);
	patch::Set(0x55E874, 0x0004);


	//Disable "To stop Carl..." message.
	patch::Set(0x63E8DF, 0xEB);

	//Disable Replays
	patch::Set(0x460390, 0xC3);
	patch::Set(0x4600F0, 0xC3);
	patch::Set(0x45F050, 0xC3);

	////More precise Rotations for placed objects --> change 0.5f to 0.000009.
		// Fixes that small rotations weren't done properly
	float* Float_001 = (float*)(0x858C14); //0.000009 float number
	patch::Set(0x5381DC, &Float_001);
	patch::Set(0x5381F2, &Float_001);

	////wheel trun speed
	patch::Set(0x871058, 0.1f);


	//// Disable ped vehicles damage when flipped
	patch::Set(0x6A776B, 0xD8DD);
	patch::Nop(0x6A776D, 4);

	//// Disable player vehicle damage when flipped
	patch::Set(0x570E7F, 0xD8DD);
	patch::Nop(0x570E81, 4);

}

void MyMisc::PatchShadowDrawDistances()
{
	//Corona Shadow Distances i.e. Light on ground
	patch::SetFloat(0x6FD3A6, TheCamera.m_fLODDistMultiplier * 80.0f);
	patch::SetFloat(0x6FD44F, TheCamera.m_fLODDistMultiplier * 80.0f);

	//ToDO: 
	//Vehicle Shadows, Pole shadows, Headlightshadows(we may implement that one completely new in vehicle DoLights)
}

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


	//// Motion Blur Alpha reduced
	patch::SetUInt(0x8D5104, 15);

	static float G_ShadowsHeight = 0.07;
	patch::Set(0x709B2D + 2, &G_ShadowsHeight);
	patch::Set(0x709B8C + 2, &G_ShadowsHeight);
	patch::Set(0x709BC5 + 2, &G_ShadowsHeight);
	patch::Set(0x709BF4 + 2, &G_ShadowsHeight);
	patch::Set(0x709C91 + 2, &G_ShadowsHeight);

	patch::Set(0x709E9C + 2, &G_ShadowsHeight);
	patch::Set(0x709EBA + 2, &G_ShadowsHeight);
	patch::Set(0x709ED5 + 2, &G_ShadowsHeight);

	patch::Set(0x70B21F + 2, &G_ShadowsHeight);
	patch::Set(0x70B371 + 2, &G_ShadowsHeight);
	patch::Set(0x70B4CF + 2, &G_ShadowsHeight);
	patch::Set(0x70B633 + 2, &G_ShadowsHeight);

	patch::Set(0x7085A7 + 2, &G_ShadowsHeight);

	patch::SetFloat(0x8CD4F0, 50.0f);

}

void MyMisc::PatchShadowDrawDistances()
{
	//Corona Shadow Distances i.e. Light on ground
	patch::SetFloat(0x6FD3A6, TheCamera.m_fLODDistMultiplier * 80.0f);
	patch::SetFloat(0x6FD44F, TheCamera.m_fLODDistMultiplier * 80.0f);



	//ToDO: 
	//Vehicle Shadows, Pole shadows, Headlightshadows(we may implement that one completely new in vehicle DoLights)
}

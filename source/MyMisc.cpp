#include "MyMisc.h"
using namespace plugin;

void MyMisc::Implement()
{
	Events::initGameEvent.after.Add(MyMisc::PatchShadowDrawDistances);

	Events::gameProcessEvent.after.Add(MyMisc::PatchShadowDrawDistances);
}

void MyMisc::PatchShadowDrawDistances()
{
	//Corona Shadow Distances i.e. Light on ground
	patch::SetFloat(0x6FD3A6, TheCamera.m_fLODDistMultiplier*80.0f);
	patch::SetFloat(0x6FD44F, TheCamera.m_fLODDistMultiplier * 80.0f);

	//ToDO: 
	//Vehicle Shadows, Pole shadows, Headlightshadows(we may implement that one completely new in vehicle DoLights)
}

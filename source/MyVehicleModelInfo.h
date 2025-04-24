#pragma once

#include "plugin.h"
#include "CVehicleModelInfo.h"
#include "MyCarFxRender.h"

using namespace plugin;

struct tRestoreEntry {
	void* Address;
	void* Value;
};

class MyVehicleModelInfo : CVehicleModelInfo
{
public:
	static void Implement();
	static void SetEditableMaterialsCB(RpMaterial* material, void* data);
	//static void SetCarCustomPlate(CVehicleModelInfo* modelInfo, RpClump* clump);
	static void FindEditableMaterialList(CVehicleModelInfo* modelInfo, int Dirtlevel);
	static void RemapDirt(CVehicleModelInfo* modelInfo, int DirtLevel);
private:

};

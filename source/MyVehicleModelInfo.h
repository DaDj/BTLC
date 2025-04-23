#pragma once

#include "plugin.h"
#include "CVehicleModelInfo.h"
#include "MyCarFxRender.h"

using namespace plugin;



class MyVehicleModelInfo : CVehicleModelInfo
{
public:
	static void Implement();
	static void FindEditableMaterialList(CVehicleModelInfo* modelInfo);
	static void RemapDirt(CVehicleModelInfo* modelInfo, int DirtLevel);
private:

};

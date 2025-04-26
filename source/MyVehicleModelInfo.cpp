#include "MyVehicleModelInfo.h"
#include <chrono>
RwObjectNameIdAssocation* CVehicleModelInfo::ms_vehicleDescs = (RwObjectNameIdAssocation*)0x8A7740;



void MyVehicleModelInfo::Implement()
{
	//Patch New Dirt Materials

	patch::RedirectJump(0x5D5DB0, MyVehicleModelInfo::RemapDirt);
	patch::RedirectCall(0x4C9648, &MyVehicleModelInfo::EmptyFindMats);
}

void MyVehicleModelInfo::SetEditableMaterialsCB(RpMaterial* material, void* data)
{
	tRestoreEntry** pEntries = (tRestoreEntry**)data;
	int CarColorID;
	int LightID = -1;
	//int MaterialColor = *(int*)&material->color & 0xFFFFFF;
	CRGBA MaterialColor = material->color;
	int LighttextureType = 0;
	bool IsBroken = false;


	if (ms_pRemapTexture && material && RpMaterialGetTexture(material) && material->texture->name[0] == '#')
	{
		(*pEntries)->Address = &material->texture;
		(*pEntries)->Value = material->texture;
		(*pEntries)++;
		material->texture = ms_pRemapTexture;
	}


}


void MyVehicleModelInfo::EmptyFindMats(CVehicleModelInfo* modelInfo)
{
	//We don't do anything here atm.
}

void MyVehicleModelInfo::FindEditableMaterialList(CVehicleModelInfo* modelInfo, int Dirtlevel)
{
	const int MaxNumDirtMats = 32;
	RpClump* clump = reinterpret_cast<RpClump*>(modelInfo->m_pRwObject);
	RpAtomic* atomic;
	RpGeometry* Geometry;
	RwTexture* texture = nullptr;
	const char* texName = "";

	for (RwLLLink* link = rwLinkListGetFirstLLLink(&clump->atomicList); link != rwLinkListGetTerminator(&clump->atomicList); link = rwLLLinkGetNext(link))
	{
		atomic = rwLLLinkGetData(link, RpAtomic, inClumpLink);
		if (atomic == nullptr)
			break;
		Geometry = atomic->geometry;
		int NumMaterials = Geometry->matList.numMaterials;
		for (int i = 0; i < NumMaterials; i++)
		{
			texture = RpMaterialGetTexture(Geometry->matList.materials[i]);
			texName = RwTextureGetName(texture);
			if (texture && texName)
			{
				if (strcmp(texName, "vehiclegrunge256") == 0)
				RpMaterialSetTexture(Geometry->matList.materials[i],MyCarFxRender::ms_aDirtTextures[Dirtlevel]);

				if (strcmp(texName, "vehicle_genericmud_truck") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_2[Dirtlevel]);

				if (strcmp(texName, "vehiclegrunge_iv") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_3[Dirtlevel]);

				if (strcmp(texName, "generic_glasswindows2") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_4[Dirtlevel]);

				if (strcmp(texName, "tyrewall_dirt_1") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_5[Dirtlevel]);
			}
		}
	}

	texture = nullptr;
	texName = "";
	for (uint32_t i = 0; i < modelInfo->m_pVehicleStruct->m_nNumExtras; i++)
	{
		Geometry = modelInfo->m_pVehicleStruct->m_apExtras[i]->geometry;
		int NumMaterials = Geometry->matList.numMaterials;
		for (int i = 0; i < NumMaterials; i++)
		{
			texture = RpMaterialGetTexture(Geometry->matList.materials[i]);
			texName = RwTextureGetName(texture);
			if (texture && texName)
			{
				if (strcmp(texName, "vehiclegrunge256") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures[Dirtlevel]);

				if (strcmp(texName, "vehicle_genericmud_truck") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_2[Dirtlevel]);

				if (strcmp(texName, "vehiclegrunge_iv") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_3[Dirtlevel]);

				if (strcmp(texName, "generic_glasswindows2") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_4[Dirtlevel]);

				if (strcmp(texName, "tyrewall_dirt_1") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_5[Dirtlevel]);
			}
		}
	}
}

void MyVehicleModelInfo::RemapDirt(CVehicleModelInfo* modelInfo, int DirtLevel)
{
	FindEditableMaterialList(modelInfo, DirtLevel);  //takes only 0.005ms so its fineeeee 
}


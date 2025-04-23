#include "MyVehicleModelInfo.h"



void MyVehicleModelInfo::Implement()
{

	Events::initGameEvent.after += []()
		{
			//Patch New Dirt Materials (includes silents fix for Dirtlevels)
			patch::RedirectJump(0x5D5DB0, MyVehicleModelInfo::RemapDirt);
			patch::RedirectCall(0x4C9648, MyVehicleModelInfo::FindEditableMaterialList);
			patch::Set(0x4C964D, 0x0FEBCE8B);
		};
}

void MyVehicleModelInfo::FindEditableMaterialList(CVehicleModelInfo* modelInfo)
{
	
	const int MaxNumDirtMats = 32;
	RpClump* clump = reinterpret_cast<RpClump*>(modelInfo->m_pRwObject);

	RpAtomic* atomic;
	RpGeometry* Geometry;

	int Idx = 0;

	for (RwLLLink* link = rwLinkListGetFirstLLLink(&clump->atomicList); link != rwLinkListGetTerminator(&clump->atomicList); link = rwLLLinkGetNext(link))
	{
		atomic = rwLLLinkGetData(link, RpAtomic, inClumpLink);
		if (atomic == nullptr)
			break;
		Geometry = atomic->geometry;
		int NumMaterials = Geometry->matList.numMaterials;
		for (int i = 0; i < NumMaterials; i++)
		{
			if (RwTexture* texture = RpMaterialGetTexture(Geometry->matList.materials[i]))
			{
				if (const char* texName = RwTextureGetName(texture))
				{
					if (strcmp(texName, "vehiclegrunge256") == 0 || strcmp(texName, "vehicle_genericmud_truck") == 0 || strcmp(texName, "vehiclegrunge_iv") == 0)
					{
						if (Idx <= MaxNumDirtMats)
						{
							modelInfo->m_apDirtMaterials[Idx] = (Geometry->matList.materials[i]);
							Idx++;
						}	
					}
				}
			}
		}
	}

	for (uint32_t i = 0; i < modelInfo->m_pVehicleStruct->m_nNumExtras; i++)
	{
		Geometry = modelInfo->m_pVehicleStruct->m_apExtras[i]->geometry;
		int NumMaterials = Geometry->matList.numMaterials;
		for (int i = 0; i < NumMaterials; i++)
		{
			if (RwTexture* texture = RpMaterialGetTexture(Geometry->matList.materials[i]))
			{
				if (const char* texName = RwTextureGetName(texture))
				{
					if (strcmp(texName, "vehiclegrunge256") == 0 || strcmp(texName, "vehicle_genericmud_truck") == 0 || strcmp(texName, "vehiclegrunge_iv") == 0)
					{
						if (Idx <= MaxNumDirtMats)
						{
							modelInfo->m_apDirtMaterials[Idx] = (Geometry->matList.materials[i]);
							Idx++;
						}
					}
				}
			}
		}
	}

	modelInfo->m_nCurrentPrimaryColor = -1;
	modelInfo->m_nCurrentSecondaryColor = -1;
	modelInfo->m_nCurrentTertiaryColor = -1;
	modelInfo->m_nCurrentQuaternaryColor = -1;

}


void MyVehicleModelInfo::RemapDirt(CVehicleModelInfo* modelInfo, int DirtLevel)
{
	for (size_t i = 0; i < 32; i++)
	{
		//RpMaterialSetTexture(materials[i], materials[i]->texture);
		
		if (modelInfo->m_apDirtMaterials[i] == nullptr)
			continue;

		if (strcmp(modelInfo->m_apDirtMaterials[i]->texture->name, "vehicle_genericmud_truck") == 0)
			RpMaterialSetTexture(modelInfo->m_apDirtMaterials[i], MyCarFxRender::ms_aDirtTextures_2[DirtLevel]);

		if (strcmp(modelInfo->m_apDirtMaterials[i]->texture->name, "vehiclegrunge_iv") == 0)
			RpMaterialSetTexture(modelInfo->m_apDirtMaterials[i], MyCarFxRender::ms_aDirtTextures_3[DirtLevel]);

		if (strcmp(modelInfo->m_apDirtMaterials[i]->texture->name, "vehiclegrunge256") == 0)
			RpMaterialSetTexture(modelInfo->m_apDirtMaterials[i], MyCarFxRender::ms_aDirtTextures[DirtLevel]);
		
	}
}


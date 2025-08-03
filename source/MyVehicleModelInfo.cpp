#include "MyVehicleModelInfo.h"

RwObjectNameIdAssocation* CVehicleModelInfo::ms_vehicleDescs = (RwObjectNameIdAssocation*)0x8A7740;


RwTexture* MyVehicleModelInfo::ms_pMyLightsTexture;
RwTexture* MyVehicleModelInfo::ms_pMyLightsOnTexture;
VehicleLightData MyVehicleModelInfo::VehLightStatus;

void MyVehicleModelInfo::Implement()
{

	//Patch New Dirt Materials
	patch::RedirectJump(0x5D5DB0, MyVehicleModelInfo::RemapDirt);
	patch::RedirectCall(0x4C9648, &MyVehicleModelInfo::EmptyFindMats);
	//patch::RedirectCall(0x4C8414, &MyVehicleModelInfo::SetEditableMaterialsCB);
	patch::RedirectJump(0x4C8220, &MyVehicleModelInfo::SetEditableMaterialsCB);
}

void MyVehicleModelInfo::UpdateLightsTextures(CVehicleModelInfo* modelInfo)
{ //Checks if the current txd of the model has a custom light textures and tries to load it.

	TxdDef* txd = CTxdStore::ms_pTxdPool->GetAt(modelInfo->m_nTxdIndex);
	if (txd)
	{
		MyVehicleModelInfo::ms_pMyLightsTexture = RwTexDictionaryFindNamedTexture(txd->m_pRwDictionary, "vehiclelights");
		MyVehicleModelInfo::ms_pMyLightsOnTexture = RwTexDictionaryFindNamedTexture(txd->m_pRwDictionary, "vehiclelights_on");
	}
	else
	{
		MyVehicleModelInfo::ms_pMyLightsTexture = nullptr;
		MyVehicleModelInfo::ms_pMyLightsOnTexture = nullptr;
	}
}

RpMaterial* MyVehicleModelInfo::SetEditableMaterialsCB(RpMaterial* material, void* data)
{
	tRestoreEntry** pEntries = (tRestoreEntry**)data;
	int LightID = -1;
	//int MaterialColor = *(int*)&material->color & 0xFFFFFF;
	CRGBA MaterialColor = material->color;
	CRGBA MaterialColor2 = *(int*)&material->color & 0xFFFFFF;
	CRGBA MColor = material->color;
	int LighttextureType = 0;
	LightState LightStatus = LIGHT_OFF;

	if (ms_pRemapTexture && material && RpMaterialGetTexture(material) && material->texture->name[0] == '#')
	{
		(*pEntries)->Address = &material->texture;
		(*pEntries)->value = *(int*)material->texture;
		(*pEntries)++;
		material->texture = (RwTexture*)ms_pRemapTexture->raster;
	}

	if (material->texture != nullptr && material->texture == ms_pLightsTexture)
		LighttextureType = 1;
	else if (ms_pMyLightsTexture != nullptr && material->texture == ms_pMyLightsTexture)
		LighttextureType = 2;
	else
		LighttextureType = 0;

	
	if (LighttextureType)
	{
		if (MColor.r == 255 && MColor.g == 175 && MColor.b == 0)  //Left Headlight
			LightStatus = VehLightStatus.HeadlightL;

		else if (MColor.r == 0 && MColor.g == 255 && MColor.b == 200) //Right Headlight
			LightStatus = VehLightStatus.HeadlightR;

		else if (MColor.r == 255 && MColor.g == 174 && MColor.b == 0) //Left Foglight
			LightStatus = VehLightStatus.FogLightFL;

		else if (MColor.r == 0 && MColor.g == 255 && MColor.b == 199) //Right Foglight
			LightStatus = VehLightStatus.FogLightFR;

		else if (MColor.r == 183 && MColor.g == 255 && MColor.b == 0) //Left Front Indicator
			LightStatus = VehLightStatus.IndicatorFL;

		else if (MColor.r == 255 && MColor.g == 58 && MColor.b == 0) //Right Front Indicator
			LightStatus = VehLightStatus.IndicatorFR;

		else if (MColor.r == 182 && MColor.g == 255 && MColor.b == 0) //Left Middle Indicator
			LightStatus = VehLightStatus.IndicatorRL; //we don't use middle atm

		else if (MColor.r == 255 && MColor.g == 57 && MColor.b == 0) //Right Middle Indicator
			LightStatus = VehLightStatus.IndicatorRR; //we don't use middle atm

		else if (MColor.r == 181 && MColor.g == 255 && MColor.b == 0) //Left Rear Indicator
			LightStatus = VehLightStatus.IndicatorRL;

		else if (MColor.r == 255 && MColor.g == 56 && MColor.b == 0) //Right Rear Indicator
			LightStatus = VehLightStatus.IndicatorRR;

		else if (MColor.r == 185 && MColor.g == 255 && MColor.b == 0) //Left Taillight
			LightStatus = VehLightStatus.TaillightL;

		else if (MColor.r == 255 && MColor.g == 60 && MColor.b == 0) //Right Taillight
			LightStatus = VehLightStatus.TaillightR;

		else if (MColor.r == 184 && MColor.g == 255 && MColor.b == 0) //Left Brakelight
			LightStatus = VehLightStatus.BrakelightL;

		else if (MColor.r == 255 && MColor.g == 59 && MColor.b == 0) //Right Brakelight
			LightStatus = VehLightStatus.BrakelightR;

		else if (MColor.r == 0 && MColor.g == 255 && MColor.b == 198) //Left ReverseLight
			LightStatus = VehLightStatus.ReverselightL;

		else if (MColor.r == 255 && MColor.g == 173 && MColor.b == 0) //Right ReverseLight
			LightStatus = VehLightStatus.ReverselightR;

		(*pEntries)->Address = &material->color;
		(*pEntries)->value = *(int*)&material->color;
		(*pEntries)++;
		material->color.red = 255;
		material->color.blue = 255;
		material->color.green = 255;

		if (LightStatus == LIGHT_BROKEN && material->color.alpha < 255)
			material->color.alpha = 0;

		if (LightStatus != LIGHT_OFF)
		{
			(*pEntries)->Address = &material->texture;
			(*pEntries)->value = (int)material->texture;
			(*pEntries)++;

			(*pEntries)->Address = &material->surfaceProps.ambient;
			(*pEntries)->value = *(int*)&material->surfaceProps.ambient;
			(*pEntries)++;

			(*pEntries)->Address = &material->surfaceProps.specular;
			(*pEntries)->value = *(int*)&material->surfaceProps.specular;
			(*pEntries)++;

			(*pEntries)->Address = &material->surfaceProps.diffuse;
			(*pEntries)->value = *(int*)&material->surfaceProps.diffuse;
			(*pEntries)++;

			material->surfaceProps.ambient = 16.0;
			material->surfaceProps.specular = 0;
			material->surfaceProps.diffuse = 0;

			if (LighttextureType == 1)
				material->texture = ms_pLightsOnTexture;
			else if (LighttextureType == 2)
				material->texture = ms_pMyLightsOnTexture;

			
		}
	}
	else
	{

		int CarColorID = -1;
		if (MColor.r == 60 && MColor.g == 255 && MColor.b == 0)
			CarColorID = CVehicleModelInfo::ms_currentCol[0];
		else if (MColor.r == 255 && MColor.g == 0 && MColor.b == 175)
			CarColorID = CVehicleModelInfo::ms_currentCol[1];
		else if (MColor.r == 0 && MColor.g == 255 && MColor.b == 255)
			CarColorID = CVehicleModelInfo::ms_currentCol[2];
		else if (MColor.r == 255 && MColor.g == 0 && MColor.b == 255)
			CarColorID = CVehicleModelInfo::ms_currentCol[3];
		else
			return material;

		(*pEntries)->Address = &material->color;
		(*pEntries)->value = *(int*)&material->color;
		(*pEntries)++;
		CRGBA VehColor = CVehicleModelInfo::ms_vehicleColourTable[CarColorID];
		RpMaterialGetColor(material)->red = VehColor.r;
		RpMaterialGetColor(material)->green = VehColor.g;
		RpMaterialGetColor(material)->blue = VehColor.b;

	}
	return material;
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
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures[Dirtlevel]);

				if (strcmp(texName, "vehiclegrunge_iv") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_2[Dirtlevel]);

				if (strcmp(texName, "generic_glasswindows2") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_3[Dirtlevel]);

				if (strcmp(texName, "tyrewall_dirt_1") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_4[Dirtlevel]);

				if (strcmp(texName, "vehicle_generic_detail") == 0)
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

				if (strcmp(texName, "vehiclegrunge_iv") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_2[Dirtlevel]);

				if (strcmp(texName, "generic_glasswindows2") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_3[Dirtlevel]);

				if (strcmp(texName, "tyrewall_dirt_1") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_4[Dirtlevel]);

				if (strcmp(texName, "vehicle_generic_detail") == 0)
					RpMaterialSetTexture(Geometry->matList.materials[i], MyCarFxRender::ms_aDirtTextures_5[Dirtlevel]);
			}
		}
	}
}

void MyVehicleModelInfo::RemapDirt(CVehicleModelInfo* modelInfo, int DirtLevel)
{
	FindEditableMaterialList(modelInfo, DirtLevel);  //takes only 0.005ms so its fineeeee 
}


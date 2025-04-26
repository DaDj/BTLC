#pragma once
#include "plugin.h"
#include "CTxdStore.h"
#include "CClothesBuilder.h"
#include "RWExtend.h"

using namespace plugin;

class MyCarFxRender
{
public:
	//Dirttextures
	static RwTexture** ms_aDirtTextures; //vehiclegrunge256 (vanilla SA)
	static RwTexture* ms_aDirtTextures_2[16];//vehicle_genericmud_truck from IV
	static RwTexture* ms_aDirtTextures_3[16];//unused
	static RwTexture* ms_aDirtTextures_4[16];//unused
	static RwTexture* ms_aDirtTextures_5[16];//unused
	static RwTexture* ms_aDirtTextures_6[16];//unused

	static void Shutdown();
	static void InitialiseDirtTexture();
	static void MyShutdown();
	static void InitialiseDirtTextures();
	
	static void InitialiseBlendTextureSingle(char* CleanName, char* DirtName, RwTexture** TextureArray);
	static void InitialiseDirtTextureSingle(char* name, RwTexture** Array);
	static void Implement();
};
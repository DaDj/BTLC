#include "MyCarFxRender.h"
#include "RenderWare.h"


RwTexture** MyCarFxRender::ms_aDirtTextures = (RwTexture**)0xC02BD0;
RwTexture* MyCarFxRender::ms_aDirtTextures_2[16] = {};
RwTexture* MyCarFxRender::ms_aDirtTextures_3[16] = {};
RwTexture* MyCarFxRender::ms_aDirtTextures_4[16] = {};
RwTexture* MyCarFxRender::ms_aDirtTextures_5[16] = {};
RwTexture* MyCarFxRender::ms_aDirtTextures_6[16] = {};

void MyCarFxRender::Implement()
{
	patch::RedirectCall(0x53CA75, MyCarFxRender::MyShutdown);
	patch::RedirectCall(0x53CA61, MyCarFxRender::MyShutdown);
	patch::RedirectCall(0x5B8FFD, MyCarFxRender::InitialiseDirtTextures);
}

void MyCarFxRender::Shutdown() //org
{
	((void(__cdecl*)())0x5D5AD0)();
}

void MyCarFxRender::InitialiseDirtTexture() //org
{
	((void(__cdecl*)())0x5D5BC0)();
}

void MyCarFxRender::MyShutdown()
{
	Shutdown();
	for (int i = 0; i < 16; i++)
	{
		RwTextureDestroy(ms_aDirtTextures_2[i]);
		RwTextureDestroy(ms_aDirtTextures_3[i]);
		RwTextureDestroy(ms_aDirtTextures_4[i]);
		RwTextureDestroy(ms_aDirtTextures_5[i]);
		//RwTextureDestroy(ms_aDirtTextures_6[i]);
	}
}

void MyCarFxRender::InitialiseBlendTextureSingle(char* CleanName, char* DirtName, RwTexture** TextureArray)
{
	RwTexture* SrcTexture;
	RwTexture* DestTexture;

	int Textureindex = CTxdStore::FindTxdSlot("VEHICLE");
	if (Textureindex == -1)
	{
		Textureindex = CTxdStore::AddTxdSlot("VEHICLE");
		CTxdStore::LoadTxd(Textureindex, "MODELS\\GENERIC\\VEHICLE.TXD");
		CTxdStore::AddRef(Textureindex);
	}
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(Textureindex);
	

	SrcTexture = RwTextureRead(CleanName, 0); //MyRwReadTexture(CleanName, 0);
	SrcTexture->filterAddressing = rwFILTERLINEAR;

	DestTexture = RwTextureRead(DirtName, 0); //MyRwReadTexture(DirtName, 0);
	DestTexture->filterAddressing = rwFILTERLINEAR;

	if (SrcTexture && DestTexture)
	{
		for (size_t i = 0; i < 16; i++)
		{
			float FacB = (1.0/ 15.0) * i;
			float FacA = 1 - FacB;
			TextureArray[i] = CClothesBuilder::CopyTexture(SrcTexture);
			RwTextureSetName(TextureArray[i], CleanName);
			CClothesBuilder::BlendTextures(TextureArray[i], DestTexture, FacA, FacB);
		}
	}
	CTxdStore::PopCurrentTxd();
}

void MyCarFxRender::InitialiseDirtTextureSingle(char* name, RwTexture** dirtTextureArray)
{
	RwTexture* texture;

	int Textureindex = CTxdStore::FindTxdSlot("VEHICLE");
	if (Textureindex == -1)
	{
		Textureindex = CTxdStore::AddTxdSlot("VEHICLE");
		CTxdStore::LoadTxd(Textureindex, "MODELS\\GENERIC\\VEHICLE.TXD");
		CTxdStore::AddRef(Textureindex);
	}
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(Textureindex);
	//texture = RwReadTexture("vehicle_genericmud_truck", 0);
	
	
	
	texture = RwTextureRead(name, 0); //MyRwReadTexture(name, 0);
	texture->filterAddressing = rwFILTERLINEAR;

	for (int texid = 0; texid < 16; texid++)
	{
		dirtTextureArray[texid] = CClothesBuilder::CopyTexture(texture);
		RwTextureSetName(dirtTextureArray[texid], name);

		RwRaster* Dirtraster = dirtTextureArray[texid]->raster;
		RwUInt8* dirtArray = RwRasterLock(Dirtraster, 0, 3);
		RwUInt8* PointertoPixel = dirtArray;
		int width_counter = 0;
		int alpha = (4080 - (texid * 255)) / 16;

		for (int i = 1; i < texture->raster->height; i++)
		{
			PointertoPixel = (dirtArray + width_counter);

			for (int i2 = 0; i2 < texture->raster->width; i2++)
			{
				*(PointertoPixel + 0) = *(PointertoPixel + 0) / 16 * texid + alpha;
				*(PointertoPixel + 1) = *(PointertoPixel + 1) / 16 * texid + alpha;
				*(PointertoPixel + 2) = *(PointertoPixel + 2) / 16 * texid + alpha;
				PointertoPixel += 4;
			}
			width_counter = i * texture->raster->width * 4;
		}
		RwRasterUnlock(Dirtraster);
	}
	CTxdStore::PopCurrentTxd();
}

void MyCarFxRender::InitialiseDirtTextures()
{
	InitialiseDirtTexture();

	//Dirt Textures which blend to white
	InitialiseDirtTextureSingle((char*)"vehiclegrunge_iv", ms_aDirtTextures_2);

	//Textures which belnd between two images
	InitialiseBlendTextureSingle((char*)"generic_glasswindows2", (char*)"generic_glasswindows2_d", ms_aDirtTextures_3);
	InitialiseBlendTextureSingle((char*)"tyrewall_dirt_1", (char*)"tyrewall_dirt_1d", ms_aDirtTextures_4);
	InitialiseBlendTextureSingle((char*)"vehicle_generic_detail", (char*)"vehicle_generic_detaild", ms_aDirtTextures_5);
}
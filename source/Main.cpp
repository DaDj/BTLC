#include "plugin.h"
#include "MyHud.h"
#include "MyRadar.h"
#include "MyObjectData.h"
#include "MyDebug.h"
#include "MyWeather.h"
#include "MyVehicleModelInfo.h"
#include "MyCarFxRender.h"
#include "MyFileMgr.h"
#include "MyVehicle.h"
#include "MyMisc.h"
#include "MyEntity.h"
#include "Fx/MyFxInfoGroundCollide.h"
#pragma warning(disable:26495) 
#pragma warning(disable:26439) 

using namespace plugin;
static char userPath[MAX_PATH];
class BTLC
{
public:
	BTLC()
	{
		CModelInfo::ms_modelInfoPtrs = *(CBaseModelInfo***)0x509CB1;//fla compatiblity --> as far as I understand plugin sdk doesn't support fla?
		CVehicleModelInfo::ms_vehicleColourTable = *(CRGBA**)0x4C8390; //fla compatiblity 
		
	

		MyFileMgr::Implement(); //User Dir inside Gta Folder
		MyHud::Implement(); // New UI
		MyRadar::Implement(); // New UI
		MyObjectData::Implement(); 	//Custom Center of Mass 
		MyWeather::Implement();  //New Weather effects & Always Weather Region San Fierro
		MyEntity::Implement(); //GetIsOnScreen for Entities

		MyVehicle::Implement();
		Events::initRwEvent.after.Add(MyCarFxRender::Implement); //Initialise Dirt textures
		Events::initRwEvent.after.Add(MyVehicleModelInfo::Implement); //Dirt textures , lights ect.

		MyMisc::Implement(); //Small patches and changes 
		MyFxInfoGroundCollide::Implement();
#if _DEBUG
		MyDebug::Implement();
#endif	
	};
} BTLCPlugin;
  
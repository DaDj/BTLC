#include "plugin.h"
#include "MyHud.h"
#include "MyRadar.h"
#include "MyObjectData.h"
#include "MyDebug.h"
#include "MyWeather.h"
#include "MyVehicleModelInfo.h"
#include "MyCarFxRender.h"

#pragma warning(disable:26495) 
#pragma warning(disable:26439) 

using namespace plugin;

class BTLC {
public:
    BTLC() {
		//Events::initRwEvent.Add();
	/*	Events::initRwEvent.after += []()
		{
		};*/
		CModelInfo::ms_modelInfoPtrs = *(CBaseModelInfo***)0x509CB1;//fla compatiblity --> as far as I understand plugin sdk doesn't support fla?

		
		MyHud::Implement(); // New UI
		MyRadar::Implement(); // New UI
		MyObjectData::Implement(); 	//Custom Center of Mass 
		MyWeather::Implement();  //NEw Weather effects & Always Weather Region San Fierro

		
		MyCarFxRender::Implement(); //Initialise Dirt textures
		MyVehicleModelInfo::Implement(); //Dirt textures , lights ect.

		#if _DEBUG
		MyDebug::Implement();
		#endif	
	};
} BTLCPlugin;

#include "plugin.h"
#include "MyHud.h"
#include "MyRadar.h"
#include "MyObjectData.h"

using namespace plugin;

class BTLC {
public:
    BTLC() {
		//Events::initRwEvent.Add();
	/*	Events::initRwEvent.after += []()
		{
		};*/
		MyHud::Implement();
		MyRadar::Implement();
		MyObjectData::Implement();

		CModelInfo::ms_modelInfoPtrs = *(CBaseModelInfo***)0x509CB1;
	};
} BTLCPlugin;

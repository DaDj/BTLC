#include "plugin.h"
#include "MyHud.h"

using namespace plugin;

class BTLC {
public:
    BTLC() {
		//Events::initRwEvent.Add();
		/*Events::initRwEvent.after += []()
		{}*/
		MyHud::Implement();

	};
} BTLCPlugin;

#include "plugin.h"
#include "CWorld.h";
#include "CGeneral.h"
#include "MovementInfo.h"



using namespace plugin;

class FxInterpInfo_c {
public:
    void* vtable;
    unsigned char m_nLoopMode;  // 0: Clamp, 1: Loop, 2: PingPong
    unsigned char m_nNumKeys; //NUM_KEYS: 1 / Timestamps
    unsigned short m_nCount;  //How many different FX_INTERP_DATA:  
    unsigned short* m_pTimes;

  //void GetVal(float* outValues, float delta);
};


class FxInterpInfo32_c : public FxInterpInfo_c {
public:
    unsigned short** m_Keys;
public:
    void GetVal(float* outValues, float delta);
};



// Base Info Header
class FxInfo_c {
public:
    void* vtable;           // 0x00
    unsigned short m_nType;         // 0x04 (eFxInfoType)
    unsigned char m_bTimeModePrt;  // 0x06 (0: Life, 1: Time, etc.)
};


class MyFxInfoGroundCollide : FxInfo_c
{
protected:
    FxInterpInfo32_c m_InterpInfo;
public:
	static void Implement();
	static void __fastcall GetValue(MyFxInfoGroundCollide* thisPtr, int _edx, float currentTime, float mult, float totalTime, float len, bool useConst, MovementInfo_t* info);
};

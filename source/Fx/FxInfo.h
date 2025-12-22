#pragma once



#include "EmissionInfo.h"
#include "MovementInfo.h"
#include "../CFileMgr.h"
#include "eFxInfoType.h"

class  FxInfo_c {
protected:
    void* vtable; // 4 bytes
    eFxInfoType m_nType;
    unsigned char        m_bTimeModeParticle;
    unsigned char _pad[2];      // 0x06 (CRITICAL: Aligns the struct size to 0x8)

public:
    FxInfo_c() = default;    // 0x4A4B30
    virtual ~FxInfo_c() = 0; // 0x4A4940
    virtual void Load(FILESTREAM file, int version) = 0;
    virtual void GetValue(float currentTime, float mult, float totalTime, float length, bool useConst, void* info) = 0;

    friend class FxInfoManager_c;
    friend class FxEmitterBP_c;
};


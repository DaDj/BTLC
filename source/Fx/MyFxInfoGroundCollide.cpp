#include "MyFxInfoGroundCollide.h"
#include "MemoryMgr.h"
#include "utils.h"

void MyFxInfoGroundCollide::Implement()
{
    plugin::patch::RedirectJump(0x4A7100, &MyFxInfoGroundCollide::GetValue);
}


void FxInterpInfo32_c::GetVal(float* outValues, float delta) {
    plugin::CallMethod<0x4A89C0, FxInterpInfo32_c*, float*, float>(this, outValues, delta);

}

void __fastcall MyFxInfoGroundCollide::GetValue(MyFxInfoGroundCollide* thisPtr, int _edx,  float currentTime, float mult, float totalTime, float len, bool useConst, MovementInfo_t* info)
{
    if (!thisPtr->m_bTimeModePrt)
        mult = currentTime / len;  

    MovementInfo_t& movement = *static_cast<MovementInfo_t*>(info);

    CVector origin = movement.m_Pos;
    // previous Z = origin - velocity * dt
    float prevZ = origin.z - movement.m_Vel.z * totalTime;

    // Make sure we pass ProcessVerticalLine a start z >= end z
    float startZ = std::max(origin.z, prevZ);
    float endZ = std::min(origin.z, prevZ);
    CVector start = origin;
    start.z = startZ;

    CColPoint colPoint;
    CEntity* colEntity;
    CStoredCollPoly duemmy;
 
    if (!CWorld::ProcessVerticalLine(start, endZ, colPoint, colEntity, 1, 1, 0, 0, 0, 0, &duemmy))
        return;

    float colZ = colPoint.m_vecPoint.z;

    // Only proceed if we are currently below the collision point and we crossed it this frame
    if (origin.z >= colZ)    // currently at/above ground → nothing to do
        return;
    if (prevZ < colZ)       // previously below ground → didn't cross from above
        return;

    float values[16];
    thisPtr->m_InterpInfo.GetVal(values, mult);

 
   //can read or write
   //int16_t** rawVal = (int16_t**)thisPtr->m_InterpInfo.m_Keys;
   //float VALUE_SCALE = 0.001;
   //float val = rawVal[0][1] * VALUE_SCALE; // Read
   //Write (updates actual memory)
   //rawVal[0][0] = 8000;
   //rawVal[0][2] = 0;

    // values[2] = Randomness factor
    float rndScale = values[2] * totalTime * 5.0f;

    // Generate random direction
    float rx = ((float)(CGeneral::GetRandomNumber() % 10000) / 5000.0f - 1.0f);
    float ry = ((float)(CGeneral::GetRandomNumber() % 10000) / 5000.0f - 1.0f);
    float rz = ((float)(CGeneral::GetRandomNumber() % 10000) / 5000.0f - 1.0f);

    CVector randomDir(rx, ry, rz);
    randomDir *= rndScale; // Use operator*= 

    // 2. Calculate Reflection Physics
    // We need to know how "hard" the particle hits the wall (Scalar projection)
    float impactSpeed = DotProduct(movement.m_Vel, colPoint.m_vecNormal);

    // values[0] = Elasticity (Bounce Factor). 
    // 1.0 = Full bounce. 0.0 = Slide.
    float elasticity = values[0];
    if (elasticity < 0.0f) elasticity = 0.0f; // Safety clamp

    // Reflection Formula: R = V - (1 + e)(V . N)N
    // Calculate the rejection vector (the force pushing back)
    float rejectionFactor = impactSpeed * (1.0f + elasticity);

    CVector rejection = colPoint.m_vecNormal; // Copy normal
    rejection *= rejectionFactor;             // Scale normal by impact force

    // Calculate new velocity
    CVector outVel = movement.m_Vel; // Start with current velocity
    outVel -= rejection;             // Subtract the wall force (operator-=)

    // 3. Apply Randomness
    outVel += randomDir;             // Add random noise (operator+=)

    // 4. Apply Damping (Friction)
    // values[1] = Damping. (e.g. 0.9 preserves 90% speed)
    // If values[1] is 0, the particle stops dead. Ensure your array has non-zero here!
    outVel *= values[1];

    // 5. Apply Results
    movement.m_Pos.z = colZ + 0.05f; // Lift slightly to prevent getting stuck
    movement.m_Vel = outVel;         // Update velocity

   
    return;
}


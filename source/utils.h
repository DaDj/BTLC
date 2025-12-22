#include "plugin.h"
#pragma once


inline bool KeyJustPressed(unsigned int keyCode) {
	return (GetAsyncKeyState(keyCode) & 0x0001) != 0;
}


inline float DotProduct(const CVector& v1, const CVector& v2)
{
	return v1.z * v2.z + v1.y * v2.y + v1.x * v2.x;
}
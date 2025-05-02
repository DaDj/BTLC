#include "plugin.h"
#pragma once


inline bool KeyJustPressed(unsigned int keyCode) {
	return (GetAsyncKeyState(keyCode) & 0x0001) != 0;
}
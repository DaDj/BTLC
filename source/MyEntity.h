#pragma once
#include "plugin.h"
#include "CEntity.h"
#include "CCamera.h"
#include "CPlaceable.h"

class MyEntity : public CEntity
{
public:
	static void Implement();
	 bool GetIsOnScreen();
};
#pragma once
#include "plugin.h"
#include "CRadar.h"
#include "MyHud.h"
#include "CMenuManager.h"
using namespace plugin;
class MyRadar : CRadar
{
public:
	static void Implement();
	static void Init();
	static void Shutdown();
	static void TransformRadarPointToScreenSpace(CVector2D& out, CVector2D& in);
};


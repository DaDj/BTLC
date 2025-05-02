#pragma once
#include "plugin.h"
#include "CWeather.h"
using namespace plugin;

class MyWeather : CWeather
{
public:
	static void Implement();
	static void UpdateWeatherRegion(CVector *Coords);
};
#include "MyWeather.h"

void MyWeather::Implement()
{
	patch::RedirectCall(0x72A7A4, MyWeather::UpdateWeatherRegion);
	patch::RedirectCall(0x72B85E, MyWeather::UpdateWeatherRegion);
	patch::RedirectCall(0x72B8B4, MyWeather::UpdateWeatherRegion);
}

void MyWeather::UpdateWeatherRegion(CVector* Coords)
{
	CWeather::WeatherRegion = WEATHER_REGION_SF;
	return;
}

#include "MyRadar.h"

void MyRadar::Implement()
{
	patch::RedirectJump(0x583480, MyRadar::TransformRadarPointToScreenSpace);
}


void MyRadar::Init()
{
}

void MyRadar::Shutdown()
{
}

void MyRadar::TransformRadarPointToScreenSpace(CVector2D& out, CVector2D& in)
{
	__asm push edx
	if (FrontEndMenuManager.m_bDrawRadarOrMap)
	{
		out.x = FrontEndMenuManager.m_fMapBaseX + FrontEndMenuManager.m_fMapZoom * in.x;
		out.y = FrontEndMenuManager.m_fMapBaseY - FrontEndMenuManager.m_fMapZoom * in.y;
	}
	else
	{
		out.x = SCREEN_COORD(MyHud::RadarRadius * in.x) + SCREEN_COORD(MyHud::RadarPosX + MyHud::RadarRadiusOut);
		out.y = SCREEN_COORD_BOTTOM(MyHud::RadarPosY + MyHud::RadarRadiusOut) - SCREEN_COORD(MyHud::RadarRadius * in.y)  ;
	}
	__asm pop edx
}

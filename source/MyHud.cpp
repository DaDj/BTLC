#include "MyHud.h"

using namespace plugin;

const char* MyHud::SpritesNames[6] = {
 "radarringfront",
  "radarringback",
 "mp_higher",
 "mp_level",
 "mp_lower",
 "hud_target"
};


int MyHud::prevMoney = 0;
int MyHud::MoneyDelta = 0;
HUDSHOWSTATE MyHud::MoneyState = STATE_OFF;
int MyHud::MoneyAlpha = 255;
HUDSHOWSTATE MyHud::MoneyDeltaState = STATE_OFF;
int MyHud::MoneyDeltaAlpha = 0;
int MyHud::MoneyShowTime = 0;
int MyHud::MoneyFadeTimer = 0;
int MyHud::MoneyMoveTimer = 0;
int MyHud::PrevAmmo = 0;
HUDSHOWSTATE MyHud::WeaponState = STATE_OFF;
int MyHud::WeaponTimer = 0;
HUDSHOWSTATE MyHud::VehicleNameState = STATE_OFF;




float MyHud::MoneyPosX = 20.0f;
float MyHud::MoneyPosY = 30.0f;
float MyHud::MoneySizeX = 1.0f;
float MyHud::MoneySizeY = 2.0f;
float MyHud::WeaponAmmoPosX = 20.0f;
float MyHud::WeaponAmmoPosY = 50.0f;
float MyHud::WeaponAmmoSizeX = 1.0f;
float MyHud::WeaponAmmoSizeY = 2.0f;
float MyHud::WeaponIconPosX = 30.0f;
float MyHud::WeaponIconPosY = 120.0f;
float MyHud::WeaponIconSize = 130.0f;
float MyHud::RadarPosX = 90.0f;
float MyHud::RadarPosY = 80.0f;
float MyHud::RadarRadius = 95.0;
float MyHud::RadarRadiusOut = 1.16 * RadarRadius;
float MyHud::RadarRadiusSmall = 95.0f;
float MyHud::RadarRadiusBig = 130.0f;
float MyHud::ZoneSizeX = 1.3f;
float MyHud::ZoneSizeY = 2.5f;
float MyHud::ZonePosX = 50.0f;
float MyHud::ZonePosY = 70.0f;
float MyHud::VehicleNamePosX = 50.0f;
float MyHud::VehicleNamePosY = 130.0f;
float MyHud::VehicleNameSizeX = 1.3f;
float MyHud::VehicleNameSizeY = 2.5f;

float MyHud::UnitCircleX[NUM_STEPSCIRCLE];
float MyHud::UnitCircleY[NUM_STEPSCIRCLE];
CSprite2d MyHud::Sprites[6];


void MyHud::Implement()
{
	patch::RedirectCall(0x58FBD6, MyHud::DrawPlayerInfo);
	patch::RedirectCall(0x5BD76F, MyHud::Init);
	patch::RedirectCall(0x53BBA2, MyHud::Shutdown);
	patch::RedirectCall(0x58D542, MyHud::DrawZoneText);
	patch::RedirectCall(0x58FC53, MyHud::DrawRadar);
	patch::RedirectCall(0x58FBE9, MyHud::DrawVehicleName);
}

void MyHud::Init()
{
	CHud::Initialise();

	int txd = CTxdStore::AddTxdSlot("hud");
	CTxdStore::LoadTxd(txd, "MODELS\\HUD.TXD");
	CTxdStore::AddRef(txd);
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(txd);

	for (int i = 0; i < 6; i++)
		Sprites[i].SetTexture((char*)SpritesNames[i]);

	CTxdStore::PopCurrentTxd();

	InitUnitCircle();
}

void MyHud::Shutdown()
{
	CHud::Shutdown();

	int HUDTxd = CTxdStore::FindTxdSlot("hud");
	CTxdStore::RemoveTxdSlot(HUDTxd);

	for (int i = 0; i < 6; i++)
		Sprites[i].Delete();
}

void MyHud::InitUnitCircle()
{
	float Degree = 360;
	float DegreeStep = 360 / NUM_STEPSCIRCLE;
	int Polycount = NUM_STEPSCIRCLE;

	for (int i = 0; i < Polycount; i++)
	{
		UnitCircleX[i] = sin(Degree * 3.141 / 180);
		UnitCircleY[i] = cos(Degree * 3.141 / 180);
		Degree -= DegreeStep;
	}
}

void MyHud::DrawPlayerInfo()
{
	CPlayerPed* Playa = FindPlayerPed(-1);;

	//We draw the Radarback texture as background for health and Armor
	CRect RadarBackRec = CRect(SCREEN_COORD_LEFT(RadarPosX), SCREEN_COORD_BOTTOM(RadarPosY), SCREEN_COORD_LEFT(RadarPosX + RadarRadiusOut * 2), SCREEN_COORD_BOTTOM(RadarPosY + RadarRadiusOut * 2));
	Sprites[1].Draw(RadarBackRec, CRGBA(255, 255, 255, 255));


	float Offset = DrawMoneyInfo();
	DrawWeaponInfo(Offset);
	DrawHealthandArmor();
}

float MyHud::DrawMoneyInfo()
{
	CPlayerInfo playa = CWorld::Players[0];
	int NewMoney = playa.m_nMoney;
	float PosY = 1;
	int PosMoneyChange = 1;
	char string[40];

	if (NewMoney != prevMoney || KeyPressed(VK_TAB))
	{
		if (NewMoney != prevMoney)
			MoneyDelta = NewMoney - prevMoney;

		if (MoneyState == STATE_ON)
			MoneyShowTime = 0;
		else
		{
			MoneyMoveTimer = 0;
			MoneyState = STATE_MOVE_Y;
			MoneyShowTime = 0;
		}
	}


	switch (MoneyState)
	{
	case STATE_FADE_IN:
		MoneyState = STATE_ON;
		MoneyAlpha = 255;
		PosY = 1;
		break;
	case STATE_FADE_OUT:
		MoneyFadeTimer += CTimer::ms_fTimeStep * 0.02 - 1000.0f;

		if (MoneyFadeTimer < 0)
			MoneyFadeTimer = 0;
		if (MoneyFadeTimer > 600)
			MoneyFadeTimer = 0;

		MoneyAlpha = MoneyFadeTimer / 600.0f * 255.0f;
		if (MoneyAlpha == 0.0f)
		{
			MoneyMoveTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;
			if (MoneyMoveTimer < 0.0)
				MoneyState = STATE_OFF;
			PosY = MoneyMoveTimer / 200.0f * PosMoneyChange;
		}

		break;
	case STATE_ON:
		MoneyShowTime += CTimer::ms_fTimeStep * 0.02 * 1000.0f;
		if (MoneyShowTime > 7000)
		{
			MoneyState = STATE_FADE_OUT;
			MoneyFadeTimer = 600;
			MoneyMoveTimer = 200;
		}
		if (MoneyShowTime > 4000)
			MoneyDelta = 0;

		MoneyAlpha = 255;
		PosY = 1;
		break;
	case STATE_OFF:
		MoneyAlpha = 0;
		PosY = 0.0f;
		break;
	case STATE_MOVE_Y:
		MoneyMoveTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
		if (MoneyMoveTimer > 200)
			MoneyState = STATE_FADE_IN;
		PosY = MoneyMoveTimer / 200.0f * PosMoneyChange;
		MoneyAlpha = 0;
		break;
	default:
		break;
	}

	prevMoney = NewMoney;

	CFont::SetProportional(true);
	CFont::SetBackground(false, false);
	CFont::SetFontStyle(FONT_PRICEDOWN);
	CFont::SetOrientation(ALIGN_RIGHT);
	CFont::SetEdge(1);
	CFont::SetScale(SCREEN_MULTIPLIER(MoneySizeX), SCREEN_MULTIPLIER(MoneySizeY));
	CFont::SetDropColor(CRGBA::CRGBA(30, 30, 30, MoneyAlpha));
	CFont::SetColor(CRGBA::CRGBA(200, 200, 200, MoneyAlpha));
	sprintf(string, "$%d", NewMoney);
	CFont::PrintString(SCREEN_COORD_RIGHT(MoneyPosX), SCREEN_COORD_TOP(MoneyPosY), string);

	if (MoneyDelta != 0)
	{
		PosY += 1;
		if (MoneyDelta > 0)
		{
			sprintf(string, " + $%d", MoneyDelta);
			CFont::SetColor(CRGBA::CRGBA(80, 150, 80, MoneyAlpha));
		}
		else
		{
			sprintf(string, " - $%d", abs(MoneyDelta));
			CFont::SetColor(CRGBA::CRGBA(150, 80, 80, MoneyAlpha));
		}

		float Posy = MoneyPosY + MoneySizeY * 25.0;
		CFont::PrintString(SCREEN_COORD_RIGHT(MoneyPosX), SCREEN_COORD_TOP(Posy), string);
	}

	return PosY;
}

void MyHud::DrawRadar()
{
	if (FrontEndMenuManager.m_nPrefsRadarMode == 2 || (m_ItemToFlash == ITEM_RADAR && CTimer::m_FrameCounter & 8))
		return;

	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwFILTERNEAREST);
	CRadar::DrawMap();

	if (FrontEndMenuManager.m_bPrefsShowHud)
	{
		CRect BackCircle = CRect(SCREEN_COORD_LEFT(RadarPosX), SCREEN_COORD_BOTTOM(RadarPosY), SCREEN_COORD_LEFT(RadarPosX + RadarRadiusOut * 2), SCREEN_COORD_BOTTOM(RadarPosY + RadarRadiusOut * 2));
		Sprites[0].Draw(BackCircle, CRGBA::CRGBA(255, 255, 255, 255));
	}
	if (FrontEndMenuManager.m_nPrefsRadarMode < 2)
		CRadar::DrawBlips();
}

void MyHud::DrawZoneText()
{
	float Fontalpha = 255.0;
	if (m_pZoneName && m_pZoneName != m_pLastZoneName)
	{
		switch (m_ZoneState)
		{
		case STATE_OFF:
			if (!CTheScripts::bPlayerIsOffTheMap && CTheScripts::bDisplayHud)
			{
				m_ZoneState = STATE_FADE_IN;
				m_ZoneNameTimer = 0;
				m_ZoneFadeTimer = 0;
				m_ZoneToPrint = m_pZoneName;
				/*	if (m_VehicleState == 2 || m_VehicleState == 1)
						m_VehicleState = 3;*/
			}
			break;
		case STATE_ON:
			m_ZoneNameTimer = 0;
			m_ZoneFadeTimer = 0;
			m_ZoneToPrint = m_pZoneName;
			break;
		default:
			break;
		}
		m_pLastZoneName = m_ZoneToPrint;
	}

	if (m_ZoneState)
	{
		switch (m_ZoneState)
		{
		case STATE_ON:
			Fontalpha = 255.0;
			CHud::m_ZoneNameTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
			if (m_ZoneNameTimer > 3000.0)
				m_ZoneState = STATE_FADE_OUT;
			break;
		case STATE_FADE_IN:
			if (!TheCamera.GetFading() && TheCamera.GetScreenFadeStatus() != 2)
				m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
			if (m_ZoneFadeTimer > 1000.0)
			{
				m_ZoneFadeTimer = 1000;
				m_ZoneState = STATE_ON;
			}
			Fontalpha = m_ZoneFadeTimer * 0.001 * 255.0;
			break;
		case STATE_FADE_OUT:
			if (!TheCamera.GetFading())
				m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;
			if (m_ZoneFadeTimer <= 0.0)
			{
				m_ZoneFadeTimer = 0;
				m_ZoneState = STATE_OFF;
			}
			Fontalpha = m_ZoneFadeTimer * 0.001 * 255.0;
			break;
		default:
			break;
		}

		//float& unknownA = *(float*)0xBAA3E0;
		//float& unknownB = *(float*)0xBAA3E4;

		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetFontStyle(FONT_MENU);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetEdge(1);
		CFont::SetScale(SCREEN_MULTIPLIER(ZoneSizeX), SCREEN_MULTIPLIER(ZoneSizeY));
		CFont::SetDropColor(CRGBA::CRGBA(30, 30, 30, Fontalpha));
		CFont::SetColor(CRGBA::CRGBA(180, 180, 180, Fontalpha));
		CFont::PrintString(SCREEN_COORD_RIGHT(ZonePosX), SCREEN_COORD_BOTTOM(ZonePosY), m_ZoneToPrint);

	}

}

void MyHud::DrawVehicleName()
{
	float Fontalpha = 255.0;
	if (!m_pVehicleName)
	{
		VehicleNameState = STATE_OFF;
		m_VehicleNameTimer = 0;
		m_VehicleFadeTimer = 0;
		m_pLastVehicleName = nullptr;
		return;
	}

	if (m_pVehicleName != m_pLastVehicleName)
	{
		switch (VehicleNameState)
		{
		case STATE_OFF:
			VehicleNameState = STATE_FADE_IN;
			m_VehicleNameTimer = 0;
			m_VehicleFadeTimer = 0;
			m_pVehicleNameToPrint = m_pVehicleName;
			break;
		case STATE_ON:
		case STATE_FADE_IN:
		case STATE_FADE_OUT:
		case STATE_SWITCH:
			VehicleNameState = STATE_SWITCH;
			m_VehicleNameTimer = 0;
			//_VehicleFadeTimer = 200;
			break;
		default:
			break;
		}
		m_pLastVehicleName = m_pVehicleName;
	}

	switch (VehicleNameState)
	{
	case STATE_OFF:
		Fontalpha = 0;
		break;
	case STATE_ON:
		Fontalpha = 255;
		m_VehicleNameTimer += CTimer::ms_fTimeStep * 0.02 * 1000;
		if (m_VehicleNameTimer > 3000)
		{
			VehicleNameState = STATE_FADE_OUT;
			m_VehicleFadeTimer = 1000;
		}
		break;
	case STATE_FADE_IN:
		m_VehicleFadeTimer += CTimer::ms_fTimeStep * 0.02 * 1000;
		if (m_VehicleFadeTimer > 1000)
		{
			VehicleNameState = STATE_ON;
			m_VehicleNameTimer = 0;
		}
		Fontalpha = m_VehicleFadeTimer * 0.001 * 255.0;
		break;
	case STATE_FADE_OUT:
		m_VehicleFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;
		if (m_VehicleFadeTimer < 0)
		{
			VehicleNameState = STATE_OFF;
			m_VehicleNameTimer = 0;
			m_VehicleFadeTimer = 0;
		}
		Fontalpha = m_VehicleFadeTimer * 0.001 * 255.0;
		break;
	case STATE_SWITCH:
		Fontalpha = 0;
		m_VehicleFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;
		if (m_VehicleFadeTimer < 0)
		{
			VehicleNameState = STATE_FADE_IN;
			m_VehicleNameTimer = 0;
			m_VehicleFadeTimer = 0;
			m_pVehicleNameToPrint = m_pLastVehicleName;
		}
		Fontalpha = m_VehicleFadeTimer * 0.001 * 255.0;

		break;
	default:
		break;
	}

	CFont::SetProportional(true);
	CFont::SetBackground(false, false);
	CFont::SetFontStyle(FONT_MENU);
	CFont::SetOrientation(ALIGN_RIGHT);
	CFont::SetEdge(1);
	CFont::SetScale(SCREEN_MULTIPLIER(VehicleNameSizeX), SCREEN_MULTIPLIER(VehicleNameSizeY));
	CFont::SetDropColor(CRGBA::CRGBA(30, 30, 30, Fontalpha));
	CFont::SetColor(CRGBA::CRGBA(110, 110, 110, Fontalpha));
	CFont::PrintString(SCREEN_COORD_RIGHT(VehicleNamePosX), SCREEN_COORD_BOTTOM(VehicleNamePosY), m_pVehicleNameToPrint);
}

void MyHud::DrawWeaponInfo(float Offset)
{
	CPed* Playa = FindPlayerPed(-1);;
	int Alpha = 0;
	eTaskType ActiveTask = Playa->m_pIntelligence->m_TaskMgr.GetActiveTask()->GetId();
	eCamMode CamMode = TheCamera.m_aCams[TheCamera.m_nActiveCam].m_nMode;

	if (m_LastWeapon != Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_eWeaponType || ActiveTask == TASK_SIMPLE_USE_GUN
		|| Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_nState == 1 || CamMode == MODE_AIMWEAPON || KeyPressed(VK_TAB)
		|| Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_nAmmoTotal != PrevAmmo)
	{
		PrevAmmo = Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_nAmmoTotal;
		m_LastWeapon = Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_eWeaponType;
		WeaponTimer = 0;
		if (WeaponState == STATE_ON)
			WeaponState = STATE_ON;
		else
			WeaponState = STATE_FADE_IN;
	}

	switch (WeaponState)
	{
	case STATE_OFF:
		Alpha = 0;
		break;
	case STATE_ON:
		WeaponTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
		if (WeaponTimer > 5000)
		{
			WeaponState = STATE_FADE_OUT;
			m_WeaponFadeTimer = 600;
		}
		Alpha = 255;
		break;
	case STATE_FADE_IN:
		m_WeaponFadeTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
		Alpha = m_WeaponFadeTimer / 400.0f * 255.0f;
		if (m_WeaponFadeTimer > 400)
		{
			m_WeaponFadeTimer = 0;
			WeaponTimer = 0;
			WeaponState = STATE_ON;
			Alpha = 255;
		}
		break;
	case STATE_FADE_OUT:
		m_WeaponFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;
		if (m_WeaponFadeTimer < 0.0)
		{
			m_WeaponFadeTimer = 0;
			WeaponState = STATE_OFF;
		}
		Alpha = m_WeaponFadeTimer / 600.0f * 255.0f;
		break;
	case STATE_MOVE_Y:
		break;
	default:
		break;
	}


	DrawWeaponAmmo(Alpha, Offset);
	DrawWeaponIcon(Alpha, Offset);
}

void MyHud::DrawWeaponAmmo(float Alpha, float Offset)
{
	CPlayerPed* Playa = FindPlayerPed(-1);
	eWeaponType CurrWeapon = Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_eWeaponType;
	int AmmoInClip = Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_nAmmoInClip;
	int AmmoTotal = Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_nAmmoTotal - AmmoInClip;
	int MaxAmmo = CWeaponInfo::GetWeaponInfo(CurrWeapon, 1)->m_nAmmoClip;

	if (CurrWeapon < 15 || CurrWeapon == WEAPONTYPE_PARACHUTE || CurrWeapon == WEAPONTYPE_NIGHTVISION
		|| CurrWeapon == WEAPONTYPE_DETONATOR || CurrWeapon == WEAPONTYPE_INFRARED)
		return;



	int PosY = WeaponAmmoPosY + Offset * 35.0f; //Offset is defined as an factor between 0 and 1

	char Thestring[40];
	CFont::SetProportional(true);
	CFont::SetBackground(false, false);
	CFont::SetFontStyle(FONT_PRICEDOWN);
	CFont::SetOrientation(ALIGN_RIGHT);
	CFont::SetEdge(1);
	CFont::SetScale(SCREEN_MULTIPLIER(WeaponAmmoSizeX), SCREEN_MULTIPLIER(WeaponAmmoSizeY));
	CFont::SetDropColor(CRGBA::CRGBA(30, 30, 30, Alpha));

	if (MaxAmmo > 1 && AmmoInClip < 1000)
	{
		CFont::SetColor(CRGBA::CRGBA(100, 100, 100, Alpha));
		sprintf(Thestring, "%d", AmmoInClip);
		CFont::PrintString(SCREEN_COORD_RIGHT(WeaponAmmoPosX), SCREEN_COORD_TOP(PosY), Thestring);

		CFont::SetColor(CRGBA::CRGBA(200, 200, 200, Alpha));
		sprintf(Thestring, "%d", AmmoTotal);
		CFont::PrintString(SCREEN_COORD_RIGHT(WeaponAmmoPosX + 70.0f), SCREEN_COORD_TOP(PosY), Thestring);
	}
	else
	{
		CFont::SetColor(CRGBA::CRGBA(100, 100, 100, Alpha));
		sprintf(Thestring, "%d", AmmoTotal);
		CFont::PrintString(SCREEN_COORD_RIGHT(WeaponAmmoPosX), SCREEN_COORD_TOP(PosY), Thestring);
	}

}

void MyHud::DrawWeaponIcon(float Alpha, float Offset)
{


	int PosY = WeaponIconPosY + Offset * 35.0f;
	CPlayerPed* Playa = FindPlayerPed(-1);
	CRect Icon;
	int WeaponModel = CWeaponInfo::GetWeaponInfo(Playa->m_aWeapons[Playa->m_nSelectedWepSlot].m_eWeaponType, 1)->m_nModelId;

	if (WeaponModel <= 0)
	{	//This Icon width is 2x height -->WeaponIconSize*2 
		Icon = CRect(SCREEN_COORD_RIGHT(WeaponIconSize * 2 + WeaponIconPosX), SCREEN_COORD_TOP(PosY), SCREEN_COORD_RIGHT(WeaponIconPosX), SCREEN_COORD_TOP(WeaponIconSize + PosY));
		CHud::Sprites[0].Draw(Icon, CRGBA::CRGBA(255, 255, 255, Alpha));
	}
	else
	{
		CBaseModelInfo* Model = CModelInfo::GetModelInfo(WeaponModel);
		TxdDef* txd = CTxdStore::ms_pTxdPool->GetAt(Model->m_nTxdIndex);
		if (txd && txd->m_pRwDictionary)
		{
			RwTexture* IconTex = RwTexDictionaryFindNamedTexture(txd->m_pRwDictionary, "icon");
			if (!IconTex)
				IconTex = RwTexDictionaryFindHashNamedTexture(txd->m_pRwDictionary, CKeyGen::AppendStringToKey(Model->m_nKey, "ICON"));

			if (IconTex)
			{
				float tempWidth = WeaponIconSize;
				CTxdStore::PushCurrentTxd();
				if (IconTex->raster->height != IconTex->raster->width)
					tempWidth = WeaponIconSize * 2;

				//PosY = WeaponIconPosY + Offset * 35.0f;
				Icon = CRect(SCREEN_COORD_RIGHT(tempWidth + WeaponIconPosX), SCREEN_COORD_TOP(PosY), SCREEN_COORD_RIGHT(WeaponIconPosX), SCREEN_COORD_TOP(WeaponIconSize + PosY));

				CSprite2d IconSprite = CSprite2d::CSprite2d();
				CTxdStore::SetCurrentTxd(Model->m_nTxdIndex);
				IconSprite.SetTexture(IconTex->name);
				IconSprite.Draw(Icon, CRGBA::CRGBA(255, 255, 255, Alpha));
				CTxdStore::PopCurrentTxd();
				IconSprite.Delete();
			}
		}
	}

}

void MyHud::DrawHealthandArmor()
{
	CPlayerPed* Playa = FindPlayerPed(-1);
	int Polycount = NUM_STEPSCIRCLE;

	CRGBA color_health = CRGBA::CRGBA(87, 124, 88, 255);
	CRGBA color_armor = CRGBA::CRGBA(74, 148, 160, 255);

	float percentage_health = (Playa->m_fHealth / Playa->m_fMaxHealth) * 100;
	float percentage_armor = Playa->m_fArmour;

	//why can't i write test & test2 directly into min()???
	int test = ceil((Polycount - 1) * percentage_health / 200);
	int test2 = (Polycount / 2);
	int IndexHealth = std::min(test2, test);
	int IndexMax = ceil((Polycount - 1) * (percentage_armor / 200 + percentage_health / 200)); //Maximum Index filled

	float x[NUM_STEPSCIRCLE];
	float y[NUM_STEPSCIRCLE];
	float x2[NUM_STEPSCIRCLE];
	float y2[NUM_STEPSCIRCLE];

	for (int i = 0; i < Polycount; i++)
	{

		float Offsetx = RadarPosX + RadarRadiusOut;
		float OffsetY = RadarPosY + RadarRadiusOut;

		x2[i] = SCREEN_COORD_LEFT(UnitCircleX[i] * (RadarRadiusOut * 0.96) + Offsetx);
		y2[i] = SCREEN_COORD_BOTTOM(UnitCircleY[i] * (RadarRadiusOut * 0.96) + OffsetY);
		x[i] = SCREEN_COORD_LEFT(UnitCircleX[i] * RadarRadius + Offsetx);
		y[i] = SCREEN_COORD_BOTTOM(UnitCircleY[i] * RadarRadius + OffsetY);
	}

	for (int i = 0; i < IndexMax; i++)
	{
		if (i == Polycount - 2)
			CSprite2d::Draw2DPolygon(x[i], y[i], x2[i], y2[i], x[0], y[0], x2[0], y2[0], color_armor);
		else if (i < IndexHealth)
			CSprite2d::Draw2DPolygon(x[i], y[i], x2[i], y2[i], x[i + 1], y[i + 1], x2[i + 1], y2[i + 1], color_health);
		else if (i >= IndexHealth)
			CSprite2d::Draw2DPolygon(x[i], y[i], x2[i], y2[i], x[i + 1], y[i + 1], x2[i + 1], y2[i + 1], color_armor);
	}

}


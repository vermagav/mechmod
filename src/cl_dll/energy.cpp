/*
* Draw Fuel/Energy
* @version 1/05/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

float g_flFuel;
HSPRITE	g_hsprOverhead_green;
HSPRITE	g_hsprOverhead_red;
HSPRITE	g_hsprBBox;

#define MAX_FUEL 1000
#define MAX_POWER 1000

// Hud information
// Top Weapons Bar
#define OFFSET_WEP_TOP	5
#define OFFSET_WEP_LEFT	22
#define WEPBAR_WIDTH	165
// Side (armor?) bar
#define OFFSET_SIDEB_TOP	27
#define OFFSET_SIDEB_LEFT	239
// Armor/Energy/Power bar offsets
#define OFFSET_AEP_BARS_TOP		31
#define OFFSET_AEP_BARS_LEFT	107
#define AEP_HEIGHT	12
#define AEP_WIDTH	121
#define AEP_BETWEEN	5
// Flymode dot
#define FLYMODE_TOP		31
#define FLYMODE_LEFT	45
#define FLYMODE_HEIGHT	8
#define FLYMODE_BETWEEN	10


DECLARE_MESSAGE(m_Energy, Power);
DECLARE_MESSAGE(m_Energy, Flymode);
DECLARE_MESSAGE(m_Energy, MaxHealth);

int CHudEnergy::Init(void)
{	HOOK_MESSAGE(Power);
	HOOK_MESSAGE(Flymode);
	HOOK_MESSAGE(MaxHealth);
	m_iFlags |= HUD_ACTIVE;
	m_flPower = 0;
	m_iFlyMode = 0;
	m_iMaxHealth = 100;
	m_iMaxArmor = 100;
	gHUD.AddHudElem(this);
	return 1;
}

int CHudEnergy::VidInit(void)
{	m_HUD_mainback = gHUD.GetSpriteIndex( "mainhud_back" );
	m_HUD_mainbars = gHUD.GetSpriteIndex( "mainhud_bars" );	
	m_HUD_flymode = gHUD.GetSpriteIndex( "mainhud_flymode" );
	m_HUD_wepbar = gHUD.GetSpriteIndex( "mainhud_bar_wep" );
	m_HUD_sidebar = gHUD.GetSpriteIndex( "mainhud_bar_side" );
	m_iMainWidth = gHUD.GetSpriteRect(m_HUD_mainback).right - gHUD.GetSpriteRect(m_HUD_mainback).left;
	m_iMainHeight = gHUD.GetSpriteRect(m_HUD_mainback).bottom - gHUD.GetSpriteRect(m_HUD_mainback).top;
	g_hsprOverhead_green = SPR_Load( "sprites/overhead-green.spr" );
	g_hsprOverhead_red = SPR_Load( "sprites/overhead-red.spr" );
	g_hsprBBox = SPR_Load("sprites/bbox.spr");
	return 1;
}

int CHudEnergy::Draw(float flTime)
{	if ( (gHUD.m_iHideHUDDisplay & HIDEHUD_HEALTH) || gEngfuncs.IsSpectateOnly() )
		return 1;

	if(gEngfuncs.GetLocalPlayer() && (gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT))))
	{	int x,y;
		int iSubtract = 0;

		// Get position for the background of the hud
		x = 0;
		y = ScreenHeight - m_iMainHeight;

		// Draw background
		SPR_Set(gHUD.GetSprite(m_HUD_mainback), 255, 255, 255);
		SPR_DrawHoles(0, x, y, &gHUD.GetSpriteRect(m_HUD_mainback));

		// Offset x + y correctly
		x += OFFSET_AEP_BARS_LEFT;
		y += OFFSET_AEP_BARS_TOP;

		// Set Sprite
		SPR_Set(gHUD.GetSprite(m_HUD_mainbars), 255, 255, 255);

		/*
		* Draw Health
		*/

		// Set rect
		wrect_t rect = gHUD.GetSpriteRect( m_HUD_mainbars );
		rect.bottom = AEP_HEIGHT;
		int iHealth = gHUD.m_Health.m_iHealth;
		if(iHealth > m_iMaxHealth)
			iHealth = m_iMaxHealth;
		iSubtract = (AEP_WIDTH) * (1-float(iHealth)/float(m_iMaxHealth));
		if(iSubtract < AEP_WIDTH)
		{	rect.right -= iSubtract;
			// Draw Health
			SPR_Draw(0, x, y, &rect);
		}
		// Draw armor as well
		if(gHUD.m_Battery.m_iBat > 0)
		{	rect.bottom += AEP_HEIGHT;
			rect.top += AEP_HEIGHT;
			rect.right = AEP_WIDTH;

			int iArmor = gHUD.m_Battery.m_iBat;
			if(iArmor > m_iMaxArmor)
				iArmor = m_iMaxArmor;
			iSubtract = (AEP_WIDTH) * (1-float(iArmor)/float(m_iMaxArmor));
			if(iSubtract < AEP_WIDTH)
			{	rect.right -= iSubtract;
				// Draw Health
				SPR_Draw(0, x, y, &rect);
			}
		}
		else
		{	rect.bottom += AEP_HEIGHT;
			rect.top += AEP_HEIGHT;
		}

		
		/*
		* Draw Energy
		*/

		// Add offset
		y += AEP_HEIGHT + AEP_BETWEEN;
		rect.bottom += AEP_HEIGHT;
		rect.top += AEP_HEIGHT;
		rect.right = AEP_WIDTH;

		// Regular fuel mode
		if(g_flFuel >= 0)
		{	iSubtract = (AEP_WIDTH) * (1-(g_flFuel / (float)MAX_FUEL));
		}
		// Negitive fuel ( red bar )
		else
		{	iSubtract = (AEP_WIDTH) * (1-(-(g_flFuel) / (float)MAX_FUEL));
			// Set red bar sprite	
			rect.bottom += AEP_HEIGHT;
			rect.top += AEP_HEIGHT;
		}

		if(iSubtract < AEP_WIDTH)
		{	rect.right -= iSubtract;
			// Draw energy
			SPR_Draw(0, x, y, &rect);
		}

		// Push down so we can use the correct bar for the power
		if(g_flFuel >= 0)
		{	rect.bottom += AEP_HEIGHT;
			rect.top += AEP_HEIGHT;
		}

		/*
		* Draw Power
		*/

		// Add offset
		y += AEP_HEIGHT + AEP_BETWEEN;
		rect.bottom += AEP_HEIGHT;
		rect.top += AEP_HEIGHT;
		rect.right = AEP_WIDTH;

		// Get decrease amount
		iSubtract = (AEP_WIDTH * (1-(m_flPower / (float)MAX_POWER)));
	
		// Only draw if we have enough
		if(iSubtract < AEP_WIDTH)
		{	rect.right -= iSubtract;
			// Draw energy
			SPR_Draw(0, x, y, &rect);
		}

		/*
		* Draw Fly Mode
		*/
		if(m_iFlyMode)
		{	// Get x/y coords
			y = (ScreenHeight - m_iMainHeight) + FLYMODE_TOP + (FLYMODE_HEIGHT + FLYMODE_BETWEEN) * (m_iFlyMode-1);
			x = FLYMODE_LEFT;

			SPR_Set(gHUD.GetSprite(m_HUD_flymode), 255, 255, 255);
			rect = gHUD.GetSpriteRect( m_HUD_flymode );
			rect.top = FLYMODE_HEIGHT * (m_iFlyMode-1);
			rect.bottom = FLYMODE_HEIGHT * (m_iFlyMode);

			// Draw flymode
			SPR_DrawHoles(0, x, y, &rect);

		}

		// Draw weapon ammo (if a weapon is currently out)
		WEAPON *pWeapon = gHUD.m_Ammo.m_pWeapon;
		if(!pWeapon)
		{	return 1;
		}

		// Only draw weapons that use ammo
		if (pWeapon->iAmmoType <= 0)
			return 1;

		// Get x/y coords
		y = (ScreenHeight - m_iMainHeight) + OFFSET_WEP_TOP;
		x = OFFSET_WEP_LEFT;

		SPR_Set(gHUD.GetSprite(m_HUD_wepbar), 255, 255, 255);
		rect = gHUD.GetSpriteRect( m_HUD_wepbar );

		rect.right -= WEPBAR_WIDTH * ((float)1-((float)pWeapon->iClip/(float)pWeapon->iMax1));
	
		// Draw wep ammo bar
		SPR_DrawHoles(0, x, y, &rect);
	}
	return 1;
}

int CHudEnergy::MsgFunc_Power(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	m_flPower = READ_COORD();
	return 1;
}

int CHudEnergy::MsgFunc_Flymode(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	m_iFlyMode = READ_BYTE();
	return 1;
}

int CHudEnergy::MsgFunc_MaxHealth(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	m_iMaxHealth = READ_BYTE();
	m_iMaxArmor = READ_BYTE();
	return 1;
}
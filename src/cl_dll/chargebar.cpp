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

extern float g_flCurrentCharge;
extern float g_flMinCharge;

int CHudChargeBar::Init(void)
{	m_iFlags |= HUD_ACTIVE;
	Reset();

	gHUD.AddHudElem(this);
	return 1;
};

void CHudChargeBar::Reset( void )
{	g_flCurrentCharge = 0;
	g_flMinCharge = 0;
}

int CHudChargeBar::VidInit(void)
{	// Sprites
	m_HUD_main = gHUD.GetSpriteIndex( "charge_main" );
	m_HUD_barafter = gHUD.GetSpriteIndex( "charge_barafter" );
	m_HUD_barpre = gHUD.GetSpriteIndex( "charge_barpre" );

	// Background sprite size
	m_iMainWidth = gHUD.GetSpriteRect(m_HUD_main).right - gHUD.GetSpriteRect(m_HUD_main).left;
	m_iMainHeight = gHUD.GetSpriteRect(m_HUD_main).bottom - gHUD.GetSpriteRect(m_HUD_main).top;
	return 1;
};

int CHudChargeBar::Draw(float flTime)
{	if ( (gHUD.m_iHideHUDDisplay & HIDEHUD_HEALTH) || gEngfuncs.IsSpectateOnly() )
	{	return 1;
	}

	if(!(gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT))))
	{	return 1;
	}

	// No charge for us to draw
	if(g_flCurrentCharge <= 0)
	{	return 1;
	}

	// Get position for the background of the hud
	int x = ScreenWidth - m_iMainWidth;
	int y = ScreenHeight - m_iMainHeight;

	// Draw background
	SPR_Set(gHUD.GetSprite(m_HUD_main), 255, 255, 255);
	SPR_DrawHoles(0, x, y, &gHUD.GetSpriteRect(m_HUD_main));

	// Draw Bars
	wrect_t rect;
	
	if(g_flCurrentCharge > g_flMinCharge)
	{	SPR_Set(gHUD.GetSprite(m_HUD_barafter), 255, 255, 255);
		rect = gHUD.GetSpriteRect( m_HUD_barafter );
	}
	else
	{	SPR_Set(gHUD.GetSprite(m_HUD_barpre), 255, 255, 255);
		rect = gHUD.GetSpriteRect( m_HUD_barpre );
	}

	int iHeight = rect.bottom - rect.top;
	float flRealHeight = (1-(g_flCurrentCharge / float(100))) * iHeight;
	float flRegHeight = (g_flCurrentCharge / float(100)) * iHeight;
	float flHeight;

	flHeight = flRealHeight;
	rect.top += flHeight;

	x += 5;
	y += 23 + flHeight;

	// Draw background
	SPR_DrawHoles(0, x, y, &rect);

	return 1;
}

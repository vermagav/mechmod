/*
* Crosshair
* @version 1/08/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "triangleapi.h"
#include "pm_defs.h"
#include "event_api.h"
#include "pmtrace.h"

#include <string.h>
#include <stdio.h>

extern cvar_t *cl_crosshair;
extern vec3_t g_CameraAngles;

#define DLLEXPORT __declspec( dllexport )
extern "C"
{	int DLLEXPORT CL_IsThirdPerson( void );
};

int CHudCrosshair::Init(void)
{	m_iFlags |= HUD_ACTIVE;
	m_iCrosshair = 1;
	m_iMouseOverEnt = 0;
	m_flMouseOverTime = -9999;
	gHUD.AddHudElem(this);
	return 1;
}

void CHudCrosshair::Reset( void )
{	m_iMouseOverEnt = 0;
	m_flMouseOverTime = -9999;
}

int CHudCrosshair::VidInit(void)
{	m_HUD_Crosshair = gHUD.GetSpriteIndex("crosshair_1");
	return 1;
}

void CHudCrosshair::SetCrosshairSprite( void )
{	char szCrosshair[32];
	sprintf(szCrosshair,"crosshair_%i", m_iCrosshair);
	m_HUD_Crosshair = gHUD.GetSpriteIndex(szCrosshair);
}

int CHudCrosshair::Draw(float flTime)
{	// Get crosshair cvar
	int iCrosshair = (int)cl_crosshair->value + 1;
	if(iCrosshair > 8)
		iCrosshair = 8;
	else if(iCrosshair < 1)
		iCrosshair = 1;

	// Set crosshair
	if(iCrosshair != m_iCrosshair)
	{	m_iCrosshair = iCrosshair;
		SetCrosshairSprite();
	}

	cl_entity_t *pClient = gEngfuncs.GetLocalPlayer();

	if(!pClient || (gHUD.m_iHideHUDDisplay & HIDEHUD_HEALTH) || gEngfuncs.IsSpectateOnly())
	{	return 1;
	}

	// Get Crosshair Origin
	vec3_t v_forward, vecScreen, vecOrigin, vecViewOfs;
	vecOrigin = pClient->origin;
	// Set view angles
	if(CL_IsThirdPerson())
	{	AngleVectors( g_CameraAngles, v_forward, NULL, NULL);	
	}
	else
	{	vec3_t vecViewAngles;
		gEngfuncs.GetViewAngles( (float *)vecViewAngles);
		AngleVectors( vecViewAngles, v_forward, NULL, NULL);	
	}

	// Traceline
	gEngfuncs.pEventAPI->EV_LocalPlayerViewheight( vecViewOfs );
	vecOrigin = vecOrigin + vecViewOfs;

	pmtrace_t trace;


	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers ( pClient->index - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vecOrigin,vecOrigin + v_forward * 8192, PM_STUDIO_BOX, -1, &trace );

	physent_t *pe = gEngfuncs.pEventAPI->EV_GetPhysent( trace.ent );
	if(pe && pe->player)
	{	m_iMouseOverEnt = pe->info;
		m_flMouseOverTime = flTime;
	}
	
	gEngfuncs.pEventAPI->EV_PopPMStates();

	// Get endpos to screen
	gEngfuncs.pTriAPI->WorldToScreen(trace.endpos, vecScreen);

	// Draw Crosshair
	int iXOffset = 32;
	int iYOffset = 32;
	SPR_Set(gHUD.GetSprite(m_HUD_Crosshair), 255, 255, 255);
	SPR_DrawAdditive(0,  XPROJECT(vecScreen.x)-iXOffset, YPROJECT(vecScreen.y)-iYOffset, &gHUD.GetSpriteRect(m_HUD_Crosshair));

	return 1;
}
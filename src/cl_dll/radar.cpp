/*
* Radar
* @version 1/11/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "triangleapi.h"
#include "pm_defs.h"
#include "event_api.h"
#include "pmtrace.h"
#include "vgui_TeamFortressViewport.h"

#include <string.h>
#include <stdio.h>

#define MAX_RADAR_DIST		2000

// Radar offsets
#define OFFSET_RAD_HEIGHT	81
#define OFFSET_RAD_WIDTH	82
#define RAD_DIAM			62
// Info box
#define INFO_WIDTH			218
#define INFO_OFFSET_WIDTH	19
#define INFO_OFFSET_HEIGHT	13
#define INFO_OFFSET_LINE	10
// Slide time
#define SLIDETIME	(float)0.5

#define PI_180 (3.14159265358979/180.0)
void VectorAngles( const float *forward, float *angles );
extern char *sLocalisedClasses[];
extern char *sMMClassSelection1[];
extern rendercheck_t g_RenderCheck[33];

int CHudRadar::Init(void)
{	m_blRadar = true;
	m_blPrevRadar = true;
	m_blSliding = false;
	m_flSlideTime = 0;
	m_iyBottomRadar = 0;
	m_iFlags |= HUD_ACTIVE;
	SetClassNames();
	gHUD.AddHudElem(this);
	return 1;
}

int CHudRadar::VidInit(void)
{	m_HUD_radar = gHUD.GetSpriteIndex("radar");
	m_HUD_radar_info = gHUD.GetSpriteIndex("radar_info");
	m_HUD_radardot = gHUD.GetSpriteIndex("radar_dot");
	return 1;
}

// Set class names, save time from constantly using BufferedLocaliseTextString
void CHudRadar::SetClassNames( void )
{	strcpy(szClassNames[0],"");
	for(int i=PC_VF1J;i<PC_RANDOM;i++)
	{	strcpy(szClassNames[i],sMMClassSelection1[i] );//CHudTextMessage::BufferedLocaliseTextString( sLocalisedClasses[i] ));
	}
}

int CHudRadar::Draw(float flTime)
{	if ( (gHUD.m_iHideHUDDisplay & HIDEHUD_HEALTH) || gEngfuncs.IsSpectateOnly() || !(gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT))))
	{	return 1;
	}

	cl_entity_t *pLocal = gEngfuncs.GetLocalPlayer();
	if(!pLocal)
	{	return 1;
	}

	float flPlayerAngles, flLength, flAngle, flRadians, pos_x, pos_y;
	vec3_t vecViewAngles, v_right, vecPlayerOrigin, vecOtherOrigin, vecDifference, vecAngles;

	/*
	* Draw Background radar
	*/

	// Get origin for radar
	int x = ScreenWidth;
	int y = 0;

	int iRadarWidth = gHUD.GetSpriteRect(m_HUD_radar).right - gHUD.GetSpriteRect(m_HUD_radar).left;
	int iRadarHeight = gHUD.GetSpriteRect(m_HUD_radar).bottom - gHUD.GetSpriteRect(m_HUD_radar).top;

	int infox = ScreenWidth - INFO_WIDTH;
	int infoy = iRadarHeight;
	int infoheight = gHUD.GetSpriteRect(m_HUD_radar_info).bottom - gHUD.GetSpriteRect(m_HUD_radar_info).top;

	x -= iRadarWidth;
	
	// Slide radar + info bar in/out when we enable/disable
	if(m_blPrevRadar != m_blRadar && !m_blSliding)
	{	m_blSliding = true;
		m_flSlideTime = flTime;
	}
	// We switched the radar on/off while in the middle of sliding
	else if(m_blPrevRadar == m_blRadar && m_blSliding)
	{	m_blPrevRadar = !m_blPrevRadar;
		// Invert time
		float flSlideTime = flTime - m_flSlideTime;
		flSlideTime = SLIDETIME - flSlideTime;
		m_flSlideTime = flTime - flSlideTime;
	}

	if(m_blSliding)
	{	// Sliding is done, disable
		if((m_flSlideTime + SLIDETIME) < flTime)
		{	m_blSliding = false;
			m_flSlideTime = 0;
			m_blPrevRadar = m_blRadar;

			// Done sliding and radar is off, disable
			if(!m_blRadar)
			{	return 1;
			}
		}
		// we're still sliding
		else
		{	float flPercent;
			if(m_blRadar)
				flPercent = (SLIDETIME-(flTime - m_flSlideTime))/SLIDETIME;
			else
				flPercent = (flTime - m_flSlideTime)/SLIDETIME;
			
			// Slide Radar
			y = -iRadarHeight * flPercent;
			// Slide Infobox
			infox += INFO_WIDTH * flPercent;
		}
	}

	// Done sliding and radar is off, disable
	if(!m_blRadar && !m_blSliding)
	{	return 1;
	}
	// Draw Radar
	SPR_Set(gHUD.GetSprite(m_HUD_radar), 255, 255, 255);
	SPR_DrawHoles(0, x,y, &gHUD.GetSpriteRect(m_HUD_radar));

	// Draw info box
	SPR_Set(gHUD.GetSprite(m_HUD_radar_info), 255,255,255);
	SPR_DrawHoles(0, infox,infoy, &gHUD.GetSpriteRect(m_HUD_radar_info));

	if(m_blSliding)
		m_iyBottomRadar = y + iRadarHeight;
	else
		m_iyBottomRadar = infoy + infoheight;

	// Sliding or radar is just off, dont display information
	if(m_blSliding || !m_blRadar)
	{	return 1;
	}
	/*
	* Get Player Angles
	*/

	// Setting player angles
	gEngfuncs.GetViewAngles((float *)vecViewAngles);
	AngleVectors(vecViewAngles, NULL, v_right, NULL );
	// Inverse vector
	VectorInverse(v_right);
	v_right.z = 0;

	// Get angles
	VectorAngles(v_right,vecAngles);
	flPlayerAngles = vecAngles.y;

	// strictly 0 to 360
	if(flPlayerAngles < 0)
		flPlayerAngles += 360;

	// Move x and y to the middle of the radar
	x += OFFSET_RAD_WIDTH;
	y += OFFSET_RAD_HEIGHT;

	/*
	* Loop and draw dots
	*/
	for(int i=0; i<MAX_PLAYERS; i++)
	{	int r=255,g=255,b=255;
		bool blDrawInfo = false;
		cl_entity_t *pPlayer = gEngfuncs.GetEntityByIndex(i);
		// Invalid
		if(!pPlayer)
		{	continue;
		}

		// Not player
		if(!pPlayer->player)
		{	continue;
		}

		// Render check, make sure player is being rendered
		if(!g_RenderCheck[i].blRendered)
		{	continue;
		}

		// Draw yourself as a orange dot
		if(pPlayer == pLocal)
		{	r=255;
			g=255;
			b=0;
		}

		// Get Origins
		vecPlayerOrigin = pLocal->origin;
		vecOtherOrigin = pPlayer->origin;
		vecPlayerOrigin.z = 0;
		vecOtherOrigin.z = 0;
		vecDifference = vecPlayerOrigin - vecOtherOrigin;

		flLength = vecDifference.Length();

		// Player is too far away, dont draw
		if(flLength > MAX_RADAR_DIST)
			continue;

		vecDifference = vecDifference.Normalize();

		// Set 0 to 360
		VectorAngles(vecDifference, vecAngles);
		flAngle = vecAngles.y;

		if(flAngle < 0)
			flAngle += 360;

		// Subtract Angles
		flAngle = flPlayerAngles - flAngle;

		// Set length according to the size of the radar
		flLength = (flLength/(float)MAX_RADAR_DIST) * RAD_DIAM;

		// Angles to radians
		flRadians = flAngle * PI_180;
		pos_x = int(cos(flRadians)*flLength);
		pos_y = int(sin(flRadians)*flLength);

		// Set positions + draw radar
		pos_x += x;
		pos_y += y;

		// Shift dot over so it's drawn in the correct position
		int iDotWidth = (gHUD.GetSpriteRect(m_HUD_radardot).right - gHUD.GetSpriteRect(m_HUD_radardot).left)/2;
		int iDotHeight = (gHUD.GetSpriteRect(m_HUD_radardot).bottom - gHUD.GetSpriteRect(m_HUD_radardot).top)/2;
		pos_x -= iDotWidth;
		pos_y -= iDotHeight;

		// Person is targeted by our missles, color them red
		if( gHUD.m_Lockon.FindLock( pPlayer->index ) != -1 )
		{	r=255;
			g=0;
			b=0;
		}

		// Yellow if our crosshair is/was over the player
		if(	gHUD.m_Crosshair.m_iMouseOverEnt == pPlayer->index && ((flTime - gHUD.m_Crosshair.m_flMouseOverTime) < 3) )
		{	UnpackRGB(r,g,b, RGB_YELLOWISH);
			blDrawInfo = true;
		}

		// Draw
		SPR_Set(gHUD.GetSprite(m_HUD_radardot), r,g,b);
		SPR_DrawAdditive(0, pos_x,pos_y, &gHUD.GetSpriteRect(m_HUD_radardot));

		// Draw information if specified
		if(blDrawInfo)
		{	int line_height=0,line_width=0;
			pos_x = ScreenWidth - INFO_WIDTH + INFO_OFFSET_WIDTH;
			pos_y = iRadarHeight + INFO_OFFSET_HEIGHT;

			hud_player_info_t *pl_info = &g_PlayerInfoList[i];
			char szName[128];

			// Get colors
			float flTeamR,flTeamG,flTeamB;
			if((g_PlayerExtraInfo[i].teamnumber == g_PlayerExtraInfo[pLocal->index].teamnumber) && gHUD.m_Teamplay)
			{	flTeamR=0;
				flTeamG=0.9;
				flTeamB=0;
			}
			else
			{	flTeamR=1.0;
				flTeamG=0.2;
				flTeamB=0.2;
			}
			
			// Draw player name
			strcpy(szName,"Name: ");
			int iNameX = DrawConsoleString( pos_x, pos_y, szName );
			// Draw name in color, depending if player is on your team
			strcpy(szName, pl_info->name);
			// Set color
			gEngfuncs.pfnDrawSetTextColor(flTeamR,flTeamG,flTeamB);
			// Draw
			DrawConsoleString( iNameX, pos_y, szName );


			// Adjust offset
			GetConsoleStringSize( szName, &line_width, &line_height );
			pos_y += line_height + INFO_OFFSET_LINE;
			// Draw Label
			strcpy(szName,"Mech: ");
			iNameX = DrawConsoleString( pos_x, pos_y, szName );
			// Get Mech Name
			int iClass = g_PlayerExtraInfo[i].playerclass;
			if(iClass < 0)
				iClass = 0;
			else if(iClass >= PC_LASTCLASS)
				iClass = PC_LASTCLASS-1;
			strcpy(szName, szClassNames[iClass] );
			// Set Color
			gEngfuncs.pfnDrawSetTextColor(flTeamR,flTeamG,flTeamB);
			// Draw Mech Name
			DrawConsoleString( iNameX, pos_y, szName );
		}
	}

	return 1;
}
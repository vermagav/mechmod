/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  hud_msg.cpp
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "r_efx.h"

//added by jason for proper removal of systems on map changes
//added 1-5-03
#include "com_model.h"
//#include "spr_texture.h"
//added 1-15-04
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "psys_man.h"
#include "eff_test.h"
#include "ent_trail.h"
#include "ent_trail_man.h"
#include "triangleAPI.h"
#include "gl-triapi.h"
// added 7-21-04
#include "gl_pbuffer.h"
// added 8-31-04
#include "alpha_bomb.h"
// added by pcjoe
#include "eff_tracerbullet.h"
extern CEffTracerBullet *g_TracerBullet;
extern "C" int g_iHullsFixed;


extern bool g_blReplaceTexture;

//added 1-20-03
#include "eff_water.h"

// added 4-15-04
#include "ds_scythe.h"

// added 4-17-04
#include "cg_glow.h" 
#include "cg_shader.h"
#include "gl_bored.h"
#include "tri-vgui.h"
//end jason

#define MAX_CLIENTS 32

extern BEAM *pBeam;
extern BEAM *pBeam2;

/// USER-DEFINED SERVER MESSAGE HANDLERS

int CHud :: MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf )
{
	// added by jason
	g_iHullsFixed = 0;


	ASSERT( iSize == 0 );

	// clear all hud data
	HUDLIST *pList = m_pHudList;

	while ( pList )
	{
		if ( pList->p )
			pList->p->Reset();
		pList = pList->pNext;
	}

	// reset sensitivity
	m_flMouseSensitivity = 0;

	// reset concussion effect
	m_iConcussionEffect = 0;

	return 1;
}

void CAM_ToFirstPerson(void);

void CHud :: MsgFunc_ViewMode( const char *pszName, int iSize, void *pbuf )
{
	CAM_ToFirstPerson();
}

void CHud :: MsgFunc_InitHUD( const char *pszName, int iSize, void *pbuf )
{
	// prepare all hud data
	HUDLIST *pList = m_pHudList;

	while (pList)
	{
		if ( pList->p )
			pList->p->InitHUDData();
		pList = pList->pNext;
	}

	//Probably not a good place to put this.
	pBeam = pBeam2 = NULL;
	
	//added by jason 1-5-03
	//clean up our used memory
	// added by jason

	g_CGLTriAPI.OGLActive( );
	gTexture.KillTextures( );
	gTexture.Precache( );
//	g_SprTex.RemoveAll( );		//remove all textures
	GetManager()->RemoveAll( );	//remove all particle systems
	g_PartPool.DrainPool( );	//remove all pooled particles
	//gTGAReplace.RemoveAllTex();
	g_blReplaceTexture = true;
	GetManager( )->AddSystem( new CEffWater( ) );
	g_TrailMan.RemoveAll( );
	// added 4-15-04
	//g_Scythe.~CScythe( );
	g_Scythe.Init( );
	// added 4-17-04
	g_CGShader.RemoveAll( );
	g_CGShader.SetShaderEnv( );
	g_Glow.InitScreenGlow( );
	g_Effects.LoadData();
	g_AlphaBomb.Initialize( );
	gHUD.m_blHideHud = false;
	

	g_ClassMenu.Initialize( );
	g_TeamMenu.Initialize( );
	g_Shadow.LoadTex( );
	gHUD.m_Saberfx.m_SaberFX.clear();

	//g_PBuffer.Init( );
	//g_PartPool.InitPool( );	//create a pool
	//create an effect
//	GetManager()->AddSystem( new CEffTest() );
	// HACK HACK: Pcjoe - Reset tracer system
	g_TracerBullet = NULL;
	
	//end jason
}


int CHud :: MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_Teamplay = READ_BYTE();

	return 1;
}


int CHud :: MsgFunc_Damage(const char *pszName, int iSize, void *pbuf )
{
	int		armor, blood;
	Vector	from;
	int		i;
	float	count;
	
	BEGIN_READ( pbuf, iSize );
	armor = READ_BYTE();
	blood = READ_BYTE();

	for (i=0 ; i<3 ; i++)
		from[i] = READ_COORD();

	count = (blood * 0.5) + (armor * 0.5);

	if (count < 10)
		count = 10;

	// TODO: kick viewangles,  show damage visually

	return 1;
}

int CHud :: MsgFunc_Concuss( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_iConcussionEffect = READ_BYTE();
	if (m_iConcussionEffect)
		this->m_StatusIcons.EnableIcon("dmg_concuss",255,160,0);
	else
		this->m_StatusIcons.DisableIcon("dmg_concuss");
	return 1;
}

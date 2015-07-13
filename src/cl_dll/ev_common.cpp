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
// shared event functions
#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"

#include "r_efx.h"

#include "eventscripts.h"
#include "event_api.h"
#include "pm_shared.h"

//added by jason
#include "pmtrace.h"
#include "pm_defs.h"
extern cvar_t *cl_shelllife;
// end jason

#define IS_FIRSTPERSON_SPEC ( g_iUser1 == OBS_IN_EYE || (g_iUser1 && (gHUD.m_Spectator.m_pip->value == INSET_IN_EYE)) )
/*
=================
GetEntity

Return's the requested cl_entity_t
=================
*/
struct cl_entity_s *GetEntity( int idx )
{
	return gEngfuncs.GetEntityByIndex( idx );
}

/*
=================
GetViewEntity

Return's the current weapon/view model
=================
*/
struct cl_entity_s *GetViewEntity( void )
{
	return gEngfuncs.GetViewModel();
}

/*
=================
EV_CreateTracer

Creates a tracer effect
=================
*/
void EV_CreateTracer( float *start, float *end )
{
	gEngfuncs.pEfxAPI->R_TracerEffect( start, end );
}

/*
=================
EV_IsPlayer

Is the entity's index in the player range?
=================
*/
qboolean EV_IsPlayer( int idx )
{
	if ( idx >= 1 && idx <= gEngfuncs.GetMaxClients() )
		return true;

	return false;
}

/*
=================
EV_IsLocal

Is the entity == the local player
=================
*/
qboolean EV_IsLocal( int idx )
{
	// check if we are in some way in first person spec mode
	if ( IS_FIRSTPERSON_SPEC  )
		return (g_iUser2 == idx);
	else
		return gEngfuncs.pEventAPI->EV_IsLocal( idx - 1 ) ? true : false;
}

/*
=================
EV_GetGunPosition

Figure out the height of the gun
=================
*/
void EV_GetGunPosition( event_args_t *args, float *pos, float *origin )
{
	int idx;
	vec3_t view_ofs;

	idx = args->entindex;

	VectorClear( view_ofs );
	view_ofs[2] = DEFAULT_VIEWHEIGHT;

	if ( EV_IsPlayer( idx ) )
	{
		// in spec mode use entity viewheigh, not own
		if ( EV_IsLocal( idx ) && !IS_FIRSTPERSON_SPEC )
		{
			// Grab predicted result for local player
			gEngfuncs.pEventAPI->EV_LocalPlayerViewheight( view_ofs );
		}
		/* Removed by Pcjoe
		else if ( args->ducking == 1 )
		{
			view_ofs[2] = VEC_DUCK_VIEW;
		}*/
	}

	VectorAdd( origin, view_ofs, pos );
}

/*
=================
EV_EjectBrass

Bullet shell casings
=================
*/
//added by jason
void ShellCallback( struct tempent_s *ent, float frametime, float currenttime )
{
	vec3_t vPrevOrg = ent->entity.origin;

	ent->tentOffset.z += -800 * frametime * ent->x;

	vec3_t vCurOrg = vPrevOrg + ent->tentOffset * frametime;
	
	if( fabs( ent->tentOffset.z ) < 5 && gEngfuncs.PM_PointContents( vPrevOrg - vec3_t( 0,0,2 ), NULL ) != CONTENTS_EMPTY )
	{
		ent->x = 0;
		ent->tentOffset.z = 0;

		if( ent->tentOffset.x < 0 )
		{
			ent->tentOffset.x += 5;
		}
		if( ent->tentOffset.x > 0 )
		{
			ent->tentOffset.x -= 5;
		}
		if( fabs( ent->tentOffset.x ) < 5 )
		{
			ent->tentOffset.x = 0;
		}

		if( ent->tentOffset.y < 0 )
		{
			ent->tentOffset.y += 5;
		}
		if( ent->tentOffset.y > 0 )
		{
			ent->tentOffset.y -= 5;
		}
		if( fabs( ent->tentOffset.y ) < 5 )
		{
			ent->tentOffset.y = 0;
		}
	}	
	else
	{
		ent->x = 1;
		ent->entity.angles.y += gEngfuncs.pfnRandomLong( 1, 15 );
	}

	
	

	pmtrace_s *ptr = gEngfuncs.PM_TraceLine( vPrevOrg + vec3_t( 0,0,10 ), 
		vCurOrg, PM_TRACELINE_PHYSENTSONLY, 2, 1 );

	if( ptr->fraction != 1.0 && ptr->fraction != 0.0 )
	{
		// make the particle bounce
		float flBackoff = DotProduct ( ent->tentOffset, ptr->plane.normal ) * ( ent->bounceFactor + 1);
			
		for (int i=0; i<3; i++)
		{
			float flChange = ptr->plane.normal[i] * flBackoff;

			ent->tentOffset[i] = ent->tentOffset[i] - flChange;

		}
		vCurOrg.z = ptr->endpos.z;
	}

	ent->entity.origin = vCurOrg;
}
// end jason
void EV_EjectBrass( float *origin, float *velocity, float rotation, int model, int soundtype )
{
	vec3_t endpos;
	VectorClear( endpos );
	endpos[1] = rotation;
	// edited by jason 1-18-03
	//gEngfuncs.pEfxAPI->R_TempModel( origin, velocity, endpos, 2.5, model, soundtype );
	model_s *mod;
	mod = gEngfuncs.CL_LoadModel( "models/shell.mdl", &model );
	
	TEMPENTITY *pTempEnt;
	pTempEnt = gEngfuncs.pEfxAPI->CL_TentEntAllocCustom( origin, mod, 1, ShellCallback );
	pTempEnt->die += cl_shelllife->value;
	pTempEnt->bounceFactor = .6;
	pTempEnt->hitSound = soundtype;
	pTempEnt->tentOffset = velocity;
	pTempEnt->x = 1;
	pTempEnt->entity.angles = endpos;
	pTempEnt->flags = 0;
	pTempEnt->flags = FTENT_CLIENTCUSTOM | FTENT_COLLIDEALL | FTENT_PERSIST;
	// end jason
}

/*
=================
EV_GetDefaultShellInfo

Determine where to eject shells from
=================
*/
void EV_GetDefaultShellInfo( event_args_t *args, float *origin, float *velocity, float *ShellVelocity, float *ShellOrigin, float *forward, float *right, float *up, float forwardScale, float upScale, float rightScale )
{
	int i;
	vec3_t view_ofs;
	float fR, fU;

	int idx;

	idx = args->entindex;

	VectorClear( view_ofs );
	view_ofs[2] = DEFAULT_VIEWHEIGHT;

	if ( EV_IsPlayer( idx ) )
	{
		if ( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_LocalPlayerViewheight( view_ofs );
		}
		/* Removed by Pcjoe
		else if ( args->ducking == 1 )
		{
			view_ofs[2] = VEC_DUCK_VIEW;
		}*/
	}

	fR = gEngfuncs.pfnRandomFloat( 50, 70 );
	fU = gEngfuncs.pfnRandomFloat( 100, 150 );

	for ( i = 0; i < 3; i++ )
	{
		ShellVelocity[i] = velocity[i] + right[i] * fR + up[i] * fU + forward[i] * 25;
		ShellOrigin[i]   = origin[i] + view_ofs[i] + up[i] * upScale + forward[i] * forwardScale + right[i] * rightScale;
	}
}

/*
=================
EV_MuzzleFlash

Flag weapon/view model for muzzle flash
=================
*/
void EV_MuzzleFlash( void )
{
	// Add muzzle flash to current weapon model
	cl_entity_t *ent = GetViewEntity();
	if ( !ent )
	{
		return;
	}

	// Or in the muzzle flash
	ent->curstate.effects |= EF_MUZZLEFLASH;
}
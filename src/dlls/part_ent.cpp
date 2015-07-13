/**********************************************************
*			PARTICLE SYSTEM ENTITYS
***********************************************************
*	Purpose: Map based entitys for particle system
*	Created On: 1/9/04
*	Last Edited: 1/9/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "../cl_dll/eff_def.h"
#include "part_ent.h"

LINK_ENTITY_TO_CLASS( particle_brush, CPartBrush );

void CPartBrush::Spawn( void )
{
	// spawn the brush
	pev->movetype	= MOVETYPE_NONE;
	pev->solid		= SOLID_NOT;
	UTIL_SetOrigin(pev, pev->origin);
	SET_MODEL( ENT(pev), STRING(pev->model) );
	
	pev->rendermode		= kRenderTransAlpha;
	pev->renderamt = 0;
	m_iFlags = pev->spawnflags;
	if( !pev->spawnflags )
	{
		m_iFlags = 1;
	}
}

void CPartBrush::KeyValue( KeyValueData *pkvd )
{
	//extract all our data
	if( FStrEq( pkvd->szKeyName, "partspersec" ) )
	{
		m_iPPS = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "bounces" ) )
	{
		m_iBounces = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "syslife" ) )
	{
		m_flSysLife = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "delay" ) )
	{
		m_flDelay = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "partdelay" ) )
	{
		m_flPartDelay = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "stsize" ) )
	{
		m_flStSize = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "edsize" ) )
	{
		m_flEdSize = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "bncscl" ) )
	{
		m_flBncScl = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "grav" ) )
	{
		m_flGrav = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "framerate" ) )
	{
		m_flFR = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "life" ) )
	{
		m_flLife = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	//testing for color stuff
	else if( FStrEq( pkvd->szKeyName, "stcol" ) )
	{
		//lets hope that does it
		char One[8], Two[8], Three[8], Four[8];
		sscanf( pkvd->szValue, "%s %s %s %s\n", &One, &Two, &Three, &Four );
		m_clStCol.r = atoi( One );
		m_clStCol.g = atoi( Two );
		m_clStCol.b = atoi( Three );
		m_clStCol.a = atoi( Four );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "edcol" ) )
	{
		//lets hope that does it
		char One[8], Two[8], Three[8], Four[8];
		sscanf( pkvd->szValue, "%s %s %s %s\n", &One, &Two, &Three, &Four );
		m_clEdCol.r = atoi( One );
		m_clEdCol.g = atoi( Two );
		m_clEdCol.b = atoi( Three );
		m_clEdCol.a = atoi( Four );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "vang" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f`\n", &m_vAng.x, &m_vAng.y, &m_vAng.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "pfmaxvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vPMaxVel.x, &m_vPMaxVel.y, &m_vPMaxVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "pfminvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vPMinVel.x, &m_vPMinVel.y, &m_vPMinVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "maxvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vMaxVel.x, &m_vMaxVel.y, &m_vMaxVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "minvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vMinVel.x, &m_vMinVel.y, &m_vMinVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "sprite" ) )
	{
		strcpy( m_rgcSpr, pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "iangl" ) )
	{
		m_iAngLock = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

void CPartBrush::MsgClient( CBasePlayer *pPlayer )
{
	//msg client with orignial data
	MESSAGE_BEGIN( MSG_ONE, gmsgPartMsg, NULL, pPlayer->pev );
		//now we send our huge fucking msg
		WRITE_SHORT( entindex() );
		WRITE_SHORT( EFF_BRUSH );

		WRITE_SHORT( m_iPPS );
		WRITE_SHORT( m_iBounces );
		WRITE_SHORT( m_iFlags );
		WRITE_SHORT( m_iAngLock );

		WRITE_BYTE( m_clEdCol.r );
		WRITE_BYTE( m_clEdCol.g );
		WRITE_BYTE( m_clEdCol.b );
		WRITE_BYTE( m_clEdCol.a );

		WRITE_BYTE( m_clStCol.r );
		WRITE_BYTE( m_clStCol.g );
		WRITE_BYTE( m_clStCol.b );
		WRITE_BYTE( m_clStCol.a );

		WRITE_COORD( m_flSysLife );
		WRITE_COORD( m_flDelay );
		WRITE_COORD( m_flPartDelay );
		WRITE_COORD( m_flStSize );
		WRITE_COORD( m_flEdSize );
		WRITE_COORD( m_flBncScl );
		WRITE_COORD( m_flGrav );
		WRITE_COORD( m_flFR );
		WRITE_COORD( m_flLife );

		WRITE_COORD( m_vAng.x );
		WRITE_COORD( m_vAng.y );
		WRITE_COORD( m_vAng.z );

		WRITE_COORD( m_vPMaxVel.x );
		WRITE_COORD( m_vPMaxVel.y );
		WRITE_COORD( m_vPMaxVel.z );

		WRITE_COORD( m_vPMinVel.x );
		WRITE_COORD( m_vPMinVel.y );
		WRITE_COORD( m_vPMinVel.z );

		WRITE_COORD( m_vMaxVel.x );
		WRITE_COORD( m_vMaxVel.y );
		WRITE_COORD( m_vMaxVel.z );
 
		WRITE_COORD( m_vMinVel.x );
		WRITE_COORD( m_vMinVel.y );
		WRITE_COORD( m_vMinVel.z );

		WRITE_STRING( m_rgcSpr );
	MESSAGE_END( );
}

LINK_ENTITY_TO_CLASS( particle_point, CPartPoint );

void CPartPoint::Spawn( void )
{
	Precache( );
	// spawn the brush
	pev->movetype	= MOVETYPE_NONE;
	pev->solid		= SOLID_NOT;
	pev->skin		= CONTENTS_EMPTY;
	SET_MODEL( ENT(pev), "models/null.mdl" );
	UTIL_SetOrigin(pev, pev->origin);

	
	pev->rendermode		= kRenderTransAlpha;
	pev->renderamt		= 0;

	m_iFlags = pev->spawnflags;
	if( !pev->spawnflags )
	{
		m_iFlags = 1;
	}
}

void CPartPoint::Precache( void )
{
	PRECACHE_MODEL("models/null.mdl");
}

void CPartPoint::KeyValue( KeyValueData *pkvd )
{
	//extract all our data
	if( FStrEq( pkvd->szKeyName, "partspersec" ) )
	{
		m_iPPS = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "bounces" ) )
	{
		m_iBounces = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "syslife" ) )
	{
		m_flSysLife = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "delay" ) )
	{
		m_flDelay = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "partdelay" ) )
	{
		m_flPartDelay = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "stsize" ) )
	{
		m_flStSize = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "edsize" ) )
	{
		m_flEdSize = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "bncscl" ) )
	{
		m_flBncScl = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "grav" ) )
	{
		m_flGrav = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "framerate" ) )
	{
		m_flFR = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "life" ) )
	{
		m_flLife = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	//testing for color stuff
	else if( FStrEq( pkvd->szKeyName, "stcol" ) )
	{
		//lets hope that does it
		char One[8], Two[8], Three[8], Four[8];
		sscanf( pkvd->szValue, "%s %s %s %s\n", &One, &Two, &Three, &Four );
		m_clStCol.r = atoi( One );
		m_clStCol.g = atoi( Two );
		m_clStCol.b = atoi( Three );
		m_clStCol.a = atoi( Four );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "edcol" ) )
	{
		//lets hope that does it
		char One[8], Two[8], Three[8], Four[8];
		sscanf( pkvd->szValue, "%s %s %s %s\n", &One, &Two, &Three, &Four );
		m_clEdCol.r = atoi( One );
		m_clEdCol.g = atoi( Two );
		m_clEdCol.b = atoi( Three );
		m_clEdCol.a = atoi( Four );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "vang" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f`\n", &m_vAng.x, &m_vAng.y, &m_vAng.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "pfmaxvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vPMaxVel.x, &m_vPMaxVel.y, &m_vPMaxVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "pfminvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vPMinVel.x, &m_vPMinVel.y, &m_vPMinVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "maxvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vMaxVel.x, &m_vMaxVel.y, &m_vMaxVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "minvel" ) )
	{
		//lets hope that does it
		sscanf( pkvd->szValue, "%f %f %f\n", &m_vMinVel.x, &m_vMinVel.y, &m_vMinVel.z );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "sprite" ) )
	{
		strcpy( m_rgcSpr, pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "iangl" ) )
	{
		m_iAngLock = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

void CPartPoint::MsgClient( CBasePlayer *pPlayer )
{
	//msg client with orignial data
	MESSAGE_BEGIN( MSG_ONE, gmsgPartMsg, NULL, pPlayer->pev );
		//now we send our huge fucking msg
		WRITE_SHORT( entindex() );
		WRITE_SHORT( EFF_POINT );

		WRITE_SHORT( m_iPPS );
		WRITE_SHORT( m_iBounces );
		WRITE_SHORT( m_iFlags );
		WRITE_SHORT( m_iAngLock );

		WRITE_BYTE( m_clEdCol.r );
		WRITE_BYTE( m_clEdCol.g );
		WRITE_BYTE( m_clEdCol.b );
		WRITE_BYTE( m_clEdCol.a );

		WRITE_BYTE( m_clStCol.r );
		WRITE_BYTE( m_clStCol.g );
		WRITE_BYTE( m_clStCol.b );
		WRITE_BYTE( m_clStCol.a );

		WRITE_COORD( m_flSysLife );
		WRITE_COORD( m_flDelay );
		WRITE_COORD( m_flPartDelay );
		WRITE_COORD( m_flStSize );
		WRITE_COORD( m_flEdSize );
		WRITE_COORD( m_flBncScl );
		WRITE_COORD( m_flGrav );
		WRITE_COORD( m_flFR );
		WRITE_COORD( m_flLife );

		WRITE_COORD( m_vAng.x );
		WRITE_COORD( m_vAng.y );
		WRITE_COORD( m_vAng.z );

		WRITE_COORD( m_vPMaxVel.x );
		WRITE_COORD( m_vPMaxVel.y );
		WRITE_COORD( m_vPMaxVel.z );

		WRITE_COORD( m_vPMinVel.x );
		WRITE_COORD( m_vPMinVel.y );
		WRITE_COORD( m_vPMinVel.z );

		WRITE_COORD( m_vMaxVel.x );
		WRITE_COORD( m_vMaxVel.y );
		WRITE_COORD( m_vMaxVel.z );
 
		WRITE_COORD( m_vMinVel.x );
		WRITE_COORD( m_vMinVel.y );
		WRITE_COORD( m_vMinVel.z );

		WRITE_STRING( m_rgcSpr );
	MESSAGE_END( );
}
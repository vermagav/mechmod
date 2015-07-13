/**********************************************************
*			TEAM SPAWN ENTITY
***********************************************************
*	Purpose: Team Spawn Points
*	Created On: 1/14/04
*	Last Edited: 1/14/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "team_spawns.h"

LINK_ENTITY_TO_CLASS(info_player_deathmatch,CTeamSpawn );
LINK_ENTITY_TO_CLASS(info_player_start,CTeamSpawn )
LINK_ENTITY_TO_CLASS( info_team_spawn, CTeamSpawn );

void CTeamSpawn::KeyValue( KeyValueData *pkvd )
{
	// check the values
	if( FStrEq( pkvd->szKeyName, "team") )
	{
		pev->team = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

void CTeamSpawn::Spawn( void )
{
	pev->solid = SOLID_NOT;

	if( !stricmp( STRING( pev->classname ), "info_player_start" ) )
	{
		pev->team = TEAM_EARTH;
	}
	else if( !stricmp( STRING( pev->classname ), "info_player_deathmatch" ) )
	{
		pev->team = TEAM_SPACE;
	}

	pev->classname = MAKE_STRING( "info_team_spawn" );

//	ALERT( at_console, "SPAWNS %i\n", pev->team );
}

/******************************************************************
*				MECHMOD SEIGE GAMERULES
*******************************************************************
*	Author:		Jason "ssjason123" Matson
*	Created:	1-28-04
*	Purpose:	Seige gamemode rules.
******************************************************************/
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"
#include	"weapons.h"
#include	"gamerules.h"
#include	"teamplay_gamerules.h"
#include	"mmseige_gamerules.h"
#include	"game.h"
#include	"team_spawns.h"
#include	"voice_gamemgr.h"
#include	"mm_func_obj.h"
#include	"mmweaponents.h"
#include	"decals.h"

extern DLL_GLOBAL BOOL	g_fGameOver;
extern CVoiceGameMgr	g_VoiceGameMgr;
extern int				gmsgTeamInfo;
extern int				gmsgDecalMsg;


CMechModSeige::CMechModSeige( void )
{
	//constructor
	ResetData( );
}

CMechModSeige::~CMechModSeige( void )
{
	// destructor
}

BOOL CMechModSeige::IsSeige( void )
{
	// We are playing a seige game
	return TRUE;
}

void CMechModSeige::Think( void )
{
	g_VoiceGameMgr.Update(gpGlobals->frametime);

	if ( g_fGameOver )   // someone else quit the game already
	{
		CHalfLifeMultiplay::Think();
		return;
	}

	// Heres where we have the meat of the gameplay

	bool blResetRound = false;
	// we have to check to see if a teams building ents are dead or not
	for( int iTeams = TEAM_EARTH; iTeams <= TEAM_SPACE; iTeams ++ )
	{
		bool blEnt = false;
		// loop through each team and look for its objective entitys
		CBaseEntity *pEnt = NULL;
		pEnt = UTIL_FindEntityByClassname( pEnt, "func_objective" );
		while( pEnt && !blEnt )
		{
			CMMObjective *pObj = ( CMMObjective  * ) pEnt;
			blEnt = pObj->IsTeamAlive( iTeams );
			pEnt = UTIL_FindEntityByClassname( pEnt, "func_objective" );
		}
		
		if( !blEnt )
		{
			m_blCanSpawn[iTeams] = false;
		}

		// if a team cant spawn there is no objectives left
		if( !m_blCanSpawn[iTeams] )
		{
			int iTeamates = 0, iDead = 0;
			// if all members of a team are dead round ends
			for( int iIndex = 0; iIndex <= gpGlobals->maxClients; iIndex ++ )
			{
				pEnt = UTIL_PlayerByIndex( iIndex );
				if( pEnt )
				{
					if( pEnt->pev->team == iTeams )
					{
						// if we have a player check if all players on a team are dead
						iTeamates ++;

						if( pEnt->IsAlive( ) == FALSE )
						{
							iDead++;
						}	
					}
				}
			}
			if( iDead == iTeamates )
			{
				blResetRound = true;
			}
		}		
	}

	
	// if they are all disabled remove any blocking ents

	// if roundtime is up reset the round
	float flRoundTime = ( CVAR_GET_FLOAT( "mp_roundtime" ) * 60 );

	if( flRoundTime && flRoundTime + m_flStartTime < gpGlobals->time )
	{
		// reset the round
		ResetRound( );
	}
	else if( blResetRound && m_blRoundStart )
	{
		m_flStartTime = gpGlobals->time - flRoundTime + 2;		// 2 is a round restart delay
		m_blRoundStart = false;
	}
	
	float flTimeLimit = CVAR_GET_FLOAT("mp_timelimit") * 60;

	if ( flTimeLimit != 0 && gpGlobals->time >= flTimeLimit )
	{
		GoToIntermission();
		return;
	}
}

BOOL CMechModSeige::IsAllowedToSpawn( CBaseEntity *pEntity )
{
	// Check if player is allowed to spawn

	// if the buildings are around for players team allow them to respawn
	if( pEntity )
	{
		if( pEntity->IsPlayer( ) )
		{
			// make sure they are a player before casting them
			CBasePlayer *pPlayer = ( CBasePlayer *) pEntity;

			if( pPlayer )
			{
				// check there team
				if( pPlayer->pev->team == TEAM_EARTH )
				{
					// check against the objectives
					if( m_blCanSpawn[pPlayer->pev->team] )
					{
						return TRUE;
					}
					else
					{
						pPlayer->m_iPrevTeam = pPlayer->pev->team;
						pPlayer->StartObserver( pPlayer->pev->origin, pPlayer->pev->angles );
						return TRUE;
					}
				}
				else if( pPlayer->pev->team == TEAM_SPACE )
				{
					// check against the objectives
					if( m_blCanSpawn[pPlayer->pev->team] )
					{
						return TRUE;
					}
					else
					{
						pPlayer->m_iPrevTeam = pPlayer->pev->team;
						pPlayer->StartObserver( pPlayer->pev->origin, pPlayer->pev->angles );
						return TRUE;
					}
				}
				else
				{
					// player must be an observer let em spawn
					return TRUE;
				}
			}
		}
	}

	return TRUE;
}

void CMechModSeige::ResetRound( void )
{
	
	ALERT( at_console, "Round Reset\n" );
	// Reset our main vars
	ResetData( );

	// Reset any entitys
	CBaseEntity *pEnt = NULL;
	pEnt = UTIL_FindEntityByClassname( pEnt, "func_objective" );
	while( pEnt )
	{
		CMMObjective *pObj = ( CMMObjective  * ) pEnt;
		pObj->Reset( );
		pEnt = UTIL_FindEntityByClassname( pEnt, "func_objective" );
	}

	// reset missles
	pEnt = UTIL_FindEntityByClassname( pEnt, "missle" );
	while( pEnt )
	{
		CBaseMissle *pMissle = ( CBaseMissle *) pEnt;
		pMissle->Remove( );
		pEnt = NULL;
		pEnt = UTIL_FindEntityByClassname( pEnt, "missle" );
	}
	

	// reset players
	for( int iIndex = 0; iIndex < gpGlobals->maxClients; iIndex ++ )
	{
		pEnt = UTIL_PlayerByIndex( iIndex );
		if( pEnt )
		{
			CBasePlayer *pPlayer = ( CBasePlayer * ) pEnt;
			// Switch Teams
			if( pEnt->pev->team == TEAM_EARTH )
			{
				pEnt->pev->team = TEAM_SPACE;
				strcpy( pPlayer->m_szTeamName, TEAM2_NAME );
				pPlayer->DisableSpecialModes( );
			}
			else if( pEnt->pev->team == TEAM_SPACE )
			{
				pEnt->pev->team = TEAM_EARTH;
				strcpy( pPlayer->m_szTeamName, TEAM1_NAME );
				pPlayer->DisableSpecialModes( );
			}
			else 
			{
				if( pPlayer->m_iPrevTeam == TEAM_EARTH )
				{
					pPlayer->StopObserver( );
					pEnt->pev->team = TEAM_SPACE;
					strcpy( pPlayer->m_szTeamName, TEAM2_NAME );
					pPlayer->DisableSpecialModes( );
				}
				else if( pPlayer->m_iPrevTeam == TEAM_SPACE )
				{
					pPlayer->StopObserver( );
					pEnt->pev->team = TEAM_EARTH;
					strcpy( pPlayer->m_szTeamName, TEAM1_NAME );
					pPlayer->DisableSpecialModes( );
				}
			}

			MESSAGE_BEGIN( MSG_ALL, gmsgTeamInfo );
				WRITE_BYTE( iIndex );
				WRITE_STRING( pPlayer->m_szTeamName );
			MESSAGE_END();
			pPlayer->UpdateScoreBoard();

			pEnt->Spawn( );
		}
	}

	// Reset Decals
	MESSAGE_BEGIN( MSG_ALL, gmsgDecalMsg );
		WRITE_BYTE( 1 );
	MESSAGE_END( );
	// Remove dead bodies
	pEnt = NULL;
	pEnt = UTIL_FindEntityByClassname( pEnt, "bodyque" );
	while( pEnt )
	{
		UTIL_Remove( pEnt );
		pEnt = UTIL_FindEntityByClassname( pEnt, "bodyque" );
	}
}

void CMechModSeige::ResetData( void )
{
	m_flStartTime = gpGlobals->time;
	for( int iCount = 0; iCount < MAX_TEAMS; iCount ++ )
	{
		m_blCanSpawn[iCount] = true;
	}
	m_blRoundStart = true;
}
/******************************************************************
*				MECHMOD FUNC OBJECTIVE
*******************************************************************
*	Author:		Jason "ssjason123" Matson
*	Created:	1-28-04
*	Purpose:	Objective entity for Seige game rules
******************************************************************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "mm_func_obj.h"
#include "game.h"
#include "gamerules.h"
#include "player.h"
#include "../cl_dll/eff_def.h"

extern DLL_GLOBAL Vector		g_vecAttackDir;
extern int						gmsgPartMsg;


LINK_ENTITY_TO_CLASS( func_objective, CMMObjective );

void CMMObjective::Spawn( void )
{
	Precache( );
	// spawn the ent
	pev->solid		= SOLID_BSP;
    pev->movetype	= MOVETYPE_PUSH;
	pev->takedamage	= DAMAGE_YES;

	SET_MODEL(ENT(pev), STRING(pev->model) );

	Reset( );
}

void CMMObjective::Precache( void )
{
	m_idShard = PRECACHE_MODEL( "models/metalplategibs.mdl" );
}

void CMMObjective::Reset( void )
{
	pev->effects	= 0;
	pev->health		= m_iLife;
	pev->team		= m_iTeam;
	pev->solid		= SOLID_BSP;
    pev->movetype	= MOVETYPE_PUSH;
	pev->takedamage	= DAMAGE_YES;
	m_blAlive		= true;
}

void CMMObjective::Die( void )
{
	// no longer act as if the objective is in the world
	pev->solid		= SOLID_NOT;
	pev->effects	= EF_NODRAW;
	m_blAlive		= false;

	char cFlag		= BREAK_METAL;
	// TODO: add destroy effects

	vec3_t vecSpot = pev->origin + (pev->mins + pev->maxs) * 0.5;
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, vecSpot );
		WRITE_BYTE( TE_BREAKMODEL);

		// position
		WRITE_COORD( vecSpot.x );
		WRITE_COORD( vecSpot.y );
		WRITE_COORD( vecSpot.z );

		// size
		WRITE_COORD( pev->size.x);
		WRITE_COORD( pev->size.y);
		WRITE_COORD( pev->size.z);

		// velocity
		WRITE_COORD( 0 ); 
		WRITE_COORD( 0 );
		WRITE_COORD( 0 );

		// randomization
		WRITE_BYTE( 10 ); 

		// Model
		WRITE_SHORT( m_idShard );	//model id#

		// # of shards
		WRITE_BYTE( 0 );	// let client decide

		// duration
		WRITE_BYTE( 25 );// 2.5 seconds

		// flags
		WRITE_BYTE( cFlag );
	MESSAGE_END();

	MESSAGE_BEGIN( MSG_ALL, gmsgPartMsg );
		WRITE_SHORT( entindex() );
		WRITE_SHORT( EFF_WASTEDEXP );
		WRITE_COORD( vecSpot.x );
		WRITE_COORD( vecSpot.y );
		WRITE_COORD( vecSpot.z );
		WRITE_COORD( pev->size.Length( ) );
	MESSAGE_END( );
}

void CMMObjective::KeyValue( KeyValueData *pkvd )
{
	// life for objective based objects
	if( FStrEq( pkvd->szKeyName, "life" ) )
	{
		m_iLife = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "iteam" ) )
	{
		m_iTeam = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "ipoints" ) )
	{
		m_iPoints = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

int CMMObjective::TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, 
			    float flDamage, int bitsDamageType )
{
	// dont allow team to dmg there own 
	vec3_t vecTemp = pevInflictor->origin - ( pev->absmin + ( pev->size * 0.5 ) );

	g_vecAttackDir = vecTemp.Normalize( );

	if( pevInflictor->team == pev->team || pevAttacker->team == pev->team )
	{
		flDamage = 0;
	}

	pev->health -= flDamage;

	if( pev->health <= 0 )
	{
		// Kill the entity in a nonviolent way
		pevAttacker->frags += m_iPoints;
		Die( );
		// Pcjoe: Update score board
		CBasePlayer *pPlayer = ( CBasePlayer * )Instance( pevAttacker );
		pPlayer->UpdateScoreBoard();
		return 1;
	}

	return 1;
}

bool CMMObjective::IsTeamAlive( int iTeam )
{
	if( iTeam == m_iTeam )
	{
		return m_blAlive;
	}
	return false;
}
// added by jason 4-19-04
class CEnvModel : public CBaseEntity
{
public:
	void Spawn ( void );
};


LINK_ENTITY_TO_CLASS( env_model, CEnvModel );

void CEnvModel :: Spawn( )
{
	pev->movetype	= MOVETYPE_NONE;
	pev->solid		= SOLID_BBOX;
	pev->health		= 0;
	pev->takedamage = DAMAGE_NO;
	pev->effects		= 0;
	pev->yaw_speed		= 0;
	pev->sequence		= 0;
	pev->frame			= 0;

//	pev->nextthink += 1.0;
//	SetThink (WalkMonsterDelay);

	//ResetSequenceInfo( );
	pev->frame = 0;
	
	PRECACHE_MODEL((char *)STRING(pev->model));
	SET_MODEL(ENT(pev),	STRING(pev->model));
	//MonsterInit();
}

LINK_ENTITY_TO_CLASS( func_wind, CNewFuncWind );

void CNewFuncWind::Spawn( void )
{
	Precache( );
	// spawn the brush
	pev->movetype	= MOVETYPE_NONE;
	pev->solid		= SOLID_NOT;
	pev->skin		= CONTENTS_WIND;
	SET_MODEL( ENT(pev), STRING( pev->model ) );
	UTIL_SetOrigin(pev, pev->origin);

	pev->rendermode		= kRenderTransAlpha;
	pev->renderamt		= 0;
}

void CNewFuncWind::Precache( void )
{
	PRECACHE_MODEL("models/null.mdl");
}


void CNewFuncWind::KeyValue( KeyValueData *pkvd )
{
	//extract all our data
	if( FStrEq( pkvd->szKeyName, "random" ) )
	{
		pev->fuser1 = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if( FStrEq( pkvd->szKeyName, "speed" ) )
	{
		pev->fuser2 = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

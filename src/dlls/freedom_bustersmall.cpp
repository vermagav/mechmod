/*
* Freedom rapid fire
* @version: 10/22/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "mmweapons.h"

LINK_ENTITY_TO_CLASS( weapon_freedomrapid, CFreedom_BusterSmall );

/*
* Custom Spawn
*/
void CFreedom_BusterSmall::CustomSpawn( void )
{	SetClassname( "weapon_freedomrapid" );
	SetV_model( "models/null.mdl" );
	SetP_model( "models/p_freedom_gun.mdl" );
	SetAnimPrefix( "rifle" );
	SetDamage( 40 );
	SetBulletType ( BULLET_PLAYER_MP5 );
}

/*
* Precache
*/
void CFreedom_BusterSmall::Precache( )
{	m_iShell = PRECACHE_MODEL( "models/shell.mdl" );
	m_event = PRECACHE_EVENT( 1, "events/rapidbulletfire.sc" );
	PRECACHE_SOUND ("weapons/rapid_fire.wav");
	PRECACHE_MODEL("models/p_freedom_gun.mdl");
}

/*
* Emit Tracer FX
*/
void CFreedom_BusterSmall::EmitPrimaryTracerFX( int flags, Vector vecDir )
{	
	// modified by grOOvy: 29/10/04
	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_event, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 3, 0, 0 );
}

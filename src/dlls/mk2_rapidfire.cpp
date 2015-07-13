/*
* MK2 rapid fire
* @version: 10/24/04
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

LINK_ENTITY_TO_CLASS( weapon_mk2rapid, CMK2_RapidFire );

/*
* Custom Spawn
*/
void CMK2_RapidFire::CustomSpawn( void )
{	SetClassname( "weapon_mk2rapid" );
	SetAnimPrefix( "topcannon" ); // modifed by grOOvy: 28/10/04
	SetDamage( 30 );
	SetBulletType ( BULLET_PLAYER_MP5 );
}

/*
* Precache
*/
void CMK2_RapidFire::Precache( )
{	m_iShell = PRECACHE_MODEL( "models/shell.mdl" );
	m_event = PRECACHE_EVENT( 1, "events/rapidbulletfire.sc" );
	PRECACHE_SOUND ("weapons/rapid_fire.wav");
}

/*
* Emit Tracer FX
*/
void CMK2_RapidFire::EmitPrimaryTracerFX( int flags, Vector vecDir )
{	
	// modified by grOOvy: 29/10/04
	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_event, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 2, 0, 0 );
}
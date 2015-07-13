/*
* VF1J rapid fire
* @version: 1/15/04
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

LINK_ENTITY_TO_CLASS( weapon_vf1jrapid, CVf1j_RapidFire );

/*
* Custom Spawn
*/
void CVf1j_RapidFire::CustomSpawn( void )
{	SetClassname( "weapon_vf1jrapid" );
	SetV_model( "models/null.mdl" );
	SetP_model( "models/p_vf1j_gunpod.mdl" );
	SetAnimPrefix( "gunpod" );
	// added by grOOvy: 12 July 2004
	SetDamage( 16 );
	SetBulletType ( BULLET_PLAYER_MP5 );
}

/*
* Precache
*/
void CVf1j_RapidFire::Precache( )
{	PRECACHE_MODEL("models/p_vf1j_gunpod.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");              
	PRECACHE_SOUND("items/clipinsert1.wav");
	PRECACHE_SOUND("items/cliprelease1.wav");
	PRECACHE_SOUND ("weapons/hks1.wav");// H to the K
	PRECACHE_SOUND ("weapons/hks2.wav");// H to the K
	PRECACHE_SOUND ("weapons/hks3.wav");// H to the K
	PRECACHE_SOUND ("weapons/rapid_fire.wav"); // added by grOOvy
	PRECACHE_SOUND ("weapons/357_cock1.wav");
	m_iShell = PRECACHE_MODEL ("models/shell.mdl");// brass shellTE_MODEL
	m_event = PRECACHE_EVENT( 1, "events/rapidbulletfire.sc" );
}

/*
* Emit Tracer FX
*/
void CVf1j_RapidFire::EmitPrimaryTracerFX( int flags, Vector vecDir )
{	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_event, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0 );
}
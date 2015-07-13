/**********************************************************
*			DEATHSCYTHE SHIELD GUN ATTACK
***********************************************************
*	Purpose: Deathscythes Shield Gun Attack
*	Created On: 12th July 2004
*	Last Edited: 25th October 2004
*	Created By: Gav 'grOOvy' Verma
*	Created For: MechMod
**********************************************************/

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

LINK_ENTITY_TO_CLASS( weapon_dsshieldgun, CDS_ShieldGun );

/*
* Custom Spawn
*/
void CDS_ShieldGun::CustomSpawn( void )
{	SetClassname( "weapon_dsshieldgun" );
	SetV_model( "models/null.mdl" );
	SetAnimPrefix( "shield" );
	SetDamage( 25 );
	SetP_model( "models/p_deathscythe_scythe.mdl" ); // added by grOOvy 29/10/04
	SetBulletType ( BULLET_PLAYER_BUCKSHOT );
}

/*
* Precache
*/
void CDS_ShieldGun::Precache( )
{	
	PRECACHE_SOUND("items/9mmclip1.wav");              
	PRECACHE_SOUND("items/clipinsert1.wav");
	PRECACHE_SOUND("items/cliprelease1.wav");
	PRECACHE_SOUND ("weapons/hks1.wav");
	PRECACHE_SOUND ("weapons/hks2.wav");
	PRECACHE_SOUND ("weapons/hks3.wav");
	PRECACHE_SOUND ("weapons/rapid_fire.wav");
	PRECACHE_SOUND ("weapons/357_cock1.wav");
	m_iShell = PRECACHE_MODEL ("models/shell.mdl");// brass shellTE_MODEL
	m_event = PRECACHE_EVENT( 1, "events/rapidbulletfire.sc" );
	PRECACHE_SOUND("weapons/laser_fire.wav");
}


/*
* Emit Tracer FX
*/
void CDS_ShieldGun::EmitPrimaryTracerFX( int flags, Vector vecDir )
{	
	// modified by grOOvy: 29/10/04
	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_event, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 2, 1, 0, 0 );
}

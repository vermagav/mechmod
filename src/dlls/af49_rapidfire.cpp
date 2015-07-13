/**********************************************************
*			AF49 RAPID FIRE WEAPON
***********************************************************
*	Purpose: AF-49 Silver Axe Rapid Fire weapon
*	Created On: 2/27/04
*	Last Edited: 2/27/04
*	Created By: Jason "ssjason123" Matson
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

LINK_ENTITY_TO_CLASS( weapon_af49rapid, CAf49_RapidFire ); 

void CAf49_RapidFire::CustomSpawn( void )
{
	SetClassname( "weapon_af49rapid" );
	SetV_model( "models/null.mdl" );
	SetAnimPrefix( "axm" );
	// added by grOOvy: 12 July 2004
	SetDamage( 16 );
	SetBulletType ( BULLET_PLAYER_MP5 );
}

void CAf49_RapidFire::Precache( void )
{
	// Precache models here woo
	PRECACHE_MODEL( "models/p_silveraxe_axegun.mdl" );
	m_iShell = PRECACHE_MODEL( "models/shell.mdl" );
	m_event = PRECACHE_EVENT( 1, "events/rapidbulletfire.sc" );
	PRECACHE_SOUND ("weapons/rapid_fire.wav"); // added by grOOvy
}

void CAf49_RapidFire::EmitPrimaryTracerFX( int flags, Vector vecDir )
{
	// second to last value is which side the bullets come from on the gun yay
	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_event, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 1, 0 );
}

/**********************************************************
*			FREEDOM BUSTER CHARGED CANNON WEAPON
***********************************************************
*	Purpose: Freedom Buster Charged Cannon
*	Created On: 25 October 2004
*	Last Edited: 25 October 2004
*	Created By: Gav "grOOvy" Verma
*	Created For: MechMod
**********************************************************/

// Note to Jason: Currently I've set this up as a copy of the Alpha bomb. Please modify as you need to.

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
#include "../cl_dll/ent_trail_def.h"

#ifndef CLIENT_DLL
extern int gmsgEntTrail;
#endif

LINK_ENTITY_TO_CLASS( weapon_freedombig, CFreedom_BusterBig );

/*
* Custom Spawn
*/
void CFreedom_BusterBig::CustomSpawn( void )
{	SetClassname( "weapon_freedombig" );
	SetV_model( "models/w_alphabomb.mdl" );
	SetP_model( "models/p_freedom_gun.mdl" );
	SetAnimPrefix( "rifle" );  // modifed by grOOvy: 28/10/04
}

/*
* Precache
*/
void CFreedom_BusterBig::Precache( )
{	PRECACHE_MODEL("models/p_freedom_gun.mdl");
	PRECACHE_SOUND("weapons/buster_fire.wav");
	PRECACHE_SOUND("weapons/buster_charge.wav");
//	PRECACHE_MODEL("sprites/alphacharge.spr" );
}

/*
* Fire Attack
*/
void CFreedom_BusterBig::FireAttack( float flCharged )
{	
#ifndef CLIENT_DLL
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	// Buster Cannon fire

	
	CBaseEntity *pEnt = CBusterBig::CreateBusterBig( m_pPlayer->pev->origin, gpGlobals->v_forward, m_pPlayer, flCharged );
	AddEffects( pEnt );
	// Emit sound
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/buster_fire.wav", 1, ATTN_NORM);
#endif
}

void CFreedom_BusterBig::StartChargeFunc( void )
{
#ifndef CLIENT_DLL
	// add sprite attachment
	if( !m_pCharge )
	{	
		// Emit sound
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/buster_charge.wav", 1, ATTN_NORM);
		m_pCharge = CSprite::SpriteCreate( "sprites/alphacharge.spr", m_pPlayer->pev->origin, TRUE );
		m_pCharge->pev->health = 100;
		m_pCharge->pev->framerate = 10.0f;
		m_pCharge->SetThink( &CSprite::AnimateThink );
		m_pCharge->pev->nextthink = gpGlobals->time;
		m_pCharge->SetTransparency( kRenderTransAdd, 0, 0, 0, 0, kRenderFxNone );

		m_pCharge->SetAttachment( m_pPlayer->edict(), 1 );
		m_pCharge->SetScale( 0.0f );
	}
#endif
}

void CFreedom_BusterBig::EndChargeFunc( void )
{
#ifndef CLIENT_DLL
	// remove sprite attachment
	if( m_pCharge )
	{
		UTIL_Remove( m_pCharge );
		m_pCharge = NULL;
		// Stop charge sound from playing
		STOP_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/buster_charge.wav"); 

	}
#endif
}


/*
* Add special effects ( trails, head gfx )
*/
void CFreedom_BusterBig::AddEffects( CBaseEntity *pEnt )
{	
#ifndef CLIENT_DLL
	// Generate trail
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( pEnt->entindex( ) );	// entity
		WRITE_BYTE( 1 );			// living status
		WRITE_SHORT( kRenderTransAdd );	// rendermode
		WRITE_COORD( 14 );				// start size
		WRITE_COORD( 14 );				// end size
		WRITE_COORD( 80 );				// # of links
		WRITE_COORD( 55 );				// link length
		WRITE_BYTE( 255 );				// color r
		WRITE_BYTE( 255 );				// color g
		WRITE_BYTE( 255 );				// color b
		WRITE_STRING( "sprites/blueredbeam.spr" );	// sprite name
		WRITE_SHORT( TRAIL_BEAM );	// type of trail
	MESSAGE_END( );
#endif
}

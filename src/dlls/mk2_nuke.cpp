/**********************************************************
*			MK-2 MONSTER NUKE MISSILE WEAPON
***********************************************************
*	Purpose: MK-2 MONSTER Nuke Missile
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

LINK_ENTITY_TO_CLASS( weapon_mk2nuke, CMK2_NukeMissile );

/*
* Custom Spawn
*/
void CMK2_NukeMissile::CustomSpawn( void )
{	SetClassname( "weapon_mk2nuke" );
	SetV_model( "models/null.mdl" );
	SetAnimPrefix( "nuke" ); // added by grOOvy: 28/10/04
}

/*
* Precache
*/
void CMK2_NukeMissile::Precache( )
{	
	PRECACHE_MODEL( "models/w_nuke.mdl" );
	PRECACHE_SOUND("weapons/nuclear_fire.wav");
	PRECACHE_SOUND("weapons/nuclear_charge.wav");
	//PRECACHE_MODEL("sprites/alphacharge.spr" );
}

/*
* Fire Attack
*/
void CMK2_NukeMissile::FireAttack( float flCharged )
{	

#ifndef CLIENT_DLL
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	// Nuke Missile Fire - let's fire! \o/
	CMKNuke::CreateNuke( m_pPlayer->pev->origin, gpGlobals->v_forward, m_pPlayer, flCharged );
	// Emit sound
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/nuclear_fire.wav", 1, ATTN_NORM);
#endif

}

void CMK2_NukeMissile::StartChargeFunc( void )
{
#ifndef CLIENT_DLL
	// add sprite attachment
	// removed by jason
	if( !m_pCharge )
	{	
		// Emit sound
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/nuclear_charge.wav", 1, ATTN_NORM);
		m_pCharge = CSprite::SpriteCreate( "sprites/alphacharge.spr", m_pPlayer->pev->origin, TRUE );
		m_pCharge->pev->health = 100;
		m_pCharge->pev->framerate = 10.0f;
		m_pCharge->SetTransparency( kRenderTransAdd, 0, 0, 0, 0, kRenderFxNone );

		m_pCharge->SetAttachment( m_pPlayer->edict(), 1 );
		m_pCharge->SetScale( 0.0f );
	}
#endif
}

void CMK2_NukeMissile::EndChargeFunc( void )
{
#ifndef CLIENT_DLL
	// remove sprite attachment
	// removed by jason
	
	if( m_pCharge )
	{
		UTIL_Remove( m_pCharge );
		m_pCharge = NULL;
		// Stop charge sound from playing
		STOP_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/nuclear_charge.wav"); 
	}
#endif
}

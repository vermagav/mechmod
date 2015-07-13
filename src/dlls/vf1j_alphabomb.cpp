/*
* VF1J alpha bomb
* @version: 1/31/04
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

LINK_ENTITY_TO_CLASS( weapon_vf1jalpha, CVf1j_AlphaBomb );

/*
* Custom Spawn
*/
void CVf1j_AlphaBomb::CustomSpawn( void )
{	SetClassname( "weapon_vf1jalpha" );
	SetV_model( "models/null.mdl" );
	SetP_model( "models/p_vf1j_gunpod.mdl" );
	SetAnimPrefix( "gunpod" );
}

/*
* Precache
*/
void CVf1j_AlphaBomb::Precache( )
{	PRECACHE_MODEL("models/p_vf1j_gunpod.mdl");
	PRECACHE_SOUND("weapons/alphabomb_fire.wav"); // added by grOOvy
	PRECACHE_MODEL("sprites/alphacharge.spr" );	// added by jason
}

/*
* Fire Attack
*/
void CVf1j_AlphaBomb::FireAttack( float flCharged )
{	
#ifndef CLIENT_DLL
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	// Alphabomb fire
	CAlphaBomb::CreateAlphaBomb( m_pPlayer->pev->origin, gpGlobals->v_forward, m_pPlayer, flCharged );
	// Emit sound
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/alphabomb_fire.wav", 1, ATTN_NORM);
#endif
}
// added by jason
void CVf1j_AlphaBomb::StartChargeFunc( void )
{
#ifndef CLIENT_DLL
	// add sprite attachment
	if( !m_pCharge )
	{	
		m_pCharge = CSprite::SpriteCreate( "sprites/alphacharge.spr", m_pPlayer->pev->origin, TRUE );
		m_pCharge->pev->health = 100;
		m_pCharge->pev->framerate = 10.0f;
		m_pCharge->SetThink( &CSprite::AnimateThink );
		m_pCharge->pev->nextthink = gpGlobals->time;
		m_pCharge->SetTransparency( kRenderTransAdd, 255, 255, 255, 255, kRenderFxNone );

		m_pCharge->SetAttachment( m_pPlayer->edict(), 1 );
		m_pCharge->SetScale( .25f );
	}
#endif
}

void CVf1j_AlphaBomb::EndChargeFunc( void )
{
#ifndef CLIENT_DLL
	// remove sprite attachment
	if( m_pCharge )
	{
		UTIL_Remove( m_pCharge );
		m_pCharge = NULL;
	}
#endif
}

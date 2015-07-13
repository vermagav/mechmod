/**********************************************************
*			AF49 WASTED MISSLE WEAPON
***********************************************************
*	Purpose: AF-49 Silver Axe wasted missle
*	Created On: 2/29/04
*	Last Edited: 2/29/04
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

LINK_ENTITY_TO_CLASS( weapon_af49wasted, CAf49_WastedMissle );

void CAf49_WastedMissle::CustomSpawn( void )
{
	SetClassname( "weapon_af49wasted" );
	SetV_model( "models/null.mdl" );
}

void CAf49_WastedMissle::Precache( void )
{
	// put sounds and models here
	PRECACHE_SOUND("weapons/wastedmissile_fire.wav"); // added by grOOvy
}

void CAf49_WastedMissle::FireAttack( int iIndex )
{
#ifndef CLIENT_DLL
	CBaseEntity *pEntity = UTIL_PlayerByIndex(iIndex);
	if(pEntity)
	{	// Get velocity
		Vector vecVelocity = (pEntity->pev->origin - m_pPlayer->pev->origin).Normalize();

		// need to create an entity for the missle
		CWastedMissle::CreateMissle( m_pPlayer->pev->origin, vecVelocity, (CBasePlayer*)pEntity, m_pPlayer );

		// grOOvy: Wasted Missle sound on fire
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/wastedmissile_fire.wav", 1, ATTN_NORM);
	}
#endif
}

/**********************************************************
*			MK-2 MONSTER ROCKET BARRELS WEAPON
***********************************************************
*	Purpose: MK-2 MONSTER Rocket Barrels
*	Created On: 25 October 2004
*	Last Edited: 25 October 2004
*	Created By: Gav "grOOvy" Verma
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

LINK_ENTITY_TO_CLASS( weapon_mk2barrels, CMK2_RocketBarrels ); 

void CMK2_RocketBarrels::CustomSpawn( void )
{	SetClassname( "weapon_mk2barrels" );
	SetAnimPrefix( "armcannon_rocket" ); // added by grOOvy: 28/10/04
	SetV_model( "models/null.mdl" );
}

void CMK2_RocketBarrels::Precache( void )
{	
	PRECACHE_SOUND("weapons/barrels_fire.wav");
}

void CMK2_RocketBarrels::FireAttack( int iIndex )
{
	// Joes missle code
#ifndef CLIENT_DLL
//	ALERT(at_console,"Fire Missle at index %i\n", iIndex);
	CBaseEntity *pEntity = UTIL_PlayerByIndex(iIndex);
	if(pEntity)
	{	// Get velocity
		Vector vecVelocity = (pEntity->pev->origin - m_pPlayer->pev->origin).Normalize();
		UTIL_MakeVectors(UTIL_VecToAngles(vecVelocity));

		// Get some upwards velocity
		Vector vecUp = (vecVelocity * 0.3 + gpGlobals->v_up * 0.7);

		// Get some right velocity
		Vector vecRight = (gpGlobals->v_right * RANDOM_FLOAT(-1,1));

		// Add together
		vecVelocity = vecUp * 0.5 + vecRight * 0.5;
		vecVelocity = vecVelocity.Normalize();

		// Fire 4 missiles at the same time
		for (int i=0; i<4; i++)
			CGenericMissle::CreateHomingMissle( m_pPlayer->pev->origin, vecVelocity, (CBasePlayer*)pEntity, m_pPlayer);

	}
#endif
}
/*
* VF1J homing missles
* @version: 1/17/04
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

LINK_ENTITY_TO_CLASS( weapon_vf1jhoming, CVf1j_HomingMissles );

/*
* Custom Spawn
*/
void CVf1j_HomingMissles::CustomSpawn( void )
{	SetClassname( "weapon_vf1jhoming" );
}

/*
* Precache
*/
void CVf1j_HomingMissles::Precache( )
{
}

/*
* Called to fire attack
*/
void CVf1j_HomingMissles::FireAttack(int iIndex)
{	
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

		// Create missle
		CGenericMissle::CreateHomingMissle( m_pPlayer->pev->origin, vecVelocity, (CBasePlayer*)pEntity, m_pPlayer);
	}
#endif
}
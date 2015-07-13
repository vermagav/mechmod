/*
* Class system
* @version: January 7th, 2004
* @Author: Joseph Florencio
*/

#include "extdll.h"
#include "util.h"

#include "cbase.h"
#include "player.h"

extern int gmsgMaxHealth;

void CBasePlayerClass::SetModel( void )
{	UTIL_SetPlayerModel(GetModelName(), m_pOwner->pev);
	g_engfuncs.pfnSetClientKeyValue( m_pOwner->entindex(), g_engfuncs.pfnGetInfoKeyBuffer( m_pOwner->edict() ), "model", GetModelName() );
}

void CBasePlayerClass::SetHealth( void )
{	m_pOwner->pev->health = m_pOwner->pev->max_health = MaxHealth();
}
void CBasePlayerClass::SetArmor( void )
{	m_pOwner->pev->armorvalue = MaxArmor();
}

void CBasePlayerClass::SendMaxHealth( void )
{	MESSAGE_BEGIN( MSG_ONE, gmsgMaxHealth, NULL, m_pOwner->pev );
		WRITE_BYTE( MaxHealth() );
		WRITE_BYTE( MaxArmor() );
	MESSAGE_END();
}
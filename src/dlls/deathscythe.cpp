/*
* Deathscythe
* @version: January 7th, 2003
* @Author: Joseph Florencio
*/

#include "extdll.h"
#include "util.h"

#include "cbase.h"
#include "player.h"
#include "weapons.h"

void CDeathscythe::GiveWeapons( void )
{	m_pOwner->GiveNamedItem( "weapon_dsscythe" );
	m_pOwner->GiveNamedItem( "weapon_dsshieldgun" );
}

void CDeathscythe::AddSaberStyles( void )
{	m_pOwner->m_Saber.AddAttribute("scythe");
}
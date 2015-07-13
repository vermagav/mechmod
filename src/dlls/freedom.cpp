/*
* Freedom
* @version: January 7th, 2003
* @Author: Joseph Florencio
*/

#include "extdll.h"
#include "util.h"

#include "cbase.h"
#include "player.h"
#include "weapons.h"

void CFreedom::GiveWeapons(  void )
{	
	m_pOwner->GiveNamedItem( "weapon_freedomsaber" );
	m_pOwner->GiveNamedItem( "weapon_freedomrapid" );	
	m_pOwner->GiveNamedItem( "weapon_freedom5way" );
	m_pOwner->GiveNamedItem( "weapon_freedombig" );
}

void CFreedom::AddSaberStyles( void )
{	m_pOwner->m_Saber.AddAttribute("freedomsaber");
}
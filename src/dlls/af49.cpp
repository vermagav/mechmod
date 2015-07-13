/*
* AF-49 Silver Axe
* @version: January 7th, 2003
* @Author: Joseph Florencio
*/

#include "extdll.h"
#include "util.h"

#include "cbase.h"
#include "player.h"
#include "weapons.h"

void CAf49::GiveWeapons(  void )
{	m_pOwner->GiveNamedItem( "weapon_af49twinblade" );
	// added by jason
	m_pOwner->GiveNamedItem( "weapon_af49rapid" );
	m_pOwner->GiveNamedItem( "weapon_af49homing" );
	m_pOwner->GiveNamedItem( "weapon_af49wasted" );
}

void CAf49::AddSaberStyles( void )
{	m_pOwner->m_Saber.AddAttribute("af49blade");
}
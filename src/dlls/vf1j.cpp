/*
* VF-1J Valkyrie
* @version: January 7th, 2003
* @Author: Joseph Florencio
*/

#include "extdll.h"
#include "util.h"

#include "cbase.h"
#include "player.h"
#include "weapons.h"

void CVf1j::GiveWeapons(  void )
{	m_pOwner->GiveNamedItem( "weapon_vf1jsaber" );
	m_pOwner->GiveNamedItem( "weapon_vf1jrapid" );
	m_pOwner->GiveNamedItem( "weapon_vf1jhoming" );
	m_pOwner->GiveNamedItem( "weapon_vf1jalpha" );
}

void CVf1j::AddSaberStyles( void )
{	m_pOwner->m_Saber.AddAttribute("vf1jsaber");
}
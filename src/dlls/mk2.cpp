/**********************************************************
*	    		MK-2 MONSTER CLASS FILE
***********************************************************
*	Purpose: MK-2 MONSTER Class File
*	Created On: 25 October 2004
*	Last Edited: 25 October 2004
*	Created By: Gav "grOOvy" Verma
*	Created For: MechMod
**********************************************************/


#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"

void CMK2::GiveWeapons( void )
{	
	m_pOwner->GiveNamedItem( "weapon_mk2rapid" );
	m_pOwner->GiveNamedItem( "weapon_mk2homing" );
	m_pOwner->GiveNamedItem( "weapon_mk2barrels" );
	m_pOwner->GiveNamedItem( "weapon_mk2nuke" );

}

void CMK2::AddSaberStyles( void )
{	m_pOwner->m_Saber.AddAttribute("mk2");
}
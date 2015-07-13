/**********************************************************
*			DEATHSCYTHE SCYTHE
***********************************************************
*	Purpose: deathscythes scythe
*	Created On: 4/15/04
*	Last Edited: 4/15/04
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

LINK_ENTITY_TO_CLASS( weapon_dsscythe, CDS_Scythe );

void CDS_Scythe::CustomSpawn( void )
{	SetClassname( "weapon_dsscythe" );
	SetP_model( "models/p_deathscythe_scythe.mdl" );
}

void CDS_Scythe::Precache( void )
{	// put sounds and models here
	PRECACHE_MODEL( "models/p_deathscythe_scythe.mdl" );
}
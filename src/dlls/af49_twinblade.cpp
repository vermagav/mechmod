/*
* AF49 twin blade
* @version: 7/29/04
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

LINK_ENTITY_TO_CLASS( weapon_af49twinblade, CAf49_TwinBlade );

/*
* Custom Spawn
*/
void CAf49_TwinBlade::CustomSpawn( void )
{	SetClassname( "weapon_af49twinblade" );
}

/*
* Precache
*/
void CAf49_TwinBlade::Precache( )
{
}
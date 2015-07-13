/*
* Freedom saber
* @version: 7/19/04
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

LINK_ENTITY_TO_CLASS( weapon_freedomsaber, CFreedom_Saber );

/*
* Custom Spawn
*/
void CFreedom_Saber::CustomSpawn( void )
{	SetClassname( "weapon_freedomsaber" );
	SetP_model( "models/p_freedom_sabre.mdl" );
}

/*
* Precache
*/
void CFreedom_Saber::Precache( )
{	PRECACHE_MODEL("models/p_freedom_sabre.mdl");
}
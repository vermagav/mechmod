/*
* VF1J saber
* @version: 1/31/04
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

LINK_ENTITY_TO_CLASS( weapon_vf1jsaber, CVf1j_Saber );

/*
* Custom Spawn
*/
void CVf1j_Saber::CustomSpawn( void )
{	SetClassname( "weapon_vf1jsaber" );
	SetP_model( "models/p_vf1j_sabre.mdl" );
}

/*
* Precache
*/
void CVf1j_Saber::Precache( )
{	PRECACHE_MODEL("models/p_vf1j_sabre.mdl");
}
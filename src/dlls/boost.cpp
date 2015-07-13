/*
* Boost FX
* @version 12/21/03
* @author: Joseph 'Pcjoe' Florencio
*/

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"weapons.h"
#include	"soundent.h"
#include	"gamerules.h"
#include	"player.h"

extern gmsgBoost;

#define BOOST_DIETIME	3

LINK_ENTITY_TO_CLASS( boostfx, CBoost );

void CBoost :: Spawn( void )
{	Precache();
	pev->movetype	= MOVETYPE_FOLLOW;
	pev->solid		= SOLID_NOT;
	pev->classname = MAKE_STRING( "boostfx" );
	pev->nextthink = gpGlobals->time;
	UTIL_SetSize( pev, Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
}

CBoost *CBoost::CreateBoost( CBasePlayer *pOwner, BOOL fDoubleJump )
{	CBoost *pBoost = GetClassPtr((CBoost *)NULL);
	pBoost->Spawn();
	pBoost->pev->owner = pOwner->edict();
	pBoost->pev->aiment = pOwner->edict();
	pBoost->m_pOwner = pOwner;
	pBoost->m_fDoubleJump = fDoubleJump;

	if(fDoubleJump)
	{	pBoost->SetThink( &CBoost::RemoveThink );
		pBoost->pev->nextthink = gpGlobals->time + BOOST_DIETIME;
	}
	else
	{	pBoost->SetTouch( NULL );
		pBoost->SetThink( NULL );
	}

	// Select + Set Model
	char szModel[128];
	sprintf(szModel,"models/boosters_%s.mdl",pOwner->m_pClass->GetModelName() );
	SET_MODEL(ENT( pBoost->pev ), szModel);

	// Add Boost FX
	pBoost->SendBoostMsg();

	return pBoost;
}

void CBoost::SendBoostMsg( void )
{	MESSAGE_BEGIN( MSG_ALL, gmsgBoost, NULL );
		WRITE_SHORT(entindex());
		WRITE_BYTE(m_pOwner->m_pClass->BoostColor());
		WRITE_BYTE(m_fDoubleJump);
	MESSAGE_END();
}

void CBoost::Precache()
{	PRECACHE_MODEL("models/boosters_vf1j.mdl");
	PRECACHE_MODEL("models/boosters_silveraxe.mdl");
	PRECACHE_MODEL("models/boosters_deathscythe.mdl");
	PRECACHE_MODEL("models/boosters_freedom.mdl");
	PRECACHE_MODEL("models/boosters_monster.mdl");
}

void CBoost::Remove()
{	// Remove Boost FX
	MESSAGE_BEGIN( MSG_ALL, gmsgBoost, NULL );
		WRITE_SHORT(entindex());
		WRITE_BYTE(0);
		WRITE_BYTE(0);
	MESSAGE_END();

	m_pOwner = NULL;
	UTIL_Remove(this);
}

void EXPORT CBoost::RemoveThink( void )
{	if(m_pOwner)
		m_pOwner->m_pBoost = NULL;
	Remove();
}
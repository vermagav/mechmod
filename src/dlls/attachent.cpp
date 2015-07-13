/*
* Attachment Entity
* @version: 8/08/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"

LINK_ENTITY_TO_CLASS( attachent, CAttachEnt );

/*
* Spawn
*/
void CAttachEnt::Spawn( void )
{	Precache();
	pev->movetype	= MOVETYPE_FOLLOW;
	pev->solid		= SOLID_NOT;
	pev->classname = MAKE_STRING( "attachent" );
	pev->nextthink = gpGlobals->time;
	UTIL_SetSize( pev, Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
}

/*
* Create Model
*/
CAttachEnt *CAttachEnt::CreateModel( CBasePlayer *pOwner, char *pModel )
{	CAttachEnt *pAtchMdl = GetClassPtr((CAttachEnt *)NULL);
	// Spawn
	pAtchMdl->Spawn();
	// Set owner
	pAtchMdl->SetAttachedPlayer( pOwner );
	pAtchMdl->pev->owner = pOwner->edict();
	pAtchMdl->pev->aiment = pOwner->edict();
	// Set model
	pAtchMdl->SetModel( pModel );
	// Set Think
	pAtchMdl->SetThink( &CAttachEnt::AnimateThink );
	// Set animation owner
	pAtchMdl->m_Anim.SetOwner( (CBaseAnimating*)pAtchMdl );
	// Return ptr
	return pAtchMdl;
}

/*
* Precache
*/
void CAttachEnt::Precache( void )
{
}

/*
* Animate Think
*/
void CAttachEnt::AnimateThink( void )
{	int animDesired = -1;

	// Advance to next frame
	StudioFrameAdvance();

	// Manage anim list
	m_Anim.ManageAnimList();

	// Get currenta nim
	std::list<entanim_s>::iterator iterAnim = m_Anim.GetCurrentAnim();

	// Get sequence idx of current anim
	if(iterAnim != NULL)
	{	animDesired = m_Anim.SetAnimation(iterAnim);
	}

	// Check to see if we have a current anim
	if(animDesired == -1)
	{	return;
	}

	// set invalid anims to 0
	if(animDesired == -1)
	{	animDesired = 0;
	}

	// Already using the desired animation?
	if (pev->sequence != animDesired)
	{
		// removed by jason
		//	ALERT(at_console,"Anim Changed!\n");
		pev->sequence		= animDesired;
		pev->frame			= 0;
		ResetSequenceInfo( );
	}

	// Set next think
	pev->nextthink = gpGlobals->time + 0.1;
}

/*
* Set Ent Model
*/
void CAttachEnt::SetModel( char *pModel )
{	// Check for valid model
	if(!pModel)
		return;
	// Set model
	UTIL_SetModel( pModel, pev );
	m_Anim.Reset();
}

/*
* Disable attached model (cleanup)
*/
void CAttachEnt::Disable( void )
{	// Reset anims
	m_Anim.Reset();
	// Remove entity
	UTIL_Remove(this);
}
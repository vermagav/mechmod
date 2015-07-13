/*
* Extended Player Animation System
* @version 12/20/03
* @author: Joseph 'Pcjoe' Florencio
*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"

#ifndef CLIENT_DLL
/*
* Remove Weapon Anims
*/
void CPlayerAnimation::RemoveWepAnims( void )
{	RemoveAllAnims( PLAY_MISC );
}

/*
* Add Weapon animation
*/
std::list<entanim_s>::iterator CPlayerAnimation::AddWeaponAnimation( PLAY_ANIM eAnim )
{	CBasePlayer *pOwner = GetOwnerPlr();
	if(!pOwner)
	{	return NULL;
	}
	if(eAnim == PLAY_MISC)
	{	return NULL;
	}

	char szAnim[64];
	BOOL fLoop = FALSE;
	sprintf(szAnim, "%s_", pOwner->m_szAnimExtention);

	switch(eAnim)
	{	
	case PLAY_DRAW:
		strcat(szAnim,"draw");
		break;
	case PLAY_ATTACK1:
		strcat(szAnim,"shoot1");
		break;
	case PLAY_ATTACK2:
		strcat(szAnim,"shoot2");
		break;
	case PLAY_CHARGE1:
		strcat(szAnim,"charge_idle1");
		fLoop = TRUE;
		break;
	case PLAY_CHARGE2:
		strcat(szAnim,"charge_idle2");
		fLoop = TRUE;
		break;
	case PLAY_CHARGE_SHOOT1:
		strcat(szAnim,"charge_shoot1");
		break;
	case PLAY_CHARGE_SHOOT2:
		strcat(szAnim,"charge_shoot2");
		break;
	}

	// Add animation
	std::list<entanim_s>::iterator iterAnim = AddAnimation(fLoop, ANIMTYPE_UPPERONLY, gpGlobals->time, 1, szAnim);
	// Set animation type
	if(iterAnim != NULL)
	{	iterAnim->ePlayAnim = eAnim;
	}
	return iterAnim;
}
#endif
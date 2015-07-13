/*
* Extended Entity Animation System
* @version 8/08/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"


#ifndef CLIENT_DLL
/*
* Reset
*/
void CEntAnimation::Reset( void )
{	
	SetOwner( NULL );
	m_Animations.clear();
}
#endif

CEntAnimation::CEntAnimation()
{	
#ifndef CLIENT_DLL
	Reset();
#endif
}

CEntAnimation::~CEntAnimation()
{	
#ifndef CLIENT_DLL
	Reset();
#endif
}

#ifndef CLIENT_DLL

/*
* Remove All Anims
*/
void CEntAnimation::RemoveAllAnims( void )
{	RemoveAllAnims( PLAY_NONE );
}

/*
* Remove All Anims
*/
void CEntAnimation::RemoveAllAnims( PLAY_ANIM playanim )
{	if(playanim == PLAY_NONE)
	{	m_Animations.clear();
		return;
	}
	std::list<entanim_s>::iterator begin = m_Animations.begin();
	std::list<entanim_s>::iterator end = m_Animations.end();

	while(begin != end)
	{	if(begin->ePlayAnim != playanim)
		{	begin = m_Animations.erase(begin);
			continue;
		}
		begin++;
	}
}

/*
* Manage Animation List
*/
void CEntAnimation::ManageAnimList( void )
{	// Take care of current animation
	ManageCurrentAnim( GetCurrentAnim() );

	std::list<entanim_s>::iterator begin = m_Animations.begin();
	std::list<entanim_s>::iterator end = m_Animations.end();
	std::list<entanim_s>::iterator current = GetCurrentAnim();

	while(begin != end)
	{	// Never delete current animation, delete old anims
		if((begin != current) && (begin->flStartTime < gpGlobals->time))
		{	begin = m_Animations.erase(begin);
			continue;
		}
		begin++;
	}
}


/*
* Manage Current Anim
*/
void CEntAnimation::ManageCurrentAnim( std::list<entanim_s>::iterator iterAnim )
{	if(iterAnim == NULL || !m_pOwner)
	{	return;
	}

	if((m_pOwner->pev->sequence == m_pOwner->LookupSequence(iterAnim->strAnim[iterAnim->iCurAnim].c_str())) &&
		m_pOwner->m_fSequenceFinished)
	{	// Go to next animation
		if(iterAnim->iCurAnim == iterAnim->iMaxAnims-1)
		{	if(!iterAnim->fLoop)
			{	CBasePlayer *pPlayer = GetOwnerPlr();
				// Handle saber animations
				if(pPlayer && iterAnim->fSaber)
				{	std::list<saber_move_s>::iterator iterSaber = pPlayer->m_Saber.GetCurrentSwing();
					if(iterSaber != NULL)
					{//	ALERT(at_console,"Complete saber move!\n");
						iterSaber->fComplete = TRUE;
						pPlayer->m_Saber.ManageSaber();
					}
					else
					{	RemoveAnimation( iterAnim );
					}
				}
				// Delete anim normally
				else
				{	RemoveAnimation( iterAnim );
				}
			}
		}
		else
		{	iterAnim->iCurAnim++;
			SetActivity(iterAnim->eActivity);
		}
	}
}

/*
* Add Animation
*/
std::list<entanim_s>::iterator CEntAnimation::AddAnimation( BOOL fLooping, ANIMTYPE eAnimType, float flTime, int iNum, ... )
{	if(!m_pOwner || iNum <= 0)
	{	return NULL;
	}

	entanim_s NewAnim;
	// Reset Values
	NewAnim.iCurAnim = 0;
	NewAnim.iMaxAnims = iNum;
	NewAnim.flStartTime = flTime;
	NewAnim.fInit = FALSE;
	NewAnim.fLoop = fLooping;
	NewAnim.fSaber = FALSE;
	NewAnim.eActivity = ACT_RESET;
	NewAnim.ePlayAnim = PLAY_MISC;
	NewAnim.ePriority = PRIOR_VERYHIGH;
	NewAnim.eAnimType = eAnimType;

	// Add Animations
	va_list arguments;
	va_start(arguments, iNum);
	for(int i=0;i<iNum;i++)
	{	std::string strAnim( va_arg(arguments, char *) );
		NewAnim.strAnim.push_back( strAnim );
	}

	// Debug - don't add animation if we don't have ALL of the animations
	for(i=0;i<iNum;i++)
	{	if(m_pOwner->LookupSequence( NewAnim.strAnim[i].c_str() ) == -1)
		{	return NULL;
		}
	}
	
	// Add to linked list
	m_Animations.push_front(NewAnim);
	std::list<entanim_s>::iterator iterNewAnim = m_Animations.begin();

	ManageAnimList();

	return iterNewAnim;
}

/*
* Set Priority
*/
void CEntAnimation::SetPriority( PRIORITY_ANIMS priority, std::list<entanim_s>::iterator iterAnim )
{	if(iterAnim != NULL)
	{	iterAnim->ePriority = priority;
	}
}

/*
* Remove Animation
*/
std::list<entanim_s>::iterator CEntAnimation::RemoveAnimation( std::list<entanim_s>::iterator iterAnim )
{	return m_Animations.erase( iterAnim );
}

/*
* Get Current Animation
*/
std::list<entanim_s>::iterator CEntAnimation::GetCurrentAnim( void )
{	float flMaxTime = 0;
	std::list<entanim_s>::iterator iterBegin = m_Animations.begin();
	std::list<entanim_s>::iterator iterEnd = m_Animations.end();
	std::list<entanim_s>::iterator iterCurrent = NULL;

	while(iterBegin != iterEnd)
	{	if((iterBegin->flStartTime <= gpGlobals->time) && (iterBegin->flStartTime > flMaxTime))
		{	iterCurrent = iterBegin;
			flMaxTime = iterBegin->flStartTime;
		}
		iterBegin++;
	}
	return iterCurrent;
}

/*
* Set Animation
*/
int CEntAnimation::SetAnimation( std::list<entanim_s>::iterator iterAnim )
{	if(iterAnim == NULL)
	{	return 0;
	}
	// Init Anim
	InitAnim( iterAnim );

	// Set Sequence and Activity
	int animDesired = m_pOwner->LookupSequence( iterAnim->strAnim[iterAnim->iCurAnim].c_str() );
	if(GetOwnerPlr())
		GetOwnerPlr()->m_IdealActivity = iterAnim->eActivity;

	return animDesired;
}

/*
* Set Activity
*/
void CEntAnimation::SetActivity( Activity &Activity )
{	if(GetOwnerPlr() ? (((CBasePlayer*)m_pOwner)->m_IdealActivity != ACT_ANIMSYSTEM1) : FALSE)
	{	Activity = ACT_ANIMSYSTEM1;
	}
	else
	{	Activity = ACT_ANIMSYSTEM2;
	}

	// Restart sequence
	m_pOwner->pev->frame = 0;
	m_pOwner->ResetSequenceInfo();
}

/*
* Init Anim
*/
void CEntAnimation::InitAnim( std::list<entanim_s>::iterator iterAnim )
{	if(iterAnim != NULL && !iterAnim->fInit)
	{	iterAnim->fInit = TRUE;
		SetActivity(iterAnim->eActivity);
	}
}
#endif

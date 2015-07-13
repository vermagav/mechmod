/*
* Lockon
* @version 1/17/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "triangleapi.h"
#include "pm_defs.h"
#include "event_api.h"

#include <string.h>
#include <stdio.h>

#define LOCK_CLEAR		0
#define LOCK_REMOVE		1
#define LOCK_REMOVEALL	2
#define LOCK_ADD		3
#define PRELOCK_ADD		4
#define MISLOCK_ADD		5

DECLARE_MESSAGE(m_Lockon, Lockon);

int CHudLockon::Init(void)
{	m_iFlags |= HUD_ACTIVE;
	HOOK_MESSAGE(Lockon);
	gHUD.AddHudElem(this);
	for(int i=0; i < MAX_LOCKS; i++)
	{	ResetLockData(i);
	}
	return 1;
}

int CHudLockon::VidInit(void)
{	m_HUD_lockon = gHUD.GetSpriteIndex("lockon");
	m_HUD_prelock = gHUD.GetSpriteIndex("prelock");
	m_HUD_misslelock = gHUD.GetSpriteIndex("misslelock");
	return 1;
}


void CHudLockon::Reset( void )
{	for(int i=0; i < MAX_LOCKS; i++)
	{	ResetLockData(i);
	}
}

int CHudLockon::Draw(float flTime)
{	if ( gEngfuncs.IsSpectateOnly() || !(gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT))))
	{	// HACK HACK: Reset all locks while player is "dead"
		for(int n=0;n<MAX_LOCKS;n++)
		{	if(m_Locks[n].blLock)
			{	ResetLockData(n);
			}
		}
		return 1;
	}

	cl_entity_t *pLocal = gEngfuncs.GetLocalPlayer();
	if(!pLocal)
	{	return 1;
	}

	for(int i=0;i<MAX_LOCKS;i++)
	{	// Draw on hud
		if(m_Locks[i].blLock)
		{	vec3_t vecScreen;
			cl_entity_t *pEntity = gEngfuncs.GetEntityByIndex( m_Locks[i].iIndex );
			if(!pEntity)
			{	continue;
			}

			if(m_Locks[i].iAmt == 0)
			{	ResetLockData(i);
				continue;
			}

			// Get endpos to screen
			gEngfuncs.pTriAPI->WorldToScreen(pEntity->origin, vecScreen);
			// Z is clipped ( entity is not on our screen )
			if(vecScreen.z == -1)
			{	continue;
			}

			// Prelock
			if(m_Locks[i].iAmt == -1)
			{	int iLockWidth = gHUD.GetSpriteRect(m_HUD_prelock).right - gHUD.GetSpriteRect(m_HUD_prelock).left;
				int iLockHeight = gHUD.GetSpriteRect(m_HUD_prelock).bottom - gHUD.GetSpriteRect(m_HUD_prelock).top;
				// Draw Crosshair
				SPR_Set(gHUD.GetSprite(m_HUD_prelock), 255, 255, 255);
				SPR_DrawAdditive(0,  XPROJECT(vecScreen.x)-iLockWidth/2, YPROJECT(vecScreen.y)-iLockHeight/2, &gHUD.GetSpriteRect(m_HUD_prelock));
			}
			// Missle lock
			else if(m_Locks[i].iType == LOCKTYPE_MISSLE)
			{	int iLockWidth = gHUD.GetSpriteRect(m_HUD_misslelock).right - gHUD.GetSpriteRect(m_HUD_misslelock).left;
				int iLockHeight = gHUD.GetSpriteRect(m_HUD_misslelock).bottom - gHUD.GetSpriteRect(m_HUD_misslelock).top;
				// Draw Crosshair
				SPR_Set(gHUD.GetSprite(m_HUD_misslelock), 255, 255, 255);
				SPR_DrawAdditive(0,  XPROJECT(vecScreen.x)-iLockWidth/2, YPROJECT(vecScreen.y)-iLockHeight/2, &gHUD.GetSpriteRect(m_HUD_misslelock));
			}
			// Reg lock
			else
			{	int iLockWidth = gHUD.GetSpriteRect(m_HUD_lockon).right - gHUD.GetSpriteRect(m_HUD_lockon).left;
				int iLockHeight = gHUD.GetSpriteRect(m_HUD_lockon).bottom - gHUD.GetSpriteRect(m_HUD_lockon).top;
				// Draw Crosshair
				SPR_Set(gHUD.GetSprite(m_HUD_lockon), 255, 255, 255);
				SPR_DrawAdditive(m_Locks[i].iAmt-1,  XPROJECT(vecScreen.x)-iLockWidth/2, YPROJECT(vecScreen.y)-iLockHeight/2, &gHUD.GetSpriteRect(m_HUD_lockon));
			}
		}
	}
	return 1;
}

void CHudLockon::AddLock( int idx )
{	int i = FindLock(idx);

	// Already exist, just add a new lock
	if(i != -1)
	{	if(m_Locks[i].iAmt == -1)
		{	m_Locks[i].iAmt = 1;
			// Emit sound
			PlaySound("player/lock_on.wav", 1);
		}
		else
		{	m_Locks[i].iAmt++;
		}

		if(m_Locks[i].iAmt > MAX_LOCKS)
			m_Locks[i].iAmt = MAX_LOCKS;
		return;
	}
	// Add new lock
	else
	{	// Find a free space
		for(i=0;i<MAX_LOCKS;i++)
		{	if(!m_Locks[i].blLock)
			{	break;
			}
		}
		// (impossible), no free lock spaces
		if(i==MAX_LOCKS)
		{	ConsolePrint("Error: No free lock spaces!\n");
			return;
		}

		// Add lock data
		m_Locks[i].blLock = true;
		m_Locks[i].iAmt = 1;
		m_Locks[i].iType = LOCKTYPE_PLAYER;
		m_Locks[i].iIndex = idx;
	}
}


void CHudLockon::AddPreLock( int type, int idx )
{	int i = FindLock(idx);

	// Already exist, reset to prelock status (shouldnt be happening)
	if(i != -1)
	{	m_Locks[i].iAmt = -1;
		return;
	}
	// Add new lock
	else
	{	// Find a free space
		for(i=0;i<MAX_LOCKS;i++)
		{	if(!m_Locks[i].blLock)
			{	break;
			}
		}
		// (impossible), no free lock spaces
		if(i==-1)
		{	ConsolePrint("Error: No free lock spaces!\n");
			return;
		}

		// Add lock data
		m_Locks[i].blLock = true;
		m_Locks[i].iAmt = -1;
		m_Locks[i].iType = type;
		m_Locks[i].iIndex = idx;
	}
}

void CHudLockon::AddMisLock( int idx )
{	int i = FindLock(idx);

	// Already exist, set lock status
	if(i != -1)
	{	m_Locks[i].iType = LOCKTYPE_MISSLE;
		m_Locks[i].iAmt = 1;
		return;
	}
	// Add new lock
	else
	{	// Find a free space
		for(i=0;i<MAX_LOCKS;i++)
		{	if(!m_Locks[i].blLock)
			{	break;
			}
		}
		// (impossible), no free lock spaces
		if(i==-1)
		{	ConsolePrint("Error: No free lock spaces!\n");
			return;
		}

		// Add lock data
		m_Locks[i].blLock = true;
		m_Locks[i].iAmt = 1;
		m_Locks[i].iType = LOCKTYPE_MISSLE;
		m_Locks[i].iIndex = idx;
	}
}

int CHudLockon::FindLock( int idx )
{	for(int i=0;i<MAX_LOCKS;i++)
	{	if(m_Locks[i].blLock && m_Locks[i].iIndex == idx)
		{	return i;
		}
	}

	return -1;
}

void CHudLockon::RemoveLock( bool blAll, int idx )
{	int i = -1;
	// Find lock
	if((i=FindLock(idx)) != -1)
	{	m_Locks[i].iAmt--;
		if((m_Locks[i].iAmt <= 0) || blAll)
		{	ResetLockData(i);
		}
	}
}

void CHudLockon::ResetLockData( int i )
{	m_Locks[i].blLock = false;
	m_Locks[i].iAmt = 0;
	m_Locks[i].iIndex = 0;
}

// BROKEN: No clue, perhaps i should remove instead of attempt to fix...
void CHudLockon::ClearLocks( void )
{	for(int i=0;i<MAX_LOCKS;i++);
	{	m_Locks[i].blLock = false;
		m_Locks[i].iAmt = 0;
		m_Locks[i].iIndex = 0;
	}
//	ConsolePrint("clear locks\n");
}

int CHudLockon::MsgFunc_Lockon(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	int iType = READ_BYTE();
	int idx = READ_BYTE();
	int i;
	switch(iType)
	{	
	case LOCK_CLEAR:
		for(i=0; i < MAX_LOCKS; i++)
		{	ResetLockData(i);
		}
		break;
	case LOCK_REMOVE:
		RemoveLock( false, idx );
		break;
	case LOCK_REMOVEALL:
		RemoveLock( true, idx );
		break;
	case LOCK_ADD:
		AddLock(idx);
		break;
	case PRELOCK_ADD:
		AddPreLock(LOCKTYPE_PLAYER,idx);
		break;
	case MISLOCK_ADD:
		AddMisLock(idx);
		break;
	}
	return 1;
}
/*
* Set Client Speed
* @version 12/16/03
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

int CHudSpeed::Init(void)
{	m_iSpeed = 0;
	m_iFlags |= HUD_ACTIVE;
	gHUD.AddHudElem(this);
	return 1;
}

int CHudSpeed::VidInit(void)
{	return 1;
}

void CHudSpeed::SetSpeed( void )
{	char szSpeed[256];
	// Send client cmds to set speed
	sprintf(szSpeed,"cl_forwardspeed %i",m_iSpeed);
	ClientCmd(szSpeed);

	sprintf(szSpeed,"cl_backspeed %i",m_iSpeed);
	ClientCmd(szSpeed);

	sprintf(szSpeed,"cl_sidespeed %i",m_iSpeed);
	ClientCmd(szSpeed);
}

void CHudSpeed::Think( void )
{	cl_entity_t *pPlayer = gEngfuncs.GetLocalPlayer();
	if(pPlayer)
	{	if(m_iSpeed != pPlayer->curstate.iuser4)
		{	m_iSpeed = pPlayer->curstate.iuser4;
			SetSpeed();
		}
	}
}
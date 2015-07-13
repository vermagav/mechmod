/*
* Saber FX
* @version 12/21/03
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

HSPRITE	g_hsprSaber;
HSPRITE	g_hsprSaber_end;
HSPRITE	g_hsprSaber_trail;

DECLARE_MESSAGE(m_Saberfx, Saberfx);
using namespace std;
int CHudSaberFX::Init(void)
{	m_SaberFX.clear();
	m_iFlags |= HUD_ACTIVE;

	HOOK_MESSAGE(Saberfx);
	gHUD.AddHudElem(this);
	return 1;
};

int CHudSaberFX::VidInit(void)
{	g_hsprSaber = SPR_Load( "sprites/saber.spr" );
	g_hsprSaber_end = SPR_Load( "sprites/saber_end.spr" );
	g_hsprSaber_trail = SPR_Load( "sprites/saber_trail.spr" );
	return 1;
};


void CHudSaberFX::Reset( void )
{	list<saberfx_s>::iterator current;
	ForEachIn( current, m_SaberFX )
	{	current->flSparkEmitTime = 0;
	}
}

void CHudSaberFX::InitHUDData( void )
{	m_SaberFX.clear();
}

int CHudSaberFX::Draw(float flTime)
{	return 1;
}

void CHudSaberFX::AddSaberfx( int idx )
{	// Already have a member
	list<saberfx_s>::iterator Find = FindSaberfx(idx);
	if(Find != NULL)
	{	return;
	}

	saberfx_s NewSaber;

	NewSaber.iIndex = idx;
	NewSaber.flSaberLength = 0;
	NewSaber.flFraction = 0;
	NewSaber.flSparkEmitTime = 0;
	NewSaber.blUseSaberPos = false;
	// Reset saber trail lengths
	for(int i=0;i<MAX_SABER_LINKS;i++)
		NewSaber.vecSaberTrailEnd[i] = vec3_t(0,0,0);
	// Add bone structure and disable rendering of saber entity
	NewSaber.iterRecPos = gHUD.m_BonePos.AddBoneRecordWeapon( idx );

	// Add to list
	m_SaberFX.push_front(NewSaber);
}

list<saberfx_s>::iterator CHudSaberFX::RemoveSaberfx( int idx )
{	return RemoveSaberfx(FindSaberfx(idx));
}

list<saberfx_s>::iterator CHudSaberFX::RemoveSaberfx( list<saberfx_s>::iterator iterSaber )
{	if(iterSaber == NULL)
	{	return NULL;
	}

	// Remove bone structure
	gHUD.m_BonePos.RemoveBoneRecord( iterSaber->iterRecPos );

	// Erase member
	return m_SaberFX.erase(iterSaber);

}

list<saberfx_s>::iterator CHudSaberFX::FindSaberfx( int idx )
{	list<saberfx_s>::iterator current;

	ForEachIn( current, m_SaberFX )
	{	if(current->iIndex == idx)
		{	break;
		}
	}
	if(current == m_SaberFX.end())
	{	return NULL;
	}

	return current;
}

int CHudSaberFX::MsgFunc_Saberfx(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	int idx = READ_SHORT();
	int on = READ_BYTE();

	if(on)
	{	AddSaberfx(idx);
	}
	else
	{	RemoveSaberfx(idx);
	}

	return 1;
}
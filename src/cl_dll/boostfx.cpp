/*
* Boost FX
* @version 12/21/03
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

DECLARE_MESSAGE(m_Boostfx, Boostfx);
using namespace std;
int CHudBoostFX::Init(void)
{	m_BoostFX.clear();
	m_iFlags |= HUD_ACTIVE;

	HOOK_MESSAGE(Boostfx);
	gHUD.AddHudElem(this);
	return 1;
};

int CHudBoostFX::VidInit(void)
{	m_hsprBoostBlue_start = SPR_Load( "sprites/boost_start_blue.spr" );
	m_hsprBoostBlue_end = SPR_Load( "sprites/boost_end_blue.spr" );
	m_hsprBoostRed_start = SPR_Load( "sprites/boost_start_red.spr" );
	m_hsprBoostRed_end = SPR_Load( "sprites/boost_end_red.spr" );
	m_hsprBoostPurple_start = SPR_Load( "sprites/boost_start_purple.spr" );
	m_hsprBoostPurple_end = SPR_Load( "sprites/boost_end_purple.spr" );
	return 1;
};


void CHudBoostFX::Reset( void )
{	list<boostfx_s>::iterator iterCurrent;
	// Reset animation times
	ForEachIn(iterCurrent, m_BoostFX)
	{	iterCurrent->flAnimateStart = 0;
		iterCurrent->flAnimateEnd = 0;
	}
}

void CHudBoostFX::InitHUDData( void )
{	// Remove all boost fx
	list<boostfx_s>::iterator iterBegin = m_BoostFX.begin();
	while(iterBegin != m_BoostFX.end())
	{	iterBegin = RemoveBoostfx(iterBegin);
	}
}

int CHudBoostFX::Draw(float flTime)
{	return 1;
}

void CHudBoostFX::AddBoostfx( int idx, int fx, bool dbljump )
{	// Already have a member, just update FX	
	list<boostfx_s>::iterator iterFind = FindBoostfx(idx);
	if(iterFind != NULL)
	{	iterFind->iFX = fx;
		if(iterFind->blDoubleJump != dbljump && !dbljump)
		{	iterFind->blDoubleJump = dbljump;
			iterFind->flDieTime = 0;
		}
		return;
	}

	boostfx_s NewBoost;

	NewBoost.iIndex = idx;
	NewBoost.iFX = fx;
	NewBoost.iFrameStart = 0;
	NewBoost.iFrameEnd = 0;
	NewBoost.flAnimateStart = 0;
	NewBoost.flAnimateEnd = 0;
	NewBoost.blDoubleJump = dbljump;
	// Add bone structure and disable rendering of boost entity
	NewBoost.iterRecPos = gHUD.m_BonePos.AddBoneRecord( idx );
	NewBoost.iterRecPos->blRender = false;

	// Double jump
	if(dbljump)
	{	NewBoost.flDieTime = gEngfuncs.GetClientTime() + BOOST_DOUBLEJUMPTIME;
	}
	else
	{	NewBoost.flDieTime = 0;
	}
	switch(fx)
	{
	default:
	case BOOST_BLUE:
		{	NewBoost.hsprStartSprite = m_hsprBoostBlue_start;
			NewBoost.hsprEndSprite = m_hsprBoostBlue_end;
			break;
		}
	case BOOST_RED:
		{	NewBoost.hsprStartSprite = m_hsprBoostRed_start;
			NewBoost.hsprEndSprite = m_hsprBoostRed_end;
			break;
		}
	case BOOST_PURPLE:
		{	NewBoost.hsprStartSprite = m_hsprBoostPurple_start;
			NewBoost.hsprEndSprite = m_hsprBoostPurple_end;
			break;
		}
	}

	// Add to list
	m_BoostFX.push_front(NewBoost);

}

list<boostfx_s>::iterator CHudBoostFX::RemoveBoostfx( int idx )
{	return RemoveBoostfx(FindBoostfx(idx));
}

list<boostfx_s>::iterator CHudBoostFX::RemoveBoostfx( list<boostfx_s>::iterator iterBoost )
{	if(iterBoost == NULL)
	{	return NULL;
	}

	// Remove bone structure
	gHUD.m_BonePos.RemoveBoneRecord( iterBoost->iterRecPos );

	// Remove from list
	return m_BoostFX.erase(iterBoost);

}

list<boostfx_s>::iterator CHudBoostFX::FindBoostfx( int idx )
{	list<boostfx_s>::iterator iterCurrent;

	ForEachIn(iterCurrent, m_BoostFX)
	{	if(iterCurrent->iIndex == idx)
		{	break;
		}
	}

	if(iterCurrent == m_BoostFX.end())
	{	return NULL;
	}
	return iterCurrent;
}

int CHudBoostFX::MsgFunc_Boostfx(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	int idx = READ_SHORT();
	int fx = READ_BYTE();
	int dbljump = READ_BYTE();

	if(fx)
	{	AddBoostfx(idx, fx, dbljump ? true : false);
	}
	else
	{	RemoveBoostfx(idx);
	}

	return 1;
}
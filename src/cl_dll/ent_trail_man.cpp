/**********************************************************
*			ENTITY TRAIL MANAGER CODE
***********************************************************
*	Purpose: Enhanced entity trails manager
*	Created On: 3/2/04
*	Last Edited: 3/2/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "com_model.h"
#include "texture.h"
#include "ent_trail.h"
#include "ent_trail_man.h"
#include "ent_trail_def.h"

CTrailMan g_TrailMan;

CTrailMan::CTrailMan( void )
{
	// init our vars
	Init( );
}

CTrailMan::~CTrailMan( void )
{
	RemoveAll( );
}

void CTrailMan::Init( void )
{
	m_pTrail = NULL;
}

void CTrailMan::RemoveAll( void )
{
	CEntTrail *pTmp = m_pTrail;

	while( pTmp )
	{
		pTmp = RemoveTrail( pTmp );
	}

	m_pTrail = NULL;
}

CEntTrail *CTrailMan::RemoveTrail( CEntTrail *pTrail )
{
	if( pTrail )
	{
		CEntTrail *pTmp = pTrail->m_pNext;

		if( pTrail == m_pTrail )
		{
			m_pTrail = pTrail->m_pNext;
		}

		if( pTrail->m_pNext )
		{
			pTrail->m_pNext->m_pPrev = pTrail->m_pPrev;
		}
		if( pTrail->m_pPrev )
		{
			pTrail->m_pPrev->m_pNext = pTrail->m_pNext;
		}

		pTrail->RemoveTrail( );

		delete pTrail;

		return pTmp;
	}

	return NULL;
}


void CTrailMan::RenderTrails( void )
{
	CEntTrail *pTrail = m_pTrail;

	while( pTrail )
	{
		pTrail->RenderTrail( );

		if( !pTrail->IsAlive( ) )
		{
			pTrail = RemoveTrail( pTrail );
		}
		else
		{
			pTrail = pTrail->m_pNext;
		}
	}
}

void CTrailMan::AddTrail( int iIndex, int iRender, int iType, float flStSize, float flEdSize,
						  float flLinks, float flLinkLen, float flClr[], char *pSprName, char *pBoneName )
{
	CEntTrail *pTrail = new CEntTrail;

	pTrail->m_pNext = m_pTrail;
	pTrail->m_pPrev = NULL;

	if( m_pTrail )
	{
		m_pTrail->m_pPrev = pTrail;
	}

	// fill in everything else here
	pTrail->SetEnt( iIndex );
	pTrail->SetType( iType );
	pTrail->SetSizes( flStSize, flEdSize );
	if( iRender )
		pTrail->SetRendermode( iRender );
	if( flLinks )
		pTrail->SetLinks( flLinks );
	if( flLinkLen )
		pTrail->SetLinkLen( flLinkLen );
	if( flClr )
		pTrail->SetColor( flClr );
	if( pSprName )
		pTrail->SetSprName( pSprName );
	if( iType == TRAIL_BONE )
	{
		pTrail->SetBones( iIndex );
		pTrail->SetBoneName( pBoneName );
	}

	m_pTrail = pTrail;
}

CEntTrail *CTrailMan::FindTrail( int iIndex, int iType, char *pBone )
{
	CEntTrail *pTrail = m_pTrail;

	while( pTrail )
	{
		if( pTrail->GetIndex( ) == iIndex )
		{
			if( iType == pTrail->GetType( ) )
			{
				if( iType == TRAIL_BONE )
				{
					if( !strcmp( pBone, pTrail->GetBoneName( ) ) )
					{
						return pTrail;
					}
				}
				else
				{
					return pTrail;
				}
			}
		}
		pTrail = pTrail->m_pNext;
	}

	return NULL;
}
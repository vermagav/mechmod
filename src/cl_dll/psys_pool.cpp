/**********************************************************
*			PARTICLE SYSTEM POOL
***********************************************************
*	Purpose: Stores a "pool" of free partcles for less 
*			memoy management
*	Created On: 1/4/04
*	Last Edited: 1/4/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "com_model.h"
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"

CPartPool g_PartPool;

CPartPool::CPartPool( void )
{
	// constructor
	// on spawn make a pool
	m_iPooledParts = 0;
	m_pFreeParts = NULL;
	//InitPool( );
}

CPartPool::~CPartPool( void )
{
	// destructor
	// remove all free parts
	DrainPool( );
}

void CPartPool::InitPool( void )
{
	m_pFreeParts = NULL;
	//create our huge list of parts since we need it
	for( int iCount = 0; iCount < INIT_PARTS; iCount ++ )
	{
		CreatePart( );
	}
}

void CPartPool::CreatePart( void )
{
	//create a single particle
	Particle_s *pPart = new Particle_s;
	
	pPart->pPrev = NULL;
	pPart->pNext = m_pFreeParts;

	if( m_pFreeParts )
	{
		m_pFreeParts->pPrev = pPart;
	}

	m_pFreeParts = pPart;

	m_iPooledParts ++;
}

void CPartPool::DrainPool( void )
{
	// remove all parts from the pool
	Particle_s *pPart = m_pFreeParts;

	while( pPart )
	{
		Particle_s *pTmp = NULL;
		if( pPart->pNext )
		{
			pTmp = pPart->pNext;
			pTmp->pPrev = NULL;
			m_pFreeParts = pTmp;
		}
		delete pPart;
		pPart = pTmp;
		//should be 0 at the end, only worry is in use particles
		m_iPooledParts --;
	}

	m_pFreeParts = NULL;
}

void CPartPool::DoublePooled( void )
{
	// enlarge pool by 2x, ::Girls, Now thats big!::
	for( int iCount = m_iPooledParts; iCount > 0; iCount -- )
	{
		//double it
		CreatePart( );
	}
}

Particle_s *CPartPool::DrawFromPool( void )
{
	// they want a single particle now lets give it to em
	
	if( m_iPooledParts == 0 )
	{
		InitPool( );
	}

	Particle_s *pPart = m_pFreeParts, *pTmp = NULL;

	if( !pPart )
	{
		DoublePooled( );
	}

	pPart = m_pFreeParts;

	if( pPart )
	{
		if( pPart->pNext )
		{
			pTmp = pPart->pNext;
			pTmp->pPrev = NULL;
		}
	}
	m_pFreeParts = pTmp;

	return pPart;
}

void CPartPool::ReturnToPool( Particle_s *pPart )
{
	// return this particle to the pool of which it came
	pPart->pPrev = NULL;
	pPart->pNext = m_pFreeParts;
	if( m_pFreeParts )
	{
		m_pFreeParts->pPrev = pPart;
	}
	m_pFreeParts = pPart;
	// we arent going to worry about reseting everythign else that will be the emitters job
}
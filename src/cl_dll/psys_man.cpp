/**********************************************************
*			PARTICLE SYSTEM MANAGER
***********************************************************
*	Purpose: Particle system manager keeps track of all the 
*			running systems
*	Created On: 1/5/04
*	Last Edited: 1/5/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/
#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "com_model.h"
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "psys_man.h"


CPartSysManager *GetManager( void );

CPartSysManager *GetManager( void )
{
	//get the manager 
	return CPartSysManager::GetManager( );
}


CPartSysManager::CPartSysManager( void )
{
	// init our vars
	m_pHeadSystem = NULL;
}

CPartSysManager::~CPartSysManager( void )
{
	// delete everything
	RemoveAll( );
}

void CPartSysManager::RemoveAll( void )
{
	// remove all systems
	CParticleSystem *pSys = m_pHeadSystem;
	while( pSys )
	{
		pSys = Remove( pSys );
	}

	m_pHeadSystem = NULL;
}

CParticleSystem *CPartSysManager::Remove( CParticleSystem *pSys )
{
	// remove a single link from our linked list
	CParticleSystem *pTmp = pSys->m_pNext;
	if( pSys->m_pNext )
	{
		pSys->m_pNext->m_pPrev = pSys->m_pPrev;
	}
	if( pSys->m_pPrev )
	{
		pSys->m_pPrev->m_pNext = pSys->m_pNext;
	}

	if( pSys == m_pHeadSystem )
	{
		m_pHeadSystem = pTmp;
	}

	// remove attached particles
	pSys->ReturnAllParts( );

	// looks like were good here
	delete pSys;

	return pTmp;
}

void CPartSysManager::AddSystem( CParticleSystem *pSys )
{
	// add a system to our super cool linked list
	pSys->m_pNext = m_pHeadSystem;
	pSys->m_pPrev = NULL;

	if( m_pHeadSystem )
	{
		// set its prev
		m_pHeadSystem->m_pPrev = pSys;
	}

	//pSys->m_pTex = NULL;

	// and were linked
	m_pHeadSystem = pSys;
}

void CPartSysManager::UpdateSystems( void )
{
	// render our system
	CParticleSystem *pSys = m_pHeadSystem, *pTmp = NULL;
	while( pSys )
	{
		pTmp = pSys->m_pNext;
		// draw it
		if( !pSys->RenderSystem( ) )
		{
			//remove it
			Remove( pSys );
		}
		//TODO: ADD REMOVAL OF OLD SYSTEMS
		pSys = pTmp;
	}
}

CPartSysManager *CPartSysManager::GetManager( void )
{
	// get the manager
	static CPartSysManager *pMan = NULL;

	// create one if we dont have one
	if( !pMan )
	{
		pMan = new CPartSysManager;
	}

	return pMan;
}

CParticleSystem *CPartSysManager::Find( int iSystem, int iIndex )
{
	// find a system and return it
	CParticleSystem *pSys = m_pHeadSystem;

	while( pSys )
	{
		if( iSystem == pSys->m_iSystem )
		{
			//find index;
			if( iIndex == pSys->m_iIndex )
			{
				return pSys;
			}
		}
		pSys = pSys->m_pNext;
	}

	//no system found
	return NULL;
}

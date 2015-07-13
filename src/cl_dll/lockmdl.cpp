/*
* Lock Model
* @version 10/11/04
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "triangleapi.h"
#include "pm_defs.h"
#include "event_api.h"

DECLARE_MESSAGE(m_LockMdl, LockMdl);

/*
* Init Hud Data
*/
int CHudLockMdl::Init( void )
{	Reset();
	HOOK_MESSAGE(LockMdl);
	gHUD.AddHudElem(this);
	return 1;
}

/*
* Reset
*/
void CHudLockMdl::Reset( void )
{	m_LockEnts.clear();
}

/*
* Model Lock message
*/
int CHudLockMdl::MsgFunc_LockMdl(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	int idx = READ_SHORT();
	int iStatus = READ_BYTE();
	// Add index
	if(iStatus)
	{	
		// Get entity data
		cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex(idx);
		if(!pEnt)
		{	return 1;
		}

		// Fillout lockmdl data structure
		lockmdl_t lockmdl;
		lockmdl.iIndex = idx;
		lockmdl.vecAng = pEnt->angles;

		// Add to list
		m_LockEnts.push_front(lockmdl);
	}
	// remove
	else
	{	std::list<lockmdl_t>::iterator iterCurrent;
		ForEachIn(iterCurrent,m_LockEnts)
		{	if(iterCurrent->iIndex == idx)
			{	m_LockEnts.erase(iterCurrent);
				break;
			}
		}
	}
	return 1;
}
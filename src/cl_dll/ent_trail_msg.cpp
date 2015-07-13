/**********************************************************
*			ENTITY TRAIL MESSANGER CODE
***********************************************************
*	Purpose: Enhanced entity trails message system
*	Created On: 3/2/04
*	Last Edited: 3/2/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "com_model.h"
#include "texture.h"
#include "ent_trail.h"
#include "ent_trail_man.h"
#include "ent_trail_def.h"
//#include "spr_texture.h"
#include "r_studioint.h"
#include "event_api.h"
#include "r_efx.h"

extern engine_studio_api_t IEngineStudio;

DECLARE_MESSAGE( m_EntTrail, EntTrail );

int CHudEntTrail::Init( void )
{
	HOOK_MESSAGE( EntTrail );
	m_iFlags = 0;

	return 1;
}

int CHudEntTrail::VidInit( void )
{
	return 1;
}

int CHudEntTrail::MsgFunc_EntTrail( const char *pszName, int iSize, void *pbuf )
{
	// read in our message here
	int iIndex, iRender, iStatus, iType;
	float flStSize, flEdSize, flLinks, flLinkLen, flColor[3];
	char *pSprName = NULL, *pBone = NULL;

	BEGIN_READ( pbuf, iSize );
		iIndex = READ_SHORT( );
		iStatus = READ_BYTE( );

		if( iStatus == 1 )
		{
				iRender = READ_SHORT( );
				flStSize = READ_COORD( );
				flEdSize = READ_COORD( );
				flLinks = READ_COORD( );
				flLinkLen = READ_COORD( );
				flColor[0] = float( READ_BYTE( ) )/ 255.0f;
				flColor[1] = float( READ_BYTE( ) )/ 255.0f;
				flColor[2] = float( READ_BYTE( ) )/ 255.0f;
				pSprName = READ_STRING( );
				iType = READ_SHORT( );
				if( iType == TRAIL_BONE )
				{
					pBone = READ_STRING( );
				}

			if( IEngineStudio.IsHardware( ) )
			{
				g_TrailMan.AddTrail( iIndex, iRender, iType, flStSize, flEdSize, flLinks,
									 flLinkLen, flColor, pSprName, pBone );
			}
			else
			{
				// create a regular trail
				int iMod;
				iMod = gEngfuncs.pEventAPI->EV_FindModelIndex( pSprName );
				gEngfuncs.pEfxAPI->R_BeamFollow( iIndex, iMod, flLinkLen*flLinks/500, abs( flEdSize + flStSize )/2, flColor[0], flColor[1], flColor[2], 1.0f );
			}
		}
		else
		{
			iType = READ_SHORT( );
			if( iType == TRAIL_BONE )
			{
				pBone = READ_STRING( );
			}

			CEntTrail *pTrail = g_TrailMan.FindTrail( iIndex, iType, pBone );
			if( pTrail )
			{ 
				pTrail->RemoveWhenDone( );
			}
		}

	return 1;
}
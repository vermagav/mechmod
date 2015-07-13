/**********************************************************
*			EFFECTS FOR ALL PARTICLE SYSTEMS
***********************************************************
*	Purpose: Common effects for particle systems
*	Created On: 1/6/04
*	Last Edited: 1/6/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "pmtrace.h"
#include "pm_defs.h"
#include "triangleapi.h"
#include "com_model.h"
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "eff_all.h"
#include "gl-triapi.h"

CPartEffects g_PartEffects;

void CPartEffects::RenderQuad( Particle_s *pPart )
{
	vec3_t vPoint;

	g_CGLTriAPI.Texture2D( pPart->pTex, pPart->iCurFrame );
	g_CGLTriAPI.Color4fv( pPart->clColor );
	

	g_CGLTriAPI.Begin( TRI_QUADS );

		g_CGLTriAPI.TexCoord2f( 1, 1 );
		vPoint = pPart->vOrg + ( pPart->vRight * pPart->flSize) - ( pPart->vUp * pPart->flSize);
		g_CGLTriAPI.Vertex3fv( vPoint );

		g_CGLTriAPI.TexCoord2f( 0, 1 );
		vPoint = pPart->vOrg - ( pPart->vRight * pPart->flSize) - ( pPart->vUp * pPart->flSize);
		g_CGLTriAPI.Vertex3fv( vPoint );

		g_CGLTriAPI.TexCoord2f( 0, 0 );
		vPoint = pPart->vOrg - ( pPart->vRight * pPart->flSize) + ( pPart->vUp * pPart->flSize);
		g_CGLTriAPI.Vertex3fv( vPoint );

		g_CGLTriAPI.TexCoord2f( 1, 0 );
		vPoint = pPart->vOrg + ( pPart->vRight * pPart->flSize) + ( pPart->vUp * pPart->flSize);
		g_CGLTriAPI.Vertex3fv( vPoint );

	g_CGLTriAPI.End( );
	

	/*if( pPart->pTex->iType == TEX_SPRITE )
	{

		gEngfuncs.pTriAPI->SpriteTexture( pPart->pTex->pModel, pPart->iCurFrame );

		gEngfuncs.pTriAPI->Begin( TRI_QUADS );
		gEngfuncs.pTriAPI->Color4f( pPart->clColor.r, pPart->clColor.g, pPart->clColor.b, pPart->clColor.a );
		
		gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
		vPoint = pPart->vOrg + ( pPart->vRight * pPart->flSize) - ( pPart->vUp * pPart->flSize);
		gEngfuncs.pTriAPI->Vertex3fv( vPoint );

		gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
		vPoint = pPart->vOrg - ( pPart->vRight * pPart->flSize) - ( pPart->vUp * pPart->flSize);
		gEngfuncs.pTriAPI->Vertex3fv( vPoint );

		gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
		vPoint = pPart->vOrg - ( pPart->vRight * pPart->flSize) + ( pPart->vUp * pPart->flSize);
		gEngfuncs.pTriAPI->Vertex3fv( vPoint );

		gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
		vPoint = pPart->vOrg + ( pPart->vRight * pPart->flSize) + ( pPart->vUp * pPart->flSize);
		gEngfuncs.pTriAPI->Vertex3fv( vPoint );

		gEngfuncs.pTriAPI->End( );
	}
	else
	{
		if( gTexture.IsOpenGL( ) )
		{
			glDisable(GL_ALPHA_TEST);
			
			glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
			
			glBindTexture( GL_TEXTURE_2D, pPart->pTex->iTexNum + pPart->iCurFrame );

			glColor4f( pPart->clColor.r, pPart->clColor.g, pPart->clColor.b, pPart->clColor.a );
			glBegin( GL_QUADS );

				glTexCoord2f( 1, 1 );
				vPoint = pPart->vOrg + ( pPart->vRight * pPart->flSize) - ( pPart->vUp * pPart->flSize);
				glVertex3fv( vPoint );

				glTexCoord2f( 0, 1 );
				vPoint = pPart->vOrg - ( pPart->vRight * pPart->flSize) - ( pPart->vUp * pPart->flSize);
				glVertex3fv( vPoint );

				glTexCoord2f( 0, 0 );
				vPoint = pPart->vOrg - ( pPart->vRight * pPart->flSize) + ( pPart->vUp * pPart->flSize);
				glVertex3fv( vPoint );

				glTexCoord2f( 1, 0 );
				vPoint = pPart->vOrg + ( pPart->vRight * pPart->flSize) + ( pPart->vUp * pPart->flSize);
				glVertex3fv( vPoint );

			glEnd( );

			//glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
		
		}
	}*/
}

void CPartEffects::Anim( Particle_s *pPart )
{
	// animate particle
	// make sure our crucial things exist
	if( pPart )
	{
		// like textures
		if( pPart->pTex )
		{
			//check if the texture is there
		//	if( pPart->pTex->pModel )
			{
				//add a frame to cur frame
				if( gEngfuncs.GetClientTime( ) > ( pPart->flLastAnim + pPart->flFrameRate ) )
				{
					pPart->flLastAnim = gEngfuncs.GetClientTime( );
					pPart->iCurFrame ++;
					//check if were on the max frame
					if( pPart->iCurFrame >= pPart->pTex->iFrames )
					{
						// restart anim
						pPart->iCurFrame = 0;
					}
				}
			}
		}
	}
}

bool CPartEffects::Collision( Particle_s *pPart )
{
	// check collsion
	int iContents = gEngfuncs.PM_PointContents( pPart->vOrg, NULL );
	if( iContents == CONTENTS_WIND )
	{
		// if we are in a force based object get the force to add to the particle
		pmtrace_s *pmTrace = gEngfuncs.PM_TraceLine( pPart->vOrg, pPart->vOrg, PM_TRACELINE_PHYSENTSONLY, 2, -1 );

		// get entitys index
		if( pmTrace->ent != 0 )
		{
			cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex( pmTrace->ent );

			if( pEnt && pEnt->curstate.skin == CONTENTS_WIND )
			{
				// add wind to object
				vec3_t vVel = pEnt->curstate.angles + vec3_t( gEngfuncs.pfnRandomFloat( -pEnt->curstate.fuser1, pEnt->curstate.fuser1 ),
					gEngfuncs.pfnRandomFloat( -pEnt->curstate.fuser1, pEnt->curstate.fuser1 ),
					gEngfuncs.pfnRandomFloat( -pEnt->curstate.fuser1, pEnt->curstate.fuser1 ) );	// angle randomization
				vVel = vVel.Normalize( );
				vVel = vVel * pEnt->curstate.fuser2;		// speed scaler

				pPart->vVel = pPart->vVel + vVel;
			}
		}
		return true;
	}
	else if( iContents != CONTENTS_EMPTY )
	{
		return true;
	}

	return false;
}

void CPartEffects::CalcDeltas( Particle_s *pPart, float flDist )
{
	// set color
	float Tmp = 0;
	Tmp = ( ( pPart->clEdCol.r - pPart->clStCol.r ) / pPart->flLife ) * flDist;
	pPart->clColor.r += Tmp;
	Tmp = ( ( pPart->clEdCol.g - pPart->clStCol.g ) / pPart->flLife ) * flDist;
	pPart->clColor.g += Tmp;
	Tmp = ( ( pPart->clEdCol.b - pPart->clStCol.b ) / pPart->flLife ) * flDist;
	pPart->clColor.b += Tmp;
	Tmp = ( ( pPart->clEdCol.a - pPart->clStCol.a ) / pPart->flLife ) * flDist;
	pPart->clColor.a += Tmp;

	if( pPart->clColor.r > 1.0 )
	{
		pPart->clColor.r = 1.0;
	}
	else if( pPart->clColor.r < 0.0 )
	{
		pPart->clColor.r = 0;
	}
	if( pPart->clColor.g > 1.0 )
	{
		pPart->clColor.g = 1.0;
	}
	else if( pPart->clColor.g < 0.0 )
	{
		pPart->clColor.g = 0;
	}
	if( pPart->clColor.b > 1.0 )
	{
		pPart->clColor.b = 1.0;
	}
	else if( pPart->clColor.b < 0.0 )
	{
		pPart->clColor.b = 0;
	}
	if( pPart->clColor.a > 1.0 )
	{
		pPart->clColor.a = 1.0;
	}
	else if( pPart->clColor.a < 0.0 )
	{
		pPart->clColor.a = 0;
	}
	// set size
	Tmp = ( ( pPart->flEdSize - pPart->flStSize ) / pPart->flLife ) * flDist;
	pPart->flSize += Tmp;
	if( pPart->flStSize > pPart->flEdSize )
	{
		if( pPart->flSize > pPart->flStSize )
		{
			pPart->flSize = pPart->flStSize;
		}
		else if( pPart->flSize < pPart->flEdSize )
		{
			pPart->flSize = pPart->flEdSize;
		}
	}
	else if( pPart->flEdSize > pPart->flStSize )
	{
		if( pPart->flSize < pPart->flStSize )
		{
			pPart->flSize = pPart->flStSize;
		}
		else if( pPart->flSize > pPart->flEdSize )
		{
			pPart->flSize = pPart->flEdSize;
		}
	}
}

void CPartEffects::Bounce( Particle_s *pPart )
{
	//now for the pretty bouncing particles

	pmtrace_t *pTrace = gEngfuncs.PM_TraceLine( pPart->vPOrg, pPart->vOrg, PM_TRACELINE_ANYVISIBLE, 2, -1 );

	if( pTrace->fraction != 1.0 )
	{
		// make the particle bounce
		float flBackoff = DotProduct ( pPart->vVel, pTrace->plane.normal ) * ( pPart->flBounceScl + 1);

		for (int i=0; i<3; i++)
		{
			float flChange = pTrace->plane.normal[i] * flBackoff;

			pPart->vVel[i] = pPart->vVel[i] - flChange;

		}
		pPart->iBounces --;

	}
}

void CPartEffects::AnimOnCol( Particle_s *pPart )
{
	// check for collision
	if( Collision( pPart ) )
	{
		//set gravity to 0, and velocity to 0
		pPart->flGrav = 0;
		pPart->vVel = vec3_t( 0,0,0 );

		if( pPart->iCurFrame == 0 )		//first run
		{
			// set soemthign saying it has already been 0
			pPart->iBounces = 1;
			pmtrace_t *pTrace = gEngfuncs.PM_TraceLine( pPart->vPOrg, pPart->vOrg, PM_TRACELINE_ANYVISIBLE, 2, -1 );
			if( pTrace->fraction != 1.0 )
			{
				pPart->vOrg = pTrace->endpos;
			}

		}
		Anim( pPart );
		if( pPart->iCurFrame == 0 )
		{
			// set a flag to kill it
			pPart->iBounces = -1;
		}
	}
}

vec3_t CPartEffects::RandomRoundVec( vec3_t vMin, vec3_t vMax )
{
	float flMin, flMax;

	flMin = vMin.Length( );
	flMax = vMax.Length( );

	vec3_t vVec( gEngfuncs.pfnRandomFloat( vMin.x, vMax.x ), gEngfuncs.pfnRandomFloat( vMin.y, vMax.y ),
		gEngfuncs.pfnRandomFloat( vMin.z, vMax.z ) );
	vVec = vVec.Normalize( ) * gEngfuncs.pfnRandomFloat( flMin, flMax );

	return vVec;
}

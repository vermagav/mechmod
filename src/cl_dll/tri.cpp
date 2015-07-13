//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "r_efx.h"
#include "pm_defs.h"
#include "event_api.h"
#include "pmtrace.h"
#include "StudioModelRenderer.h"
#include "GameStudioModelRenderer.h"

//added by jason 1-5-04
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "psys_man.h"
// EFFECT FILES 
#include "eff_test.h"
// added 1-15-04
#include "tga_replace.h"
#include "r_studioint.h"
extern engine_studio_api_t IEngineStudio;
// added 1-18-03
extern cvar_t *cl_gltextures;
#include "ent_trail.h"
#include "ent_trail_man.h"
// added 4-15-04
#include "ds_scythe.h"
// added 4-17-04
#include "cg_glow.h"
#include "gl_bored.h"
#include "gl_pbuffer.h"
// added 8-31-04
#include "alpha_bomb.h"
#include "tri-vgui.h"
#include "vgui_TeamFortressViewport.h"
//end jason

// Added by Pcjoe
#include	"gl-triapi.h"

void BoosterFX( void );
void SaberFX( void );

using namespace std;

#define DLLEXPORT __declspec( dllexport )

extern "C"
{
	void DLLEXPORT HUD_DrawNormalTriangles( void );
	void DLLEXPORT HUD_DrawTransparentTriangles( void );
	// Added by Pcjoe
	int DLLEXPORT CL_IsThirdPerson( void );
};

// Added by Pcjoe
// Defines
#define MAX_BOOSTER	8
#define BOOSTER_LENGTH 7
#define WIDTH_BOOSTER 2
#define MIN_POWER (float)0.75
#define SABER_ENLONGATE_RATE (float)1.00
#define SABER_WIDTH (float)1.2
#define SPARK_SPARK_AREA	40
#define SABER_SPARK_DELAY	0.01
// Global Angles
vec3_t v_forward, v_right, v_up;
// Sprites
extern HSPRITE	g_hsprSaber;
extern HSPRITE	g_hsprSaber_end;
extern HSPRITE	g_hsprSaber_trail;
extern HSPRITE	g_hsprOverhead_green;
extern HSPRITE	g_hsprOverhead_red;
extern HSPRITE	g_hsprBBox;
// Misc
extern vec3_t g_CameraAngles;
extern cvar_t *cl_sabercollision;
extern cvar_t *cl_sabertrails;
extern CGameStudioModelRenderer g_StudioRenderer;
extern rendercheck_t g_RenderCheck[33];
extern CGLTriAPI g_CGLTriAPI;

void DrawBBox(cl_entity_t *pEntity)
{
	// Invalid enitity, dont draw
	if(!pEntity)
		return;

	// Get angles
	vec3_t v_entForward, v_entRight, v_entUp;
	AngleVectors(pEntity->curstate.angles, v_entForward, v_entRight, v_entUp );

	// Get Width, Length, and Height
	int iWidth, iLength, iHeight;
	iWidth = abs(pEntity->curstate.mins.x-pEntity->curstate.maxs.x)/2;
	iLength = abs(pEntity->curstate.maxs.y-pEntity->curstate.mins.y)/2;
	iHeight = abs(pEntity->curstate.maxs.z-pEntity->curstate.mins.z)/2;

	// Fill in vectors
	vec3_t vecBackLowerLeft, vecBackLowerRight, vecBackUpperLeft, vecBackUpperRight;
	vec3_t vecFrontLowerLeft, vecFrontLowerRight, vecFrontUpperLeft, vecFrontUpperRight;

	// Without angles!
	/*
	// Back
	// -- Lower Left / Right
	vecBackLowerLeft = pEntity->curstate.origin + pEntity->curstate.mins;
	vecBackLowerLeft.x = pEntity->curstate.origin.x + pEntity->curstate.maxs.x;
	vecBackLowerRight = pEntity->curstate.origin + pEntity->curstate.mins;
	// -- Upper Left / Right
	vecBackUpperLeft = pEntity->curstate.origin + pEntity->curstate.maxs;
	vecBackUpperLeft.y = pEntity->curstate.origin.y + pEntity->curstate.mins.y;
	vecBackUpperRight = pEntity->curstate.origin + pEntity->curstate.mins;
	vecBackUpperRight.z = pEntity->curstate.origin.z + pEntity->curstate.maxs.z;

	// Front
	// -- Lower Left / Right
	vecFrontLowerLeft = pEntity->curstate.origin + pEntity->curstate.maxs;
	vecFrontLowerLeft.z = pEntity->curstate.origin.z + pEntity->curstate.mins.z;
	vecFrontLowerRight = pEntity->curstate.origin + pEntity->curstate.mins;
	vecFrontLowerRight.y = pEntity->curstate.origin.y + pEntity->curstate.maxs.y;
	// -- Upper Left / Right
	vecFrontUpperLeft = pEntity->curstate.origin + pEntity->curstate.maxs;
	vecFrontUpperRight = pEntity->curstate.origin + pEntity->curstate.maxs;
	vecFrontUpperRight.x = pEntity->curstate.origin.x + pEntity->curstate.mins.x;
	*/
	// With Angles
	// Back
	vecBackLowerLeft = pEntity->curstate.origin + v_entRight*-iWidth + v_entForward*-iLength + v_entUp*-iHeight;
	vecBackLowerRight = pEntity->curstate.origin + v_entRight*iWidth + v_entForward*-iLength + v_entUp*-iHeight;
	// -- Upper Left / Right
	vecBackUpperLeft = pEntity->curstate.origin + v_entRight*-iWidth + v_entForward*-iLength + v_entUp*iHeight;
	vecBackUpperRight = pEntity->curstate.origin + v_entRight*iWidth + v_entForward*-iLength + v_entUp*iHeight;

	// Front
	// -- Lower Left / Right
	vecFrontLowerLeft = pEntity->curstate.origin + v_entRight*-iWidth + v_entForward*iLength + v_entUp*-iHeight;
	vecFrontLowerRight = pEntity->curstate.origin + v_entRight*iWidth + v_entForward*iLength + v_entUp*-iHeight;
	// -- Upper Left / Right
	vecFrontUpperLeft = pEntity->curstate.origin + v_entRight*-iWidth + v_entForward*iLength + v_entUp*iHeight;
	vecFrontUpperRight = pEntity->curstate.origin + v_entRight*iWidth + v_entForward*iLength + v_entUp*iHeight;


	// Set sprite texture
	if ( !gEngfuncs.pTriAPI->SpriteTexture( (struct model_s *)gEngfuncs.GetSpritePointer( g_hsprBBox ), 0 ))
	{
		return;
	}

	// Draw sprite
	g_CGLTriAPI.RenderMode( kRenderTransTexture );
	g_CGLTriAPI.CullFace( TRI_NONE );
	g_CGLTriAPI.Begin( TRI_QUADS );
	g_CGLTriAPI.Color4f( 255, 255, 255, 255 );

	// --- Top

	// Lower Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackUpperLeft );

	// Upper Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontUpperLeft );

	// Upper Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontUpperRight );

	// Lower Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackUpperRight );

	// --- Bottom

	// Lower Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackLowerLeft );

	// Upper Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontLowerLeft );

	// Upper Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontLowerRight );

	// Lower Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackLowerRight );

	// --- Left

	// Lower Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecFrontLowerLeft );

	// Upper Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontUpperLeft );

	// Upper Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecBackUpperLeft );

	// Lower Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackLowerLeft );

	// --- Right

	// Lower Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackLowerRight );

	// Upper Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv( vecBackUpperRight );

	// Upper Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontUpperRight );

	// Lower Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecFrontLowerRight );

	// --- Front

	// Lower Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecFrontLowerLeft );

	// Upper Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontUpperLeft );

	// Upper Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecFrontUpperRight );

	// Lower Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecFrontLowerRight );

	// --- Back

	// Lower Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackLowerLeft );

	// Upper Left
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv( vecBackUpperLeft );

	// Upper Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecBackUpperRight );

	// Lower Right
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecBackLowerRight );

	// End Drawing
	g_CGLTriAPI.End();
	g_CGLTriAPI.RenderMode( kRenderNormal );

}

void DrawAllBBoxes( void )
{
	for(int i = 0; i < 32; i++)
	{
		cl_entity_t *pEntity = gEngfuncs.GetEntityByIndex(i);
		if(pEntity && pEntity->player)
			DrawBBox(pEntity);
	}
}

void Draw_GlobalSprite( HSPRITE sprite, vec3_t vecOrigin, int iRenderType, float flSize, int r, int g, int b, int a, int frame )
{
	vec3_t vPoint = vecOrigin;
	float flSizeHalf = flSize/2;

	// Set sprite texture
	if ( !gEngfuncs.pTriAPI->SpriteTexture( (struct model_s *)gEngfuncs.GetSpritePointer( sprite ), frame ))
	{
		return;
	}

	// Draw sprite
	g_CGLTriAPI.RenderMode( iRenderType );
	//g_CGLTriAPI.CullFace( TRI_FRONT );
	g_CGLTriAPI.CullFace( TRI_NONE );
	g_CGLTriAPI.Begin( TRI_QUADS );
	g_CGLTriAPI.Color4f( r, g, b, a );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	vPoint = vecOrigin + ( v_right * flSizeHalf) - ( v_up * flSizeHalf);
	g_CGLTriAPI.Vertex3fv( vPoint );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	vPoint = vecOrigin - ( v_right * flSizeHalf) - ( v_up * flSizeHalf);
	g_CGLTriAPI.Vertex3fv( vPoint );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	vPoint = vecOrigin - ( v_right * flSizeHalf) + ( v_up * flSizeHalf);
	g_CGLTriAPI.Vertex3fv( vPoint );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	vPoint = vecOrigin + ( v_right * flSizeHalf) + ( v_up * flSizeHalf);
	g_CGLTriAPI.Vertex3fv( vPoint );


	// End Drawing
	g_CGLTriAPI.End();
	g_CGLTriAPI.RenderMode( kRenderNormal );
}

int AnimateSprite( HSPRITE hsprSprite, int &iCurFrame, float &flAnimate, float flRate )
{	int iMaxFrames = SPR_Frames(hsprSprite);
	if(flAnimate < gHUD.m_flTime)
	{	flAnimate = gHUD.m_flTime + flRate;
		iCurFrame++;
	}
	if(iCurFrame >= iMaxFrames)
	{	iCurFrame = 0;
	}
	return iCurFrame;
}

void SetupSaberRender( HSPRITE hsprSprite )
{	if ( !gEngfuncs.pTriAPI->SpriteTexture( (struct model_s *)gEngfuncs.GetSpritePointer( hsprSprite ), 0 ))
	{	return;
	}//Color4f
//	gEngfuncs.pTriAPI->RenderMode( kRenderTransAdd );
//	g_CGLTriAPI.DisableDepthMask( );

	g_CGLTriAPI.RenderMode( kRenderTransAdd );
//	gEngfuncs.pTriAPI->CullFace( TRI_NONE );
	g_CGLTriAPI.CullFace( TRI_NONE );
//	gEngfuncs.pTriAPI->Begin( TRI_QUADS );
	g_CGLTriAPI.Begin( TRI_QUADS );
//	gEngfuncs.pTriAPI->Color4f( 1.0, 1.0, 1.0, 1.0 );
	g_CGLTriAPI.Color4f( 1.0, 1.0, 1.0, 1.0 );

	
}

void EndSaberRender( void )
{//	gEngfuncs.pTriAPI->End();
//	gEngfuncs.pTriAPI->RenderMode( kRenderNormal );
	g_CGLTriAPI.End();
//	g_CGLTriAPI.EnableDepthMask( );
}

void RenderSaberPeice( vec3_t vecStart, vec3_t vecEnd )
{	// Right
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd + v_right * SABER_WIDTH );

	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv(vecStart + v_right * SABER_WIDTH );

	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecStart - v_right * SABER_WIDTH );

	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd - v_right * SABER_WIDTH );

	// Front
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd + v_up * SABER_WIDTH );

	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv(vecStart + v_up * SABER_WIDTH );

	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecStart - v_up * SABER_WIDTH );

	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd - v_up * SABER_WIDTH );
	
	// Right
/*	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd + v_right * SABER_WIDTH );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	gEngfuncs.pTriAPI->Vertex3fv(vecStart + v_right * SABER_WIDTH );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	gEngfuncs.pTriAPI->Vertex3fv( vecStart - v_right * SABER_WIDTH );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd - v_right * SABER_WIDTH );

	// Front
	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd + v_up * SABER_WIDTH );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	gEngfuncs.pTriAPI->Vertex3fv(vecStart + v_up * SABER_WIDTH );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	gEngfuncs.pTriAPI->Vertex3fv( vecStart - v_up * SABER_WIDTH );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd - v_up * SABER_WIDTH );*/
}

#define SABER_MAX_TRAIL_LENGTH	(float)12
#define SABER_MAX_MTRAIL_LENGTH	(float)15
#define SABER_INTERPOL_SPEED	(float)20
#define SABER_MIN_TRAIL			(float)0.3

Vector RenderSaberTrail( vec3_t vecLastStart, vec3_t vecStart, vec3_t vecEnd, vec3_t vecStartOld, vec3_t vecEndOld, vec3_t vecPlayerOrigin, int iStartLink, int iEndLink, int iLinks, list<saberfx_s>::iterator iterSaber )
{	if(iterSaber->blUseSaberPos)
	{	vec3_t vecStartDist = (vecPlayerOrigin - vecStart) - (iterSaber->vecLastOrigin - vecStartOld);
		vec3_t vecEndDist = (vecPlayerOrigin - vecEnd) - (iterSaber->vecLastOrigin - vecEndOld);

		// Get start of saber to end of saber length ( as the saber goes out, trail gets bigger
		float flSaberTrailStart = SABER_MIN_TRAIL + (float(iStartLink) / float(iLinks)) * (1-SABER_MIN_TRAIL);
		float flSaberTrailEnd = SABER_MIN_TRAIL + (float(iEndLink) / float(iLinks)) * (1-SABER_MIN_TRAIL);

		// Get Length
		float flLengthStart = vecStartDist.Length();
		float flLengthEnd = vecEndDist.Length();
		// How much it will move in 1 second
		flLengthStart *= 1 / gHUD.m_flTimeDelta;
		flLengthEnd *= 1 / gHUD.m_flTimeDelta;

		// Cap off
		if(flLengthStart > SABER_MAX_MTRAIL_LENGTH)
			flLengthStart = SABER_MAX_MTRAIL_LENGTH;
		if(flLengthEnd > SABER_MAX_MTRAIL_LENGTH)
			flLengthEnd = SABER_MAX_MTRAIL_LENGTH;

		flLengthStart /= SABER_MAX_MTRAIL_LENGTH;
		flLengthEnd /= SABER_MAX_MTRAIL_LENGTH;

		flLengthStart *= SABER_MAX_TRAIL_LENGTH;
		flLengthEnd *= SABER_MAX_TRAIL_LENGTH;

		// Normalize
		vecStartDist = vecStartDist.Normalize() * flLengthStart;
		vecEndDist = vecEndDist.Normalize() * flLengthEnd;

		// Interpol saber trail
		float flInterpol = gHUD.m_flTimeDelta * SABER_INTERPOL_SPEED;
		if(flInterpol > 1)
			flInterpol = 1;
		vec3_t vecStartInterpol = (vecStartDist - iterSaber->vecSaberTrailEnd[iStartLink]) * flInterpol;
		vec3_t vecEndInterpol = (vecEndDist - iterSaber->vecSaberTrailEnd[iEndLink]) * flInterpol;
		iterSaber->vecSaberTrailEnd[iStartLink] = iterSaber->vecSaberTrailEnd[iStartLink] + vecStartInterpol;
		iterSaber->vecSaberTrailEnd[iEndLink] = iterSaber->vecSaberTrailEnd[iEndLink] + vecEndInterpol;

		// Overwrite

		// removed by jason causeing weird saber off to inf problems -1.#IND0
		vecStartDist = iterSaber->vecSaberTrailEnd[iStartLink];
		vecEndDist = iterSaber->vecSaberTrailEnd[iEndLink];

	
		if( vecStartDist.x > 20 || vecStartDist.x < -20 )
		{
			vecStartDist = vec3_t( 0, 0, 0 );
		}


		// Trail
	//	g_CGLTriAPI.Brightness( 1 );
		gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
		gEngfuncs.pTriAPI->Vertex3fv( vecEnd );

	//	g_CGLTriAPI.Brightness( 1 );
		gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
		gEngfuncs.pTriAPI->Vertex3fv( vecStart );
	
	//	g_CGLTriAPI.Brightness( 1 );
		gEngfuncs.pTriAPI->TexCoord2f( 0.95f, 1 );	// HACK HACK: White dots seem to appear at the end of the saber trail.. just cut it off
		if(!iStartLink)
		{	gEngfuncs.pTriAPI->Vertex3fv( vecStart + vecStartDist * flSaberTrailStart );
		}
		else
		{	gEngfuncs.pTriAPI->Vertex3fv( vecLastStart );
		}

	//	g_CGLTriAPI.Brightness( 1 );
		gEngfuncs.pTriAPI->TexCoord2f( 0.95f, 0 );	// HACK HACK: White dots seem to appear at the end of the saber trail.. just cut it off
		gEngfuncs.pTriAPI->Vertex3fv( vecEnd + vecEndDist * flSaberTrailEnd );

		// Set next start trail point
		return (vecEnd + vecEndDist * flSaberTrailEnd);
	}

	return vec3_t(0,0,0);
}

void SpawnSaberSparks( vec3_t vecOrigin, list<saberfx_s>::iterator iterSaber )
{	if(iterSaber->flSparkEmitTime < gHUD.m_flTime)
	{	iterSaber->flSparkEmitTime = gHUD.m_flTime + SABER_SPARK_DELAY;
		gEngfuncs.pEfxAPI->R_SparkEffect( vecOrigin, 8, -SPARK_SPARK_AREA, SPARK_SPARK_AREA );
	}
}

void SaberCollision( vec3_t vecSaberOrigin[MAX_SABER_LINKS], float flLength[MAX_SABER_LINKS], int &iRenderLinks, float &flCurLength, list<saberfx_s>::iterator iterSaber )
{	// No saber collision
	if(cl_sabercollision->value == 0)
	{	return;
	}

	pmtrace_t * trace;
	// Trace the main beam segments (not the end peice)
	if(iRenderLinks>0)
	{	for(int i=0;i<iRenderLinks;i++)
		{	trace = gEngfuncs.PM_TraceLine( vecSaberOrigin[i], vecSaberOrigin[i+1], PM_TRACELINE_PHYSENTSONLY, 2, -1 );
			
			// We hit something, cut off saber here
			if(trace->fraction < 1)
			{	vec3_t vecDir = (vecSaberOrigin[i+1]-vecSaberOrigin[i]).Normalize();

				iRenderLinks = i;
				flCurLength = flLength[i] * trace->fraction;

				SpawnSaberSparks( vecSaberOrigin[i]+vecDir*flCurLength, iterSaber );
				return;
			}
		}
	}

	// Do a trace for the last peice
	vec3_t vecSaberEnd = (vecSaberOrigin[iRenderLinks+1] - vecSaberOrigin[iRenderLinks]).Normalize() * flCurLength;
	trace = gEngfuncs.PM_TraceLine( vecSaberOrigin[iRenderLinks], vecSaberOrigin[iRenderLinks]+vecSaberEnd, PM_TRACELINE_PHYSENTSONLY, 2, -1 );
			
	// We hit something, cut off saber here
	if(trace->fraction < 1)
	{	flCurLength *= trace->fraction;
		vecSaberEnd = (vecSaberOrigin[iRenderLinks+1] - vecSaberOrigin[iRenderLinks]).Normalize() * flCurLength;
		SpawnSaberSparks( vecSaberOrigin[iRenderLinks]+vecSaberEnd, iterSaber );
	}
}

void RenderSaber( vec3_t vecSaberOrigin[MAX_SABER_LINKS], vec3_t vecPlayerOrigin, float flLength[MAX_SABER_LINKS], int iLinks, list<saberfx_s>::iterator iterSaber )
{	// Get current length
	float flMaxLength = iterSaber->flSaberLength * iterSaber->flFraction;
	float flCurLength = flMaxLength;
	int iRenderLinks=0;
	iLinks-=2;

	for(int i=0;i<iLinks;i++)
	{	if(flCurLength>=flLength[i])
		{	flCurLength -= flLength[i];
			iRenderLinks++;
		}
		else
		{	break;
		}
	}
	// Set + Render Saber Collision
	SaberCollision(vecSaberOrigin, flLength, iRenderLinks, flCurLength, iterSaber);


	/*
	* Render Saber Beam
	*/

	// added by jason
	vec3_t vOrg = vec3_t( 0, 0, 0 );

	if( !IEngineStudio.IsHardware( ) )
	{
		// find software player offset
	//	vOrg = vecPlayerOrigin + vec3_t( -5, -23, -25 );
	}
	// end jason
	SetupSaberRender( g_hsprSaber );
	// Render multiple links
	if(iRenderLinks>0)
	{	for(i=0;i<iRenderLinks;i++)
		{	RenderSaberPeice(vecSaberOrigin[i] + vOrg, vecSaberOrigin[i+1] + vOrg);
		}
	}
	EndSaberRender();

		// moved by jason
		// Render last peice
	SetupSaberRender( g_hsprSaber_end );
	// Get origin for last peice + Render
	vec3_t vecSaberEnd = (vecSaberOrigin[iRenderLinks+1] - vecSaberOrigin[iRenderLinks]).Normalize() * flCurLength;
	RenderSaberPeice(vecSaberOrigin[iRenderLinks] + vOrg, vecSaberOrigin[iRenderLinks]+vecSaberEnd + vOrg);
	EndSaberRender();
	/*
	* Render Saber Trail
	*/

	if(cl_sabertrails->value != 0)
	{	
		vec3_t vecLastStart = vec3_t(0,0,0);

		if( iterSaber == NULL )
			return;

		SetupSaberRender( g_hsprSaber_trail );
		// Render multiple links
		if(iRenderLinks>0)
		{	for(i=0;i<iRenderLinks;i++)
			{	// Render saber trail
				vecLastStart = RenderSaberTrail(vecLastStart,vecSaberOrigin[i], vecSaberOrigin[i+1], iterSaber->vecLastSaberPos[i],
					iterSaber->vecLastSaberPos[i+1], vecPlayerOrigin, i, i+1, iRenderLinks+1, iterSaber);
			}
			
		}

		// Render last peice of trail
			RenderSaberTrail(vecPlayerOrigin, vecSaberOrigin[iRenderLinks], vecSaberOrigin[iRenderLinks]+vecSaberEnd, 
				iterSaber->vecLastSaberPos[iRenderLinks], iterSaber->vecLastSaberPos[iRenderLinks]+vecSaberEnd,
				vecPlayerOrigin, iRenderLinks, iRenderLinks+1, iRenderLinks+1, iterSaber);

		EndSaberRender();
	}

}

void SaberFX( void )
{	list<saberfx_s>::iterator iterCurrent;
	cl_entity_t *pEntity = NULL;

	ForEachIn(iterCurrent,gHUD.m_Saberfx.m_SaberFX)
	{	pEntity = gEngfuncs.GetEntityByIndex( iterCurrent->iIndex );

		// Render Saberfx
		if(iterCurrent->iterRecPos != NULL && iterCurrent->iterRecPos->blCanUse && iterCurrent->iterRecPos->blRenderedFrame && pEntity)
		{	int iLinks=0;
			float flTotalLength=0;
			float flLength[MAX_SABER_LINKS];
			bool blValid=false;
			char szPrefix[16];
			char szName[16];
			vec3_t vecSaberOrigin[MAX_SABER_LINKS];

			// Copy prefix name
			strcpy(szPrefix,"saber_reg_");
			// Get saber origins
			for(int i=0; i<MAX_SABER_LINKS;i++)
			{	sprintf(szName,"%s%i",szPrefix,i+1);
				vecSaberOrigin[i] = gHUD.m_BonePos.GetBoneOrigin( iterCurrent->iterRecPos, szName, blValid );

				// Check if valid
				if(!blValid)
				{	break;
				}

				// Add to total count
				iLinks++;
			}

			// You need atleast 2 links to make a beam
			if(iLinks < 2)
			{	continue;
			}

		
			// Get Length
			for(i=0;i<iLinks-1;i++)
			{	flLength[i] = Length(vecSaberOrigin[i] - vecSaberOrigin[i+1]);
				flTotalLength += flLength[i];
			}

			// Set and add to saber length
			iterCurrent->flSaberLength = flTotalLength;
			iterCurrent->flFraction += gHUD.m_flTimeDelta * SABER_ENLONGATE_RATE;	// Enlongate in 2 seconds
			if(iterCurrent->flFraction>1)
				iterCurrent->flFraction = 1;

			// Render saber
			RenderSaber(vecSaberOrigin, pEntity->origin, flLength, iLinks, iterCurrent);

			// Save last saber pos
			for(i=0;i<iLinks;i++)
			{	iterCurrent->vecLastSaberPos[i] = vecSaberOrigin[i];
				iterCurrent->vecLastOrigin = pEntity->origin;
				iterCurrent->blUseSaberPos = true;
			}
		}
	}
}

#define BOOST_ANIM_RATE 0.05

void RenderBoost( vec3_t vecOrigin, vec3_t vecBoostVelocity, vec3_t vecRealVelocity, float flPower, list<boostfx_s>::iterator iterBoost )
{	vec3_t v_bForwardRight, v_bForwardLeft, vecStart, vecEnd;
	// cap off min power
	if(!iterBoost->blDoubleJump)
	{	if(flPower<MIN_POWER)
			flPower = MIN_POWER;
	}
	float flBoostWidth = WIDTH_BOOSTER * flPower;

	// Get start + end of boosters
	vecStart = vecOrigin;
	vecEnd = vecOrigin + vecBoostVelocity * BOOSTER_LENGTH * flPower;

	// Get diagnal directions
	v_bForwardRight = v_right + v_up;
	v_bForwardLeft = -v_right + v_up;
	VectorNormalize(v_bForwardRight);
	VectorNormalize(v_bForwardLeft);

	if ( !gEngfuncs.pTriAPI->SpriteTexture( (struct model_s *)gEngfuncs.GetSpritePointer( iterBoost->hsprEndSprite ), AnimateSprite( iterBoost->hsprEndSprite, iterBoost->iFrameEnd, iterBoost->flAnimateEnd, BOOST_ANIM_RATE ) ) )
	{	return;
	}
	
	// Right
	g_CGLTriAPI.RenderMode( kRenderTransAdd );
	g_CGLTriAPI.CullFace( TRI_NONE );
	g_CGLTriAPI.Begin( TRI_QUADS );
	g_CGLTriAPI.Color4f( 1.0, 1.0, 1.0, 1.0 );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd + v_right * flBoostWidth );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv(vecStart + v_right * flBoostWidth );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecStart - v_right * flBoostWidth );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd - v_right * flBoostWidth );

	// Front
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd + v_up * flBoostWidth );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv(vecStart + v_up * flBoostWidth );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecStart - v_up * flBoostWidth );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecEnd - v_up * flBoostWidth );
/*
	// Front-right
	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd + v_bForwardRight * flBoostWidth );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	gEngfuncs.pTriAPI->Vertex3fv(vecStart + v_bForwardRight * flBoostWidth );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	gEngfuncs.pTriAPI->Vertex3fv( vecStart - v_bForwardRight * flBoostWidth );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd - v_bForwardRight * flBoostWidth );

	// Front-left
	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd + v_bForwardLeft * flBoostWidth );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	gEngfuncs.pTriAPI->Vertex3fv(vecStart + v_bForwardLeft * flBoostWidth );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	gEngfuncs.pTriAPI->Vertex3fv( vecStart - v_bForwardLeft * flBoostWidth );

	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( vecEnd - v_bForwardLeft * flBoostWidth );*/

	g_CGLTriAPI.End();
	g_CGLTriAPI.RenderMode( kRenderNormal );

	if ( !gEngfuncs.pTriAPI->SpriteTexture( (struct model_s *)gEngfuncs.GetSpritePointer( iterBoost->hsprStartSprite ), AnimateSprite( iterBoost->hsprStartSprite, iterBoost->iFrameStart, iterBoost->flAnimateStart, BOOST_ANIM_RATE ) ))
	{	return;
	}

	// Start Sprite
	g_CGLTriAPI.RenderMode( kRenderTransAdd );
	g_CGLTriAPI.CullFace( TRI_NONE );
	g_CGLTriAPI.Begin( TRI_QUADS );
	g_CGLTriAPI.Color4f( 1.0, 1.0, 1.0, 1.0 );

	vecStart = vecStart + vecBoostVelocity;
	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 0 );
	g_CGLTriAPI.Vertex3fv( vecStart + (-v_up-v_right) * flBoostWidth * 1.2 );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 0, 1 );
	g_CGLTriAPI.Vertex3fv( vecStart + (v_up-v_right) * flBoostWidth * 1.2 );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 1 );
	g_CGLTriAPI.Vertex3fv( vecStart + (v_up+v_right) * flBoostWidth * 1.2 );

	g_CGLTriAPI.Brightness( 1 );
	g_CGLTriAPI.TexCoord2f( 1, 0 );
	g_CGLTriAPI.Vertex3fv( vecStart + (-v_up+v_right) * flBoostWidth * 1.2 );

	g_CGLTriAPI.End();
	g_CGLTriAPI.RenderMode( kRenderNormal );
}

void BoosterFX( void )
{	list<boostfx_s>::iterator iterCurrent;
	cl_entity_t *pEntity = NULL;

	ForEachIn(iterCurrent,gHUD.m_Boostfx.m_BoostFX)
	{	pEntity = gEngfuncs.GetEntityByIndex( iterCurrent->iIndex );

		// Remove dead double jump ents
		if(iterCurrent->blDoubleJump && iterCurrent->flDieTime < gEngfuncs.GetClientTime())
		{	iterCurrent = gHUD.m_Boostfx.RemoveBoostfx( iterCurrent );
			continue;
		}

		// Lets render the fx!
		if(iterCurrent->iterRecPos->blCanUse && iterCurrent->iterRecPos->blRenderedFrame && pEntity)
		{	int i, iMaxSpeed, iBackNum=0, iDownNum=0, iBackDownNum=0;
			float s;
			vec3_t vecBoneBack[MAX_BOOSTER];
			vec3_t vecBoneDown[MAX_BOOSTER];
			vec3_t vecBoneBackDown[MAX_BOOSTER];
			char szName[32];
			char szParent[32];
			bool blValid, blStopBack=false, blStopDown=false, blStopBackDown=false;

			// Get Owner
			cl_entity_t *pOwner = gEngfuncs.GetEntityByIndex( pEntity->curstate.aiment );
			vec3_t vecVel;
			if(pOwner)
			{	vecVel = g_StudioRenderer.GetVelocity(pOwner);
			}
			else
			{	continue;
			}


			// Get Bone origins for boosters
			for(i=1;i<=MAX_BOOSTER;i++)
			{	// Back boosters
				if(!blStopBack)
				{	sprintf(szName,"boost_back_%i", i);
					vecBoneBack[i] = gHUD.m_BonePos.GetBoneOrigin( iterCurrent->iterRecPos, szName, blValid );
					if(blValid)
					{	iBackNum = i;
					}
					else
					{	blStopBack = true;
					}
				}

				// Down boosters
				if(!blStopDown)
				{	sprintf(szName,"boost_down_%i", i);
					vecBoneDown[i] = gHUD.m_BonePos.GetBoneOrigin( iterCurrent->iterRecPos, szName, blValid );
					if(blValid)
					{	iDownNum = i;
					}
					else
					{	blStopDown = true;
					}
				}

				// BackDown (angled) boosters
				if(!blStopBackDown)
				{	sprintf(szName,"boost_backdown_%i", i);
					vecBoneBackDown[i] = gHUD.m_BonePos.GetBoneOrigin( iterCurrent->iterRecPos, szName, blValid );
					if(blValid)
					{	iBackDownNum = i;
					}
					else
					{	blStopBackDown = true;
					}
				}
				
			}			

			// Get booster amount
			if(iterCurrent->blDoubleJump)
			{	s = (iterCurrent->flDieTime - gEngfuncs.GetClientTime())/BOOST_DOUBLEJUMPTIME;
			}
			else
			{	s = float(Length(vecVel));
				// Get max speed
				iMaxSpeed = pOwner->curstate.iuser4;

				if(pOwner->curstate.movetype == MOVETYPE_PLAYERBOOST)
				{	s = 1;
				}
				else
				{	if(s > iMaxSpeed)
						s = iMaxSpeed;

					s /= iMaxSpeed;
				}
			}

			VectorNormalize(vecVel);

			if(iBackNum)
			{	for(i=1;i<=iBackNum;i++)
				{	// Get Back
					vec3_t vecBack;
					AngleVectors(pOwner->angles, vecBack, NULL, NULL);
					vecBack = -vecBack;

					RenderBoost( vecBoneBack[i], vecBack, vecVel, s, iterCurrent );
				}
			}

			if(iDownNum)
			{	for(i=1;i<=iDownNum;i++)
				{	vec3_t vecParent, vecVelocity;
					sprintf(szName,"boost_down_%i", i);

					// Get parent bone + parent bone - booster velocity
					vecParent = gHUD.m_BonePos.GetParentBoneOrigin( iterCurrent->iterRecPos, szName, blValid, *szParent );
					if(!blValid)
					{	continue;
					}
					vecVelocity = vecBoneDown[i] - vecParent;
					VectorNormalize(vecVelocity);


					RenderBoost( vecBoneDown[i], vecVelocity, vecVel, s, iterCurrent );
				}
			}

			if(iBackDownNum)
			{	for(i=1;i<=iBackDownNum;i++)
				{	sprintf(szName,"boost_backdown_%i", i);

					// Get parent bone + parent bone - booster velocity
					vec3_t vecSpine1 = gHUD.m_BonePos.GetParentBoneOrigin( iterCurrent->iterRecPos, szName, blValid, *szParent );
					vec3_t vecSpine = gHUD.m_BonePos.GetParentBoneOrigin( iterCurrent->iterRecPos, szParent, blValid, *szParent );
					
					if(!blValid)
					{	continue;
					}
					// Get Back
					vec3_t vecBack;
					AngleVectors(pOwner->angles, vecBack, NULL, NULL);
					vecBack = -vecBack;

					// Get Down
					vec3_t vecVelocity = vecSpine - vecSpine1;
					VectorNormalize(vecVelocity);

					// blend
					vecVelocity = vecVelocity + vecBack;
					VectorNormalize(vecVelocity);

					RenderBoost( vecBoneBackDown[i], vecVelocity, vecVel, s, iterCurrent );
				}
			}
		}
	}
}

void ResetRecBoneFrame( void )
{	recboneiter iterCurrent;
	ForEachIn(iterCurrent, gHUD.m_BonePos.m_RecBones)
	{	iterCurrent->blRenderedFrame = false;
	}
}

#define MAX_ICONDIST	2500
void DrawIconOverHead( int idx )
{	cl_entity_t *pEntity = gEngfuncs.GetEntityByIndex( idx );
	cl_entity_t	*pLocal = gEngfuncs.GetLocalPlayer();
	if(!pEntity || !pLocal)
	{	return;
	}

	if( !pEntity->player )
		return;

	if( !pEntity->model )
		return;
	if( !stricmp( pEntity->model->name, "models/null.mdl" ) )
		return;
	if( pEntity->curstate.effects & EF_NODRAW )
		return;

/*	if( pEntity->curstate.health < 0 )
		return;*/

	if( g_PlayerExtraInfo[idx].teamnumber == 0 )
	{
		return;
	}

	if( g_iTeamNumber == 0 )
	{
		return;
	}


	bool blSameTeam = (((g_PlayerExtraInfo[idx].teamnumber == g_PlayerExtraInfo[pLocal->index].teamnumber) && gHUD.m_Teamplay) ? true : false);

	// Dont draw yourself
	if(pEntity == pLocal)
	{	return;
	}

	// too far away
	if(!blSameTeam && ((pLocal->origin - pEntity->origin).Length() > MAX_ICONDIST))
	{	return;
	}

	// All seems well, draw the icon
	vec3_t vecIcon = pEntity->origin + vec3_t(0,0,32);
	
	// Green Sprite
	if(blSameTeam)
		Draw_GlobalSprite(g_hsprOverhead_green, vecIcon, kRenderTransAlpha, 12, 255,255,255,255,0);
	// Red Sprite
	else
		Draw_GlobalSprite(g_hsprOverhead_red, vecIcon, kRenderTransAlpha, 12, 255,255,255,255,0);

}

/*
=================
Draw_Triangles

Example routine.  Draws a sprite offset from the player origin.
=================
*/

// Test
/*
#include "r_efx.h"
bool blTest = false;
TEMPENTITY	*pEntity;
*/

bool g_blReplaceTexture = true;
void Draw_Triangles( void )
{
	int i;
	
	// Set view angles
	if(CL_IsThirdPerson())
	{	AngleVectors( g_CameraAngles, v_forward, v_right, v_up);	
	}
	else
	{	vec3_t vecViewAngles;
		gEngfuncs.GetViewAngles( (float *)vecViewAngles);
		AngleVectors( vecViewAngles, v_forward, v_right, v_up);	
	}

	if(( int )gEngfuncs.pfnGetCvarFloat( "cg_blur_steps" ) != 0)
		g_Effects.PreRender( );
	
	//added by jason 1-15-04
	if( cl_gltextures->value )
	{
		if( g_blReplaceTexture )
		{
			if( IEngineStudio.IsHardware() == 1 )
			{
				gTGAReplace.ReplaceAllTextures( );
				g_blReplaceTexture = false;
			}
		}
	}
	//end jason
	g_Glow.Test( );

	//g_Shadow.RenderShadow( );
	g_Shadow.blRenderd = false;
	

	if( ( int )gEngfuncs.pfnGetCvarFloat( "cg_glow_all" ) == 0 )
		g_Glow.CopyAndClearDisplay( );
	

	// Draw boosterfx
	BoosterFX();

	// Draw saberfx
	g_CGLTriAPI.DisableDepthMask( );
	SaberFX();
	if( g_Effects.m_blGo )
		g_Scythe.RenderScythe( );
	g_CGLTriAPI.EnableDepthMask( );

	g_AlphaBomb.LoopEffect( );
	
	// added by jason for particle system 1-5-03
	// Draw Particles
	GetManager()->UpdateSystems( );
	g_TrailMan.RenderTrails( );

	if( gEngfuncs.pfnGetCvarFloat( "cg_water" ) != 0)
		g_Effects.Render( );



	// added by jason
	if( ( int )gEngfuncs.pfnGetCvarFloat( "cg_glow_all" ) == 0 )
		g_Glow.RenderSelection( );
	else
		g_Glow.RenderScreenGlow( );
	// end jason


//	g_ClassMenu.Render( );
	if( gViewPort->m_pClassMenu->isVisible( ) )
	{
		g_ClassMenu.Render( );
	}
	else if( gViewPort->m_pTeamMenu->isVisible( )  )
	{
		g_TeamMenu.Render( );
	}
	else
	{
		if( gHUD.m_blHideHud )
			gHUD.m_blHideHud = false;
	}
		

//	g_PBuffer.RenderToScreen( );

	// added by jason for particle system 1-5-03
	// Draw Particles
//	GetManager()->UpdateSystems( );
//	g_TrailMan.RenderTrails( );
	// addded 4-15-04
//	g_Scythe.RenderScythe( );

	if(( int )gEngfuncs.pfnGetCvarFloat( "cg_blur_steps" ) != 0)
		g_Effects.PostRender( );

	// DEBUG: Draw BBoxes
//	DrawAllBBoxes();

	// Reset record bone frame render status
	ResetRecBoneFrame();

	// Render Check
	for( i=1;i<=32;i++)
	{	
		if(g_RenderCheck[i].blFlag)
		{	g_RenderCheck[i].blFlag = false;
		}
		else
		{	g_RenderCheck[i].blRendered = false;
		}
		// Draw icons over player head
		if(g_RenderCheck[i].blRendered)
		{	DrawIconOverHead(i);
		}
	}
}

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void DLLEXPORT HUD_DrawNormalTriangles( void )
{

	gHUD.m_Spectator.DrawOverview();
	g_Effects.WaterPlane( );
	
	
#if defined( TEST_IT )
//	Draw_Triangles();
#endif
}

/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void DLLEXPORT HUD_DrawTransparentTriangles( void )
{	Draw_Triangles();
}
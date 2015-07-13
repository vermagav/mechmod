/**********************************************************
*				OPENGL BORED EFFECTS
***********************************************************
*	Purpose: effects i created when i was bored
*	Created On: 5/21/04
*	Last Edited: 5/21/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include	"hud.h"
#include	"cl_util.h"
#include	"studio_util.h"

#include	<windows.h>
#include	<gl/gl.h>
#include	"glext.h"
#include	"cg_glow.h"
#include	"gl_bored.h"
#include	"texture.h"
#include	"tga_replace.h"
#include	"triangleAPI.h"
CBoredEffects g_Effects;
#include	"gl_pbuffer.h"
#include	"texture.h"
#include	"gl-triapi.h"

#include "ent_trail.h"
#include "ent_trail_man.h"
#include "ds_scythe.h"
#include "alpha_bomb.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "psys_man.h"

extern void BoosterFX( void );
extern void SaberFX( void );

bool g_blForceNormal = false;



void CBoredEffects::WaterPlane( void )
{
	if( ( int )gEngfuncs.pfnGetCvarFloat( "cg_water" ) == 0 )
	{
		m_blEnable = false;
	}



	if( m_blEnable  )
	{
		for( int j = 0; j < 1024; j++ )
		{
			cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex( j );

			if( pEnt )
			{
				if( pEnt->curstate.skin == CONTENTS_WATER )
				{
					pEnt->baseline.effects = EF_NODRAW;
					pEnt->baseline.renderamt = 0;
					pEnt->baseline.rendermode = kRenderTransAdd;

					pEnt->prevstate.effects = EF_NODRAW;
					pEnt->prevstate.renderamt = 0;
					pEnt->prevstate.rendermode = kRenderTransAdd;

					pEnt->curstate.effects = EF_NODRAW;
					pEnt->curstate.renderamt = 0;
					pEnt->curstate.rendermode = kRenderTransAdd;

				}
			}
		}
	}
}



void CBoredEffects::Render( void )
{
	if( !m_blEnable )
	{
		LoadData( );
	}

	m_blGo = true;

	if( m_blEnable )
	{
	//	BlackScr( );
		


		if( !g_Glow.m_blExts )
		{
			gEngfuncs.Con_Printf( "NO MULTITEXTURE FUNC\n" );
			return;
		}

		int j;

		glDisable( GL_BLEND );
		glEnable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, m_iScreen );
	//	glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_flWidth, m_flHeight, 0 );
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );
			
		//RenderScreenQuad( );
	//	glBindTexture( GL_TEXTURE_2D, m_iDepth );
	//	glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );


		BlackScr( );
	//	glClearColor( 0.925f, 0.00f, 0.549f, 1.0f );
	//	glClear( GL_COLOR_BUFFER_BIT );

		//g_PBuffer.ReleaseImage( );


		glDisable( GL_CULL_FACE );
		//glDisable( GL_DEPTH_TEST );
		glDepthMask( GL_FALSE );

		glBindTexture( GL_TEXTURE_2D, m_iNormal );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

		for( j = 0; j < 1024; j++ )
		{
			cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex( j );

			if( pEnt )
			{
				if( pEnt->curstate.skin == CONTENTS_WATER )
				{
						
					vec3_t vOrg1, vOrg2, vOrg3, vOrg4;
					vec3_t vTan, vBiNorm, vNorm, vTmp1, vTmp2, vTmp3;
					vec3_t vUv1, vUv2, vUv3, vUv4, vEye, vUvOffset;


					vUv1 = vec3_t( 0, 0, 0 );
					vUv2 = vUv1;
					vUv2.y = ( pEnt->curstate.maxs.y - pEnt->curstate.mins.y )/ 128;
					vUv3 = vUv2;
					vUv3.x = ( pEnt->curstate.maxs.x - pEnt->curstate.mins.x )/ 128;
					vUv4 = vUv3;
					vUv4.y = 0;
					vUvOffset.x = gEngfuncs.GetClientTime( )/2.0f;
					vUvOffset.y = cos( gEngfuncs.GetClientTime( ) )/5.0f;

					vOrg1 = vec3_t( pEnt->curstate.mins.x, pEnt->curstate.mins.y, pEnt->curstate.maxs.z + 1 );
					vOrg2 = vec3_t( pEnt->curstate.mins.x, pEnt->curstate.maxs.y, pEnt->curstate.maxs.z + 1);
					vOrg3 = vec3_t( pEnt->curstate.maxs.x, pEnt->curstate.maxs.y, pEnt->curstate.maxs.z + 1);
					vOrg4 = vec3_t( pEnt->curstate.maxs.x, pEnt->curstate.mins.y, pEnt->curstate.maxs.z + 1);
				

					//glDisable( GL_TEXTURE_2D );
					

				//	glMatrixMode( GL_MODELVIEW );

					//glDisable( GL_DEPTH_TEST );
					//glDepthMask(GL_FALSE); 
					
					
					glBegin( GL_QUADS );
					//	glNormal3fv( vec3_t( 0, 0, 1 ) );
						glTexCoord2fv( vUv1 + vUvOffset );
						glVertex3fv( vOrg1 );
						glTexCoord2fv( vUv2 + vUvOffset );
						glVertex3fv( vOrg2 );
						glTexCoord2fv( vUv3 + vUvOffset );
						glVertex3fv( vOrg3 );
						glTexCoord2fv( vUv4 + vUvOffset );
						glVertex3fv( vOrg4 );
					glEnd( );
					
					
					//glEnable( GL_DEPTH_TEST );


					//glEnable( GL_TEXTURE_2D );
					
				}
			}
		}

		glEnable( GL_CULL_FACE );

		glDepthMask(GL_TRUE); 

		
		
		glBindTexture( GL_TEXTURE_2D, m_iDecal );
	//	glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_flSize, m_flSize, 0 );
		
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );

		BlackScr( );


		//glClear( GL_CURRENT_BIT );

		glDisable( GL_TEXTURE_2D );
		
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	//	glDepthMask(GL_FALSE); 
		glDisable( GL_CULL_FACE );

		for( j = 0; j < 1024; j++ )
		{
			cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex( j );

			if( pEnt )
			{
				if( pEnt->curstate.skin == CONTENTS_WATER )
				{
						
					vec3_t vOrg1, vOrg2, vOrg3, vOrg4;
					vec3_t vTan, vBiNorm, vNorm, vTmp1, vTmp2, vTmp3;
					vec3_t vUv1, vUv2, vUv3, vUv4, vEye, vUvOffset;


					vOrg1 = vec3_t( pEnt->curstate.mins.x, pEnt->curstate.mins.y, pEnt->curstate.maxs.z );
					vOrg2 = vec3_t( pEnt->curstate.mins.x, pEnt->curstate.maxs.y, pEnt->curstate.maxs.z );
					vOrg3 = vec3_t( pEnt->curstate.maxs.x, pEnt->curstate.maxs.y, pEnt->curstate.maxs.z );
					vOrg4 = vec3_t( pEnt->curstate.maxs.x, pEnt->curstate.mins.y, pEnt->curstate.maxs.z );


					
					glBegin( GL_QUADS );
						glVertex3fv( vOrg1 );
						glVertex3fv( vOrg2 );
						glVertex3fv( vOrg3 );
						glVertex3fv( vOrg4 );
					glEnd( );

				}
			}
		}
		glEnable( GL_CULL_FACE );

		// render our black Objects
	//	glDepthMask( GL_TRUE );

		//glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		if( gEngfuncs.pfnGetCvarFloat( "cg_clipwater" ) )
		{
			StartBlack( );

			g_blForceNormal = true;
			BoosterFX();

			// Draw saberfx
			g_CGLTriAPI.DisableDepthMask( );
			SaberFX();
			g_CGLTriAPI.EnableDepthMask( );
			g_AlphaBomb.LoopEffect( );
			
			// added by jason for particle system 1-5-03
			// Draw Particles
			GetManager()->UpdateSystems( );
			g_TrailMan.RenderTrails( );
			
			g_Scythe.RenderScythe( );
			// addded 4-15-04
			

			EndBlack( );

			g_blForceNormal = false;
			g_CGLTriAPI.RenderMode( kRenderNormal );
		}

		glDepthMask(GL_FALSE); 
		

		glBindTexture( GL_TEXTURE_2D, m_iMask );
		//glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_flSize, m_flSize, 0 );
		glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );

		BlackScr( );


	//	g_Glow.m_glActiveTexARB( GL_TEXTURE0_ARB );
		glBindTexture( GL_TEXTURE_2D, m_iScreen);

		RenderScreenQuad( );

		
		g_CGShader.StartVertProf( );
		g_CGShader.StartFragProf( );
					
					
		if( m_pBumpVP )
			m_pBumpVP->Bind( );
		else
			gEngfuncs.Con_Printf( "Error on Vertex\n" );
					
		if( m_pBumpFP )
			m_pBumpFP->Bind( );
		else
			gEngfuncs.Con_Printf( "Error on fragment\n" );


		g_Glow.m_glActiveTexARB( GL_TEXTURE0_ARB );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_iDecal );

		
		g_Glow.m_glActiveTexARB( GL_TEXTURE1_ARB );
		glEnable( GL_TEXTURE_2D );
		//glGetIntegerv( GL_TEXTURE_BINDING_2D, &m_iTexture );
		glBindTexture( GL_TEXTURE_2D, m_iIdentity );
		
		

		g_Glow.m_glActiveTexARB( GL_TEXTURE2_ARB );
		glEnable( GL_TEXTURE_2D );
		//glGetIntegerv( GL_TEXTURE_BINDING_2D, &iSecTex );
		glBindTexture( GL_TEXTURE_2D, m_iScreen );

		g_Glow.m_glActiveTexARB( GL_TEXTURE3_ARB );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_iMask );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix( );
		glLoadIdentity( );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix( );
		glLoadIdentity( );

		glOrtho( 0, 1, 1, 0, 0.1f, 4096 );


		cgGLSetStateMatrixParameter( m_ModelProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );
		float mat2d[4] = {0, 0.05f, 0, 0.05f};
					
		cgGLSetParameter4fv( m_texDecal, mat2d );

		float flSize = 0.965f;

		//float flSize = (float( ScreenHeight )/ m_flSize) + ((( m_flSize/float( ScreenHeight) ) -1 )* 2);//0.95f;//( float( ScreenHeight/4 ) / float( m_flSize ) );
		//gEngfuncs.Con_Printf( "SH: %i, SIZE: %f, END: %f\n", ScreenHeight, m_flSize, flSize );
		cgGLSetParameter1f( m_Screen, flSize );

		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );


		glBegin( GL_QUADS );
			glTexCoord2f( 0, 0 );
			glVertex3f( 0, 1, -1 );


			glTexCoord2f( ScreenWidth/m_flWidth, 0 );
			glVertex3f( 1, 1, -1 );


			glTexCoord2f( ScreenWidth/m_flWidth, ScreenHeight/m_flHeight );
			glVertex3f( 1, 0, -1 );


			glTexCoord2f( 0, ScreenHeight/m_flHeight );
			glVertex3f( 0, 0, -1 );
		glEnd( );

		glMatrixMode( GL_PROJECTION );
		glPopMatrix( );

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix( );

		

		//glMatrixMode( GL_MODELVIEW );

		g_Glow.m_glActiveTexARB( GL_TEXTURE1_ARB );
		glDisable( GL_TEXTURE_2D );
		//glBindTexture( GL_TEXTURE_2D, m_iTexture );

		g_Glow.m_glActiveTexARB( GL_TEXTURE2_ARB );
		glDisable( GL_TEXTURE_2D );
		//glBindTexture( GL_TEXTURE_2D, m_iSecTex);

		g_Glow.m_glActiveTexARB( GL_TEXTURE3_ARB );
		glDisable( GL_TEXTURE_2D );

		g_Glow.m_glActiveTexARB( GL_TEXTURE0_ARB );
					
		g_CGShader.EndFragProf( );
		g_CGShader.EndVertProf( );
		
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );
		glDisable( GL_BLEND );

		//glDisable( GL_TEXTURE_2D );
	//	glDisable( GL_TEXTURE_2D );
	//	glEnable( GL_TEXTURE_2D );


	//	glDepthMask(GL_FALSE);
	}
}

CBoredEffects::CBoredEffects( )
{
	m_blEnable = false;
}

void CBoredEffects::LoadData( void )
{
	m_blGo = false;
	m_blEnable = true;

	if( !gTexture.IsOpenGL( ) )
	{
		m_blEnable = false;
		//gEngfuncs.Con_Printf( "NOT RUNNING IN OGL\n" );
		return;
	}

/*	if( ScreenWidth > 1024 )	// TAKES TOO LONG TO COPY LARGER TEXTURES
	{
		gEngfuncs.Con_Printf( "TOOO LARGE RESOLUTION\n" );
		m_blEnable = false;
	}*/

	if( ( int )gEngfuncs.pfnGetCvarFloat( "cg_water" ) == 0 )
	{
		gEngfuncs.Con_Printf( "CG WATER NOT ON\n" );
		m_blEnable = false;
	}

	char *pExt;
	bool blNvidia = true;
	pExt = strdup( ( char * ) glGetString( GL_EXTENSIONS ) );

	if( !isInString( "GL_NV_texture_shader", pExt ) )
		blNvidia = false;

	if( m_blEnable )
	{
		m_flWidth = 2;
		while( m_flWidth < ScreenWidth )
		{
			m_flWidth *= 2;
		}

		m_flHeight = 2;
		while( m_flHeight < ScreenHeight )
		{
			m_flHeight *= 2;
		}

		m_iScreen = 256;
		gTexture.CreateEmptyTex( m_flWidth, m_flHeight, m_iScreen, GL_TEXTURE_2D, GL_RGB );

		gTexture.CreateEmptyTex( m_flWidth, m_flHeight, m_iMask, GL_TEXTURE_2D, GL_RGB );

		gTexture.CreateEmptyTex( 256, 256, m_iIdentity, GL_TEXTURE_2D, GL_RGB );
		
		gTexture.CreateEmptyTex( m_flWidth, m_flHeight, m_iDecal, GL_TEXTURE_2D, GL_RGB );

		char myIdentityTexture[256][256][2];
		for(int y=0; y<256; y++)
		for(int x=0; x<256; x++)
		{
		  myIdentityTexture[y][x][0] = x;
		  myIdentityTexture[y][x][1] = y;
		}
		glBindTexture( GL_TEXTURE_2D, m_iIdentity );
		
		if( blNvidia )
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DSDT8_NV, 256, 256, 0, GL_DSDT_NV, GL_UNSIGNED_BYTE,  myIdentityTexture);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,  myIdentityTexture);
		}


		gTexture.CreateEmptyTex( 8, 8, m_iNormal, GL_TEXTURE_2D, GL_RGB );
		if( !gTexture.OverwriteTexture( m_iNormal, "noise", 0, false ) )
		{
			gEngfuncs.Con_Printf( "File Not Found\n" );
		}

		
		m_pBumpVP = g_CGShader.AddShader( "water_vp.cg", VERTEX_SHADER );

		if( m_pBumpVP )
		{
			m_ModelProj = cgGetNamedParameter( m_pBumpVP->cgProg, "ModelViewProj" );
			m_Screen = cgGetNamedParameter( m_pBumpVP->cgProg, "flScreen" );
		}
		else
		{
			gEngfuncs.Con_Printf( "Error on vertex\n" );
		}

		if( g_CGShader.GetFragProfile( ) == CG_PROFILE_FP20 )
		{
			m_pBumpFP = g_CGShader.AddShader( "fp20_water_fp.cg", FRAGMENT_SHADER );
			m_pBlackFP = g_CGShader.AddShader( "fp20_black_fp.cg", FRAGMENT_SHADER );
		}
		else
		{
			m_pBumpFP = g_CGShader.AddShader( "water_fp.cg", FRAGMENT_SHADER );
			m_pBlackFP = g_CGShader.AddShader( "black_fp.cg", FRAGMENT_SHADER );
		}

		m_pBlackVP = g_CGShader.AddShader( "black_vp.cg", VERTEX_SHADER );

		m_MVP = cgGetNamedParameter( m_pBlackVP->cgProg, "ModelViewProj" );
		
		if( !m_pBlackFP )
		{
			gEngfuncs.Con_Printf( "Error Loading Black Shader\n" );
		}

		if( m_pBumpFP )
		{
			m_texDecal = cgGetNamedParameter( m_pBumpFP->cgProg, "TexOffset" );
		}
		else
		{
			gEngfuncs.Con_Printf( "Error on fragment\n" );
		}


		if( !g_Glow.m_blExts )
		{
			gEngfuncs.Con_Printf( "NO MULTITEXTURE FUNC\n" );
			return;
		}


		g_Glow.m_glActiveTexARB( GL_TEXTURE0_ARB );
	}

}

void CBoredEffects::RenderScreenQuad( void )
{
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );

	glOrtho( 0, 1, 1, 0, 0.1f, 100 );

	glDisable( GL_DEPTH_TEST );
	glCullFace( GL_NONE );
	glDisable( GL_CULL_FACE );

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glBegin( GL_QUADS );
		glTexCoord2f( 0, 0 );
		glVertex3f( 0, 1, -1 );
		glTexCoord2f( ScreenWidth/m_flWidth, 0 );
		glVertex3f( 1, 1, -1 );
		glTexCoord2f( ScreenWidth/m_flWidth, ScreenHeight/m_flHeight );
		glVertex3f( 1, 0, -1 );
		glTexCoord2f( 0, ScreenHeight/m_flHeight );
		glVertex3f( 0, 0, -1 );
	glEnd( );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );

	glMatrixMode( GL_MODELVIEW );
}

void CBoredEffects::BlackScr( void )
{
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );

	glOrtho( 0, 1, 1, 0, 0.1f, 100 );

	glDisable( GL_DEPTH_TEST );
	glCullFace( GL_NONE );
	glDisable( GL_CULL_FACE );
	glDisable( GL_TEXTURE_2D );

	glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );

	glBegin( GL_QUADS );
		glVertex3f( 0, 1, -1 );
		glVertex3f( 1, 1, -1 );
		glVertex3f( 1, 0, -1 );
		glVertex3f( 0, 0, -1 );
	glEnd( );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );

	glMatrixMode( GL_MODELVIEW );
}

void CBoredEffects::PreRender( void )
{
	if( gTexture.IsOpenGL( ) )
	{
		g_Glow.m_glActiveTexARB( GL_TEXTURE1_ARB );
		glGetIntegerv( GL_TEXTURE_BINDING_2D, &m_iTexture );

		g_Glow.m_glActiveTexARB( GL_TEXTURE2_ARB );
		glGetIntegerv( GL_TEXTURE_BINDING_2D, &m_iSecTex );

		g_Glow.m_glActiveTexARB( GL_TEXTURE0_ARB );
	}
}

void CBoredEffects::PostRender( void )
{
	if( gTexture.IsOpenGL( ) )
	{

		g_Glow.m_glActiveTexARB( GL_TEXTURE1_ARB );
		glDisable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_iTexture );

		g_Glow.m_glActiveTexARB( GL_TEXTURE2_ARB );
		glDisable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_iSecTex);

		g_Glow.m_glActiveTexARB( GL_TEXTURE3_ARB );
		glDisable( GL_TEXTURE_2D );

		g_Glow.m_glActiveTexARB( GL_TEXTURE0_ARB );
	}
}

void CBoredEffects::StartBlack( void )
{
	g_CGShader.StartFragProf( );
	g_CGShader.StartVertProf( );
	if( m_pBlackFP )
		m_pBlackFP->Bind( );
	if( m_pBlackVP )
		m_pBlackVP->Bind( );

	cgGLSetStateMatrixParameter( m_MVP, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

}

void CBoredEffects::EndBlack( void )
{
	g_CGShader.EndFragProf();
	g_CGShader.EndVertProf( );
}

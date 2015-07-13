/**********************************************************
*					CG GLOW SHADER 
***********************************************************
*	Purpose: Fake hdr light glow effect
*	Created On: 4/17/04
*	Last Edited: 4/19/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
*	Code based on the tutorial from the hl collective
*	Credits: "Francis 'DeathWish' Woodhouse"
**********************************************************/

#include	"hud.h"
#include	"cl_util.h"
#include	"cg_glow.h"
#include	"texture.h"

//#include	<il/il.h>
//#include	<il/ilu.h>
//#include	<il/ilut.h>
extern int g_iWaterLevel;

extern bool isInString( char *string, const char *compare );


CGlow g_Glow;

CGlow::CGlow( )
{
	m_blInit = false;
	m_blRect = false;
	m_glActiveTexARB = NULL;
	m_glMultiTexCoord2fARB = NULL;
	m_blExts = false;
}

CGlow::~CGlow( )
{
	m_blInit = false;
	m_blRect = false;
	m_glActiveTexARB = NULL;
	m_glMultiTexCoord2fARB = NULL;
	m_blExts = false;
}

void CGlow::InitScreenGlow( void )
{
	if( IEngineStudio.IsHardware( ) != 1 )
	{
		return;
	}

	char *pExt;
	pExt = strdup( ( char * ) glGetString( GL_EXTENSIONS ) );
	
	m_glActiveTexARB = ( PFNGLACTIVETEXTUREARBPROC )wglGetProcAddress( "glActiveTextureARB" );
	m_glMultiTexCoord2fARB = ( PFNGLMULTITEXCOORD2FARBPROC )wglGetProcAddress( "glMultiTexCoord2fARB" );
	m_blExts = true;

	if( isInString( "GL_NV_texture_rectangle", pExt ) || isInString( "GL_EXT_texture_rectangle", pExt ) )
	{
		m_blRect = true;

		//gEngfuncs.pfnRegisterVariable( "cg_blur_steps", "4", 0 );

		// OPENGL EXTENSION LOADING
		glBindTexture( GL_TEXTURE_2D, 0 );
		

		// TEXTURE CREATION
		unsigned char* pTex = new unsigned char[ScreenWidth * ScreenHeight * 4];
		memset( pTex, 0, ScreenWidth * ScreenHeight * 4 );

	//	m_uiSceneTex = 3500;
		gTexture.CreateEmptyTex( ScreenWidth, ScreenHeight, m_uiSceneTex, GL_TEXTURE_RECTANGLE_NV, GL_RGB );
		//glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiSceneTex );

		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth, ScreenHeight, 0 );
		//glTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pTex );
		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR );	
		//glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//m_uiBlurTex = 3501;
		//glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiBlurTex );
		gTexture.CreateEmptyTex( ScreenWidth/2, ScreenHeight/2, m_uiBlurTex, GL_TEXTURE_RECTANGLE_NV, GL_RGB );
		
		//glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth/2, ScreenHeight/2, 0 );
		//glTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pTex );
		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR );	
		//glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_REPEAT);


		//m_uiBlurTex = 3502;
		gTexture.CreateEmptyTex( ScreenWidth, ScreenHeight, m_uiWorld, GL_TEXTURE_RECTANGLE_NV, GL_RGB );

		gTexture.CreateEmptyTex( ScreenWidth, ScreenHeight, m_uiDepth, GL_TEXTURE_RECTANGLE_NV, GL_DEPTH_COMPONENT );
		//glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiWorld );
		
		//glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth, ScreenHeight, 0 );
		//glTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pTex );
		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//glTexParameterf( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR );	
		//glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		delete [] pTex;

		m_blInit = true;

		//if( ( int )gEngfuncs.pfnGetCvarFloat( "cg_blur_steps" ) == 0 )
		//	return;
		// CG INIT

		//m_cgContext = cgCreateContext( );

		/*if( !m_cgContext )
		{
			gEngfuncs.Con_Printf( "Couldnt make cg context" );
			//MessageBox( NULL, "Couldnt make cg context", NULL, NULL );
			return;
		}*/

		// VERTEX PROFILES

		/*m_cgVertProf = cgGLGetLatestProfile( CG_GL_VERTEX );
		if( m_cgVertProf == CG_PROFILE_UNKNOWN )
		{
			gEngfuncs.Con_Printf( "couldnt fetch valid vp profile" );
			//MessageBox( NULL, "couldnt fetch valid vp profile", NULL, NULL );
			return;
		}

		cgGLSetOptimalOptions( m_cgVertProf );*/

		// VP LOADING
		/*if( !LoadCGProgram( &m_cgVP_GlowDarken, m_cgVertProf, "shaders/glow_darken_vp.cg" ) )
			return;

		if( !LoadCGProgram( &m_cgVP_GlowBlur, m_cgVertProf, "shaders/glow_blur_vp.cg" ) )
			return;

		if( !LoadCGProgram( &m_cgVP_GlowComb, m_cgVertProf, "shaders/glow_combine_vp.cg" ) )
			return;*/

		m_pVPGlowDark = g_CGShader.AddShader( "glow_darken_vp.cg", VERTEX_SHADER );
		m_pVPGlowComb = g_CGShader.AddShader( "glow_combine_vp.cg", VERTEX_SHADER );
		m_pVPGlowBlur = g_CGShader.AddShader( "glow_blur_vp.cg", VERTEX_SHADER );

	//	m_pVPTest = g_CGShader.AddShader( "offset_vp.cg", VERTEX_SHADER );

		// VP PARAM GRABBING
		m_cgpVP0_ModelViewMatrix = cgGetNamedParameter( m_pVPGlowDark->cgProg , "ModelViewProj" );

		m_cgpVP1_ModelViewMatrix = cgGetNamedParameter( m_pVPGlowBlur->cgProg, "ModelViewProj" );

		m_cgpVP2_ModelViewMatrix = cgGetNamedParameter( m_pVPGlowComb->cgProg, "ModelViewProj" );

	//	m_cgpVP3_ModelViewMatrix = cgGetNamedParameter( m_pVPGlowComb->cgProg, "projTM" );
	//	m_cgpVP3_Time = cgGetNamedParameter( m_pVPGlowComb->cgProg, "time" );

		m_cgpVP1_XOffset = cgGetNamedParameter( m_pVPGlowBlur->cgProg, "XOffset" );
		m_cgpVP1_YOffset = cgGetNamedParameter( m_pVPGlowBlur->cgProg, "YOffset" );
		

		// FRAGMENT PROFILE
		m_pFPGlowDark = g_CGShader.AddShader( "glow_darken_fp.cg", FRAGMENT_SHADER );
		m_pFPGlowBlur = g_CGShader.AddShader( "glow_blur_fp.cg", FRAGMENT_SHADER );
		m_pFPGlowComb = g_CGShader.AddShader( "glow_combine_fp.cg", FRAGMENT_SHADER );
	//	m_pFPTest = g_CGShader.AddShader( "offset_fp.cg", FRAGMENT_SHADER );

		/*m_cgFragProf = cgGLGetLatestProfile( CG_GL_FRAGMENT );
		if( m_cgFragProf == CG_PROFILE_UNKNOWN )
		{
			gEngfuncs.Con_Printf( "couldnt fetch valid fp profile" );
			//MessageBox( NULL, "couldnt fetch valid fp profile", NULL, NULL );
			return;
		}

		cgGLSetOptimalOptions( m_cgFragProf );

		// FP LOADING
		if( !LoadCGProgram( &m_cgFP_GlowDarken, m_cgFragProf, "shaders/glow_darken_fp.cg" ) )
			return;

		if( !LoadCGProgram( &m_cgFP_GlowBlur, m_cgFragProf, "shaders/glow_blur_fp.cg" ) )
			return;

		if( !LoadCGProgram( &m_cgFP_GlowComb, m_cgFragProf, "shaders/glow_combine_fp.cg" ) )
			return;*/
	}
}

void CGlow::DrawQuad( int iWidth, int iHeight )
{
	glBegin( GL_QUADS );

	glTexCoord2f( 0, 0 );
	glVertex3f( 0, 1, -1 );
	glTexCoord2f( 0, iHeight );
	glVertex3f( 0, 0, -1 );
	glTexCoord2f( iWidth, iHeight );
	glVertex3f( 1, 0, -1 );
	glTexCoord2f( iWidth, 0 );
	glVertex3f( 1, 1, -1 );

	glEnd( );
}

void CGlow::DoBlur( unsigned int uiSrcTex, unsigned int uiDstTex, int iSrcWidth, int iSrcHeight, 
					int iDstWidth, int iDstHeight, float flXOfs, float flYOfs )
{
	m_pVPGlowBlur->Bind( ); //cgGLBindProgram( m_cgVP_GlowBlur )
	m_pFPGlowBlur->Bind( );
	//cgGLBindProgram( m_cgFP_GlowBlur );

	m_glActiveTexARB( GL_TEXTURE0_ARB );
	glEnable( GL_TEXTURE_RECTANGLE_NV );
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, uiSrcTex );

	m_glActiveTexARB( GL_TEXTURE1_ARB );
	glEnable( GL_TEXTURE_RECTANGLE_NV );
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, uiSrcTex );
	
	m_glActiveTexARB( GL_TEXTURE2_ARB );
	glEnable( GL_TEXTURE_RECTANGLE_NV );
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, uiSrcTex );

	m_glActiveTexARB( GL_TEXTURE3_ARB );
	glEnable( GL_TEXTURE_RECTANGLE_NV );
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, uiSrcTex );

	cgGLSetParameter1f( m_cgpVP1_XOffset, flXOfs );
	cgGLSetParameter1f( m_cgpVP1_YOffset, flYOfs );

	glViewport( 0, 0, iDstWidth, iDstHeight );

	DrawQuad( iSrcWidth, iSrcHeight );

	glBindTexture( GL_TEXTURE_RECTANGLE_NV, uiDstTex );
	//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, iDstWidth, iDstHeight, 0 );
	glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, iDstWidth, iDstHeight );
	

}

void CGlow::RenderScreenGlow( void )
{
	if( IEngineStudio.IsHardware( ) != 1 )
		return;

	if( !m_blInit )
		InitScreenGlow( );

	if( m_blRect )
	{

	/*	glDisable( GL_BLEND );

		glDisable( GL_DEPTH_TEST );
		glEnable( GL_TEXTURE_RECTANGLE_NV );
		glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiDepth );
		//glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, 1024, 1024, 0 );
		glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );
	
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix( );
		glLoadIdentity( );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix( );
		glLoadIdentity( );

		glOrtho( 0, 1, 1, 0, 0.1, 100 );

		DrawQuad( ScreenWidth, ScreenHeight );
		
		glMatrixMode( GL_PROJECTION );
		glPopMatrix( );
		
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix( );

		glDisable( GL_TEXTURE_RECTANGLE_NV );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_BLEND );*/
		if( ( int )gEngfuncs.pfnGetCvarFloat( "cl_glglow" ) == 1 )
		{
			RenderGLGlow( );
			return;
		}

		if( ( int )gEngfuncs.pfnGetCvarFloat( "cg_blur_steps" ) == 0 )
			return;

		glDisable( GL_DEPTH_TEST );

		glDisable( GL_TEXTURE_2D );
		// Step1: grab the screen and put it into a texture
		m_glActiveTexARB( GL_TEXTURE0_ARB );
		glEnable( GL_TEXTURE_RECTANGLE_NV );
			
		glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiSceneTex );
		//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth, ScreenHeight, 0 );
		glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );
	

		// Step 2: set up an ortho projection
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix( );
		glLoadIdentity( );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix( );
		glLoadIdentity( );
		glOrtho( 0, 1, 1, 0, 0.1, 100 );

		glColor3f( 1, 1, 1 );

		// Step 3: init cg programs and parameters for darkening mid to dark aread of scene
		//cgGLEnableProfile( m_cgVertProf );
		//cgGLEnableProfile( m_cgFragProf );
		g_CGShader.StartVertProf( );
		g_CGShader.StartFragProf( );

		//cgGLBindProgram( m_cgVP_GlowDarken );
		//cgGLBindProgram( m_cgFP_GlowDarken );
		m_pVPGlowDark->Bind( );
		m_pFPGlowDark->Bind( );

		cgGLSetStateMatrixParameter( m_cgpVP0_ModelViewMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

		// Step 4: Render the Current scene to texture to a new, lower-res texture darkening non-bright areas of the scene
		glViewport( 0,0, ScreenWidth/2, ScreenHeight/2 );

		m_glActiveTexARB( GL_TEXTURE0_ARB );
		glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiSceneTex );

		DrawQuad( ScreenWidth, ScreenHeight );

		glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiBlurTex );
		//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth/2, ScreenHeight/2, 0 );
		glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, ScreenWidth/2, ScreenHeight/2 );
	

		// Step 5: Init cg programs for blurring
		m_pVPGlowBlur->Bind( );//cgGLBindProgram( m_cgVP_GlowBlur );
		m_pFPGlowBlur->Bind( );//cgGLBindProgram( m_cgFP_GlowBlur );

		cgGLSetStateMatrixParameter( m_cgpVP1_ModelViewMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

		// Step 6: apply blur
		int iNumBlur = ( int )gEngfuncs.pfnGetCvarFloat( "cg_blur_steps" );
		
		for( int i = 0; i < iNumBlur; i ++ )
		{
			DoBlur( m_uiBlurTex, m_uiBlurTex, ScreenWidth/2, ScreenHeight/2, ScreenWidth/2, ScreenHeight/2, 1, 0 );
			DoBlur( m_uiBlurTex, m_uiBlurTex, ScreenWidth/2, ScreenHeight/2, ScreenWidth/2, ScreenHeight/2, 0, 1 );
		}

		// Step 7: SetUp CG for combining blured glow and orignal scene
		m_pVPGlowComb->Bind( );//cgGLBindProgram( m_cgVP_GlowComb );
		m_pFPGlowComb->Bind( );//cgGLBindProgram( m_cgFP_GlowComb );

		cgGLSetStateMatrixParameter( m_cgpVP2_ModelViewMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

		m_glActiveTexARB( GL_TEXTURE0_ARB );
		glEnable( GL_TEXTURE_RECTANGLE_NV );
		glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiSceneTex );
		
		m_glActiveTexARB( GL_TEXTURE1_ARB );
		//int iTexture;
		//glGetIntegerv( GL_TEXTURE_BINDING_2D, &m_iTexture );
		glEnable( GL_TEXTURE_RECTANGLE_NV );
		glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiBlurTex );

		// Step 8: Do the combination, rendering to the screen without grabbing it to a texture
		glViewport( 0, 0, ScreenWidth, ScreenHeight );

		DrawQuad( ScreenWidth/2, ScreenHeight/2 );

		// Step 9: Restore the original projection and modelview matries and disable rectangular textures on all units
		glMatrixMode( GL_PROJECTION );
		glPopMatrix( );
		
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix( );

		g_CGShader.EndVertProf( );
		g_CGShader.EndFragProf( );//cgGLDisableProfile( m_cgVertProf );
		//cgGLDisableProfile( m_cgFragProf );

		m_glActiveTexARB( GL_TEXTURE0_ARB );
		glDisable( GL_TEXTURE_RECTANGLE_NV );

		m_glActiveTexARB( GL_TEXTURE1_ARB );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_TEXTURE_RECTANGLE_NV );

		m_glActiveTexARB( GL_TEXTURE2_ARB );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_TEXTURE_RECTANGLE_NV );

		m_glActiveTexARB( GL_TEXTURE3_ARB );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_TEXTURE_RECTANGLE_NV );

		m_glActiveTexARB( GL_TEXTURE0_ARB );

		glEnable( GL_DEPTH_TEST );

		glEnable( GL_TEXTURE_2D );

	}

}

/*void CGlow::GLGlow( void )
{
	if( IEngineStudio.IsHardware( ) != 1 )
		return;

	RenderGLGlow( );
}*/

void CGlow::RenderGLGlow( void )
{
/*
	MULTIPLY    :  (GL_ZERO, GL_SRC_COLOR)
	SCREEN      :  (GL_ONE, GL_ONE_MINUS_SRC_COLOR)
	ADD         :  (GL_ONE, GL_ONE )
	MULTI + ADD :  (GL_ONE, GL_SRC_COLOR)
	OPAQUE      :  (GL_ONE, GL_ZERO )*/
	glEnable( GL_TEXTURE_RECTANGLE_NV );
	glDisable( GL_DEPTH_TEST );
		
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiSceneTex );
	//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth, ScreenHeight, 0 );
	glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );
	

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );


	glOrtho( 0, 1, 1, 0, 0.1, 100 );

	glColor3f( 1.0f, 1.0f, 1.0f );

	glViewport( 0,0, ScreenWidth, ScreenHeight );
	
	//m_glActiveTexARB( GL_TEXTURE0_ARB );
	glEnable( GL_TEXTURE_RECTANGLE_NV );
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiSceneTex );

	DrawQuad( ScreenWidth*2, ScreenHeight*2 );
	
	

	glEnable( GL_TEXTURE_RECTANGLE_NV );
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiBlurTex );
	
	//ilutGLScreen( );
	glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, ScreenWidth/2, ScreenHeight/2 );
	
	//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth/2, ScreenHeight/2, 0 );
	//ilBindImage( m_uiBlurTex );
	//iluBlurAvg( 1 );

	//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth, ScreenHeight, 0 );
	//glTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL ), ilGetInteger( IL_IMAGE_WIDTH ), ilGetInteger( IL_IMAGE_HEIGHT ), 0, 
	//			  ilGetInteger( IL_IMAGE_FORMAT ), GL_UNSIGNED_BYTE, ilGetData( ) );

	glViewport( 0, 0, ScreenWidth, ScreenHeight );

	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiSceneTex );
	glColor3f( 0.5f, 0.5f, 0.5f );
	DrawQuad( ScreenWidth, ScreenHeight );


	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiBlurTex );
	glEnable( GL_BLEND );
	
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBlendFunc( GL_ONE, GL_ONE );
	DrawQuad( ScreenWidth/2, ScreenHeight/2 );
	glBlendFunc( GL_ONE, GL_ZERO );
	glDisable( GL_BLEND );


	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );
	
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );


	glEnable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_RECTANGLE_NV );
}


void CGlow::CopyAndClearDisplay( void )
{
	if( !gTexture.IsOpenGL( ) )
		return;

	glEnable( GL_TEXTURE_RECTANGLE_NV );
		
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiWorld );
	//glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth, ScreenHeight, 0 );
	glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );
	

	glDisable( GL_TEXTURE_RECTANGLE_NV );


	glDisable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );

	glOrtho( 0, 1, 1, 0, 0.1, 100 );

	glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );

	glViewport( 0,0, ScreenWidth, ScreenHeight );
	
	DrawQuad( ScreenWidth, ScreenHeight );


	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );
	
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );
	
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
}

void CGlow::CopySecondTex( void )
{
/*	glEnable( GL_TEXTURE_RECTANGLE_NV );
		
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiBlurTex );
	glCopyTexImage2D( GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, 0, 0, ScreenWidth, ScreenHeight, 0 );

	glDisable( GL_TEXTURE_RECTANGLE_NV );*/
}

void CGlow::RenderDisplay( void )
{
/*	glEnable( GL_TEXTURE_RECTANGLE_NV );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );

	glOrtho( 0, 1, 1, 0, 0.1, 100 );

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glViewport( 0,0, ScreenWidth, ScreenHeight );
	

	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiWorld );
	DrawQuad( ScreenWidth, ScreenHeight );


	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );
	
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	glDisable( GL_TEXTURE_RECTANGLE_NV );*/
}

void CGlow::BlendTexture( void )
{
	glEnable( GL_TEXTURE_RECTANGLE_NV );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );

	glOrtho( 0, 1, 1, 0, 0.1, 100 );

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glViewport( 0,0, ScreenWidth, ScreenHeight );
	
	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiWorld );

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );

	DrawQuad( ScreenWidth, ScreenHeight );

	glDisable( GL_BLEND );

	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );
	
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	glDisable( GL_TEXTURE_RECTANGLE_NV );
}

void CGlow::RenderSelection( void )
{
	if( IEngineStudio.IsHardware( ) != 1 )
		return;

	glDisable( GL_DEPTH_TEST );
	
	g_Glow.RenderScreenGlow( );

	//g_Glow.CopySecondTex( );
	
	//g_Glow.RenderDisplay( );
	g_Glow.BlendTexture( );
	glEnable( GL_DEPTH_TEST );
}
void CGlow::Test( void )
{
	if( IEngineStudio.IsHardware( ) != 1 )
		return;

	if( !gEngfuncs.pfnGetCvarFloat( "gl_waterblur" ) )
		return;

	if( !m_blInit )
		InitScreenGlow( );

	if( m_blRect )
	{
		glDisable( GL_DEPTH_TEST );
		g_Glow.TestRender( );
		glEnable( GL_DEPTH_TEST );
	}

}

void CGlow::TestRender( void )
{
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );

	glOrtho( 0, 1, 1, 0, 0.1, 100 );

	
	glViewport( 0,0, ScreenWidth, ScreenHeight );

	glBindTexture( GL_TEXTURE_RECTANGLE_NV, m_uiWorld );
	
	if( gEngfuncs.PM_PointContents( gEngfuncs.GetLocalPlayer( )->origin + vec3_t( 0, 0, 8 ), NULL ) == CONTENTS_WATER  )
	{
		glColor4f( 0.80f, 0.80f, 1.00f, 0.6f );
	}
	else
	{
		glColor4f( 1.0f, 1.0f, 1.0f, 0.0f );
	}
	DrawQuad( ScreenWidth, ScreenHeight );

	glCopyTexSubImage2D( GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, ScreenWidth, ScreenHeight );
	
	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );
	
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	glDisable( GL_TEXTURE_RECTANGLE_NV );
	glDisable( GL_BLEND );

	glBlendFunc( GL_ONE, GL_ZERO );

}

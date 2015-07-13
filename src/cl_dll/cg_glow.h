/**********************************************************
*					CG GLOW SHADER 
***********************************************************
*	Purpose: Fake hdr light glow effect
*	Created On: 4/17/04
*	Last Edited: 4/17/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
*	Code based on the tutorial from the hl collective
*	Credits: "Francis 'DeathWish' Woodhouse"
**********************************************************/


#ifndef		CGLOW_H
#define		CGLOW_H

#include	<windows.h>
#include	<gl/gl.h>
#include	"glext.h"
#include	"cg_shader.h"

#include	"r_studioint.h"

extern		engine_studio_api_t IEngineStudio;

class CGlow
{
public:
	CGlow( );
	~CGlow( );
	void	InitScreenGlow( void );
	void	CopyAndClearDisplay( void );
	void	RenderSelection( void );
	void	RenderScreenGlow( void );
	void	Test( void );
	void	TestRender( void );

	PFNGLACTIVETEXTUREARBPROC	m_glActiveTexARB;
	PFNGLMULTITEXCOORD2FARBPROC	m_glMultiTexCoord2fARB;
	bool	m_blExts;

private:
	void	CopySecondTex( void );
	void	RenderDisplay( void );
	void	BlendTexture( void );
	void	RenderGLGlow( void );
	void	GLGlow( void );
	//bool	LoadCGProgram( CGprogram *pProg, CGprofile cgProfile, const char *pFile );
	
	void	DrawQuad( int iWidth, int iHeight );
	void	DoBlur( unsigned int uiSrcTex, unsigned int uiDstTex, int iSrcWidth, int iSrcHeight, 
					int iDstWidth, int iDstHeight, float flXOfs, float flYOfs );


	bool		m_blInit;
	bool		m_blRect;	// checks if they have rectangle extension


	Shader_s	*m_pFPGlowDark;
	Shader_s	*m_pVPGlowDark;

	Shader_s	*m_pFPGlowBlur;
	Shader_s	*m_pVPGlowBlur;

	Shader_s	*m_pFPGlowComb;
	Shader_s	*m_pVPGlowComb;

	Shader_s	*m_pVPTest;
	Shader_s	*m_pFPTest;

	CGparameter	m_cgpVP0_ModelViewMatrix;
	CGparameter	m_cgpVP1_ModelViewMatrix;
	CGparameter m_cgpVP2_ModelViewMatrix;
	CGparameter m_cgpVP3_ModelViewMatrix;
	CGparameter	m_cgpVP1_XOffset;
	CGparameter	m_cgpVP1_YOffset;
	CGparameter m_cgpVP3_Time;

	unsigned int	m_uiSceneTex;
	unsigned int	m_uiBlurTex;

	unsigned int	m_uiWorld;

	unsigned int	m_uiDepth;
};

extern CGlow g_Glow;

#endif
/**********************************************************
*				OPENGL BORED EFFECTS
***********************************************************
*	Purpose: effects i created when i was bored
*	Created On: 5/21/04
*	Last Edited: 5/21/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#ifndef		CGBORED
#define		CGBORED

#include	"cg_shader.h"

class CBoredEffects
{
public:
	CBoredEffects( void );
	/*************
	* Water Plane
	* @purpose: removes the hl waterplane
	* @param void: no param
	* @return void: no return
	*************/
	void WaterPlane( void );
	/*************
	* Render
	* @purpose: renders the new water
	* @param void: no param
	* @return void: no return
	*************/
	void Render( void );
	/*************
	* Load Data
	* @purpose: initializes our vars
	* @param void: no param
	* @return void: no return
	*************/
	void LoadData( void );
	/*************
	* Render Screen Quad
	* @purpose: renders bound texture to screen
	* @param void: no param
	* @return void: no return
	*************/
	void RenderScreenQuad( void );
	/*************
	* Blac Scr
	* @purpose: renders a black quad to screen, faster then clearcolor
	* @param void: no param
	* @return void: no return
	*************/
	void BlackScr( void );

	
	void PreRender( void );


	void PostRender( void );

	void StartBlack( void );

	void EndBlack( void );

	bool m_blGo;
private:
	bool			m_blEnable;		// is allowd to run effect
	float			m_flWidth, m_flHeight;		// texture size
	unsigned int	m_iMask;		// mask texture
	unsigned int	m_iDecal;		// decal texture
	unsigned int	m_iNormal;		// grayscale texture
	unsigned int	m_iScreen;		// screen texture
	unsigned int	m_iIdentity;	// dsdt texture for fp20
	Shader_s		*m_pBumpFP;		// fragment shader
	Shader_s		*m_pBumpVP;		// vertex shader
	Shader_s		*m_pBlackFP;	// Fragement shader for grayscale
	Shader_s		*m_pBlackVP;	// Vertex shader for grayscale
	CGparameter		m_MVP;			// model view prog matrix for gray
	CGparameter		m_ModelProj;	// modelview projection matrix
	CGparameter		m_texDecal;		// texture offset, too lazy to change
	CGparameter		m_Screen;		// image offset
	int				m_iTexture, m_iSecTex;
};

class CShadow
{
public:
	CShadow( void );
	void RenderShadow( void );//vec3_t vOrg );
	void LoadTex( void );
	bool blRenderd;
	void SetShadowOrg( vec3_t vOrg );
private:
//	vec3_t		m_vOrg1, m_vOrg2, m_vOrg3, m_vOrg4, m_vOrg5;
	Texture_s *m_pTex;
	std::list<vec3_t> m_vOrgs;
};

extern CShadow g_Shadow;

extern CBoredEffects g_Effects;

#endif

/**********************************************************
*				CG SHADER MANAGER
***********************************************************
*	Purpose: Manages shader files
*	Created On: 4/19/04
*	Last Edited: 4/19/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#ifndef		CG_SHADER_H
#define		CG_SHADER_H

#include	<windows.h>

#include	<cg/cg.h>
#include	<cg/cgGL.h>
#include	"texture.h"

#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cggl.lib")

struct Shader_s
{
	char		szName[32];			// shader name
	int			iType;				// shader type, frag or vert
	CGprogram	cgProg;				// shader program data
	Shader_s	*pNext;				// next link in list
	Shader_s	*pPrev;				// previous link in list
	/*************
	* Bind
	* @purpose: binds shader for drawing use
	* @param void: no param
	* @return void: no return
	*************/
	void		Bind( void ) { if( gTexture.IsOpenGL( ) )cgGLBindProgram( cgProg ); };
};

#define		FRAGMENT_SHADER	1
#define		VERTEX_SHADER	2


class CCGShader
{
public:
				CCGShader( );		// constructor
				~CCGShader( );		// destructor

	/*************
	* Is Supported
	* @purpose: Tells us if cg is fully supported
	* @param void: no param
	* @return bool: if its supported or not
	*************/
	bool		IsSupported( void );
	/*************
	* Remove all
	* @purpose: Removes all loaded shaders on map change
	* @param void: no param
	* @return void: no return
	*************/
	void		RemoveAll( void );
	/*************
	* Remove
	* @purpose: removes a selected shader
	* @param Shader_s *pShader: pointer to shader to be removed
	* @return Shader_s: Next Shader in list
	*************/
	Shader_s	*Remove( Shader_s *pShader );
	/*************
	* AddShader
	* @purpose: Adds shader by name, and type
	* @param char *pName: filename of the shader
	* @param int iType: weither its a fragment shader or vertexshader
	* @return Shader_s: shader we added
	*************/
	Shader_s	*AddShader( char *pName, int iType );
	/*************
	* StartVertProf
	* @purpose: enables vertex profile
	* @param void: no param
	* @return void: no return
	*************/
	void		StartVertProf( void ) { if( gTexture.IsOpenGL( ) ) cgGLEnableProfile( m_cgVertProf ); }
	/*************
	* StartFragProf
	* @purpose: enables fragment profile
	* @param void: no param
	* @return void: no return
	*************/
	void		StartFragProf( void ) { if( gTexture.IsOpenGL( ) )cgGLEnableProfile( m_cgFragProf ); }
	/*************
	* EndVertProf
	* @purpose: disables vertex profile
	* @param void: no param
	* @return void: no return
	*************/
	void		EndVertProf( void ) { if( gTexture.IsOpenGL( ) )cgGLDisableProfile( m_cgVertProf ); }
	/*************
	* EndFragProf
	* @purpose: disables fragment profile
	* @param void: no param
	* @return void: no return
	*************/
	void		EndFragProf( void ) { if( gTexture.IsOpenGL( ) )cgGLDisableProfile( m_cgFragProf ); }
	/*************
	* GetFragProfile
	* @purpose: gets fragment profile for selective shaders
	* @param void: no param
	* @return CGprofile: profile
	*************/
	CGprofile	GetFragProfile( void );
	/*************
	* SetShaderEnv
	* @purpose: sets up shader info 
	* @param void: no param
	* @return void: no return
	*************/
	void		SetShaderEnv( void );

private:
	/*************
	* FindShader
	* @purpose: finds shader by name
	* @param char *pName: shaders name
	* @return Shader_s: shader if it was found
	*************/
	Shader_s	*FindShader( char *pName );
	/*************
	* LoadShader
	* @purpose: Loads Shader by name, and type
	* @param char *pName: Shader file name
	* @param int iType: type of shader, frag or vert
	* @return bool: tells us if shader was loaded
	*************/
	bool		LoadShader( char *pName, int iType );
	

	CGcontext	m_cgContext;		// cg context
	CGprofile	m_cgVertProf;		// vertex profile
	CGprofile	m_cgFragProf;		// fragment profile
	Shader_s	*m_pHeadShader;		// head to list of shaders
	bool		m_blIsSupported;	// tells us if cg is supported on the vid card
};

extern CCGShader g_CGShader;

#endif
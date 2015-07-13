/**********************************************************
*				OPENGL TRIAPI WRAPPER
***********************************************************
*	Purpose: calls approprite gl or tri func
*	Created On: 5/20/04
*	Last Edited: 5/20/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#ifndef GLTRIAPI_H
#define GLTRIAPI_H

#include "triangleAPI.h"


class CGLTriAPI
{
public:
			CGLTriAPI( void );		// constructor
	/*************
	* OGL ACTIVE
	* @purpose: gets info if ogl is active rendering device
	* @param void: no param
	* @return void: no return
	*************/
	void	OGLActive( void );
	/*************
	* Begin
	* @purpose: start of primitive rendering
	* @param int iPrim: type of primitive to render
	* @return void: no return
	*************/
	void	Begin( int iPrim );
	/*************
	* End
	* @purpose: End of primitive rendering
	* @param void: no param
	* @return void: no return
	*************/
	void	End( void );
	/*************
	* End
	* @purpose: set brightness
	* @param flBrightness: brightness
	* @return void: no return
	*************/
	void	Brightness( float flBrightness );
	/*************
	* Color4f
	* @purpose: set vertex color
	* @param float r, g, b, a: color components to set
	* @return void: no return
	*************/
	void	Color4f( float r, float g, float b, float a );
	/*************
	* Color4fv
	* @purpose: set vertex color
	* @param float *flColor: color to set
	* @return void: no return
	*************/
	void	Color4fv( float *flColor );
	/*************
	* Color4fv
	* @purpose: set vertex color
	* @param colorRGBA clColor: color to set
	* @return void: no return
	*************/
	void	Color4fv( colorRGBA clColor );
	/*************
	* Color4ub
	* @purpose: set vertex color
	* @param unsigned char r, g, b, a: color to set
	* @return void: no return
	*************/
	void	Color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a );
	/*************
	* Color4ubv
	* @purpose: set vertex color
	* @param unsigned char *cColor: color to set
	* @return void: no return
	*************/
	void	Color4ubv( unsigned char *cColor );
	/*************
	* TexCoord2f
	* @purpose: set vertex texture coordinates
	* @param float u,v: uv texture coords
	* @return void: no return
	*************/
	void	TexCoord2f( float u, float v );
	/*************
	* TexCoord2fv
	* @purpose: set vertex texture coordinates
	* @param float *uv: uv texture coords
	* @return void: no return
	*************/
	void	TexCoord2fv( float *uv );
	/*************
	* Vertex3f
	* @purpose: set vertex
	* @param float x,y,z: xyz coordinates of point
	* @return void: no return
	*************/
	void	Vertex3f( float x, float y, float z );
	/*************
	* Vertex3fv
	* @purpose: set vertex
	* @param float *point: xyz coordinates of point
	* @return void: no return
	*************/
	void	Vertex3fv( float *point );
	/*************
	* CullFace
	* @purpose: set culling mode
	* @param int iCull: culling mode
	* @return void: no return
	*************/
	void	CullFace( TRICULLSTYLE iCull );
	/*************
	* Texture2D
	* @purpose: set texture for surface
	* @param Texture_s *pTex: Texture Data
	* @param int iFrame: animation frame
	* @return void: no return
	*************/
	void	Texture2D( Texture_s *pTex, int iFrame );
	/*************
	* Texture2D
	* @purpose: set texture for surface
	* @param model_s *pTex: halflife sprite texture
	* @param int iFrame: animation frame
	* @return void: no return
	*************/
	void	Texture2D( model_s *pTex, int iFrame );
	/*************
	* RenderMode
	* @purpose: set render mode
	* @param int iMode: rendermode to set
	* @return void: no return
	*************/
	void	RenderMode( int iMode );
	/*************
	* EnableDepthMask
	* @purpose: (OGLONLY) enables depth masking, writes to depth buffer
	* @param void: no param
	* @return void: no return
	*************/
	void	EnableDepthMask( void );
	/*************
	* Disable Depth Mask
	* @purpose: ( OGLONLY ) disables depth masking, obeys depth buffer but doesnt write to it
	* @param void: no param
	* @return void: no return
	*************/
	void	DisableDepthMask( void );

private:
	/*************
	* IsOGL
	* @purpose: tells us if the render'er is opengl
	* @param void: no param
	* @return bool: is it opengl
	*************/
	bool	IsOGL( void );


	bool	m_blOGL;		// are we using ogl
};

extern CGLTriAPI g_CGLTriAPI;
#endif
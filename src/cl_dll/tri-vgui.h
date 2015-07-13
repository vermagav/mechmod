/**********************************************************
*			TRI API VGUI SCREENS FOR MORE FLEXABILITY
***********************************************************
*	Purpose: Displays triapi stuff behind vgui
*	Created On: 9/13/04
*	Last Edited: 9/13/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#ifndef TRI_VGUI
#define TRI_VGUI

#include "texture.h"
#include <string>

class C2DVector
{
public:
	// constructors
	inline	C2DVector( void )						{}
	inline	C2DVector( float flX, float flY )		{ x = flX; y = flY; }
	inline	C2DVector( const C2DVector &vVec )		{ x = vVec.x; y = vVec.y; }
	inline	C2DVector( float flXY[] )				{ x = flXY[0]; y = flXY[1]; }

	// destructors
	inline	~C2DVector( void )						{}

	// operators
	inline	C2DVector	operator-( void ) const					{ return C2DVector( -x, -y ); }
	inline	C2DVector	operator*( float flScale ) const		{ return C2DVector( x * flScale, y * flScale ); }
	inline	C2DVector	operator/( float flScale ) const		{ return C2DVector( x / flScale, y / flScale ); }
	inline	C2DVector	operator+( C2DVector vXY ) const		{ return C2DVector( x + vXY.x, y + vXY.y ); }
	inline	C2DVector	operator-( C2DVector vXY ) const		{ return C2DVector( x - vXY.x, y - vXY.y ); }
	inline	bool		operator==( C2DVector vXY ) const		{ return ( x == vXY.x && y == vXY.y ); }
	inline	bool		operator!=( C2DVector vXY ) const		{ return ( !( *this == vXY ) ); }

	union
	{
		float		flXY[2];

		struct
		{
			float	x;
			float	y;
		};
		struct
		{
			float	u;
			float	v;
		};
	};

private:
};

typedef C2DVector vec2_t;

class CTriMenu
{
public:
			CTriMenu( void );
			~CTriMenu( void );
	void	UpdateTexture( const char *pTexName );
	void	UpdatePosition( vec2_t vStart, float flHeight, float flWidth );
	void	Draw( void );
	vec3_t	MakePointOrtho( vec2_t vOrg );
	Texture_s	*m_pTex;	// texture of object
private:
	vec2_t		m_vPos[4];	// coords of object
	
	std::string m_strTex;	// texture name
};

extern vec3_t	g_vViewOrg;
extern vec3_t	g_vecCamAngles;
extern vec3_t	g_vPlayerAngles;

class CClassMenu
{
public:
	void	Render( void );
	void	Reset( void );
	void	Initialize( void );
private:
	CTriMenu m_TriMenu;
	CTriMenu m_TriMenu1;
	CTriMenu m_TriMenu2;
	CTriMenu m_TriMenu3;
};

class CTeamMenu
{
public:
	void	Render( void );
	void	Reset( void );
	void	Initialize( void );
private:
	CTriMenu m_TriMenu;
	CTriMenu m_TriMenu1;
	CTriMenu m_TriMenu2;
	CTriMenu m_TriMenu3;
};

extern CClassMenu g_ClassMenu;
extern CTeamMenu g_TeamMenu;


#endif
/**********************************************************
*				CG SHADER MANAGER
***********************************************************
*	Purpose: Manages shader files
*	Created On: 4/19/04
*	Last Edited: 5/20/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include	"hud.h"
#include	"cl_util.h"
#include	"r_studioint.h"
#include	"cg_shader.h"

CCGShader g_CGShader;

extern		engine_studio_api_t IEngineStudio;

CCGShader::CCGShader( )
{
	m_pHeadShader	= NULL;
	m_cgContext		= NULL;
	m_blIsSupported = false;
}

CCGShader::~CCGShader( )
{
	RemoveAll( );
	m_cgContext		= NULL;
	m_blIsSupported = false;
}

Shader_s *CCGShader::FindShader( char *pName )
{
	Shader_s *pShader = m_pHeadShader;

	while( pShader )
	{
		if( !stricmp( pName, pShader->szName ) )
		{
			return pShader;
		}
		
		pShader = pShader->pNext;
	}

	return NULL;
}

void CCGShader::RemoveAll( void )
{
	Shader_s *pShader = m_pHeadShader;

	while( pShader )
	{
		pShader = Remove( pShader );
	}

	m_pHeadShader = NULL;
	m_blIsSupported = false;
}

Shader_s *CCGShader::Remove( Shader_s *pShader )
{
	Shader_s *pTmp = pShader->pNext;

	if( pShader == m_pHeadShader )
	{
		m_pHeadShader = pShader->pNext;
	}

	if( pShader->pNext )
	{
		pShader->pNext->pPrev = pShader->pPrev;
	}
	if( pShader->pPrev )
	{
		pShader->pPrev->pNext = pShader->pNext;
	}

	delete pShader;

	return pTmp;
}

Shader_s *CCGShader::AddShader( char *pName, int iType )
{
	if( IEngineStudio.IsHardware( ) != 1 )
	{
		return NULL;
	}

	Shader_s *pNew = FindShader( pName );

	if( !m_cgContext )
		SetShaderEnv( );
	
	if( pNew )
	{
		return pNew;
	}

	char szName[128];
	sprintf( szName, "%s/shaders/%s", gEngfuncs.pfnGetGameDirectory( ), pName );

	if( LoadShader( szName, iType ) )
	{
		pNew = new Shader_s;

		strcpy( pNew->szName, pName );

		if( iType == FRAGMENT_SHADER )
			pNew->cgProg = cgCreateProgramFromFile( m_cgContext, CG_SOURCE, szName, m_cgFragProf, "main", 0 );
		if( iType == VERTEX_SHADER )
			pNew->cgProg = cgCreateProgramFromFile( m_cgContext, CG_SOURCE, szName, m_cgVertProf, "main", 0 );
		
		pNew->iType = iType;

		pNew->pPrev = NULL;
		pNew->pNext = m_pHeadShader;

		if( m_pHeadShader )
		{
			m_pHeadShader->pPrev = pNew;
		}

		m_pHeadShader = pNew;


		cgGLLoadProgram( pNew->cgProg );

	}

	return pNew;
}

bool CCGShader::LoadShader( char *pName, int iType )
{
	CGprogram	cgValid;

	if( iType == FRAGMENT_SHADER )
		cgValid = cgCreateProgramFromFile( m_cgContext, CG_SOURCE, pName, m_cgFragProf, "main", 0 );
	if( iType == VERTEX_SHADER )
		cgValid = cgCreateProgramFromFile( m_cgContext, CG_SOURCE, pName, m_cgVertProf, "main", 0 );


	if( !cgValid )
	{
		gEngfuncs.Con_Printf( "%s\n", cgGetErrorString( cgGetError( ) ) );
		return false;
	}

	return true;
}

void CCGShader::SetShaderEnv( void  )
{
	m_cgContext = cgCreateContext( );

	if( !m_cgContext )
	{
		// PRINT ERROR
		m_blIsSupported = false;
		return;
	}

	m_cgVertProf = cgGLGetLatestProfile( CG_GL_VERTEX );

	if( m_cgVertProf == CG_PROFILE_UNKNOWN )
	{
		// PRINT ERROR
		m_blIsSupported = false;
		return;
	}

	cgGLSetOptimalOptions( m_cgVertProf );

	m_cgFragProf = cgGLGetLatestProfile( CG_GL_FRAGMENT );

	if( m_cgFragProf == CG_PROFILE_UNKNOWN )
	{
		// PRINT ERROR
		m_blIsSupported = false;
		return;
	}

	cgGLSetOptimalOptions( m_cgFragProf );

	m_blIsSupported = true;
}


bool CCGShader::IsSupported( void )
{
	// lets us know if cg is supported
	return m_blIsSupported;
}

CGprofile CCGShader::GetFragProfile( void )
{
	return m_cgFragProf;
}
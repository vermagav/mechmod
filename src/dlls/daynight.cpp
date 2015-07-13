/**********************************************************
*				DAY NIGHT FADING
***********************************************************
*	Purpose: Day and night fading
*	Created On: 1/22/04
*	Last Edited: 1/22/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
// entity to define day and night effect fading on normal lights

class CDayNight : public CBaseEntity
{
public:
	void	Spawn( void );
	void	KeyValue( KeyValueData *pkvd );
	void	EXPORT	Update( void );
	char	FindColor( int iTod );
private:
	float	m_flStart;	// Start time
	float	m_flLast;	// Last iteration
	float	m_flEnd;	// End Time
	int		m_iCurTod;	// current time of day
	int		m_iEdDay;	// end of day time
	int		m_iStDay;	// start of day time
	int		m_iDays;	// How many days
	char	m_cTOD;		// Time of day
	bool	m_blInc;	// Do we incrament or decrament
};

LINK_ENTITY_TO_CLASS( daynight, CDayNight );

void CDayNight::Spawn( void )
{
	// Spawn info for day and night cycle
	m_flStart = gpGlobals->time;
	m_cTOD = FindColor( m_iCurTod );
	LIGHT_STYLE( 0, &m_cTOD );

	pev->nextthink = gpGlobals->time +0.1;
	SetThink( Update );
}


char CDayNight::FindColor( int iTod )
{
	// colors are 97 - 122 NOTE: 97 is black 122 is white
	int iTmp = (iTod-12 ) * (iTod-12 );
	iTod = ((-12.0/144.0)* iTmp ) + 12;//abs( ( iTod/2 )-12 ) + 100;	// if iTod = 0 midnight so black
	iTod += 100;

	return char( iTod );
}

void EXPORT CDayNight::Update( void )
{
	// first find time to map change
	m_flEnd = ( CVAR_GET_FLOAT("mp_timeleft") );

	// Find Time increments
	float flTime = m_flEnd / ( m_iEdDay - m_iStDay + m_iDays * 24 );
	pev->nextthink = gpGlobals->time + flTime; 

	m_iCurTod ++;
	if( m_iCurTod < 0 )
	{
		//m_blInc = true;
		m_iCurTod = 24;
	}
	if( m_iCurTod > 24 )
	{
		//m_blInc = false;
		m_iCurTod = 0;
	}

	m_cTOD = FindColor( m_iCurTod );
	LIGHT_STYLE( 0, &m_cTOD );

	//ALERT( at_console, "%i, time to change: %f\n", m_cTOD, pev->nextthink );
}

void CDayNight::KeyValue( KeyValueData *pkvd )
{
	// get our ents data
	if( FStrEq( pkvd->szKeyName, "Start" ) )
	{
		m_iCurTod = m_iStDay = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	if( FStrEq( pkvd->szKeyName, "End" ) )
	{
		m_iEdDay = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	if( FStrEq( pkvd->szKeyName, "Days" ) )
	{
		m_iDays = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CBaseEntity::KeyValue( pkvd );
	}
}

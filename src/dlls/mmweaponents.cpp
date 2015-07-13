/*
* Mechmod base entity weapon system
* @version: 1/18/04
* @author: Joseph 'Pcjoe' Florencio
*/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "mmweapons.h"

// added by jason
#include "../cl_dll/ent_trail_def.h"
extern int gmsgEntTrail;
#include "../cl_dll/eff_def.h"
extern int gmsgPartMsg;

/*
* ---------------
* Base Projectile
* ---------------
*/

/*
* Spawn
*/
void CBaseProjectile::Spawn( void )
{	Precache();
	pev->movetype	= MOVETYPE_FLYMISSILE;
	pev->solid		= SOLID_TRIGGER;
	pev->classname = MAKE_STRING( GetEntClassName() );
	// added by jason
	pev->framerate = 999.0f;
	UTIL_SetSize( pev, -GetSize(), GetSize() );
	SET_MODEL(ENT(pev), GetModel());
}


/*
* Explode on touch
*/
void EXPORT CBaseProjectile::ExplodeTouch( CBaseEntity *pOther )
{	// Only explode if you hit something that's not yourself, and the missle owner doesnt share the same team
	if(pOther != m_pOwner)
	{	if(pOther->IsPlayer())
		{	if(!g_pGameRules->FPlayerCanTakeDamage(m_pOwner, (CBasePlayer*)pOther))
			{	return;
			}
		}
		// Fellow missle, dont blow up when you collide with missles on the same team as the owner
		else if(pOther->Classify() == CLASS_MISSLE || pOther->Classify() == CLASS_ENERGY)
		{	// Projectile shares the same team with us
			CBasePlayer *pPlayer = ((CBasePlayer*)CBaseEntity::Instance(pOther->pev->owner));
			if(pPlayer)
			{	if(!g_pGameRules->FPlayerCanTakeDamage(pPlayer, m_pOwner))
				{	return;
				}
			}

			// One of our projectiles
			if(pOther->pev->owner == m_pOwner->edict())
			{	return;
			}
		}
		ExplodeProjectile();
	}
}

/*
* Emit Explosion FX
*/
void CBaseProjectile::EmitExplosionFX( void )
{	/*MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_EXPLOSION );		// This makes a dynamic light and the explosion sprites/sound
		WRITE_COORD( pev->origin.x );	// Send to PAS because of the sound
		WRITE_COORD( pev->origin.y );
		WRITE_COORD( pev->origin.z );
		WRITE_SHORT( g_sModelIndexFireball );
		WRITE_BYTE( (30) * .60  ); // scale * 10
		WRITE_BYTE( 15  ); // framerate
		WRITE_BYTE( TE_EXPLFLAG_NONE );
	MESSAGE_END();*/
	// added by jason
	if( !stricmp( GetEntClassName(), "wastedmissle" ) )
	{
		MESSAGE_BEGIN( MSG_ALL, gmsgPartMsg );
			WRITE_SHORT( entindex() );
			WRITE_SHORT( EFF_WASTEDEXP );
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
			WRITE_COORD( AF49_WASTEDMISSLE_RADIUS );
		MESSAGE_END( );
	}
	else if( !stricmp( GetEntClassName( ), "alphabomb" ) || !stricmp( GetEntClassName( ), "busterbig" )  )
	{
		MESSAGE_BEGIN( MSG_ALL, gmsgPartMsg );
			WRITE_SHORT( entindex() );
			WRITE_SHORT( EFF_ALPHABOMB );
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
			WRITE_COORD( VF1J_ALPHABOMB_RADIUS );
		MESSAGE_END( );
	}
	else if( !stricmp( GetEntClassName( ), "nuke" ) )
	{
		MESSAGE_BEGIN( MSG_ALL, gmsgPartMsg );
			WRITE_SHORT( entindex() );
			WRITE_SHORT( EFF_NUKE );
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
		MESSAGE_END( );
	}
	else
	{
		MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_EXPLOSION );		// This makes a dynamic light and the explosion sprites/sound
			WRITE_COORD( pev->origin.x );	// Send to PAS because of the sound
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
			WRITE_SHORT( g_sModelIndexFireball );
			WRITE_BYTE( (30) * .60  ); // scale * 10
			WRITE_BYTE( 15  ); // framerate
			WRITE_BYTE( TE_EXPLFLAG_NONE );
		MESSAGE_END();
	}
}

/*
* Explode
*/
void CBaseProjectile::ExplodeProjectile( void )
{	// Emit Damage
	::RadiusDamage( pev->origin, pev, m_pOwner->pev, GetDamage()*GetChargeAmt(), GetExplosionRadius()*GetChargeAmt(), Classify(), DMG_BLAST );

	// Emit explodion FX
	EmitExplosionFX();

	// Remove
	Remove();
}

/*
* Shoot down projectile with rapid fire
*/
void CBaseProjectile::ShootDown( void )
{	// Emit Damage
	::RadiusDamage( pev->origin, pev, m_pOwner->pev, GetDamage()*GetChargeAmt()*GetShootDownReducer(), GetExplosionRadius()*GetChargeAmt(), Classify(), DMG_BLAST );

	// Emit explodion FX
	EmitExplosionFX();

	// Remove
	Remove();
}


/*
* Remove Missle
*/
void CBaseProjectile::Remove( void )
{	// Projectile exploded
	m_fExploded = TRUE;

	// Added by jason
	RemoveTrail( );

	// Remove client renderer
	RemoveClientRenderer();

	// Remove entity
	UTIL_Remove(this);
}

/*
* ---------------
* Base Missle
* ---------------
*/

/*
* Precache
*/
void CBaseMissle::Precache( void )
// modifed by jason
{	PRECACHE_MODEL("models/w_missle.mdl");
	m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
}

/*
* Make Homing Missle
*/
void CBaseMissle::MakeHomingMissle( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner )
{	// Init
	Spawn();
	m_pOwner = pOwner;
	m_pLock = (CBaseEntity*)pLock->entindex();
	m_flSpeed = GetStartSpeed();
	pev->owner = pOwner->edict();

	// Set Origin
	UTIL_SetOrigin( pev, vecOrigin );
	// Set Velocity
	pev->velocity = vecDir.Normalize();
	vecVelocity = vecDir;
	// Set ROT
	m_flROT = GetROT();
	m_flROTDecrease = GetROTDecrease();
	m_flMaxDot = GetMaxDOT();
	// Reset unused varibles
	m_fExploded = FALSE;
	
	// Set touch/think
	SetTouch( &CBaseProjectile::ExplodeTouch );
	SetThink( &CBaseMissle::FollowThink );
	pev->nextthink = gpGlobals->time;

	// Add FX
	AddTrail();
	AddClientRenderer();

	// Add onto total player missles
	pOwner->m_iMissles++;
	// added by jason
	pev->framerate = 999.0f;
}

/*
* Make Missle
*/
void CBaseMissle::MakeMissle( Vector vecOrigin, Vector vecDir, CBasePlayer *pOwner )
{	// Init
	Spawn();
	m_pOwner = pOwner;
	m_pLock = NULL;
	m_flSpeed = GetStartSpeed();
	pev->owner = pOwner->edict();


	// Set Origin
	UTIL_SetOrigin( pev, vecOrigin );
	// Set Velocity
	pev->velocity = vecDir * GetStartSpeed();
	vecVelocity = vecDir;
	// Set Angles
	pev->angles = UTIL_VecToAngles(vecDir);
	// Set ROT
	m_flROT = 0;
	m_flROTDecrease = 0;
	m_flMaxDot = 0;
	// Reset unused varibles
	m_fExploded = FALSE;
	
	// Set touch/think
	SetTouch( &CBaseProjectile::ExplodeTouch );
	SetThink( NULL );

	// Add FX
	AddTrail();
	AddClientRenderer();

	// Add onto total player missles
	pOwner->m_iMissles++;
}


/*
* Follow think
*/
void EXPORT CBaseMissle::FollowThink( void )
{	CBasePlayer *pLock = NULL;
	if(m_pLock)
	{	pLock = ((CBasePlayer*)m_pLock);
	}
	// No target to lock on to
	if(!pLock)
	{	SetThink( NULL );
		return;
	}

	// Check if locked target is still alive / connected
	if(!pLock->IsAlive() || pLock->pev->solid == SOLID_NOT)
	{	SetThink( NULL );
		return;
	}

	// Get velocity from missle to lockon target
	Vector vecVel = (pLock->pev->origin - pev->origin);
	vecVel = vecVel.Normalize();

	// Get current velocity
	Vector vecRealNorm = vecVelocity.Normalize();

	// Use decreased rate of turn if the following entity is out of the missles FOV
	float flROT=m_flROT;
	if (DotProduct (vecVel , vecRealNorm) > m_flMaxDot )
	{	flROT = m_flROTDecrease;
	}

	// Using the ROT, get the new velocity
	vecVelocity = (vecRealNorm * (1-flROT) + vecVel * flROT);

	// Set velocity and angles
	pev->velocity = vecVelocity * m_flSpeed;
	pev->angles = UTIL_VecToAngles(vecVelocity);

	// Next think...
	pev->nextthink = gpGlobals->time + 0.1;
}

/*
* Add missle trail
*/
void CBaseMissle::AddTrail( void )
{
	// modified by jason
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( entindex( ) );	// entity
		WRITE_BYTE( 1 );			// living status
		WRITE_SHORT( kRenderTransAdd );	// rendermode
		WRITE_COORD( 5 );				// start size
		WRITE_COORD( 5 );				// end size
		WRITE_COORD( 80 );				// # of links
		WRITE_COORD( 30 );				// link length
		WRITE_BYTE( 255 );				// color r
		WRITE_BYTE( 255 );				// color g
		WRITE_BYTE( 255 );				// color b
		WRITE_STRING( "sprites/smoke.spr" );	// sprite name
		WRITE_SHORT( TRAIL_ORIGIN );	// type of trail
		// if TRAIL_BONE 
		//WRITE_STRING( "bonename" );	// name of bone
	MESSAGE_END( );
}

/*
* Remove Trail
*/
void CBaseMissle::RemoveTrail( void )
{
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( entindex( ) );	// entity
		WRITE_BYTE( 0 );			// living status
		WRITE_SHORT( TRAIL_ORIGIN );// type of trail to remove
		// if TRAIL_BONE 
		//WRITE_STRING( "bonename" );	// name of bone
	MESSAGE_END( );
}

/*
* Remove Missle
*/
void CBaseMissle::Remove( void )
{	// Reduce amount of missles
	m_pOwner->m_iMissles--;
	if(m_pOwner->m_iMissles<0)
		m_pOwner->m_iMissles=0;

	// Call regular remove function
	CBaseProjectile::Remove();
}

/*
* ---------------
* Generic Missle
* ---------------
*/

LINK_ENTITY_TO_CLASS( genericmissle, CGenericMissle );

/*
* Create Homing Missle
*/
CGenericMissle *CGenericMissle::CreateHomingMissle( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner )
{	CGenericMissle *pMissle = GetClassPtr((CGenericMissle *)NULL);
	pMissle->MakeHomingMissle( vecOrigin, vecDir, pLock, pOwner );

	return pMissle;
}

/*
* ---------------
* Wasted Missle
* ---------------
*/

LINK_ENTITY_TO_CLASS( wastedmissle, CWastedMissle );
/*
* Precache
*/
void CWastedMissle::Precache( void )
{	CBaseMissle::Precache();
	m_iTrail = PRECACHE_MODEL("sprites/green_smoke.spr");
	PRECACHE_MODEL( "models/w_wastedmissle.mdl" );
}

/*
* Create Missle
*/
CWastedMissle *CWastedMissle::CreateMissle( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner )
{	CWastedMissle *pMissle = GetClassPtr((CWastedMissle *)NULL);
	pMissle->MakeHomingMissle( vecOrigin, vecDir, pLock, pOwner );

	return pMissle;
}


/*
* Add missle trail
*/
void CWastedMissle::AddTrail( void )
{	// Added by jason
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( entindex( ) );	// entity
		WRITE_BYTE( 1 );			// living status
		WRITE_SHORT( kRenderTransAdd );	// rendermode
		WRITE_COORD( 8 );				// start size
		WRITE_COORD( 40 );				// end size
		WRITE_COORD( 80 );				// # of links
		WRITE_COORD( 30 );				// link length
		WRITE_BYTE( 255 );				// color r
		WRITE_BYTE( 255 );				// color g
		WRITE_BYTE( 255 );				// color b
		WRITE_STRING( "sprites/green_smoke.spr" );	// sprite name
		WRITE_SHORT( TRAIL_ORIGIN );	// type of trail
	MESSAGE_END( );
}


/*
* ---------------
* Freedom Blast
* ---------------
*/
LINK_ENTITY_TO_CLASS( freedomblast, CFreedomBlast );

/*
* Create Homing Missle
*/
CFreedomBlast *CFreedomBlast::CreateCFreedomBlast( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner )
{	CFreedomBlast *pBlast = GetClassPtr((CFreedomBlast *)NULL);
	pBlast->MakeHomingMissle( vecOrigin, vecDir, pLock, pOwner );
	pBlast->SetThink( &CFreedomBlast::FollowThink );
	return pBlast;
}

/*
* Remove Trail
*/
void CFreedomBlast::RemoveTrail( void )
{
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( entindex( ) );	// entity
		WRITE_BYTE( 0 );			// living status
		WRITE_SHORT( TRAIL_BEAM );// type of trail to remove
	MESSAGE_END( );
}

/*
* Follow think
*/
void EXPORT CFreedomBlast::FollowThink( void )
{	CBasePlayer *pLock = NULL;
	if(m_pLock)
	{	pLock = ((CBasePlayer*)m_pLock);
	}
	// No target to lock on to
	if(!pLock)
	{	SetThink( NULL );
		return;
	}

	// Check if locked target is still alive / connected
	if(!pLock->IsAlive() || pLock->pev->solid == SOLID_NOT)
	{	SetThink( NULL );
		return;
	}

	// Get velocity from missle to lockon target
	Vector vecVel = (pLock->pev->origin - pev->origin);
	vecVel = vecVel.Normalize();

	// Get current velocity
	Vector vecRealNorm = vecVelocity.Normalize();

	// Use decreased rate of turn if the following entity is out of the missles FOV
	float flROT=m_flROT;
	float flDot = DotProduct (vecVel , vecRealNorm);

	// Check for max dot
	if (flDot < m_flMaxDot )
	{	flROT = m_flROTDecrease;
	}

	// Using the ROT, get the new velocity
	vecVelocity = (vecRealNorm * (1-flROT) + vecVel * flROT);

	// Set velocity and angles
	pev->velocity = vecVelocity * m_flSpeed;
	pev->angles = UTIL_VecToAngles(vecVelocity);

	// Check dotproduct to see if we have a good lock. If so, stop trying to bend
	if(flDot > FREEDOM_5WAY_DOTCUT)
	{	SetThink(NULL);
		return;
	}

	// Next think...
	pev->nextthink = gpGlobals->time + 0.1;

}

/*
* -----------------------
* Base Energy Projectile
* -----------------------

/*
* Make Energy Projectile
*/
void CBaseEnergyProjectile::MakeEnergyProjectile( Vector vecOrigin, Vector vecDir, CBasePlayer *pOwner, float flCharged )
{	// Init
	Spawn();
	m_pOwner = pOwner;
	m_flSpeed = GetStartSpeed();
	pev->owner = pOwner->edict();

	// Set Origin
	UTIL_SetOrigin( pev, vecOrigin );
	// Set Velocity
	pev->velocity = vecDir * GetStartSpeed();
	vecVelocity = vecDir;
	// Set Angles
	pev->angles = UTIL_VecToAngles(vecDir);
	// Reset unused varibles
	m_fExploded = FALSE;
	// Set Charged amount
	m_flChargedPercent = flCharged;
	
	// Set touch/think
	SetTouch( &CBaseProjectile::ExplodeTouch );
	SetThink( NULL );

	// Add FX
	AddTrail();
	AddClientRenderer();
}

/*
* -----------------------
* VF1J Alphabomb
* -----------------------
*/
LINK_ENTITY_TO_CLASS( alphabomb, CAlphaBomb );

/*
* Precache
*/
void CAlphaBomb::Precache( void )
{	PRECACHE_MODEL( "models/w_alphabomb.mdl" );
}


/*
* Create Projectile (Alphabomb)
*/
CAlphaBomb *CAlphaBomb::CreateAlphaBomb( Vector vecOrigin, Vector vecDir,  CBasePlayer *pOwner, float flCharged )
{	CAlphaBomb *pProjectile = GetClassPtr((CAlphaBomb *)NULL);
	pProjectile->MakeEnergyProjectile( vecOrigin, vecDir, pOwner, flCharged );

	// added by jason
	if( pProjectile )
	{
	//	ALERT( at_console, "Charge: %f\n", flCharged );
		pProjectile->pev->frame = flCharged;
		pProjectile->pev->renderamt = 0;
	}

	return pProjectile;
}

void CAlphaBomb::RemoveTrail( void )
{
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( entindex( ) );	// entity
		WRITE_BYTE( 0 );			// living status
		WRITE_SHORT( TRAIL_BEAM );// type of trail to remove
	MESSAGE_END( );
}

/*
// added by jason
void CAlphaBomb::AlphaAnim( void )
{
	pev->nextthink = gpGlobals->time + float( 1/24 );

	pev->skin ++;

	if( pev->skin > 15 )
		pev->skin = 0;
}*/

// added by jason
LINK_ENTITY_TO_CLASS( nuke, CMKNuke );

/*
* Precache
*/
void CMKNuke::Precache( void )
{
	// AWSOME HOW THIS FUCKING DOESNT WORK
	PRECACHE_MODEL( "models/w_nuke.mdl" );
}

/*
* Create Projectile (Nuke)
*/
CMKNuke *CMKNuke::CreateNuke( Vector vecOrigin, Vector vecDir, CBasePlayer *pOwner, float flCharged )
{
	CMKNuke *pProjectile = GetClassPtr( ( CMKNuke * )NULL );
	pProjectile->MakeEnergyProjectile( vecOrigin, vecDir, pOwner, flCharged );

	if( pProjectile )
	{
		pProjectile->pev->frame = flCharged;
	}

	return pProjectile;
}

/*
* -----------------------
* Freedom buster big
* -----------------------
*/
LINK_ENTITY_TO_CLASS( busterbig, CBusterBig );

/*
* Precache
*/
void CBusterBig::Precache( void )
{	PRECACHE_MODEL( "models/w_alphabomb.mdl" );
}


/*
* Create Projectile (Alphabomb)
*/
CBusterBig *CBusterBig::CreateBusterBig( Vector vecOrigin, Vector vecDir,  CBasePlayer *pOwner, float flCharged )
{	CBusterBig *pProjectile = GetClassPtr((CBusterBig *)NULL);
	pProjectile->MakeEnergyProjectile( vecOrigin, vecDir, pOwner, flCharged );

	// added by jason
	if( pProjectile )
	{
	//	ALERT( at_console, "Charge: %f\n", flCharged );
		pProjectile->pev->frame = flCharged;
		pProjectile->pev->renderamt = 0;
	}

	return pProjectile;
}

void CBusterBig::RemoveTrail( void )
{
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( entindex( ) );	// entity
		WRITE_BYTE( 0 );			// living status
		WRITE_SHORT( TRAIL_BEAM );// type of trail to remove
	MESSAGE_END( );
}

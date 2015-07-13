/*
* Freedom 5 way attack
* @version: 6/29/04
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
#include "../cl_dll/ent_trail_def.h"

#ifndef CLIENT_DLL
extern int gmsgEntTrail;
#endif

LINK_ENTITY_TO_CLASS( weapon_freedom5way, CFreedom_5Way );

/*
* Freedom Blast Class
*/

/*
* Constructor for CFreedomCannons
*/
CFreedomCannons::CFreedomCannons(CBasePlayer *pOwner)
{	m_pOwner = pOwner;
	SetStats(std::string(""),0,0,Vector(0,0,0));
}

/*
* Set Stats
*/
void CFreedomCannons::SetStats( std::string &strSprite, int iSize, int iDamage, Vector vecOffset )
{	m_strSprite = strSprite.c_str();
	m_iSize = iSize;
	m_iDamage = iDamage;
	m_vecOffset = vecOffset;
}

/*
* Add special effects ( trails, head gfx )
*/
void CFreedomCannons::AddEffects( CFreedomBlast *pBlast )
{	
#ifndef CLIENT_DLL
	// Generate trail
	MESSAGE_BEGIN( MSG_ALL, gmsgEntTrail );
		WRITE_SHORT( pBlast->entindex( ) );	// entity
		WRITE_BYTE( 1 );			// living status
		WRITE_SHORT( kRenderTransAdd );	// rendermode
		WRITE_COORD( m_iSize );				// start size
		WRITE_COORD( m_iSize );				// end size
		WRITE_COORD( 80 );				// # of links
		WRITE_COORD( 55 );				// link length
		WRITE_BYTE( 255 );				// color r
		WRITE_BYTE( 255 );				// color g
		WRITE_BYTE( 255 );				// color b
		WRITE_STRING( m_strSprite.c_str() );	// sprite name
		WRITE_SHORT( TRAIL_BEAM );	// type of trail
	MESSAGE_END( );
#endif
}

/*
* Fire Attack
*/
void CFreedomCannons::Fire( CBasePlayer *pPlayer )
{	
#ifndef CLIENT_DLL
	if(!pPlayer)
	{	return;
	}
	UTIL_MakeVectors(m_pOwner->pev->v_angle);

	// Get real offset using view angles
	Vector vecRealOffset = m_pOwner->pev->origin + ((gpGlobals->v_forward * m_vecOffset.y) + (gpGlobals->v_right * m_vecOffset.x) + (gpGlobals->v_up * m_vecOffset.z));

	// Shoot beam and add special effects
	CFreedomBlast *pBlast = CFreedomBlast::CreateCFreedomBlast( vecRealOffset, gpGlobals->v_forward, pPlayer, m_pOwner );

	// Set Damage
	pBlast->SetDamage( m_iDamage );
	// Add Effects
	AddEffects( pBlast );
#endif
}

/*
* Custom Spawn
*/
void CFreedom_5Way::CustomSpawn( void )
{	SetClassname( "weapon_freedom5way" );
	SetAnimPrefix( "5wayattack" ); // added by grOOvy: 28/10/04
	SetP_model( "models/p_freedom_gun.mdl" ); // added by grOOvy: 02/11/04
	ResetCharge();
}

/*
* Deploy weapon
*/
BOOL CFreedom_5Way::Deploy( )
{	
	CustomSpawn( );
	ResetCharge();
	UpdateClient();
	m_iFireNum = 0;
#ifndef CLIENT_DLL
	// Reset anims
	m_pPlayer->m_Anim.RemoveWepAnims();

	// Set external model
	if(CBaseLockOn::Deploy())
	{	m_pPlayer->EnabledAttachMdl( "models/cannon.mdl" );
		m_pPlayer->m_pAttachEnt->m_Anim.AddAnimation( TRUE, ANIMTYPE_FULLBODY, gpGlobals->time, 2, "open", "open_idle" );
	}
	else
	{	return FALSE;
	}

	// Clear freedom blast data
	m_FreedomBlast.clear();
	// Configure all 5 blast
	CFreedomCannons blast(m_pPlayer);
	// Blast 1
	blast.SetStats( std::string("sprites/busterbeam.spr"), 10, FREEDOM_5WAY_YG_DAMAGE, Vector(12,0,-13) );
	m_FreedomBlast.push_back(blast);
	// Blast 2
	blast.SetStats( std::string("sprites/busterbeam.spr"), 10, FREEDOM_5WAY_YG_DAMAGE, Vector(-12,0,-13) );
	m_FreedomBlast.push_back(blast);
	// Blast 3
	blast.SetStats( std::string("sprites/blueredbeam.spr"), 14, FREEDOM_5WAY_BR_DAMAGE, Vector(-12,0,18) );
	m_FreedomBlast.push_back(blast);
	// Blast 5
	blast.SetStats( std::string("sprites/blueredbeam.spr"), 14, FREEDOM_5WAY_BR_DAMAGE, Vector(12, 0,18) );
	m_FreedomBlast.push_back(blast);
	// Blast 5
	blast.SetStats( std::string("sprites/greenbeam.spr"), 6, FREEDOM_5WAY_YG_DAMAGE, Vector(7.5,0,0) );
	m_FreedomBlast.push_back(blast);
#endif
	return TRUE;
}

/*
* Can Holster
*/
BOOL CFreedom_5Way::CanHolster( void )
{	return !m_pPlayer->m_fFreedomBlast;
}

/*
* Holster
*/
void CFreedom_5Way::Holster( int skiplocal  )
{	ResetCharge();
	UpdateClient();
#ifndef CLIENT_DLL
	// Reset anims
	m_pPlayer->m_Anim.RemoveWepAnims();
	// Disable external model
	m_pPlayer->DisableAttachMdl();
#endif
	// Reset freedom blast
	m_FreedomBlast.clear();
	// Lockon holster
	CBaseLockOn::Holster();
}

/*
* Precache
*/
void CFreedom_5Way::Precache( )
{	
#ifndef CLIENT_DLL
	PRECACHE_MODEL("models/cannon.mdl");
	PRECACHE_MODEL("sprites/busterbeam.spr");
	PRECACHE_MODEL("sprites/greenbeam.spr");
	PRECACHE_MODEL("sprites/blueredbeam.spr");
	PRECACHE_SOUND("weapons/5way_fire.wav");
#endif
}

/*
* Called to fire attack
*/
void CFreedom_5Way::FireAttack(int iIndex)
{	
#ifndef CLIENT_DLL
//	ALERT(at_console,"Fire 1!\n");
	// Make sure our firing setup isn't empty, or shot more blast then we can handle
	if(m_FreedomBlast.empty())
	{	return;
	}
	if(m_iFireNum>=(signed)m_FreedomBlast.size())
	{	return;
	}

	// Fire attack
	m_FreedomBlast[m_iFireNum].Fire( (CBasePlayer*)UTIL_PlayerByIndex(iIndex) );
	// Increment fire
	m_iFireNum++;
#endif
}

/*
* Start Charge
*/
void CFreedom_5Way::StartCharge( void )
{	
#ifndef CLIENT_DLL
	// Start adding charge
	AddCharge(ChargeRate()*gpGlobals->frametime);
	m_fCharging = TRUE;
	// Add weapon anim
	m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE2 );
#endif
}

BOOL CFreedom_5Way::CanFire( void )
{	return (m_flCurCharge > MinCharge() ? TRUE : FALSE);
}

/*
* Handle Charge Fire
*/
void CFreedom_5Way::HandleChargeFire( void )
{	
#ifndef CLIENT_DLL
	if(m_fCharging)
	{	// Fire attack and play anim
		if(CanFire())
		{	FireAttack(m_flCurCharge/MaxCharge());
			m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE_SHOOT2 );
		}
		// Reset weapon animations
		else
		{	m_pPlayer->m_Anim.RemoveWepAnims();
		}
		ResetCharge();
	}
#endif
}

/*
* Handle Charge
*/
BOOL CFreedom_5Way::HandleCharge( void )
{
#ifndef CLIENT_DLL
	// Start charging
	if(!m_pPlayer->m_fFreedomBlast)
	{
		if(!m_fCharging)
		{	// Start Charge
			StartCharge();

			// Update client
			UpdateClient();

			// Reset current lockons
			ClearLocks();
			ResetAttemptLockon();
		}
		// Continue charging
		else
		{	if(!ChargeMax())
			{	// Start adding charge
				AddCharge(ChargeRate()*gpGlobals->frametime);
			}

			// Add charge animation if we have no current animation (overwritten by boost/fall impact/etc...)
			if(m_pPlayer->m_Anim.GetCurrentAnim() == NULL)
			{	m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE2 );
			}
			
			// Update client
			UpdateClient();
		}
	}
#endif
	return m_fCharging;
}

/*
* Primary Attack
*/
void CFreedom_5Way::PrimaryAttack()
{	// Currently charging
	if(HandleCharge())
	{	// Check Locks
		CheckLocks();

#ifndef CLIENT_DLL
		if(IsAttemptingLockon())
		{	// Not ready to fully lockon yet
			if(!HandleAttemptLockon())
			{	return;
			}
			// Stop attempting to lock on and fully lock
			else
			{	AddLock(m_pAttemptLock->entindex());
				ResetAttemptLockon();
			}
		}

		// No more lockons after the charge is full
		if(ChargeMax() || (m_FreedomBlast.size() == GetAmtLocked()))
		{	return;
		}

		// Loop through all clients on server and save the ones within locking range
		int iAmount=0;
		float flLockDots[64];
		CBasePlayer *pLocks[64];

		for(int i=1;i<gpGlobals->maxClients;i++)
		{	CBasePlayer *pPlayer = ((CBasePlayer*)UTIL_PlayerByIndex(i));

			// Invalid lock, continue
			if(!CanLock(pPlayer))
			{	continue;
			}
			
			// Only allow one lock per entity
			bool blContinue = false;
			for(int n=0;n<MaxLocks();n++)
			{	if(m_LockData[n].blLocked)
				{	if(m_LockData[n].iIndex == i)
					{	blContinue = true;
						break;
					}
				}
			}
			if(blContinue)
				continue;

			float flDot=0;
			// Check if player is in lock range
			if(InInitLockRange(pPlayer->pev->origin, flDot))
			{	pLocks[iAmount] = pPlayer;
				flLockDots[iAmount] = flDot;
				iAmount++;
			}
		}

		// No locks
		if(!iAmount)
		{	return;
		}

		// Find closest dot with the least amount of locks
		int iLowestLock = MAX_LOCKS+1;
		float flMaxDot = -5;
		int iNum = 0;
		int iLockedAmt;
		for(i=0;i<iAmount;i++)
		{	IsLocked(pLocks[i]->entindex(), iLockedAmt);
			// Lower locked amount, auto target
			if(iLockedAmt < iLowestLock)
			{	iLowestLock = iLockedAmt;
				flMaxDot = flLockDots[i];
				iNum = i;
			}
			// Equal locked amount, check dots
			if(iLockedAmt == iLowestLock)
			{	if(flLockDots[i] > flMaxDot)
				{	iLowestLock = iLockedAmt;
					flMaxDot = flLockDots[i];
					iNum = i;
				}
			}
		}

		if(pLocks[iNum]->entindex() != 0)
		{	// Found target to lock onto, init lock
			AddAttemptLock(pLocks[iNum]);
		}
#endif
	}
}

/*
* Secondary Attack
*/
void CFreedom_5Way::SecondaryAttack( void )
{
	CBaseLockOn::SecondaryAttack( );
#ifndef CLIENT_DLL
	// Fire attack
	if(m_iFireNum > 0)
	{	// add anim
		m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE_SHOOT2 );
		
		// Set flying
		if(!FBitSet( pev->flags, FL_ONGROUND ))
		{		BOOL fNegFuel = m_pPlayer->m_fNegFuel;
			m_pPlayer->m_fNegFuel = FALSE;
			// added by jason
			m_pPlayer->m_iFlyMode = FLY_NORMAL;
			m_pPlayer->StartFly();
			// added by jason
			m_pPlayer->SendFlyMode( );
			m_pPlayer->m_fNegFuel = fNegFuel;
		}

		// stop player and add freedom blast flag
		m_pPlayer->m_fFreedomBlast = TRUE;
		m_pPlayer->m_flSlowDown = 0.0;
		m_pPlayer->pev->velocity = Vector(0,0,0);

		// Lock model
		UTIL_LockModelAngles( (CBaseEntity*)m_pPlayer, TRUE );

		m_iFireNum = 0;
	}
#endif
}

/*
* Weapon Idle
*/
void CFreedom_5Way::WeaponIdle( void )
{
#ifndef CLIENT_DLL
	// Remove prelocks
	DeletePrelock();
	// Fire attack
//	HandleChargeFire();
	if(m_fCharging)
	{	m_pPlayer->m_Anim.RemoveWepAnims();
		ResetCharge();
	}
#endif
}


/*
* In initial lock range
*/
BOOL CFreedom_5Way::InInitLockRange( Vector vecOrigin, float &flDot )
{	flDot = 0;
	// Player is too far away
	if((vecOrigin - m_pPlayer->pev->origin).Length() > MaxLockDistance())
	{	return FALSE;
	}

	vec3_t vecLOS = (vecOrigin - m_pPlayer->pev->origin);
	vecLOS = vecLOS.Normalize();
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);

	flDot = DotProduct (vecLOS , gpGlobals->v_forward);
	if (flDot > VIEW_FIELD_ULTRA_NARROW+0.035 )
		return TRUE;

	return FALSE;
}

/*
* Delete prelock
*/
void CFreedom_5Way::DeletePrelock( void )
{	// If we're attempting a lockon, remove it.
	if(IsAttemptingLockon())
	{	RemoveClientLock(true, m_pAttemptLock->entindex());
		ResetAttemptLockon();
	}
}

/*
* Update Client Data
*/
void CFreedom_5Way::UpdateClient( void )
{	if(m_fCharging)
	{	pev->fuser1 = GetPercentCharge()*100;
		// HACK HACK - force bar to go green when we have 1+ locks, red otherwise
		if(GetAmtLocked() > 0)
		{	pev->fuser2 = 0;
		}
		else
		{	pev->fuser2 = 101;
		}
	}
	else
	{	pev->fuser1 = 0;
		pev->fuser2 = 0;
	}
}

/*
* Add Charge
*/
void CFreedom_5Way::AddCharge( float flAmount )
{	m_flCurCharge += flAmount;
	if(m_flCurCharge > MaxCharge())
		m_flCurCharge = MaxCharge();
}

/*
* Reset Charge
*/
void CFreedom_5Way::ResetCharge( void )
{	m_flCurCharge = 0;
	m_fCharging = FALSE;
	UpdateClient();
}

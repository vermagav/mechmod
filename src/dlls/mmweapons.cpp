/*
* Mechmod base weapon system
* @version: 1/14/04
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

#ifndef CLIENT_DLL
extern int gmsgLockon;
#endif

#define LOCK_CLEAR		0
#define LOCK_REMOVE		1
#define LOCK_REMOVEALL	2
#define LOCK_ADD		3
#define PRELOCK_ADD		4
#define MISLOCK_ADD		5

/*
* Base weapons system
*/

/*
* Spawn
*/
void CMMWeapon::Spawn( )
{	Precache( );
	SET_MODEL(ENT(pev), "models/null.mdl");
	// Clear weapon values
	m_iPrimaryMax = 0;
	m_iSecondaryMax = 0;
	m_iPrimaryCur = 0;
	m_iSecondaryCur = 0;
	m_iWeight = GetWeight();
	SetID( GetID() );
	SetV_model("models/null.mdl");
	SetP_model("models/null.mdl");
	SetAnimPrefix("");

	CustomSpawn();	// Call weapon spawn
	FallInit();// get ready to fall down.
}

/*
* Add Weapon to player
*/
int CMMWeapon::AddToPlayer( CBasePlayer *pPlayer)
{
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{	MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

/*
* Use Decrement
*/
BOOL CMMWeapon::UseDecrement( void )
{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
}

/*
* Get information about the current weapon
*/
int CMMWeapon::GetItemInfo( ItemInfo *p )
{	p->pszName = STRING(pev->classname);
	p->iMaxAmmo1 = -1;
	p->iMaxAmmo2 = -1;

	// Set primary ammo
	if(UsePowerPrimary())
		p->pszAmmo1 = NULL;
	else
		p->pszAmmo1 = "NeverRunOutClips";

	// Set secondary ammo
	if(UsePowerSecondary())
		p->pszAmmo2 = NULL;
	else
		p->pszAmmo2 = "NeverRunOutClips";

	p->iMaxClip = WEAPON_NOCLIP;
	p->iId = m_iId = GetID();
	p->iWeight = GetWeight();
	p->iFlags = ITEM_FLAG_NOAUTOSWITCHEMPTY;
	p->iSlot = iItemSlot()-1;
	p->iPosition = iPosition();
	GetItemInfoCustom(p);
	return 1;
}

/*
* Deploy weapon
*/
BOOL CMMWeapon::Deploy( )
{	return DefaultDeploy( m_pV_model, m_pP_model, 0, m_pAnimPrefix );
}

/*
* Set view model
*/
void CMMWeapon::SetV_model( char *pModel )
{	m_pV_model = pModel;
}

/*
* Set player (weapon) model
*/
void CMMWeapon::SetP_model( char *pModel )
{	m_pP_model = pModel;
}

/*
* Set animation prefix
*/
void CMMWeapon::SetAnimPrefix( char *pPrefix )
{	m_pAnimPrefix = pPrefix;
}

/*
* Set classname
*/
void CMMWeapon::SetClassname( char *pName )
{	pev->classname = MAKE_STRING(pName);
}

/*
* Base Melee weapon
*/

/*
* Spawn
*/
void CBaseMelee::Spawn( )
{	CMMWeapon::Spawn();
}

#ifndef CLIENT_DLL
extern int gmsgSaber;
#endif

/*
* Deploy weapon
*/
BOOL CBaseMelee::Deploy( )
{	StartSaberFX();
	// Default Deploy
	return CMMWeapon::Deploy();
}

/*
* Holster
*/
void CBaseMelee::Holster( int skiplocal  )
{	StopSaberFX();
}

/*
* Start SaberFX
*/
void CBaseMelee::StartSaberFX( void )
{
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ALL, gmsgSaber, NULL );
		WRITE_SHORT(m_pPlayer->entindex());
		WRITE_BYTE(1);
	MESSAGE_END();
#endif
}

/*
* Stop SaberFX
*/
void CBaseMelee::StopSaberFX( void )
{
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ALL, gmsgSaber, NULL );
		WRITE_SHORT(m_pPlayer->entindex());
		WRITE_BYTE(0);
	MESSAGE_END();
#endif
}

/*
* Rapid Fire Weapons
*/

/*
* Spawn
*/
void CBaseRapidFireGun::Spawn( )
{
	//added by jason
	SetDamage( 0 );	
	
	// added by grOOvy
	SetBulletType ( BULLET_PLAYER_MP5 );

	CMMWeapon::Spawn();
	// Set weapon values
	m_iPrimaryMax = MaxRounds();
	m_iPrimaryCur = MaxRounds();
	m_iClip = MaxRounds();
}

/*
* Deploy weapon
*/
BOOL CBaseRapidFireGun::Deploy( )
{	// Clear lockons
	ClearLocks();
	ResetAttemptLockon();
	// Default Deploy
	return CMMWeapon::Deploy();
}

/*
* Holster
*/
void CBaseRapidFireGun::Holster( int skiplocal )
{	// Clear lockons
	ClearLocks();
	ResetAttemptLockon();
}

/*
* Get information about the current weapon
*/
int CBaseRapidFireGun::GetItemInfo( ItemInfo *p )
{	CMMWeapon::GetItemInfo(p);
	p->iMaxClip = MaxRounds();
	p->iMaxAmmo1 = MaxRounds();
	return 1;
}

/*
* Reload
*/
void CBaseRapidFireGun::Reload( void )
{	if(!m_fInReload)
		DefaultReload( ReloadTime() );
}

/*
* Default Reload
*/
BOOL CBaseRapidFireGun::DefaultReload( float flDelay )
{	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + flDelay;
	m_fInReload = TRUE;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
	return TRUE;
}

/*
* Primary Attack
*/
void CBaseRapidFireGun::PrimaryAttack()
{	
#ifndef CLIENT_DLL
// Reset attempting lockons
	if(IsAttemptingLockon())
	{	RemoveClientLock(m_pAttemptLock->entindex());
		ResetAttemptLockon();
	}
#endif
	if (m_iClip <= 0)
	{	Reload();
		return;
	}

#ifndef CLIENT_DLL
	// Check Locks
	CheckLocks();

	CBaseMissle *pMissle = GetBestLock();
	if(pMissle)
	{	pMissle->ShootDown();
	}
#endif
	
	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iPrimaryCur--;
	m_iClip = m_iPrimaryCur;

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	// Added by Pcjoe
#ifndef CLIENT_DLL
	m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_ATTACK1 );
#endif

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = GetAiming();
	Vector vecDir;

	vecDir = ShootBullet(vecSrc, vecAiming);

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = FEV_GLOBAL;
#endif
	flags = FEV_SERVER;

	EmitPrimaryTracerFX(flags, vecDir);

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + PrimaryFireRate();
	
	if ( m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + PrimaryFireRate();

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase();
}

/*
* Shoot Bullet
*/
Vector CBaseRapidFireGun::ShootBullet( Vector vecSrc, Vector vecAiming )
{
	// modifed by jason
	// modified by grOOvy: 12 July 2004
	return m_pPlayer->FireBulletsPlayer( 1, vecSrc, vecAiming, VECTOR_CONE_6DEGREES, 8192, m_iBulletType, 2, m_iDamage, m_pPlayer->pev, m_pPlayer->random_seed );
}

/*
* Secondary Attack
*/
void CBaseRapidFireGun::SecondaryAttack( void )
{	// Check Locks
	CheckLocks();

#ifndef CLIENT_DLL
	// Maxed out on locks, dont try more
	if(m_iCurrentLocks >= MaxLocks())
	{	// Reset attempting lockons
		if(IsAttemptingLockon())
		{	RemoveClientLock(m_pAttemptLock->entindex());
			ResetAttemptLockon();
		}
		return;
	}

	if(IsAttemptingLockon())
	{	// Not ready to fire yet
		if(!HandleAttemptLockon())
		{	return;
		}
		// Stop attempting to lock on and fully lock
		else
		{
			if( m_pAttemptLock && !FNullEnt( m_pAttemptLock->pev ) )
			{
				AddLock(m_pAttemptLock->entindex());
				ResetAttemptLockon();
			}
			else
			{
				return;
			}
		}
	}

	// Loop through all clients on server an save the ones within locking range
	const int maxsearchlock=64;
	int iAmount=0;
	float flLockDots[maxsearchlock];
	CBaseMissle *pLocks[maxsearchlock];

	CBaseEntity *pEntity = NULL;
	//UTIL_FindEntityInSphere(CBaseEntity *pStartEntity, const Vector &vecCenter, float flRadius);
	while ((pEntity = UTIL_FindEntityInSphere( pEntity, m_pPlayer->pev->origin, 8192 )) != NULL)
	{	// Entity must be classified as a missle
		if(pEntity->Classify() != CLASS_MISSLE)
		{	continue;
		}

		// Too many missles
		if(iAmount >= maxsearchlock)
		{	break;
		}

		// typecast to missle
		CBaseMissle *pMissle = (CBaseMissle*)pEntity;

		// is missle a valid ent
		if( !pMissle )
			continue;

		// Only one lock per missle
		if(IsLocked(pMissle->entindex()))
		{	continue;
		}

		// Missle is no longer solid, dont bother
		if(pMissle->pev->solid == SOLID_NOT)
		{	continue;
		}

		// Dont lock onto your own missles
		if(pMissle->pev->owner == m_pPlayer->edict())
		{	continue;
		}

		// Missle exploded, continue
		if(pMissle->m_fExploded)
		{	continue;
		}

		// This players's missles is on your team, cant target
		CBasePlayer *pPlayer = ((CBasePlayer*)CBaseEntity::Instance(pMissle->pev->owner));
		if(pPlayer)
		{	if(!g_pGameRules->FPlayerCanTakeDamage(pPlayer, m_pPlayer))
			{	continue;
			}
		}

		float flDot;
		// Check if missle is in lock range
		if(InLockRange(pMissle->pev->origin, flDot))
		{	pLocks[iAmount] = pMissle;
			flLockDots[iAmount] = flDot;
			iAmount++;
		}
	}

	// No locks
	if(!iAmount)
	{	return;
	}

	// Find closest dot with the least amount of locks
	float flLowestDot = -5;
	int iNum = 0;
	for(int i=0;i<iAmount;i++)
	{	if(flLockDots[i] > flLowestDot)
		{	flLowestDot = flLockDots[i];
			iNum = i;
		}
	}

	if(pLocks[iNum]->entindex() != 0)
	{	// Found target to lock onto, init lock
		AddAttemptLock(pLocks[iNum]);
	}
#endif
}

/*
* Weapon Idle
*/
void CBaseRapidFireGun::WeaponIdle( void )
{	CheckLocks();
#ifndef CLIENT_DLL
	// Reset attempting lockons
	if(IsAttemptingLockon())
	{	RemoveClientLock(m_pAttemptLock->entindex());
		ResetAttemptLockon();
	}
#endif
}


/*
* In lock range
*/
BOOL CBaseRapidFireGun::InLockRange( Vector vecOrigin, float &flDot )
{	flDot = 0;
	// Player is too far away
	if((vecOrigin - m_pPlayer->pev->origin).Length() > MaxLockDistance())
	{	return FALSE;
	}

	vec3_t vecLOS = (vecOrigin - m_pPlayer->pev->origin);
	vecLOS = vecLOS.Normalize();
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);

	flDot = DotProduct (vecLOS , gpGlobals->v_forward);
	if (flDot > VIEW_FIELD_NARROW )
		return TRUE;

	return FALSE;
}


/*
* Is Locked
*/
BOOL CBaseRapidFireGun::IsLocked( int iIndex )
{	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].blLocked && m_LockData[i].iIndex == iIndex)
		{	return TRUE;
		}
	}
	return FALSE;
}


/*
* Get Best Lock ( shooting missles down )
*/
CBaseMissle *CBaseRapidFireGun::GetBestLock( void )
{	
	float flMaxDot=-5;
	CBaseMissle *pClosest = NULL;
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
#ifndef CLIENT_DLL
	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].blLocked)
		{	if(m_LockData[i].iIndex <= 0)
			{	RemoveLock(m_LockData[i].iIndex);
			}
			CBaseMissle *pMissle = ((CBaseMissle*)UTIL_EntityByIndex(m_LockData[i].iIndex));
			// Invalid
			if(!pMissle)
			{	RemoveLock(m_LockData[i].iIndex);
				continue;
			}

			// Check if he's in our small view
			vec3_t vecLOS = (pMissle->pev->origin - m_pPlayer->pev->origin);
			vecLOS = vecLOS.Normalize();

			float flDot = DotProduct (vecLOS , gpGlobals->v_forward);
			if (flDot > 0.995 )
			{	if(flDot > flMaxDot)
				{	flMaxDot = flDot;
					pClosest = pMissle;
				}
			}
		}
	}
	// Ensure that we're not blowing missles up through the wall
	if(pClosest)
	{	TraceResult tr;
		UTIL_TraceLine( m_pPlayer->pev->origin, pClosest->pev->origin, ignore_monsters, ENT(m_pPlayer->pev), &tr );
		if(tr.flFraction < 1)
		{	pClosest = NULL;
		}
	}
#endif
	return pClosest;
}

/*
* Check Locks: Remove locks for locked-on dead players
*/
void CBaseRapidFireGun::CheckLocks( void )
{	// Nothing to check
	if(!m_iCurrentLocks)
	{	return;
	}

#ifndef CLIENT_DLL
	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].blLocked)
		{	if(m_LockData[i].iIndex <= 0)
			{	RemoveLock(m_LockData[i].iIndex);
			}
			CBaseMissle *pMissle = ((CBaseMissle*)UTIL_EntityByIndex(m_LockData[i].iIndex));
			// Invalid
			if(!pMissle)
			{	RemoveLock(m_LockData[i].iIndex);
				continue;
			}

			// Out of range
			float flDot;
			if(!InLockRange(pMissle->pev->origin, flDot))
			{	RemoveLock(m_LockData[i].iIndex);
			}

			// "Dead", unlock
			if(pMissle->m_fExploded)
			{	RemoveLock(m_LockData[i].iIndex);
				continue;
			}
			
			// Missle is no longer solid,unlock
			if(pMissle->pev->solid == SOLID_NOT)
			{	continue;
			}
		}
	}
#endif
}

/*
* Add Lock
*/
void CBaseRapidFireGun::AddLock( int iIndex )
{	// Dont go past your max locks
	if(m_iCurrentLocks == MaxLocks())
	{	return;
	}

	int iLock = -1;
	// Find free locked slot
	for(int i=0;i<MaxLocks();i++)
	{	if(!m_LockData[i].blLocked)
		{	iLock = i;
		}
	}

	// Locks already filled
	if(iLock == -1)
	{	return;
	}

	m_iCurrentLocks++;
	// Enter lock data
	m_LockData[iLock].blLocked = true;
	m_LockData[iLock].iIndex = iIndex;
	m_LockData[iLock].flTime = UTIL_WeaponTimeBase();

	// Add client lock
	AddClientLock(iIndex);
}

/*
* Remove Lock
*/
void CBaseRapidFireGun::RemoveLock( int iIndex )
{	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].iIndex == iIndex)
		{	m_iCurrentLocks--;
			// Reset Data
			m_LockData[i].blLocked = false;
			m_LockData[i].flTime = 0;
			m_LockData[i].iIndex = 0;
		}
	}
	RemoveClientLock(iIndex);
}

/*
* Clear Locks
*/
void CBaseRapidFireGun::ClearLocks( void )
{	for(int i=0;i<MAX_LOCKS;i++)
	{	m_LockData[i].blLocked = false;
		m_LockData[i].flTime = 0;
		m_LockData[i].iIndex = 0;
	}
	m_iCurrentLocks = 0;
	// Remove clientside
	ClearClientLocks();
}

/*
* Clear Client Locks
*/
void CBaseRapidFireGun::ClearClientLocks( void )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		WRITE_BYTE( LOCK_CLEAR );
		WRITE_BYTE( 0 );
	MESSAGE_END();
#endif
}


/*
* Add Client PreLock
*/
void CBaseRapidFireGun::AddClientPreLock( int iIndex )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		WRITE_BYTE( PRELOCK_ADD );
		WRITE_BYTE( iIndex );
	MESSAGE_END();
#endif
}

/*
* Add Client Lock
*/
void CBaseRapidFireGun::AddClientLock( int iIndex )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		WRITE_BYTE( MISLOCK_ADD );
		WRITE_BYTE( iIndex );
	MESSAGE_END();
#endif
}

/*
* Remove Client Lock
*/
void CBaseRapidFireGun::RemoveClientLock( int iIndex )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		WRITE_BYTE( LOCK_REMOVE );
		WRITE_BYTE( iIndex );
	MESSAGE_END();
#endif
}


/*
* Is attempting to lock onto an entity
*/
BOOL CBaseRapidFireGun::IsAttemptingLockon( void )
{	if(m_pAttemptLock)
		return TRUE;
	return FALSE;
}

/*
* Add attempt lock
*/
void CBaseRapidFireGun::AddAttemptLock( CBaseMissle *pMissle )
{	m_pAttemptLock = pMissle;
	m_flAttemptLockTime = gpGlobals->time;
	// Only add client gfx if we dont have a "real" lock to the player
	bool blExist = false;
	for(int n=0;n<MaxLocks();n++)
	{	if(m_LockData[n].iIndex == m_pAttemptLock->entindex())
		{	blExist = true;
		}
	}
	if(!blExist)
	{	AddClientPreLock(pMissle->entindex());
	}
	// Reset prelock
	else
	{	ResetAttemptLockon();
	}
}

/*
* Handle attempt lockon
*/
BOOL CBaseRapidFireGun::HandleAttemptLockon( void )
{	// Ready to fully lock
	if((m_flAttemptLockTime + LockDelay()) < gpGlobals->time)
	{	return TRUE;
	}
	return FALSE;
}

/*
* Reset attempt lockon
*/
void CBaseRapidFireGun::ResetAttemptLockon( void )
{	m_pAttemptLock = NULL;
	m_flAttemptLockTime = 0;
}

/*
* ========================
* Charge Weapons
* ========================
*/

/*
* Spawn
*/
void CBaseChargeGun::Spawn( )
{	CMMWeapon::Spawn();
	ResetCharge();
}

/*
* Deploy weapon
*/
BOOL CBaseChargeGun::Deploy( )
{	ResetCharge();
	UpdateClient();
#ifndef CLIENT_DLL
	m_pPlayer->m_Anim.RemoveWepAnims();
#endif
	// Default Deploy
	return CMMWeapon::Deploy();
}

/*
* Holster
*/
void CBaseChargeGun::Holster( int skiplocal )
{	ResetCharge();
	UpdateClient();
#ifndef CLIENT_DLL
	m_pPlayer->m_Anim.RemoveWepAnims();
#endif
}

/*
* Start Charge
*/
void CBaseChargeGun::StartCharge( void )
{	
#ifndef CLIENT_DLL
	// Start adding charge
	AddCharge(ChargeRate()*gpGlobals->frametime);
	m_fCharging = TRUE;
	// Add weapon anim
	m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE2 );
	// Added by jason
	StartChargeFunc( );
#endif
}

/*
* Handle Charge Fire
*/
void CBaseChargeGun::HandleChargeFire( void )
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
* Primary Attack
*/
void CBaseChargeGun::PrimaryAttack()
{	
#ifndef CLIENT_DLL
	// Start charging
	if(!m_fCharging)
	{	if(m_pPlayer->UsePower(UsePowerRate()*gpGlobals->frametime))
		{	// Start Charge
			StartCharge();
		}

		// Update client
		UpdateClient();
	}
	// Continue charging
	else
	{	if(ChargeMax() ? TRUE : m_pPlayer->UsePower(UsePowerRate()*gpGlobals->frametime))
		{	// Start adding charge
			AddCharge(ChargeRate()*gpGlobals->frametime);

			// Add charge animation if we have no current animation (overwritten by boost/fall impact/etc...)
			if(m_pPlayer->m_Anim.GetCurrentAnim() == NULL)
			{	m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE2 );
			}
		}
		// Fire Attack
		else
		{	HandleChargeFire();
		}
		
		// Update client
		UpdateClient();
	}
#endif
}

/*
* Secondary Attack
*/
void CBaseChargeGun::SecondaryAttack( void )
{
#ifndef CLIENT_DLL
	// Fire attack
	HandleChargeFire();
#endif
}

/*
* Weapon Idle
*/
void CBaseChargeGun::WeaponIdle( void )
{
#ifndef CLIENT_DLL
	// Fire attack
	HandleChargeFire();
#endif
}

/*
* Update Client Data
*/
void CBaseChargeGun::UpdateClient( void )
{	if(m_fCharging)
	{	pev->fuser1 = GetPercentCharge()*100;
		pev->fuser2 = (MinCharge()/MaxCharge())*100;
	}
	else
	{	pev->fuser1 = 0;
		pev->fuser2 = 0;
	}
}

/*
* Add Charge
*/
void CBaseChargeGun::AddCharge( float flAmount )
{	m_flCurCharge += flAmount;
	if(m_flCurCharge > MaxCharge())
		m_flCurCharge = MaxCharge();
}

/*
* Reset Charge
*/
void CBaseChargeGun::ResetCharge( void )
{	m_flCurCharge = 0;
	m_fCharging = FALSE;
	UpdateClient();
	// Added by jason
	EndChargeFunc( );
}

/*
* ========================
* Lockon Weapons
* ========================
*/

/*
* Spawn
*/
void CBaseLockOn::Spawn( )
{	CMMWeapon::Spawn();
	m_pAttemptLock = NULL;
	m_flAttemptLockTime = 0;
}

/*
* Deploy weapon
*/
BOOL CBaseLockOn::Deploy( )
{	// Clear lockons
	ClearLocks();
	ResetAttemptLockon();
	#ifndef CLIENT_DLL // added by grOOvy: 02/11/04
		m_pPlayer->m_Anim.RemoveWepAnims();
	#endif
	// Default Deploy
	return CMMWeapon::Deploy();
}

/*
* Holster
*/
void CBaseLockOn::Holster( int skiplocal )
{	// Clear lockons
	ClearLocks();
	ResetAttemptLockon();
}

/*
* Primary Attack
*/
void CBaseLockOn::PrimaryAttack( )
{	// Check Locks
	CheckLocks();

#ifndef CLIENT_DLL

	m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE2 ); // added by grOOvy: 30/10/04
	if(IsAttemptingLockon())
	{	// Not ready to fire yet
		if(!HandleAttemptLockon())
		{	return;
		}
		// Stop attempting to lock on and fully lock
		else
		{	AddLock(m_pAttemptLock->entindex());
			ResetAttemptLockon();
		}
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
		
		// Only allow locking onto a single entity
		bool blContinue = false;
		if(!MultiLock())
		{	for(int n=0;n<MaxLocks();n++)
			{	if(m_LockData[n].blLocked)
				{	if(m_LockData[n].iIndex != i)
					{	blContinue = true;;
					}
				}
			}
		}
		if(blContinue)
			continue;

		float flDot;
		// Check if player is in lock range
		if(InLockRange(pPlayer->pev->origin, flDot))
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

/*
* Secondary Attack
*/
void CBaseLockOn::SecondaryAttack( )
{	
/*	FireAttack(m_pPlayer->entindex());
	return;*/

	// Cant shoot missles w/o locks
	if(!m_iCurrentLocks)
	{	return;
	}

#ifndef CLIENT_DLL
	// Check Locks
	CheckLocks();

	// Reset attempting lockons
	if(IsAttemptingLockon())
	{	// Only reset client gfx if we dont have a "real" lock to the player
		int iGarbage;
		if(!IsLocked(m_pAttemptLock->entindex(), iGarbage))
		{	RemoveClientLock(true, m_pAttemptLock->entindex());
		}
		ResetAttemptLockon();
	}
	
	if(m_pPlayer->m_iMissles >= MAX_MISSLES)
	{	return;
	}

	// Real lock delay
	if(FireDelay())
	{	float flTime = -1;
		int iLowest = -1;
		// Find the "oldest" lock
		for(int i=0;i<MaxLocks();i++)
		{	if(m_LockData[i].blLocked && (flTime == -1 || m_LockData[i].flTime < flTime))
			{	flTime = m_LockData[i].flTime;
				iLowest = i;
			}
		}

		// No locks, restart all
		if(iLowest == -1)
		{	ClearLocks();
			return;
		}

		// Make sure we have the power to fire
		if(!m_pPlayer->UsePower(PowerCost()))
		{	return;
		}

		// Fire the oldest lock
		// Modified by grOOvy, 16/04/2004
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, Firesound().c_str(), 1, ATTN_NORM); // Emit sound
		
		m_pPlayer->m_Anim.AddWeaponAnimation( PLAY_CHARGE_SHOOT2 ); // added by grOOvy: 30/10/04
		FireAttack(m_LockData[iLowest].iIndex);
		RemoveLock(false, m_LockData[iLowest].iIndex);
		
		// Add delay
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + FireDelay();
	}
	// Shoot all instantly
	else
	{	// If we can shoot all the missles without going over the limit, use the Clearlocks method
		bool blClearLocks = true;
		if((m_pPlayer->m_iMissles + GetAmtLocked()) > MAX_MISSLES || !m_pPlayer->UsePower(GetAmtLocked()*PowerCost()))
		{	blClearLocks = false;
		}

		// Modified by grOOvy, 16/04/2004
		if(m_pPlayer->CanUsePower(PowerCost()))
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, Firesound().c_str(), 1, ATTN_NORM); // Emit sound

		for(int i=0;i<MaxLocks();i++)
		{	// Max Missles
			if(m_pPlayer->m_iMissles >= MAX_MISSLES)
				break;

			// Use Power
			if(!blClearLocks)
			{	if(!m_pPlayer->UsePower(PowerCost()))
					break;
			}
			
			// Fire Missles
			if(m_LockData[i].blLocked)
			{	FireAttack(m_LockData[i].iIndex);
				if(!blClearLocks)
					RemoveLock(false, m_LockData[i].iIndex);
			}
		}
		if(blClearLocks)
		{	ClearLocks();
		}
	}
#endif
}


/*
* Weapon Idle
*/
void CBaseLockOn::WeaponIdle( void )
{	CheckLocks();
#ifndef CLIENT_DLL
	// Reset attempting lockons
	if(IsAttemptingLockon())
	{	// Only reset client gfx if we dont have a "real" lock to the player
		int iGarbage;
		if(!IsLocked(m_pAttemptLock->entindex(), iGarbage))
		{	RemoveClientLock(true, m_pAttemptLock->entindex());
		}
		ResetAttemptLockon();
	}
#endif
}


/*
* Get Amount Locked
*/
int CBaseLockOn::GetAmtLocked( void )
{	int iAmt=0;
	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].blLocked)
			iAmt++;
	}
	return iAmt;
}

/*
* In lock range
*/
BOOL CBaseLockOn::InLockRange( Vector vecOrigin, float &flDot )
{	flDot = 0;
	// Player is too far away
	if((vecOrigin - m_pPlayer->pev->origin).Length() > MaxLockDistance())
	{	return FALSE;
	}

	vec3_t vecLOS = (vecOrigin - m_pPlayer->pev->origin);
	vecLOS.Normalize();
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);

	flDot = DotProduct (vecLOS , gpGlobals->v_forward);
	/*if (flDot > VIEW_FIELD_NARROW )
		return TRUE;*/
	//Modified by grOOvy: 29/10/04
	if ( flDot > ( VIEW_FIELD_ULTRA_NARROW + 0.035 ) )
		return TRUE;

	return FALSE;
}


/*
* Is Locked
*/
BOOL CBaseLockOn::IsLocked( int iIndex, int &iAmount )
{	iAmount = 0;
	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].blLocked && m_LockData[i].iIndex == iIndex)
		{	iAmount++;
		}
	}

	if(iAmount)
	{	return TRUE;
	}
	return FALSE;
}


/*
* Can Lock
*/
BOOL CBaseLockOn::CanLock( CBasePlayer *pLock )
{
#ifndef CLIENT_DLL
	// Not valid
	if(!pLock)
	{	return FALSE;
	}

	// null ent
	if(FNullEnt( pLock->pev ))
	{	return FALSE;
	}

	// Players only
	if(!pLock->IsPlayer())
	{	return FALSE;
	}

	// Dont target dead players
	if(!pLock->IsAlive())
	{	return FALSE;
	}

	// Cant target yourself
	if(pLock == m_pPlayer)
	{	return FALSE;
	}

	// Player is a disconnected client, dont bother
	if(pLock->pev->solid == SOLID_NOT)
	{	return FALSE;
	}

	// This person is on your team, cant target
	if(!g_pGameRules->FPlayerCanTakeDamage(pLock, m_pPlayer))
	{	return FALSE;
	}
	return TRUE;
#else
	return FALSE;
#endif
}

/*
* Check Locks: Remove locks for locked-on dead players
*/
void CBaseLockOn::CheckLocks( void )
{	// Nothing to check
	if(!m_iCurrentLocks)
	{	return;
	}

#ifndef CLIENT_DLL
	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].blLocked)
		{	CBasePlayer *pPlayer = ((CBasePlayer*)UTIL_PlayerByIndex( m_LockData[i].iIndex ));
			// Invalid lock, remove
			if(!CanLock(pPlayer))
			{	RemoveLock(true, m_LockData[i].iIndex);
				continue;
			}

			// Out of range
			float flDot;
			if(!InLockRange(pPlayer->pev->origin, flDot))
			{	RemoveLock(true, m_LockData[i].iIndex);
				continue;
			}
		}
	}
#endif
}

/*
* Add Lock
*/
void CBaseLockOn::AddLock( int iIndex )
{	// Dont go past your max locks
	if(m_iCurrentLocks == MaxLocks())
	{	return;
	}

	int iLock = -1;
	// Find free locked slot
	for(int i=0;i<MaxLocks();i++)
	{	if(!m_LockData[i].blLocked)
		{	iLock = i;
		}
	}

	// Locks already filled
	if(iLock == -1)
	{	return;
	}

	m_iCurrentLocks++;
	// Enter lock data
	m_LockData[iLock].blLocked = true;
	m_LockData[iLock].iIndex = iIndex;
	m_LockData[iLock].flTime = UTIL_WeaponTimeBase();

	// Add client lock
	AddClientLock(iIndex);
//	ALERT(at_console,"Add Lock - %i\n", iIndex);
}

/*
* Remove Lock
*/
void CBaseLockOn::RemoveLock( bool blAll, int iIndex )
{	for(int i=0;i<MaxLocks();i++)
	{	if(m_LockData[i].iIndex == iIndex)
		{	m_iCurrentLocks--;
			// Reset Data
			m_LockData[i].blLocked = false;
			m_LockData[i].flTime = 0;
			m_LockData[i].iIndex = 0;
			// Remove Clientside
			if(!blAll)
			{	RemoveClientLock(false, iIndex);
				break;
			}
		}
	}
	if(blAll)
	{	RemoveClientLock(true, iIndex);
	}
//	ALERT(at_console,"Remove Lock\n");
}

/*
* Clear Locks
*/
void CBaseLockOn::ClearLocks( void )
{	for(int i=0;i<MAX_LOCKS;i++)
	{	m_LockData[i].blLocked = false;
		m_LockData[i].flTime = 0;
		m_LockData[i].iIndex = 0;
	}
	m_iCurrentLocks = 0;
	// Remove clientside
	ClearClientLocks();
}

/*
* Clear Client Locks
*/
void CBaseLockOn::ClearClientLocks( void )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		WRITE_BYTE( LOCK_CLEAR );
		WRITE_BYTE( 0 );
	MESSAGE_END();
#endif
}


/*
* Add Client PreLock
*/
void CBaseLockOn::AddClientPreLock( int iIndex )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		WRITE_BYTE( PRELOCK_ADD );
		WRITE_BYTE( iIndex );
	MESSAGE_END();
#endif
}

/*
* Add Client Lock
*/
void CBaseLockOn::AddClientLock( int iIndex )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		WRITE_BYTE( LOCK_ADD );
		WRITE_BYTE( iIndex );
	MESSAGE_END();
#endif
}

/*
* Remove Client Lock
*/
void CBaseLockOn::RemoveClientLock( bool blAll, int iIndex )
{	
#ifndef CLIENT_DLL
	MESSAGE_BEGIN( MSG_ONE, gmsgLockon, NULL, m_pPlayer->pev );
		if(blAll)
			WRITE_BYTE( LOCK_REMOVEALL );
		else
			WRITE_BYTE( LOCK_REMOVE );
		WRITE_BYTE( iIndex );
	MESSAGE_END();
#endif
}


/*
* Is attempting to lock onto an entity
*/
BOOL CBaseLockOn::IsAttemptingLockon( void )
{	if(m_pAttemptLock)
		return TRUE;
	return FALSE;
}


/*
* Add attempt lock
*/
void CBaseLockOn::AddAttemptLock( CBasePlayer *pPlayer )
{	m_pAttemptLock = pPlayer;
	m_flAttemptLockTime = gpGlobals->time;
	// Only add client gfx if we dont have a "real" lock to the player
	bool blExist = false;
	for(int n=0;n<MaxLocks();n++)
	{	if(m_LockData[n].iIndex == m_pAttemptLock->entindex())
		{	blExist = true;
		}
	}
	if(!blExist)
	{	AddClientPreLock(pPlayer->entindex());
	}
}

/*
* Handle attempt lockon
*/
BOOL CBaseLockOn::HandleAttemptLockon( void )
{	// Ready to fully lock
	if((m_flAttemptLockTime + LockDelay()) < gpGlobals->time)
	{	return TRUE;
	}
	return FALSE;
}

/*
* Reset attempt lockon
*/
void CBaseLockOn::ResetAttemptLockon( void )
{	m_pAttemptLock = NULL;
	m_flAttemptLockTime = 0;
}

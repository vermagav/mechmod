/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef PLAYER_H
#define PLAYER_H


#include "pm_materials.h"
// Added by Pcjoe
// STL
#pragma warning (disable:4530)
#pragma warning (disable:4786)
#include <list>
#include <vector>
#include <map>
#include <string>
#define ForEachIn( a, b ) for( a = b.begin(); a != b.end(); a++ )

#include "classes.h"
#include "entanimation.h"
#include "playeranimation.h"
#include "attachent.h"
#include "boost.h"
#include "mm_sabercombat.h"


// added by jason
using namespace std;

// Edited by Pcjoe
/*
#define PLAYER_FATAL_FALL_SPEED		1024// approx 60 feet
#define PLAYER_MAX_SAFE_FALL_SPEED	580// approx 20 feet
#define PLAYER_FALL_PUNCH_THRESHHOLD (float)350 // won't punch player's screen/make scrape noise unless player falling at least this fast.
*/
#define PLAYER_FATAL_FALL_SPEED		10240
#define PLAYER_MAX_SAFE_FALL_SPEED	5120
#define PLAYER_FALL_PUNCH_THRESHHOLD (float)350 // won't punch player's screen/make scrape noise unless player falling at least this fast.
#define DAMAGE_FOR_FALL_SPEED		(float) 100 / ( PLAYER_FATAL_FALL_SPEED - PLAYER_MAX_SAFE_FALL_SPEED )// damage per unit per second.
#define PLAYER_MIN_BOUNCE_SPEED		200

//
// Player PHYSICS FLAGS bits
//
#define		PFLAG_ONLADDER		( 1<<0 )
#define		PFLAG_ONSWING		( 1<<0 )
#define		PFLAG_ONTRAIN		( 1<<1 )
#define		PFLAG_ONBARNACLE	( 1<<2 )
#define		PFLAG_DUCKING		( 1<<3 )		// In the process of ducking, but totally squatted yet
#define		PFLAG_USING			( 1<<4 )		// Using a continuous entity
#define		PFLAG_OBSERVER		( 1<<5 )		// player is locked in stationary cam mode. Spectators can move, observers can't.

//
// generic player
//
//-----------------------------------------------------
//This is Half-Life player entity
//-----------------------------------------------------
#define CSUITPLAYLIST	4		// max of 4 suit sentences queued up at any time

#define SUIT_GROUP			TRUE
#define	SUIT_SENTENCE		FALSE

#define	SUIT_REPEAT_OK		0
#define SUIT_NEXT_IN_30SEC	30
#define SUIT_NEXT_IN_1MIN	60
#define SUIT_NEXT_IN_5MIN	300
#define SUIT_NEXT_IN_10MIN	600
#define SUIT_NEXT_IN_30MIN	1800
#define SUIT_NEXT_IN_1HOUR	3600

#define CSUITNOREPEAT		32

#define	SOUND_FLASHLIGHT_ON		"items/flashlight1.wav"
#define	SOUND_FLASHLIGHT_OFF	"items/flashlight1.wav"

#define TEAM_NAME_LENGTH	16

// Added by Pcjoe
#define MAX_FUEL	1000
#define MAX_POWER	1000

typedef enum
{
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,
} PLAYER_ANIM;

#define MAX_ID_RANGE 2048
#define SBAR_STRING_SIZE 128

enum sbar_data
{
	SBAR_ID_TARGETNAME = 1,
	SBAR_ID_TARGETHEALTH,
	SBAR_ID_TARGETARMOR,
	SBAR_END,
};

// Added by Pcjoe
typedef enum
{	FLY_NONE=0,
	FLY_NORMAL,
	FLY_FAST,
	FLY_BOOST,
} FLY_MODE;

typedef enum
{	BOOST_NONE=0,
	BOOST_FRONT,
	BOOST_BACK,
	BOOST_LEFT,
	BOOST_RIGHT,
	BOOST_UP,
	BOOST_DOWN,
}BOOST_DIRECTIONS;

#define CHAT_INTERVAL 1.0f

class CBasePlayer : public CBaseMonster
{
public:
	int					random_seed;    // See that is shared between client & server for shared weapons code
	int					m_iPlayerSound;// the index of the sound list slot reserved for this player
	int					m_iTargetVolume;// ideal sound volume. 
	int					m_iWeaponVolume;// how loud the player's weapon is right now.
	int					m_iExtraSoundTypes;// additional classification for this weapon's sound
	int					m_iWeaponFlash;// brightness of the weapon flash
	float				m_flStopExtraSoundTime;
	
	float				m_flFlashLightTime;	// Time until next battery draw/Recharge
	int					m_iFlashBattery;		// Flashlight Battery Draw

	int					m_afButtonLast;
	int					m_afButtonPressed;
	int					m_afButtonReleased;
	
	edict_t			   *m_pentSndLast;			// last sound entity to modify player room type
	float				m_flSndRoomtype;		// last roomtype set by sound entity
	float				m_flSndRange;			// dist from player to sound entity

	float				m_flFallVelocity;
	
	int					m_rgItems[MAX_ITEMS];
	int					m_fKnownItem;		// True when a new item needs to be added
	int					m_fNewAmmo;			// True when a new item has been added
	
	unsigned int		m_afPhysicsFlags;	// physics flags - set when 'normal' physics should be revisited or overriden
	float				m_fNextSuicideTime; // the time after which the player can next use the suicide command

// these are time-sensitive things that we keep track of
	float				m_flTimeStepSound;	// when the last stepping sound was made
	float				m_flTimeWeaponIdle; // when to play another weapon idle animation.
	float				m_flSwimTime;		// how long player has been underwater
	float				m_flDuckTime;		// how long we've been ducking
	float				m_flWallJumpTime;	// how long until next walljump

	float				m_flSuitUpdate;					// when to play next suit update
	int					m_rgSuitPlayList[CSUITPLAYLIST];// next sentencenum to play for suit update
	int					m_iSuitPlayNext;				// next sentence slot for queue storage;
	int					m_rgiSuitNoRepeat[CSUITNOREPEAT];		// suit sentence no repeat list
	float				m_rgflSuitNoRepeatTime[CSUITNOREPEAT];	// how long to wait before allowing repeat
	int					m_lastDamageAmount;		// Last damage taken
	float				m_tbdPrev;				// Time-based damage timer

	float				m_flgeigerRange;		// range to nearest radiation source
	float				m_flgeigerDelay;		// delay per update of range msg to client
	int					m_igeigerRangePrev;
	int					m_iStepLeft;			// alternate left/right foot stepping sound
	char				m_szTextureName[CBTEXTURENAMEMAX];	// current texture name we're standing on
	char				m_chTextureType;		// current texture type

	int					m_idrowndmg;			// track drowning damage taken
	int					m_idrownrestored;		// track drowning damage restored

	int					m_bitsHUDDamage;		// Damage bits for the current fame. These get sent to 
												// the hude via the DAMAGE message
	BOOL				m_fInitHUD;				// True when deferred HUD restart msg needs to be sent
	BOOL				m_fGameHUDInitialized;
	int					m_iTrain;				// Train control position
	BOOL				m_fWeapon;				// Set this to FALSE to force a reset of the current weapon HUD info

	EHANDLE				m_pTank;				// the tank which the player is currently controlling,  NULL if no tank
	float				m_fDeadTime;			// the time at which the player died  (used in PlayerDeathThink())

	BOOL			m_fNoPlayerSound;	// a debugging feature. Player makes no sound if this is true. 
	BOOL			m_fLongJump; // does this player have the longjump module?

	float       m_tSneaking;
	int			m_iUpdateTime;		// stores the number of frame ticks before sending HUD update messages
	int			m_iClientHealth;	// the health currently known by the client.  If this changes, send a new
	int			m_iClientBattery;	// the Battery currently known by the client.  If this changes, send a new
	int			m_iHideHUD;		// the players hud weapon info is to be hidden
	int			m_iClientHideHUD;
	int			m_iFOV;			// field of view
	int			m_iClientFOV;	// client's known FOV
	// usable player items 
	CBasePlayerItem	*m_rgpPlayerItems[MAX_ITEM_TYPES];
	CBasePlayerItem *m_pActiveItem;
	CBasePlayerItem *m_pClientActiveItem;  // client version of the active item
	CBasePlayerItem *m_pLastItem;
	// shared ammo slots
	int	m_rgAmmo[MAX_AMMO_SLOTS];
	int	m_rgAmmoLast[MAX_AMMO_SLOTS];

	Vector				m_vecAutoAim;
	BOOL				m_fOnTarget;
	int					m_iDeaths;
	float				m_iRespawnFrames;	// used in PlayerDeathThink() to make sure players can always respawn

	int m_lastx, m_lasty;  // These are the previous update's crosshair angles, DON"T SAVE/RESTORE

	int m_nCustomSprayFrames;// Custom clan logo frames for this player
	float	m_flNextDecalTime;// next time this player can spray a decal

	char m_szTeamName[TEAM_NAME_LENGTH];

	virtual void Spawn( void );
	void Pain( void );

//	virtual void Think( void );
	virtual void Jump( void );
	virtual void Duck( void );
	virtual void PreThink( void );
	virtual void PostThink( void );
	virtual Vector GetGunPosition( void );
	virtual int TakeHealth( float flHealth, int bitsDamageType );
	virtual void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	virtual int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	virtual void	Killed( entvars_t *pevAttacker, int iGib );
	virtual Vector BodyTarget( const Vector &posSrc ) { return Center( ) + pev->view_ofs * RANDOM_FLOAT( 0.5, 1.1 ); };		// position to shoot at
	virtual void StartSneaking( void ) { m_tSneaking = gpGlobals->time - 1; }
	virtual void StopSneaking( void ) { m_tSneaking = gpGlobals->time + 30; }
	virtual BOOL IsSneaking( void ) { return m_tSneaking <= gpGlobals->time; }
	virtual BOOL IsAlive( void ) { return (pev->deadflag == DEAD_NO) && pev->health > 0; }
	virtual BOOL ShouldFadeOnDeath( void ) { return FALSE; }
	virtual	BOOL IsPlayer( void ) { return TRUE; }			// Spectators should return FALSE for this, they aren't "players" as far as game logic is concerned

	virtual BOOL IsNetClient( void ) { return TRUE; }		// Bots should return FALSE for this, they can't receive NET messages
															// Spectators should return TRUE for this
	virtual const char *TeamID( void );

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	void RenewItems(void);
	void PackDeadPlayerItems( void );
	void RemoveAllItems( BOOL removeSuit );
	BOOL SwitchWeapon( CBasePlayerItem *pWeapon );

	// JOHN:  sends custom messages if player HUD data has changed  (eg health, ammo)
	virtual void UpdateClientData( void );
	
	static	TYPEDESCRIPTION m_playerSaveData[];

	// Player is moved across the transition by other means
	virtual int		ObjectCaps( void ) { return CBaseMonster :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	virtual void	Precache( void );
	BOOL			IsOnLadder( void );
	BOOL			FlashlightIsOn( void );
	void			FlashlightTurnOn( void );
	void			FlashlightTurnOff( void );
	
	void UpdatePlayerSound ( void );
	void DeathSound ( void );

	int Classify ( void );
	void SetAnimation( PLAYER_ANIM playerAnim );
	void SetWeaponAnimType( const char *szExtention );
	char m_szAnimExtention[32];

	// custom player functions
	virtual void ImpulseCommands( void );
	void CheatImpulseCommands( int iImpulse );

	void StartDeathCam( void );
	void StartObserver( Vector vecPosition, Vector vecViewAngle );
	// Pcjoe: Robin Walker's Spectator code
	void    StopObserver( void );
	void	Observer_FindNextPlayer( bool bReverse );
	void	Observer_HandleButtons();
	void	Observer_SetMode( int iMode );
	EHANDLE	m_hObserverTarget;
	float	m_flNextObserverInput;
	int     IsObserver() { return pev->iuser1; };

	void AddPoints( int score, BOOL bAllowNegativeScore );
	void AddPointsToTeam( int score, BOOL bAllowNegativeScore );
	BOOL AddPlayerItem( CBasePlayerItem *pItem );
	BOOL RemovePlayerItem( CBasePlayerItem *pItem );
	void DropPlayerItem ( char *pszItemName );
	BOOL HasPlayerItem( CBasePlayerItem *pCheckItem );
	BOOL HasNamedPlayerItem( const char *pszItemName );
	BOOL HasWeapons( void );// do I have ANY weapons?
	void SelectPrevItem( int iItem );
	void SelectNextItem( int iItem );
	void SelectLastItem(void);
	void SelectItem(const char *pstr);
	void ItemPreFrame( void );
	void ItemPostFrame( void );
	void GiveNamedItem( const char *szName );
	void EnableControl(BOOL fControl);

	int  GiveAmmo( int iAmount, char *szName, int iMax );
	void SendAmmoUpdate(void);

	void WaterMove( void );
	void EXPORT PlayerDeathThink( void );
	void PlayerUse( void );

	void CheckSuitUpdate();
	void SetSuitUpdate(char *name, int fgroup, int iNoRepeat);
	void UpdateGeigerCounter( void );
	void CheckTimeBasedDamage( void );

	BOOL FBecomeProne ( void );
	void BarnacleVictimBitten ( entvars_t *pevBarnacle );
	void BarnacleVictimReleased ( void );
	static int GetAmmoIndex(const char *psz);
	int AmmoInventory( int iAmmoIndex );
	int Illumination( void );

	void ResetAutoaim( void );
	Vector GetAutoaimVector( float flDelta  );
	Vector AutoaimDeflection( Vector &vecSrc, float flDist, float flDelta  );

	void ForceClientDllUpdate( void );  // Forces all client .dll specific data to be resent to client.

	void DeathMessage( entvars_t *pevKiller );

	void SetCustomDecalFrames( int nFrames );
	int GetCustomDecalFrames( void );
	//int PlayerSpeed() {return ;}
	// Removed by Pcjoe
//	void CBasePlayer::TabulateAmmo( void );

	float m_flStartCharge;
	float m_flAmmoStartCharge;
	float m_flPlayAftershock;
	float m_flNextAmmoBurn;// while charging, when to absorb another unit of player's ammo?
	
	//Player ID
	void InitStatusBar( void );
	void UpdateStatusBar( void );
	int m_izSBarState[ SBAR_END ];
	float m_flNextSBarUpdateTime;
	float m_flStatusBarDisappearDelay;
	char m_SbarString0[ SBAR_STRING_SIZE ];
	char m_SbarString1[ SBAR_STRING_SIZE ];
	
	float m_flNextChatTime;
	
	// Rick
    void StartFly(void);
	void StopFly(void);
	void ShowVGUI( int iMenu );

	// added by grOOvy
	void incScore (int);
	void decScore (int);
	void setScore (int);
	// added 17/04/2004
	void setBlastScore (int);
	int iScoreBlastType;
	// end grOOvy

	// Added by Pcjoe
	/*
	* Change class
	*/
	void ChangeClass( void );
	
	/*
	* Change m_iPlayerclass, not the actual class though
	* @Param int iClass: The class the player will change into
	*/
	void ChangeNumericalClass( int iClass );

	/*
	* Update (local) Score Board
	*/
	void UpdateScoreBoard ( void );
	void LocalUpdateScoreBoard ( void );

	/*
	* Disable Special Modes
	*/
	void DisableSpecialModes( void );

	/*
	* Next Fly Mode
	*/
	void NextFlyMode( void );

	/*
	* Prev Fly Mode
	*/
	void PrevFlyMode( void );

	/*
	* Toggle Flight
	*/
	void ToggleFlight( void );

	/*
	* Send current fly mode
	*/
	void SendFlyMode( void );

	/*
	* Get Boost Direction
	*/
	int GetBoostDir( void );

	/*
	* Enable Boost
	*/
	void EnableBoost( void );

	/*
	* Handle Boost
	*/
	void HandleBoost( void );

	/*
	* Disable Boost
	*/
	void DisableBoost( void );

	/*
	* Get Flight Multiplier ( Multiplied by MaxSpeed )
	*/
	float GetFlightMultiplier( void );

	/*
	* Get Max Speed of player
	* @return int: Max speed of player
	*/
	int GetMaxSpeed( void );

	/*
	* Can Use Fuel
	* @return BOOL: True if we can use the specified amount
	*/
	BOOL CanUseFuel( float flAmount );

	/*
	* Use Fuel
	* @return BOOL: True if we can use the specified amount
	*/
	BOOL UseFuel( float flAmount );

	/*
	* Add Fuel
	*/
	void AddFuel( float flAmount );

	/*
	* Handle Fuel
	*/
	void HandleFuel( void );

	/*
	* Activate Negitive Fuel mode
	* @param float flTime: How long we will be in negitive fuel mode
	*/
	void StartNegFuel( float flTime );

	/*
	* Check Negitive Fuel mode
	*/
	void CheckNegFuel( void );

	/*
	* Disable Negitive Fuel mode
	*/
	void StopNegFuel( void );

	/*
	* Use Power
	* @return BOOL: True if we can use the specified amount
	*/
	BOOL UsePower( float flAmount );

	/*
	* Add Power
	*/
	void AddPower( float flAmount );

	/*
	* Can Use Power
	* @return BOOL: True if we can use the specified amount
	*/
	BOOL CanUsePower( float flAmount );

	/*
	* Convert Power To Energy
	*/
	void PowerToEnergy( void );

	/*
	* Enable Attached Model
	*/
	void EnabledAttachMdl( char *pModel );

	/*
	* Disable Attached Model
	*/
	void DisableAttachMdl( void );

	/*
	* Animation System
	*/

	/*
	* Set Animation - Only used for death animation
	@ Activity Activity: Ideal Activity
	*/
	void SetAnimation( Activity Activity );

	/*
	* Play Animation
	*/
	void PlayAnimation( void );

	/*
	* Full Body Animations
	* @return BOOL: If we're playing a full body animation
	*/
	BOOL FullBodyAnimation( std::list<entanim_s>::iterator iterAnim );

	/*
	* Get Weapon Idle
	* @return int: Animation Sequence
	*/
	int GetWeaponIdle( void );

	/*
	* Upper Body Animation
	* @return int: Upper Body Sequence
	*/
	int UpperBodyAnimation( std::list<entanim_s>::iterator iterAnim );

	/*
	* Gait Animation
	* @return int: Gait Sequence
	*/
	int GaitAnimation( std::list<entanim_s>::iterator iterAnim );

	/*
	* Add damage power
	* @param entvars_t *pevInflictor: Actual entity that "inflicted" the damage
	* @param float flDamage: Damage inflicted
	* @param int bitsDamageType: Damage Type
	*/
	void AddDamagePower( entvars_t *pevInflictor, float flDamage, int bitsDamageType );

	// Added by Rick
	BOOL				m_fFlying;          //set when flying

	// Added by grOOvy
	int					m_iPlayerClass;
	int					m_iPlayerScore;
	int					m_iClientScore;

	// Added by Pcjoe
	int					m_iClientSpeed;	// Speed the client has us set at
	int					m_iFlyMode;		// Fly Mode
	int					m_iBoostDir;	// Boost Direction
	int					m_iMissles;		// Amount of missles the player has out
	float				m_flSlowDown;	// Multiplied by the max speed
	float				m_flNegFuel;	// Negitive fuel time
	float				m_flPowerConvert;// Last power convert use
	float				m_flPower;		// (primarily) weapon power
	float				m_flClientPower;// Clients weapon power
	float				m_flKilled;		// Time Killed
	Vector				v_LastAngles;	// Last player angles, fix for rotating corpses
	BOOL				m_fNegFuel;		// Negitive fuel
	BOOL				m_fImpact;		// We hit the ground, impact
	BOOL				m_fBoost;		// Boost mode
	BOOL				m_fDoubleJump;	// Double jump mode
	BOOL				m_fFreedomBlast;// Freedom 5 way attack
	CPlayerAnimation	m_Anim;			// Extended Animation System
	CSaber				m_Saber;		// Saber system
	CBasePlayerClass	*m_pClass;		// Player class
	CBoost				*m_pBoost;		// Booster FX
	CAttachEnt			*m_pAttachEnt;	// Attached entity class

	// added by jason
	int					m_iPrevTeam;	// Previous team for seige gameplay
	// added by jason
	int					m_iNextClass;
	float				m_flRespawnTime;
};

#define AUTOAIM_2DEGREES  0.0348994967025
#define AUTOAIM_5DEGREES  0.08715574274766
#define AUTOAIM_8DEGREES  0.1391731009601
#define AUTOAIM_10DEGREES 0.1736481776669


extern int	gmsgHudText;
extern BOOL gInitHUD;

#endif // PLAYER_H

#define MENU_DEFAULT				1
#define MENU_TEAM 				    2
#define MENU_CLASS 				    3
#define MENU_MAPBRIEFING			4
#define MENU_INTRO 				    5
#define MENU_CLASSHELP				6
#define MENU_CLASSHELP2 			7
#define MENU_REPEATHELP 			8

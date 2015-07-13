/*
* MechMod Saber Handler
* @version: 5/18/04
* @author: Joseph 'Pcjoe' Florencio
*/
#ifndef SABERCOMBAT_H
#define SABERCOMBAT_H
#include "studio.h"

// tokamak defines
#ifndef CLIENT_DLL
#include <tokamak.h>
#pragma comment( lib, "tokamak.lib" )
#endif


#define MAX_RIGIDBODIES		96	// worst case scenario 32 players as ds 1 bbox, and 2 for saber
#define TOTAL_BODIES		MAX_RIGIDBODIES	// no animated bodies
#define	TIME_STEP			1.0f/30.0f		// work at a 30 fps scale

typedef enum
{	SABER_IDLE=1,
	SABER_ATTACK,
	SABER_DEFEND
}SABER_STATUS;

typedef enum
{	SBR_D_U=1,
	SBR_D_UR,
	SBR_D_R,
	SBR_D_DR,
	SBR_D_D,
	SBR_D_DL,
	SBR_D_L,
	SBR_D_UL
}SABER_DIR;

// INSERT CUSTOM STRUCTS HERE
struct saber_move_s
{
	float				flStartTime;	// Start time of swing
	float				flEstEndTime;	// End time ( or estimated if it's animation dependent )
	float				flDmg;			// Damage emitted by swing
	BOOL				fInit;			// Data is initialized by the saber system
	BOOL				fStall;			// If this is a "stall" in a swinging sequence
	BOOL				fStallFreezeLastFrame;// Freeze last frame for stall
	BOOL				fComplete;		// This swing is complete
	std::vector<int>	prevhitents;	// Previous entities we have already collided with
	std::string			strMoveName;	// Move name from script ( directional only )
	std::string			strCmbName;		// Combo name from script ( "" if nothing )
	std::string			strAnim;		// Animation
	int					iSaberDir[2];	// Direction of saber swing
};

// Saber attribute file
#include "mm_saberattribute.h"

class CSaber
{
public:
	// Constructor/Deconstructor
	CSaber();
	~CSaber();

#ifndef CLIENT_DLL
	/*
	* Reset
	*/
	void Reset( void );
	
	/*
	* Set Owner
	*/
	void SetOwner( CBasePlayer *plr ) { m_pOwner = plr; }

	/*
	* Get Owner
	*/
	CBasePlayer *GetOwner( void ) { return m_pOwner; }

	/*
	* Has melee weapon selected
	*/
	BOOL MeleeWpSel( void );

	/*
	* Can Use Saber
	*/
	BOOL CanUseSaber( void );

	/*
	* Holding Attack
	*/
	BOOL HoldingAttack( void );

	/*
	* Holding Defend
	*/
	BOOL HoldingDefend( void );

	/*
	* Set Status ( idle, attack, defend )
	* NOTE - m_eSaberStatus is set in this function, return is just for additional functionality
	* @return int: Saber status we set
	*/
	int SetStatus ( void );

	/*
	* Manage Saber
	*/
	void ManageSaber( void );

	/*
	* Complete Old Swings
	*/
	void CompleteOldSwings( void );
	
	/*
	* Remove outdated swings
	*/
	void RemoveOutdatedSwings( void );

	/*
	* Update From Script
	*/
	void UpdateFromScript( void );

	/*
	* Manage Stall
	*/
	void ManageStall( std::list<saber_move_s>::iterator iterSwing );

	/*
	* Remove Completed Stalls
	*/
	void RemoveCompletedStalls( void );

	/*
	* Update Saber Data
	*/
	void UpdateSaberData( void );

	/*
	* Get Swing Direction
	* @return int: Swing Direction
	*/
	int GetSwingDirection( void );
	std::string GetSwingDirectionString( void );

	/*
	* Get Opposite Direction
	* @param int iSwingDirection: Direction we want the opposite of
	*/
	int GetOppositeDirection( int iSwingDirection );

	/*
	* Start Idle
	*/
	void StartIdle( void );

	/*
	* Start Defend
	*/
	void StartDefend( void );

	/*
	* Stop Defend
	*/
	void StopDefend( void );

	/*
	* Handle Defend
	*/
	void HandleDefend( void );

	/*
	* Get Swing Percent
	*/
	float GetSwingPercent( void );

	/*
	* Set Swing Percent
	* @param float flPercent: Percentage of swing we're setting ( frame = 256 * percent )
	*/
	void SetSwingPercent( float flPercent );

	/*
	* Restore Swing Percent
	*/
	void RestoreSwingPercent( void ) { SetSwingPercent( GetSwingPercent() ); }

	/*
	* Get Current Swing
	*/
	std::list<saber_move_s>::iterator GetCurrentSwing( void );

	/*
	* Init Swing
	* @param std::list<saber_move_s>::iterator iterSaber: Iterator to saber swing that needs to be inited
	*/
	void InitSwing( std::list<saber_move_s>::iterator iterSaber );

	/*
	* Swings Complete
	* @return BOOL: If all current swings are completed
	*/
	BOOL SwingsComplete( void );

	/*
	* Saber Data Empty
	*/
	BOOL SaberDataEmpty( void ) { return m_SaberData.empty(); }

	/*
	* Get Studio Header
	* @return studiohdr_t*: Studio header
	*/
	studiohdr_t *GetStudioHeader( void );

	/*
	* Get Bone Origin
	* @param char *pName: Name of bone we're searching for
	* @return Vector: Bone origin
	*/
	Vector GetBoneOrigin( char *pName );

	/*
	* Set Reference Model
	*/
	void SetReferenceModel( void );

	/*
	* Restore Model ( from reference model )
	*/
	void RestoreModel( void );

	/*
	* Run Collision
	*/
	BOOL RunCollision( void );

	/*
	* Collision
	*/
	void Collision( void );

	/*
	* Add Attribute
	*/
	void AddAttribute( const char *pAttribute );

	/*
	* Reset Attributes
	*/
	void ResetAttributes( void );

	/*
	* Switch to next attribute
	*/
	void NextAttribute( void );

	/*
	* Switch to previous attribute
	*/
	void PreviousAttribute( void );

	/*
	* Get Current Attribute
	*/
	std::string GetCurrentAttribute( void );

	/*
	* Can Use Move
	*/
	BOOL CanUseMove( std::vector<std::string> strList );

	/*
	* Get Current Swing Direction
	*/
	std::string GetCurSwingDirection( void );

	/*
	* Get Saber Status
	* @purpose: lets us know if they are attacking or whatnot
	*/
	SABER_STATUS GetStatus( void ) { return m_eSaberStatus; }
	
	// Saber Attributes
	CSaberStyle		m_Attribute;
private:
	float	m_flSaberDelay;
	std::list<saber_move_s>	m_SaberData;
	std::vector<std::string> m_strAttributeNames;
	std::vector<std::string>::iterator m_strCurrentAttribute;
	SABER_STATUS	m_eSaberStatus;
	CBasePlayer		*m_pOwner;
#endif
};

#ifndef CLIENT_DLL
class CSaberPhys
{
public:
	/*
	* InitPhysics
	*/
	void InitPhysics( void );

	/*
	* CreatePlayerBox
	*/
	void CreatePlayerBox( CBasePlayer *pPlayer );


	void CreateSaberBox( CBasePlayer *pPlayer );

	/*
	* RemovePlayerBox
	*/
	void RemovePlayerBox( CBasePlayer *pPlayer );

	/*
	* UpdatePlayers
	*/
	void UpdatePlayers( CBasePlayer *pPlayer );

	/*
	*UpdatePhysics
	*/
	void UpdatePhysics( void );

	void TmpPhys( void );

	// who hit [ player attacking ] [ player hit ]
	bool					m_blWhoHit[32][32];
	bool					m_blWasSwinging[33];
	/*
	* KillPhysics
	*/
	void KillPhysics( void );
	static int/*void*/ CollisionCallback( neRigidBody * bodyA, neRigidBody * bodyB, neCustomCDInfo & cdInfo );
private:
	// added by jason for collision
	// so far i think i only store it here
//	std::list<neRigidBody*>	m_neBodies;		// if im storing these per player i need a list
	neSimulator					*m_pSim;
	neAllocatorDefault			m_neAllocator;
	neRigidBody					*m_pRigidBody[65];
	static bool					m_blIsValid[32];
};

//( neCollisionInfo &collisionInfo );

extern CSaberPhys g_SaberPhys;
#endif
#endif

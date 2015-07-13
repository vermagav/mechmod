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
//			
//  hud.h
//
// class CHud declaration
//
// CHud handles the message, calculation, and drawing the HUD
//


#define RGB_YELLOWISH 0x00FFA000 //255,160,0
#define RGB_REDISH 0x00FF1010 //255,160,0
#define RGB_GREENISH 0x0000A000 //0,160,0

// Pcjoe: STL
#pragma warning (disable:4530)
#pragma warning (disable:4786)
#include <list>
#include <vector>
#include <map>
#include <string>

#include "wrect.h"
#include "cl_dll.h"
#include "ammo.h"
// Added by Pcjoe
#include "studio.h"
#include "com_model.h"

#define DHN_DRAWZERO 1
#define DHN_2DIGITS  2
#define DHN_3DIGITS  4
#define MIN_ALPHA	 100	

// Pcjoe: Max locks
#define MAX_LOCKS	16
#define LOCKTYPE_PLAYER	1
#define LOCKTYPE_MISSLE	2

#define		HUDELEM_ACTIVE	1

typedef struct {
	int x, y;
} POSITION;

enum 
{ 
	MAX_PLAYERS = 64,
	MAX_TEAMS = 64,
	MAX_TEAM_NAME = 16,
};

typedef struct {
	unsigned char r,g,b,a;
} RGBA;

//added by jason
//typedef RGBA colorRGBA;	// not float :(
struct colorRGBA
{
	// colors
	float r, g, b, a;
};

typedef struct cvar_s cvar_t;


#define HUD_ACTIVE	1
#define HUD_INTERMISSION 2

#define MAX_PLAYER_NAME_LENGTH		32

#define	MAX_MOTD_LENGTH				1536

// Pcjoe: Snippet from Percy
#define ForEachIn( a, b ) for( a = b.begin(); a != b.end(); a++ )

// Record Bone Origins
struct recordbone_s
{	int					idx;
	int					iMaxBones;
	bool				blCanUse;
	bool				blRender;
	bool				blWeapon;
	bool				blRenderedFrame;
	char				szName[MAXSTUDIOBONES][32];
	float				pos[MAXSTUDIOBONES][3];
	int					iParent[MAXSTUDIOBONES];
};
typedef std::list<recordbone_s>::iterator recboneiter;

// Boost colors
#define	BOOST_BLUE		1
#define	BOOST_RED		2
#define	BOOST_PURPLE	3
#define BOOST_DOUBLEJUMPTIME	1
// Boost FX
struct boostfx_s
{	int iIndex;
	int iFX;
	int iFrameStart;
	int iFrameEnd;
	float flAnimateStart;
	float flAnimateEnd;
	float flDieTime;
	bool blDoubleJump;
	HSPRITE	hsprStartSprite;
	HSPRITE hsprEndSprite;

	recboneiter	iterRecPos;
};

// Saber FX
#define MAX_SABER_LINKS 32

struct saberfx_s
{	int		iIndex;
	bool	blUseSaberPos;
	float	flSaberLength;
	float	flFraction;
	float	flSparkEmitTime;
	vec3_t	vecSaberTrailEnd[MAX_SABER_LINKS];
	vec3_t	vecLastSaberPos[MAX_SABER_LINKS];
	vec3_t	vecLastOrigin;

	recboneiter	iterRecPos;
};


// Render check
typedef struct
{	bool blRendered;
	bool blFlag;
}rendercheck_t;

// Lockon data
typedef struct
{	int iIndex;
	int iAmt;
	int iType;
	bool blLock;
}lockon_t;

// LockMdl data
typedef struct
{	int iIndex;
	vec3_t vecAng;
}lockmdl_t;

//
//-----------------------------------------------------
//
class CHudBase
{
public:
	POSITION  m_pos;
	int   m_type;
	int	  m_iFlags; // active, moving, 
	virtual		~CHudBase() {}
	virtual int Init( void ) {return 0;}
	virtual int VidInit( void ) {return 0;}
	virtual int Draw(float flTime) {return 0;}
	virtual void Think(void) {return;}
	virtual void Reset(void) {return;}
	virtual void InitHUDData( void ) {}		// called every time a server is connected to

};

struct HUDLIST {
	CHudBase	*p;
	HUDLIST		*pNext;
};



//
//-----------------------------------------------------
//
#include "..\game_shared\voice_status.h"
#include "hud_spectator.h"


//
//-----------------------------------------------------
//
class CHudAmmo: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	void Think(void);
	void Reset(void);
	int DrawWList(float flTime);
	int MsgFunc_CurWeapon(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_WeaponList(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_AmmoX(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_AmmoPickup( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_WeapPickup( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_ItemPickup( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_HideWeapon( const char *pszName, int iSize, void *pbuf );

	void SlotInput( int iSlot );
	void _cdecl UserCmd_Slot1( void );
	void _cdecl UserCmd_Slot2( void );
	void _cdecl UserCmd_Slot3( void );
	void _cdecl UserCmd_Slot4( void );
	void _cdecl UserCmd_Slot5( void );
	void _cdecl UserCmd_Slot6( void );
	void _cdecl UserCmd_Slot7( void );
	void _cdecl UserCmd_Slot8( void );
	void _cdecl UserCmd_Slot9( void );
	void _cdecl UserCmd_Slot10( void );
	void _cdecl UserCmd_Close( void );
	void _cdecl UserCmd_NextWeapon( void );
	void _cdecl UserCmd_PrevWeapon( void );

	WEAPON *m_pWeapon;
private:
	float m_fFade;
	RGBA  m_rgba;
	// Pcjoe: made public
//	WEAPON *m_pWeapon;
	int	m_HUD_bucket0;
	int m_HUD_selection;
	// Pcjoe: weapon sprites
	int m_HUD_wpleft;
	int m_HUD_wpcenter;
	int m_HUD_wpright;
	int m_iweapedgeWidth;
	int m_iweapedgeHeight;
	int m_iweapcenterWidth;
	int m_iweapcenterHeight;
	// Pcjoe: Sliding
	bool m_blSliding;
	bool m_blSlideIn;
	bool m_blPrevStatus;
	float m_flSlideTime;
	float m_flLastSelect;

	/*
	* Pcjoe: Slide In (dissapear)
	*/
	void SlideIn( void );

	/*
	* Pcjoe: Slide Out (appear)
	*/
	void SlideOut( void );

	/*
	* Pcjoe: Get Y Base
	*/
	int GetYBase( int iHeight );
};

//
//-----------------------------------------------------
//

class CHudAmmoSecondary: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	void Reset( void );
	int Draw(float flTime);

	int MsgFunc_SecAmmoVal( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_SecAmmoIcon( const char *pszName, int iSize, void *pbuf );

private:
	enum {
		MAX_SEC_AMMO_VALUES = 4
	};

	int m_HUD_ammoicon; // sprite indices
	int m_iAmmoAmounts[MAX_SEC_AMMO_VALUES];
	float m_fFade;
};


#include "health.h"


#define FADE_TIME 100


//
//-----------------------------------------------------
//
class CHudGeiger: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	int MsgFunc_Geiger(const char *pszName, int iSize, void *pbuf);
	
private:
	int m_iGeigerRange;

};

//
//-----------------------------------------------------
//
class CHudTrain: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	int MsgFunc_Train(const char *pszName, int iSize, void *pbuf);

private:
	HSPRITE m_hSprite;
	int m_iPos;

};

//
//-----------------------------------------------------
//
// REMOVED: Vgui has replaced this.
//
/*
class CHudMOTD : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float flTime );
	void Reset( void );

	int MsgFunc_MOTD( const char *pszName, int iSize, void *pbuf );

protected:
	static int MOTD_DISPLAY_TIME;
	char m_szMOTD[ MAX_MOTD_LENGTH ];
	float m_flActiveRemaining;
	int m_iLines;
};
*/

//
//-----------------------------------------------------
//
class CHudStatusBar : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float flTime );
	void Reset( void );
	void ParseStatusString( int line_num );

	int MsgFunc_StatusText( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_StatusValue( const char *pszName, int iSize, void *pbuf );

protected:
	enum { 
		MAX_STATUSTEXT_LENGTH = 128,
		MAX_STATUSBAR_VALUES = 8,
		MAX_STATUSBAR_LINES = 2,
	};

	char m_szStatusText[MAX_STATUSBAR_LINES][MAX_STATUSTEXT_LENGTH];  // a text string describing how the status bar is to be drawn
	char m_szStatusBar[MAX_STATUSBAR_LINES][MAX_STATUSTEXT_LENGTH];	// the constructed bar that is drawn
	int m_iStatusValues[MAX_STATUSBAR_VALUES];  // an array of values for use in the status bar

	int m_bReparseString; // set to TRUE whenever the m_szStatusBar needs to be recalculated

	// an array of colors...one color for each line
	float *m_pflNameColors[MAX_STATUSBAR_LINES];
};

//
//-----------------------------------------------------
//
// REMOVED: Vgui has replaced this.
//
/*
class CHudScoreboard: public CHudBase
{
public:
	int Init( void );
	void InitHUDData( void );
	int VidInit( void );
	int Draw( float flTime );
	int DrawPlayers( int xoffset, float listslot, int nameoffset = 0, char *team = NULL ); // returns the ypos where it finishes drawing
	void UserCmd_ShowScores( void );
	void UserCmd_HideScores( void );
	int MsgFunc_ScoreInfo( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_TeamInfo( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_TeamScore( const char *pszName, int iSize, void *pbuf );
	void DeathMsg( int killer, int victim );

	int m_iNumTeams;

	int m_iLastKilledBy;
	int m_fLastKillTime;
	int m_iPlayerNum;
	int m_iShowscoresHeld;

	void GetAllPlayersInfo( void );
private:
	struct cvar_s *cl_showpacketloss;

};
*/

struct extra_player_info_t 
{
	short frags;
	short deaths;
	short playerclass;
	short teamnumber;
	char teamname[MAX_TEAM_NAME];
};

struct team_info_t 
{
	char name[MAX_TEAM_NAME];
	short frags;
	short deaths;
	short ping;
	short packetloss;
	short ownteam;
	short players;
	int already_drawn;
	int scores_overriden;
	int teamnumber;
};

extern hud_player_info_t	g_PlayerInfoList[MAX_PLAYERS+1];	   // player info from the engine
extern extra_player_info_t  g_PlayerExtraInfo[MAX_PLAYERS+1];   // additional player info sent directly to the client dll
extern team_info_t			g_TeamInfo[MAX_TEAMS+1];
extern int					g_IsSpectator[MAX_PLAYERS+1];


//
//-----------------------------------------------------
//
class CHudDeathNotice : public CHudBase
{
public:
	int Init( void );
	void InitHUDData( void );
	int VidInit( void );
	int Draw( float flTime );
	int MsgFunc_DeathMsg( const char *pszName, int iSize, void *pbuf );

private:
	int m_HUD_d_skull;  // sprite index of skull icon
};

//
//-----------------------------------------------------
//
class CHudMenu : public CHudBase
{
public:
	int Init( void );
	void InitHUDData( void );
	int VidInit( void );
	void Reset( void );
	int Draw( float flTime );
	int MsgFunc_ShowMenu( const char *pszName, int iSize, void *pbuf );

	void SelectMenuItem( int menu_item );

	int m_fMenuDisplayed;
	int m_bitsValidSlots;
	float m_flShutoffTime;
	int m_fWaitingForMore;
};

//
//-----------------------------------------------------
//
class CHudSayText : public CHudBase
{
public:
	int Init( void );
	void InitHUDData( void );
	int VidInit( void );
	int Draw( float flTime );
	int MsgFunc_SayText( const char *pszName, int iSize, void *pbuf );
	void SayTextPrint( const char *pszBuf, int iBufSize, int clientIndex = -1 );
	void EnsureTextFitsInOneLineAndWrapIfHaveTo( int line );
friend class CHudSpectator;

private:

	struct cvar_s *	m_HUD_saytext;
	struct cvar_s *	m_HUD_saytext_time;
};

//
//-----------------------------------------------------
//
class CHudBattery: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	int MsgFunc_Battery(const char *pszName,  int iSize, void *pbuf );
	
	int	  m_iBat;
private:
	HSPRITE m_hSprite1;
	HSPRITE m_hSprite2;
	wrect_t *m_prc1;
	wrect_t *m_prc2;
	// Edited by Pcjoe, made public
//	int	  m_iBat;	
	float m_fFade;
	int	  m_iHeight;		// width of the battery innards
};


//
//-----------------------------------------------------
//
class CHudFlashlight: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	void Reset( void );
	int MsgFunc_Flashlight(const char *pszName,  int iSize, void *pbuf );
	int MsgFunc_FlashBat(const char *pszName,  int iSize, void *pbuf );
	
private:
	HSPRITE m_hSprite1;
	HSPRITE m_hSprite2;
	HSPRITE m_hBeam;
	wrect_t *m_prc1;
	wrect_t *m_prc2;
	wrect_t *m_prcBeam;
	float m_flBat;	
	int	  m_iBat;	
	int	  m_fOn;
	float m_fFade;
	int	  m_iWidth;		// width of the battery innards
};

//
//-----------------------------------------------------
//
/*
* Set Client Speed
* @version 12/16/03
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudSpeed: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Think
	*/
	void Think(void);

private:
	
	/*
	* Set player speed
	*/
	void SetSpeed( void );

	int m_iSpeed;
};

//
//-----------------------------------------------------
//
/*
* Get Bone Positions
* @version 12/21/03
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudBonePos: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Reset
	*/
	void ResetBoneFx( void );

	/*
	* Draw
	*/
	int Draw(float flTime);

	/*
	* Add Bone Record
	* @param int idx: Index of entity
	* @return recboneiter: Bone we added
	*/
	recboneiter AddBoneRecord( int idx );
	recboneiter AddBoneRecordWeapon( int idx );
	recboneiter AddBoneRecordNormal( int idx );

	/*
	* Remove Bone Record (by entity index)
	* @param int idx: Index of entity
	* @return recboneiter: Bone after the one we just removed
	*/
	recboneiter RemoveBoneRecord( int idx );
	recboneiter RemoveBoneRecordWeapon( int idx );

	/*
	* Add Bone Record
	* @param int idx: Index of entity
	* @return recordbone_s: Bone after the one we just removed
	*/
	recboneiter RemoveBoneRecord( recboneiter iterBone );

	/*
	* Find Bone Record
	* @param int idx: Index of entity
	* @return recboneiter: If we found the bone
	*/
	recboneiter FindBoneRecord( int idx );
	recboneiter FindBoneRecordAll( int idx );
	recboneiter FindBoneRecordWeapon( int idx );

	/*
	* Get Bone Origin
	* @param recordbone_s: Reference bone origins
	* @param char *pName: Name of specific bone
	* @param int iBone: Bone index
	* @param bool &blValid: If this bone is valid
	* @return vec3_t GetBoneOrigin: Bone Origin
	*/
	vec3_t GetBoneOrigin( recboneiter iterBone, char *pName, bool &blValid );
	vec3_t GetBoneOrigin( recboneiter iterpBone, int iBone, bool &blValid );
	// modifed by jason
	vec3_t GetBoneOriginName( recboneiter iterpBone, int iBone, bool &blValid, char &szParent );
	vec3_t GetParentBoneOrigin( recboneiter iterpBone, char *pName, bool &blValid, char &szParent );

	std::list<recordbone_s> m_RecBones;
};

//
//-----------------------------------------------------
//
/*
* BoostFX
* @version 12/21/03
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudBoostFX: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Reset
	*/
	void Reset( void );

	/*
	* Init Hud data on connect
	*/
	void InitHUDData( void );

	/*
	* Draw
	*/
	int Draw(float flTime);

	/*
	* Boost FX message
	*/
	int MsgFunc_Boostfx(const char *pszName,  int iSize, void *pbuf );

	/*
	* Add Boostfx
	*/
	void AddBoostfx( int idx, int fx, bool dbljump );

	/*
	* Remove Boostfx
	* @param int idx: Entity Index
	* @return std::list<boostfx_s>::iterator: member after deleted one
	*/
	std::list<boostfx_s>::iterator RemoveBoostfx( int idx );

	/*
	* Remove Boostfx
	* @param std::list<boostfx_s>::iterator iterBoost: Boost structure
	* @return std::list<boostfx_s>::iterator: member after deleted one
	*/
	std::list<boostfx_s>::iterator RemoveBoostfx( std::list<boostfx_s>::iterator iterBoost );

	/*
	* Find Boostfx
	* @param int idx: entity index
	* @return std::list<boostfx_s>::iterator: Boost fx we searched for
	*/
	std::list<boostfx_s>::iterator FindBoostfx( int idx );

	std::list<boostfx_s> m_BoostFX;
private:
	HSPRITE m_hsprBoostBlue_end;
	HSPRITE m_hsprBoostRed_end;
	HSPRITE m_hsprBoostPurple_end;
	HSPRITE m_hsprBoostBlue_start;
	HSPRITE m_hsprBoostRed_start;
	HSPRITE m_hsprBoostPurple_start;
};

//
//-----------------------------------------------------
//
/*
* SaberFX
* @version 12/22/03
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudSaberFX: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Reset
	*/
	void Reset( void );

	/*
	* Init Hud data on connect
	*/
	void InitHUDData( void );

	/*
	* Draw
	*/
	int Draw(float flTime);

	/*
	* Saber FX message
	*/
	int MsgFunc_Saberfx(const char *pszName,  int iSize, void *pbuf );

	/*
	* Add Saberfx
	*/
	void AddSaberfx( int idx );

	/*
	* Remove Saberfx
	* @param int idx: Entity Index
	* @return std::list<saberfx_s>::iterator: member after deleted one
	*/
	std::list<saberfx_s>::iterator RemoveSaberfx( int idx );

	/*
	* Remove Saberfx
	* @param std::list<saberfx_s>::iterator iterSaber: Saber structure
	* @return std::list<saberfx_s>::iterator: member after deleted one
	*/
	std::list<saberfx_s>::iterator RemoveSaberfx( std::list<saberfx_s>::iterator iterSaber );

	/*
	* Find Saberfx
	* @param int idx: entity index
	* @return std::list<saberfx_s>::iterator: Saber fx we searched for
	*/
	std::list<saberfx_s>::iterator FindSaberfx( int idx );

	std::list<saberfx_s> m_SaberFX;
};

//
//-----------------------------------------------------
//

/*
* Draw Fuel/Energy
* @version 1/05/04
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudEnergy: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Draw
	*/
	int Draw(float flTime);

	/*
	* Power message
	*/
	int MsgFunc_Power(const char *pszName,  int iSize, void *pbuf );

	/*
	* FlyMode message
	*/
	int MsgFunc_Flymode(const char *pszName,  int iSize, void *pbuf );

	/*
	* Max health message
	*/
	int MsgFunc_MaxHealth(const char *pszName,  int iSize, void *pbuf );

	// Sprite sizes
	int m_iMainWidth;
	int m_iMainHeight;
private:
	// sprites
	int m_HUD_mainback;
	int m_HUD_mainbars;
	int m_HUD_flymode;
	int m_HUD_wepbar;
	int m_HUD_sidebar;

	// Power amount
	float m_flPower;
	float m_iFlyMode;

	// Max Health/Armor
	int	m_iMaxHealth;
	int m_iMaxArmor;
};

//
//-----------------------------------------------------
//

/*
* Crosshair
* @version 1/08/04
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudCrosshair: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Reset
	*/
	void Reset( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Draw
	*/
	int Draw(float flTime);

	int m_iMouseOverEnt;
	float m_flMouseOverTime;

private:
	/*
	* Set player Crosshair
	*/
	void SetCrosshairSprite( void );

	int m_iCrosshair;
	int m_HUD_Crosshair;
};

//
//-----------------------------------------------------
//

/*
* Radar
* @version 1/11/04
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudRadar: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Draw
	*/
	int Draw(float flTime);

	// Radar on/off
	bool m_blRadar;
	bool m_blSliding;
	int	m_iyBottomRadar;
private:
	/*
	* Set Class Names to local string
	*/
	void SetClassNames( void );

	// HACK HACK: PC_RANDOM is 7
	char szClassNames[7][32];

	// sprites
	int m_HUD_radar;
	int m_HUD_radar_info;
	int m_HUD_radardot;

	bool m_blPrevRadar;
	float m_flSlideTime;
};

//
//-----------------------------------------------------
//

/*
* Lockon
* @version 1/17/04
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudLockon: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Reset
	*/
	void Reset( void );

	/*
	* Draw
	*/
	int Draw(float flTime);
	
	/*
	* Lockon message
	*/
	int MsgFunc_Lockon(const char *pszName,  int iSize, void *pbuf );

	/*
	* Find Lock
	* @param int idx: Index of entity we're finding
	* @return int: m_Locks member index
	*/
	int FindLock( int idx );
private:
	/*
	* Add Lock
	* @param int idx: Index of entity we're locking
	*/
	void AddLock( int idx );
	
	/*
	* Add Prelock
	* @param int idx: Index of entity we're locking
	*/
	void AddPreLock( int type, int idx );

	/*
	* Add Missle Lock
	* @param int idx: Index of entity we're locking
	*/
	void AddMisLock( int idx );

	/*
	* Remove Lock
	* @param blAll: Remove all locks related to this index
	* @param int idx: index of entity
	*/
	void RemoveLock( bool blAll, int idx );

	/*
	* Reset Lock Data
	* @param int i: m_Locks index
	*/
	void ResetLockData( int i );

	/*
	* Clear all locks
	*/
	void ClearLocks( void );
	
	int m_HUD_lockon;		// Lock on to enemy with laser/missle weapon
	int m_HUD_misslelock;	// Using a gun to lock onto missle entities + shoot them down
	int m_HUD_prelock;		// Attempting to lock
	lockon_t m_Locks[MAX_LOCKS];
};

//
//-----------------------------------------------------
//

/*
* Draw Charge Bar
* @version 1/31/04
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudChargeBar: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Reset
	*/
	void Reset( void );

	/*
	* Init sprites/models/etc
	*/
	int VidInit( void );

	/*
	* Draw
	*/
	int Draw(float flTime);

	// Background sprite size
	int m_iMainWidth;
	int m_iMainHeight;

private:
	// Sprites
	int m_HUD_main;
	int m_HUD_barafter;
	int m_HUD_barpre;
};

//
//-----------------------------------------------------
//

/*
* Lock Models
* @version 10/11/04
* @author: Joseph 'Pcjoe' Florencio
*/
class CHudLockMdl: public CHudBase
{
public:
	/*
	* Init Hud Data
	*/
	int Init( void );

	/*
	* Reset
	*/
	void Reset( void );

	/*
	* Model Lock message
	*/
	int MsgFunc_LockMdl(const char *pszName,  int iSize, void *pbuf );

	std::list<lockmdl_t> m_LockEnts;
};

//
//-----------------------------------------------------
//

/**********************************
*		CHUDPARTMSG
* created on: 1-6-03
* last modified: 1-6-03
* created by: Jason "ssjason123" Matson
**********************************/

class CHudPartMsg : public CHudBase
{
public:
	/*********************
	*	Init
	* @purpose: Initialize our vars
	* @param void: no param
	* @return int: if we init'ed properly
	*********************/
	int			Init( void );
	/*********************
	*	Vid Init
	* @purpose: Initalizes vars mostly
	* @param void: no param
	* @return int: if it was properly init'ed
	*********************/
	int			VidInit( void );
	/*********************
	*	MsgFunc_PartMsg
	* @purpose: Meat of the system, recives particle msgs
	* @param char *pszName: standard hl msging stuff
	* @param int iSize: standard hl msging stuff
	* @param void *pbuf: standard hl msging stuff
	* @return int: did we successfuly recive the msg
	*********************/
	int			MsgFunc_PartMsg( const char *pszName, int iSize, void *pbuf );
private:
};

/**********************************
*		CHUDDECALMSG
* created on: 2-8-04
* last modified: 2-8-04
* created by: Jason "ssjason123" Matson
**********************************/

class CHudDecalMsg : public CHudBase
{
public:
	/*********************
	*	Init
	* @purpose: Initialize our vars
	* @param void: no param
	* @return int: if we init'ed properly
	*********************/
	int			Init( void );
	/*********************
	*	Vid Init
	* @purpose: Initalizes vars mostly
	* @param void: no param
	* @return int: if it was properly init'ed
	*********************/
	int			VidInit( void );
	/*********************
	*	MsgFunc_DecalMsg
	* @purpose: Meat of the system, recives decal msgs
	* @param char *pszName: standard hl msging stuff
	* @param int iSize: standard hl msging stuff
	* @param void *pbuf: standard hl msging stuff
	* @return int: did we successfuly recive the msg
	*********************/
	int			MsgFunc_DecalMsg( const char *pszName, int iSize, void *pbuf );
	/*********************
	* RemoveDecals
	* @purpose: Removes decals
	* @param void: no param
	* @return void: no return
	*********************/
	void		RemoveDecals( void );
private:
};

/**********************************
*		CHUDENTRAIL
* created on: 3-2-04
* last modified: 3-2-04
* created by: Jason "ssjason123" Matson
**********************************/

class CHudEntTrail : public CHudBase
{
public:
	/*********************
	*	Init
	* @purpose: Initialize our vars
	* @param void: no param
	* @return int: if we init'ed properly
	*********************/
	int			Init( void );
	/*********************
	*	Vid Init
	* @purpose: Initalizes vars mostly
	* @param void: no param
	* @return int: if it was properly init'ed
	*********************/
	int			VidInit( void );
	/*********************
	*	MsgFunc_DecalMsg
	* @purpose: Meat of the system, recives decal msgs
	* @param char *pszName: standard hl msging stuff
	* @param int iSize: standard hl msging stuff
	* @param void *pbuf: standard hl msging stuff
	* @return int: did we successfuly recive the msg
	*********************/
	int			MsgFunc_EntTrail( const char *pszName, int iSize, void *pbuf );
private:
};

//
//-----------------------------------------------------
//

// Score Module, by grOOvy //
 
class CHudScore : public CHudBase 
{ 
public: 
	int Init( void ); 
	int VidInit( void ); 
	int Draw( float flTime ); // This always has to be called draw, and always take a float param 
	int MsgFunc_Score ( const char *pszName,  int iSize, void *pbuf ); 
 
private: 
	int m_iScore;
};


//
//-----------------------------------------------------
//

const int maxHUDMessages = 16;
struct message_parms_t
{
	client_textmessage_t	*pMessage;
	float	time;
	int x, y;
	int	totalWidth, totalHeight;
	int width;
	int lines;
	int lineLength;
	int length;
	int r, g, b;
	int text;
	int fadeBlend;
	float charTime;
	float fadeTime;
};

//
//-----------------------------------------------------
//

class CHudTextMessage: public CHudBase
{
public:
	int Init( void );
	static char *LocaliseTextString( const char *msg, char *dst_buffer, int buffer_size );
	static char *BufferedLocaliseTextString( const char *msg );
	char *LookupString( const char *msg_name, int *msg_dest = NULL );
	int MsgFunc_TextMsg(const char *pszName, int iSize, void *pbuf);
};

//
//-----------------------------------------------------
//

class CHudMessage: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	int MsgFunc_HudText(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_GameTitle(const char *pszName, int iSize, void *pbuf);

	float FadeBlend( float fadein, float fadeout, float hold, float localTime );
	int	XPosition( float x, int width, int lineWidth );
	int YPosition( float y, int height );

	void MessageAdd( const char *pName, float time );
	void MessageAdd(client_textmessage_t * newMessage );
	void MessageDrawScan( client_textmessage_t *pMessage, float time );
	void MessageScanStart( void );
	void MessageScanNextChar( void );
	void Reset( void );

private:
	client_textmessage_t		*m_pMessages[maxHUDMessages];
	float						m_startTime[maxHUDMessages];
	message_parms_t				m_parms;
	float						m_gameTitleTime;
	client_textmessage_t		*m_pGameTitle;

	int m_HUD_title_life;
	int m_HUD_title_half;
};

//
//-----------------------------------------------------
//
#define MAX_SPRITE_NAME_LENGTH	24

class CHudStatusIcons: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	void Reset( void );
	int Draw(float flTime);
	int MsgFunc_StatusIcon(const char *pszName, int iSize, void *pbuf);

	enum { 
		MAX_ICONSPRITENAME_LENGTH = MAX_SPRITE_NAME_LENGTH,
		MAX_ICONSPRITES = 4,
	};

	
	//had to make these public so CHud could access them (to enable concussion icon)
	//could use a friend declaration instead...
	void EnableIcon( char *pszIconName, unsigned char red, unsigned char green, unsigned char blue );
	void DisableIcon( char *pszIconName );

private:

	typedef struct
	{
		char szSpriteName[MAX_ICONSPRITENAME_LENGTH];
		HSPRITE spr;
		wrect_t rc;
		unsigned char r, g, b;
	} icon_sprite_t;

	icon_sprite_t m_IconList[MAX_ICONSPRITES];

};

//
//-----------------------------------------------------
//



class CHud
{
private:
	HUDLIST						*m_pHudList;
	HSPRITE						m_hsprLogo;
	int							m_iLogo;
	client_sprite_t				*m_pSpriteList;
	int							m_iSpriteCount;
	int							m_iSpriteCountAllRes;
	float						m_flMouseSensitivity;
	int							m_iConcussionEffect; 

public:

	// added by jason
	bool	m_blHideHud;
	// end jason

	HSPRITE						m_hsprCursor;
	float m_flTime;	   // the current client time
	float m_fOldTime;  // the time at which the HUD was last redrawn
	double m_flTimeDelta; // the difference between flTime and fOldTime
	Vector	m_vecOrigin;
	Vector	m_vecAngles;
	int		m_iKeyBits;
	int		m_iHideHUDDisplay;
	int		m_iFOV;
	int		m_Teamplay;
	int		m_iRes;
	cvar_t  *m_pCvarStealMouse;
	cvar_t	*m_pCvarDraw;

	int m_iFontHeight;
	int DrawHudNumber(int x, int y, int iFlags, int iNumber, int r, int g, int b );
	int DrawHudString(int x, int y, int iMaxX, char *szString, int r, int g, int b );
	int DrawHudStringReverse( int xpos, int ypos, int iMinX, char *szString, int r, int g, int b );
	int DrawHudNumberString( int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b );
	int GetNumWidth(int iNumber, int iFlags);

private:
	// the memory for these arrays are allocated in the first call to CHud::VidInit(), when the hud.txt and associated sprites are loaded.
	// freed in ~CHud()
	HSPRITE *m_rghSprites;	/*[HUD_SPRITE_COUNT]*/			// the sprites loaded from hud.txt
	wrect_t *m_rgrcRects;	/*[HUD_SPRITE_COUNT]*/
	char *m_rgszSpriteNames; /*[HUD_SPRITE_COUNT][MAX_SPRITE_NAME_LENGTH]*/

	struct cvar_s *default_fov;
public:
	HSPRITE GetSprite( int index ) 
	{
		return (index < 0) ? 0 : m_rghSprites[index];
	}

	wrect_t& GetSpriteRect( int index )
	{
		return m_rgrcRects[index];
	}

	
	int GetSpriteIndex( const char *SpriteName );	// gets a sprite index, for use in the m_rghSprites[] array

	CHudAmmo		m_Ammo;
	CHudHealth		m_Health;
	CHudSpectator		m_Spectator;
	CHudGeiger		m_Geiger;
	CHudBattery		m_Battery;
	CHudTrain		m_Train;
	CHudFlashlight	m_Flash;
	CHudMessage		m_Message;
	CHudStatusBar   m_StatusBar;
	CHudDeathNotice m_DeathNotice;
	CHudSayText		m_SayText;
	CHudMenu		m_Menu;
	CHudAmmoSecondary	m_AmmoSecondary;
	CHudTextMessage m_TextMessage;
	CHudStatusIcons m_StatusIcons;
	// Added by Pcjoe
	CHudSpeed		m_Speed;
	CHudBonePos		m_BonePos;
	CHudBoostFX		m_Boostfx;
	CHudSaberFX		m_Saberfx;
	CHudEnergy		m_Energy;
	CHudCrosshair	m_Crosshair;
	CHudRadar		m_Radar;
	CHudLockon		m_Lockon;
	CHudChargeBar	m_ChargeBar;
	CHudLockMdl		m_LockMdl;
	// added by jason 1-6-03
	CHudPartMsg		m_PartMsg;
	CHudDecalMsg	m_DecalMsg;
	CHudEntTrail	m_EntTrail;
	// end jason
	// added by grOOvy
	CHudScore		m_Score;
	// end grOOvy

	void Init( void );
	void VidInit( void );
	void Think(void);
	int Redraw( float flTime, int intermission );
	int UpdateClientData( client_data_t *cdata, float time );

	CHud() : m_iSpriteCount(0), m_pHudList(NULL) {}  
	~CHud();			// destructor, frees allocated memory

	// user messages
	int _cdecl MsgFunc_Damage(const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_Logo(const char *pszName,  int iSize, void *pbuf);
	int _cdecl MsgFunc_ResetHUD(const char *pszName,  int iSize, void *pbuf);
	void _cdecl MsgFunc_InitHUD( const char *pszName, int iSize, void *pbuf );
	void _cdecl MsgFunc_ViewMode( const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_SetFOV(const char *pszName,  int iSize, void *pbuf);
	int  _cdecl MsgFunc_Concuss( const char *pszName, int iSize, void *pbuf );

	// Screen information
	SCREENINFO	m_scrinfo;

	int	m_iWeaponBits;
	int	m_fPlayerDead;
	int m_iIntermission;

	// sprite indexes
	int m_HUD_number_0;


	void AddHudElem(CHudBase *p);

	float GetSensitivity();

};

class TeamFortressViewport;

extern CHud gHUD;
extern TeamFortressViewport *gViewPort;

extern int g_iPlayerClass;
extern int g_iTeamNumber;
extern int g_iUser1;
extern int g_iUser2;
extern int g_iUser3;
// Added by Pcjoe
extern float g_flFuel;
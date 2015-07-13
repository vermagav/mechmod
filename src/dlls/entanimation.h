/*
* Extended Entity Animation System
* @version 8/08/04
* @author: Joseph 'Pcjoe' Florencio
*/
#ifndef ENTANIM_H
#define ENTANIM_H

typedef enum
{	ANIMTYPE_FULLBODY=0,
	ANIMTYPE_UPPERGAIT,
	ANIMTYPE_UPPERONLY,
}ANIMTYPE;

typedef enum
{	PLAY_NONE,	// Used stricly for removing animations
	PLAY_DRAW,
	PLAY_ATTACK1,
	PLAY_ATTACK2,
	PLAY_CHARGE1,
	PLAY_CHARGE2,
	PLAY_CHARGE_SHOOT1,
	PLAY_CHARGE_SHOOT2,
	PLAY_MISC,
} PLAY_ANIM;

typedef enum
{	PRIOR_VERYLOW=0,
	PRIOR_LOW,
	PRIOR_MEDIUM,
	PRIOR_HIGH,
	PRIOR_VERYHIGH,
}PRIORITY_ANIMS;


struct entanim_s
{	std::vector<std::string> strAnim;
	int				iCurAnim;
	int				iMaxAnims;
	float			flStartTime;
	BOOL			fLoop;
	BOOL			fInit;
	BOOL			fSaber;
	Activity		eActivity;
	ANIMTYPE		eAnimType;
	PLAY_ANIM		ePlayAnim;
	PRIORITY_ANIMS	ePriority;
};

/*
* Entity Animation System
*/
class CEntAnimation
{
public:
	// Constructor + Destructor
	CEntAnimation();
	~CEntAnimation();

	/*
	* Set Owner
	* @param CBasePlayer *pl: Entity we're setting as owner
	*/
	void SetOwner( CBaseAnimating *pEnt ) { m_pOwner = pEnt; }

	/*
	* Get Owner
	*/
	CBaseEntity *GetOwner( void ) { return (CBaseEntity*)m_pOwner; }

	/*
	* Get Owner Player
	*/
	CBasePlayer *GetOwnerPlr( void ) 
		{ return (m_pOwner && m_pOwner->IsPlayer()) ? (CBasePlayer*)m_pOwner : NULL; }

	/*
	* Reset
	*/
	void Reset( void );

	/*
	* Remove All Anims
	* @param PLAY_ANIM playanim: remove everything except these kind of anims.
	*/
	void RemoveAllAnims( void );
	void RemoveAllAnims( PLAY_ANIM playanim );

	/*
	* Manage Animation List
	*/
	void ManageAnimList( void );

	/*
	* Manage Current Anim
	*/
	void ManageCurrentAnim( std::list<entanim_s>::iterator iterAnim );

	/*
	* Add Animation
	* @param BOOL fLooping: Loop the animation
	* @param ANIMTYPE eAnimType: Play it as a fullbody, upper+gait, or gait-only animation
	* @param float flTime: Time we're going to play this animation
	* @param int iNum: Number of animations we're going to add
	* @return std::list<entanim_s>::iterator: Animation we added
	*/
	std::list<entanim_s>::iterator AddAnimation( BOOL fLooping, ANIMTYPE eAnimType, float flTime, int iNum, ... );

	/*
	* Set Priority
	* @param PRIORITY_ANIMS priority: Priority we're setting this animation to
	* @param entanim_s *pAnim: Animation we're setting it for
	*/
	void SetPriority( PRIORITY_ANIMS priority, std::list<entanim_s>::iterator iterAnim );

	/*
	* Remove Animation
	* @param entanim_s *pAnim: Animation we're removing
	* @return std::list<entanim_s>::iterator: Animation after the deleted
	*/
	std::list<entanim_s>::iterator RemoveAnimation( std::list<entanim_s>::iterator iterAnim );

	/*
	* Get Current Animation
	* @return std::list<entanim_s>::iterator: Current Animation
	*/
	std::list<entanim_s>::iterator GetCurrentAnim( void );
		
	/*
	* Set Animation in PlayAnimation in player.cpp
	*/
	int SetAnimation( std::list<entanim_s>::iterator iterAnim );

	/*
	* Set Activity
	*/
	void SetActivity( Activity &Activity );

	/*
	* Init Anim
	*/
	void InitAnim( std::list<entanim_s>::iterator iterAnim );

private:
	std::list<entanim_s> m_Animations;
	CBaseAnimating		*m_pOwner;
};
#endif

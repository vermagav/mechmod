/*
* MechMod Saber Handler
* @version: 5/18/04
* @author: Joseph 'Pcjoe' Florencio
*/
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"weapons.h"
#include	"soundent.h"
#include	"gamerules.h"
#include	"player.h"
#include	"animation.h"
#include	"mmweapons.h"

#ifndef CLIENT_DLL
bool		CSaberPhys::m_blIsValid[32];
#endif

inline int FNullEnt( CBaseEntity *ent ) { return (ent == NULL) || FNullEnt( ent->edict() ); }


#ifndef CLIENT_DLL
CSaberPhys g_SaberPhys;
#endif

using namespace std;

// Constructor/Deconstructor
CSaber::CSaber()
{	
#ifndef CLIENT_DLL
	Reset();
	m_Attribute.SetSaber(this);
#endif
}

CSaber::~CSaber()
{	
#ifndef CLIENT_DLL
	Reset();
#endif
}

#ifndef CLIENT_DLL
/*
* Reset
*/
void CSaber::Reset( void )
{	m_flSaberDelay = 0;
	m_SaberData.clear();
	m_eSaberStatus = SABER_IDLE;
	ResetAttributes();
	SetOwner(NULL);
}

/*
* Has melee weapon selected
*/
BOOL CSaber::MeleeWpSel( void )
{	if(!m_pOwner)
		return FALSE;
	CBasePlayerWeapon *pWeapon = ((CBasePlayerWeapon*)m_pOwner->m_pActiveItem);
	if(pWeapon)
	{	if(pWeapon->MMWeapon())
		{	CMMWeapon *pMMWeapon = (CMMWeapon*)pWeapon;
			return pMMWeapon->MeleeWeapon();
		}
	}
	return FALSE;
}


/*
* Can Use Saber
*/
BOOL CSaber::CanUseSaber( void )
{	if(!MeleeWpSel())
	{	return FALSE;
	}

	if(m_pOwner->m_fBoost || m_pOwner->m_fImpact)
	{	if(!m_SaberData.empty())
		{	m_SaberData.clear();
		}
		m_Attribute.ResetQueuedList();
		return FALSE;
	}
	return TRUE;
}

/*
* Holding Attack
*/
BOOL CSaber::HoldingAttack( void )
{	if(!m_pOwner)
		return FALSE;
	return ((m_pOwner->pev->button & IN_ATTACK) && !(m_pOwner->pev->button & IN_ATTACK2));
}

/*
* Holding Defend
*/
BOOL CSaber::HoldingDefend( void )
{	if(!m_pOwner)
		return FALSE;
	return (m_pOwner->pev->button & IN_ATTACK2);
}

/*
* Set Status ( idle, attack, defend )
*/
int CSaber::SetStatus ( void )
{	if(m_eSaberStatus == SABER_ATTACK)
	{	list<saber_move_s>::iterator iterSaber = GetCurrentSwing();
		if(iterSaber != NULL)
		{	// Just starting swing, can remove and switch to blocking
			if(!iterSaber->fInit)
			{	if(HoldingDefend())
				{	if(m_eSaberStatus != SABER_DEFEND)
						StartDefend();
					return int(m_eSaberStatus = SABER_DEFEND);
				}
			}
		}
	}
	// We have saber moves but not in attack mode
	if(!SwingsComplete())
	{	if(m_eSaberStatus == SABER_DEFEND)
			StopDefend();
		return int(m_eSaberStatus = SABER_ATTACK);
	}

	// Change to saber defend
	if(HoldingDefend())
	{	if(m_eSaberStatus != SABER_DEFEND)
			StartDefend();
		return int(m_eSaberStatus = SABER_DEFEND);
	}

	// Idle saber
	if(m_eSaberStatus == SABER_DEFEND)
		StopDefend();
	// Dont idle if we still have moves
	if(m_SaberData.empty())
	{	if(m_eSaberStatus != SABER_IDLE)
			StartIdle();
	}
	else
	{	return int(m_eSaberStatus = SABER_ATTACK);
	}
	return int(m_eSaberStatus = SABER_IDLE);
}

/*
* Manage Saber
*/
void CSaber::ManageSaber( void )
{	if(!CanUseSaber())
		return;

	// Remove oudated swings ( old stalls, etc )
	CompleteOldSwings();
	RemoveCompletedStalls();
	
	// Handle blocking
	if(SetStatus() == SABER_DEFEND)
	{	HandleDefend();
	}
	else
	{	// Delay till next swing
		if(m_flSaberDelay < gpGlobals->time)
		{	// Dont load swings when you're already over the max
			int iMaxSwings = m_Attribute.GetGlobalInt( "maxswings" );
			if(!iMaxSwings || (m_SaberData.size() < iMaxSwings))
			{	// Update data from script
				UpdateFromScript();

				// Update saber move
				UpdateSaberData();

				// Remove Completed Stalls
				ManageStall(GetCurrentSwing());
				RemoveCompletedStalls();
			}
			// Max hit, erase moves and delay
			else
			{	m_Attribute.ResetQueuedList();
				m_SaberData.clear();
				m_flSaberDelay = gpGlobals->time + m_Attribute.GetGlobalFloat( "finishswingdelay" );
			}
		}
		// remove outdated swings
		RemoveOutdatedSwings();

		// Reset status
		SetStatus();

	//	ALERT(at_console,"Move count - %i\n", m_SaberData.size());

		// Get current swing
		list<saber_move_s>::iterator iterSwing = GetCurrentSwing();
		if(iterSwing == NULL)
			return;

		// Initilize swing
		if(iterSwing != NULL && !iterSwing->fInit)
		{	InitSwing(iterSwing);
		}
	}
}

/*
* Remove outdated swings
*/
void CSaber::CompleteOldSwings( void )
{	if(!SwingsComplete())
	{	list<saber_move_s>::iterator iterSwing = GetCurrentSwing();
		if(iterSwing != NULL && (iterSwing->fInit && (GetSwingPercent() > 1)))
		{	iterSwing->fComplete = TRUE;
			// If we're timing out a stall, restart all anims
			if(iterSwing->fStall)
			{//	ALERT(at_console,"Time out stall\n");
				m_SaberData.clear();
				m_Attribute.ResetQueuedList();
				m_flSaberDelay = gpGlobals->time + m_Attribute.GetGlobalFloat( "stallcomp_delay" );
			}
		}
	}
}

/*
* Remove outdated swings
*/
void CSaber::RemoveOutdatedSwings( void )
{	// remove swings, they're not combo linked
	if(SwingsComplete() && !m_SaberData.empty())
	{	m_SaberData.clear();
		// Reset animation as well
		m_pOwner->m_Anim.RemoveWepAnims();
	}
}

/*
* Update From Script
*/
void CSaber::UpdateFromScript( void )
{	BOOL fProcess = FALSE;
	// Holding onto valid direction
	if(GetSwingDirection())
	{	// We have no moves, try to add some
		if(m_SaberData.empty())
		{	fProcess = TRUE;	
		}
		else if(m_Attribute.GetGlobalInt( "maxswings" ) && (m_SaberData.size() >= m_Attribute.GetGlobalInt( "maxswings" )))
		{	fProcess = FALSE;
		}
		else
		{	// continue over stalls
			list<saber_move_s>::iterator iterSaber = GetCurrentSwing();
			if(iterSaber != NULL && iterSaber->fStall)
			{	fProcess = TRUE;
			}
			// continue over complete moves
			else if(iterSaber == NULL && !m_SaberData.empty())
			{	list<saber_move_s>::iterator iterSaber = m_SaberData.begin();
				if(iterSaber != NULL && iterSaber->fComplete)
				{	fProcess = TRUE;
				}
			}
		}
	}

	// Add data from script
	if(fProcess)
	{	m_Attribute.UpdateDataFromScript();
	}
}

/*
* Manage Stall
*/
void CSaber::ManageStall( list<saber_move_s>::iterator iterSwing )
{	if(iterSwing != NULL && iterSwing->fStall)
	{	if(!HoldingAttack() || (iterSwing != m_SaberData.begin()))
		{	iterSwing->fComplete = TRUE;
		}
	}
}

/*
* Remove Completed Stalls
*/
void CSaber::RemoveCompletedStalls( void )
{	list<saber_move_s>::iterator iterSwing = m_SaberData.begin();
	while(iterSwing != m_SaberData.end())
	{	if(iterSwing->fStall && iterSwing->fComplete)
		{	iterSwing = m_SaberData.erase(iterSwing);
			continue;
		}
		iterSwing++;
	}
}

/*
* Update Saber Data
*/
void CSaber::UpdateSaberData( void )
{	// reset queue list if player isnt holding onto primary fire on new swing
	if(SwingsComplete())
	{	if(!HoldingAttack())
		{	m_Attribute.ResetQueuedList();
			if(!m_SaberData.empty())
				m_SaberData.clear();
		}
	}
	// Setup next move
	while(m_Attribute.UpdateSaber())
	{	m_SaberData.push_front(m_Attribute.GetNextSaberData());
	}
}

/*
* Get Swing Direction
*/
int CSaber::GetSwingDirection( void )
{	// Attack is no held down, no direction
	if(!HoldingAttack())
	{	return 0;
	}
	BOOL fUp = m_pOwner->pev->button & IN_FORWARD;
	BOOL fDown = m_pOwner->pev->button & IN_BACK;
	BOOL fLeft = ((m_pOwner->pev->button & IN_LEFT) || (m_pOwner->pev->button & IN_MOVELEFT));
	BOOL fRight = ((m_pOwner->pev->button & IN_RIGHT) || (m_pOwner->pev->button & IN_MOVERIGHT));

	// Diaganols
	if(fUp && fRight)
		return SBR_D_UR;
	else if(fUp && fLeft)
		return SBR_D_UL;
	else if(fDown && fRight)
		return SBR_D_DR;
	else if(fDown && fLeft)
		return SBR_D_DL;
	// Regular directions
	else if(fLeft)
		return SBR_D_L;
	else if(fRight)
		return SBR_D_R;
	else if(fDown)
		return SBR_D_D;
	else if(fUp)
		return SBR_D_U;

	return 0;
}

string CSaber::GetSwingDirectionString( void )
{	int iDirection = GetSwingDirection();
	if(!iDirection)
	{	return string("");
	}

	switch(iDirection)
	{
	case SBR_D_U:
		{	return string("up");
		}
	case SBR_D_UR:
		{	return string("upperright");
		}
	case SBR_D_R:
		{	return string("right");
		}
	case SBR_D_DR:
		{	return string("lowerright");
		}
	case SBR_D_D:
		{	return string("down");
		}
	case SBR_D_DL:
		{	return string("lowerleft");
		}
	case SBR_D_L:
		{	return string("left");
		}
	case SBR_D_UL:
		{	return string("upperleft");
		}
	}
	return string("");
}

/*
* Get Opposite Direction
*/
int CSaber::GetOppositeDirection( int iSwingDirection )
{	iSwingDirection += SBR_D_UL/2;
	if(iSwingDirection > SBR_D_UL)
		iSwingDirection -= SBR_D_UL;
	return iSwingDirection;
}

/*
* Start Idle
*/
void CSaber::StartIdle( void )
{	m_SaberData.clear();	// Clear all moves
	m_pOwner->m_Anim.RemoveAllAnims(); // remove animations
//	m_pOwner->m_Anim.AddAnimation( TRUE, ANIMTYPE_UPPERONLY, gpGlobals->time, 1, m_Attribute.GetGlobalString("idle_anim").c_str() );
}

/*
* Start Defend
*/
void CSaber::StartDefend( void )
{	m_SaberData.clear();	// Clear all moves
	m_pOwner->m_Anim.AddAnimation( TRUE, ANIMTYPE_UPPERONLY, gpGlobals->time, 1, m_Attribute.GetGlobalString("blocking_anim").c_str() );
}

/*
* Stop Defend
*/
void CSaber::StopDefend( void )
{	m_pOwner->m_Anim.RemoveWepAnims();
}

/*
* Handle Defend
*/
void CSaber::HandleDefend( void )
{
}

/*
* Get Swing Percent
*/
float CSaber::GetSwingPercent( void )
{	list<saber_move_s>::iterator iterSwing = GetCurrentSwing();
	if(iterSwing == NULL)
		return 0;
	return ((gpGlobals->time-iterSwing->flStartTime)/(iterSwing->flEstEndTime-iterSwing->flStartTime));
}

/*
* Set Swing Percent
*/
void CSaber::SetSwingPercent( float flPercent )
{	list<saber_move_s>::iterator iterSwing = GetCurrentSwing();
	if(iterSwing == NULL)
		return;
	if(iterSwing->fStall)
		return;
	if(m_pOwner->LookupSequence(iterSwing->strAnim.c_str()) == m_pOwner->pev->sequence)
	{	m_pOwner->pev->frame = 256 * flPercent;
	}
}

/*
* Get Current Swing
*/
list<saber_move_s>::iterator CSaber::GetCurrentSwing( void )
{	if(m_SaberData.empty())
		return NULL;

	list<saber_move_s>::iterator current = m_SaberData.begin();
	list<saber_move_s>::iterator end = m_SaberData.end();

	while(current != end && !current->fComplete)
	{	list<saber_move_s>::iterator next = current;
		next++;
		if((next == end) ? true : next->fComplete)
		{	return current;
		}
		current++;
	}
	return NULL;
}

/*
* Init Swing
*/
void CSaber::InitSwing( list<saber_move_s>::iterator iterSaber )
{	if(iterSaber->fInit)
	{	return;
	}

	// Set basic varibles
	iterSaber->fInit = TRUE;
	iterSaber->fComplete = FALSE;
	iterSaber->flStartTime = gpGlobals->time;

	/*
	* Estimate ending time
	*/
	if(iterSaber->fStall)
	{	// Already has "stall" time set, just add current time
//		ALERT(at_console,"Stall time - %f\n", iterSaber->flEstEndTime);
		iterSaber->flEstEndTime += gpGlobals->time;
//		ALERT(at_console,"Stall Move\n");
	}
	else
	{	int iOldSequence = m_pOwner->pev->sequence;
		// Set saber sequence
		SetReferenceModel();
		m_pOwner->pev->sequence = m_pOwner->LookupSequence( iterSaber->strAnim.c_str() );
		if(m_pOwner->pev->sequence != -1)
		{	void *pmodel = GET_MODEL_PTR( ENT(m_pOwner->pev) );
			float flFrameRate=0, flGarbage=0;
			// Get frametime
			GetSequenceInfo( pmodel, m_pOwner->pev, &flFrameRate, &flGarbage );
			// Set estimated end time
		//	iterSaber->flEstEndTime = gpGlobals->time + 256/(flFrameRate*m_pOwner->pev->framerate);
			float flSwingTime = (255/(flFrameRate*m_pOwner->pev->framerate))*2;
		//	float flSwingTime = 0.4;
			iterSaber->flEstEndTime = gpGlobals->time + flSwingTime;
		}
		else
		{	// Animation doesnt exist, set any time ( will exit below when addanimation fails )
//			ALERT(at_console,"Missing animation %s for saber melee!\n", iterSaber->strAnim.c_str());
			RestoreModel();
			m_SaberData.clear();
			m_Attribute.ResetQueuedList();
			return;
		}
		// Set sequence back to previous
		m_pOwner->pev->sequence = iOldSequence;
	}
	RestoreModel();
	/*
	* end of estimating ending time
	*/

	// Add animation
	m_pOwner->m_Anim.RemoveAllAnims();
	list<entanim_s>::iterator iterAnim =
		m_pOwner->m_Anim.AddAnimation( iterSaber->fStall, ANIMTYPE_UPPERONLY, gpGlobals->time, 1, iterSaber->strAnim.c_str() );
	// Error message, cant add animation correctly
	if(iterAnim == NULL)
	{//	ALERT(at_console,"Missing animation %s for saber melee!\n", iterSaber->strAnim.c_str());
		m_SaberData.clear();
		m_Attribute.ResetQueuedList();
		return;
	}
	// Set as saber type anim
	iterAnim->fSaber = TRUE;

	// start playing animations immediatly
	m_pOwner->PlayAnimation();

	// Set to last frame if we're playing a stall
	if(iterSaber->fStall && iterSaber->fStallFreezeLastFrame)
	{	m_pOwner->pev->frame = 255;
	}

	// added by jason play sound
	if(!iterSaber->fStall)
	{	EMIT_SOUND( ENT( m_pOwner->pev ), CHAN_WEAPON, m_Attribute.GetGlobalString("swingsound").c_str(), 1, ATTN_NORM );
		// Pcjoe: Reset hit data
		for( int i = 0; i < 32; i ++ )
		{	g_SaberPhys.m_blWhoHit[m_pOwner->entindex()-1][i] = false;
		}
	}
}

/*
* Swings Complete
*/
BOOL CSaber::SwingsComplete( void )
{	if(GetCurrentSwing() == NULL)
	{	return TRUE;
	}
	return FALSE;
}

/*
* Get Studio Header
*/
studiohdr_t *CSaber::GetStudioHeader( void )
{	if(!m_pOwner)
		return NULL;

	// Get model pointer
	void *pModel = GET_MODEL_PTR( ENT(m_pOwner->pev) );
	if(!pModel)
		return NULL;
	// Get model header
	return (studiohdr_t *)pModel;
}

/*
* Get Bone Origin
*/
Vector CSaber::GetBoneOrigin( char *pName )
{	mstudiobone_t *pbones = NULL;
	studiohdr_t *pstudiohdr = GetStudioHeader();
	if(!pstudiohdr)
		return Vector(0,0,0);

	pbones = (mstudiobone_t *)((byte *)pstudiohdr + pstudiohdr->boneindex);

	// find bone with a similiar name and return it's origin
	for (int i = 0; i < pstudiohdr->numbones; i++) 
	{	if(!stricmp(pName, pbones[i].name))
		{	Vector vecOrigin, vecAngles;
			m_pOwner->GetBonePosition(i, vecOrigin, vecAngles);
			return vecOrigin;
		}
	}

	return Vector(0,0,0);
}

/*
* Set Reference Model
*/
void CSaber::SetReferenceModel( void )
{	if(!m_pOwner)
		return;

	// Get model name
	string strRefModel = m_Attribute.GetGlobalString("refmodel");
	if(strRefModel == "")
	{	return;
	}
	// Append model name onto directory name
	string strModel = "models/melee/" + strRefModel;

	// Set model
	UTIL_SetModel( strModel.c_str(), m_pOwner->pev );

	// Set appropiate sequence
	std::list<entanim_s>::iterator iterAnim = m_pOwner->m_Anim.GetCurrentAnim();
	if(iterAnim != NULL)
	{	m_pOwner->pev->sequence = m_pOwner->LookupSequence( iterAnim->strAnim[iterAnim->iCurAnim].c_str() );
	}
}

/*
* Restore Model ( from reference model )
*/
void CSaber::RestoreModel( void )
{	if(!m_pOwner)
		return;
	m_pOwner->m_pClass->SetModel();

	// Set appropiate sequence
	std::list<entanim_s>::iterator iterAnim = m_pOwner->m_Anim.GetCurrentAnim();
	if(iterAnim != NULL)
	{	m_pOwner->pev->sequence = m_pOwner->LookupSequence( iterAnim->strAnim[iterAnim->iCurAnim].c_str() );
	}
}

/*
* Run Collision
*/
BOOL CSaber::RunCollision( void )
{	if(!CanUseSaber() || SwingsComplete())
		return FALSE;
	return TRUE;
}

/*
* Collision
*/
void CSaber::Collision( void )
{	// Jason, it's all you ^_^
}

// added by jason
void CSaberPhys::InitPhysics( void )
{
	neV3 neGrav;

	neGrav.Set( 0, 0, 0 );	// we manually update our objects

	neSimulatorSizeInfo neSizeInfo;
	
	neSizeInfo.rigidBodiesCount = MAX_RIGIDBODIES;
	neSizeInfo.animatedBodiesCount = 0;
	neSizeInfo.geometriesCount = TOTAL_BODIES;
	neSizeInfo.overlappedPairsCount = MAX_RIGIDBODIES * ( MAX_RIGIDBODIES - 1 ) / 2;

	neSizeInfo.rigidParticleCount = 0;
	neSizeInfo.constraintsCount = 0;
	neSizeInfo.terrainNodesStartCount = 0;

	m_pSim = neSimulator::CreateSimulator( neSizeInfo, &m_neAllocator, &neGrav );

	//m_pSim->SetCollisionCallback( CollisionCallback );
	m_pSim->SetCustomCDRB2RBCallback( CollisionCallback );

	for( int i = 0; i <= 32; i ++ )
	{
		m_blWasSwinging[i] = false;
		if(i>=32)
			break;

		for( int j = 0; j < 32; j ++ )
		{
			m_blWhoHit[i][j] = false;
		}
	}

	//TmpPhys( );

	for( i = 0; i < 64; i++ )
	{
		m_pRigidBody[i] = NULL;
	}
}

void CSaberPhys::TmpPhys( void )
{
	m_pRigidBody[64] = m_pSim->CreateRigidBody( );
	neRigidBody *pBody = m_pRigidBody[64];
	neGeometry *pGeom = pBody->AddGeometry( );

	pGeom->SetBoxSize( 100, 100, 100 );
	pBody->UpdateBoundingInfo( );
	pBody->SetMass( 1 );
	pBody->SetInertiaTensor( neBoxInertiaTensor( 10, 10, 10, 1 ) );
	
	
//	pBody->SetCollisionID( 1 );
	neT3 bbox;
	bbox.SetIdentity( );

	bbox.rot[0].n.X = 10;			// not actual rotations jsut bbox dimensions i belive
	bbox.rot[1].n.Y = 10;
	bbox.rot[2].n.Z = 10;
	bbox.rot.SetTranspose( bbox.rot * 10 );
	pBody->UseCustomCollisionDetection( true, &bbox, 10 );
	

	pBody->SetUserData( 8 );

	neV3 vPos;
	vPos.Set( 0, 0, 0 );

	pBody->SetPos( vPos );
	pBody->GravityEnable( false );

//	m_neBodies.push_back( pBody );
}

void CSaberPhys::CreatePlayerBox( CBasePlayer *pPlayer )
{
	int iIndex = pPlayer->entindex( );
	neGeometry *pGeom;
	m_blIsValid[iIndex - 1] = true;

	if( !m_pRigidBody[iIndex -1 ] )
	{
		m_pRigidBody[iIndex-1] = m_pSim->CreateRigidBody( );
		pGeom = m_pRigidBody[iIndex-1]->AddGeometry( );
	}
	else
	{
		return;
	}

	pGeom->SetBoxSize( pPlayer->pev->maxs.x, pPlayer->pev->maxs.y, pPlayer->pev->maxs.z );
	m_pRigidBody[iIndex-1]->UpdateBoundingInfo( );
	m_pRigidBody[iIndex-1]->SetMass( 1 );
	m_pRigidBody[iIndex-1]->SetInertiaTensor( neBoxInertiaTensor( pPlayer->pev->maxs.x, pPlayer->pev->maxs.y, pPlayer->pev->maxs.z, 1 ) );
	
	
//	m_pRigidBody[iIndex-1]->SetCollisionID( 1 );
	neT3 bbox;
	bbox.SetIdentity( );
	bbox.rot[0].n.X = pPlayer->pev->maxs.x;			// not actual rotations jsut bbox dimensions i belive
	bbox.rot[1].n.Y = pPlayer->pev->maxs.y;
	bbox.rot[2].n.Z = pPlayer->pev->maxs.z;
	bbox.rot.SetTranspose( bbox.rot * 10 );
	m_pRigidBody[iIndex-1]->UseCustomCollisionDetection( true, &bbox, 16 );
	

	m_pRigidBody[iIndex-1]->SetUserData( pPlayer->entindex() );

	neV3 vPos;
	vPos.Set( pPlayer->pev->origin.x, pPlayer->pev->origin.y, pPlayer->pev->origin.z );

	m_pRigidBody[iIndex-1]->SetPos( vPos );
	m_pRigidBody[iIndex-1]->GravityEnable( false );

	//m_neBodies.push_back( pBody );

	// Clear out players attack list
	for( int i = 0; i < 32; i ++ )
		m_blWhoHit[ pPlayer->entindex()-1 ][i] = false;


//	TmpPhys( );
}



void CSaberPhys::CreateSaberBox( CBasePlayer *pPlayer )
{
	// create a saber based on the players info
	int iIndex = pPlayer->entindex( ) + 32;
	neGeometry *pGeom;

//	neRigidBody *pBody = m_pRigidBody[iIndex*2 + 1];
	if( !m_pRigidBody[iIndex] )
	{
		m_pRigidBody[iIndex] = m_pSim->CreateRigidBody( );
		pGeom = m_pRigidBody[iIndex]->AddGeometry( );
	}
	else
	{
		return;
	}

	// shift by 6 so we dont confuse player 2's bbox as player 1's weapon
	int iWeapon = pPlayer->entindex() + 32;	// shift left if player is 32 will be 64 easy check
	m_pRigidBody[iIndex]->SetUserData( iWeapon );

	//pPlayer->m_Saber.SetReferenceModel();

	pPlayer->pev->sequence = 0;
	pPlayer->m_Saber.SetReferenceModel( );
	vec3_t vHand = pPlayer->m_Saber.GetBoneOrigin( "saber_0" );	// start of saber
	vec3_t vOrg = pPlayer->m_Saber.GetBoneOrigin( "saber_1" );		// end of saber
	pPlayer->m_Saber.RestoreModel( );

	// set saber dimensions	 
	vec3_t vSaber;
	neV3 vPos;
	
	vSaber = vHand - vOrg;
	vec3_t vTmp = vSaber;
	vSaber = vSaber/2 + vHand;

	vPos.Set( vSaber.x, vSaber.y, vSaber.z );
	
	m_pRigidBody[iIndex]->SetPos( vPos );
	vSaber = vTmp;

	float flLength = vSaber.Length( );
	vSaber.x = 1;
	vSaber.y = 1;
	vSaber.z = flLength/2;	// center of box

	pGeom->SetBoxSize( vSaber.x, vSaber.y, vSaber.z );
	m_pRigidBody[iIndex]->UpdateBoundingInfo( );

	m_pRigidBody[iIndex]->SetInertiaTensor( neBoxInertiaTensor( vSaber.x, vSaber.y, vSaber.z, 1 ) );
	

	vSaber = vHand - vOrg;
	vSaber = vSaber/2;
	vPos.Set( vSaber.x, vSaber.y, vSaber.z );

	m_pRigidBody[iIndex]->SetPos( vPos );

//	m_pRigidBody[iIndex]->SetCollisionID( 1 );

	neT3 bbox;
	bbox.SetIdentity( );

	bbox.rot[0].n.X = vSaber.x;			// not actual rotations jsut bbox dimensions i belive
	bbox.rot[1].n.Y = vSaber.y;
	bbox.rot[2].n.Z = vSaber.z;
	bbox.rot.SetTranspose( bbox.rot * 10 );
	m_pRigidBody[iIndex]->UseCustomCollisionDetection( true, &bbox, vSaber.z );
	

	vec3_t vRot;

	vRot = vHand - vOrg;
	vRot = vRot.Normalize( );
	vPos.Set( vRot.x, vRot.y, vRot.z );
	neM3 mAngle;

	//angle matrice
	mAngle.SetIdentity( );

	mAngle.RotateXYZ( vPos );

	m_pRigidBody[iIndex]->SetRotation( mAngle );

	m_pRigidBody[iIndex]->GravityEnable( false );

//	m_neBodies.push_back( pBody );

}

void CSaberPhys::RemovePlayerBox( CBasePlayer *pPlayer )
{
/*	std::list<neRigidBody *>::iterator neBody;

	ForEachIn( neBody, m_neBodies )
	{
		if( *neBody )
		{
			if( (*neBody)->GetUserData() == pPlayer->entindex() )
			{
				// remove object
				m_pSim->FreeRigidBody( (*neBody ) );
				neBody = m_neBodies.erase( neBody );
				break;
			}
			// also remove saber box
			if( (*neBody)->GetUserData() == pPlayer->entindex() << 6 )
			{
				// remove object
				m_pSim->FreeRigidBody( (*neBody ) );
				neBody = m_neBodies.erase( neBody );
				break;
			}

		}
	}*/

	int iIndex = pPlayer->entindex( ) - 1;

	m_blIsValid[iIndex] = false;

/*	if( m_pRigidBody[iIndex] )
	{
		m_pSim->FreeRigidBody( m_pRigidBody[iIndex] );
		m_pRigidBody[iIndex] = NULL;
	}
	if( m_pRigidBody[iIndex+33] )
	{
		m_pSim->FreeRigidBody( m_pRigidBody[iIndex+33] );
		m_pRigidBody[iIndex+33] = NULL;
	}*/
/*	neV3 vPos;
	vPos.Set( 4096, 4096, 4096 );
	m_pRigidBody[iIndex]->SetPos( vPos );
	m_pRigidBody[iIndex+33]->SetPos( vPos );*/
}

void CSaberPhys::UpdatePlayers( CBasePlayer *pPlayer )
{
	if( !pPlayer )
	{
		return;
	}

//	ALERT( at_console, "1\n" );
	int iIndex = pPlayer->entindex();

	if( pPlayer->m_Saber.GetStatus() == SABER_ATTACK && !m_blWasSwinging[iIndex] )
	{
		m_blWasSwinging[iIndex] = true;
	}

	// modifed by jason to check for each swing part
	// Edited by Pcjoe
/*	list<saber_move_s>::iterator iterSwing = pPlayer->m_Saber.GetCurrentSwing();
	if( iterSwing != NULL )
	{
		if( iterSwing->fStall && m_blWasSwinging[iIndex-1]  )
		{
			m_blWasSwinging[iIndex-1] = false;
		//	ALERT( at_console, "Did we finish last swing\n" );

			// Clear out players attack list
			for( int i = 0; i < 32; i ++ )
				m_blWhoHit[iIndex][i] = false;
		}
	}
	// assume iterSwing == NULL
	else if( m_blWasSwinging[iIndex] )*/
	if( pPlayer->m_Saber.SwingsComplete() && m_blWasSwinging[iIndex] )
	{
		m_blWasSwinging[iIndex] = false;
	//	ALERT( at_console, "FallBack Clear\n" );

		// Clear out players attack list
		for( int i = 0; i < 32; i ++ )
			m_blWhoHit[iIndex-1][i] = false;
	}

	for( int i = 0; i < 64; i++ )
	{

		if( !m_pRigidBody[i] )
		{
			continue;
		}
		if( (m_pRigidBody[i])->GetUserData( ) == iIndex - 1 )
		{
			neM3 vAng; vAng.SetIdentity( );

			(m_pRigidBody[i])->SetRotation( vAng );

			neV3 vPos;
			if( pPlayer->IsAlive( ) )
			{
				vPos.Set( pPlayer->pev->origin.x, pPlayer->pev->origin.y, pPlayer->pev->origin.z );
			}
			else
			{
				vPos.Set( 4096, 4096, 4096 );
			}
			(m_pRigidBody[i])->SetPos( vPos );

			//ALERT( at_console, "Box org: %f, %f, %f\n", (*neBody)->GetPos( ).n.X, (*neBody)->GetPos( ).n.Y, (*neBody)->GetPos( ).n.Z );
		}
		else if( (m_pRigidBody[i])->GetUserData( ) == iIndex + 32 )
		{
			// saber adjust its info
			
			neM3 mAngle;
			neV3 vPos;
			vec3_t vSaber1, vSaber2, vOrg;

			//pPlayer->pev->sequence = 0;
			if( pPlayer->IsAlive( ) && pPlayer->IsPlayer( ) )
			{
				if( pPlayer->m_Saber.GetStatus( ) == SABER_ATTACK )
				{
					pPlayer->m_Saber.SetReferenceModel( );
					vSaber1 = pPlayer->m_Saber.GetBoneOrigin( "saber_0" );
					vSaber2 = pPlayer->m_Saber.GetBoneOrigin( "saber_1" );
					
					pPlayer->m_Saber.RestoreModel();
				}
				else
				{
					vSaber1 = vSaber2 = pPlayer->pev->origin;
				}
			}
			else
			{
				vSaber1 = vSaber2 = pPlayer->pev->origin;
			}

		//	ALERT( at_console, "Org: %f, %f, %f\n", vSaber1.x, vSaber1.y, vSaber1.z );
		//	ALERT( at_console, "Player Org: %f, %f, %f\n", pPlayer->pev->origin.x, pPlayer->pev->origin.y, pPlayer->pev->origin.z );

			vOrg = vSaber2 - vSaber1;
			vOrg = vOrg/2;
			vec3_t vTmp = vOrg.Normalize();
			neV3 vPos1;
			vPos1.Set( vTmp.x, vTmp.y, vTmp.z );
			vOrg = vOrg + vSaber1;
			vPos.Set( vOrg.x, vOrg.y, vOrg.z );

		
			mAngle.SetIdentity( );
			mAngle.RotateXYZ( vPos1 );

			(m_pRigidBody[i])->SetRotation( mAngle );
			(m_pRigidBody[i])->SetPos( vPos );

			//ALERT( at_console, "Saber org: %f, %f, %f\n", (*neBody)->GetPos( ).n.X, (*neBody)->GetPos( ).n.Y, (*neBody)->GetPos( ).n.Z );
		}
	}

//	ALERT( at_console, "3\n" );
}

// call back didnt like being part of the class
int CSaberPhys::CollisionCallback( neRigidBody * bodyA, neRigidBody * bodyB, neCustomCDInfo & cdInfo )//( neCollisionInfo &collisionInfo )
{
	//ALERT( at_console, "Call Back Called\n" );
	// check the user data for both objects
#ifndef CLIENT_DLL
//	ALERT( at_console, "Start CallBack\n" );
	if( bodyA == NULL || bodyB == NULL )
	{
	//	ALERT( at_console, "Exit Naturally One Rigid body was null\n" );
		return 1;
	}
 
	// going line by line
//	ALERT( at_console, "1\n" );
	neRigidBody *pBodyA = bodyA;//(neRigidBody *)collisionInfo.bodyA;
//	ALERT( at_console, "2\n" );
	neRigidBody *pBodyB = bodyB;//(neRigidBody *)collisionInfo.bodyB;

//	ALERT( at_console, "3\n" );
	int iDataA = pBodyA->GetUserData( );
//	ALERT( at_console, "4\n" );
	int iDataB = pBodyB->GetUserData( );

	//ALERT( at_console, "5\n" );

	if( ( iDataA <= 32 && iDataB <= 32 ) || ( iDataA > 32 && iDataB > 32 ) )
	{
	//	ALERT( at_console, "Exit Naturally 2 sabers or 2 players\n" );
		return 1;
	}

//	ALERT( at_console, "6\n" );

	int iDataAShift = -29, iDataBShift = -29;


	CBasePlayer *pPly1 = ( CBasePlayer * ) UTIL_EntityByIndex( iDataA );
	//ALERT( at_console, "7\n" );
	CBasePlayer *pPly2 = ( CBasePlayer * ) UTIL_EntityByIndex( iDataB );
	//ALERT( at_console, "8\n" );
	if( iDataA > 32 )
	{
		iDataAShift = iDataA -32;//>> 6;
		pPly1 = ( CBasePlayer * ) UTIL_EntityByIndex( iDataAShift );
	}
	if( iDataB > 32 )
	{
		iDataBShift = iDataB -32;//>> 6;
		pPly2 = ( CBasePlayer * ) UTIL_EntityByIndex( iDataBShift );
	}

	if( iDataAShift == iDataB || iDataBShift == iDataA )
	{
		//ALERT( at_console, "SABER COLLISION %i, %i\n", iDataAShift, iDataB );
	//	ALERT( at_console, "Exit Naturally Player And Saber\n" );
		return 1;	// player saber colliding with self
	}
	

	if( !FNullEnt(pPly1) || !FNullEnt(pPly2) )
	{
		bool blOk = false;
		if( !FNullEnt(pPly1) )
		{
			if( pPly1->m_Saber.GetStatus() == SABER_ATTACK )
				blOk = true;
		}

		if( !FNullEnt(pPly2) )
		{
			if( pPly2->m_Saber.GetStatus() == SABER_ATTACK )
				blOk = true;
		}
		if( blOk )
		{
			
		//	ALERT( at_console, "PLAYER COLLISION\n" );
			// saber and player colliding find saber and player
			neRigidBody *pPlayer, *pSaber;


			int iIndex, iAttacker;
			
			// iDataA is a saber
			if( iDataA > 32 )		
			{
				pSaber = pBodyA;		// saber of attacking player
				pPlayer = pBodyB;		// body of dmged player
				iIndex = iDataB;		// index of player
				iAttacker = iDataAShift;// attackers index	
			}
			else
			{
				pSaber = pBodyB;		// saber of attacking player
				pPlayer = pBodyA;		// body of dmged player
				iIndex = iDataA;		// index of player
				iAttacker = iDataBShift;// attackers index
				
			}

			if( m_blIsValid[iIndex-1] && m_blIsValid[iAttacker-1] )
			{
			}
			else
			{
				return 1;
			}
			
			pPly1 = ( CBasePlayer * )UTIL_EntityByIndex( iAttacker );
			pPly2 = ( CBasePlayer * )UTIL_EntityByIndex( iIndex );

			if( pPly1 && pPly2 )
			{
				vec3_t vOrg = pPly1->pev->origin - pPly2->pev->origin;
				if( vOrg.Length( ) > 60.0f )
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}

			if( !pPly2->IsAlive( ) || !pPly1->IsAlive( ) )
			{
				return 1;
			}

	
			
			//	ALERT( at_console, "Attacking Player: %s, index: %i\n", STRING( pPly1->pev->netname ), iAttacker );
			if( pPly1->m_Saber.GetStatus( ) == SABER_ATTACK )
			{
			//	CBaseEntity *pTmp = pPly1;
			//	pPly1 = pPly2
			}
			else if( pPly2->m_Saber.GetStatus( ) == SABER_ATTACK )
			{
				CBasePlayer *pTmp = pPly1;
				pPly1 = pPly2;
				pPly2 = pTmp;

				int iTmp = iIndex;
				iIndex = iAttacker;
				iAttacker = iTmp;
			}
			else
			{
				return 1;
			}

		//	pPly1 = ( CBasePlayer * )UTIL_EntityByIndex( iAttacker );
		//	pPly2 = ( CBasePlayer * )UTIL_EntityByIndex( iIndex );
				//	ALERT( at_console, "Attack Succeded\n" );
				/*	ALERT( at_console, "Collison pt: %f, %f, %f\n", cdInfo.worldContactPointA.n.X,
				cdInfo.worldContactPointA.n.Y, cdInfo.worldContactPointA.n.Z );*/
				if( !g_SaberPhys.m_blWhoHit[iAttacker-1][iIndex-1] )
				{
					//	ALERT( at_console, "you got me\n" );
				//	ALERT( at_console, "Attacker: %i, Defender: %i\n", iAttacker, iIndex );
					
					float flDmg = 0;
					if( pPly1->m_Saber.GetCurrentSwing( ) != NULL )
					{
						flDmg = pPly1->m_Saber.GetCurrentSwing( )->flDmg;
					}
					else
					{
						return 1;
					}
					g_SaberPhys.m_blWhoHit[iAttacker-1][iIndex-1] = true;
					
					//	if( pPly2->IsAlive( ) && pPly2->IsPlayer( ) )
						if( pPly2->IsPlayer( ) && pPly1->IsPlayer( ) )
						{
							//	ALERT( at_console, "Damaged Player: %s, dmg: %f\n", STRING( pPly1->pev->netname ), flDmg );
							
							// added by grOOvy: 31/10/04
							EMIT_SOUND( ENT( pPly1->pev ), CHAN_WEAPON, "weapons/beamsword_hit.wav", 1, ATTN_NORM );
							if( pPly1->IsAlive( ) )
							{
//								ALERT( at_console, "Pre Dmg\n" );
								pPly2->TakeDamage( pPly1->pev, pPly1->pev, flDmg, DMG_SLASH );
							//	pPly1->TakeDamage( pPly2->pev, pPly2->pev, flDmg, DMG_SLASH );
//								ALERT( at_console, "Post Dmg\n" );
								// Pcjoe: Push Back
								pPly2->pev->velocity = (pPly2->pev->origin - pPly1->pev->origin).Normalize() * pPly1->m_Saber.m_Attribute.GetGlobalFloat("saberpush");
							}
						}
					
					}
//					ALERT( at_console,  "saber x:%f y:%f z:%f\n", pSaber->GetPos( ).n.X, pSaber->GetPos( ).n.Y, pSaber->GetPos( ).n.Z );
//				ALERT( at_console,  "bbox x:%f y:%f z:%f\n", pPlayer->GetPos( ).n.X, pPlayer->GetPos( ).n.Y, pPlayer->GetPos( ).n.Z );
//				ALERT( at_console,  "player x:%f y:%f z:%f\n", pPly1->pev->origin.x, pPly1->pev->origin.y, pPly1->pev->origin.z );
		}
	}
//	ALERT( at_console, "Exit Naturally\n" );
#endif

	return 1;
}

void CSaberPhys::UpdatePhysics( void )
{
	m_pSim->Advance( TIME_STEP );
}

void CSaberPhys::KillPhysics( void )
{
	//m_neBodies.clear();
	for( int i = 0; i < 65; i++ )
	{
		if( i < 32 )
		{
			m_blIsValid[i] = false;
		}
		if( m_pRigidBody[i] )
		{
			if( m_pSim )
			{
				m_pSim->FreeRigidBody( m_pRigidBody[i] );
			}
			m_pRigidBody[i] = NULL;
		}
	}

	if( m_pSim )
	{
		neSimulator::DestroySimulator( m_pSim );
	}
}
// end jason

/*
* Add Attribute
*/
void CSaber::AddAttribute( const char *pAttribute )
{	m_strAttributeNames.push_back( string( pAttribute ) );
	if(m_strCurrentAttribute == NULL)
		m_strCurrentAttribute = m_strAttributeNames.begin();
}

/*
* Reset Attributes
*/
void CSaber::ResetAttributes( void )
{
	m_strAttributeNames.clear();
	m_strCurrentAttribute = NULL;
}

/*
* Switch to next attribute
*/
void CSaber::NextAttribute( void )
{	if(m_strAttributeNames.empty())
		return;
	// Attribute isnt set, make it the first member
	if(m_strCurrentAttribute == NULL)
	{	m_strCurrentAttribute = m_strAttributeNames.begin();
		return;
	}

	// Go to next attribute
	m_strCurrentAttribute++;
	if(m_strCurrentAttribute == m_strAttributeNames.end())
	{	m_strCurrentAttribute = m_strAttributeNames.begin();
	}
}

/*
* Switch to previous attribute
*/
void CSaber::PreviousAttribute( void )
{	if(m_strAttributeNames.empty())
		return;
	// Attribute isnt set, make it the first member
	if(m_strCurrentAttribute == NULL)
	{	m_strCurrentAttribute = m_strAttributeNames.begin();
		return;
	}

	// Go to next attribute
	if(m_strCurrentAttribute == m_strAttributeNames.begin())
	{	m_strCurrentAttribute = m_strAttributeNames.end();
		m_strCurrentAttribute--;
		
	}
	else
	{	m_strCurrentAttribute--;
	}
}

/*
* Get Current Attribute
*/
string CSaber::GetCurrentAttribute( void )
{	if(m_strCurrentAttribute == NULL)
	{	return string("");
	}
	return string(m_strCurrentAttribute->c_str());
}

/*
* Can Use Move
*/
BOOL CSaber::CanUseMove( std::vector<std::string> strList )
{
	if(strList.empty() || m_SaberData.empty())
		return FALSE;

	vector<std::string>::reverse_iterator iterCmbMove = strList.rbegin();
	vector<std::string>::reverse_iterator iterCmbMoveEnd = strList.rend();
	list<saber_move_s>::iterator iterSaber = m_SaberData.begin();
	list<saber_move_s>::iterator iterSaberEnd = m_SaberData.end();

	// Check to see if the moves match up
	int iSameMoves = 0;
	while((iterCmbMove != iterCmbMoveEnd) && (iterSaber != iterSaberEnd))
	{	// Skip all "stalls"
		while((iterSaber != iterSaberEnd) && iterSaber->fStall)
		{	iterSaber++;
		}
		if(iterSaber == iterSaberEnd)
		{	break;
		}

		// Compare moves
		if(!iSameMoves) // First move, use current move to compare
		{	if((*iterCmbMove) == GetSwingDirectionString())
			{	iterCmbMove++;
				iSameMoves++;
			}
			else
			{	break;
			}
		}
		else if(iterSaber->strMoveName == (*iterCmbMove) || iterSaber->strCmbName == (*iterCmbMove))
		{	iterSaber++;
			iterCmbMove++;
			iSameMoves++;
		}
		else
		{	break;
		}
	}

	// Compare list size
	if(iSameMoves == strList.size())
		return TRUE;
	return FALSE;
}

/*
* Get Current Swing Direction
*/
string CSaber::GetCurSwingDirection( void )
{	if(m_SaberData.empty())
	{	return string("");
	}
	list<saber_move_s>::iterator iterSaber = GetCurrentSwing();
	if(iterSaber != NULL)
	{	return iterSaber->strMoveName;
	}
	return string("");
}
#endif
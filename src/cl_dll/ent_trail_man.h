/**********************************************************
*			ENTITY TRAIL MANAGER CODE
***********************************************************
*	Purpose: Enhanced entity trails manager
*	Created On: 3/2/04
*	Last Edited: 3/2/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

// trail manager class
class CTrailMan
{
public:
				CTrailMan( void );			// Constructor
				~CTrailMan( void );			// Destructor
	/*********************
	* Init
	* @purpose: intializes our vars
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void		Init( void );
	/*********************
	* AddTrail
	* @purpose: Adds a new trail based on inputs
	* @param iIndex: index to entity that gets trail
	* @param iRender: Rendermode of trail
	* @param iType: type of origin of system
	* @param flStSize: start size of trail beam
	* @param flEdSize: end size of trail beam
	* @parma flLinks: # of links in the system
	* @param flLinkLen: length of each link
	* @param flClr[]: color of the beam
	* @param pSprName: sprite name for trail
	* @param pBoneName: name of bone for bone based trail
	* @reutrn void: no return
	*********************/			
	void		AddTrail( int iIndex, int iRender, int iType, float flStSize, float flEdSize,
						  float flLinks, float flLinkLen, float flClr[], char *pSprName, char *pBoneName );
	/*********************
	* RemoveTrail
	* @purpose: removes a trail from the list
	* @param pTrail: trail we want to remove
	* @reutrn CEntTrail: next trail in list
	*********************/
	CEntTrail	*RemoveTrail( CEntTrail *pTrail );
	/*********************
	* RemoveAll
	* @purpose: Removes all trails from list
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void		RemoveAll( void );
	/*********************
	* RenderTrails
	* @purpose: Renders each trail
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void		RenderTrails( void );
	/*********************
	* Find trail
	* @purpose: find trail by index and type, if its a type bone will find by bonename as well
	* @param iIndex: index of entity 
	* @param iType: type of origin of trail
	* @param pBone: name of bone if bone based trail
	* @reutrn void: no return
	*********************/
	CEntTrail	*FindTrail( int iIndex, int iType, char *pBone );
private:
	CEntTrail	*m_pTrail;	// linked list of trails
};

extern CTrailMan g_TrailMan;
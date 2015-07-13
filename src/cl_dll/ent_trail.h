/**********************************************************
*			ENTITY TRAIL CODE
***********************************************************
*	Purpose: Enhanced entity trails
*	Created On: 3/1/04
*	Last Edited: 3/2/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

struct UV_s
{
	float flU;			// u texture coords
	float flV;			// v texture coords
};

struct TrailPoint_s
{
	vec3_t	vPoint;		// point of rotation
	vec3_t	vAngle;		// angle of rotation
	vec3_t	vTrail[4];	// points on the trail used for drawing
	UV_s	uvCoord[2];	// uv coords for points
	float	flSize;		// size at current position
	float	flStart;	// start time
	float	flDist;		// distance to ent
	int		iFrame;		// sprite frame for rendering
	float	flAlpha;	// alpha value of current point

	TrailPoint_s *pPrev;// previous link in linked list
	TrailPoint_s *pNext;// next point in linked list
};

class CEntTrail
{
public:
			CEntTrail( );		// Constructor
			CEntTrail( int iIndex );	// overloaded Constructor
			~CEntTrail( );		// Destructor
	/*********************
	* Init
	* @purpose: intializes our vars
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void	Init( void );
	/*********************
	* RemoveTrail
	* @purpose: removes the trail
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void	RemoveTrail( void );
	/*********************
	* AddLink
	* @purpose: adds a new link to the trail
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void	AddLink( void );
	/*********************
	* UpdateTrail
	* @purpose: updates all the values in the trail
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void	UpdateTrail( void );
	/*********************
	* RenderTrail
	* @purpose: Draws the trail
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void	RenderTrail( void );
	/*********************
	* RemoveLink
	* @purpose: Removes a single link from the linked list
	* @param pPoint: the link we want to remove
	* @reutrn TrailPoint_s: the next link in the list
	*********************/
	TrailPoint_s	*RemoveLink( TrailPoint_s *pPoint );
	/*********************
	* SetEnt
	* @purpose: Sets the entity
	* @param iIndex: index to the entity
	* @reutrn void: no return
	*********************/
	void	SetEnt( int iIndex ) { m_pEnt = gEngfuncs.GetEntityByIndex( iIndex ); m_iIndex = iIndex;}
	/*********************
	* SetRenderMode
	* @purpose: sets the rendermode for drawing
	* @param iMode: value to set the rendermode to
	* @reutrn void: no return
	*********************/
	void	SetRendermode( int iMode ) { m_iRendermode = iMode; }
	/*********************
	* SetSizes
	* @purpose: Sets the start and end sizes of the trail
	* @param flSt: starting size
	* @param flEd: end size
	* @reutrn void: no return
	*********************/
	void	SetSizes( float flSt, float flEd ) { m_flStSize = flSt; m_flEdSize = flEd; }
	/*********************
	* SetLinks
	* @purpose: sets the max number of links in the trail
	* @param flLinks: max number of links
	* @reutrn void: no return
	*********************/
	void	SetLinks( float flLinks ) { m_flMaxLinks = flLinks; }
	/*********************
	* SetLinkLen
	* @purpose: Sets the link length
	* @param flLen: the length to set
	* @reutrn void: no return
	*********************/
	void	SetLinkLen( float flLen ) { m_flLinkLength = flLen; m_flLineTime = m_flLinkLength*m_flMaxLinks/500; }
	/*********************
	* SetColor
	* @purpose: sets the color based on an array of colors
	* @param flClr[]: array of colors to set
	* @reutrn void: no return
	*********************/
	void	SetColor( float flClr[] ) { m_flColor[0] = flClr[0]; m_flColor[1] = flClr[1]; m_flColor[2] = flClr[2]; }
	/*********************
	* SetColor
	* @purpose: sets colors based on individual colors
	* @param flR: red color to set
	* @param flG: green color to set
	* @param flB: blue color to set
	* @reutrn void: no return
	*********************/
	void	SetColor( float flR, float flG, float flB ) { m_flColor[0] = flR; m_flColor[1] = flG; m_flColor[2] = flB; }
	/*********************
	* SetSprName
	* @purpose: set sprite texture name
	* @param pName: pointer to the name of the texture
	* @reutrn void: no return
	*********************/
	void	SetSprName( char *pName ) { strcpy( m_cSprName, pName ); }
	/*********************
	* SetBoneName
	* @purpose: Sets the bone name for bone lookup systems
	* @param pName: name of bone to set
	* @reutrn void: no return
	*********************/
	void	SetBoneName( char *pName ) { strcpy( m_cBones, pName ); }
	/*********************
	* SetType
	* @purpose: sets type of origins to get
	* @param iType: type of origin to set
	* @reutrn void: no return
	*********************/
	void	SetType( int iType ) { m_iType = iType; }
	/*********************
	* SetBones
	* @purpose: Sets up bone index if a bone system
	* @param iIndex: index of entity to get bones for
	* @reutrn void: no return
	*********************/
	void	SetBones( int iIndex );
	/*********************
	* IsAlive
	* @purpose: Checks if system is still alive
	* @param void: no param
	* @reutrn bool: lets us know if the system is alive
	*********************/
	bool	IsAlive( void ) { return m_blAlive; }
	/*********************
	* GetIndex
	* @purpose: Gets index of entity on system
	* @param void: no param
	* @reutrn int: entitys index
	*********************/
	int		GetIndex( void ) { return m_iIndex; }
	/*********************
	* GetBoneName
	* @purpose: Gets the name of the bone that we use as an origin
	* @param void: no param
	* @reutrn char: name of the bone
	*********************/
	char	*GetBoneName( void ) { return m_cBones; }
	/*********************
	* GetType
	* @purpose: Gets type of origin of system
	* @param void: no param
	* @reutrn int: the type of origin the system uses
	*********************/
	int		GetType( void ) { return m_iType; }
	/*********************
	* RmeoveWhenDone
	* @purpose: Tells us to remove the system when its done fading
	* @param void: no param
	* @reutrn void: no return
	*********************/
	void	RemoveWhenDone( void );


	CEntTrail		*m_pNext;		// next entity trail
	CEntTrail		*m_pPrev;		// previous entity trail
	cl_entity_t		*m_pEnt;		// entity linked to
private:
	TrailPoint_s	*m_pHeadPoint;	// head point in linked list
	float			m_flColor[3];	// color vector rgb
	float			m_flStSize;		// start size
	float			m_flEdSize;		// end size
	float			m_flMaxLinks;	// max # of links
	float			m_flLinkLength;	// length of a link
	float			m_flLineTime;	// how long trail lives for
	char			m_cSprName[64];	// sprite name
	char			m_cBones[32];	// bone name
	Texture_s		*m_pSpr;		// sprite of trail
	bool			m_blAlive;		// is trail alive
	bool			m_blDeadCheck;	// check if we declared trail dead
	int				m_iLinks;		// # of links so far
	int				m_iType;		// type of bone to attach to
	int				m_iRendermode;	// rendermode
	int				m_iIndex;		// index of entity
	recboneiter		m_Bones;		// index to entitys bones
};

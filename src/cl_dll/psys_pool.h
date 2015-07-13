/**********************************************************
*			PARTICLE SYSTEM POOL
***********************************************************
*	Purpose: Stores a "pool" of free partcles for less 
*			memoy management
*	Created On: 1/4/04
*	Last Edited: 1/11/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

// moved to hud.h by jason, easy access to other files
/*struct colorRGBA
{
	// colors
	float r, g, b, a;
};*/

struct Particle_s
{
	//all of our particle attributes go here TODO: WORK ON YOUR SPELLING
	vec3_t		vOrg;				// origin
	vec3_t		vPOrg;				// previous origin
	vec3_t		vAng;				// angle if spec'ed
	vec3_t		vUp;				// up vector
	vec3_t		vRight;				// right vector
	vec3_t		vVel;				// velocity
	vec3_t		vMinVel;			// min velocity
	vec3_t		vMaxVel;			// max velocity
	vec3_t		vPMinVel;			// minimum per frame velocity
	vec3_t		vPMaxVel;			// maximum per frame velocity
	colorRGBA	clColor;			// color rgba
	colorRGBA	clStCol;			// start color
	colorRGBA	clEdCol;			// end color
	float		flLife;				// life
	float		flSpawned;			// spawn time
	float		flSize;				// color size
	float		flStSize;			// start size
	float		flEdSize;			// end size
	float		flGrav;				// Gravity
	float		flFrameRate;		// sprite framerate
	float		flLastAnim;			// last time the sprite animated a frame
	float		flBounceScl;		// bounce scaler
	int			iBounces;			// number of bounces
	int			iCurFrame;			// current frame
	int			iFlags;				// flags for special effects
	//SprTex_s	*pTex;				// texture
	Texture_s	*pTex;				// texture
	Particle_s	*pPrev;				// previous particle
	Particle_s	*pNext;				// next particle
};

class CPartPool
{
public:
	CPartPool( void );				// constructor
	~CPartPool( void );				// destructor
	/*******************
	*	DrawFromPool
	* @purpose: Takes a particle from the pool and lets us use it
	* @param void: no param
	* @return *Particle_s: single particle taken from the pool 
	*******************/
	Particle_s	*DrawFromPool( void );
	/*******************
	*	DrainPool
	* @purpose: Removes the particles from the pool
	* @param void: no param
	* @return void: no param 
	*******************/
	void DrainPool( void );
	/*******************
	*	ReturnToPool
	* @purpose: Removes the particles from the pool
	* @param Particle_s *pPart: Particle we want to return to the free list
	* @return void: no param 
	*******************/
	void ReturnToPool( Particle_s *pPart );
	/*******************
	*	InitPool
	* @purpose: Initalizes pool for particle creation
	* @param void: no param
	* @return void: no param
	*******************/
	void		InitPool( void );
private:
	/*******************
	*	DoublePooled
	* @purpose: Enlarges the pool for more particles
	* @param void: no param
	* @return void: no param
	*******************/
	void		DoublePooled( void );
	/*******************
	*	CreatePart
	* @purpose: Creates a particle adds it to free parts list
	* @param void: no param
	* @return void: no param
	*******************/
	void		CreatePart( void );

	int			m_iPooledParts;		// amount of particles that have been pooled so far	
	Particle_s	*m_pFreeParts;		// link list of all our free particles
};

#define	INIT_PARTS	300				// how many particles are we gonna spawn in the pool

extern CPartPool g_PartPool;		// global access to particle pool, no need for more than one

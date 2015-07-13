/**********************************************************
*			SPRITE TEXTURE MANAGMENT SYSTEM
***********************************************************
*	Purpose: Manager textures for particle system and 
*			anything else that uses spr files down the road
*	Created On: 1/4/04
*	Last Edited: 1/4/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

struct SprTex_s
{
	model_t		*m_pTex;				// actual texture
	SprTex_s	*m_pNext;				// next link in linked list
	SprTex_s	*m_pPrev;				// prevous link in linked list
};

class CSprTex
{
public:
	CSprTex( void );					// constructor
	~CSprTex( void );					// destructor
	/************************
	*	Remove All
	* @purpose: Removes all sprtex's
	* @param void: no param
	* @return void: no return
	************************/
	void		RemoveAll( void );
	/************************
	*	Remove 
	* @purpose: remove a single sprtex
	* @param SprTex_s *pSpr: The sprtex to be removed
	* @return *SprTex_s: Next SprTex in the list
	************************/
	SprTex_s	*Remove( SprTex_s *pSpr );
	/************************
	*	Find
	* @purpose: Find the sprite
	* @param char *pName: The name of the sprite we want
	* @return *SprTex_s: The Texture we are looking for
	************************/
	SprTex_s	*Find( char *pName );
private:
	/************************
	*	LookUp
	* @purpose: Lookup sprite by name
	* @param char *pName: The name of the sprite we want
	* @return *SprTex_s: The Texture we are looking for if it exists
	************************/
	SprTex_s	*LookUp( char *pName );
	/************************
	*	Create 
	* @purpose: Create a new sprite
	* @param char *pName: The name of the sprite we want
	* @return *SprTex_s: The new Texture
	************************/
	SprTex_s	*Create( char *pName );

	SprTex_s	*m_pHeadTex;				// head link in linked list
};

extern CSprTex g_SprTex;

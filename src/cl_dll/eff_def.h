/**********************************************************
*			EFFECTS DEFINES
***********************************************************
*	Purpose: Common effects for particle systems
*	Created On: 1/9/04
*	Last Edited: 1/9/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

// internal system defines, this file should also be common with server 
#define EFF_REMOVE		0
#define EFF_BRUSH		1
#define EFF_POINT		2

#define EFF_SMOKE		3

#define EFF_WASTEDEXP	4
#define EFF_ALPHABOMB	5

#define EFF_NUKE		6

// flags

/*	spawnflags(Flags) =
	[
		1 : "Collision" : 1
		2 : "Life" : 0
		4 : "Anim On Collision" : 0
		8 : "Bounce" : 0
	]*/

#define EFF_FLAGS_COL	1
#define EFF_FLAGS_LIFE	2
#define EFF_FLAGS_AOC	4
#define EFF_FLAGS_BNC	8

#define ANG_LOCK_NONE	0
#define	ANG_LOCK_X		1
#define ANG_LOCK_Y		2
#define ANG_LOCK_Z		3
#define	ANG_LOCK_CUST	4
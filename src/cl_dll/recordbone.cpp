/*
* Record Bone Origins
* @version 12/21/03
* @author: Joseph 'Pcjoe' Florencio
*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

int CHudBonePos::Init(void)
{	m_RecBones.clear();
	m_iFlags |= HUD_ACTIVE;
	gHUD.AddHudElem(this);
	return 1;
};

int CHudBonePos::VidInit(void)
{	return 1;
};

void CHudBonePos::ResetBoneFx( void )
{	m_RecBones.clear();
}

int CHudBonePos::Draw(float flTime)
{	return 1;
}

recboneiter CHudBonePos::AddBoneRecordWeapon( int idx )
{	cl_entity_t *pEntity = gEngfuncs.GetEntityByIndex( idx );
	if(!pEntity)
		return NULL;

	// We already have a structure recording bone information
	recboneiter iterRecBone = FindBoneRecordWeapon(idx);
	if(iterRecBone != NULL)
	{	return iterRecBone;
	}

	recordbone_s NewBone;
	NewBone.idx = idx;
	NewBone.iMaxBones = 0;
	NewBone.blCanUse = false;
	NewBone.blRender = true;
	NewBone.blWeapon = true;
	NewBone.blRenderedFrame = false;

	// Add to list
	m_RecBones.push_front(NewBone);
	return m_RecBones.begin();
}

recboneiter CHudBonePos::AddBoneRecordNormal( int idx )
{	cl_entity_t *pEntity = gEngfuncs.GetEntityByIndex( idx );
	if(!pEntity)
		return NULL;

	// We already have a structure recording bone information
	recboneiter iterRecBone = FindBoneRecord(idx);
	if(iterRecBone != NULL)
	{	return iterRecBone;
	}	

	recordbone_s NewBone;
	NewBone.idx = idx;
	NewBone.iMaxBones = 0;
	NewBone.blCanUse = false;
	NewBone.blRender = true;
	NewBone.blWeapon = false;
	NewBone.blRenderedFrame = false;

	// Add to list
	m_RecBones.push_front(NewBone);
	return m_RecBones.begin();
}

recboneiter CHudBonePos::AddBoneRecord( int idx )
{	return AddBoneRecordNormal(idx);
}

recboneiter CHudBonePos::RemoveBoneRecord( int idx )
{	return RemoveBoneRecord(FindBoneRecord(idx));
}

recboneiter CHudBonePos::RemoveBoneRecordWeapon( int idx )
{	return RemoveBoneRecord(FindBoneRecordWeapon(idx));
}


recboneiter CHudBonePos::RemoveBoneRecord( recboneiter iterBone )
{	if(iterBone == NULL)
	{	return NULL;
	}

	// Remove from list
	return m_RecBones.erase(iterBone);
}

recboneiter CHudBonePos::FindBoneRecord( int idx )
{	recboneiter iterCurrent;
	ForEachIn(iterCurrent,m_RecBones)
	{	if(!iterCurrent->blWeapon && iterCurrent->idx == idx)
		{	break;
		}
	}
	if(iterCurrent == m_RecBones.end())
	{	return NULL;
	}

	return iterCurrent;
}

recboneiter CHudBonePos::FindBoneRecordAll( int idx )
{	recboneiter iterCurrent;
	ForEachIn(iterCurrent,m_RecBones)
	{	if(iterCurrent->idx == idx)
		{	break;
		}
	}
	if(iterCurrent == m_RecBones.end())
	{	return NULL;
	}

	return iterCurrent;
}

recboneiter  CHudBonePos::FindBoneRecordWeapon( int idx )
{	recboneiter iterCurrent;
	ForEachIn(iterCurrent,m_RecBones)
	{	if(iterCurrent->blWeapon && iterCurrent->idx == idx)
		{	break;
		}
	}
	if(iterCurrent == m_RecBones.end())
	{	return NULL;
	}

	return iterCurrent;
}


// modifed by jason
vec3_t CHudBonePos::GetParentBoneOrigin( recboneiter iterBone, char *pName, bool &blValid, char &szParent )
{	blValid = false;
	if(iterBone == NULL)
	{	return vec3_t(0,0,0);
	}

	int iBone = -1;
	for(int i=0;i<iterBone->iMaxBones;i++)
	{	if(!stricmp(pName, iterBone->szName[i]))
		{	iBone = i;
			break;
		}
	}

	if(iBone != -1)
	{	return GetBoneOriginName( iterBone, iterBone->iParent[iBone], blValid, szParent );
	}
	else
	{	return vec3_t(0,0,0);
	}
}

vec3_t CHudBonePos::GetBoneOrigin( recboneiter iterBone, int iBone, bool &blValid )
{	blValid = false;
	if(iterBone == NULL)
	{	return vec3_t(0,0,0);
	}

	if(iBone != -1 && iBone < iterBone->iMaxBones)
	{	blValid = true;
		return iterBone->pos[iBone];
	}

	return vec3_t(0,0,0);
}

vec3_t CHudBonePos::GetBoneOriginName( recboneiter iterBone, int iBone, bool &blValid, char &szParent )
{	blValid = false;
	if(iterBone == NULL)
	{	return vec3_t(0,0,0);
	}

	if(iBone != -1 && iBone < iterBone->iMaxBones)
	{	blValid = true;

		// added by jason
		strcpy( &szParent, &iterBone->szName[iBone][0] );
		return iterBone->pos[iBone];
	}

	return vec3_t(0,0,0);
}

vec3_t CHudBonePos::GetBoneOrigin( recboneiter iterBone, char *pName, bool &blValid )
{	blValid = false;
	if(iterBone == NULL)
	{	return vec3_t(0,0,0);
	}

	for(int i=0;i<iterBone->iMaxBones;i++)
	{	if(!stricmp(pName, iterBone->szName[i]))
		{	blValid = true;
			return iterBone->pos[i];
		}
	}
	return vec3_t(0,0,0);
}

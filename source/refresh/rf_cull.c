/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2009-2010 Quake 2 Evolution.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
===========================================================================
*/

//
// rf_cull.c
//

#include "r_local.h"

/*
==============================================================================

	FRUSTUM CULLING	
	
==============================================================================
*/

/*
 ==================
 R_CullBox

 - Returns true if the box is completely outside the frustum
 ==================
*/
qboolean R_CullBox (const vec3_t mins, const vec3_t maxs, int clipFlags)
{
	cplane_t	*plane;
	int			i;

	if (r_noCull->integer)
		return false;

	for (i = 0, plane = r_frustum; i < 4; i++, plane++)
	{
		if (!(clipFlags & (1<<i)))
			continue;

		switch (plane->signbits){
		case 0:
			if (plane->normal[0]*maxs[0] + plane->normal[1]*maxs[1] + plane->normal[2]*maxs[2] < plane->dist)
				return true;
			break;
		case 1:
			if (plane->normal[0]*mins[0] + plane->normal[1]*maxs[1] + plane->normal[2]*maxs[2] < plane->dist)
				return true;
			break;
		case 2:
			if (plane->normal[0]*maxs[0] + plane->normal[1]*mins[1] + plane->normal[2]*maxs[2] < plane->dist)
				return true;
			break;
		case 3:
			if (plane->normal[0]*mins[0] + plane->normal[1]*mins[1] + plane->normal[2]*maxs[2] < plane->dist)
				return true;
			break;
		case 4:
			if (plane->normal[0]*maxs[0] + plane->normal[1]*maxs[1] + plane->normal[2]*mins[2] < plane->dist)
				return true;
			break;
		case 5:
			if (plane->normal[0]*mins[0] + plane->normal[1]*maxs[1] + plane->normal[2]*mins[2] < plane->dist)
				return true;
			break;
		case 6:
			if (plane->normal[0]*maxs[0] + plane->normal[1]*mins[1] + plane->normal[2]*mins[2] < plane->dist)
				return true;
			break;
		case 7:
			if (plane->normal[0]*mins[0] + plane->normal[1]*mins[1] + plane->normal[2]*mins[2] < plane->dist)
				return true;
			break;
		default:
			return false;
		}
	}

	return false;
}

/*
 ==================
 R_CullSphere

 - Returns true if the sphere is completely outside the frustum
 ==================
*/
qboolean R_CullSphere (const vec3_t origin, float radius, int clipFlags)
{
	cplane_t	*plane;
	int			i;

	if (r_noCull->integer)
		return false;

	for (i = 0, plane = r_frustum; i < 4; i++, plane++)
	{
		if (!(clipFlags & (1<<i)))
			continue;

		if (DotProduct(origin, plane->normal) - plane->dist <= -radius)
			return true;
	}

	return false;
}

/*
==============================================================================

	MAP VISIBILITY CULLING
	
==============================================================================
*/

/*
 ==================
 R_CullSurface
 ==================
*/
qboolean R_CullSurface (surface_t *surf, const vec3_t origin, int clipFlags)
{
	cplane_t	*plane;
	float		dist;

	if (r_noCull->integer)
		return false;

	// Find which side of the node we are on
	plane = surf->plane;
	if (plane->type < 3)
		dist = origin[plane->type] - plane->dist;
	else
		dist = DotProduct (origin, plane->normal) - plane->dist;

	if (!(surf->flags & SURF_PLANEBACK))
	{
		if (dist <= BACKFACE_EPSILON)
			return true;	// Wrong side
	}
	else 
	{
		if (dist >= -BACKFACE_EPSILON)
			return true;	// Wrong side
	}

	// Cull
	if (clipFlags)
	{
		if (R_CullBox (surf->mins, surf->maxs, clipFlags))
			return true;
	}

	return false;
}

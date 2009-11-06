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
// s_types.h
//

#ifndef NULL
# define NULL ((void *)0)
#endif

// ===========================================================================

typedef unsigned char 		byte;
typedef enum {false, true}	qboolean;

// ===========================================================================

typedef float   vec_t;
typedef float	vec2_t[2];
typedef float	vec3_t[3];
typedef float	vec4_t[4];

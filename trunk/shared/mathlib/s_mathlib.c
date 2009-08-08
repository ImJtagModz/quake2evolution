/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2008-x Evolution.

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
This file is a part of the evolution rewritten system. If any of this code
is based of another engine or engines, names will be credited!. If you
want to use this code please credit!
===========================================================================
*/

//
// s_mathlib.c - math library main file
//

#include "../s_local.h"

/*
=============================================================================

	MATH LIBRARY

=============================================================================
*/

/*
===============
Q_FastSqrt

5% margin of error
===============
*/
#ifdef id386
float Q_FastSqrt(float value)
{
	float result;
	__asm {
		mov eax, value
		sub eax, 0x3f800000
		sar eax, 1
		add eax, 0x3f800000
		mov result, eax
	}
	return result;
}
#endif // id386
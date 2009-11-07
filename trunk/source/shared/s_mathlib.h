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
// s_mathlib.h
//

/*
==============================================================================

	VECTOR MATH
	
==============================================================================
*/ 

#define Vec2Add(a,b,out)		((out)[0]=(a)[0]+(b)[0],(out)[1]=(a)[1]+(b)[1])
#define Vec2Clear(in)			(*(int *)&(in)[0]=0,*(int *)&(in)[1]=0)
#define Vec2Compare(v1,v2)		((v1)[0]==(v2)[0] && (v1)[1]==(v2)[1])
#define Vec2Copy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1])
#define Vec2DistSquared(v1,v2)	(((v1)[0]-(v2)[0])*((v1)[0]-(v2)[0])+((v1)[1]-(v2)[1])*((v1)[1]-(v2)[1]))
#define Vec2Dist(v1,v2)			(sqrtf(Vec2DistSquared((v1),(v2))))
#define Vec2DistFast(v1,v2)		(Q_FastSqrt(Vec2DistSquared((v1),(v2))))
#define Vec2Inverse(v)			((v)[0]=-(v)[0],(v)[1]=-(v)[1])
#define Vec2MA(v,s,b,o)			((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s))
#define Vec2Negate(a,b)			((b)[0]=-(a)[0],(b)[1]=-(a)[1])
#define Vec2Scale(in,s,out)		((out)[0]=(in)[0]*(s),(out)[1]=(in)[1]*(s))
#define Vec2Set(v,x,y)			((v)[0]=(x),(v)[1]=(y))
#define Vec2Subtract(a,b,c)		((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1])

#define CrossProduct(v1,v2,cr)	((cr)[0]=(v1)[1]*(v2)[2]-(v1)[2]*(v2)[1],(cr)[1]=(v1)[2]*(v2)[0]-(v1)[0]*(v2)[2],(cr)[2]=(v1)[0]*(v2)[1]-(v1)[1]*(v2)[0])
#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])

#define Vec3Add(a,b,out)		((out)[0]=(a)[0]+(b)[0],(out)[1]=(a)[1]+(b)[1],(out)[2]=(a)[2]+(b)[2])
#define Vec3Average(a,b,c)		((c)[0]=((a)[0]+(b)[0])*0.5f,(c)[1]=((a)[1]+(b)[1])*0.5f, (c)[2]=((a)[2]+(b)[2])*0.5f)
#define Vec3Clear(a)			(*(int *)&(a)[0]=0,*(int *)&(a)[1]=0,*(int *)&(a)[2]=0)
#define Vec3Compare(v1,v2)		((v1)[0]==(v2)[0] && (v1)[1]==(v2)[1] && (v1)[2]==(v2)[2])
#define Vec3Copy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define Vec3DistSquared(v1,v2)	((((v1)[0]-(v2)[0])*((v1)[0]-(v2)[0])+((v1)[1]-(v2)[1])*((v1)[1]-(v2)[1])+((v1)[2]-(v2)[2])*((v1)[2]-(v2)[2])))
#define Vec3Dist(v1,v2)			(sqrtf(Vec3DistSquared((v1),(v2))))
#define Vec3DistFast(v1,v2)		(Q_FastSqrt(Vec3DistSquared((v1),(v2))))
#define Vec3Inverse(v)			((v)[0]=-(v)[0],(v)[1]=-(v)[1],(v)[2]=-(v)[2])
#define Vec3Length(v)			(sqrtf(DotProduct((v),(v))))
#define Vec3LengthFast(v)		(Q_FastSqrt(DotProduct((v),(v))))
#define Vec3MA(v,s,b,o)			((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define Vec3Negate(a,b)			((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2])
#define Vec3Scale(in,s,out)		((out)[0]=(in)[0]*(s),(out)[1]=(in)[1]*(s),(out)[2]=(in)[2]*(s))
#define Vec3Set(v,x,y,z)		((v)[0]=(x),(v)[1]=(y),(v)[2]=(z))
#define Vec3Subtract(a,b,c)		((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])

#define Vec4Add(a,b,c)			((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2],(c)[3]=(a)[3]+(b)[3])
#define Vec4Average(a,b,c)		((c)[0]=((a)[0]+(b)[0])*0.5f,(c)[1]=((a)[1]+(b)[1])*0.5f,(c)[2]=((a)[2]+(b)[2])*0.5f,(c)[3]=((a)[3]+(b)[3])*0.5f)
#define Vec4Clear(a)			(*(int *)&(a)[0]=0,*(int *)&(a)[1]=0,*(int *)&(a)[2]=0,*(int *)&(a)[3]=0)
#define Vec4Copy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])
#define Vec4Identity(a)			((a)[0]=(a)[1]=(a)[2]=(a)[3]=0)
#define Vec4MA(v,s,b,o)			((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s),(o)[3]=(v)[3]+(b)[3]*(s))
#define Vec4Negate(a,b)			((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2],(b)[3]=-(a)[3])
#define Vec4Scale(in,s,out)		((out)[0]=(in)[0]*(s),(out)[1]=(in)[1]*(s),(out)[2]=(in)[2]*(s),(out)[3]=(in)[3]*(s))
#define Vec4Set(v,u,x,y,z)		((v)[0]=(u),(v)[1]=(x),(v)[2]=(y),(v)[3]=(z))
#define Vec4Subtract(a,b,c)		((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2],(c)[3]=(a)[3]-(b)[3])
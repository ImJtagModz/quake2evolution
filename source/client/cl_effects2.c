/*
Copyright (C) 1997-2001 Id Software, Inc.

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

*/


// cl_effects.c -- client side effects parsing and management


#include "cl_local.h"


/*
 =======================================================================

 LIGHT STYLE MANAGEMENT

 =======================================================================
*/

typedef struct {
	int		length;
	float	map[MAX_QPATH];
	vec3_t	rgb;
} clightStyle_t;

static clightStyle_t	cl_lightStyles[MAX_LIGHTSTYLES];
static int				cl_lastOfs;


/*
 =================
 CL_ClearLightStyles
 =================
*/
void CL_ClearLightStyles (void){

	memset(cl_lightStyles, 0, sizeof(cl_lightStyles));
	cl_lastOfs = -1;
}

/*
 =================
 CL_RunLightStyles
 =================
*/
void CL_RunLightStyles (void){

	clightStyle_t	*ls;
	int				i, ofs;

	ofs = cl.time / 100;
	if (ofs == cl_lastOfs)
		return;
	cl_lastOfs = ofs;

	for (i = 0, ls = cl_lightStyles; i < MAX_LIGHTSTYLES; i++, ls++){
		if (!ls->length){
			ls->rgb[0] = ls->rgb[1] = ls->rgb[2] = 1.0;
			continue;
		}

		if (ls->length == 1)
			ls->rgb[0] = ls->rgb[1] = ls->rgb[2] = ls->map[0];
		else
			ls->rgb[0] = ls->rgb[1] = ls->rgb[2] = ls->map[ofs % ls->length];
	}
}

/*
 =================
 CL_AddLightStyles
 =================
*/
void CL_AddLightStyles (void){

	clightStyle_t	*ls;
	int				i;

	for (i = 0, ls = cl_lightStyles; i < MAX_LIGHTSTYLES; i++, ls++)
		R_SetLightStyle(i, ls->rgb[0], ls->rgb[1], ls->rgb[2]);
}

/*
 =================
 CL_SetLightStyle
 =================
*/
void CL_SetLightStyle (int style){

	char	*s;
	int		i, len;

	s = cl.configStrings[CS_LIGHTS + style];

	len = strlen(s);
	if (len >= MAX_QPATH)
		Com_Error(ERR_DROP, "CL_SetLightStyle: style length = %i", len);

	if (style < 0 || style >= MAX_LIGHTSTYLES)
		Com_Error(ERR_DROP, "CL_SetLightStyle: style = %i", style);

	cl_lightStyles[style].length = len;

	for (i = 0; i < len; i++)
		cl_lightStyles[style].map[i] = (float)(s[i] - 'a') / (float)('m' - 'a');
}


/*
 =======================================================================

 DYNAMIC LIGHT MANAGEMENT

 =======================================================================
*/

typedef struct {
	qboolean	active;
	int			start;
	int			end;
	vec3_t		origin;
	float		intensity;
	vec3_t		color;
	qboolean	fade;
} cdlight_t;

static cdlight_t	cl_dynamicLights[MAX_DLIGHTS];


/*
 =================
 CL_AllocDynamicLight
 =================
*/
static cdlight_t *CL_AllocDynamicLight (void){

	int		i;
	int		time, index;

	for (i = 0; i < MAX_DLIGHTS; i++){
		if (!cl_dynamicLights[i].active){
			memset(&cl_dynamicLights[i], 0, sizeof(cl_dynamicLights[i]));
			return &cl_dynamicLights[i];
		}
	}

	// Find the oldest light
	time = cl.time;
	index = 0;

	for (i = 0; i < MAX_DLIGHTS; i++){
		if (cl_dynamicLights[i].start < time){
			time = cl_dynamicLights[i].start;
			index = i;
		}
	}

	memset(&cl_dynamicLights[index], 0, sizeof(cl_dynamicLights[index]));
	return &cl_dynamicLights[index];
}

/*
 =================
 CL_ClearDynamicLights
 =================
*/
void CL_ClearDynamicLights (void){

	memset(cl_dynamicLights, 0, sizeof(cl_dynamicLights));
}

/*
 =================
 CL_AddDynamicLights
 =================
*/
void CL_AddDynamicLights (void){

	int			i;
	cdlight_t	*dl;
	float		intensity;

	for (i = 0, dl = cl_dynamicLights; i < MAX_DLIGHTS; i++, dl++){
		if (!dl->active)
			continue;

		if (cl.time >= dl->end){
			dl->active = false;
			continue;
		}

		if (!dl->fade)
			intensity = dl->intensity;
		else {
			intensity = (float)(cl.time - dl->start) / (dl->end - dl->start);
			intensity = dl->intensity * (1.0 - intensity);
		}

		R_AddLightToScene(dl->origin, intensity, dl->color[0], dl->color[1], dl->color[2]);
	}
}

/*
 =================
 CL_DynamicLight
 =================
*/
void CL_DynamicLight (const vec3_t org, float intensity, float r, float g, float b, qboolean fade, int duration){

	cdlight_t	*dl;

	dl = CL_AllocDynamicLight();
	dl->active = true;

	dl->start = cl.time;
	dl->end = dl->start + duration;
	VectorCopy(org, dl->origin);
	dl->intensity = intensity;
	dl->color[0] = r;
	dl->color[1] = g;
	dl->color[2] = b;
	dl->fade = fade;
}

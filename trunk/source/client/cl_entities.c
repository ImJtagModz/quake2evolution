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
// cl_entities.c
//

#include "cl_local.h"

/*
 ==================
 CL_AddShellEntity
 ==================
*/
static void CL_AddShellEntity (entity_t *ent, unsigned int effects, qboolean light)
{
	// Quad and pent can do different things on client
	if (effects & EF_PENT)
	{
		effects &= ~EF_PENT;
		effects |= EF_COLOR_SHELL;
		ent->renderFX |= RF_SHELL_RED;
	}

	if (effects & EF_QUAD)
	{
		effects &= ~EF_QUAD;
		effects |= EF_COLOR_SHELL;
		ent->renderFX |= RF_SHELL_BLUE;
	}

	if (effects & EF_DOUBLE)
	{
		effects &= ~EF_DOUBLE;
		effects |= EF_COLOR_SHELL;
		ent->renderFX |= RF_SHELL_DOUBLE;
	}

	if (effects & EF_HALF_DAMAGE)
	{
		effects &= ~EF_HALF_DAMAGE;
		effects |= EF_COLOR_SHELL;
		ent->renderFX |= RF_SHELL_HALF_DAM;
	}

	if (effects & EF_COLOR_SHELL)
	{
		// PMM - at this point, all of the shells have been handled
		// if we're in the rogue pack, set up the custom mixing, otherwise just
		// keep going
		//			if(Developer_searchpath(2) == 2)
		//			{
		// all of the solo colors are fine.  we need to catch any of the combinations that look bad
		// (double & half) and turn them into the appropriate color, and make double/quad something special
		if (ent->renderFX & RF_SHELL_HALF_DAM)
		{
			if(Developer_searchpath(2) == 2)
			{
				// ditch the half damage shell if any of red, blue, or double are on
				if (ent->renderFX & (RF_SHELL_RED|RF_SHELL_BLUE|RF_SHELL_DOUBLE))
					ent->renderFX &= ~RF_SHELL_HALF_DAM;
			}
		}

		if (ent->renderFX & RF_SHELL_DOUBLE)
		{
			if(Developer_searchpath(2) == 2)
			{
				// lose the yellow shell if we have a red, blue, or green shell
				if (ent->renderFX & (RF_SHELL_RED|RF_SHELL_BLUE|RF_SHELL_GREEN))
					ent->renderFX &= ~RF_SHELL_DOUBLE;
				// if we have a red shell, turn it to purple by adding blue
				if (ent->renderFX & RF_SHELL_RED)
					ent->renderFX |= RF_SHELL_BLUE;
				// if we have a blue shell (and not a red shell), turn it to cyan by adding green
				else if (ent->renderFX & RF_SHELL_BLUE)
					// go to green if it's on already, otherwise do cyan (flash green)
					if (ent->renderFX & RF_SHELL_GREEN)
						ent->renderFX &= ~RF_SHELL_BLUE;
					else
						ent->renderFX |= RF_SHELL_GREEN;
			}
		}

		ent->flags = ent->renderFX | RF_TRANSLUCENT;
		ent->alpha = 0.30;
		V_AddEntity (ent);
	}
}

/*
 ==================
 CL_AddEntityTrails
 ==================
*/
static void CL_AddEntityTrails (centity_t *cent, const vec3_t org, unsigned int effects, qboolean viewer)
{
	if (effects & EF_ROCKET)
	{
		CL_RocketTrail (cent->lerp_origin, org, cent);
		V_AddLight (org, 200, 1.0f, 1.0f, 0.0f);
	}
	else if (effects & EF_BLASTER)
	{
		if (effects & EF_TRACKER)
		{
			CL_BlasterTrail2 (cent->lerp_origin, org);
			V_AddLight (org, 200, 0.0f, 1.0f, 0.0f);		
		}
		else
		{
			CL_BlasterTrail (cent->lerp_origin, org);
			V_AddLight (org, 200, 1.0f, 1.0f, 0.0f);
		}
	}
	else if (effects & EF_HYPERBLASTER)
	{
		if (effects & EF_TRACKER)						
			V_AddLight (org, 200, 0.0f, 1.0f, 0.0f);		
		else											
			V_AddLight (org, 200, 1.0f, 1.0f, 0.0f);
	}
	else if (effects & EF_GIB)
		CL_DiminishingTrail (cent->lerp_origin, org, cent, effects);
	else if (effects & EF_GREENGIB)
		CL_DiminishingTrail (cent->lerp_origin, org, cent, effects);				
	else if (effects & EF_GRENADE)
		CL_DiminishingTrail (cent->lerp_origin, org, cent, effects);
	else if (effects & EF_FLIES)
		CL_FlyEffect (cent, org);
	else if (effects & EF_BFG)
	{
		if (effects & EF_ANIM_ALLFAST)
		{
			CL_BfgParticles (cent);
			V_AddLight (org, 200, 0.0f, 1.0f, 0.0f);
		}
	}
	else if (effects & EF_TRAP)
	{
		CL_TrapParticles (cent);
		V_AddLight (org, 100 + (rand() % 100), 1.0f, 0.8f, 0.1f);
	}
	else if (effects & EF_FLAG1)
	{
		CL_FlagTrail (cent->lerp_origin, org, 242);
		V_AddLight (org, 225, 1.0f, 0.1f, 0.1f);
	}
	else if (effects & EF_FLAG2)
	{
		CL_FlagTrail (cent->lerp_origin, org, 115);
		V_AddLight (org, 225, 0.1f, 0.1f, 1.0f);
	}
	else if (effects & EF_TAGTRAIL)
	{
		CL_TagTrail (cent->lerp_origin, org, 220);
		V_AddLight (org, 225, 1.0f, 1.0f, 0.0f);
	}
	else if (effects & EF_TRACKERTRAIL)
	{
		if (effects & EF_TRACKER)
			V_AddLight (org, 50 + (500 * (sin(cl.time / 500.0) + 1.0f)), -1.0f, -1.0f, -1.0f);
		else
		{
			CL_Tracker_Shell (cent->lerp_origin);
			V_AddLight (org, 155, -1.0f, -1.0f, -1.0f);
		}
	}
	else if (effects & EF_TRACKER)
	{
		CL_TrackerTrail (cent->lerp_origin, org, 0);
		V_AddLight (org, 200, -1.0f, -1.0f, -1.0f);
	}
	else if (effects & EF_IONRIPPER)
	{
		CL_IonripperTrail (cent->lerp_origin, org);
		V_AddLight (org, 100, 1.0f, 0.5f, 0.5f);
	}
	else if (effects & EF_BLUEHYPERBLASTER)
		V_AddLight (org, 200, 0.0f, 0.0f, 1.0f);
	else if (effects & EF_PLASMA)
	{
		if (effects & EF_ANIM_ALLFAST)
			CL_BlasterTrail (cent->lerp_origin, org);
		V_AddLight (org, 130, 1.0f, 0.5f, 0.5f);
	}
}

/*
 ==================
 CL_AddPacketEntities
 ==================
*/
void CL_AddPacketEntities (frame_t *frame)
{
	centity_t			*cent;
	entity_state_t		*state;
	clientinfo_t		*ci;
	entity_t			ent;
	float				autorotate;
	int					autoanim;
	int					i;

	// Bonus items rotate at a fixed rate
	autorotate = anglemod(cl.time/10);

	// Brush models can auto animate their frames
	autoanim = 2*cl.time/1000;

	memset (&ent, 0, sizeof(ent));

	for (i = 0; i < frame->num_entities; i++)
	{
		state = &cl_parse_entities[(frame->parse_entities+i)&(MAX_PARSE_ENTITIES-1)];

		cent = &cl_entities[state->number];

		// Set frame
		if (state->effects & EF_ANIM01)
			ent.frame = autoanim & 1;
		else if (state->effects & EF_ANIM23)
			ent.frame = 2 + (autoanim & 1);
		else if (state->effects & EF_ANIM_ALL)
			ent.frame = autoanim;
		else if (state->effects & EF_ANIM_ALLFAST)
			ent.frame = cl.time / 100;
		else
			ent.frame = state->frame;

		if (state->renderfx & (RF_FRAMELERP|RF_BEAM))
		{	// Step origin discretely, because the frames do the
			// animation properly
			VectorCopy (cent->current.origin, ent.origin);
			VectorCopy (cent->current.old_origin, ent.oldorigin);
		}
		else
		{	// Interpolate origin
			ent.origin[0] = ent.oldorigin[0] = cent->prev.origin[0] + cl.lerpfrac * (cent->current.origin[0] - cent->prev.origin[0]);
			ent.origin[1] = ent.oldorigin[1] = cent->prev.origin[1] + cl.lerpfrac * (cent->current.origin[1] - cent->prev.origin[1]);		
			ent.origin[2] = ent.oldorigin[2] = cent->prev.origin[2] + cl.lerpfrac * (cent->current.origin[2] - cent->prev.origin[2]);		
		}

		// Add laser beams
		if (state->renderfx & RF_BEAM)
		{	// the four beam colors are encoded in 32 bits of skinnum (hack)
			ent.alpha = 0.30;
			ent.skinnum = (state->skinnum >> ((rand() % 4)*8)) & 0xff;
			ent.model = NULL;
		}
		else
		{
			// set skin
			if (state->modelindex == 255)
			{	// use custom player skin
				ent.skinnum = 0;
				ci = &cl.clientinfo[state->skinnum & 0xff];
				ent.skin = ci->skin;
				ent.model = ci->model;
				if (!ent.skin || !ent.model)
				{
					ent.skin = cl.baseclientinfo.skin;
					ent.model = cl.baseclientinfo.model;
				}

				// Expansion
				if (state->renderfx & RF_USE_DISGUISE)
				{
					if(!strncmp((char *)ent.skin, "players/male", 12))
					{
						ent.skin = R_RegisterSkin ("players/male/disguise.pcx");
						ent.model = R_RegisterModel ("players/male/tris.md2");
					}
					else if(!strncmp((char *)ent.skin, "players/female", 14))
					{
						ent.skin = R_RegisterSkin ("players/female/disguise.pcx");
						ent.model = R_RegisterModel ("players/female/tris.md2");
					}
					else if(!strncmp((char *)ent.skin, "players/cyborg", 14))
					{
						ent.skin = R_RegisterSkin ("players/cyborg/disguise.pcx");
						ent.model = R_RegisterModel ("players/cyborg/tris.md2");
					}
				}
			}
			else
			{
				ent.skinnum = state->skinnum;
				ent.skin = NULL;
				ent.model = cl.model_draw[state->modelindex];
			}
		}

		// only used for black hole model right now, FIXME: do better
		if (state->renderfx == RF_TRANSLUCENT)
			ent.alpha = 0.70;

		// render effects (fullbright, translucent, etc)
		if ((state->effects & EF_COLOR_SHELL))
			ent.flags = 0;	// renderfx go on color shell entity
		else
			ent.flags = state->renderfx;

		// Items bob up and down continuously
		if (cl_itemBob->value)
		{
			if (state->effects & EF_BOB) 
			{
				float scale = 0.005f + state->number * 0.00001f;
				float bob = 4 + cos((cl.time + 1000) * scale) * 4;
				ent.oldorigin[2] += bob;
				ent.origin[2] += bob;
			}
		}

		// Calculate angles
		if (state->effects & EF_ROTATE)
		{	// Some bonus items auto-rotate
			ent.angles[0] = 0;
			ent.angles[1] = autorotate;
			ent.angles[2] = 0;
		}
		else if (state->effects & EF_SPINNINGLIGHTS)
		{
			ent.angles[0] = 0;
			ent.angles[1] = anglemod(cl.time/2) + state->angles[1];
			ent.angles[2] = 180;
			{
				vec3_t forward;
				vec3_t start;

				AngleVectors (ent.angles, forward, NULL, NULL);
				VectorMA (ent.origin, 64, forward, start);
				V_AddLight (start, 100, 1, 0, 0);
			}
		}
		else
		{	// Interpolate angles
			ent.angles[0] = LerpAngle (cent->prev.angles[0], cent->current.angles[0], cl.lerpfrac);
			ent.angles[1] = LerpAngle (cent->prev.angles[1], cent->current.angles[1], cl.lerpfrac);
			ent.angles[2] = LerpAngle (cent->prev.angles[2], cent->current.angles[2], cl.lerpfrac);
		}

		if (state->number == cl.playernum+1)
		{
			ent.flags |= RF_VIEWERMODEL;	// only draw from mirrors
			// FIXME: still pass to refresh

			if (state->effects & EF_FLAG1)
				V_AddLight (ent.origin, 225, 1.0, 0.1, 0.1);
			else if (state->effects & EF_FLAG2)
				V_AddLight (ent.origin, 225, 0.1, 0.1, 1.0);
			else if (state->effects & EF_TAGTRAIL)						//PGM
				V_AddLight (ent.origin, 225, 1.0, 1.0, 0.0);	//PGM
			else if (state->effects & EF_TRACKERTRAIL)					//PGM
				V_AddLight (ent.origin, 225, -1.0, -1.0, -1.0);	//PGM
			continue;
		}

		// If set to invisible, skip
		if (!state->modelindex)
			continue;

		// BFG and Phalanx effects are just sprites
		if (state->effects & EF_BFG)
		{
			ent.flags |= RF_TRANSLUCENT;
			ent.alpha = 0.30;
		}
		if (state->effects & EF_PLASMA)
		{
			ent.flags |= RF_TRANSLUCENT;
			ent.alpha = 0.6;
		}

		if (state->effects & EF_SPHERETRANS)
		{
			ent.flags |= RF_TRANSLUCENT;
			// PMM - *sigh*  yet more EF overloading
			if (state->effects & EF_TRACKERTRAIL)
				ent.alpha = 0.6;
			else
				ent.alpha = 0.3;
		}

		// Render effects
		ent.renderFX = state->renderfx;

		// Add to refresh list
		V_AddEntity (&ent);

		// Color shells generate a separate entity for the main model
		if (cl_drawShells->value)
			CL_AddShellEntity (&ent, state->effects, !(ent.renderFX & RF_VIEWERMODEL));

		// Make sure these get reset
		ent.skin = NULL;		// never use a custom skin on others
		ent.skinnum = 0;
		ent.flags = 0;
		ent.alpha = 0;

		// duplicate for linked models
		if (state->modelindex2)
		{
			if (state->modelindex2 == 255)
			{	// custom weapon
				ci = &cl.clientinfo[state->skinnum & 0xff];
				i = (state->skinnum >> 8); // 0 is default weapon model
				if (!cl_vwep->value || i > MAX_CLIENTWEAPONMODELS - 1)
					i = 0;
				ent.model = ci->weaponmodel[i];
				if (!ent.model) {
					if (i != 0)
						ent.model = ci->weaponmodel[0];
					if (!ent.model)
						ent.model = cl.baseclientinfo.weaponmodel[0];
				}
			}
			else
				ent.model = cl.model_draw[state->modelindex2];

			// PMM - check for the defender sphere shell .. make it translucent
			// replaces the previous version which used the high bit on modelindex2 to determine transparency
			if (!Q_strcasecmp (cl.configstrings[CS_MODELS+(state->modelindex2)], "models/items/shell/tris.md2"))
			{
				ent.alpha = 0.32;
				ent.flags = RF_TRANSLUCENT;
			}
			// pmm

			V_AddEntity (&ent);

			//PGM - make sure these get reset.
			ent.flags = 0;
			ent.alpha = 0;
			//PGM
		}
		if (state->modelindex3)
		{
			ent.model = cl.model_draw[state->modelindex3];
			V_AddEntity (&ent);
		}
		if (state->modelindex4)
		{
			ent.model = cl.model_draw[state->modelindex4];
			V_AddEntity (&ent);
		}

		// Power screen
		if (state->effects & EF_POWERSCREEN)
		{
			ent.model = clMedia.powerScreenModel;
			ent.oldframe = 0;
			ent.frame = 0;
			ent.flags |= (RF_TRANSLUCENT | RF_SHELL_GREEN);
			ent.alpha = 0.30;
			V_AddEntity (&ent);
		}

		// Add automatic trails
        if (state->effects & ~EF_ROTATE)
			CL_AddEntityTrails (cent, ent.origin, state->effects, (ent.renderFX & RF_VIEWERMODEL));

		VectorCopy (ent.origin, cent->lerp_origin);
	}
}
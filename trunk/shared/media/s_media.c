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
// s_media.c - media initialization
//

#include "../s_local.h"

/*
=============================================================================

	MEDIA INITIALIZATION

=============================================================================
*/

/*
=================
S_ModelMediaInit
=================
*/
void S_ModelMediaInit ()
{
	// Segment models
	shMedia.cl_mod_parasite_segment          = R_RegisterModel ("models/monsters/parasite/segment/tris.md2");
	shMedia.cl_mod_grapple_cable             = R_RegisterModel ("models/ctf/segment/tris.md2");
	shMedia.cl_mod_powerscreen               = R_RegisterModel ("models/items/armor/effect/tris.md2");

	// Local models (will be brass)

	// Beam models
	shMedia.cl_mod_lightning                 = R_RegisterModel ("models/proj/lightning/tris.md2");
	shMedia.cl_mod_heatbeam                  = R_RegisterModel ("models/proj/beam/tris.md2");
	shMedia.cl_mod_monster_heatbeam          = R_RegisterModel ("models/proj/widowbeam/tris.md2");

	// Disguise models (male,cyborg and female)

	// Explotion models
	shMedia.cl_mod_explode                   = R_RegisterModel ("models/objects/explode/tris.md2");
	shMedia.cl_mod_explo4_big                = R_RegisterModel ("models/objects/r_explode2/tris.md2");
	shMedia.cl_mod_explo4                    = R_RegisterModel ("models/objects/r_explode/tris.md2");
	shMedia.cl_mod_bfg_explo                 = R_RegisterModel ("sprites/s_bfg2.sp2");

	// Smoke model
	shMedia.cl_mod_smoke                     = R_RegisterModel ("models/objects/smoke/tris.md2");

	// Flash
	shMedia.cl_mod_flash                     = R_RegisterModel ("models/objects/flash/tris.md2");
}

/*
=================
S_SoundMediaInit
=================
*/
void S_SoundMediaInit ()
{
	char	name[MAX_QPATH];
	int     i;

	// UI sounds

	// Generic sounds
	shMedia.sfx.cl_sfx_disrexp		     = S_RegisterSound ("weapons/disrupthit.wav");
	shMedia.sfx.cl_sfx_grenexp		     = S_RegisterSound ("weapons/grenlx1a.wav");
	shMedia.sfx.cl_sfx_rockexp		     = S_RegisterSound ("weapons/rocklx1a.wav");
	shMedia.sfx.cl_sfx_watrexp		     = S_RegisterSound ("weapons/xpld_wat.wav");

	shMedia.sfx.cl_sfx_itemRespawn       = S_RegisterSound ("items/respawn1.wav");
	shMedia.sfx.cl_sfx_lashit            = S_RegisterSound ("weapons/lashit.wav");
	shMedia.sfx.cl_sfx_lightning         = S_RegisterSound ("weapons/tesla.wav");

	shMedia.sfx.cl_sfx_playerFall        = S_RegisterSound ("player/fall2.wav"); // "*fall2.wav"
	shMedia.sfx.cl_sfx_playerFallShort   = S_RegisterSound ("player/land1.wav");
	shMedia.sfx.cl_sfx_playerFallFar     = S_RegisterSound ("player/fall1.wav"); // "*fall1.wav"

	shMedia.sfx.cl_sfx_playerTeleport	 = S_RegisterSound ("misc/tele1.wav");
	shMedia.sfx.cl_sfx_bigTeleport	     = S_RegisterSound ("misc/bigtele.wav");

	for (i = 0; i < 5; i++)
	{
		Com_sprintf (name, sizeof(name), "weapons/machgf%ib.wav", i+1);
		shMedia.sfx.mz.cl_sfx_machineGun[i]	= S_RegisterSound (name);

		if (i > 3)
			continue;

		Com_sprintf (name, sizeof(name), "player/step%i.wav", i+1);
		shMedia.sfx.cl_sfx_footsteps[i]	    = S_RegisterSound (name);

		if (i > 2)
			continue;

		Com_sprintf (name, sizeof(name), "world/ric%i.wav", i+1);
		shMedia.sfx.cl_sfx_ricochet[i]	    = S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "world/spark%i.wav", i+1);
		shMedia.sfx.cl_sfx_spark[i]		    = S_RegisterSound (name);
	}

	// Muzzleflash sounds
	shMedia.sfx.mz.cl_sfx_bfgFire			= S_RegisterSound ("weapons/bfg__f1y.wav");
	shMedia.sfx.mz.cl_sfx_blasterFire		= S_RegisterSound ("weapons/blastf1a.wav");
	shMedia.sfx.mz.cl_sfx_etfRifleFire		= S_RegisterSound ("weapons/nail1.wav");
	shMedia.sfx.mz.cl_sfx_grenadeFire		= S_RegisterSound ("weapons/grenlf1a.wav");
	shMedia.sfx.mz.cl_sfx_grenadeReload		= S_RegisterSound ("weapons/grenlr1b.wav");
	shMedia.sfx.mz.cl_sfx_hyperBlasterFire	= S_RegisterSound ("weapons/hyprbf1a.wav");
	shMedia.sfx.mz.cl_sfx_ionRipperFire		= S_RegisterSound ("weapons/rippfire.wav");

	shMedia.sfx.mz.cl_sfx_phalanxFire		= S_RegisterSound ("weapons/plasshot.wav");
	shMedia.sfx.mz.cl_sfx_railgunFire		= S_RegisterSound ("weapons/railgf1a.wav");
	shMedia.sfx.mz.cl_sfx_railgunReload		= S_RegisterSound ("weapons/railgr1a.wav");
	shMedia.sfx.mz.cl_sfx_rocketFire		= S_RegisterSound ("weapons/rocklf1a.wav");
	shMedia.sfx.mz.cl_sfx_rocketReload		= S_RegisterSound ("weapons/rocklr1b.wav");
	shMedia.sfx.mz.cl_sfx_shotgunFire		= S_RegisterSound ("weapons/shotgf1b.wav");
	shMedia.sfx.mz.cl_sfx_shotgun2Fire		= S_RegisterSound ("weapons/shotg2.wav");
	shMedia.sfx.mz.cl_sfx_shotgunReload		= S_RegisterSound ("weapons/shotgr1b.wav");
	shMedia.sfx.mz.cl_sfx_superShotgunFire	= S_RegisterSound ("weapons/sshotf1b.wav");
	shMedia.sfx.mz.cl_sfx_trackerFire		= S_RegisterSound ("weapons/disint2.wav");

	// Monster muzzleflash sounds
}


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
// s_media.h - media initialization
//

void S_ModelMediaInit ();
void S_SoundMediaInit ();

/*
=============================================================================

	SHARED MEDIA STRUCTERS

=============================================================================
*/

// Muzzle flash sounds
typedef struct shMzMedia_s {
	struct sfx_s	*cl_sfx_bfgFire;
	struct sfx_s	*cl_sfx_blasterFire;
	struct sfx_s	*cl_sfx_etfRifleFire;
	struct sfx_s	*cl_sfx_grenadeFire;
	struct sfx_s	*cl_sfx_grenadeReload;
	struct sfx_s	*cl_sfx_hyperBlasterFire;
	struct sfx_s	*cl_sfx_ionRipperFire;
	struct sfx_s	*cl_sfx_machineGun[5];
	struct sfx_s	*cl_sfx_phalanxFire;
	struct sfx_s	*cl_sfx_railgunFire;
	struct sfx_s	*cl_sfx_railgunReload;
	struct sfx_s	*cl_sfx_rocketFire;
	struct sfx_s	*cl_sfx_rocketReload;
	struct sfx_s	*cl_sfx_shotgunFire;
	struct sfx_s	*cl_sfx_shotgun2Fire;
	struct sfx_s	*cl_sfx_shotgunReload;
	struct sfx_s	*cl_sfx_superShotgunFire;
	struct sfx_s	*cl_sfx_trackerFire;
} shMzMedia_t;

// Monster muzzle flash sounds
typedef struct shMz2Media_s {
	int i;
} shMz2Media_t;

typedef struct shMediaSounds_s {

	// Muzzle flash sounds
	shMzMedia_t			mz;
	shMz2Media_t		mz2;

	struct sfx_s		*cl_sfx_footsteps[4];

	struct sfx_s		*cl_sfx_ricochet[3];
	struct sfx_s		*cl_sfx_spark[3];

	struct sfx_s	    *cl_sfx_disrexp;
	struct sfx_s	    *cl_sfx_grenexp;
	struct sfx_s	    *cl_sfx_rockexp;
	struct sfx_s	    *cl_sfx_watrexp;

	struct sfx_s        *cl_sfx_itemRespawn;
	struct sfx_s        *cl_sfx_lashit;
	struct sfx_s	    *cl_sfx_lightning;

	struct sfx_s		*cl_sfx_playerFall;
	struct sfx_s		*cl_sfx_playerFallShort;
	struct sfx_s		*cl_sfx_playerFallFar;

	struct sfx_s		*cl_sfx_playerTeleport;
	struct sfx_s		*cl_sfx_bigTeleport;
} shMediaSounds_t;

typedef struct shMedia_s {

	// Models
	struct model_s		*cl_mod_parasite_segment;
	struct model_s	    *cl_mod_grapple_cable;
	struct model_s	    *cl_mod_powerscreen;

	struct model_s	    *cl_mod_lightning;
	struct model_s	    *cl_mod_heatbeam;
	struct model_s	    *cl_mod_monster_heatbeam;

	struct model_s	    *cl_mod_explode;
	struct model_s	    *cl_mod_explo4;
	struct model_s	    *cl_mod_bfg_explo;
	struct model_s	    *cl_mod_plasmaexplo;
	struct model_s	    *cl_mod_explo4_big;

	struct model_s	    *cl_mod_smoke;
	struct model_s	    *cl_mod_flash;

	// Sounds
	shMediaSounds_t     sfx;
} shMedia_t;

extern shMedia_t	shMedia;
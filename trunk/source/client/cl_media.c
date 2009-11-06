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
// cl_media.c
//

#include "cl_local.h"

/*
 ==================
 CL_GFXMediaInit
 ==================
*/
void CL_GFXMediaInit ()
{
}

/*
 ==================
 CL_ModelMediaInit
 ==================
*/
void CL_ModelMediaInit ()
{
	// Segment models
	clMedia.parasiteSegmentModel	= R_RegisterModel ("models/monsters/parasite/segment/tris.md2");
	clMedia.grappleCableModel		= R_RegisterModel ("models/ctf/segment/tris.md2");
	clMedia.powerScreenModel		= R_RegisterModel ("models/items/armor/effect/tris.md2");

	// Local models
	//clMedia.brassMGModel			= R_RegisterModel ("egl/models/brass/mg/tris.md3");
	//clMedia.brassSGModel			= R_RegisterModel ("egl/models/brass/sg/tris.md3");

	// Beam models
	clMedia.lightningModel			= R_RegisterModel ("models/proj/lightning/tris.md2");
	clMedia.heatBeamModel			= R_RegisterModel ("models/proj/beam/tris.md2");
	clMedia.monsterHeatBeamModel	= R_RegisterModel ("models/proj/widowbeam/tris.md2");

	// Disguise models
	clMedia.maleDisguiseModel		= R_RegisterModel ("players/male/tris.md2");
	clMedia.femaleDisguiseModel		= R_RegisterModel ("players/female/tris.md2");
	clMedia.cyborgDisguiseModel		= R_RegisterModel ("players/cyborg/tris.md2");

	// FIXME: Are these pre-cached in the gamex86.dll?
	R_RegisterModel ("models/objects/laser/tris.md2");
	R_RegisterModel ("models/objects/grenade2/tris.md2");
	R_RegisterModel ("models/weapons/v_machn/tris.md2");
	R_RegisterModel ("models/weapons/v_handgr/tris.md2");
	R_RegisterModel ("models/weapons/v_shotg2/tris.md2");
	R_RegisterModel ("models/objects/gibs/bone/tris.md2");
	R_RegisterModel ("models/objects/gibs/sm_meat/tris.md2");
	R_RegisterModel ("models/objects/gibs/bone2/tris.md2");

	Draw_FindPic ("w_machinegun");
	Draw_FindPic ("a_bullets");
	Draw_FindPic ("i_health");
	Draw_FindPic ("a_grenades");
}

/*
 ==================
 CL_SoundMediaInit
 ==================
*/
void CL_SoundMediaInit ()
{
	char	name[MAX_QPATH];
	int		i;

	// Generic sounds
	clMedia.sfx.disruptExplo		= S_RegisterSound ("weapons/disrupthit.wav");
	clMedia.sfx.grenadeExplo		= S_RegisterSound ("weapons/grenlx1a.wav");
	clMedia.sfx.rocketExplo			= S_RegisterSound ("weapons/rocklx1a.wav");
	clMedia.sfx.waterExplo			= S_RegisterSound ("weapons/xpld_wat.wav");

	clMedia.sfx.itemRespawn			= S_RegisterSound ("items/respawn1.wav");
	clMedia.sfx.laserHit			= S_RegisterSound ("weapons/lashit.wav");
	clMedia.sfx.lightning			= S_RegisterSound ("weapons/tesla.wav");

	clMedia.sfx.playerFall			= S_RegisterSound ("*fall2.wav");
	clMedia.sfx.playerFallShort		= S_RegisterSound ("player/land1.wav");
	clMedia.sfx.playerFallFar		= S_RegisterSound ("*fall1.wav");

	clMedia.sfx.playerTeleport		= S_RegisterSound ("misc/tele1.wav");
	clMedia.sfx.bigTeleport			= S_RegisterSound ("misc/bigtele.wav");

	for (i = 0; i < 4; i++) 
	{
		Q_snprintfz (name, sizeof(name), "player/step%i.wav", i+1);
		clMedia.sfx.steps.standard[i]		= S_RegisterSound (name);

		if (i > 2)
			continue;

		Q_snprintfz (name, sizeof(name), "world/ric%i.wav", i+1);
		clMedia.sfx.ricochet[i]				= S_RegisterSound (name);

		if (i > 1)
			continue;

		Q_snprintfz (name, sizeof(name), "world/spark%i.wav", i+1);
		clMedia.sfx.spark[i]				= S_RegisterSound (name);
	}

	// Muzzleflash sounds
	clMedia.sfx.mz.bfgFireSfx			= S_RegisterSound ("weapons/bfg__f1y.wav");
	clMedia.sfx.mz.blasterFireSfx		= S_RegisterSound ("weapons/blastf1a.wav");
	clMedia.sfx.mz.etfRifleFireSfx		= S_RegisterSound ("weapons/nail1.wav");
	clMedia.sfx.mz.grenadeFireSfx		= S_RegisterSound ("weapons/grenlf1a.wav");
	clMedia.sfx.mz.grenadeReloadSfx		= S_RegisterSound ("weapons/grenlr1b.wav");
	clMedia.sfx.mz.hyperBlasterFireSfx	= S_RegisterSound ("weapons/hyprbf1a.wav");
	clMedia.sfx.mz.ionRipperFireSfx		= S_RegisterSound ("weapons/rippfire.wav");

	for (i = 0; i < 5; i++) 
	{
		Q_snprintfz (name, sizeof(name), "weapons/machgf%ib.wav", i+1);
		clMedia.sfx.mz.machineGunSfx[i]	= S_RegisterSound (name);
	}

	clMedia.sfx.mz.phalanxFireSfx		= S_RegisterSound ("weapons/plasshot.wav");
	clMedia.sfx.mz.railgunFireSfx		= S_RegisterSound ("weapons/railgf1a.wav");
	clMedia.sfx.mz.railgunReloadSfx		= S_RegisterSound ("weapons/railgr1a.wav");
	clMedia.sfx.mz.rocketFireSfx		= S_RegisterSound ("weapons/rocklf1a.wav");
	clMedia.sfx.mz.rocketReloadSfx		= S_RegisterSound ("weapons/rocklr1b.wav");
	clMedia.sfx.mz.shotgunFireSfx		= S_RegisterSound ("weapons/shotgf1b.wav");
	clMedia.sfx.mz.shotgun2FireSfx		= S_RegisterSound ("weapons/shotg2.wav");
	clMedia.sfx.mz.shotgunReloadSfx		= S_RegisterSound ("weapons/shotgr1b.wav");
	clMedia.sfx.mz.superShotgunFireSfx	= S_RegisterSound ("weapons/sshotf1b.wav");
	clMedia.sfx.mz.trackerFireSfx		= S_RegisterSound ("weapons/disint2.wav");

	// Monster muzzleflash sounds
	clMedia.sfx.mz2.chicRocketSfx		= S_RegisterSound ("chick/chkatck2.wav");
	clMedia.sfx.mz2.floatBlasterSfx		= S_RegisterSound ("floater/fltatck1.wav");
	clMedia.sfx.mz2.flyerBlasterSfx		= S_RegisterSound ("flyer/flyatck3.wav");
	clMedia.sfx.mz2.gunnerGrenadeSfx	= S_RegisterSound ("gunner/gunatck3.wav");
	clMedia.sfx.mz2.gunnerMachGunSfx	= S_RegisterSound ("gunner/gunatck2.wav");
	clMedia.sfx.mz2.hoverBlasterSfx		= S_RegisterSound ("hover/hovatck1.wav");
	clMedia.sfx.mz2.jorgMachGunSfx		= S_RegisterSound ("boss3/xfire.wav");
	clMedia.sfx.mz2.machGunSfx			= S_RegisterSound ("infantry/infatck1.wav");
	clMedia.sfx.mz2.makronBlasterSfx	= S_RegisterSound ("makron/blaster.wav");
	clMedia.sfx.mz2.medicBlasterSfx		= S_RegisterSound ("medic/medatck1.wav");
	clMedia.sfx.mz2.soldierBlasterSfx	= S_RegisterSound ("soldier/solatck2.wav");
	clMedia.sfx.mz2.soldierMachGunSfx	= S_RegisterSound ("soldier/solatck3.wav");
	clMedia.sfx.mz2.soldierShotgunSfx	= S_RegisterSound ("soldier/solatck1.wav");
	clMedia.sfx.mz2.superTankRocketSfx	= S_RegisterSound ("tank/rocket.wav");
	clMedia.sfx.mz2.tankBlasterSfx		= S_RegisterSound ("tank/tnkatck3.wav");

	for (i = 0; i < 5; i++) 
	{
		Q_snprintfz (name, sizeof(name), "tank/tnkatk2%c.wav", 'a' + i);
		clMedia.sfx.mz2.tankMachGunSfx[i] = S_RegisterSound (name);
	}

	clMedia.sfx.mz2.tankRocketSfx		= S_RegisterSound ("tank/tnkatck1.wav");

	// Brass sounds
}
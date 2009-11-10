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
==============================================================================

	MUZZLE FLASH PARSING	
	
==============================================================================
*/  

/*
 ==================
 CL_ParsePlayerMuzzleFlash
 ==================
*/
void CL_ParsePlayerMuzzleFlash ()
{
	centity_t	*cent;
	vec3_t		origin, forward, right;
	float		volume = 1.0f;
	int			weapon;
	int			ent;

	// Parse it to the server
	ent = MSG_ReadShort (&net_message);
	if (ent < 1 || ent >= MAX_EDICTS)
		Com_Error (ERR_DROP, "CL_ParsePlayerMuzzleFlash: Bad entity, ent = %i", ent);

	weapon = MSG_ReadByte (&net_message);
	
	// Silenced weapon
	if (weapon & MZ_SILENCED)
	{
		weapon &= ~MZ_SILENCED;
		volume = 0.2f;
	}

	cent = &cl.entities[ent];

	// Locate the origin
	AngleVectors(cent->current.angles, forward, right, NULL);
	origin[0] = cent->current.origin[0] + forward[0] * 18 + right[0] * 16;
	origin[1] = cent->current.origin[1] + forward[1] * 18 + right[1] * 16;
	origin[2] = cent->current.origin[2] + forward[2] * 18 + right[2] * 16;

	// Faster to just set CG brass here
	switch (weapon){
	case MZ_CHAINGUN1:
	case MZ_CHAINGUN2:
	case MZ_CHAINGUN3:
		CL_MachinegunEjectBrass (cent, 1, 10, -8, 18);
		break;
	}

	switch (weapon){
	// MZ_BLASTER
	case MZ_BLASTER:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.blasterFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_BLUEHYPERBLASTER
	case MZ_BLUEHYPERBLASTER:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0, 0, 1, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.hyperBlasterFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_HYPERBLASTER
	case MZ_HYPERBLASTER:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.hyperBlasterFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_MACHINEGUN
	case MZ_MACHINEGUN:
		CL_MachinegunEjectBrass (cent, 1, 15, -8, 18);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.machineGunSfx[(rand () % 5)], volume, ATTN_NORM, 0);
		break;

	// MZ_SHOTGUN
	case MZ_SHOTGUN:
		CL_ShotgunEjectBrass (cent, 1, 12, -6, 16);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.shotgunFireSfx, volume, ATTN_NORM, 0);
		S_StartSound (NULL, ent, CHAN_AUTO, clMedia.sfx.mz.shotgunReloadSfx, volume, ATTN_NORM, 100);
		break;

	// MZ_SSHOTGUN
	case MZ_SSHOTGUN:
		CL_ShotgunEjectBrass (cent, 2, 8, -8, 16);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.superShotgunFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_CHAINGUN1
	case MZ_CHAINGUN1:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.25f, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.machineGunSfx[(rand () % 5)], volume, ATTN_NORM, 0);
		break;

	// MZ_CHAINGUN2
	case MZ_CHAINGUN2:
		CL_DynamicLight (origin, 255 + (rand () & 31), 1, 0.5f, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.machineGunSfx[(rand () % 5)], volume, ATTN_NORM, 0);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.machineGunSfx[(rand () % 5)], volume, ATTN_NORM, 50);
		break;

	// MZ_CHAINGUN3
	case MZ_CHAINGUN3:
		CL_DynamicLight (origin, 250 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.machineGunSfx[(rand () % 5)], volume, ATTN_NORM, 0);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.machineGunSfx[(rand () % 5)], volume, ATTN_NORM, 33);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.machineGunSfx[(rand () % 5)], volume, ATTN_NORM, 66);
		break;

	// MZ_RAILGUN
	case MZ_RAILGUN:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0.5f, 0.5f, 1, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.railgunFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_ROCKET
	case MZ_ROCKET:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.5f, 0.2f, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.rocketFireSfx, volume, ATTN_NORM, 0);
		S_StartSound (NULL, ent, CHAN_AUTO, clMedia.sfx.mz.rocketReloadSfx, volume, ATTN_NORM, 100);
		break;

	// MZ_GRENADE
	case MZ_GRENADE:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.5f, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.grenadeFireSfx, volume, ATTN_NORM, 0);
		S_StartSound (NULL, ent, CHAN_AUTO, clMedia.sfx.mz.grenadeReloadSfx, volume, ATTN_NORM, 100);
		break;

	// MZ_BFG
	case MZ_BFG:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.bfgFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_LOGIN
	case MZ_LOGIN:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.grenadeFireSfx, 1, ATTN_NORM, 0);
		CL_LogParticles (cent->current.origin, 0, 1, 0);
		break;

	// MZ_LOGOUT
	case MZ_LOGOUT:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.grenadeFireSfx, 1, ATTN_NORM, 0);
		CL_LogParticles (cent->current.origin, 1, 0, 0);
		break;

	// MZ_RESPAWN
	case MZ_RESPAWN:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.grenadeFireSfx, 1, ATTN_NORM, 0);
		CL_LogParticles (cent->current.origin, 1, 1, 0);
		break;

	// MZ_PHALANX
	case MZ_PHALANX:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.5f, 0.5f, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.phalanxFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_IONRIPPER
	case MZ_IONRIPPER:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.5f, 0.5f, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.ionRipperFireSfx, volume, ATTN_NORM, 0);
		break;
			
	// MZ_ETF_RIFLE
	case MZ_ETF_RIFLE:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0.9f, 0.7f, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.etfRifleFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_SHOTGUN2
	case MZ_SHOTGUN2:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.shotgun2FireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_HEATBEAM
	case MZ_HEATBEAM:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 100);
		break;

	// MZ_BLASTER2
	case MZ_BLASTER2:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.blasterFireSfx, volume, ATTN_NORM, 0);
		break;

	// MZ_TRACKER
	case MZ_TRACKER:
		CL_DynamicLight (origin, 200 + (rand () & 31), -1, -1, -1, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.trackerFireSfx, volume, ATTN_NORM, 0);
		break;		

	// MZ_NUKE1
	case MZ_NUKE1:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0, 0, false, 100);
		break;

	// MZ_NUKE2
	case MZ_NUKE2:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 100);
		break;

	// MZ_NUKE4
	case MZ_NUKE4:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0, 0, 1, false, 100);
		break;

	// MZ_NUKE8
	case MZ_NUKE8:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0, 1, 1, false, 100);
		break;
	}
}

/*
 ==================
 CL_ParseMonsterMuzzleFlash
 ==================
*/
void CL_ParseMonsterMuzzleFlash ()
{
	centity_t	*cent;
	vec3_t		origin, forward, right, shell;
	int			flash;
	int			ent;

	// Parse it to the server
	ent = MSG_ReadShort(&net_message);
	if (ent < 1 || ent >= MAX_EDICTS)
		Com_Error(ERR_DROP, "CL_ParseMonsterMuzzleFlash: ent = %i", ent);

	flash = MSG_ReadByte(&net_message);

	cent = &cl.entities[ent];

	// Locate the origin
	AngleVectors(cent->current.angles, forward, right, NULL);
	origin[0] = cent->current.origin[0] + forward[0] * monster_flash_offset[flash][0] + right[0] * monster_flash_offset[flash][1];
	origin[1] = cent->current.origin[1] + forward[1] * monster_flash_offset[flash][0] + right[1] * monster_flash_offset[flash][1];
	origin[2] = cent->current.origin[2] + forward[2] * monster_flash_offset[flash][0] + right[2] * monster_flash_offset[flash][1] + monster_flash_offset[flash][2];
	
	// Locate origin for brass (no need?)
	VectorMA (origin, -5, forward, shell);
	VectorMA (origin,  15, forward, origin);

	switch (flash){
	// MZ2_INFANTRY_MACHINEGUN
	case MZ2_INFANTRY_MACHINEGUN_1:
	case MZ2_INFANTRY_MACHINEGUN_2:
	case MZ2_INFANTRY_MACHINEGUN_3:
	case MZ2_INFANTRY_MACHINEGUN_4:
	case MZ2_INFANTRY_MACHINEGUN_5:
	case MZ2_INFANTRY_MACHINEGUN_6:
	case MZ2_INFANTRY_MACHINEGUN_7:
	case MZ2_INFANTRY_MACHINEGUN_8:
	case MZ2_INFANTRY_MACHINEGUN_9:
	case MZ2_INFANTRY_MACHINEGUN_10:
	case MZ2_INFANTRY_MACHINEGUN_11:
	case MZ2_INFANTRY_MACHINEGUN_12:
	case MZ2_INFANTRY_MACHINEGUN_13:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.machGunSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_SOLDIER_MACHINEGUN
	case MZ2_SOLDIER_MACHINEGUN_1:
	case MZ2_SOLDIER_MACHINEGUN_2:
	case MZ2_SOLDIER_MACHINEGUN_3:
	case MZ2_SOLDIER_MACHINEGUN_4:
	case MZ2_SOLDIER_MACHINEGUN_5:
	case MZ2_SOLDIER_MACHINEGUN_6:
	case MZ2_SOLDIER_MACHINEGUN_7:
	case MZ2_SOLDIER_MACHINEGUN_8:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand	() & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.soldierMachGunSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_GUNNER_MACHINEGUN
	case MZ2_GUNNER_MACHINEGUN_1:
	case MZ2_GUNNER_MACHINEGUN_2:
	case MZ2_GUNNER_MACHINEGUN_3:
	case MZ2_GUNNER_MACHINEGUN_4:
	case MZ2_GUNNER_MACHINEGUN_5:
	case MZ2_GUNNER_MACHINEGUN_6:
	case MZ2_GUNNER_MACHINEGUN_7:
	case MZ2_GUNNER_MACHINEGUN_8:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.gunnerMachGunSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_ACTOR_MACHINEGUN, MZ2_SUPERTANK_MACHINEGUN, MZ2_TURRET_MACHINEGUN 
	case MZ2_ACTOR_MACHINEGUN_1:
	case MZ2_SUPERTANK_MACHINEGUN_1:
	case MZ2_SUPERTANK_MACHINEGUN_2:
	case MZ2_SUPERTANK_MACHINEGUN_3:
	case MZ2_SUPERTANK_MACHINEGUN_4:
	case MZ2_SUPERTANK_MACHINEGUN_5:
	case MZ2_SUPERTANK_MACHINEGUN_6:
	case MZ2_TURRET_MACHINEGUN:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.machGunSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_BOSS2_MACHINEGUN, MZ2_CARRIER_MACHINEGUN
	case MZ2_BOSS2_MACHINEGUN_L1:
	case MZ2_BOSS2_MACHINEGUN_L2:
	case MZ2_BOSS2_MACHINEGUN_L3:
	case MZ2_BOSS2_MACHINEGUN_L4:
	case MZ2_BOSS2_MACHINEGUN_L5:
	case MZ2_CARRIER_MACHINEGUN_L1:
	case MZ2_CARRIER_MACHINEGUN_L2:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.machGunSfx, 1, ATTN_NONE, 0);
		break;

	// MZ2_SOLDIER_BLASTER, MZ2_TURRET_BLASTER
	case MZ2_SOLDIER_BLASTER_1:
	case MZ2_SOLDIER_BLASTER_2:
	case MZ2_SOLDIER_BLASTER_3:
	case MZ2_SOLDIER_BLASTER_4:
	case MZ2_SOLDIER_BLASTER_5:
	case MZ2_SOLDIER_BLASTER_6:
	case MZ2_SOLDIER_BLASTER_7:
	case MZ2_SOLDIER_BLASTER_8:
	case MZ2_TURRET_BLASTER:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.soldierBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_FLYER_BLASTER
	case MZ2_FLYER_BLASTER_1:
	case MZ2_FLYER_BLASTER_2:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.flyerBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_MEDIC_BLASTER 
	case MZ2_MEDIC_BLASTER_1:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound  (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.medicBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_HOVER_BLASTER
	case MZ2_HOVER_BLASTER_1:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.hoverBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_FLOAT_BLASTER
	case MZ2_FLOAT_BLASTER_1:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.floatBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_SOLDIER_SHOTGUN
	case MZ2_SOLDIER_SHOTGUN_1:
	case MZ2_SOLDIER_SHOTGUN_2:
	case MZ2_SOLDIER_SHOTGUN_3:
	case MZ2_SOLDIER_SHOTGUN_4:
	case MZ2_SOLDIER_SHOTGUN_5:
	case MZ2_SOLDIER_SHOTGUN_6:
	case MZ2_SOLDIER_SHOTGUN_7:
	case MZ2_SOLDIER_SHOTGUN_8:
		CL_ShotgunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.soldierShotgunSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_TANK_BLASTER
	case MZ2_TANK_BLASTER_1:
	case MZ2_TANK_BLASTER_2:
	case MZ2_TANK_BLASTER_3:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.tankBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_TANK_MACHINEGUN
	case MZ2_TANK_MACHINEGUN_1:
	case MZ2_TANK_MACHINEGUN_2:
	case MZ2_TANK_MACHINEGUN_3:
	case MZ2_TANK_MACHINEGUN_4:
	case MZ2_TANK_MACHINEGUN_5:
	case MZ2_TANK_MACHINEGUN_6:
	case MZ2_TANK_MACHINEGUN_7:
	case MZ2_TANK_MACHINEGUN_8:
	case MZ2_TANK_MACHINEGUN_9:
	case MZ2_TANK_MACHINEGUN_10:
	case MZ2_TANK_MACHINEGUN_11:
	case MZ2_TANK_MACHINEGUN_12:
	case MZ2_TANK_MACHINEGUN_13:
	case MZ2_TANK_MACHINEGUN_14:
	case MZ2_TANK_MACHINEGUN_15:
	case MZ2_TANK_MACHINEGUN_16:
	case MZ2_TANK_MACHINEGUN_17:
	case MZ2_TANK_MACHINEGUN_18:
	case MZ2_TANK_MACHINEGUN_19:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.tankMachGunSfx[(rand () % 5)], 1, ATTN_NORM, 0);
		break;

	// MZ2_CHICK_ROCKET, MZ2_TURRET_ROCKET
	case MZ2_CHICK_ROCKET_1:
	case MZ2_TURRET_ROCKET:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.5f, 0.2f, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.chickRocketSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_TANK_ROCKET
	case MZ2_TANK_ROCKET_1:
	case MZ2_TANK_ROCKET_2:
	case MZ2_TANK_ROCKET_3:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.5f, 0.2f, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.tankRocketSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_SUPERTANK_ROCKET, MZ2_BOSS2_ROCKET, MZ2_CARRIER_ROCKET
	case MZ2_SUPERTANK_ROCKET_1:
	case MZ2_SUPERTANK_ROCKET_2:
	case MZ2_SUPERTANK_ROCKET_3:
	case MZ2_BOSS2_ROCKET_1:
	case MZ2_BOSS2_ROCKET_2:
	case MZ2_BOSS2_ROCKET_3:
	case MZ2_BOSS2_ROCKET_4:
	case MZ2_CARRIER_ROCKET_1:
		CL_DynamicLight (origin, 200 + (rand()&31), 1, 0.5f, 0.2f, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.superTankRocketSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_GUNNER_GRENADE
	case MZ2_GUNNER_GRENADE_1:
	case MZ2_GUNNER_GRENADE_2:
	case MZ2_GUNNER_GRENADE_3:
	case MZ2_GUNNER_GRENADE_4:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 0.5f, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.gunnerGrenadeSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_GLADIATOR_RAILGUN, MZ2_CARRIER_RAILGUN, MZ2_WIDOW_RAIL
	case MZ2_GLADIATOR_RAILGUN_1:
	case MZ2_CARRIER_RAILGUN:
	case MZ2_WIDOW_RAIL:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0.5f, 0.5f, 1.0f, false, 1);
		break;

	// MZ2_MAKRON_BFG
	case MZ2_MAKRON_BFG:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0.5f, 1, 0.5f, false, 1);
		break;

	// MZ2_MAKRON_BLASTER
	case MZ2_MAKRON_BLASTER_1:
	case MZ2_MAKRON_BLASTER_2:
	case MZ2_MAKRON_BLASTER_3:
	case MZ2_MAKRON_BLASTER_4:
	case MZ2_MAKRON_BLASTER_5:
	case MZ2_MAKRON_BLASTER_6:
	case MZ2_MAKRON_BLASTER_7:
	case MZ2_MAKRON_BLASTER_8:
	case MZ2_MAKRON_BLASTER_9:
	case MZ2_MAKRON_BLASTER_10:
	case MZ2_MAKRON_BLASTER_11:
	case MZ2_MAKRON_BLASTER_12:
	case MZ2_MAKRON_BLASTER_13:
	case MZ2_MAKRON_BLASTER_14:
	case MZ2_MAKRON_BLASTER_15:
	case MZ2_MAKRON_BLASTER_16:
	case MZ2_MAKRON_BLASTER_17:
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.makronBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_JORG_MACHINEGUN
	case MZ2_JORG_MACHINEGUN_L1:
	case MZ2_JORG_MACHINEGUN_L2:
	case MZ2_JORG_MACHINEGUN_L3:
	case MZ2_JORG_MACHINEGUN_L4:
	case MZ2_JORG_MACHINEGUN_L5:
	case MZ2_JORG_MACHINEGUN_L6:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.jorgMachGunSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_JORG_MACHINEGUN 
	case MZ2_JORG_MACHINEGUN_R1:
	case MZ2_JORG_MACHINEGUN_R2:
	case MZ2_JORG_MACHINEGUN_R3:
	case MZ2_JORG_MACHINEGUN_R4:
	case MZ2_JORG_MACHINEGUN_R5:
	case MZ2_JORG_MACHINEGUN_R6:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		break;

	// MZ2_JORG_BFG
	case MZ2_JORG_BFG_1:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0.5f, 1, 0.5f, false, 1);
		break;

	// MZ2_BOSS2_MACHINEGUN, MZ2_CARRIER_MACHINEGUN
	case MZ2_BOSS2_MACHINEGUN_R1:
	case MZ2_BOSS2_MACHINEGUN_R2:
	case MZ2_BOSS2_MACHINEGUN_R3:
	case MZ2_BOSS2_MACHINEGUN_R4:
	case MZ2_BOSS2_MACHINEGUN_R5:
	case MZ2_CARRIER_MACHINEGUN_R1:
	case MZ2_CARRIER_MACHINEGUN_R2:
		CL_MachinegunEjectBrass (cent, 1, monster_flash_offset[flash][0], monster_flash_offset[flash][1], monster_flash_offset[flash][2]);
		CL_DynamicLight (origin, 200 + (rand () & 31), 1, 1, 0, false, 1);
		CL_SmokePuffParticles (origin, 3, 1);
		break;

	// MZ2_STALKER_BLASTER, MZ2_DAEDALUS_BLASTER, MZ2_MEDIC_BLASTER,
	// MZ2_WIDOW_BLASTER, MZ2_WIDOW_RUN
	case MZ2_STALKER_BLASTER:
	case MZ2_DAEDALUS_BLASTER:
	case MZ2_MEDIC_BLASTER_2:
	case MZ2_WIDOW_BLASTER:
	case MZ2_WIDOW_BLASTER_SWEEP1:
	case MZ2_WIDOW_BLASTER_SWEEP2:
	case MZ2_WIDOW_BLASTER_SWEEP3:
	case MZ2_WIDOW_BLASTER_SWEEP4:
	case MZ2_WIDOW_BLASTER_SWEEP5:
	case MZ2_WIDOW_BLASTER_SWEEP6:
	case MZ2_WIDOW_BLASTER_SWEEP7:
	case MZ2_WIDOW_BLASTER_SWEEP8:
	case MZ2_WIDOW_BLASTER_SWEEP9:
	case MZ2_WIDOW_BLASTER_100:
	case MZ2_WIDOW_BLASTER_90:
	case MZ2_WIDOW_BLASTER_80:
	case MZ2_WIDOW_BLASTER_70:
	case MZ2_WIDOW_BLASTER_60:
	case MZ2_WIDOW_BLASTER_50:
	case MZ2_WIDOW_BLASTER_40:
	case MZ2_WIDOW_BLASTER_30:
	case MZ2_WIDOW_BLASTER_20:
	case MZ2_WIDOW_BLASTER_10:
	case MZ2_WIDOW_BLASTER_0:
	case MZ2_WIDOW_BLASTER_10L:
	case MZ2_WIDOW_BLASTER_20L:
	case MZ2_WIDOW_BLASTER_30L:
	case MZ2_WIDOW_BLASTER_40L:
	case MZ2_WIDOW_BLASTER_50L:
	case MZ2_WIDOW_BLASTER_60L:
	case MZ2_WIDOW_BLASTER_70L:
	case MZ2_WIDOW_RUN_1:
	case MZ2_WIDOW_RUN_2:
	case MZ2_WIDOW_RUN_3:
	case MZ2_WIDOW_RUN_4:
	case MZ2_WIDOW_RUN_5:
	case MZ2_WIDOW_RUN_6:
	case MZ2_WIDOW_RUN_7:
	case MZ2_WIDOW_RUN_8:
		CL_DynamicLight (origin, 200 + (rand () & 31), 0, 1, 0, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz2.tankBlasterSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_WIDOW_DISRUPTOR
	case MZ2_WIDOW_DISRUPTOR:
		CL_DynamicLight (origin, 200 + (rand () & 31), -1, -1, -1, false, 1);
		S_StartSound (NULL, ent, CHAN_WEAPON, clMedia.sfx.mz.trackerFireSfx, 1, ATTN_NORM, 0);
		break;

	// MZ2_WIDOW_PLASMABEAM, MZ2_WIDOW2_BEAMER, MZ2_WIDOW2_BEAM_SWEEP
	case MZ2_WIDOW_PLASMABEAM:
	case MZ2_WIDOW2_BEAMER_1:
	case MZ2_WIDOW2_BEAMER_2:
	case MZ2_WIDOW2_BEAMER_3:
	case MZ2_WIDOW2_BEAMER_4:
	case MZ2_WIDOW2_BEAMER_5:
	case MZ2_WIDOW2_BEAM_SWEEP_1:
	case MZ2_WIDOW2_BEAM_SWEEP_2:
	case MZ2_WIDOW2_BEAM_SWEEP_3:
	case MZ2_WIDOW2_BEAM_SWEEP_4:
	case MZ2_WIDOW2_BEAM_SWEEP_5:
	case MZ2_WIDOW2_BEAM_SWEEP_6:
	case MZ2_WIDOW2_BEAM_SWEEP_7:
	case MZ2_WIDOW2_BEAM_SWEEP_8:
	case MZ2_WIDOW2_BEAM_SWEEP_9:
	case MZ2_WIDOW2_BEAM_SWEEP_10:
	case MZ2_WIDOW2_BEAM_SWEEP_11:
		CL_DynamicLight (origin, 300 + (rand () & 100), 1, 1, 0, false, 200);
		break;
	}
}
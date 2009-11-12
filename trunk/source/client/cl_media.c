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
 CL_UpdateLoading
 ==================
*/
static void CL_UpdateLoading (const char *string)
{
	Q_strncpyz (cls.loadingInfo.string, string, sizeof(cls.loadingInfo.string));
	cls.loadingInfo.percent += 10;

	// Keep it updated full time
	Sys_PumpMessages ();
	CL_UpdateScreen ();
}

/*
 ==================
 CL_LoadMap
 ==================
*/
static void CL_LoadMap ()
{
	vec3_t	        skyAxis;
	float	        skyRotate;
	unsigned int	checksum;

	// Load the collision map
	CL_UpdateLoading ("COLLISION MAP");

	CM_LoadMap (cl.configStrings[CS_MODELS+1], true, &checksum);

	if (checksum != atoi(cl.configStrings[CS_MAPCHECKSUM]))
		Com_Error (ERR_DROP, "Local map version differs from server: %i != %s", checksum, cl.configStrings[CS_MAPCHECKSUM]);

	// Load the map
	CL_UpdateLoading ("MAP");

	skyRotate = atof(cl.configStrings[CS_SKYROTATE]);
	sscanf (cl.configStrings[CS_SKYAXIS], "%f %f %f", &skyAxis[0], &skyAxis[1], &skyAxis[2]);

	R_LoadWorldMap (cl.configStrings[CS_MODELS+1], cl.configStrings[CS_SKY], skyRotate, skyAxis);
}

/*
 ==================
 CL_SoundMediaInit
 ==================
*/
static void CL_SoundMediaInit ()
{
	char	name[MAX_QPATH];
	int		i;

	// Load sounds
	CL_UpdateLoading ("SOUNDS");

	// Generic sounds
	clMedia.sfx.disruptExplosion		= S_RegisterSound ("weapons/disrupthit.wav");
	clMedia.sfx.grenadeExplosion		= S_RegisterSound ("weapons/grenlx1a.wav");
	clMedia.sfx.rocketExplosion		    = S_RegisterSound ("weapons/rocklx1a.wav");
	clMedia.sfx.waterExplosion			= S_RegisterSound ("weapons/xpld_wat.wav");

	clMedia.sfx.itemRespawn			    = S_RegisterSound ("items/respawn1.wav");
	clMedia.sfx.laserHit			    = S_RegisterSound ("weapons/lashit.wav");
	clMedia.sfx.lightning			    = S_RegisterSound ("weapons/tesla.wav");

	clMedia.sfx.playerFall			    = S_RegisterSound ("*fall2.wav");
	clMedia.sfx.playerFallShort		    = S_RegisterSound ("player/land1.wav");
	clMedia.sfx.playerFallFar		    = S_RegisterSound ("*fall1.wav");

	clMedia.sfx.playerTeleport		    = S_RegisterSound ("misc/tele1.wav");
	clMedia.sfx.bigTeleport			    = S_RegisterSound ("misc/bigtele.wav");

	for (i = 0; i < 4; i++)
	{
		Q_snprintfz (name, sizeof (name), "player/step%i.wav", i+1);
		clMedia.sfx.steps.standard[i]		= S_RegisterSound (name);

		if (i < 2)
			continue;

		Q_snprintfz (name, sizeof (name), "world/spark%i.wav", i+1);
		clMedia.sfx.spark[i]				= S_RegisterSound (name);

		Q_snprintfz (name, sizeof (name), "world/ric%i.wav", i+1);
		clMedia.sfx.ricochet[i]				= S_RegisterSound (name);
	}

	// Player muzzleflash sounds
	clMedia.sfx.mz.bfgFireSfx			= S_RegisterSound ("weapons/bfg__f1y.wav");
	clMedia.sfx.mz.blasterFireSfx		= S_RegisterSound ("weapons/blastf1a.wav");
	clMedia.sfx.mz.etfRifleFireSfx		= S_RegisterSound ("weapons/nail1.wav");
	clMedia.sfx.mz.grenadeFireSfx		= S_RegisterSound ("weapons/grenlf1a.wav");
	clMedia.sfx.mz.grenadeReloadSfx		= S_RegisterSound ("weapons/grenlr1b.wav");
	clMedia.sfx.mz.hyperBlasterFireSfx	= S_RegisterSound ("weapons/hyprbf1a.wav");
	clMedia.sfx.mz.ionRipperFireSfx		= S_RegisterSound ("weapons/rippfire.wav");

	for (i = 0; i < 5; i++) 
	{
		Q_snprintfz (name, sizeof (name), "weapons/machgf%ib.wav", i+1);
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
	clMedia.sfx.mz2.chickRocketSfx		= S_RegisterSound ("chick/chkatck2.wav");
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
		Q_snprintfz (name, sizeof (name), "tank/tnkatk2%c.wav", 'a' + i);
		clMedia.sfx.mz2.tankMachGunSfx[i] = S_RegisterSound (name);
	}

	clMedia.sfx.mz2.tankRocketSfx		= S_RegisterSound ("tank/tnkatck1.wav");

	// FIXME: Replace these
	cl.media.sfxMachinegunBrass = S_RegisterSound("weapons/brass_bullet.wav");
	cl.media.sfxShotgunBrass = S_RegisterSound("weapons/brass_shell.wav");

	// Register the sounds that the server references
	CL_UpdateLoading ("GAME SOUNDS");
	
	// Configstring-based sounds
	for (i = 1; i < MAX_SOUNDS; i++)
	{
		if (!cl.configStrings[CS_SOUNDS+i][0])
			break;
	
		clMedia.gameSounds[i] = S_RegisterSound(cl.configStrings[CS_SOUNDS+i]);
	}
}

/*
 ==================
 CL_SoundMediaInit
 ==================
*/
static void CL_ModelMediaInit ()
{
	int		i;

	// Load models
	CL_UpdateLoading ("MODELS");

	// Segment models
	clMedia.parasiteSegmentModel	= R_RegisterModel ("models/monsters/parasite/segment/tris.md2");
	clMedia.powerScreenModel		= R_RegisterModel ("models/items/armor/effect/tris.md2");	
	
	// Brass models
	clMedia.machinegunBrassModel    = R_RegisterModel ("models/brass/bullet/tris.md3");
	clMedia.shotgunBrassModel       = R_RegisterModel ("models/brass/shell/tris.md3");

	// Better to precache these since id software did it
	R_RegisterModel ("models/objects/laser/tris.md2");
	R_RegisterModel ("models/objects/grenade2/tris.md2");
	R_RegisterModel ("models/weapons/v_machn/tris.md2");
	R_RegisterModel ("models/weapons/v_handgr/tris.md2");
	R_RegisterModel ("models/weapons/v_shotg2/tris.md2");
	R_RegisterModel ("models/objects/gibs/bone/tris.md2");
	R_RegisterModel ("models/objects/gibs/sm_meat/tris.md2");
	R_RegisterModel ("models/objects/gibs/bone2/tris.md2");

	// Register the models that the server references
	CL_UpdateLoading ("GAME MODELS");

	Q_strncpyz (cl.weaponModels[0], "weapon", sizeof(cl.weaponModels[0]));
	cl.numWeaponModels = 1;

	for (i = 1; i < MAX_MODELS; i++)
	{
		if (!cl.configStrings[CS_MODELS+i][0])
			break;

		if (cl.configStrings[CS_MODELS+i][0] == '#')
		{
			// Special player weapon model
			if (cl.numWeaponModels < MAX_CLIENTWEAPONMODELS){
				Com_StripExtension(cl.configStrings[CS_MODELS+i]+1, cl.weaponModels[cl.numWeaponModels], sizeof(cl.weaponModels[cl.numWeaponModels]));
				cl.numWeaponModels++;
			}
		} 
		else 
		{
			clMedia.gameModels[i] = R_RegisterModel(cl.configStrings[CS_MODELS+i]);

			if (cl.configStrings[CS_MODELS+i][0] == '*')
				clMedia.gameCModels[i] = CM_InlineModel(cl.configStrings[CS_MODELS+i]);
			else
				clMedia.gameCModels[i] = NULL;
		}
	}
}

/*
 ==================
 CL_GFXMediaInit
 ==================
*/
static void CL_GFXMediaInit ()
{
	// Load particles
	CL_UpdateLoading("PARTICLES");

	clMedia.particleTable[PT_ENERGY]		= R_RegisterShader ("particles/energy");

	clMedia.particleTable[PT_SMOKE1]		= R_RegisterShader ("particles/smoke1");
	clMedia.particleTable[PT_SMOKE2]		= R_RegisterShader ("particles/smoke2");
	clMedia.particleTable[PT_SMOKE3]		= R_RegisterShader ("particles/smoke3");

	clMedia.particleTable[PT_SMOKE4]		= R_RegisterShader ("particles/smoke4");
	clMedia.particleTable[PT_SMOKE5]		= R_RegisterShader ("particles/smoke5");

	clMedia.particleTable[PT_FIRE1]		    = R_RegisterShader ("particles/fire1");
	clMedia.particleTable[PT_FIRE2]		    = R_RegisterShader ("particles/fire2");
	clMedia.particleTable[PT_FIRE3]		    = R_RegisterShader ("particles/fire3");

	clMedia.particleTable[PT_SPARK]		    = R_RegisterShader ("particles/spark");
}

/*
 ==================
 CL_PicMediaInit
 ==================
*/
static void CL_PicMediaInit ()
{
	int		i, j;
	char	name[MAX_QPATH];
	static char	*sb_nums[2][11] = {
		{"num_0",  "num_1",  "num_2",  "num_3",  "num_4",  "num_5",  "num_6",  "num_7",  "num_8",  "num_9",  "num_minus"},
		{"anum_0", "anum_1", "anum_2", "anum_3", "anum_4", "anum_5", "anum_6", "anum_7", "anum_8", "anum_9", "anum_minus"}
	};

	// Load images
	CL_UpdateLoading("IMAGES");

	clMedia.lagometerMaterial           = R_RegisterShaderNoMip ("lagometer");
	clMedia.disconnectedMaterial        = R_RegisterShaderNoMip ("disconnected");
	clMedia.backTileMaterial			= R_RegisterShaderNoMip ("backTile");
	clMedia.pauseMaterial               = R_RegisterShaderNoMip ("pause");

	for (i = 0; i < NUM_CROSSHAIRS; i++)
		clMedia.crosshairMaterial[i] = R_RegisterShaderNoMip(va("pics/crosshair%i", i+1));

	// FIXME: Replace 'va' with 'Q_VarArgs' from EGL
	for (i = 0; i < 2; i++) 
	{
		for (j = 0; j < 11; j++)
			clMedia.hudNumMaterials[i][j] = R_RegisterShaderNoMip (va ("pics/%s", sb_nums[i][j]));
	}

	CL_LoadHUD ();

	// Better to precache these since id software did it
	R_RegisterShaderNoMip ("pics/w_machinegun");
	R_RegisterShaderNoMip ("pics/a_bullets");
	R_RegisterShaderNoMip ("pics/i_health");
	R_RegisterShaderNoMip ("pics/a_grenades");

	// Register the materials that the server references
	CL_UpdateLoading("GAME MATERIALS");

	for (i = 1; i < MAX_IMAGES; i++)
	{
		if (!cl.configStrings[CS_IMAGES+i][0])
			break;

		if (!strchr(cl.configStrings[CS_IMAGES+i], '/'))
			Q_snprintfz(name, sizeof(name), "pics/%s", cl.configStrings[CS_IMAGES+i]);
		else
			Com_StripExtension(cl.configStrings[CS_IMAGES+i], name, sizeof(name));

		clMedia.gameMaterials[i] = R_RegisterShaderNoMip(name);
	}
}

/*
 =================
 CL_RegisterGraphics
 =================
*/
static void CL_RegisterGraphics (void){
	// Register shaders
	CL_UpdateLoading("SHADERS");

	cl.media.viewBloodBlend = R_RegisterShaderNoMip("viewBloodBlend");
	cl.media.viewFireBlend = R_RegisterShaderNoMip("viewFireBlend");
	cl.media.viewIrGoggles = R_RegisterShaderNoMip("viewIrGoggles");

	cl.media.rocketExplosionShader = R_RegisterShader("rocketExplosion");
	cl.media.rocketExplosionWaterShader = R_RegisterShader("rocketExplosionWater");
	cl.media.grenadeExplosionShader = R_RegisterShader("grenadeExplosion");
	cl.media.grenadeExplosionWaterShader = R_RegisterShader("grenadeExplosionWater");
	cl.media.bfgExplosionShader = R_RegisterShader("bfgExplosion");
	cl.media.bfgBallShader = R_RegisterShader("bfgBall");
	cl.media.plasmaBallShader = R_RegisterShader("plasmaBall");
	cl.media.waterPlumeShader = R_RegisterShader("waterPlume");
	cl.media.waterSprayShader = R_RegisterShader("waterSpray");
	cl.media.waterWakeShader = R_RegisterShader("waterWake");
	cl.media.nukeShockwaveShader = R_RegisterShader("nukeShockwave");
	cl.media.bloodSplatShader[0] = R_RegisterShader("bloodSplat");
	cl.media.bloodSplatShader[1] = R_RegisterShader("greenBloodSplat");
	cl.media.bloodCloudShader[0] = R_RegisterShader("bloodCloud");
	cl.media.bloodCloudShader[1] = R_RegisterShader("greenBloodCloud");

	cl.media.powerScreenShellShader = R_RegisterShaderSkin("shells/powerScreen");
	cl.media.invulnerabilityShellShader = R_RegisterShaderSkin("shells/invulnerability");
	cl.media.quadDamageShellShader = R_RegisterShaderSkin("shells/quadDamage");
	cl.media.doubleDamageShellShader = R_RegisterShaderSkin("shells/doubleDamage");
	cl.media.halfDamageShellShader = R_RegisterShaderSkin("shells/halfDamage");
	cl.media.genericShellShader = R_RegisterShaderSkin("shells/generic");

	cl.media.laserBeamShader = R_RegisterShader("beams/laser");
	cl.media.grappleBeamShader = R_RegisterShader("beams/grapple");
	cl.media.lightningBeamShader = R_RegisterShader("beams/lightning");
	cl.media.heatBeamShader = R_RegisterShader("beams/heat");

	cl.media.energyParticleShader = R_RegisterShader("particles/energy");
	cl.media.glowParticleShader = R_RegisterShader("particles/glow");
	cl.media.flameParticleShader = R_RegisterShader("particles/flame");
	cl.media.smokeParticleShader = R_RegisterShader("particles/smoke");
	cl.media.liteSmokeParticleShader = R_RegisterShader("particles/liteSmoke");
	cl.media.bubbleParticleShader = R_RegisterShader("particles/bubble");
	cl.media.dropletParticleShader = R_RegisterShader("particles/droplet");
	cl.media.steamParticleShader = R_RegisterShader("particles/steam");
	cl.media.sparkParticleShader = R_RegisterShader("particles/spark");
	cl.media.impactSparkParticleShader = R_RegisterShader("particles/impactSpark");
	cl.media.trackerParticleShader = R_RegisterShader("particles/tracker");
	cl.media.flyParticleShader = R_RegisterShader("particles/fly");

	cl.media.energyMarkShader = R_RegisterShader("decals/energyMark");
	cl.media.bulletMarkShader = R_RegisterShader("decals/bulletMark");
	cl.media.burnMarkShader = R_RegisterShader("decals/burnMark");
	cl.media.bloodMarkShaders[0][0] = R_RegisterShader("decals/bloodMark1");
	cl.media.bloodMarkShaders[0][1] = R_RegisterShader("decals/bloodMark2");
	cl.media.bloodMarkShaders[0][2] = R_RegisterShader("decals/bloodMark3");
	cl.media.bloodMarkShaders[0][3] = R_RegisterShader("decals/bloodMark4");
	cl.media.bloodMarkShaders[0][4] = R_RegisterShader("decals/bloodMark5");
	cl.media.bloodMarkShaders[0][5] = R_RegisterShader("decals/bloodMark6");
	cl.media.bloodMarkShaders[1][0] = R_RegisterShader("decals/greenBloodMark1");
	cl.media.bloodMarkShaders[1][1] = R_RegisterShader("decals/greenBloodMark2");
	cl.media.bloodMarkShaders[1][2] = R_RegisterShader("decals/greenBloodMark3");
	cl.media.bloodMarkShaders[1][3] = R_RegisterShader("decals/greenBloodMark4");
	cl.media.bloodMarkShaders[1][4] = R_RegisterShader("decals/greenBloodMark5");
	cl.media.bloodMarkShaders[1][5] = R_RegisterShader("decals/greenBloodMark6");
}

/*
 ==================
 CL_RegisterClients
 ==================
*/
static void CL_RegisterClients ()
{
	int		i;

	// Load all the clients in the server
	CL_UpdateLoading ("CLIENTS");

	CL_LoadClientInfo (&cl.baseClientInfo, "unnamed\\male/grunt");

	for (i = 0; i < MAX_CLIENTS; i++)
	{
		if (!cl.configStrings[CS_PLAYERSKINS+i][0])
			continue;

		CL_LoadClientInfo (&cl.clientInfo[i], cl.configStrings[CS_PLAYERSKINS+i]);
	}
}

/*
 ==================
 CL_Loading
 ==================
*/
void CL_Loading ()
{
	if (cls.loading)
		return;

	cls.loading = true;
	memset (&cls.loadingInfo, 0, sizeof(loadingInfo_t));

	// Make sure CD audio and sounds aren't playing
	CDAudio_Stop ();
	S_StopAllSounds ();

	// Force menu and console off
	UI_SetActiveMenu (UI_CLOSEMENU);
	Con_CloseConsole ();
}

/*
 =================
 CL_DrawLoading
 =================
*/
void CL_DrawLoading (void)
{
	char	str[1024];
	float	speed;
	int		percent;

	if (!cls.loading)
		return;

	switch (cls.state){
	case CA_DISCONNECTED:
		// Still disconnected, but local server is initializing
		CL_DrawPicByName(0, 0, 640, 480, colorWhite, "ui/title_screen/title_backg");
		CL_DrawPicByName(0, 0, 640, 160, colorWhite, "ui/title_screen/q2e_logo");

		break;
	case CA_CONNECTING:
		// Awaiting connection
		CL_DrawPicByName(0, 0, 640, 480, colorWhite, "ui/title_screen/title_backg");
		CL_DrawPicByName(0, 0, 640, 160, colorWhite, "ui/title_screen/q2e_logo");

		if (Com_ServerState()){
			Q_snprintfz(str, sizeof(str), "Starting up...");
			CL_DrawString(0, 424, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}
		else {
			if (cls.serverMessage[0])
				CL_DrawString(0, 360, 16, 16, 0, 0, 640, cls.serverMessage, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);

			Q_snprintfz(str, sizeof(str), "Connecting to %s\nAwaiting connection... %i", cls.serverName, cls.connectCount);
			CL_DrawString(0, 408, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}

		break;
	case CA_CHALLENGING:
		// Awaiting challenge
		CL_DrawPicByName(0, 0, 640, 480, colorWhite, "ui/title_screen/title_backg");
		CL_DrawPicByName(0, 0, 640, 160, colorWhite, "ui/title_screen/q2e_logo");

		if (Com_ServerState()){
			Q_snprintfz(str, sizeof(str), "Starting up...");
			CL_DrawString(0, 424, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}
		else {
			if (cls.serverMessage[0])
				CL_DrawString(0, 360, 16, 16, 0, 0, 640, cls.serverMessage, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);

			Q_snprintfz(str, sizeof(str), "Connecting to %s\nAwaiting challenge... %i", cls.serverName, cls.connectCount);
			CL_DrawString(0, 408, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}

		break;
	case CA_CONNECTED:
		if (cls.downloadFile){
			// Downloading file from server
			CL_DrawPicByName(0, 0, 640, 480, colorWhite, "ui/title_screen/title_backg");
			CL_DrawPicByName(0, 0, 640, 160, colorWhite, "ui/title_screen/q2e_logo");

			if (cls.downloadStart != cls.realTime)
				speed = (float)(cls.downloadBytes / 1024) / ((cls.realTime - cls.downloadStart) / 1000);
			else
				speed = 0;

			if (Com_ServerState()){
				Q_snprintfz(str, sizeof(str), "Downloading %s... (%.2f KB/sec)", cls.downloadName, speed);
				CL_DrawString(0, 424, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
			}
			else {
				Q_snprintfz(str, sizeof(str), "Connecting to %s\nDownloading %s... (%.2f KB/sec)", cls.serverName, cls.downloadName, speed);
				CL_DrawString(0, 408, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
			}

			percent = Clamp(cls.downloadPercent - (cls.downloadPercent % 5), 5, 100);
			if (percent){
				CL_DrawPicByName(240, 160, 160, 160, colorWhite, "ui/loading/load_main2");
				CL_DrawPicByName(240, 160, 160, 160, colorWhite, va("ui/loading/percent/load_%i", percent));
				CL_DrawPicByName(240, 160, 160, 160, colorWhite, "ui/loading/load_main");
			}

			break;
		}

		// Awaiting game state
		CL_DrawPicByName(0, 0, 640, 480, colorWhite, "ui/title_screen/title_backg");
		CL_DrawPicByName(0, 0, 640, 160, colorWhite, "ui/title_screen/q2e_logo");

		if (Com_ServerState()){
			Q_snprintfz(str, sizeof(str), "Starting up...");
			CL_DrawString(0, 424, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}
		else {
			Q_snprintfz(str, sizeof(str), "Connecting to %s\nAwaiting game state...", cls.serverName);
			CL_DrawString(0, 408, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}

		break;
	case CA_LOADING:
		// Loading level data
		CL_DrawPic(0, 0, 640, 480, colorWhite, cl.media.levelshot);
		CL_DrawPic(0, 0, 640, 480, colorWhite, cl.media.levelshotDetail);
		CL_DrawPic(0, 0, 640, 160, colorWhite, cl.media.loadingLogo);

		if (Com_ServerState()){
			Q_snprintfz(str, sizeof(str), "Loading %s\n\"%s\"\n\n\nLoading... %s\n", cls.loadingInfo.map, cls.loadingInfo.name, cls.loadingInfo.string);
			CL_DrawString(0, 360, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}
		else {
			Q_snprintfz(str, sizeof(str), "Loading %s\n\"%s\"\n\nConnecting to %s\nLoading... %s\n", cls.loadingInfo.map, cls.loadingInfo.name, cls.serverName, cls.loadingInfo.string);
			CL_DrawString(0, 360, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);
		}

		percent = Clamp((cls.loadingInfo.percent / 5) - 1, 0, 19);
		if (percent){
			CL_DrawPic(240, 160, 160, 160, colorWhite, cl.media.loadingDetail[0]);
			CL_DrawPic(240, 160, 160, 160, colorWhite, cl.media.loadingPercent[percent]);
			CL_DrawPic(240, 160, 160, 160, colorWhite, cl.media.loadingDetail[1]);
		}

		break;
	case CA_PRIMED:
		// Awaiting frame
		CL_DrawPic(0, 0, 640, 480, colorWhite, cl.media.levelshot);
		CL_DrawPic(0, 0, 640, 480, colorWhite, cl.media.levelshotDetail);
		CL_DrawPic(0, 0, 640, 160, colorWhite, cl.media.loadingLogo);

		Q_snprintfz(str, sizeof(str), "Awaiting frame...");
		CL_DrawString(0, 424, 16, 16, 0, 0, 640, str, colorWhite, cls.media.charsetShader, true, DSF_FORCECOLOR|DSF_DROPSHADOW|DSF_CENTER|DSF_UPPERCASE);

		break;
	case CA_ACTIVE:
		// Still active, but local server is changing levels
		CL_FillRect(0, 0, 640, 480, colorBlack);

		break;
	}
}

/*
 =================
 CL_LoadClientInfo
 =================
*/
void CL_LoadClientInfo (clientInfo_t *ci, const char *string){

	char	model[MAX_QPATH], skin[MAX_QPATH], name[MAX_QPATH];
	char	checkMD3[MAX_QPATH], checkMD2[MAX_QPATH];
	char	checkTGA[MAX_QPATH], checkJPG[MAX_QPATH], checkPCX[MAX_QPATH];
	char	*ch;
	int		i;

	memset(ci, 0, sizeof(clientInfo_t));

	// Isolate the player name
	Q_strncpyz(ci->name, string, sizeof(ci->name));
	ch = strchr(string, '\\');
	if (ch){
		ci->name[ch-string] = 0;
		string = ch+1;
	}

	if (cl_noSkins->integer || *string == 0){
		// No custom skins or bad info string, so just use male/grunt
		ci->model = R_RegisterModel("players/male/tris.md2");
		ci->skin = R_RegisterShaderSkin("players/male/grunt");
		ci->icon = R_RegisterShaderNoMip("players/male/grunt_i");
		ci->weaponModel[0] = R_RegisterModel("players/male/weapon.md2");

		// Save model/skin in the info string
		Q_snprintfz(ci->info, sizeof(ci->info), "male/grunt");

		ci->valid = true;
		return;
	}

	// Isolate model and skin name
	Q_strncpyz(model, string, sizeof(model));
	ch = strchr(model, '/');
	if (!ch)
		ch = strchr(model, '\\');
	if (ch){
		*ch++ = 0;
		Q_strncpyz(skin, ch, sizeof(skin));
	}
	else
		skin[0] = 0;

	// If the model doesn't exist, default to male
	Q_snprintfz(checkMD3, sizeof(checkMD3), "players/%s/tris.md3", model);
	Q_snprintfz(checkMD2, sizeof(checkMD2), "players/%s/tris.md2", model);
	if (FS_LoadFile(checkMD3, NULL) == -1 && FS_LoadFile(checkMD2, NULL) == -1)
		Q_strncpyz(model, "male", sizeof(model));

	// If the skin doesn't exist, default to male/grunt
	Q_snprintfz(checkTGA, sizeof(checkTGA), "players/%s/%s.tga", model, skin);
	Q_snprintfz(checkJPG, sizeof(checkJPG), "players/%s/%s.jpg", model, skin);
	Q_snprintfz(checkPCX, sizeof(checkPCX), "players/%s/%s.pcx", model, skin);
	if (FS_LoadFile(checkTGA, NULL) == -1 && FS_LoadFile(checkJPG, NULL) == -1 && FS_LoadFile(checkPCX, NULL) == -1){
		Q_strncpyz(model, "male", sizeof(model));
		Q_strncpyz(skin, "grunt", sizeof(skin));
	}

	// If the icon doesn't exist, default to male/grunt
	Q_snprintfz(checkTGA, sizeof(checkTGA), "players/%s/%s_i.tga", model, skin);
	Q_snprintfz(checkJPG, sizeof(checkJPG), "players/%s/%s_i.jpg", model, skin);
	Q_snprintfz(checkPCX, sizeof(checkPCX), "players/%s/%s_i.pcx", model, skin);
	if (FS_LoadFile(checkTGA, NULL) == -1 && FS_LoadFile(checkJPG, NULL) == -1 && FS_LoadFile(checkPCX, NULL) == -1){
		Q_strncpyz(model, "male", sizeof(model));
		Q_strncpyz(skin, "grunt", sizeof(skin));
	}

	// If a weapon model doesn't exist, default to male/grunt
	for (i = 0; i < cl.numWeaponModels; i++){
		Q_snprintfz(checkMD3, sizeof(checkMD3), "players/%s/%s.md3", model, cl.weaponModels[i]);
		Q_snprintfz(checkMD2, sizeof(checkMD2), "players/%s/%s.md2", model, cl.weaponModels[i]);
		if (FS_LoadFile(checkMD3, NULL) == -1 && FS_LoadFile(checkMD2, NULL) == -1){
			Q_strncpyz(model, "male", sizeof(model));
			Q_strncpyz(skin, "grunt", sizeof(skin));
			break;
		}

		if (!cl_visibleWeapons->integer)
			break;		// Only one if no visible weapons
	}

	// We can now load everything
	Q_snprintfz(name, sizeof(name), "players/%s/tris.md2", model);
	ci->model = R_RegisterModel(name);

	Q_snprintfz(name, sizeof(name), "players/%s/%s", model, skin);
	ci->skin = R_RegisterShaderSkin(name);

	Q_snprintfz(name, sizeof(name), "players/%s/%s_i", model, skin);
	ci->icon = R_RegisterShaderNoMip(name);

	for (i = 0; i < cl.numWeaponModels; i++){
		Q_snprintfz(name, sizeof(name), "players/%s/%s.md2", model, cl.weaponModels[i]);
		ci->weaponModel[i] = R_RegisterModel(name);

		if (!cl_visibleWeapons->integer)
			break;		// Only one if no visible weapons
	}

	// Save model/skin in the info string
	Q_snprintfz(ci->info, sizeof(ci->info), "%s/%s", model, skin);

	ci->valid = true;
}

/*
 =================
 CL_LoadGameMedia
 =================
*/
void CL_LoadGameMedia (void){

	char	checkTGA[MAX_QPATH], checkJPG[MAX_QPATH];
	char	levelshot[MAX_QPATH];
	int		i, n;
	int		time;

	time = Sys_Milliseconds();

	// Need to precache files
	cls.state = CA_LOADING;

	// Clear all local effects (except for light styles) because they
	// now point to invalid files
	CL_ClearTempEntities();
	CL_ClearLocalEntities();
	CL_ClearDynamicLights();
	CL_ClearMarks();
	CL_ClearParticles();

	// Get map name
	Com_StripExtension(cl.configStrings[CS_MODELS+1]+5, cls.loadingInfo.map, sizeof(cls.loadingInfo.map));
	Q_strncpyz(cls.loadingInfo.name, cl.configStrings[CS_NAME], sizeof(cls.loadingInfo.name));

	// Check if a levelshot for this map exists
	Q_snprintfz(checkTGA, sizeof(checkTGA), "ui/levelshots/%s.tga", cls.loadingInfo.map);
	Q_snprintfz(checkJPG, sizeof(checkJPG), "ui/levelshots/%s.jpg", cls.loadingInfo.map);
	if (FS_LoadFile(checkTGA, NULL) == -1 && FS_LoadFile(checkJPG, NULL) == -1)
		Q_snprintfz(levelshot, sizeof(levelshot), "ui/levelshots/unknownmap");
	else
		Q_snprintfz(levelshot, sizeof(levelshot), "ui/levelshots/%s", cls.loadingInfo.map);

	// Load a few needed shaders for the loading screen
	cl.media.levelshot = R_RegisterShaderNoMip(levelshot);
	cl.media.levelshotDetail = R_RegisterShaderNoMip("ui/loading/loading_detail");
	cl.media.loadingLogo = R_RegisterShaderNoMip("ui/title_screen/q2e_logo");
	cl.media.loadingDetail[0] = R_RegisterShaderNoMip("ui/loading/load_main2");
	cl.media.loadingDetail[1] = R_RegisterShaderNoMip("ui/loading/load_main");

	for (i = 0, n = 5; i < 20; i++, n += 5)
		cl.media.loadingPercent[i] = R_RegisterShaderNoMip(va("ui/loading/percent/load_%i", n));

	// Register all the files for this level
	CL_LoadMap ();
	CL_SoundMediaInit ();
	CL_ModelMediaInit ();
	CL_GFXMediaInit ();
	CL_PicMediaInit ();

	// Q2e´s
	CL_RegisterGraphics();
	CL_RegisterClients();

	// Start the background track
	CL_PlayBackgroundTrack();

	// All precaches are now complete
	cls.state = CA_PRIMED;

	Com_Printf("CL_LoadGameMedia: %.2f seconds\n", (float)(Sys_Milliseconds() - time) / 1000.0);

	CL_UpdateLoading("");

	// Touch all the memory used for this level
	Com_TouchMemory();

	// Force menu and console off
	UI_SetActiveMenu(UI_CLOSEMENU);
	Con_CloseConsole();
}

/*
 =================
 CL_LoadLocalMedia
 =================
*/
void CL_LoadLocalMedia (void){

	// Get GL/AL information
	R_GetGLConfig(&cls.glConfig);
	S_GetALConfig(&cls.alConfig);

	// Set screen scales
	cls.screenScaleX = cls.glConfig.videoWidth / 640.0;
	cls.screenScaleY = cls.glConfig.videoHeight / 480.0;

	// Load a few needed shaders
	cls.media.whiteShader = R_RegisterShaderNoMip("white");
	cls.media.consoleShader = R_RegisterShaderNoMip("console");
	cls.media.charsetShader = R_RegisterShaderNoMip("charset");

	// Precache UI files if requested
	UI_Precache();
}

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
// rf_register.c
//

#include "r_local.h"

cvar_t	*r_noRefresh;
cvar_t	*r_noVis;
cvar_t	*r_noCull;
cvar_t	*r_noBind;
cvar_t	*r_drawWorld;
cvar_t	*r_drawEntities;
cvar_t	*r_drawParticles;
cvar_t	*r_drawPolys;
cvar_t	*r_fullbright;
cvar_t	*r_lightmap;
cvar_t	*r_lockPVS;
cvar_t	*r_logFile;
cvar_t	*r_frontBuffer;
cvar_t	*r_showCluster;
cvar_t	*r_showTris;
cvar_t	*r_showNormals;
cvar_t	*r_showTangentSpace;
cvar_t	*r_showModelBounds;
cvar_t	*r_showShadowVolumes;
cvar_t	*r_offsetFactor;
cvar_t	*r_offsetUnits;
cvar_t	*r_debugSort;
cvar_t	*r_speeds;
cvar_t	*r_clear;
cvar_t	*r_singleShader;
cvar_t	*r_skipBackEnd;
cvar_t	*r_skipFrontEnd;
cvar_t	*r_allowSoftwareGL;
cvar_t	*r_maskMiniDriver;
cvar_t	*r_glDriver;
cvar_t	*r_allowExtensions;
cvar_t	*r_arb_multitexture;
cvar_t	*r_arb_texture_env_add;
cvar_t	*r_arb_texture_env_combine;
cvar_t	*r_arb_texture_env_dot3;
cvar_t	*r_arb_texture_cube_map;
cvar_t	*r_arb_texture_compression;
cvar_t	*r_arb_vertex_buffer_object;
cvar_t	*r_arb_vertex_program;
cvar_t	*r_arb_fragment_program;
cvar_t	*r_ext_draw_range_elements;
cvar_t	*r_ext_compiled_vertex_array;
cvar_t	*r_ext_texture_edge_clamp;
cvar_t	*r_ext_texture_filter_anisotropic;
cvar_t	*r_ext_texture_rectangle;
cvar_t	*r_ext_stencil_two_side;
cvar_t	*r_ext_generate_mipmap;
cvar_t	*r_ext_swap_control;
cvar_t	*r_swapInterval;
cvar_t	*r_finish;
cvar_t	*r_stereo;
cvar_t	*r_colorBits;
cvar_t	*r_depthBits;
cvar_t	*r_stencilBits;
cvar_t	*r_mode;
cvar_t	*r_fullscreen;
cvar_t	*r_customWidth;
cvar_t	*r_customHeight;
cvar_t	*r_displayRefresh;
cvar_t	*r_ignoreHwGamma;
cvar_t	*r_gamma;
cvar_t	*r_overBrightBits;
cvar_t	*r_ignoreGLErrors;
cvar_t	*r_shadows;
cvar_t	*r_caustics;
cvar_t	*r_lodBias;
cvar_t	*r_lodDistance;
cvar_t	*gl_dynamic;
cvar_t	*gl_modulate;
cvar_t	*r_ambientScale;
cvar_t	*r_directedScale;
cvar_t	*r_intensity;
cvar_t	*r_roundImagesDown;
cvar_t	*r_maxTextureSize;
cvar_t	*r_picmip;
cvar_t	*r_textureBits;
cvar_t	*r_textureFilter;
cvar_t	*r_textureFilterAnisotropy;
cvar_t	*r_jpegCompressionQuality;
cvar_t	*r_detailTextures;
cvar_t	*r_inGameVideo;

/*
==============================================================================

	KEY HANDLING	
	
==============================================================================
*/  

/*
 ==================
 R_GfxInfo_f
 ==================
*/
void R_GfxInfo_f ()
{
	Com_Printf ("\n");
	Com_Printf ("GL_VENDOR: %s\n",           glConfig.vendorString);
	Com_Printf ("GL_RENDERER: %s\n",         glConfig.rendererString);
	Com_Printf ("GL_VERSION: %s\n",          glConfig.versionString);
	Com_Printf ("GL_EXTENSIONS: %s\n",       glConfig.extensionsString);

	Com_Printf ("GL_MAX_TEXTURE_SIZE: %i\n", glConfig.maxTextureSize);

	if (glConfig.multitexture)
		Com_Printf("GL_MAX_TEXTURE_UNITS_ARB: %i\n", glConfig.maxTextureUnits);
	if (glConfig.textureCubeMap)
		Com_Printf("GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB: %i\n", glConfig.maxCubeMapTextureSize);
	if (glConfig.fragmentProgram){
		Com_Printf("GL_MAX_TEXTURE_COORDS_ARB: %i\n", glConfig.maxTextureCoords);
		Com_Printf("GL_MAX_TEXTURE_IMAGE_UNITS_ARB: %i\n", glConfig.maxTextureImageUnits);
	}
	if (glConfig.textureFilterAnisotropic)
		Com_Printf("GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT: %.1f\n", glConfig.maxTextureMaxAnisotropy);
	if (glConfig.textureRectangle)
		Com_Printf("GL_MAX_RECTANGLE_TEXTURE_SIZE_NV: %i\n", glConfig.maxRectangleTextureSize);

	Com_Printf("\n");
	Com_Printf("PIXELFORMAT: color(%i-bits) Z(%i-bits) stencil(%i-bits)\n", glConfig.colorBits, glConfig.depthBits, glConfig.stencilBits);
	Com_Printf("MODE: %i, %i x %i %s hz:%s\n", r_mode->integer, glConfig.videoWidth, glConfig.videoHeight, (glConfig.isFullscreen) ? "fullscreen" : "windowed", (glConfig.displayFrequency) ? va("%i", glConfig.displayFrequency) : "N/A");
	Com_Printf("GAMMA: %s w/ %i overbright bits\n", (glConfig.deviceSupportsGamma) ? "hardware" : "software", (glConfig.deviceSupportsGamma) ? r_overBrightBits->integer : 0);
	Com_Printf("CPU: %s\n", Cvar_VariableString("sys_cpuString"));
	Com_Printf("picmip: %i\n", r_picmip->integer);
	Com_Printf("texture bits: %i\n", r_textureBits->integer);
	Com_Printf("texture filter: %s\n", r_textureFilter->string);
	Com_Printf("multitexture: %s\n", (glConfig.multitexture) ? "enabled" : "disabled");
	Com_Printf("texture env add: %s\n", (glConfig.textureEnvAdd) ? "enabled" : "disabled");
	Com_Printf("texture env combine: %s\n", (glConfig.textureEnvCombine) ? "enabled" : "disabled");
	Com_Printf("texture env dot3: %s\n", (glConfig.textureEnvDot3) ? "enabled" : "disabled");
	Com_Printf("texture cube map: %s\n", (glConfig.textureCubeMap) ? "enabled" : "disabled");
	Com_Printf("texture compression: %s\n", (glConfig.textureCompression) ? "enabled" : "disabled");
	Com_Printf("vertex buffer object: %s\n", (glConfig.vertexBufferObject) ? "enabled" : "disabled");
	Com_Printf("vertex program: %s\n", (glConfig.vertexProgram) ? "enabled" : "disabled");
	Com_Printf("fragment program %s\n", (glConfig.fragmentProgram) ? "enabled" : "disabled");
	Com_Printf("draw range elements: %s\n", (glConfig.drawRangeElements) ? "enabled" : "disabled");
	Com_Printf("compiled vertex array: %s\n", (glConfig.compiledVertexArray) ? "enabled" : "disabled");
	Com_Printf("texture edge clamp: %s\n", (glConfig.textureEdgeClamp) ? "enabled" : "disabled");
	Com_Printf("texture filter anisotropic: %s\n", (glConfig.textureFilterAnisotropic) ? "enabled" : "disabled");
	Com_Printf("texture rectangle: %s\n", (glConfig.textureRectangle) ? "enabled" : "disabled");
	Com_Printf("stencil two side: %s\n", (glConfig.stencilTwoSide || glConfig.separateStencil) ? "enabled" : "disabled");
	Com_Printf("generate mipmap: %s\n", (glConfig.generateMipmap) ? "enabled" : "disabled");
	Com_Printf("swap control: %s\n", (glConfig.swapControl) ? "enabled" : "disabled");
	Com_Printf("\n");
}

/*
==============================================================================

	CONFIG REGISTRATION
	
	- Registers the renderer's cvars/commands and gets the latched ones during a vid_restart
==============================================================================
*/  

/*
 ==================
 R_RegisterConfig
 ==================
*/
void R_RegisterConfig ()
{
	r_noRefresh                     = Cvar_Get ("r_noRefresh",                         "0",         CVAR_CHEAT);
	r_noVis                         = Cvar_Get ("r_noVis",                             "0",         CVAR_CHEAT);
	r_noCull                        = Cvar_Get ("r_noCull",                            "0",         CVAR_CHEAT);
	r_noBind                        = Cvar_Get ("r_noBind",                            "0",         CVAR_CHEAT);
	r_drawWorld                     = Cvar_Get ("r_drawWorld",                         "1",         CVAR_CHEAT);
	r_drawEntities                  = Cvar_Get ("r_drawEntities",                      "1",         CVAR_CHEAT);
	r_drawParticles                 = Cvar_Get ("r_drawParticles",                     "1",         CVAR_CHEAT);
	r_drawPolys                     = Cvar_Get ("r_drawPolys",                         "1",         CVAR_CHEAT);
	r_fullbright                    = Cvar_Get ("r_fullbright",                        "0",         CVAR_CHEAT | CVAR_LATCH);
	r_lightmap                      = Cvar_Get ("r_lightmap",                          "0",         CVAR_CHEAT | CVAR_LATCH);
	r_lockPVS                       = Cvar_Get ("r_lockPVS",                           "0",         CVAR_CHEAT);
	r_logFile                       = Cvar_Get ("r_logFile",                           "0",         CVAR_CHEAT);
	r_frontBuffer                   = Cvar_Get ("r_frontBuffer",                       "0",         CVAR_CHEAT);
	r_showCluster                   = Cvar_Get ("r_showCluster",                       "0",         CVAR_CHEAT);
	r_showTris                      = Cvar_Get ("r_showTris",                          "0",         CVAR_CHEAT);
	r_showNormals                   = Cvar_Get ("r_showNormals",                       "0",         CVAR_CHEAT);
	r_showTangentSpace              = Cvar_Get ("r_showTangentSpace",                  "0",         CVAR_CHEAT);
	r_showModelBounds               = Cvar_Get ("r_showModelBounds",                   "0",         CVAR_CHEAT);
	r_showShadowVolumes             = Cvar_Get ("r_showShadowVolumes",                 "0",         CVAR_CHEAT);
	r_offsetFactor                  = Cvar_Get ("r_offsetFactor",                      "-1",        CVAR_CHEAT);
	r_offsetUnits                   = Cvar_Get ("r_offsetUnits",                       "-2",        CVAR_CHEAT);
	r_debugSort                     = Cvar_Get ("r_debugSort",                         "0",         CVAR_CHEAT);
	r_speeds                        = Cvar_Get ("r_speeds",                            "0",         CVAR_CHEAT);
	r_clear                         = Cvar_Get ("r_clear",                             "0",         CVAR_CHEAT);
	r_singleShader                  = Cvar_Get ("r_singleShader",                      "0",         CVAR_CHEAT | CVAR_LATCH);
	r_skipBackEnd                   = Cvar_Get ("r_skipBackEnd",                       "0",         CVAR_CHEAT);
	r_skipFrontEnd                  = Cvar_Get ("r_skipFrontEnd",                      "0",         CVAR_CHEAT);
	r_allowSoftwareGL               = Cvar_Get ("r_allowSoftwareGL",                   "0",         CVAR_LATCH);
	r_maskMiniDriver                = Cvar_Get ("r_maskMiniDriver",                    "0",         CVAR_LATCH);
	r_glDriver                      = Cvar_Get ("r_glDriver",                          GL_DRIVER_OPENGL,    CVAR_ARCHIVE | CVAR_LATCH);
	r_allowExtensions               = Cvar_Get ("r_allowExtensions",                   "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_multitexture              = Cvar_Get ("r_arb_multitexture",                  "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_texture_env_add           = Cvar_Get ("r_arb_texture_env_add",               "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_texture_env_combine       = Cvar_Get ("r_arb_texture_env_combine",           "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_texture_env_dot3          = Cvar_Get ("r_arb_texture_env_dot3",              "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_texture_cube_map          = Cvar_Get ("r_arb_texture_cube_map",              "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_texture_compression       = Cvar_Get ("r_arb_texture_compression",           "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_vertex_buffer_object      = Cvar_Get ("r_arb_vertex_buffer_object",          "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_vertex_program            = Cvar_Get ("r_arb_vertex_program",                "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_arb_fragment_program          = Cvar_Get ("r_arb_fragment_program",              "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_draw_range_elements       = Cvar_Get ("r_ext_draw_range_elements",           "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_compiled_vertex_array     = Cvar_Get ("r_ext_compiled_vertex_array",         "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_texture_edge_clamp        = Cvar_Get ("r_ext_texture_edge_clamp",            "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_texture_filter_anisotropic= Cvar_Get ("r_ext_texture_filter_anisotropic",    "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_texture_rectangle         = Cvar_Get ("r_ext_texture_rectangle",             "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_stencil_two_side          = Cvar_Get ("r_ext_stencil_two_side",              "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_generate_mipmap           = Cvar_Get ("r_ext_generate_mipmap",               "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ext_swap_control              = Cvar_Get ("r_ext_swap_control",                  "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_swapInterval                  = Cvar_Get ("r_swapInterval",                      "0",         CVAR_ARCHIVE);
	r_finish                        = Cvar_Get ("r_finish",                            "0",         CVAR_ARCHIVE);
	r_stereo                        = Cvar_Get ("r_stereo",                            "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_colorBits                     = Cvar_Get ("r_colorBits",                         "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_depthBits                     = Cvar_Get ("r_depthBits",                         "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_stencilBits                   = Cvar_Get ("r_stencilBits",                       "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_mode                          = Cvar_Get ("r_mode",                              "3",         CVAR_ARCHIVE | CVAR_LATCH);
	r_fullscreen                    = Cvar_Get ("r_fullscreen",                        "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_customWidth                   = Cvar_Get ("r_customWidth",                       "1600",      CVAR_ARCHIVE | CVAR_LATCH);
	r_customHeight                  = Cvar_Get ("r_customHeight",                      "1024",      CVAR_ARCHIVE | CVAR_LATCH);
	r_displayRefresh                = Cvar_Get ("r_displayRefresh",                    "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ignoreHwGamma                 = Cvar_Get ("r_ignoreHwGamma",                     "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_gamma                         = Cvar_Get ("r_gamma",                             "1.0",       CVAR_ARCHIVE);
	r_overBrightBits                = Cvar_Get ("r_overBrightBits",                    "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_ignoreGLErrors                = Cvar_Get ("r_ignoreGLErrors",                    "1",         CVAR_ARCHIVE);
	r_shadows                       = Cvar_Get ("r_shadows",                           "0",         CVAR_ARCHIVE);
	r_caustics                      = Cvar_Get ("r_caustics",                          "0",         CVAR_ARCHIVE);
	r_lodBias                       = Cvar_Get ("r_lodBias",                           "0",         CVAR_ARCHIVE);
	r_lodDistance                   = Cvar_Get ("r_lodDistance",                       "250.0",     CVAR_ARCHIVE);
	r_ambientScale                  = Cvar_Get ("r_ambientScale",                      "0.6",       CVAR_ARCHIVE);
	r_directedScale                 = Cvar_Get ("r_directedScale",                     "1.0",       CVAR_ARCHIVE);
	r_intensity                     = Cvar_Get ("r_intensity",                         "1.0",       CVAR_ARCHIVE | CVAR_LATCH);
	r_roundImagesDown               = Cvar_Get ("r_roundImagesDown",                   "1",         CVAR_ARCHIVE | CVAR_LATCH);
	r_maxTextureSize                = Cvar_Get ("r_maxTextureSize",                    "512",       CVAR_ARCHIVE | CVAR_LATCH);
	r_picmip                        = Cvar_Get ("r_picmip",                            "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_textureBits                   = Cvar_Get ("r_textureBits",                       "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_textureFilter                 = Cvar_Get ("r_textureFilter",                     "GL_LINEAR_MIPMAP_LINEAR",    CVAR_ARCHIVE);
	r_textureFilterAnisotropy       = Cvar_Get ("r_textureFilterAnisotropy",           "2.0",       CVAR_ARCHIVE);
	r_jpegCompressionQuality        = Cvar_Get ("r_jpegCompressionQuality",            "75",        CVAR_ARCHIVE);
	r_detailTextures                = Cvar_Get ("r_detailTextures",                    "0",         CVAR_ARCHIVE | CVAR_LATCH);
	r_inGameVideo                   = Cvar_Get ("r_inGameVideo",                       "0",         CVAR_ARCHIVE);
	
	// These we cannot change due to some modes uses them as they are
	gl_dynamic                      = Cvar_Get ("gl_dynamic",                          "1",         CVAR_ARCHIVE);
	gl_modulate                     = Cvar_Get ("gl_modulate",                         "1.0",       CVAR_ARCHIVE | CVAR_LATCH);

	// Add the various commands
	Cmd_AddCommand ("screenshot",          R_ScreenShot_f);
	Cmd_AddCommand ("screenshotJPEG",      R_ScreenShot_f);
	Cmd_AddCommand ("envshot",             R_EnvShot_f);
	Cmd_AddCommand ("gfxinfo",			   R_GfxInfo_f);
	Cmd_AddCommand ("vboinfo",             RB_VBOInfo_f);
	Cmd_AddCommand ("modelist",            R_ModeList_f);
	Cmd_AddCommand ("texturelist",         R_TextureList_f);
	Cmd_AddCommand ("programlist",         R_ProgramList_f);
	Cmd_AddCommand ("shaderlist",          R_ShaderList_f);
	Cmd_AddCommand ("modellist",           R_ModelList_f);

	// Range check some cvars
	if (r_gamma->value > 3.0)
		Cvar_ForceSet("r_gamma", "3.0");
	else if (r_gamma->value < 0.5)
		Cvar_ForceSet("r_gamma", "0.5");

	if (r_overBrightBits->integer > 2)
		Cvar_ForceSet("r_overBrightBits", "2");
	else if (r_overBrightBits->integer < 0)
		Cvar_ForceSet("r_overBrightBits", "0");

	if (gl_modulate->value < 1.0)
		Cvar_ForceSet("gl_modulate", "1.0");

	if (r_intensity->value < 1.0)
		Cvar_ForceSet("r_intensity", "1.0");
}

/*
 ==================
 R_UnRegisterConfig
 ==================
*/
void R_UnRegisterConfig ()
{
	Cmd_RemoveCommand ("screenshot");
	Cmd_RemoveCommand ("screenshotJPEG");
	Cmd_RemoveCommand ("envshot");
	Cmd_RemoveCommand ("gfxinfo");
	Cmd_RemoveCommand ("vboinfo");
	Cmd_RemoveCommand ("modelist");
	Cmd_RemoveCommand ("texturelist");
	Cmd_RemoveCommand ("programlist");
	Cmd_RemoveCommand ("shaderlist");
	Cmd_RemoveCommand ("modellist");
}
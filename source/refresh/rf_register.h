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
// rf_register.h
//

extern cvar_t	*r_noRefresh;
extern cvar_t	*r_noVis;
extern cvar_t	*r_noCull;
extern cvar_t	*r_noBind;
extern cvar_t	*r_drawWorld;
extern cvar_t	*r_drawEntities;
extern cvar_t	*r_drawParticles;
extern cvar_t	*r_drawPolys;
extern cvar_t	*r_fullbright;
extern cvar_t	*r_lightmap;
extern cvar_t	*r_lockPVS;
extern cvar_t	*r_logFile;
extern cvar_t	*r_frontBuffer;
extern cvar_t	*r_showCluster;
extern cvar_t	*r_showTris;
extern cvar_t	*r_showNormals;
extern cvar_t	*r_showTangentSpace;
extern cvar_t	*r_showModelBounds;
extern cvar_t	*r_showShadowVolumes;
extern cvar_t	*r_offsetFactor;
extern cvar_t	*r_offsetUnits;
extern cvar_t	*r_debugSort;
extern cvar_t	*r_speeds;
extern cvar_t	*r_clear;
extern cvar_t	*r_singleShader;
extern cvar_t	*r_skipBackEnd;
extern cvar_t	*r_skipFrontEnd;
extern cvar_t	*r_allowSoftwareGL;
extern cvar_t	*r_maskMiniDriver;
extern cvar_t	*r_glDriver;
extern cvar_t	*r_allowExtensions;
extern cvar_t	*r_arb_multitexture;
extern cvar_t	*r_arb_texture_env_add;
extern cvar_t	*r_arb_texture_env_combine;
extern cvar_t	*r_arb_texture_env_dot3;
extern cvar_t	*r_arb_texture_cube_map;
extern cvar_t	*r_arb_texture_compression;
extern cvar_t	*r_arb_vertex_buffer_object;
extern cvar_t	*r_arb_vertex_program;
extern cvar_t	*r_arb_fragment_program;
extern cvar_t	*r_ext_draw_range_elements;
extern cvar_t	*r_ext_compiled_vertex_array;
extern cvar_t	*r_ext_texture_edge_clamp;
extern cvar_t	*r_ext_texture_filter_anisotropic;
extern cvar_t	*r_ext_texture_rectangle;
extern cvar_t	*r_ext_stencil_two_side;
extern cvar_t	*r_ext_generate_mipmap;
extern cvar_t	*r_ext_swap_control;
extern cvar_t	*r_swapInterval;
extern cvar_t	*r_finish;
extern cvar_t	*r_stereo;
extern cvar_t	*r_colorBits;
extern cvar_t	*r_depthBits;
extern cvar_t	*r_stencilBits;
extern cvar_t	*r_mode;
extern cvar_t	*r_fullscreen;
extern cvar_t	*r_customWidth;
extern cvar_t	*r_customHeight;
extern cvar_t	*r_displayRefresh;
extern cvar_t	*r_ignoreHwGamma;
extern cvar_t	*r_gamma;
extern cvar_t	*r_overBrightBits;
extern cvar_t	*r_ignoreGLErrors;
extern cvar_t	*r_shadows;
extern cvar_t	*r_caustics;
extern cvar_t	*r_lodBias;
extern cvar_t	*r_lodDistance;
extern cvar_t	*gl_dynamic;
extern cvar_t	*gl_modulate;
extern cvar_t	*r_ambientScale;
extern cvar_t	*r_directedScale;
extern cvar_t	*r_intensity;
extern cvar_t	*r_roundImagesDown;
extern cvar_t	*r_maxTextureSize;
extern cvar_t	*r_picmip;
extern cvar_t	*r_textureBits;
extern cvar_t	*r_textureFilter;
extern cvar_t	*r_textureFilterAnisotropy;
extern cvar_t	*r_jpegCompressionQuality;
extern cvar_t	*r_detailTextures;
extern cvar_t	*r_inGameVideo;

//
// Functions
//
void        R_GfxInfo_f ();
void        R_RegisterConfig ();
void        R_UnRegisterConfig ();

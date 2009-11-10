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


#include "r_local.h"


mat4_t			r_projectionMatrix;
mat4_t			r_worldMatrix;
mat4_t			r_entityMatrix;
mat4_t			r_textureMatrix;

cplane_t		r_frustum[4];

float			r_frameTime;

mesh_t			r_solidMeshes[MAX_MESHES];
int				r_numSolidMeshes;

mesh_t			r_transMeshes[MAX_MESHES];
int				r_numTransMeshes;

entity_t		r_entities[MAX_ENTITIES];
int				r_numEntities;

dlight_t		r_dlights[MAX_DLIGHTS];
int				r_numDLights;

particle_t		r_particles[MAX_PARTICLES];
int				r_numParticles;

poly_t			r_polys[MAX_POLYS];
int				r_numPolys;
polyVert_t		r_polyVerts[MAX_POLY_VERTS];
int				r_numPolyVerts;

entity_t		*r_nullModels[MAX_ENTITIES];
int				r_numNullModels;

lightStyle_t	r_lightStyles[MAX_LIGHTSTYLES];

refDef_t		r_refDef;

refStats_t		r_stats;

#define NUM_VIDEO_MODES	(sizeof(r_videoModes) / sizeof(videoMode_t))

typedef struct {
	const char	*description;
	int			width, height;
	int			mode;
} videoMode_t;

videoMode_t		r_videoModes[] = {
	{"Mode  0: 320x240",			 320,  240,  0},
	{"Mode  1: 400x300",			 400,  300,  1},
	{"Mode  2: 512x384",			 512,  384,  2},
	{"Mode  3: 640x480",			 640,  480,  3},
	{"Mode  4: 800x600",			 800,  600,  4},
	{"Mode  5: 960x720",			 960,  720,  5},
	{"Mode  6: 1024x768",			1024,  768,  6},
	{"Mode  7: 1152x864",			1152,  864,  7},
	{"Mode  8: 1280x1024",			1280, 1024,  8},
	{"Mode  9: 1600x1200",			1600, 1200,  9},
	{"Mode 10: 2048x1536",			2048, 1536, 10},
	{"Mode 11: 856x480 (wide)",		 856,  480, 11},
	{"Mode 12: 1920x1200 (wide)",	1920, 1200, 12},
};

/*
 ==================
 R_ModeList_f
 ==================
*/
void R_ModeList_f ()
{
	int     i;

	Com_Printf ("Mode -1: %ix%i (custom)\n", r_customWidth->integer, r_customHeight->integer);

	for (i = 0; i < NUM_VIDEO_MODES; i++)
		Com_Printf ("%s\n", r_videoModes[i].description);
}

/*
 =================
 R_RotateForEntity
 =================
*/
void R_RotateForEntity (entity_t *entity){

	mat4_t	rotateMatrix;

	rotateMatrix[ 0] = entity->axis[0][0];
	rotateMatrix[ 1] = entity->axis[0][1];
	rotateMatrix[ 2] = entity->axis[0][2];
	rotateMatrix[ 3] = 0.0;
	rotateMatrix[ 4] = entity->axis[1][0];
	rotateMatrix[ 5] = entity->axis[1][1];
	rotateMatrix[ 6] = entity->axis[1][2];
	rotateMatrix[ 7] = 0.0;
	rotateMatrix[ 8] = entity->axis[2][0];
	rotateMatrix[ 9] = entity->axis[2][1];
	rotateMatrix[10] = entity->axis[2][2];
	rotateMatrix[11] = 0.0;
	rotateMatrix[12] = entity->origin[0];
	rotateMatrix[13] = entity->origin[1];
	rotateMatrix[14] = entity->origin[2];
	rotateMatrix[15] = 1.0;

	Matrix4_MultiplyFast(r_worldMatrix, rotateMatrix, r_entityMatrix);

	qglLoadMatrixf(r_entityMatrix);
}


// =====================================================================


/*
 =================
 R_AddSpriteModelToList

 I am only keeping this for backwards compatibility
 =================
*/
static void R_AddSpriteModelToList (entity_t *entity){

	spr_t		*sprite = entity->model->sprite;
	sprFrame_t	*frame;
	vec3_t		vec;

	frame = &sprite->frames[entity->frame % sprite->numFrames];

	// Cull
	if (!r_noCull->integer){
		VectorSubtract(entity->origin, r_refDef.viewOrigin, vec);
		VectorNormalizeFast(vec);

		if (DotProduct(vec, r_refDef.viewAxis[0]) < 0)
			return;
	}

	// HACK: make it a sprite entity
	entity->entityType = ET_SPRITE;
	entity->radius = frame->radius;
	entity->rotation = 0;
	entity->customShader = frame->shader;

	// HACK: make it translucent
	if (!(entity->customShader->flags & SHADER_EXTERNAL)){
		entity->customShader->sort = SORT_BLEND;
		entity->customShader->stages[0]->flags |= SHADERSTAGE_BLENDFUNC;
		entity->customShader->stages[0]->flags &= ~SHADERSTAGE_DEPTHWRITE;
		entity->customShader->stages[0]->blendFunc.src = GL_SRC_ALPHA;
		entity->customShader->stages[0]->blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	}

	// Add it
	R_AddMeshToList(MESH_SPRITE, NULL, entity->customShader, entity, 0);
}

/*
 =================
 R_DrawSprite
 =================
*/
void R_DrawSprite (void){

	vec3_t	axis[3];
	int		i;

	if (rb_entity->rotation){
		// Rotate it around its normal
		RotatePointAroundVector(axis[1], r_refDef.viewAxis[0], r_refDef.viewAxis[1], rb_entity->rotation);
		CrossProduct(r_refDef.viewAxis[0], axis[1], axis[2]);

		// The normal should point at the viewer
		VectorNegate(r_refDef.viewAxis[0], axis[0]);

		// Scale the axes by radius
		VectorScale(axis[1], rb_entity->radius, axis[1]);
		VectorScale(axis[2], rb_entity->radius, axis[2]);
	}
	else {
		// The normal should point at the viewer
		VectorNegate(r_refDef.viewAxis[0], axis[0]);

		// Scale the axes by radius
		VectorScale(r_refDef.viewAxis[1], rb_entity->radius, axis[1]);
		VectorScale(r_refDef.viewAxis[2], rb_entity->radius, axis[2]);
	}

	// Draw it
	RB_CheckMeshOverflow(6, 4);
	
	for (i = 2; i < 4; i++){
		indexArray[numIndex++] = numVertex + 0;
		indexArray[numIndex++] = numVertex + i-1;
		indexArray[numIndex++] = numVertex + i;
	}

	vertexArray[numVertex+0][0] = rb_entity->origin[0] + axis[1][0] + axis[2][0];
	vertexArray[numVertex+0][1] = rb_entity->origin[1] + axis[1][1] + axis[2][1];
	vertexArray[numVertex+0][2] = rb_entity->origin[2] + axis[1][2] + axis[2][2];
	vertexArray[numVertex+1][0] = rb_entity->origin[0] - axis[1][0] + axis[2][0];
	vertexArray[numVertex+1][1] = rb_entity->origin[1] - axis[1][1] + axis[2][1];
	vertexArray[numVertex+1][2] = rb_entity->origin[2] - axis[1][2] + axis[2][2];
	vertexArray[numVertex+2][0] = rb_entity->origin[0] - axis[1][0] - axis[2][0];
	vertexArray[numVertex+2][1] = rb_entity->origin[1] - axis[1][1] - axis[2][1];
	vertexArray[numVertex+2][2] = rb_entity->origin[2] - axis[1][2] - axis[2][2];
	vertexArray[numVertex+3][0] = rb_entity->origin[0] + axis[1][0] - axis[2][0];
	vertexArray[numVertex+3][1] = rb_entity->origin[1] + axis[1][1] - axis[2][1];
	vertexArray[numVertex+3][2] = rb_entity->origin[2] + axis[1][2] - axis[2][2];

	inTexCoordArray[numVertex+0][0] = 0;
	inTexCoordArray[numVertex+0][1] = 0;
	inTexCoordArray[numVertex+1][0] = 1;
	inTexCoordArray[numVertex+1][1] = 0;
	inTexCoordArray[numVertex+2][0] = 1;
	inTexCoordArray[numVertex+2][1] = 1;
	inTexCoordArray[numVertex+3][0] = 0;
	inTexCoordArray[numVertex+3][1] = 1;

	for (i = 0; i < 4; i++){
		normalArray[numVertex][0] = axis[0][0];
		normalArray[numVertex][1] = axis[0][1];
		normalArray[numVertex][2] = axis[0][2];
		inColorArray[numVertex][0] = rb_entity->shaderRGBA[0];
		inColorArray[numVertex][1] = rb_entity->shaderRGBA[1];
		inColorArray[numVertex][2] = rb_entity->shaderRGBA[2];
		inColorArray[numVertex][3] = rb_entity->shaderRGBA[3];

		numVertex++;
	}
}

/*
 =================
 R_AddSpriteToList
 =================
*/
static void R_AddSpriteToList (entity_t *entity){

	vec3_t	vec;

	// Cull
	if (!r_noCull->integer){
		VectorSubtract(entity->origin, r_refDef.viewOrigin, vec);
		VectorNormalizeFast(vec);

		if (DotProduct(vec, r_refDef.viewAxis[0]) < 0)
			return;
	}

	// Add it
	R_AddMeshToList(MESH_SPRITE, NULL, entity->customShader, entity, 0);
}

/*
 =================
 R_DrawBeam
 =================
*/
void R_DrawBeam (void){

	vec3_t	axis[3];
	float	length;
	int		i;

	// Find orientation vectors
	VectorSubtract(r_refDef.viewOrigin, rb_entity->origin, axis[0]);
	VectorSubtract(rb_entity->oldOrigin, rb_entity->origin, axis[1]);

	CrossProduct(axis[0], axis[1], axis[2]);
	VectorNormalizeFast(axis[2]);

	// Find normal
	CrossProduct(axis[1], axis[2], axis[0]);
	VectorNormalizeFast(axis[0]);

	// Scale by radius
	VectorScale(axis[2], rb_entity->frame / 2, axis[2]);

	// Find segment length
	length = VectorLength(axis[1]) / rb_entity->oldFrame;

	// Draw it
	RB_CheckMeshOverflow(6, 4);
	
	for (i = 2; i < 4; i++){
		indexArray[numIndex++] = numVertex + 0;
		indexArray[numIndex++] = numVertex + i-1;
		indexArray[numIndex++] = numVertex + i;
	}

	vertexArray[numVertex+0][0] = rb_entity->origin[0] + axis[2][0];
	vertexArray[numVertex+0][1] = rb_entity->origin[1] + axis[2][1];
	vertexArray[numVertex+0][2] = rb_entity->origin[2] + axis[2][2];
	vertexArray[numVertex+1][0] = rb_entity->oldOrigin[0] + axis[2][0];
	vertexArray[numVertex+1][1] = rb_entity->oldOrigin[1] + axis[2][1];
	vertexArray[numVertex+1][2] = rb_entity->oldOrigin[2] + axis[2][2];
	vertexArray[numVertex+2][0] = rb_entity->oldOrigin[0] - axis[2][0];
	vertexArray[numVertex+2][1] = rb_entity->oldOrigin[1] - axis[2][1];
	vertexArray[numVertex+2][2] = rb_entity->oldOrigin[2] - axis[2][2];
	vertexArray[numVertex+3][0] = rb_entity->origin[0] - axis[2][0];
	vertexArray[numVertex+3][1] = rb_entity->origin[1] - axis[2][1];
	vertexArray[numVertex+3][2] = rb_entity->origin[2] - axis[2][2];

	inTexCoordArray[numVertex+0][0] = 0;
	inTexCoordArray[numVertex+0][1] = 0;
	inTexCoordArray[numVertex+1][0] = length;
	inTexCoordArray[numVertex+1][1] = 0;
	inTexCoordArray[numVertex+2][0] = length;
	inTexCoordArray[numVertex+2][1] = 1;
	inTexCoordArray[numVertex+3][0] = 0;
	inTexCoordArray[numVertex+3][1] = 1;

	for (i = 0; i < 4; i++){
		normalArray[numVertex][0] = axis[0][0];
		normalArray[numVertex][1] = axis[0][1];
		normalArray[numVertex][2] = axis[0][2];
		inColorArray[numVertex][0] = rb_entity->shaderRGBA[0];
		inColorArray[numVertex][1] = rb_entity->shaderRGBA[1];
		inColorArray[numVertex][2] = rb_entity->shaderRGBA[2];
		inColorArray[numVertex][3] = rb_entity->shaderRGBA[3];

		numVertex++;
	}
}

/*
 =================
 R_AddBeamToList
 =================
*/
static void R_AddBeamToList (entity_t *entity){

	R_AddMeshToList(MESH_BEAM, NULL, entity->customShader, entity, 0);
}

/*
 =================
 R_AddEntitiesToList
 =================
*/
static void R_AddEntitiesToList (void){

	entity_t	*entity;
	model_t		*model;
	int			i;

	if (!r_drawEntities->integer || r_numEntities == 1)
		return;

	r_stats.numEntities += (r_numEntities - 1);

	for (i = 1, entity = &r_entities[1]; i < r_numEntities; i++, entity++){
		switch (entity->entityType){
		case ET_MODEL:
			model = entity->model;

			if (!model || model->modelType == MODEL_BAD){
				r_nullModels[r_numNullModels++] = entity;
				break;
			}

			switch (model->modelType){
			case MODEL_BSP:
				R_AddBrushModelToList(entity);
				break;
			case MODEL_MD3:
			case MODEL_MD2:
				R_AddAliasModelToList(entity);
				break;
			case MODEL_SP2:
				R_AddSpriteModelToList(entity);
				break;
			default:
				Com_Error(ERR_DROP, "R_AddEntitiesToList: bad modelType (%i)", model->modelType);
			}

			break;
		case ET_SPRITE:
			R_AddSpriteToList(entity);
			break;
		case ET_BEAM:
			R_AddBeamToList(entity);
			break;
		default:
			Com_Error(ERR_DROP, "R_AddEntitiesToList: bad entityType (%i)", entity->entityType);
		}
	}
}

/*
 =================
 R_DrawNullModels
 =================
*/
static void R_DrawNullModels (void){

	entity_t	*entity;
	vec3_t		points[3];
	int			i;

	if (!r_numNullModels)
		return;

	qglLoadMatrixf(r_worldMatrix);

	// Set the state
	GL_Enable(GL_CULL_FACE);
	GL_Disable(GL_POLYGON_OFFSET_FILL);
	GL_Disable(GL_VERTEX_PROGRAM_ARB);
	GL_Disable(GL_FRAGMENT_PROGRAM_ARB);
	GL_Disable(GL_ALPHA_TEST);
	GL_Enable(GL_BLEND);
	GL_Enable(GL_DEPTH_TEST);

	GL_CullFace(GL_FRONT);
	GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL_DepthFunc(GL_LEQUAL);
	GL_DepthMask(GL_FALSE);

	// Draw them
	for (i = 0; i < r_numNullModels; i++){
		entity = r_nullModels[i];

		VectorMA(entity->origin, 15, entity->axis[0], points[0]);
		VectorMA(entity->origin, -15, entity->axis[1], points[1]);
		VectorMA(entity->origin, 15, entity->axis[2], points[2]);

		qglBegin(GL_LINES);

		qglColor4ub(255, 0, 0, 127);
		qglVertex3fv(entity->origin);
		qglVertex3fv(points[0]);

		qglColor4ub(0, 255, 0, 127);
		qglVertex3fv(entity->origin);
		qglVertex3fv(points[1]);

		qglColor4ub(0, 0, 255, 127);
		qglVertex3fv(entity->origin);
		qglVertex3fv(points[2]);

		qglEnd();
	}

	r_numNullModels = 0;
}

/*
 =================
 R_DrawParticle
 =================
*/
void R_DrawParticle (void){

	particle_t	*particle = rb_mesh->mesh;
	vec3_t		axis[3];
	int			i;

	// Draw it
	RB_CheckMeshOverflow(6, 4);
	
	for (i = 2; i < 4; i++)
	{
		indexArray[numIndex++] = numVertex + 0;
		indexArray[numIndex++] = numVertex + i-1;
		indexArray[numIndex++] = numVertex + i;
	}

	if (particle->length != 1)
	{
		// Find orientation vectors
		VectorSubtract(r_refDef.viewOrigin, particle->origin, axis[0]);
		VectorSubtract(particle->oldOrigin, particle->origin, axis[1]);
		CrossProduct(axis[0], axis[1], axis[2]);

		VectorNormalizeFast(axis[1]);
		VectorNormalizeFast(axis[2]);

		// Find normal
		CrossProduct(axis[1], axis[2], axis[0]);
		VectorNormalizeFast(axis[0]);

		VectorMA(particle->origin, -particle->length, axis[1], particle->oldOrigin);
		VectorScale(axis[2], particle->radius, axis[2]);

		vertexArray[numVertex+0][0] = particle->oldOrigin[0] + axis[2][0];
		vertexArray[numVertex+0][1] = particle->oldOrigin[1] + axis[2][1];
		vertexArray[numVertex+0][2] = particle->oldOrigin[2] + axis[2][2];
		vertexArray[numVertex+1][0] = particle->origin[0] + axis[2][0];
		vertexArray[numVertex+1][1] = particle->origin[1] + axis[2][1];
		vertexArray[numVertex+1][2] = particle->origin[2] + axis[2][2];
		vertexArray[numVertex+2][0] = particle->origin[0] - axis[2][0];
		vertexArray[numVertex+2][1] = particle->origin[1] - axis[2][1];
		vertexArray[numVertex+2][2] = particle->origin[2] - axis[2][2];
		vertexArray[numVertex+3][0] = particle->oldOrigin[0] - axis[2][0];
		vertexArray[numVertex+3][1] = particle->oldOrigin[1] - axis[2][1];
		vertexArray[numVertex+3][2] = particle->oldOrigin[2] - axis[2][2];
	}
	else 
	{
		if (particle->rotation)
		{
			// Rotate it around its normal
			RotatePointAroundVector(axis[1], r_refDef.viewAxis[0], r_refDef.viewAxis[1], particle->rotation);
			CrossProduct(r_refDef.viewAxis[0], axis[1], axis[2]);

			// The normal should point at the viewer
			VectorNegate(r_refDef.viewAxis[0], axis[0]);

			// Scale the axes by radius
			VectorScale(axis[1], particle->radius, axis[1]);
			VectorScale(axis[2], particle->radius, axis[2]);
		}
		else 
		{
			// The normal should point at the viewer
			VectorNegate(r_refDef.viewAxis[0], axis[0]);

			// Scale the axes by radius
			VectorScale(r_refDef.viewAxis[1], particle->radius, axis[1]);
			VectorScale(r_refDef.viewAxis[2], particle->radius, axis[2]);
		}

		// Top right
		vertexArray[numVertex+0][0] = particle->origin[0] + axis[1][0] + axis[2][0];
		vertexArray[numVertex+0][1] = particle->origin[1] + axis[1][1] + axis[2][1]; 
		vertexArray[numVertex+0][2] = particle->origin[2] + axis[1][2] + axis[2][2];

		// Bottom right
		vertexArray[numVertex+1][0] = particle->origin[0] - axis[1][0] + axis[2][0];
		vertexArray[numVertex+1][1] = particle->origin[1] - axis[1][1] + axis[2][1];
		vertexArray[numVertex+1][2] = particle->origin[2] - axis[1][2] + axis[2][2];

		// Bottom left
		vertexArray[numVertex+2][0] = particle->origin[0] - axis[1][0] - axis[2][0];
		vertexArray[numVertex+2][1] = particle->origin[1] - axis[1][1] - axis[2][1];
		vertexArray[numVertex+2][2] = particle->origin[2] - axis[1][2] - axis[2][2];

		// Top left
		vertexArray[numVertex+3][0] = particle->origin[0] + axis[1][0] - axis[2][0];
		vertexArray[numVertex+3][1] = particle->origin[1] + axis[1][1] - axis[2][1];
		vertexArray[numVertex+3][2] = particle->origin[2] + axis[1][2] - axis[2][2];
	}

	// Draw texture onto the triangle
	inTexCoordArray[numVertex+0][0] = 0;
	inTexCoordArray[numVertex+0][1] = 0;
	inTexCoordArray[numVertex+1][0] = 1;
	inTexCoordArray[numVertex+1][1] = 0;
	inTexCoordArray[numVertex+2][0] = 1;
	inTexCoordArray[numVertex+2][1] = 1;
	inTexCoordArray[numVertex+3][0] = 0;
	inTexCoordArray[numVertex+3][1] = 1;

	for (i = 0; i < 4; i++)
	{
		// Draw the triangle normal
		normalArray[numVertex][0] = axis[0][0];
		normalArray[numVertex][1] = axis[0][1];
		normalArray[numVertex][2] = axis[0][2];

		// Draw RGBA
		inColorArray[numVertex][0] = particle->modulate[0];
		inColorArray[numVertex][1] = particle->modulate[1];
		inColorArray[numVertex][2] = particle->modulate[2];
		inColorArray[numVertex][3] = particle->modulate[3];

		numVertex++;
	}
}

/*
 =================
 R_AddParticlesToList
 =================
*/
static void R_AddParticlesToList (void){

	particle_t	*particle;
	vec3_t		vec;
	int			i;

	if (!r_drawParticles->integer || !r_numParticles)
		return;

	r_stats.numParticles += r_numParticles;

	for (i = 0, particle = r_particles; i < r_numParticles; i++, particle++){
		// Cull
		if (!r_noCull->integer){
			VectorSubtract(particle->origin, r_refDef.viewOrigin, vec);
			VectorNormalizeFast(vec);

			if (DotProduct(vec, r_refDef.viewAxis[0]) < 0)
				continue;
		}

		// Add it
		R_AddMeshToList(MESH_PARTICLE, particle, particle->shader, r_worldEntity, 0);
	}
}

/*
 =================
 R_DrawPoly
 =================
*/
void R_DrawPoly (void){

	poly_t		*poly = rb_mesh->mesh;
	polyVert_t	*vert;
	int			i;

	RB_CheckMeshOverflow((poly->numVerts-2) * 3, poly->numVerts);

	for (i = 2; i < poly->numVerts; i++){
		indexArray[numIndex++] = numVertex + 0;
		indexArray[numIndex++] = numVertex + i-1;
		indexArray[numIndex++] = numVertex + i;
	}

	for (i = 0, vert = poly->verts; i < poly->numVerts; i++, vert++){
		vertexArray[numVertex][0] = vert->xyz[0];
		vertexArray[numVertex][1] = vert->xyz[1];
		vertexArray[numVertex][2] = vert->xyz[2];
		inTexCoordArray[numVertex][0] = vert->st[0];
		inTexCoordArray[numVertex][1] = vert->st[1];
		inColorArray[numVertex][0] = vert->modulate[0];
		inColorArray[numVertex][1] = vert->modulate[1];
		inColorArray[numVertex][2] = vert->modulate[2];
		inColorArray[numVertex][3] = vert->modulate[3];

		numVertex++;
	}
}

/*
 =================
 R_AddPolysToList
 =================
*/
static void R_AddPolysToList (void){

	poly_t	*poly;
	int		i;

	if (!r_drawPolys->integer || !r_numPolys)
		return;

	r_stats.numPolys += r_numPolys;

	for (i = 0, poly = r_polys; i < r_numPolys; i++, poly++)
		R_AddMeshToList(MESH_POLY, poly, poly->shader, r_worldEntity, 0);
}


// =====================================================================


/*
 =================
 R_QSortMeshes
 =================
*/
static void R_QSortMeshes (mesh_t *meshes, int numMeshes){

	static mesh_t	tmp;
	static int		stack[4096];
	int				depth = 0;
	int				L, R, l, r, median;
	unsigned		pivot;

	if (!numMeshes)
		return;

	L = 0;
	R = numMeshes - 1;

start:
	l = L;
	r = R;

	median = (L + R) >> 1;

	if (meshes[L].sortKey > meshes[median].sortKey){
		if (meshes[L].sortKey < meshes[R].sortKey) 
			median = L;
	} 
	else if (meshes[R].sortKey < meshes[median].sortKey)
		median = R;

	pivot = meshes[median].sortKey;

	while (l < r){
		while (meshes[l].sortKey < pivot)
			l++;
		while (meshes[r].sortKey > pivot)
			r--;

		if (l <= r){
			tmp = meshes[r];
			meshes[r] = meshes[l];
			meshes[l] = tmp;

			l++;
			r--;
		}
	}

	if ((L < r) && (depth < 4096)){
		stack[depth++] = l;
		stack[depth++] = R;
		R = r;
		goto start;
	}

	if (l < R){
		L = l;
		goto start;
	}

	if (depth){
		R = stack[--depth];
		L = stack[--depth];
		goto start;
	}
}

/*
 =================
 R_ISortMeshes
 =================
*/
static void R_ISortMeshes (mesh_t *meshes, int numMeshes){

	static mesh_t	tmp;
	int				i, j;

	if (!numMeshes)
		return;

	for (i = 1; i < numMeshes; i++){
		tmp = meshes[i];
		j = i - 1;

		while ((j >= 0) && (meshes[j].sortKey > tmp.sortKey)){
			meshes[j+1] = meshes[j];
			j--;
		}

		if (i != j+1)
			meshes[j+1] = tmp;
	}
}

/*
 =================
 R_AddMeshToList

 Calculates sort key and stores info used for sorting and batching.
 All 3D geometry passes this function.
 =================
*/
void R_AddMeshToList (meshType_t meshType, void *mesh, shader_t *shader, entity_t *entity, int infoKey){

	mesh_t	*m;

	if (shader->sort <= SORT_DECAL){
		if (r_numSolidMeshes == MAX_MESHES)
			Com_Error(ERR_DROP, "R_AddMeshToList: MAX_MESHES hit");

		m = &r_solidMeshes[r_numSolidMeshes++];
	}
	else {
		if (r_numTransMeshes == MAX_MESHES)
			Com_Error(ERR_DROP, "R_AddMeshToList: MAX_MESHES hit");

		m = &r_transMeshes[r_numTransMeshes++];
	}

	m->sortKey = (shader->sort << 28) | (shader->shaderNum << 18) | ((entity - r_entities) << 8) | (infoKey);

	m->meshType = meshType;
	m->mesh = mesh;
}


// =====================================================================


/*
 =================
 R_SetFrustum
 =================
*/
static void R_SetFrustum (void){

	int		i;

	RotatePointAroundVector(r_frustum[0].normal, r_refDef.viewAxis[2], r_refDef.viewAxis[0], -(90 - r_refDef.fovX / 2));
	RotatePointAroundVector(r_frustum[1].normal, r_refDef.viewAxis[2], r_refDef.viewAxis[0], 90 - r_refDef.fovX / 2);
	RotatePointAroundVector(r_frustum[2].normal, r_refDef.viewAxis[1], r_refDef.viewAxis[0], 90 - r_refDef.fovY / 2);
	RotatePointAroundVector(r_frustum[3].normal, r_refDef.viewAxis[1], r_refDef.viewAxis[0], -(90 - r_refDef.fovY / 2));

	for (i = 0; i < 4; i++){
		r_frustum[i].type = PLANE_NON_AXIAL;
		r_frustum[i].dist = DotProduct(r_refDef.viewOrigin, r_frustum[i].normal);
		SetPlaneSignbits(&r_frustum[i]);
	}
}

/*
 =================
 R_SetFarClip
 =================
*/
static float R_SetFarClip (void){

	float	farDist, dirDist, worldDist = 0;
	int		i;

	if (r_refDef.rdFlags & RDF_NOWORLDMODEL)
		return 4096.0;

	dirDist = DotProduct(r_refDef.viewOrigin, r_refDef.viewAxis[0]);
	farDist = dirDist + 256.0;

	for (i = 0; i < 3; i++){
		if (r_refDef.viewAxis[0][i] < 0)
			worldDist += (r_worldMins[i] * r_refDef.viewAxis[0][i]);
		else
			worldDist += (r_worldMaxs[i] * r_refDef.viewAxis[0][i]);
	}

	if (farDist < worldDist)
		farDist = worldDist;

	return farDist - dirDist + 256.0;
}

/*
 =================
 R_SetMatrices
 =================
*/
static void R_SetMatrices (void){

	float	xMax, xMin, yMax, yMin;
	float	xDiv, yDiv, zDiv;
	float	zNear, zFar;

	zNear = 4.0;
	zFar = R_SetFarClip();

	xMax = zNear * tan(r_refDef.fovX * M_PI / 360.0);
	xMin = -xMax;

	yMax = zNear * tan(r_refDef.fovY * M_PI / 360.0);
	yMin = -yMax;

	if (glConfig.stereoEnabled){
		xMax += -(2 * glState.stereoSeparation) / zNear;
		yMin += -(2 * glState.stereoSeparation) / zNear;
	}

	xDiv = 1.0 / (xMax - xMin);
	yDiv = 1.0 / (yMax - yMin);
	zDiv = 1.0 / (zFar - zNear);

	r_projectionMatrix[ 0] = (2.0 * zNear) * xDiv;
	r_projectionMatrix[ 1] = 0.0;
	r_projectionMatrix[ 2] = 0.0;
	r_projectionMatrix[ 3] = 0.0;
	r_projectionMatrix[ 4] = 0.0;
	r_projectionMatrix[ 5] = (2.0 * zNear) * yDiv;
	r_projectionMatrix[ 6] = 0.0;
	r_projectionMatrix[ 7] = 0.0;
	r_projectionMatrix[ 8] = (xMax + xMin) * xDiv;
	r_projectionMatrix[ 9] = (yMax + yMin) * yDiv;
	r_projectionMatrix[10] = -(zNear + zFar) * zDiv;
	r_projectionMatrix[11] = -1.0;
	r_projectionMatrix[12] = 0.0;
	r_projectionMatrix[13] = 0.0;
	r_projectionMatrix[14] = -(2.0 * zNear * zFar) * zDiv;
	r_projectionMatrix[15] = 0.0;

	r_worldMatrix[ 0] = -r_refDef.viewAxis[1][0];
	r_worldMatrix[ 1] = r_refDef.viewAxis[2][0];
	r_worldMatrix[ 2] = -r_refDef.viewAxis[0][0];
	r_worldMatrix[ 3] = 0.0;
	r_worldMatrix[ 4] = -r_refDef.viewAxis[1][1];
	r_worldMatrix[ 5] = r_refDef.viewAxis[2][1];
	r_worldMatrix[ 6] = -r_refDef.viewAxis[0][1];
	r_worldMatrix[ 7] = 0.0;
	r_worldMatrix[ 8] = -r_refDef.viewAxis[1][2];
	r_worldMatrix[ 9] = r_refDef.viewAxis[2][2];
	r_worldMatrix[10] = -r_refDef.viewAxis[0][2];
	r_worldMatrix[11] = 0.0;
	r_worldMatrix[12] = DotProduct(r_refDef.viewOrigin, r_refDef.viewAxis[1]);
	r_worldMatrix[13] = -DotProduct(r_refDef.viewOrigin, r_refDef.viewAxis[2]);
	r_worldMatrix[14] = DotProduct(r_refDef.viewOrigin, r_refDef.viewAxis[0]);
	r_worldMatrix[15] = 1.0;

	r_textureMatrix[ 0] = r_refDef.viewAxis[1][0];
	r_textureMatrix[ 1] = -r_refDef.viewAxis[1][1];
	r_textureMatrix[ 2] = -r_refDef.viewAxis[1][2];
	r_textureMatrix[ 3] = 0.0;
	r_textureMatrix[ 4] = -r_refDef.viewAxis[2][0];
	r_textureMatrix[ 5] = r_refDef.viewAxis[2][1];
	r_textureMatrix[ 6] = r_refDef.viewAxis[2][2];
	r_textureMatrix[ 7] = 0.0;
	r_textureMatrix[ 8] = r_refDef.viewAxis[0][0];
	r_textureMatrix[ 9] = -r_refDef.viewAxis[0][1];
	r_textureMatrix[10] = -r_refDef.viewAxis[0][2];
	r_textureMatrix[11] = 0.0;
	r_textureMatrix[12] = 0.0;
	r_textureMatrix[13] = 0.0;
	r_textureMatrix[14] = 0.0;
	r_textureMatrix[15] = 1.0;
}

/*
 =================
 R_SetGammaRamp
 =================
*/
static void R_SetGammaRamp (void){

	float	gamma;
	int		i, v;

	if (r_gamma->value > 3.0)
		Cvar_SetValue("r_gamma", 3.0);
	else if (r_gamma->value < 0.5)
		Cvar_SetValue("r_gamma", 0.5);

	gamma = (1.0 / (float)(1 << r_overBrightBits->integer)) / r_gamma->value;

	for (i = 0; i < 256; i++){
		v = 255 * pow(i / 255.0, gamma);
		v = Clamp(v, 0, 255);

		glState.gammaRamp[i] = glState.gammaRamp[i+256] = glState.gammaRamp[i+512] = ((unsigned short)v) << 8;
	}

	GLimp_SetDeviceGammaRamp(glState.gammaRamp);
}

/*
 =================
 R_RenderView
 =================
*/
void R_RenderView (void){

	if (r_skipFrontEnd->integer)
		return;

	r_numSolidMeshes = 0;
	r_numTransMeshes = 0;

	// Set up frustum
	R_SetFrustum();

	// Build mesh lists
	R_AddWorldToList();
	R_AddEntitiesToList();
	R_AddParticlesToList();
	R_AddPolysToList();

	// Sort mesh lists
	R_QSortMeshes(r_solidMeshes, r_numSolidMeshes);
	R_ISortMeshes(r_transMeshes, r_numTransMeshes);

	// Set up matrices
	R_SetMatrices();

	// Go into 3D mode
	GL_Setup3D();

	// Render everything
	RB_RenderMeshes(r_solidMeshes, r_numSolidMeshes);

	R_RenderShadows();

	RB_RenderMeshes(r_transMeshes, r_numTransMeshes);

	// Finish up
	R_DrawNullModels();
}

/*
 =================
 R_ClearScene
 =================
*/
void R_ClearScene (void){

	r_numEntities = 1;
	r_numDLights = 0;
	r_numParticles = 0;
	r_numPolys = 0;
	r_numPolyVerts = 0;
}

/*
 =================
 R_AddEntityToScene
 =================
*/
void R_AddEntityToScene (const entity_t *entity){

	if (r_numEntities >= MAX_ENTITIES)
		return;

	r_entities[r_numEntities++] = *entity;
}

/*
 =================
 R_AddLightToScene
 =================
*/
void R_AddLightToScene (const vec3_t origin, float intensity, float r, float g, float b){

	dlight_t	*dl;

	if (r_numDLights >= MAX_DLIGHTS)
		return;

	dl = &r_dlights[r_numDLights++];

	VectorCopy(origin, dl->origin);
	VectorSet(dl->color, r, g, b);
	dl->intensity = intensity;
}

/*
 =================
 R_AddParticleToScene
 =================
*/
void R_AddParticleToScene (shader_t *shader, const vec3_t origin, const vec3_t oldOrigin, float radius, float length, float rotation, const color_t modulate){

	particle_t	*particle;

	if (r_numParticles >= MAX_PARTICLES)
		return;

	particle = &r_particles[r_numParticles++];

	particle->shader = shader;
	VectorCopy(origin, particle->origin);
	VectorCopy(oldOrigin, particle->oldOrigin);
	particle->radius = radius;
	particle->length = length;
	particle->rotation = rotation;
	MakeRGBA(particle->modulate, modulate[0], modulate[1], modulate[2], modulate[3]);
}

/*
 =================
 R_AddPolyToScene
 =================
*/
void R_AddPolyToScene (shader_t *shader, int numVerts, const polyVert_t *verts){

	poly_t		*poly;

	if (r_numPolys >= MAX_POLYS || r_numPolyVerts + numVerts > MAX_POLY_VERTS)
		return;

	poly = &r_polys[r_numPolys++];

	poly->shader = shader;
	poly->numVerts = numVerts;
	poly->verts = &r_polyVerts[r_numPolyVerts];

	memcpy(poly->verts, verts, numVerts * sizeof(polyVert_t));
	r_numPolyVerts += numVerts;
}

/*
 =================
 R_RenderScene
 =================
*/
void R_RenderScene (const refDef_t *rd){

	if (r_noRefresh->integer)
		return;

	r_refDef = *rd;

	if (!(r_refDef.rdFlags & RDF_NOWORLDMODEL)){
		if (!r_worldModel)
			Com_Error(ERR_DROP, "R_RenderScene: NULL worldmodel");
	}

	// Make sure all 2D stuff is flushed
	RB_RenderMesh();

	// Render view
	R_RenderView();

	// Go into 2D mode
	GL_Setup2D();
}

/*
 =================
 R_SetLightStyle
 =================
*/
void R_SetLightStyle (int style, float r, float g, float b){

	lightStyle_t	*ls;

	if (style < 0 || style >= MAX_LIGHTSTYLES)
		return;

	ls = &r_lightStyles[style];

	ls->white = r + g + b;
	VectorSet(ls->rgb, r, g, b);
}

/*
 =================
 R_LerpTag
 =================
*/
qboolean R_LerpTag (tag_t *tag, model_t *model, int curFrame, int oldFrame, float backLerp, const char *tagName){

	int			i;
	mdl_t		*alias;
	mdlTag_t	*curTag, *oldTag;

	if (model->modelType != MODEL_MD3)
		return false;

	alias = model->alias[0];

	// Find the tag
	for (i = 0; i < alias->numTags; i++){
		if (!Q_stricmp(alias->tags[i].name, tagName))
			break;
	}

	if (i == alias->numTags){
		Com_DPrintf(S_COLOR_YELLOW "R_LerpTag: no such tag %s (%s)\n", tagName, model->realName);
		return false;
	}

	if ((curFrame < 0 || curFrame >= alias->numFrames) || (oldFrame < 0 || oldFrame >= alias->numFrames)){
		Com_DPrintf(S_COLOR_YELLOW "R_LerpTag: no such frame %i to %i (%s)\n", curFrame, oldFrame, model->realName);
		return false;
	}

	curTag = alias->tags + alias->numTags * curFrame + i;
	oldTag = alias->tags + alias->numTags * oldFrame + i;

	// Interpolate origin
	tag->origin[0] = curTag->origin[0] + (oldTag->origin[0] - curTag->origin[0]) * backLerp;
	tag->origin[1] = curTag->origin[1] + (oldTag->origin[1] - curTag->origin[1]) * backLerp;
	tag->origin[2] = curTag->origin[2] + (oldTag->origin[2] - curTag->origin[2]) * backLerp;

	// Interpolate axes
	tag->axis[0][0] = curTag->axis[0][0] + (oldTag->axis[0][0] - curTag->axis[0][0]) * backLerp;
	tag->axis[0][1] = curTag->axis[0][1] + (oldTag->axis[0][1] - curTag->axis[0][1]) * backLerp;
	tag->axis[0][2] = curTag->axis[0][2] + (oldTag->axis[0][2] - curTag->axis[0][2]) * backLerp;
	tag->axis[1][0] = curTag->axis[1][0] + (oldTag->axis[1][0] - curTag->axis[1][0]) * backLerp;
	tag->axis[1][1] = curTag->axis[1][1] + (oldTag->axis[1][1] - curTag->axis[1][1]) * backLerp;
	tag->axis[1][2] = curTag->axis[1][2] + (oldTag->axis[1][2] - curTag->axis[1][2]) * backLerp;
	tag->axis[2][0] = curTag->axis[2][0] + (oldTag->axis[2][0] - curTag->axis[2][0]) * backLerp;
	tag->axis[2][1] = curTag->axis[2][1] + (oldTag->axis[2][1] - curTag->axis[2][1]) * backLerp;
	tag->axis[2][2] = curTag->axis[2][2] + (oldTag->axis[2][2] - curTag->axis[2][2]) * backLerp;

	// Normalize axes
	VectorNormalize(tag->axis[0]);
	VectorNormalize(tag->axis[1]);
	VectorNormalize(tag->axis[2]);

	return true;
}

/*
 =================
 R_GetGLConfig

 Used by other systems to get the GL config
 =================
*/
void R_GetGLConfig (glConfig_t *config){

	if (!config)
		return;

	*config = glConfig;
}

/*
 =================
 R_GetModeInfo
 =================
*/
qboolean R_GetModeInfo (int *width, int *height, int mode){

	if (mode == -1){
		if (r_customWidth->integer < 1 || r_customHeight->integer < 1)
			return false;

		*width = r_customWidth->integer;
		*height = r_customHeight->integer;
		return true;
	}

	if (mode < 0 || mode >= NUM_VIDEO_MODES)
		return false;

	*width = r_videoModes[mode].width;
	*height = r_videoModes[mode].height;

	return true;
}

/*
 =================
 R_BeginFrame
 =================
*/
void R_BeginFrame (int realTime, float stereoSeparation){

	// Set frame time
	r_frameTime = realTime * 0.001;

	// Clear r_speeds statistics
	memset(&r_stats, 0, sizeof(refStats_t));

	// Log file
	if (r_logFile->modified){
		QGL_EnableLogging(r_logFile->integer);

		r_logFile->modified = false;
	}

	if (r_logFile->integer)
		QGL_LogPrintf("\n======= R_BeginFrame =======\n\n");	

	// Update gamma
	if (r_gamma->modified){
		R_SetGammaRamp();

		r_gamma->modified = false;
	}

	// Update texture filtering
	if (r_textureFilter->modified || r_textureFilterAnisotropy->modified){
		R_TextureFilter();

		r_textureFilter->modified = false;
		r_textureFilterAnisotropy->modified = false;
	}

	// Set draw buffer
	glState.stereoSeparation = stereoSeparation;

	if (!glConfig.stereoEnabled){
		if (glState.stereoSeparation != 0)
			Com_Error(ERR_DROP, "R_BeginFrame: stereoSeparation != 0 with stereo disabled");

		if (r_frontBuffer->integer)
			qglDrawBuffer(GL_FRONT);
		else
			qglDrawBuffer(GL_BACK);
	}
	else {
		if (glState.stereoSeparation == 0)
			Com_Error(ERR_DROP, "R_BeginFrame: stereoSeparation == 0 with stereo enabled");

		if (r_frontBuffer->integer)
			qglDrawBuffer((glState.stereoSeparation < 0) ? GL_FRONT_LEFT : GL_FRONT_RIGHT);
		else
			qglDrawBuffer((glState.stereoSeparation < 0) ? GL_BACK_LEFT : GL_BACK_RIGHT);
	}

	// Clear screen if desired
	if (r_clear->integer){
		GL_DepthMask(GL_TRUE);

		qglClearColor(1.0, 0.0, 0.5, 0.5);
		qglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Go into 2D mode
	GL_Setup2D();

	// Check for errors
	if (!r_ignoreGLErrors->integer)
		GL_CheckForErrors();
}

/*
 =================
 R_EndFrame
 =================
*/
void R_EndFrame (void){

	// Make sure all 2D stuff is flushed
	RB_RenderMesh();

	// Swap the buffers
	GLimp_SwapBuffers();

	// Print r_speeds statistics
	if (r_speeds->integer){
		switch (r_speeds->integer){
		case 1:
			Com_Printf("%i/%i shaders/stages %i meshes %i leafs %i verts %i/%i tris\n", r_stats.numShaders, r_stats.numStages, r_stats.numMeshes, r_stats.numLeafs, r_stats.numVertices, (r_stats.numIndices / 3), (r_stats.totalIndices / 3));
			break;
		case 2:
			Com_Printf("%i entities %i dlights %i particles %i polys\n", r_stats.numEntities, r_stats.numDLights, r_stats.numParticles, r_stats.numPolys);
			break;
		}
	}

	// Log file
	if (r_logFile->integer){
		QGL_LogPrintf("\n======= R_EndFrame =======\n\n");

		if (r_logFile->integer > 0)		// Negative is infinite logging
			Cvar_SetInteger("r_logFile", r_logFile->integer - 1);
	}

	// Check for errors
	if (!r_ignoreGLErrors->integer)
		GL_CheckForErrors();
}

/*
 =================
 R_Init
 =================
*/
void R_Init (qboolean all){

	Com_Printf("----- R_Init -----\n");

	if (all){
		R_RegisterConfig ();

		GLimp_Init();
	}

	RB_InitBackend();

	R_GfxInfo_f();

	R_InitTextures();
	R_InitPrograms();
	R_InitShaders();
	R_InitModels();

	if (!r_ignoreGLErrors->integer)
		GL_CheckForErrors();

	Com_Printf("----- finished R_Init -----\n");
}

/*
 =================
 R_Shutdown
 =================
*/
void R_Shutdown (qboolean all){

	Com_Printf("R_Shutdown( %i )\n", all);

	R_ShutdownModels();
	R_ShutdownShaders();
	R_ShutdownPrograms();
	R_ShutdownTextures();

	RB_ShutdownBackend();

	if (all){
		GLimp_Shutdown();

		R_UnRegisterConfig ();
	}

	Hunk_ClearToLowMark();
}

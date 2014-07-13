#include "mesh.h"

int is_blank_line(char *s){
	while(*s)
		if(!isspace(*s++))
			return 0;
	return 1;
}

static int line_num = 0;

int get_line(char buf[], int bufsize, FILE *file){
	line_num++;
	return fgets(buf, bufsize, file)? ((buf[0]=='#')||is_blank_line(buf)? get_line(buf, bufsize, file) : 1) : 0;
}

float dot3(float a[3], float b[3]){
	int i;
	float s = 0.0f;
	for(i = 0; i<3; i++)
		s += a[i]*b[i];
	return s;
}

void calc_face_normal(mesh_t *mesh, float n[3], int fi){
	int *t = mesh->tris[fi].iv;
	int i;
	float a[3], b[3];	/* first and second edge vectors */
	for(i = 0; i<3; i++){
		a[i] = mesh->verts[t[1]].p[i]-mesh->verts[t[0]].p[i];
		b[i] = mesh->verts[t[2]].p[i]-mesh->verts[t[1]].p[i];
	}
	/* n := a x b; */
	n[0] = a[1]*b[2]-a[2]*b[1];
	n[1] = a[2]*b[0]-a[0]*b[2];
	n[2] = a[0]*b[1]-a[1]*b[0];
}


/* This function is intended to only be called once, to construct the display
 * list.  Thereafter the display list should be used. */
void mesh_draw(mesh_t *mesh){
	fprintf(stderr, "Regenerating mesh for OpenGL...\n"
			"Normal style: ");
	switch(normal_style){
	case NS_DIV_BY_AREA:
		fprintf(stderr, "div by area\n");
		break;
	case NS_NORMALIZE:
		fprintf(stderr, "normalize\n");
		break;
	case NS_MUL_BY_AREA:
		break;
	case NS_FLAT:
		fprintf(stderr, "flat\n");
		break;
	default:
		fprintf(stderr, "unknown\n");
		break;
	}

	int i, j, k;
	float *norms = (float *)malloc(mesh->nt*3*sizeof(float));

	/* Compute vertex normals */
	if(normal_style!=NS_FLAT){
		fprintf(stderr, "Computing vertex normals.\n");

		/* First, zero out the normal accumulators at the vertices */
		for(i = 0; i<mesh->nv; i++){
			for(k = 0; k<3; k++){
				mesh->verts[i].n[k] = 0.0f;
			}
		}

		/* Average the face normals at the vertices */
		for(i = 0; i<mesh->nt; i++){
			float *n = &norms[i*3];			/* face normal */
			int *t = mesh->tris[i].iv;
			calc_face_normal(mesh, n, i);
			float nlen2 = dot3(n, n);

			/* Triangles with greater area contribute less to the
			 * computation of the vertex normal, since they probably
			 * are less accurate indications of the vertex normal.
			 * Anyway, that's how it seems on paper, but in practice
			 * this idea doesn't work so well.  More investigation
			 * is needed into computing good normals for triangle meshes.
			 * -ijt
			 */
			if(nlen2>0.0f){
				float nlen;
				switch(normal_style){
				case NS_DIV_BY_AREA:
					nlen = nlen2;
					break;
				case NS_NORMALIZE:
					nlen = sqrtf(nlen2);
					break;
				case NS_MUL_BY_AREA:
				case NS_FLAT:
					nlen = 1.0f;	/* not used */
					break;
				default:
					fprintf(stderr, "bad normal style\n");
					nlen = 1.0f;
					break;
				}
				for(j = 0; j<3; j++){
					n[j] /= nlen;
				}

				for(j = 0; j<3; j++){
					for(k = 0; k<3; k++){
						mesh->verts[t[j]].n[k] += n[k];
					}
				}
			}
		}

		/* Normalize */
		int num_zero_normals = 0;

		for(i = 0; i<mesh->nv; i++){
			float *n = mesh->verts[i].n;
			float nlen2 = dot3(n, n);
			if(nlen2==0.0f){
				num_zero_normals++;
			}
			else{
				float nlen = sqrtf(nlen2);
				for(k = 0; k<3; k++){
					n[k] /= nlen;
				}
			}
		}

		if(num_zero_normals){
			fprintf(stderr, "%i normals have zero length.\n", num_zero_normals);
		}
	}

	/* Convert the surface to a format that OpenGL can easily digest. */
	fprintf(stderr, "Converting mesh to a raw OpenGL format...\n");
	gl_triangle_t *gl_tris = (gl_triangle_t *)malloc(mesh->nt*sizeof(gl_triangle_t));
	for(i = 0; i<mesh->nt; i++){
		float *n = &norms[i*3];
		gl_triangle_t *glt = &gl_tris[i];
		int *t = mesh->tris[i].iv;
		if(normal_style==NS_FLAT){
			calc_face_normal(mesh, n, i);
		}
		for(j = 0; j<3; j++){
			vertex_t *v = &mesh->verts[t[j]];
			for(k = 0; k<3; k++) {
				glt->vv[j].n[k] = normal_style==NS_FLAT? n[k] : v->n[k];
				glt->vv[j].p[k] = v->p[k];
			}
		}

		/* Flip vertex normals to match the triangle normal.
		 * We can do this because these new vertices are duplicated for each
		 * triangle.
		 */
		for(j = 0; j<3; j++){
			if(dot3(glt->vv[j].n, n)<0.0){
				for(k = 0; k<3; k++){
					glt->vv[j].n[k] = 0-glt->vv[j].n[k];
				}
			}
		}
	}
	if(verbosity>=1)
		fprintf(stderr, "Done.\n");

	/* Send the data to OpenGL. */
	glInterleavedArrays(GL_N3F_V3F, /* stride */ 0, gl_tris);
	glDrawArrays(GL_TRIANGLES, 0, mesh->nt*3);

	free(gl_tris);
}

static mesh_t *checkmesh(lua_State *L){
	mesh_t *m = (mesh_t *)luaL_checkudata(L, 1, "brainmaps_mesh");
	luaL_argcheck(L, m!=NULL, 1, "`mesh' expected");
	return m;
}

static int l_mesh_get_bounds(lua_State *L){
	mesh_t *m = checkmesh(L);
	int i;
	for(i = 0; i<3; i++)
		lua_pushnumber(L, m->pmin[i]);
	for(i = 0; i<3; i++)
		lua_pushnumber(L, m->pmax[i]);
	return 6;
}

/* The passed in mesh is expected to have just its own memory allocated. */
int mesh_load(mesh_t *mesh, FILE *file){
	int i, j;
	/* Load the mesh */
	char buf[80];

	line_num = 0;
	if(!get_line(buf, sizeof(buf), file)){
		fprintf(stderr, "Premature eof.\n");
		return -1;
	}
	if(sscanf(buf, "%d %d", &mesh->nv, &mesh->nt)!=2){
		fprintf(stderr, "Bad format.  First non-comment line must contain two integers:\nthe vertex count followed by the triangle count.\n");
		return -1;
	}
	if(mesh->nv<=0){
		fprintf(stderr, "Vertex count must be positive, not %i\n", mesh->nv);
		return -1;
	}
	if(mesh->nt<=0){
		fprintf(stderr, "Triangle count must be positive, not %i\n", mesh->nt);
		return -1;
	}

	mesh->verts = (vertex_t *)malloc(mesh->nv*sizeof(vertex_t));
	mesh->tris = (triangle_t *)malloc(mesh->nt*sizeof(triangle_t));
	if((!mesh->verts)||(!mesh->tris)){
		fprintf(stderr, "Memory allocation failed.\n");
		return -1;
	}

	for(i = 0; i<mesh->nv; i++){
		if(!get_line(buf, sizeof(buf), file)){
			fprintf(stderr, "Premature eof (vertices).\n");
			return -1;
		}

		float *v = mesh->verts[i].p;
		if(sscanf(buf, "%f %f %f", &v[0], &v[1], &v[2])!=3){
			fprintf(stderr, "Expected three vertex indices but got %s on line %i\n", buf, line_num);
		}

		/* Compute the bounding box. */
		if(i==0){
			for(j = 0; j<3; j++){
				mesh->pmin[j] = mesh->pmax[j] = v[j];
			}
		}
		else{
			for(j = 0; j<3; j++){
				mesh->pmin[j] = MIN(mesh->pmin[j], v[j]);
				mesh->pmax[j] = MAX(mesh->pmax[j], v[j]);
			}
		}
	}

	for(i = 0; i<mesh->nt; i++){
		if(!get_line(buf, sizeof(buf), file)){
			fprintf(stderr, "Premature eof (triangles).\n");
			return -1;
		}

		int *v = mesh->tris[i].iv;
		if(sscanf(buf, "%d %d %d", &v[0], &v[1], &v[2])!=3){
			fprintf(stderr, "Bad format on line %d. Expected three vertex indices, but got\n%s\n", line_num, buf);
			return -1;
		}
	}

	/* Set up the display list */
	mesh->gl_display_list = glGenLists(1);
	glNewList(mesh->gl_display_list, GL_COMPILE);
	mesh_draw(mesh);
	glEndList();

	return 0;						/* ok */
}

/* Lua book, p. 250 */
static int l_mesh_load(lua_State *L){
	const char *filename = luaL_checkstring(L, 1);
	mesh_t *m = (mesh_t *)lua_newuserdata(L, sizeof(mesh_t));
	FILE *file = fopen(filename, "r");

	if(!file){
		luaL_error(L, "Could not open %s for reading.\n", filename);
	}

	/* Set the mesh's metatable. */
	luaL_getmetatable(L, "brainmaps_mesh");
	lua_setmetatable(L, -2);

	if(mesh_load(m, file)){
		luaL_error(L, "Failed to load mesh file %s", filename);
	}
	fclose(file);

	return 1;	/* The mesh pointer is on top of the stack. */
}

static int l_mesh_draw(lua_State *L){
	mesh_t *m = checkmesh(L);
	glCallList(m->gl_display_list);
	return 0;
}

#define mesh_ENTRY(f)	{# f, l_mesh_ ## f}

/* mesh class functions */
static const struct luaL_Reg meshlib_f[] = {
	mesh_ENTRY(load),
	{NULL, NULL}
};

/* mesh object methods */
static const struct luaL_Reg meshlib_m[] = {
	mesh_ENTRY(draw),
	mesh_ENTRY(get_bounds),
	{NULL, NULL}
};

static int mesh_gc(lua_State *L){
	mesh_t *m = checkmesh(L);
	if(verbosity>=1){
		fprintf(stderr, "Freeing mesh %p\n", m);
	}
	if(m){
		glDeleteLists(m->gl_display_list, 1);
		free(m->verts);
		free(m->tris);
	}
	return 0;
}

int luaopen_mesh(lua_State *L){
	luaL_newmetatable(L, "brainmaps_mesh");

	/* Set the __gc field of the metatable so that meshes will be garbage
	 * collected. */
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, mesh_gc);
	lua_settable(L, -3);

	/* p. 245 */
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);	/* pushes the metatable */
	lua_settable(L, -3);	/* metatable.__index = metatable */

	luaL_setfuncs(L, meshlib_m, 0);
	luaL_newlib(L, meshlib_f);
	return 1;
}


#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "utils.h"
#include "mesh.h"

#include <stdio.h>

int mesh_load(mesh_t* mesh, FILE *file);
void mesh_draw(mesh_t* mesh);

int is_blank_line(char *s)
{
    for( ; *s; s++) { if(!isspace(*s)) { return 0; } }
    return 1;
}

static int line_num=0;
int get_line(char buf[], int bufsize, FILE* file)
{
    line_num++;
    return 
        !fgets(buf,bufsize,file)
            ?0
            :(buf[0]=='#'||is_blank_line(buf)
                ?get_line(buf,bufsize,file)
                :1);
}

/* The passed in mesh is expected to have just its own memory allocated. */
int mesh_load(mesh_t* mesh, FILE *file)
{
    /* Load the mesh */
    { 
        int i;
        char buf[80];

        line_num=0;
        if(!get_line(buf,sizeof(buf),file)) {
            fprintf(stderr,"Premature eof.\n");
            return -1;
        }
        if(sscanf(buf,"%i %i",&mesh->nv, &mesh->nt)!=2) {
            fprintf(stderr,
"Bad format.  First non-comment line must contain two integers:\n"
"the vertex count followed by the triangle count.\n");
            return -1;
        }
        if(mesh->nv<=0) {
            fprintf(stderr,"Vertex count must be positive, not %i\n",mesh->nv);
            return -1;
        }
        if(mesh->nt<=0) {
            fprintf(stderr,"Triangle count must be positive, not %i\n",
                    mesh->nt);
            return -1;
        }

        mesh->verts=malloc(mesh->nv*sizeof(vertex_t));
        mesh->tris=malloc(mesh->nt*sizeof(triangle_t));
        if(!(mesh->verts&&mesh->tris)) {
            fprintf(stderr,"Memory allocation failed.\n");
            return -1;
        }

        for(i=0;i<mesh->nv;i++) {
            vertex_t* v=&mesh->verts[i];
            if(!get_line(buf,sizeof(buf),file)) {
                fprintf(stderr,"Premature eof (vertices).\n");
                return -1;
            }
            if(sscanf(buf,"%f %f %f",&v->p[0],&v->p[1],&v->p[2])!=3) {
                fprintf(stderr,
"Expected three vertex indices but got %s on line %i\n",
buf,line_num);
            }
        }
        for(i=0;i<mesh->nt;i++) {
            int* v=mesh->tris[i].iv;
            if(!get_line(buf,sizeof(buf),file)) {
                fprintf(stderr,"Premature eof (triangles).\n");
                return -1;
            }
            if(sscanf(buf,"%i %i %i",&v[0],&v[1],&v[2])!=3) {
                fprintf(stderr,
"Bad format on line %i.  Expected three vertex indices, but got\n"
"%s\n", line_num, buf);
                return -1;
            }
        }
    }

    /* Compute the bounding box. */
    {
        int i,j;
        for(j=0;j<3;j++) { 
            mesh->pmin[j]=mesh->pmax[j]=mesh->verts[0].p[j]; 
        }
        for(i=0;i<mesh->nv;i++) { 
            for(j=0;j<3;j++) {
                mesh->pmin[j]=min(mesh->pmin[j],mesh->verts[i].p[j]);
                mesh->pmax[j]=max(mesh->pmax[j],mesh->verts[i].p[j]);
            }
        }
    }

    /* Set up the display list */
    {   
        mesh->gl_display_list=glGenLists(1);
        glNewList(mesh->gl_display_list, GL_COMPILE);
        mesh_draw(mesh);
        glEndList();
    }

    return 0; /* ok */
}


float dot3(float a[3], float b[3])
{
    int i;
    float s=0.0f;
    for(i=0;i<3;i++) { s+=a[i]*b[i]; }
    return s;
}

void calc_face_normal(mesh_t* mesh, float n[3], int fi)
{
    int k;
    triangle_t* t=&mesh->tris[fi];
    float a[3],b[3]; /* first and second edge vectors */
    for(k=0;k<3;k++) {
        a[k]=mesh->verts[t->iv[1]].p[k]-mesh->verts[t->iv[0]].p[k];
        b[k]=mesh->verts[t->iv[2]].p[k]-mesh->verts[t->iv[1]].p[k];
    }
    /* n := a x b; */
    n[0]=a[1]*b[2]-a[2]*b[1];
    n[1]=a[2]*b[0]-a[0]*b[2];
    n[2]=a[0]*b[1]-a[1]*b[0];
}


/* This function is intended to only be called once, to construct the display 
 * list.  Thereafter the display list should be used. */
void mesh_draw(mesh_t* mesh)
{
    fprintf(stderr,"Regenerating mesh for OpenGL...\n");
    fprintf(stderr,"Normal style: %s\n",
            normal_style==NS_DIV_BY_AREA?"div by area"
            :normal_style==NS_NORMALIZE?"normalize"
            :normal_style==NS_MUL_BY_AREA?"multiply by sqrt(area)"
            :normal_style==NS_FLAT?"flat"
            :"unknown");

    /* Compute vertex normals */
    if(normal_style!=NS_FLAT) 
    {
        int num_zero_normals=0;
        int i,j,k;

        fprintf(stderr,"Computing vertex normals.\n");

        /* First, zero out the normal accumulators at the vertices */
        for(i=0;i<mesh->nv;i++) {
            for(k=0;k<3;k++) {
                mesh->verts[i].n[k]=0.0f;
            }
        }

        /* Average the face normals at the vertices */
        for(i=0; i<mesh->nt; i++) {
            float n[3]; /* face normal */
            float nlen2,nlen;
            triangle_t* t=&mesh->tris[i];
            calc_face_normal(mesh,n,i);
            nlen2=dot3(n,n);
            if(normal_style==NS_NORMALIZE) { nlen=(float)sqrt(nlen2); }

            /* Triangles with greater area contribute less to the 
             * computation of the vertex normal, since they probably
             * are less accurate indications of the vertex normal. 
             * Anyway, that's how it seems on paper, but in practice
             * this idea doesn't work so well.  More investigation 
             * is needed into computing good normals for triangle meshes.
             * -ijt
             * */
            for(k=0;k<3;k++) {
                float normalizer = 
                    (normal_style==NS_DIV_BY_AREA?nlen2 
                     :normal_style==NS_NORMALIZE?nlen 
                     :normal_style==NS_MUL_BY_AREA?1.0
                     :normal_style==NS_FLAT?1.0  /* not used */
                     :(fprintf(stderr,"bad normal style\n"),1.0f));
                n[k] /= (normalizer==0.0?1.0:normalizer); 
            }

            for(j=0;j<3;j++) {
                for(k=0;k<3;k++) {
                    mesh->verts[t->iv[j]].n[k]+=n[k];
                }
            }
        }

        /* Normalize */
        {
            for(i=0;i<mesh->nv;i++) {
                float* n=mesh->verts[i].n;
                float nlen=(float)sqrt(dot3(n,n)); 
                if(nlen==0.0) { 
                    num_zero_normals++;
                }
                else {
                    for(k=0;k<3;k++) { n[k]/=nlen; } 
                }
            }
        }

        if(num_zero_normals>0) { 
            fprintf(stderr,"%i normals have zero length.\n",num_zero_normals);
        }
    }

    /* Convert the surface to a format that OpenGL can easily digest. */
    {
        int i,j,k;
        fprintf(stderr,"Converting mesh to a raw OpenGL format...\n");
        mesh->gl_tris=malloc(mesh->nt*sizeof(gl_triangle_t));
        for(i=0;i<mesh->nt;i++) {
            float n[3];
            gl_triangle_t* glt=&mesh->gl_tris[i];
            triangle_t* t=&mesh->tris[i];
            calc_face_normal(mesh,n,i);
            for(j=0;j<3;j++) {
                vertex_t* v=&mesh->verts[t->iv[j]];
                for(k=0;k<3;k++) {
                    glt->vv[j].n[k]=normal_style==NS_FLAT?n[k]:v->n[k];
                    glt->vv[j].p[k]=v->p[k];
                }
            }

            /* Flip vertex normals to match the triangle normal.
             * We can do this because these new vertices are duplicated for each
             * triangle.
             */
            {
                for(j=0;j<3;j++) {
                    if(dot3(glt->vv[j].n,n)<0.0) {
                        for(k=0;k<3;k++) {
                            glt->vv[j].n[k]*=-1.0;
                        }
                    }
                }
            }
        }
        if(verbosity>=1) { fprintf(stderr,"Done.\n"); }
    }

    /* Send the data to OpenGL. */
    {
        /* glEnableClientState(GL_VERTEX_ARRAY); */
        glInterleavedArrays(GL_N3F_V3F,/*stride*/0,mesh->gl_tris);
        glDrawArrays(GL_TRIANGLES,0,mesh->nt*3);
    }

    free(mesh->gl_tris);
    mesh->gl_tris=NULL;
}

static mesh_t* checkmesh(lua_State* L)
{
    mesh_t *m=(mesh_t*)luaL_checkudata(L, 1, "brainmaps_mesh");    
    luaL_argcheck(L, m != NULL, 1, "`mesh' expected");
    return m;
}

static int l_mesh_get_bounds(lua_State* L)
{
    mesh_t* m = checkmesh(L);
    lua_pushnumber(L,m->pmin[0]);     
    lua_pushnumber(L,m->pmin[1]);     
    lua_pushnumber(L,m->pmin[2]);     

    lua_pushnumber(L,m->pmax[0]);     
    lua_pushnumber(L,m->pmax[1]);     
    lua_pushnumber(L,m->pmax[2]);     
    return 6;
}

/* Lua book, p. 250 */
static int l_mesh_load(lua_State* L)
{
    const char* filename=luaL_checkstring(L,1);
    FILE* file;
    mesh_t* m;

    filename=luaL_checkstring(L,1);
    m = (mesh_t*)lua_newuserdata(L,sizeof(mesh_t));

    /* Set the mesh's metatable. */
    luaL_getmetatable(L,"brainmaps_mesh");
    lua_setmetatable(L,-2);

    file=fopen(filename,"r");
    if(file==NULL) { luaL_error(L,"Could not open %s for reading.\n",filename); }
    if(mesh_load(m,file)!=0) { luaL_error(L,"Failed to load mesh file %s",filename); }
    fclose(file);

    return 1; /* The mesh pointer is on top of the stack. */
}

static int l_mesh_draw(lua_State* L)
{
    mesh_t* m = checkmesh(L);
    glCallList(m->gl_display_list);
    return 0;
}

/* mesh class functions */
static const struct luaL_reg meshlib_f[] = {
    {"load", l_mesh_load},
    {NULL, NULL}
};

/* mesh object methods */
static const struct luaL_reg meshlib_m[] = {
    {"draw", l_mesh_draw}, 
    {"get_bounds", l_mesh_get_bounds},
    {NULL, NULL}
};

static int mesh_gc(lua_State* L)
{
    mesh_t* m = checkmesh(L);
    if(verbosity>=1) { fprintf(stderr,"Freeing mesh %p\n",m); }
    if(m) { 
        glDeleteLists(m->gl_display_list, 1);
        free(m->verts);
        free(m->tris);
    } 
    return 0;
}

int luaopen_mesh(lua_State* L)
{
    luaL_newmetatable(L, "brainmaps_mesh");

    /* Set the __gc field of the metatable so that meshes will be garbage
     * collected. */
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, mesh_gc);
    lua_settable(L, -3);

    /* p. 245 */
    lua_pushstring(L, "__index");
    lua_pushvalue(L,-2); /* pushes the metatable */
    lua_settable(L,-3); /* metatable.__index = metatable */

    luaL_openlib(L, NULL, meshlib_m, 0);
    luaL_openlib(L, "mesh", meshlib_f, 0);
    return 1;
}




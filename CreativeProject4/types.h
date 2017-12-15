//
//  types.h
//  Lab3
//
//  Created by Sam Nosenzo on 10/20/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//

#ifndef types_h
#define types_h


#endif /* types_h */


typedef struct vec4
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

typedef struct mat4
{
    vec4 v0;
    vec4 v1;
    vec4 v2;
    vec4 v3;
    
} mat4;

typedef struct material {
    vec4 reflect_ambient;
    vec4 reflect_diffuse;
    vec4 reflect_specular;
    float shininess;
} material;


typedef struct obj {
    vec4 *vertices;
    vec4 *normals;
    int num_verts;
    size_t buffer_start_loc;
    int mesh_type;
    mat4 translation;
    mat4 rotation;
    float scale;
    material mat;
    vec4 loc;
    vec4 vel;
} obj;



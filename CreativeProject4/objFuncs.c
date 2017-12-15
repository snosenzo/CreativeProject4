//
//  objFuncs.c
//  Shadow_Lab4
//
//  Created by Sam Nosenzo on 11/8/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//
#include "objFuncs.h"

int getNumPointsFromSubDiv(int numSubDivisions){
    return ((int) pow(4, (double) numSubDivisions+1))*3;
}
int Index = 0;
void triangle(const vec4 a, const vec4 b, const vec4 c, obj *o){
    vec4 temp1, temp2, temp3, normal;
    subVectors(&b, &a, &temp1);
    subVectors(&c, &b, &temp2);
    crossProduct(&temp1, &temp2, &temp3);
    scalarMultVector(temp3, 1/magnitude(&temp3), &normal);
    
    vec4 *verts = (vec4 *) o->vertices;
    vec4 *normals = (vec4 *) o->normals;
    normals[Index] = normal;
    verts[Index++] = a;
    normals[Index] = normal;
    verts[Index++] = b;
    normals[Index] = normal;
    verts[Index++] = c;
}

void divide_triangle(const vec4 a, const vec4 b, const vec4 c, int count, obj *o) {
    vec4 t1, t2, t3;
    if( count > 0 ) {
        addVectors(a, b, &t1);
        scalarMultVector(t1, 1/magnitude(&t1), &t1);
        
        addVectors(a, c, &t2);
        scalarMultVector(t2, 1/magnitude(&t2), &t2);
        
        addVectors(b, c, &t3);
        scalarMultVector(t3, 1/magnitude(&t3), &t3);
        
        divide_triangle(a, t1, t2, count - 1, o);
        divide_triangle(c, t2, t3, count - 1, o);
        divide_triangle(b, t3, t1, count - 1, o);
        divide_triangle(t1, t3, t2, count - 1, o);
    } else {
        triangle(a, b, c, o);
    }
}
void tetrahedron(int count, obj *o) {
    Index = 0;
    o->num_verts = getNumPointsFromSubDiv(count);
    o->mesh_type = GL_TRIANGLES;
    o->vertices = (vec4 *) malloc(sizeof(vec4) * o->num_verts);
    o->normals = (vec4 *) malloc(sizeof(vec4) * o->num_verts);
    
    
    vec4 initVs[4];
    defineVector(0.0, 0.0, 1.0, 1.0, &(initVs[0]));
    defineVector(0.0, .942809, -0.333333, 1.0, &(initVs[1]));
    defineVector(-0.816497, -0.471405, -0.333333, 1.0, &(initVs[2]));
    defineVector(0.816497, -0.471405, -0.333333, 1.0, &(initVs[3]));
    
    divide_triangle(initVs[0], initVs[1], initVs[2], count, o);
    divide_triangle(initVs[3], initVs[2], initVs[1], count, o);
    divide_triangle(initVs[0], initVs[3], initVs[1], count, o);
    divide_triangle(initVs[0], initVs[2], initVs[3], count, o);
}


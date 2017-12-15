//
//  vecMatOps.c
//  LabRec3
//
//  Created by Sam Nosenzo on 10/25/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//

#include "vecMatOps.h"
#include <math.h>
#include <stdio.h>


void defineVector(GLfloat x, GLfloat y, GLfloat z, GLfloat w, vec4* vec) {
    vec->x = x;
    vec->y = y;
    vec->z = z;
    vec->w = w;
}

void printVector(vec4* vec) {
    printf("[ %f ]\n", vec->x);
    printf("[ %f ]\n", vec->y);
    printf("[ %f ]\n", vec->z);
    printf("[ %f ]\n\n", vec->w);
}

void crossProduct(vec4 *vec1, vec4 *vec2, vec4* res) {
    // prof says we don't need to do w for crosss product
    res->x = vec1->y * vec2->z - vec1->z * vec2->y;
    res->y = vec1->z * vec2->x - vec1->x * vec2->z;
    res->z = vec1->x * vec2->y - vec1->y * vec2->x;
    res->w = 0.0;
}

GLfloat dotProduct(vec4* vec1, vec4* vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y + vec1->z * vec2->z;
}

void scalarMultVector(vec4 vec, GLfloat scalar, vec4* res) {
    res->x = vec.x * scalar;
    res->y = vec.y * scalar;
    res->z = vec.z * scalar;
    res->w = vec.w;
}

float magnitude(vec4 *v) {
    return sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
}

void addVectors(vec4 vec1, vec4 vec2, vec4* res) {
    res->x = vec1.x + vec2.x;
    res->y = vec1.y + vec2.y;
    res->z = vec1.z + vec2.z;
    res->w = 1.0;
    //    res->w = vec1->w + vec2->w;
}

void subVectors(vec4* vec1, vec4* vec2, vec4* res) {
    res->x = vec1->x - vec2->x;
    res->y = vec1->y - vec2->y;
    res->z = vec1->z - vec2->z;
    res->w = 1.0;
    //    res->w = vec1->w - vec2->w;
}


//MATRIX FUNCTIONS:

void defineMatrix(vec4 v0, vec4 v1, vec4 v2, vec4 v3, mat4* res) {
    res->v0 = v0;
    res->v1 = v1;
    res->v2 = v2;
    res->v3 = v3;
}

void printMatrix(mat4* m) {
    printf("[\t%f\t%f\t%f\t%f\t]\n", m->v0.x, m->v1.x, m->v2.x, m->v3.x);
    printf("[\t%f\t%f\t%f\t%f\t]\n", m->v0.y, m->v1.y, m->v2.y, m->v3.y);
    printf("[\t%f\t%f\t%f\t%f\t]\n", m->v0.z, m->v1.z, m->v2.z, m->v3.z);
    printf("[\t%f\t%f\t%f\t%f\t]\n\n", m->v0.w, m->v1.w, m->v2.w, m->v3.w);
}

void scalarMultMatrix(mat4* m, GLfloat scalar, mat4* res) {
    scalarMultVector((m->v0), scalar, &(res->v0));
    scalarMultVector((m->v1), scalar, &(res->v1));
    scalarMultVector((m->v2), scalar, &(res->v2));
    scalarMultVector((m->v3), 1, &(res->v3));
}

void addMatrix(mat4* m1, mat4* m2, mat4* res) {
    addVectors((m1->v0), (m2->v0), &(res->v0));
    addVectors((m1->v1), (m2->v1), &(res->v1));
    addVectors((m1->v2), (m2->v2), &(res->v2));
    addVectors((m1->v3), (m2->v3), &(res->v3));
}

void subMatrix(mat4* m1, mat4* m2, mat4* res) {
    subVectors(&(m1->v0), &(m2->v0), &(res->v0));
    subVectors(&(m1->v1), &(m2->v1), &(res->v1));
    subVectors(&(m1->v2), &(m2->v2), &(res->v2));
    subVectors(&(m1->v3), &(m2->v3), &(res->v3));
}

void transposeMatrix(mat4* m, mat4* trans) {
    // Sets the matrices to be the same values
    *trans = *m;
    
    // swaps everythign except for the middle diagonal going across
    trans->v0.y = m->v1.x;
    trans->v0.z = m->v2.x;
    trans->v0.w = m->v3.x;
    trans->v1.x = m->v0.y;
    trans->v1.z = m->v2.y;
    trans->v1.w = m->v3.y;
    trans->v2.x = m->v0.z;
    trans->v2.y = m->v1.z;
    trans->v2.w = m->v3.z;
    trans->v3.x = m->v0.w;
    trans->v3.y = m->v1.w;
    trans->v3.z = m->v2.w;
}

void cofactorMatrix(mat4* m, mat4* cofactor) {
    // Sets equal
    *cofactor = *m;
    
    // Changes in checkerboard pattern
    cofactor->v0.y = m->v0.y * -1;
    cofactor->v0.w = m->v0.w * -1;
    cofactor->v1.x = m->v1.x * -1;
    cofactor->v1.z = m->v1.z * -1;
    cofactor->v2.y = m->v2.y * -1;
    cofactor->v2.w = m->v2.w * -1;
    cofactor->v3.x = m->v3.x * -1;
    cofactor->v3.z = m->v3.z * -1;
}

void minorMatrix(mat4* orig, mat4* minor) {
    vec4 v0, v1, v2, v3;
    v0 = orig->v0;
    v1 = orig->v1;
    v2 = orig->v2;
    v3 = orig->v3;
    
    // Some of them are multipled by -1 because I did them backwards and am too lazy to reverse them.
    //first row minors
    minor->v0.x = (v1.y*v2.z*v3.w + v2.y*v3.z*v1.w + v3.y*v1.z*v2.w - v1.w*v2.z*v3.y - v2.w*v3.z*v1.y - v3.w*v1.z*v2.y);
    minor->v1.x = (v0.y*v2.z*v3.w + v2.y*v3.z*v0.w + v3.y*v0.z*v2.w - v0.w*v2.z*v3.y - v2.w*v3.z*v0.y - v3.w*v0.z*v2.y);
    minor->v2.x = -1*(v1.y*v0.z*v3.w + v0.y*v3.z*v1.w + v3.y*v1.z*v0.w - v1.w*v0.z*v3.y - v0.w*v3.z*v1.y - v3.w*v1.z*v0.y);
    minor->v3.x = (v1.y*v2.z*v0.w + v2.y*v0.z*v1.w + v0.y*v1.z*v2.w - v1.w*v2.z*v0.y - v2.w*v0.z*v1.y - v0.w*v1.z*v2.y);
    
    //second row minors
    minor->v0.y = (v1.x*v2.z*v3.w + v2.x*v3.z*v1.w + v3.x*v1.z*v2.w - v1.w*v2.z*v3.x - v2.w*v3.z*v1.x - v3.w*v1.z*v2.x);
    minor->v1.y = (v0.x*v2.z*v3.w + v2.x*v3.z*v0.w + v3.x*v0.z*v2.w - v0.w*v2.z*v3.x - v2.w*v3.z*v0.x - v3.w*v0.z*v2.x);
    minor->v2.y = -1*(v1.x*v0.z*v3.w + v0.x*v3.z*v1.w + v3.x*v1.z*v0.w - v1.w*v0.z*v3.x - v0.w*v3.z*v1.x - v3.w*v1.z*v0.x);
    minor->v3.y = (v1.x*v2.z*v0.w + v2.x*v0.z*v1.w + v0.x*v1.z*v2.w - v1.w*v2.z*v0.x - v2.w*v0.z*v1.x - v0.w*v1.z*v2.x);
    
    //third row minors
    minor->v0.z = (v1.x*v2.y*v3.w + v2.x*v3.y*v1.w + v3.x*v1.y*v2.w - v1.w*v2.y*v3.x - v2.w*v3.y*v1.x - v3.w*v1.y*v2.x);
    minor->v1.z = (v0.x*v2.y*v3.w + v2.x*v3.y*v0.w + v3.x*v0.y*v2.w - v0.w*v2.y*v3.x - v2.w*v3.y*v0.x - v3.w*v0.y*v2.x);
    minor->v2.z = -1*(v1.x*v0.y*v3.w + v0.x*v3.y*v1.w + v3.x*v1.y*v0.w - v1.w*v0.y*v3.x - v0.w*v3.y*v1.x - v3.w*v1.y*v0.x);
    minor->v3.z = (v1.x*v2.y*v0.w + v2.x*v0.y*v1.w + v0.x*v1.y*v2.w - v1.w*v2.y*v0.x - v2.w*v0.y*v1.x - v0.w*v1.y*v2.x);
    
    //fourth row minors
    minor->v0.w = (v1.x*v2.y*v3.z + v2.x*v3.y*v1.z + v3.x*v1.y*v2.z - v1.z*v2.y*v3.x - v2.z*v3.y*v1.x - v3.z*v1.y*v2.x);
    minor->v1.w = (v0.x*v2.y*v3.z + v2.x*v3.y*v0.z + v3.x*v0.y*v2.z - v0.z*v2.y*v3.x - v2.z*v3.y*v0.x - v3.z*v0.y*v2.x);
    minor->v2.w = (v0.x*v1.y*v3.z + v1.x*v3.y*v0.z + v3.x*v0.y*v1.z - v0.z*v1.y*v3.x - v1.z*v3.y*v0.x - v3.z*v0.y*v1.x);
    minor->v3.w = (v0.x*v1.y*v2.z + v1.x*v2.y*v0.z + v2.x*v0.y*v1.z - v0.z*v1.y*v2.x - v1.z*v2.y*v0.x - v2.z*v0.y*v1.x);
}

GLfloat determinantMatrix(mat4* m, mat4* minor) {
    GLfloat determinant;
    
    determinant = m->v0.x*minor->v0.x - m->v1.x*minor->v1.x + m->v2.x*minor->v2.x - m->v3.x*minor->v3.x;
    
    return determinant;
}

GLfloat absoluteValue(GLfloat a) {
    if (a < 0) {
        a*=-1;
    }
    return a;
}

void inverseMatrix(mat4* m, mat4* res) {
    float det;
    mat4 cof, trans, minor;
    //    printf("Matrix to be inversed: \n");
    //    printMatrix(m);
    minorMatrix(m, &minor);
    //    printf("minor matrix: \n");
    //    printMatrix(&minor);
    det = absoluteValue(determinantMatrix(m, &minor));
    //    printf("cofactor matrix: \n");
    cofactorMatrix(&minor, &cof);
    //    printMatrix(&cof);
    transposeMatrix(&cof, &trans);
    //    printf("transpose matrix: \n");
    //    printMatrix(&trans);
    //    printf("det = %f\n\n\n", det);
    det = 1/det;
    scalarMultMatrix(&trans, det, res);
    //    printf("Inverse result: \n");
    //    printMatrix(res);
}

void multiplyMatrices(mat4 *m1, mat4 *m2, mat4 *res) {
    
    // First Row of new Matrix
    res->v0.x = m1->v0.x*m2->v0.x + m1->v1.x*m2->v0.y + m1->v2.x*m2->v0.z + m1->v3.x*m2->v0.w;
    res->v1.x = m1->v0.x*m2->v1.x + m1->v1.x*m2->v1.y + m1->v2.x*m2->v1.z + m1->v3.x*m2->v1.w;
    res->v2.x = m1->v0.x*m2->v2.x + m1->v1.x*m2->v2.y + m1->v2.x*m2->v2.z + m1->v3.x*m2->v2.w;
    res->v3.x = m1->v0.x*m2->v3.x + m1->v1.x*m2->v3.y + m1->v2.x*m2->v3.z + m1->v3.x*m2->v3.w;
    
    // Second row
    res->v0.y = m1->v0.y*m2->v0.x + m1->v1.y*m2->v0.y + m1->v2.y*m2->v0.z + m1->v3.y*m2->v0.w;
    res->v1.y = m1->v0.y*m2->v1.x + m1->v1.y*m2->v1.y + m1->v2.y*m2->v1.z + m1->v3.y*m2->v1.w;
    res->v2.y = m1->v0.y*m2->v2.x + m1->v1.y*m2->v2.y + m1->v2.y*m2->v2.z + m1->v3.y*m2->v2.w;
    res->v3.y = m1->v0.y*m2->v3.x + m1->v1.y*m2->v3.y + m1->v2.y*m2->v3.z + m1->v3.y*m2->v3.w;
    
    // Third Row
    res->v0.z = m1->v0.z*m2->v0.x + m1->v1.z*m2->v0.y + m1->v2.z*m2->v0.z + m1->v3.z*m2->v0.w;
    res->v1.z = m1->v0.z*m2->v1.x + m1->v1.z*m2->v1.y + m1->v2.z*m2->v1.z + m1->v3.z*m2->v1.w;
    res->v2.z = m1->v0.z*m2->v2.x + m1->v1.z*m2->v2.y + m1->v2.z*m2->v2.z + m1->v3.z*m2->v2.w;
    res->v3.z = m1->v0.z*m2->v3.x + m1->v1.z*m2->v3.y + m1->v2.z*m2->v3.z + m1->v3.z*m2->v3.w;
    
    // Fourth Row
    res->v0.w = m1->v0.w*m2->v0.x + m1->v1.w*m2->v0.y + m1->v2.w*m2->v0.z + m1->v3.w*m2->v0.w;
    res->v1.w = m1->v0.w*m2->v1.x + m1->v1.w*m2->v1.y + m1->v2.w*m2->v1.z + m1->v3.w*m2->v1.w;
    res->v2.w = m1->v0.w*m2->v2.x + m1->v1.w*m2->v2.y + m1->v2.w*m2->v2.z + m1->v3.w*m2->v2.w;
    res->v3.w = m1->v0.w*m2->v3.x + m1->v1.w*m2->v3.y + m1->v2.w*m2->v3.z + m1->v3.w*m2->v3.w;
    
}


void multMatrixVector(mat4 *m, vec4 *v, vec4 *res){
    res->x = m->v0.x*v->x + m->v1.x*v->y + m->v2.x*v->z + m->v3.x*v->w;
    res->y = m->v0.y*v->x + m->v1.y*v->y + m->v2.y*v->z + m->v3.y*v->w;
    res->z = m->v0.z*v->x + m->v1.z*v->y + m->v2.z*v->z + m->v3.z*v->w;
    res->w = m->v0.w*v->x + m->v1.w*v->y + m->v2.w*v->z + m->v3.w*v->w;
}

void getIdentityMatrix(mat4 *res) {
    vec4 v0, v1, v2, v3;
    
    defineVector(1, 0, 0, 0, &v0);
    defineVector(0, 1, 0, 0, &v1);
    defineVector(0, 0, 1, 0, &v2);
    defineVector(0, 0, 0, 1, &v3);
    
    defineMatrix(v0, v1, v2, v3, res);
}


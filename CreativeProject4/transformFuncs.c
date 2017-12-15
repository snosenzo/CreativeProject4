//
//  transformFuncs.c
//  MazeProject2
//
//  Created by Sam Nosenzo on 10/16/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "transformFuncs.h"

  


float calcZ(float x, float y ) {
    float hypXY, zTheta, z;
    hypXY = sqrtf(x*x + y*y);
    zTheta = acosf(hypXY);
    z = sinf(zTheta);
    return z;
}

void getZRotationMatrix(float theta, mat4 *rotateMatrix) {
    vec4 v0, v1, v2, v3;
    defineVector(cos(theta), sin(theta), 0, 0, &v0);
    defineVector(-1*sin(theta), cos(theta), 0, 0, &v1);
    defineVector(0, 0, 1, 0, &v2);
    defineVector(0, 0, 0, 1, &v3);
    defineMatrix(v0, v1, v2, v3, rotateMatrix);
}

void getYRotationMatrixArb(float ax, float d, mat4 *rotateMatrix) {
    vec4 v0, v1, v2, v3;
    defineVector(d, 0, ax, 0, &v0);
    defineVector(0, 1, 0, 0, &v1);
    defineVector(-1*ax, 0, d, 0,  &v2);
    defineVector(0, 0, 0, 1, &v3);
    defineMatrix(v0, v1, v2, v3, rotateMatrix);
}

void getXRotationMatrixArb(float ay, float az, float d, mat4 *rotateMatrix) {
    vec4 v0, v1, v2, v3;
    defineVector(1, 0, 0, 0, &v0);
    defineVector(0, az/d, ay/d, 0,  &v1);
    defineVector(0, -1*ay/d, az/d, 0, &v2);
    defineVector(0, 0, 0, 1, &v3);
    defineMatrix(v0, v1, v2, v3, rotateMatrix);
}

int calculateRotationMatrix(vec4 *init, vec4 *currentVec, mat4 *rotationMatrix, mat4 *lastRotationMatrix) {
    vec4 v0, v1, v2, v3;
    mat4 identity;
    defineVector(1, 0, 0, 0, &v0);
    defineVector(0, 1, 0, 0, &v1);
    defineVector(0, 0, 1, 0, &v2);
    defineVector(0, 0, 0, 1, &v3);
    
    defineMatrix(v0, v1, v2, v3, &identity);
    printf("currentVec mag: %f\n", magnitude(currentVec));
    vec4 axisOfRotation;
    mat4 temp, xRotateNeg, yRotateNeg, xRotate, yRotate, zRotate;
    float theta, xAngle, yAngle;
    
    crossProduct( init, currentVec,  &axisOfRotation);
    
    if(axisOfRotation.x != axisOfRotation.x || magnitude(&axisOfRotation) < .00001) {
        // Is NaN
        printf("can't spin\n\n");
        // can spin = 0 (can't spin)
        return 0;
    }
    float axisMag = magnitude(&axisOfRotation);
    theta = dotProduct(currentVec, init);
    theta = acosf(theta/(magnitude(currentVec)*magnitude(init)));
    
    axisOfRotation.x /= axisMag;
    axisOfRotation.y /= axisMag;
    axisOfRotation.z /= axisMag;
    
    printf("crossProduct: %f \n", magnitude(&axisOfRotation));
    
    //    printVector(&axisOfRotation);
    float d = sqrtf(axisOfRotation.y*axisOfRotation.y + axisOfRotation.z*axisOfRotation.z);
    
    xAngle = asinf(axisOfRotation.y/d);
    getXRotationMatrixArb(axisOfRotation.y, axisOfRotation.z, d, &xRotate);
    transposeMatrix(&xRotate, &xRotateNeg);
    yAngle = acosf(d/1.0);
    getYRotationMatrixArb(axisOfRotation.x, d, &yRotate);
    transposeMatrix(&yRotate, &yRotateNeg);
    
    getZRotationMatrix(-1*theta, &zRotate);
    
    multiplyMatrices( &xRotateNeg,&yRotateNeg, &temp);
    multiplyMatrices(&temp,&zRotate, &xRotateNeg);
    
    multiplyMatrices(&xRotateNeg, &yRotate,  &temp);
    multiplyMatrices(&temp, &xRotate,  rotationMatrix);
    multiplyMatrices(rotationMatrix, &identity, lastRotationMatrix);
    return 1;
}

void getYRotationMatrixTheta(float theta, mat4 *rotateMatrix) {
    vec4 v0, v1, v2, v3;
    defineVector(cos(theta), 0, sin(theta), 0, &v0);
    defineVector(0, 1, 0, 0, &v1);
    defineVector(-1*sin(theta), 0, cos(theta), 0,  &v2);
    defineVector(0, 0, 0, 1, &v3);
    defineMatrix(v0, v1, v2, v3, rotateMatrix);
}

void getXRotationMatrixTheta(float theta, mat4 *rotateMatrix) {
    vec4 v0, v1, v2, v3;
    defineVector(1, 0, 0, 0, &v0);
    defineVector(0, cos(theta), sin(theta), 0,  &v1);
    defineVector(0, -1*sin(theta), cos(theta), 0, &v2);
    defineVector(0, 0, 0, 1, &v3);
    defineMatrix(v0, v1, v2, v3, rotateMatrix);
}

void getTranslationMatrix(float x, float y, float z, mat4 *res) {
    vec4 v0, v1, v2, v3;
    defineVector(1, 0, 0, 0, &v0);
    defineVector(0, 1, 0, 0, &v1);
    defineVector(0, 0, 1, 0, &v2);
    defineVector(x, y, z, 1, &v3);
    
    defineMatrix(v0, v1, v2, v3, res);
}


//
//  transformFuncs.h
//  MazeProject2
//
//  Created by Sam Nosenzo on 10/16/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//

#ifndef transformFuncs_h
#define transformFuncs_h
#include <stdio.h>
#include <math.h>
#include "vecMatOps.h"
#endif /* transformFuncs_h */

float calcZ(float x, float y );
void getZRotationMatrix(float theta, mat4 *rotateMatrix);
void getYRotationMatrixTheta(float theta, mat4 *rotateMatrix);
void getXRotationMatrixTheta(float theta, mat4 *rotateMatrix);
void getYRotationMatrixArb(float ax, float d, mat4 *rotateMatrix);
void getXRotationMatrixArb(float ay, float az, float d, mat4 *rotateMatrix);
int calculateRotationMatrix(vec4 *init, vec4 *currentVec, mat4 *rotationMatrix, mat4 *lastRotationMatrix);
void getTranslationMatrix(float x, float y, float z, mat4 *res);


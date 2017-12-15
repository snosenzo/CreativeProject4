//
//  viewFuncs.h
//  MazeProject2
//
//  Created by Sam Nosenzo on 10/31/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//

#ifndef viewFuncs_h
#define viewFuncs_h

#include <stdio.h>
#include "transformFuncs.h"
#endif /* viewFuncs_h */


void look_at(float eyex, float eyey, float eyez, float atx, float aty, float atz, float upx, float upy, float upz, mat4*);
void frustum(float left, float right, float bottom, float top, float near, float far, mat4*);
void look_at_theta(float theta, float y, mat4 *res);
void perspective(float right, float top, float near, float far, mat4 *res);


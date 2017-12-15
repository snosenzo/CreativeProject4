//
//  objFuncs.h
//  Shadow_Lab4
//
//  Created by Sam Nosenzo on 11/8/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//

#ifndef objFuncs_h
#define objFuncs_h
#include <math.h>
#include <stdio.h>
#include "transformFuncs.h"

int getNumPointsFromSubDiv(int numSubDivisions);
void triangle(const vec4 a, const vec4 b, const vec4 c, obj *o);
vec4 unit(const vec4 v);
void divide_triangle(const vec4 a, const vec4 b, const vec4 c, int count, obj *o);
void tetrahedron(int count, obj *o);

#endif /* objFuncs_h */





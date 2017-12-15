//
//  main.c
//  CreativeProject4
//
//  Created by Sam Nosenzo on 12/5/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "initShader.h"
#include "viewFuncs.h"
#include "shapeVecs.h"
#include "objFuncs.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
mat4 identity;
//vec4 colors[2000000];
vec4 total_vertices[20000000];
vec4 vNormals[20000000];
obj objs[60];
material mats[30];

int numObjs = 0;
int numShadowObjs = 0;
int screenHeight = 1080;
int screenWidth = 1080;
int bufferCounter = 0;
int num_vertices;
GLuint translate_loc;
GLuint scale_loc;
GLuint rotation_loc;
GLuint mvm_loc;
GLuint proj_mat_loc;
float thetaX, thetaY, thetaZ;
mat4 mvm;

mat4 proj_mat;
float p_near = -.1;
float p_far = -100; //-2.2;
float p_left = .3;
float p_right = .1;
float p_top = .1;
float p_bot = .3;

vec4 _eye, _at, _up;
float eyeTheta = 0;
float camHeight = 0;

vec4 gravity = {0.0f, -.003f, 0.0f, 0.0f};


//lighting variables
vec4 light_pos;
GLuint light_pos_loc;
vec4 light_diffuse, light_specular, light_ambient;
vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
GLuint AmbientProduct_loc, DiffuseProduct_loc, SpecularProduct_loc;
float att_constant = 0;
float att_linear = 0;
float att_quadratic = .05;
GLuint att_constant_loc;
GLuint att_linear_loc;
GLuint att_quadratic_loc;
GLuint shininess_loc;
GLuint isShadow_loc;

GLuint isTextureObject_loc;

float view_theta = 0;
float view_phi = 1.2;
float view_radius = 10.0;




vec4 getRandomColorVec() {
    float x, y, z;
    
    vec4 randomColor;
    x = (float)rand()/(float)(RAND_MAX);
    y = (float)rand()/(float)(RAND_MAX);
    z = (float)rand()/(float)(RAND_MAX);
    defineVector(x, y, z, 1.0f, &randomColor);
    return randomColor;
}

void rectObj(float xScale, float yScale, float zScale, float x, float y, float z, obj *in) {
    
    in->vertices = (vec4 *) malloc(sizeof(vec4) * 24);
    in->normals = (vec4 *) malloc(sizeof(vec4) * 24);
    in->num_verts = 24;
    
    for(int j = 0; j < in->num_verts; j++) {
        in->vertices[j].x = cubeVertices[j].x * xScale;
        in->vertices[j].y = cubeVertices[j].y * yScale;
        in->vertices[j].z = cubeVertices[j].z * zScale;
        in->vertices[j].w = cubeVertices[j].w;
        
        in->normals[j].x = 0;
        in->normals[j].y = 1;
        in->normals[j].z = 0;
        in->normals[j].w = 1;
    }
    
    num_vertices += 24;
    getTranslationMatrix(x, y, z, &in->translation);
    in->rotation = identity;
    in->scale = 1;
    in->mesh_type = GL_QUADS;
    numObjs++;
}

vec4 vecProduct(vec4 v1, vec4 v2) {
    vec4 res;
    res.x = v1.x * v2.x;
    res.y = v1.y * v2.y;
    res.z = v1.z * v2.z;
    res.w = 1.0;
    return res;
}

void EyePointOnSphere(float theta, float phi, float radius) {
    _eye.x = radius * cosf(theta);
    _eye.z = -radius * sinf(theta);
    _eye.x *= cosf(phi);
    _eye.z *= cosf(phi);
    _eye.y = radius * sin(phi);
    _eye.w = 1;
}




void initObjs() {
    defineVector(0,.5, 0, 1, &(objs[numObjs].mat.reflect_ambient));
    defineVector(0, .5, 0, 1, &(objs[numObjs].mat.reflect_diffuse));
    defineVector(0, 0, 0, 1, &(objs[numObjs].mat.reflect_specular));
    objs[numObjs].mat.shininess = 2;
    rectObj(20, .01, 20, 0, -.03, 0, &objs[numObjs]);
    
    
    tetrahedron(5, &(objs[numObjs]));
    num_vertices+= objs[numObjs].num_verts;
    defineVector(0, 0, 0, 1, &(objs[numObjs].vel));
    defineVector(0, 5, 0, 1, &(objs[numObjs].loc));
    getTranslationMatrix(objs[numObjs].loc.x, objs[numObjs].loc.y, objs[numObjs].loc.z, &(objs[numObjs].translation));
    objs[numObjs].rotation = identity;
    objs[numObjs].scale = .5;
    defineVector(1, 0, 0, 1, &(objs[numObjs].mat.reflect_ambient));
    defineVector(1, 0, 0, 1, &(objs[numObjs].mat.reflect_diffuse));
    defineVector(1, 1, 1, 1, &(objs[numObjs].mat.reflect_specular));
    objs[numObjs].mat.shininess = 10;
    numShadowObjs++;
    numObjs++;
    
    for(int i = 0; i < 50; i++) {
        vec4 temp1, temp2;
        defineVector(.5, 0, .5, 1, &temp2);
        
        tetrahedron(2, &(objs[numObjs]));
        num_vertices+= objs[numObjs].num_verts;
        vec4 randVec = getRandomColorVec();
        subVectors(&randVec, &temp2, &temp1);
        printVector(&temp1);
        scalarMultVector(temp1, .2, &objs[numObjs].vel);
        defineVector(0, 5, 0, 1, &(objs[numObjs].loc));
        getTranslationMatrix(objs[numObjs].loc.x, objs[numObjs].loc.y, objs[numObjs].loc.z, &(objs[numObjs].translation));
        objs[numObjs].rotation = identity;
        objs[numObjs].scale = .1;
        vec4 color = getRandomColorVec();
        
        objs[numObjs].mat.reflect_ambient = color;
        objs[numObjs].mat.reflect_diffuse = color;
        defineVector(1, 1, 1, 1, &(objs[numObjs].mat.reflect_specular));
        objs[numObjs].mat.shininess = 15;
        numShadowObjs++;
        numObjs++;
    }
    tetrahedron(4, &(objs[numObjs]));
    num_vertices+= objs[numObjs].num_verts;
    getTranslationMatrix(light_pos.x, light_pos.y, light_pos.z, &(objs[numObjs].translation));
    objs[numObjs].rotation = identity;
    objs[numObjs].scale = .1;
    defineVector(1, 1, 1, 1, &(objs[numObjs].mat.reflect_ambient));
    defineVector(1, 1, 1, 1, &(objs[numObjs].mat.reflect_diffuse));
    defineVector(1, 1, 1, 1, &(objs[numObjs].mat.reflect_specular));
    objs[numObjs].mat.shininess = 10;
    numObjs++;
    
    
}


vec4 getShadowVec(const vec4 p, const vec4 light, float yPlane) {
    vec4 res;
    res.x = light.x - (light.y - yPlane) * ((light.x - p.x) / (light.y - p.y));
    res.y = yPlane;
    res.z = light.z - (light.y - yPlane) * ((light.z - p.z) / (light.y - p.y));
    res.w = 1.0;
    return res;
}

vec4 getTransformedVec(const vec4 v, float scale, mat4 rot, mat4 trans) {
    vec4 temp, res;
    res.x = v.x * scale;
    res.y = v.y * scale;
    res.z = v.z * scale;
    res.w = v.w;
    
    multMatrixVector(&rot, &res, &temp);
    multMatrixVector(&trans, &temp, &res);
    return res;
}

void loadObjectOnBuffer(obj *o, int offset){
    //    glBufferSubData(GL_ARRAY_BUFFER, offset, o.num_verts*sizeof(vec4), o.vertices);
    for(int i = 0; i < o->num_verts; i++ ) {
        vNormals[i+offset] = o->normals[i];
        total_vertices[i+offset] = o->vertices[i];
        //        total_vertices[i+offset] = getTransformedVec(o->vertices[i], o->scale, o->rotation, o->translation);
    }
    bufferCounter+=sizeof(vec4) * o->num_verts;
    o->buffer_start_loc = offset;
}

void init(void)
{
    
    defineVector(1, 1, 1, 1, &light_diffuse);
    defineVector(1, 1, 1, 1, &light_specular);
    defineVector(0.2, .2, .2, 1, &light_ambient);
    defineVector(0, 10, 0, 1, &light_pos);
    defineVector(0, 5, 4, 1, &_eye);
    defineVector(0, .25, 0, 1, &_at);
    defineVector(0, 1, 0, 1, &_up);
    EyePointOnSphere(view_theta, view_phi, view_radius);
    look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    perspective(p_right, p_top, p_near, p_far, &proj_mat);
    
    initObjs();
    
    // reading in texture
    
    GLubyte my_texels[500][500][3];
    unsigned char uc;
    
    FILE *fp;
    int i, j, k;
    
    fp = fopen("wood_500_500.raw", "r");
    if(fp == NULL)
    {
        printf("Unable to open file\n");
        exit(0);
    }
    
    for(i = 0; i < 500; i++) {
        for(j = 0; j < 500; j++) {
            for(k = 0; k < 3; k++) {
                fread(&uc, 1, 1, fp);
                my_texels[i][j][k] = uc;
            }
        }
    }
    fclose(fp);
    GLfloat tex_coords[24][2];
    for(int i = 0; i < 24; i+=4) {
        tex_coords[i][0] = 0.0;
        tex_coords[i][1] = 0.0;
        tex_coords[i+1][0] = 1.0;
        tex_coords[i+1][1] = 0.0;
        tex_coords[i+2][0] = 1.0;
        tex_coords[i+2][1] = 1.0;
        tex_coords[i+3][0] = 0.0;
        tex_coords[i+3][1] = 1.0;
    }
    
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    // Texture Object and Parameters
    GLuint mytex;
    glGenTextures(1, &mytex);
    glBindTexture(GL_TEXTURE_2D, mytex);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 500, // width
                 500, // height
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 my_texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    
    // Adding objects to the buffer
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*num_vertices*4, NULL, GL_STATIC_DRAW);
    
    int offset = 0;
    for(int i = 0; i < numObjs; i++ ){
        loadObjectOnBuffer(&(objs[i]), offset);
        offset+=objs[i].num_verts;
    }
    
    int bufferSpace = sizeof(vec4) * num_vertices;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices , total_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, bufferSpace, sizeof(vec4) * num_vertices, vNormals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices * 2, sizeof(tex_coords), tex_coords);

    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * (num_vertices)));
    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices * 2) );
    
    translate_loc = glGetUniformLocation(program, "translate");
    scale_loc = glGetUniformLocation(program, "scale");
    rotation_loc = glGetUniformLocation(program, "rotation");
    mvm_loc = glGetUniformLocation(program, "mvm");
    proj_mat_loc = glGetUniformLocation(program, "proj_mat");
    
    AmbientProduct_loc = glGetUniformLocation(program, "AmbientProduct");
    DiffuseProduct_loc = glGetUniformLocation(program, "DiffuseProduct");
    SpecularProduct_loc = glGetUniformLocation(program, "SpecularProduct");
    light_pos_loc = glGetUniformLocation(program, "light_pos");
    att_constant_loc = glGetUniformLocation(program, "att_constant");
    att_linear_loc = glGetUniformLocation(program, "att_linear");
    att_quadratic_loc = glGetUniformLocation(program, "att_quadratic");
    shininess_loc = glGetUniformLocation(program, "shininess");
    isShadow_loc = glGetUniformLocation(program, "isShadow");
    isTextureObject_loc = glGetUniformLocation(program, "isTextureObject");
    
    glUniform1i(glGetUniformLocation(program, "texture"), 0);

    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void) {
    mat4 temp0, temp1;
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    
    glUniformMatrix4fv(mvm_loc, 1, GL_FALSE, &mvm);
    glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, &proj_mat);
    scalarMultMatrix(&identity, .1, &temp0);
    //    glUniformMatrix4fv(scale_loc, 1, GL_FALSE, &identity);
    //    glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, &identity);
    //    glUniformMatrix4fv(translate_loc, 1, GL_FALSE, &identity);
    glUniform1i(isShadow_loc, 0);
    for(int i = 0; i < numObjs; i++) {
        if(i == 0) {
            glUniform1i(isTextureObject_loc, 1);
        }
        glUniform1fv(scale_loc, 1, &objs[i].scale);
        glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, &objs[i].rotation);
        glUniformMatrix4fv(translate_loc, 1, GL_FALSE, &objs[i].translation);
        AmbientProduct = vecProduct(objs[i].mat.reflect_ambient, light_ambient);
        DiffuseProduct = vecProduct(objs[i].mat.reflect_diffuse, light_diffuse);
        SpecularProduct = vecProduct(objs[i].mat.reflect_specular, light_specular);
        
        glUniform4fv(light_pos_loc, 1, (GLfloat *) &light_pos);
        glUniform4fv(AmbientProduct_loc, 1, (GLfloat *) &AmbientProduct);
        glUniform4fv(DiffuseProduct_loc, 1, (GLfloat *) &DiffuseProduct);
        glUniform4fv(SpecularProduct_loc, 1, (GLfloat *) &SpecularProduct);
        glUniform1f(shininess_loc, objs[i].mat.shininess);
        
        glUniform1f(att_constant_loc, att_constant);
        glUniform1f(att_linear_loc, att_linear);
        glUniform1f(att_quadratic_loc, att_quadratic);
        glDrawArrays(objs[i].mesh_type, objs[i].buffer_start_loc, objs[i].num_verts);
        glUniform1i(isTextureObject_loc, 0);
    }
    
    glUniform1i(isShadow_loc, 1);
    for(int i = 1; i < numShadowObjs+1; i++) {
        glUniform1fv(scale_loc, 1, &objs[i].scale);
        glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, &objs[i].rotation);
        glUniformMatrix4fv(translate_loc, 1, GL_FALSE, &objs[i].translation);
        glDrawArrays(objs[i].mesh_type, objs[i].buffer_start_loc, objs[i].num_verts);
    }
    glutSwapBuffers();
    
}

void resetPlaces() {
    defineVector(0, 0, 0, 1, &(objs[1].vel));
    defineVector(0, 5, 0, 1, &(objs[1].loc));
    getTranslationMatrix(objs[1].loc.x, objs[1].loc.y, objs[1].loc.z, &(objs[1].translation));
    
    for(int i = 2; i < 52; i++) {
        vec4 temp1, temp2;
        defineVector(.5, 0, .5, 1, &temp2);
        tetrahedron(2, &(objs[i]));
        num_vertices+= objs[i].num_verts;
        vec4 randVec = getRandomColorVec();
        subVectors(&randVec, &temp2, &temp1);
        printVector(&temp1);
        scalarMultVector(temp1, .2, &objs[i].vel);
        defineVector(0, 5, 0, 1, &(objs[i].loc));
        getTranslationMatrix(objs[i].loc.x, objs[i].loc.y, objs[i].loc.z, &(objs[i].translation));
    }
}


void keyboard(unsigned char key, int mousex, int mousey) {
    // light keys
    if(key =='k') {
        light_pos.x+=.05;
    }
    if(key =='h') {
        light_pos.x-=.05;
    }
    if(key =='o') {
        light_pos.y+=.05;
    }
    if(key =='l') {
        light_pos.y-=.05;
    }
    if(key =='u') {
        light_pos.z-=.05;
    }
    if(key =='j') {
        light_pos.z+=.05;
    }
    getTranslationMatrix(light_pos.x, light_pos.y, light_pos.z, &objs[numObjs-1].translation);
    // MVM keys
    if(key =='d') {
        view_theta+=.05;
    }
    if(key =='a') {
        view_theta-=.05;
    }
    if(key =='r') {
        view_radius+=.05;
    }
    if(key =='f') {
        view_radius-=.05;
    }
    if(key =='w') {
        view_phi+=.05;
    }
    if(key =='s') {
        view_phi-=.05;
    }
    
    if(key == 'c') {
        resetPlaces();
    }
    EyePointOnSphere(view_theta, view_phi, view_radius);
    look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    
//    printf("Eye: \n");
//    printVector(&_eye);
//    printf("light_pos: \n");
//    printVector(&light_pos);
    
    
    
    if(key == 'q')
        exit(0);
    glutPostRedisplay();
}

void mouseFunction(int button, int state, int x, int y) {
    
}

void dragFunction(int x, int y) {
    
}

vec4 getPointCircleOnYPlane(float radius, float theta) {
    vec4 res;
    res.x = radius * cosf(theta);
    res.z = radius * -sinf(theta);
    res.y = .5;
    res.w = 1;
    return res;
}

void idle() {
    for(int i = 1; i < numShadowObjs+1; i++) {
        vec4 temp, newLoc;
        
        addVectors(objs[i].vel, gravity, &temp);
        objs[i].vel = temp;
        if(objs[i].loc.y < objs[i].scale) {
//            printVector(&objs[i].vel);
            defineVector(objs[i].loc.x, objs[i].scale, objs[i].loc.z, 1, &objs[i].loc);
            if(magnitude(&objs[i].vel) < magnitude(&gravity)*1.1) {
                defineVector(0, 0, 0, 1, &objs[i].vel);
            } else {
                
                objs[i].vel.y *= -0.9f;
            }
        }
        if(objs[i].loc.x > 10 || objs[i].loc.x < -10) {
            objs[i].vel.x *= -1.0f;
        }
        if(objs[i].loc.z > 10 || objs[i].loc.z < -10) {
            objs[i].vel.z *= -1.0f;
        }
        addVectors(objs[i].loc, objs[i].vel, &newLoc);
        getTranslationMatrix(newLoc.x, newLoc.y, newLoc.z, &objs[i].translation);
        objs[i].loc = newLoc;
    }
    
    glutPostRedisplay();
}

int main(int argc, const char * argv[]) {
    
    
    getIdentityMatrix(&identity);
    multiplyMatrices(&identity, &identity, &mvm);
    multiplyMatrices(&identity, &identity, &proj_mat);
    srand(time(NULL));
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lighting Project");
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseFunction);
    glutMotionFunc(dragFunction);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}




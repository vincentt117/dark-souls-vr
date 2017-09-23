//
//  drawGL.cpp
//  ARToolKit6 Tracking Example
//
//  Copyright 2015-2016 Daqri LLC. All Rights Reserved.
//
//  Author(s): Philip Lamb
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <AR6/ARController.h>
#if HAVE_GL

#include "draw.h"
#include <AR6/ARG/arg.h>
#if TARGET_PLATFORM_MACOS
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

static bool rotate90 = false;
static bool flipH = false;
static bool flipV = false;

void drawSetupGL(bool rotate90_in, bool flipH_in, bool flipV_in)
{
    rotate90 = rotate90_in;
    flipH = flipH_in;
    flipV = flipV_in;
}

void drawCleanupGL()
{
}

// Something to look at, draw a rotating colour cube.
static void drawCubeGL()
{
    // Colour cube data.
    const GLfloat cube_vertices [8][3] = {
        /* +z */ {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        /* -z */ {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f} };
    const GLubyte cube_vertex_colors [8][4] = {
        {255, 255, 255, 255}, {255, 255, 0, 255}, {0, 255, 0, 255}, {0, 255, 255, 255},
        {255, 0, 255, 255}, {255, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 255, 255} };
    const GLushort cube_faces [6][4] = { /* ccw-winding */
        /* +z */ {3, 2, 1, 0}, /* -y */ {2, 3, 7, 6}, /* +y */ {0, 1, 5, 4},
        /* -x */ {3, 0, 4, 7}, /* +x */ {1, 2, 6, 5}, /* -z */ {4, 5, 6, 7} };
    int i;
    
    glPushMatrix(); // Save world coordinate system.
    glScalef(20.0f, 20.0f, 20.0f);
    glTranslatef(0.0f, 0.0f, 0.5f); // Place base of cube on marker surface.
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, cube_vertex_colors);
    glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    for (i = 0; i < 6; i++) {
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, &(cube_faces[i][0]));
    }
    glDisableClientState(GL_COLOR_ARRAY);
    glColor4ub(0, 0, 0, 255);
    for (i = 0; i < 6; i++) {
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, &(cube_faces[i][0]));
    }
    glPopMatrix();    // Restore world coordinate system.
}


void drawGL(int32_t viewport[4], float projection[16], float view[16])
{
    if (view) {

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projection);
        if (rotate90) glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);
        
        glMultMatrixf(view);
        
        drawCubeGL();
    }
}

#endif // HAVE_GL

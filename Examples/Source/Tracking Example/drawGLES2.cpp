//
//  drawGLES2.cpp
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

#if HAVE_GLES2

#include "draw.h"
#include <AR6/ARG/arg.h>
#if TARGET_PLATFORM_IOS
#  include <OpenGLES/ES2/gl.h>
#else
#  include <GLES2/gl.h>
#endif
#include <AR6/ARG/arg_mtx.h>
#include <AR6/ARG/arg_shader_gl.h>

// Indices of GL ES program uniforms.
enum {
    UNIFORM_MODELVIEW_PROJECTION_MATRIX,
    UNIFORM_COUNT
};
// Indices of of GL ES program attributes.
enum {
    ATTRIBUTE_VERTEX,
    ATTRIBUTE_COLOUR,
    ATTRIBUTE_COUNT
};
static GLint uniforms[UNIFORM_COUNT] = {0};
static GLuint program = 0;

static bool rotate90 = false;
static bool flipH = false;
static bool flipV = false;

void drawSetupGLES2(bool rotate90_in, bool flipH_in, bool flipV_in)
{
    rotate90 = rotate90_in;
    flipH = flipH_in;
    flipV = flipV_in;
}

void drawCleanupGLES2()
{
    if (program) {
        glDeleteProgram(program);
    }
}

// Something to look at, draw a rotating colour cube.
static void drawCubeGLES2(float viewProjection[16])
{
    // Colour cube data.
    const GLfloat cube_vertices [8][3] = {
        /* +z */ {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        /* -z */ {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f} };
    const GLubyte cube_vertex_colors [8][4] = {
        {255, 255, 255, 255}, {255, 255, 0, 255}, {0, 255, 0, 255}, {0, 255, 255, 255},
        {255, 0, 255, 255}, {255, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 255, 255} };
    const GLubyte cube_vertex_colors_black [8][4] = {
        {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255},
        {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 255} };
    const GLushort cube_faces [6][4] = { /* ccw-winding */
        /* +z */ {3, 2, 1, 0}, /* -y */ {2, 3, 7, 6}, /* +y */ {0, 1, 5, 4},
        /* -x */ {3, 0, 4, 7}, /* +x */ {1, 2, 6, 5}, /* -z */ {4, 5, 6, 7} };
    int i;
    float modelViewProjection[16];
    
    mtxLoadMatrixf(modelViewProjection, viewProjection);
    mtxScalef(modelViewProjection, 20.0f, 20.0f, 20.0f);
    mtxTranslatef(modelViewProjection, 0.0f, 0.0f, 0.5f); // Place base of cube on marker surface.
    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, modelViewProjection);

 	glVertexAttribPointer(ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, cube_vertices);
	glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
	glVertexAttribPointer(ATTRIBUTE_COLOUR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, cube_vertex_colors);
    glEnableVertexAttribArray(ATTRIBUTE_COLOUR);

#ifdef DEBUG
	if (!arglGLValidateProgram(program)) {
		ARLOGe("drawCube(): Error: shader program %d validation failed.\n", program);
		return;
	}
#endif
    for (i = 0; i < 6; i++) {
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, &(cube_faces[i][0]));
    }
	glVertexAttribPointer(ATTRIBUTE_COLOUR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, cube_vertex_colors_black);
    glEnableVertexAttribArray(ATTRIBUTE_COLOUR);
    for (i = 0; i < 6; i++) {
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, &(cube_faces[i][0]));
    }
}

void drawGLES2(int32_t viewport[4], float projection[16], float view[16])
{
    if (view) {
        float viewProjection[16];

        if (!program) {
            GLuint vertShader = 0, fragShader = 0;
            // A simple shader pair which accepts just a vertex position and colour, no lighting.
            const char vertShaderString[] =
                "attribute vec4 position;\n"
                "attribute vec4 colour;\n"
                "uniform mat4 modelViewProjectionMatrix;\n"
                "varying vec4 colourVarying;\n"
                "void main()\n"
                "{\n"
                    "gl_Position = modelViewProjectionMatrix * position;\n"
                    "colourVarying = colour;\n"
                "}\n";
            const char fragShaderString[] =
                "#ifdef GL_ES\n"
                "precision mediump float;\n"
                "#endif\n"
                "varying vec4 colourVarying;\n"
                "void main()\n"
                "{\n"
                    "gl_FragColor = colourVarying;\n"
                "}\n";

            if (program) arglGLDestroyShaders(0, 0, program);
            program = glCreateProgram();
            if (!program) {
                ARLOGe("draw: Error creating shader program.\n");
                return;
            }
            
            if (!arglGLCompileShaderFromString(&vertShader, GL_VERTEX_SHADER, vertShaderString)) {
                ARLOGe("draw: Error compiling vertex shader.\n");
                arglGLDestroyShaders(vertShader, fragShader, program);
                program = 0;
                return;
            }
            if (!arglGLCompileShaderFromString(&fragShader, GL_FRAGMENT_SHADER, fragShaderString)) {
                ARLOGe("draw: Error compiling fragment shader.\n");
                arglGLDestroyShaders(vertShader, fragShader, program);
                program = 0;
                return;
            }
            glAttachShader(program, vertShader);
            glAttachShader(program, fragShader);
            
            glBindAttribLocation(program, ATTRIBUTE_VERTEX, "position");
            glBindAttribLocation(program, ATTRIBUTE_COLOUR, "colour");
            if (!arglGLLinkProgram(program)) {
                ARLOGe("draw: Error linking shader program.\n");
                arglGLDestroyShaders(vertShader, fragShader, program);
                program = 0;
                return;
            }
            arglGLDestroyShaders(vertShader, fragShader, 0); // After linking, shader objects can be deleted.
            
            // Retrieve linked uniform locations.
            uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "modelViewProjectionMatrix");
        }
        glUseProgram(program);

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        
        mtxLoadMatrixf(viewProjection, projection);
        if (rotate90) mtxRotatef(viewProjection, 90.0f, 0.0f, 0.0f, -1.0f);
        
        glEnable(GL_DEPTH_TEST);
        
        mtxMultMatrixf(viewProjection, view);
        
        drawCubeGLES2(viewProjection);
    }
}

#endif // HAVE_GLES2

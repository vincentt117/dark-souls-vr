//
//  draw.h
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

#ifndef __draw_h__
#define __draw_h__

#include <AR6/AR/config.h>

#ifdef __cplusplus
extern "C" {
#endif

#if HAVE_GL
void drawSetupGL(bool rotate90_in, bool flipH_in, bool flipV_in);
void drawGL(int32_t viewport[4], float projection[16], float view[16]);
void drawCleanupGL();
#endif // HAVE_GL
#if HAVE_GL3
void drawSetupGL3(bool rotate90_in, bool flipH_in, bool flipV_in);
void drawGL3(int32_t viewport[4], float projection[16], float view[16]);
void drawCleanupGL3();
#endif // HAVE_GL3
#if HAVE_GLES2
void drawSetupGLES2(bool rotate90_in, bool flipH_in, bool flipV_in);
void drawGLES2(int32_t viewport[4], float projection[16], float view[16]);
void drawCleanupGLES2();
#endif // HAVE_GLES2

#ifdef __cplusplus
}
#endif
#endif // !__draw_h__


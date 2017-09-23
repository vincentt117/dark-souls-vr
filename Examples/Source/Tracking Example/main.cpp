//
//  main.cpp
//  ARToolKit6 Tracking Example
//
//  Copyright 2015-2016 Daqri LLC. All Rights Reserved.
//  Copyright 2008-2015 ARToolworks, Inc. All Rights Reserved.
//
//  Author(s): Daniel Bell, Philip Lamb
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef DEBUG
#  ifdef _WIN32
#    define MAXPATHLEN MAX_PATH
#    include <direct.h>               // _getcwd()
#    define getcwd _getcwd
#  else
#    include <unistd.h>
#    include <sys/param.h>
#  endif
#endif
#include <unistd.h>

#include <AR6/ARController.h>

#include <SDL2/SDL.h>
#if (HAVE_GL || HAVE_GL3)
#  include <SDL2/SDL_opengl.h>
#elif HAVE_GLES2
#  include <SDL2/SDL_opengles2.h>
#endif

#include "draw.h"

const char *vconf = NULL;

// Window and GL context.
static SDL_GLContext gSDLContext = NULL;
static int contextWidth = 0;
static int contextHeight = 0;
static bool contextWasUpdated = false;
static int32_t viewport[4];
static SDL_Window* gSDLWindow = NULL;

static ARController* arController = NULL;
static ARG_API drawAPI = ARG_API_None;

static long gFrameNo = 0;

struct trackable {
    const char *name;
    float height;
};
static const struct trackable trackables[] = {
    {"Alterra_Ticket_1.jpg", 95.3},
    {"Alterra_Postcard_2.jpg", 95.3},
    {"Alterra_Postcard_3.jpg", 127.0},
    {"Alterra_Postcard_4.jpg", 95.3}
};
static const int trackableCount = (sizeof(trackables)/sizeof(trackables[0]));

//
//
//

static void quit(int rc)
{
#if HAVE_GL
    if (drawAPI == ARG_API_GL) drawCleanupGL();
#endif
#if HAVE_GL3
    if (drawAPI == ARG_API_GL3) drawCleanupGL3();
#endif
#if HAVE_GLES2
    if (drawAPI == ARG_API_GLES2) drawCleanupGLES2();
#endif
    if (arController) {
        arController->displayFrameFinal(0);
        arController->shutdown();
        delete arController;
    }
    if (gSDLContext) {
        SDL_GL_MakeCurrent(0, NULL);
        SDL_GL_DeleteContext(gSDLContext);
    }
    if (gSDLWindow) {
        SDL_DestroyWindow(gSDLWindow);
    }
    SDL_Quit();
    exit(rc);
}

void reshape(int w, int h)
{
    contextWidth = w;
    contextHeight = h;
    ARLOGd("Resized to %dx%d.\n", w, h);
    contextWasUpdated = true;
}

int main(int argc, char *argv[])
{
    
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ARLOGe("Error: SDL initialisation failed. SDL error: '%s'.\n", SDL_GetError());
        quit(1);
        return 1;
    }
    
    // Create a window.
#if 0
    gSDLWindow = SDL_CreateWindow("ARToolKit6 Tracking Example",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              0, 0,
                              SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
                              );
#else
    gSDLWindow = SDL_CreateWindow("ARToolKit6 Tracking Example",
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  1280, 720,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
                                  );
#endif
    if (!gSDLWindow) {
        ARLOGe("Error creating window: %s.\n", SDL_GetError());
        quit(-1);
    }

    // Create an OpenGL context to draw into. If OpenGL 3.2 not available, attempt to fall back to OpenGL 1.5, then OpenGL ES 2.0
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // This is the default.
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    gSDLContext = SDL_GL_CreateContext(gSDLWindow);
    if (gSDLContext) {
        drawAPI = ARG_API_GL3;
    } else {
        ARLOGi("Unable to create OpenGL 3.2 context: %s. Will try OpenGL 1.5.\n", SDL_GetError());
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
        gSDLContext = SDL_GL_CreateContext(gSDLWindow);
        if (gSDLContext) {
            drawAPI = ARG_API_GL;
#if TARGET_PLATFORM_MACOS
            vconf = "-format=BGRA";
#endif
        } else {
            ARLOGi("Unable to create OpenGL 1.5 context: %s. Will try OpenGL ES 2.0\n", SDL_GetError());
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            gSDLContext = SDL_GL_CreateContext(gSDLWindow);
            if (gSDLContext) {
                drawAPI = ARG_API_GLES2;
            } else {
                ARLOGi("Unable to create OpenGL ES 2.0 context: %s.\n", SDL_GetError());
            }
        }
    }
    if (drawAPI == ARG_API_None) {
        ARLOGe("No OpenGL context available. Giving up.\n", SDL_GetError());
        quit(-1);
    }

    int w, h;
    SDL_GL_GetDrawableSize(SDL_GL_GetCurrentWindow(), &w, &h);
    reshape(w, h);
   
    // Initialise the ARController.
    arController = new ARController();
    if (!arController->initialiseBase()) {
        ARLOGe("Error initialising ARController.\n");
        quit(-1);
    }
    
#ifdef DEBUG
    arLogLevel = AR_LOG_LEVEL_DEBUG;
#endif
    
    // Add trackables.
    int trackableIDs[trackableCount];
#ifdef DEBUG
    char buf[MAXPATHLEN];
    ARLOGd("CWD is '%s'.\n", getcwd(buf, sizeof(buf)));
#endif
    char *resourcesDir = arUtilGetResourcesDirectoryPath(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_BEST);
    char *trackableConfig;
    for (int i = 0; i < trackableCount; i++) {
        asprintf(&trackableConfig, "2d;%s/%s;%f", resourcesDir, trackables[i].name, trackables[i].height);
        trackableIDs[i] = arController->addTrackable(trackableConfig);
        if (trackableIDs[i] == -1) {
            ARLOGe("Error adding trackable.\n");
            quit(-1);
        }
        free(trackableConfig);
    }
    arController->get2DTracker().setMaxSimultaneousTrackedImages(trackableCount);

#ifdef DEBUG
    ARLOGd("vconf is '%s'.\n", vconf);
#endif
    // Start tracking.
    arController->startRunning(vconf, NULL, NULL, 0);
    
    free(resourcesDir);
    
    // Main loop.
    bool done = false;
    while (!done) {
        
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT || (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)) {
                done = true;
                break;
            } else if (ev.type == SDL_WINDOWEVENT) {
                //ARLOGd("Window event %d.\n", ev.window.event);
                if (ev.window.event == SDL_WINDOWEVENT_RESIZED && ev.window.windowID == SDL_GetWindowID(gSDLWindow)) {
                    //int32_t w = ev.window.data1;
                    //int32_t h = ev.window.data2;
                    int w, h;
                    SDL_GL_GetDrawableSize(gSDLWindow, &w, &h);
                    reshape(w, h);
                }
            }
         }

        bool gotFrame = arController->capture();
        if (!gotFrame) {
            usleep(1000);
        } else {
            //ARLOGi("Got frame %ld.\n", gFrameNo);
            gFrameNo++;
            
            if (!arController->update()) {
                ARLOGe("Error in ARController::update().\n");
                quit(-1);
            }
  
            if (contextWasUpdated) {
                if (!arController->displayFrameInit(0)) {
                    ARLOGe("Error in ARController::displayFrameInit().\n");
                    quit(-1);
                }
                if (!arController->displayFrameSettings(0, contextWidth, contextHeight, false, false, false, ARView::HorizontalAlignment::H_ALIGN_CENTRE, ARView::VerticalAlignment::V_ALIGN_CENTRE, ARView::ScalingMode::SCALE_MODE_FIT, viewport)) {
                    ARLOGe("Error in ARController::displayFrameSettings().\n");
                    quit(-1);
                }
#if HAVE_GL
                if (drawAPI == ARG_API_GL) drawSetupGL(false, false, false);;
#endif
#if HAVE_GL3
                if (drawAPI == ARG_API_GL3) drawSetupGL3(false, false, false);;
#endif
#if HAVE_GLES2
                if (drawAPI == ARG_API_GLES2) drawSetupGLES2(false, false, false);;
#endif
                contextWasUpdated = false;
            }
            
            SDL_GL_MakeCurrent(gSDLWindow, gSDLContext);
            
            // Clear the context.
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Display the current video frame to the current OpenGL context.
            arController->displayFrame(0);
            
            ARdouble projectionARD[16];
            float projection[16];
            arController->getProjectionMatrix(0, projectionARD);
            for (int i = 0; i < 16; i++) projection[i] = (float)projectionARD[i];

            // Look for trackables, and draw on each found one.
            for (int i = 0; i < trackableCount; i++) {
                
                // Find the trackable for the given trackable ID.
                ARTrackable *trackable = arController->findTrackable(trackableIDs[i]);
                float view[16];
                if (trackable->visible) {
                    //arUtilPrintMtx16(trackable->transformationMatrix);
                    for (int i = 0; i < 16; i++) view[i] = (float)trackable->transformationMatrix[i];
                }
#if HAVE_GL
                if (drawAPI == ARG_API_GL) drawGL(viewport, projection, (trackable->visible ? view : NULL));
#endif
#if HAVE_GL3
                if (drawAPI == ARG_API_GL3) drawGL3(viewport, projection, (trackable->visible ? view : NULL));
#endif
#if HAVE_GLES2
                if (drawAPI == ARG_API_GLES2) drawGLES2(viewport, projection, (trackable->visible ? view : NULL));
#endif
            }
            
            SDL_GL_SwapWindow(gSDLWindow);
        } //end: if (gotFrame)
    } //end: while (!done) {
    
    quit(0);
    return 0;
}

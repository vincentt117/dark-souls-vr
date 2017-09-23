/*
 *  file_utils.h
 *  ARToolKit6
 *
 *  This file is part of ARToolKit.
 *
 *  Copyright 2015-2016 Daqri, LLC.
 *  Copyright 2013-2015 Daqri, LLC.
 *
 *  Author(s): Philip Lamb, Dan Bell
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include <stdbool.h>
#include <termios.h>
#include <stdio.h>
#ifndef __AR6_ARUtil_file_utils_h__
#define __AR6_ARUtil_file_utils_h__

#ifdef __cplusplus
extern "C" {
#endif

// Test for existence of regular file like 'test -f [dir/]file'.
// Returns 1 if file exists, 0 if not, or -1 in case of error and the error code in 'errno'.
int test_f(const char *file, const char *dir);

// Test for existence of directory like 'test -d dir'.
// Returns 1 if dir exists, 0 if not, or -1 in case of error and the error code in 'errno'.
int test_d(const char *dir);

// Copy a single file (with overwriting in case of target file already existing) like 'cp -f source_file target_file'.
// Returns 0 for success, -1 in case of error and the error code in 'errno'.
int cp_f(const char *source_file, const char *target_file);

// Make directory like 'mkdir -p path'.
// Returns 0 for success, -1 in case of error and the error code in 'errno'.
int mkdir_p(const char *path);

// Remove path like 'rm -rf path'.
// Returns 0 for success, -1 in case of error and the error code in 'errno'.
int rm_rf(const char *path);

// Rename path like 'rn path newPath'.
// Returns 0 for success, -1 in case of error and the error code in 'errno'.
int rn_f(const char *source_file, const char *target_file);
    
// Unzip a zip file like 'unzip -o zipPathname -d outPath'.
// Returns 0 in case of success, or error code < 0 in case of error (see unzip.h). If error code is -1, the error code is in 'errno'.
int unzip_od(const char *zipPathname, const char *outPath);

// Zip a zip file like 'zip zipPathname -d fileNames'.
// Returns 0 in case of success, or error code < 0 in case of error (see zip.h). If error code is -1, the error code is in 'errno'.
int zip_od(char *zipPathname, const char *baseFilePath, const char **fileNames, int totalFiles);

float get_file_size(const char * pathName);

// Read a single character from the terminal without echo, like 'read -s -n 1'.
char read_sn1(void);

#ifdef __cplusplus
}
#endif
#endif // !__AR6_ARUtil_file_utils_h__

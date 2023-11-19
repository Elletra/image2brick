#ifndef _IMAGE2BRICK_DEFS_H
#define _IMAGE2BRICK_DEFS_H

#define LINE_BUFFER_SIZE 256
#define MSG_PREFIX "[image2brick] "
#define ERR_PREFIX MSG_PREFIX "Error: "

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#elif defined linux
#include <limits.h>
#endif

#endif

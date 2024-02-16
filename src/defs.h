/**
 * image2brick: Convert images to Blockland macros.
 *
 * Copyright (C) 2024 Elletra
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _IMAGE2BRICK_DEFS_H
#define _IMAGE2BRICK_DEFS_H

#define LINE_BUFFER_SIZE 256
#define MSG_PREFIX "[image2brick] "
#define ERR_PREFIX MSG_PREFIX "Error: "

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#elif defined __linux__
#include <limits.h>
#endif

#endif

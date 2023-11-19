/**
 * image2brick: Convert images to Blockland macros.
 *
 * Copyright (C) 2023  Electrk
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

#ifndef _IMAGE2BRICK_FILE_PATH_H
#define _IMAGE2BRICK_FILE_PATH_H

#include <stdbool.h>

#include "defs.h"

char *file_name(char *path);
bool file_base(char *path, char buffer[PATH_MAX]);
char *file_ext(char *name);

#endif

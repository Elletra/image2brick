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

#ifndef _IMAGE2BRICK_FILE_IO_H
#define _IMAGE2BRICK_FILE_IO_H

#include <stdbool.h>
#include <stdint.h>

#include "args.h"

// ------------------------------------------------

struct Image
{
	int width;
	int height;
	int channels;
	uint8_t *data;
};

// ------------------------------------------------

bool parse_colorset(struct Args *args);
bool write_macro_file(char *image_file_base, struct Image *image, struct Args *args, char *out_file_name);

#endif

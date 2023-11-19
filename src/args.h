/**
 * image2brick: Convert images to Blockland macros.
 *
 * Copyright (C) 2023 Electrk
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

#ifndef _IMAGE2BRICK_ARGS_H
#define _IMAGE2BRICK_ARGS_H

#include <stdbool.h>

// ------------------------------------------------

struct Args
{
	bool is_vertical;
	bool cli_mode;
	bool orientation_set;
	char *image_file_path;
	char *colorset_file_path;
};

// ------------------------------------------------

bool parse_args(int argc, char **argv, struct Args *args);
void display_help();
void pause(struct Args *args);

#endif

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

#ifndef _IMAGE2BRICK_COLOR_H
#define _IMAGE2BRICK_COLOR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_COLORS 64
#define RGBA_CHANNELS 4

// ------------------------------------------------

struct ColorI
{
	int r;
	int g;
	int b;
	int a;
};

struct ColorF
{
	float r;
	float g;
	float b;
	float a;
};

// ------------------------------------------------

bool is_decimal_rgba(float r, float g, float b, float a);

struct ColorI *set_color_f(size_t index, struct ColorF *color);
struct ColorI *set_color_dec(size_t index, struct ColorF *color);

struct ColorI *get_color_i(size_t index);

size_t set_colorset_size(size_t size);
size_t get_colorset_size();

size_t find_nearest_color_rgba(int r, int g, int b, int a);

#endif

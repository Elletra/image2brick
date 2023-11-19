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

#include "color.h"

#include <float.h>
#include <math.h>

// ------------------------------------------------

static struct ColorI g_color_table[MAX_COLORS] = { 0 };
static size_t g_colorset_size = MAX_COLORS;

// ------------------------------------------------

bool is_decimal_rgba(float r, float g, float b, float a)
{
	return (floor(r) != r || floor(g) != g || floor(b) != b || floor(a) != a)
		|| (r <= 1.0 && g <= 1.0 && b <= 1.0 && a <= 1.0);
}

struct ColorI *set_color_f(size_t index, struct ColorF *color)
{
	if (index >= MAX_COLORS || color == NULL)
	{
		return NULL;
	}

	g_color_table[index].r = (int) color->r;
	g_color_table[index].g = (int) color->g;
	g_color_table[index].b = (int) color->b;
	g_color_table[index].a = (int) color->a;

	return &g_color_table[index];
}

struct ColorI *set_color_dec(size_t index, struct ColorF *color)
{
	if (index >= MAX_COLORS || color == NULL)
	{
		return NULL;
	}

	g_color_table[index].r = (int) (color->r * 255.0);
	g_color_table[index].g = (int) (color->g * 255.0);
	g_color_table[index].b = (int) (color->b * 255.0);
	g_color_table[index].a = (int) (color->a * 255.0);

	return &g_color_table[index];
}

struct ColorI *get_color_i(size_t index)
{
	return (index < MAX_COLORS) ? &g_color_table[index] : NULL;
}

// ------------------------------------------------

size_t set_colorset_size(size_t size)
{
	return g_colorset_size = (size <= MAX_COLORS) ? size : MAX_COLORS;
}

size_t get_colorset_size()
{
	return g_colorset_size;
}

// ------------------------------------------------

size_t find_nearest_color_rgba(int r, int g, int b, int a)
{
	size_t nearest = 0;
	double min_dist = DBL_MAX;

	for (size_t i = 0; i < g_colorset_size; i++)
	{
		struct ColorI *color = &g_color_table[i];

		int r_dist = color->r - r;
		int g_dist = color->g - g;
		int b_dist = color->b - b;
		int a_dist = color->a - a;

		double dist = sqrt((r_dist * r_dist) + (g_dist * g_dist) + (b_dist * b_dist) + (a_dist * a_dist));

		if (dist < min_dist)
		{
			nearest = i;
			min_dist = dist;
		}
	}

	return nearest;
}

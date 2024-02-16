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

#include "file_io.h"

#include <stdio.h>
#include <ctype.h>

#include "defs.h"
#include "color.h"

#define WRITE_OR_CLOSE(file, format, ...) if (fprintf(file, format, __VA_ARGS__) < 0)\
{\
	printf(ERR_PREFIX "Write to file failed\n");\
	fclose(file);\
	return false;\
}

// ------------------------------------------------

bool parse_colorset(struct Args *args)
{
	FILE *file = fopen(args->colorset_file_path, "r");

	if (file == NULL)
	{
		return false;
	}

	char line_buffer[LINE_BUFFER_SIZE];
	line_buffer[LINE_BUFFER_SIZE - 1] = '\0';

	struct ColorF parsed;
	size_t i = 0;

	while (fgets(line_buffer, LINE_BUFFER_SIZE, file) && i < MAX_COLORS)
	{
		if (isdigit(line_buffer[0]) && sscanf(line_buffer, "%f %f %f %f", &parsed.r, &parsed.g, &parsed.b, &parsed.a) == RGBA_CHANNELS)
		{
			if (is_decimal_rgba(parsed.r, parsed.g, parsed.b, parsed.a))
			{
				set_color_dec(i, &parsed);
			}
			else
			{
				set_color_f(i, &parsed);
			}

			i++;
		}
	}

	set_colorset_size(i);
	fclose(file);

	return true;
}

bool write_macro_file(char *image_file_base, struct Image *image, struct Args *args, char *out_file_name)
{
	if (image_file_base == NULL || image == NULL || args == NULL || out_file_name == NULL)
	{
		printf(ERR_PREFIX "write_macro_file() - One or more arguments is NULL\n");
		return false;
	}

	FILE *file = fopen(out_file_name, "w");

	if (file == NULL)
	{
		printf(ERR_PREFIX "Could not write '%s'\n", out_file_name);
		return false;
	}

	WRITE_OR_CLOSE(file, "%s_%s (image2brick)\n\n", image_file_base, args->is_vertical ? "vert" : "horiz")

	if (args->is_vertical)
	{
		WRITE_OR_CLOSE(file, "1x1\t1x1 Print\t\t\t\t\t\t\t\t\n", "")
	}
	else
	{
		WRITE_OR_CLOSE(file, "1x1F\t1x1F Print\t\t\t\t\t\t\t\t\n", "")
	}

	size_t prev_color = SIZE_MAX;

	for (int y = image->height - 1; y >= 0; y--)
	{
		for (int x = 0; x < image->width; x++)
		{
			int index = (y * image->width + x) * image->channels;

			uint8_t r = image->data[index];
			uint8_t g = (image->channels > 1) ? image->data[index + 1] : 0;
			uint8_t b = (image->channels > 2) ? image->data[index + 2] : 0;
			uint8_t a = (image->channels > 3) ? image->data[index + 3] : 255;

			size_t color = find_nearest_color_rgba(r, g, b, a);

			if (color != prev_color)
			{
				WRITE_OR_CLOSE(file, "Server\tuseSprayCan\t%zu\n", color)
				prev_color = color;
			}

			if (x > 0)
			{
				WRITE_OR_CLOSE(file, "Server\tsuperShiftBrick\t0\t-1\t0\n", "")
			}

			WRITE_OR_CLOSE(file, "Server\tplantBrick\t\t\t\t\t\t\n", "")
		}

		if (args->is_vertical)
		{
			WRITE_OR_CLOSE(file, "Server\tsuperShiftBrick\t0\t%d\t1\n", image->width - 1)
		}
		else
		{
			WRITE_OR_CLOSE(file, "Server\tsuperShiftBrick\t1\t%d\t0\n", image->width - 1)
		}
	}

	fclose(file);

	return true;
}

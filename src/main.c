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

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include "color.h"
#include "file_path.h"

#define LINE_BUFFER_SIZE 256
#define MSG_PREFIX "[image2brick] "
#define ERR_PREFIX MSG_PREFIX "Error: "

#define WRITE_OR_CLOSE(file, format, ...) if (fprintf(file, format, __VA_ARGS__) < 0)\
{\
	printf(ERR_PREFIX "Write to file failed\n");\
	fclose(file);\
	return false;\
}

struct Image
{
	int width;
	int height;
	int channels;
	stbi_uc *data;
};

struct Args
{
	bool is_vertical;
	bool cli_mode;
	bool orientation_set;
	char *image_file_path;
};

bool parse_args(int argc, char **argv, struct Args *args);
void display_help();
void pause(struct Args *args);
bool parse_colorset();
bool write_macro_file(char *image_file_base, struct Image *image, struct Args *args, char *out_file_name);

int main(int argc, char **argv)
{
	struct Args args;

	if (!parse_args(argc, argv, &args))
	{
		return 1;
	}

	if (!parse_colorset())
	{
		printf(ERR_PREFIX "Could not open 'colorSet.txt'\n");
		pause(&args);
		return 1;
	}

	char image_file_base[PATH_MAX];

	if (!file_base(args.image_file_path, image_file_base))
	{
		printf(ERR_PREFIX "Failed to parse file path base\n");
		pause(&args);
		return 1;
	}

	if (!args.orientation_set && !args.cli_mode)
	{
		int ch;

		do
		{
			printf(
				MSG_PREFIX
				"Please select orientation:\n"
				"  1. Vertical\n"
				"  2. Horizontal\n"
				"\n"
			);

			fflush(stdout);

			ch = getchar();

			if (ch == '1' || ch == '2')
			{
				while ((ch = getchar()) != '\n' && ch != EOF);
				break;
			}

			printf("\n" MSG_PREFIX "Invalid option.\n");

			while ((ch = getchar()) != '\n' && ch != EOF);
		}
		while (true);
	}

	char out_file_name[PATH_MAX];
	snprintf(out_file_name, PATH_MAX, "%s_%s.txt", image_file_base, args.is_vertical ? "vert" : "horiz");

	struct Image image;
	image.data = stbi_load(args.image_file_path, &image.width, &image.height, &image.channels, 0);

	if (image.data == NULL)
	{
		printf(ERR_PREFIX "Failed to open '%s' - %s\n", args.image_file_path, stbi_failure_reason());
		pause(&args);
		return 1;
	}

	if (!write_macro_file(image_file_base, &image, &args, out_file_name))
	{
		stbi_image_free(image.data);
		pause(&args);
		return 1;
	}

	stbi_image_free(image.data);

	printf(MSG_PREFIX "Successfully created macro file '%s'\n", out_file_name);
	pause(&args);

	return 0;
}

bool parse_args(int argc, char **argv, struct Args *args)
{
	bool set_horizontal = false;
	bool set_vertical = false;
	bool set_out_file = false;
	bool set_image_file = false;

	for (int i = 1; i < argc; i++)
	{
		char *arg = argv[i];

		if (!strcmp(arg, "-v"))
		{
			if (set_horizontal)
			{
				printf(ERR_PREFIX "Orientation already set with '-h'\n");
				return false;
			}

			args->is_vertical = true;
			args->orientation_set = true;
			set_vertical = true;
		}
		else if (!strcmp(arg, "-h"))
		{
			if (set_vertical)
			{
				printf(ERR_PREFIX "Orientation already set with '-v'\n");
				return false;
			}

			args->is_vertical = false;
			args->orientation_set = true;
			set_horizontal = true;
		}
		else if (!strcmp(arg, "-X"))
		{
			args->cli_mode = true;
		}
		else if (arg[0] == '-')
		{
			printf(ERR_PREFIX "Unrecognized command-line option '%s'\n", arg);

			if (strlen(arg) == 2)
			{
				if (arg[1] == 'V' || arg[1] == 'H')
				{
					printf(ERR_PREFIX "Did you mean '-%c'?\n", tolower(arg[1]));
				}
				else if (arg[1] == 'x')
				{
					printf(ERR_PREFIX "Did you mean '-%c'?\n", toupper(arg[1]));
				}
			}

			return false;
		}
		else if (set_image_file)
		{
			printf(ERR_PREFIX "Multiple image files specified.\n");
			return false;
		}
		else
		{
			args->image_file_path = arg;
			set_image_file = true;
		}
	}

	if (!set_image_file || args->image_file_path == NULL)
	{
		printf(ERR_PREFIX "No image file specified.\n");
		display_help();
		pause(args);

		return false;
	}

	if (!set_vertical && !set_horizontal && args->cli_mode)
	{
		printf(ERR_PREFIX "No orientation specified.\n");
		display_help();
		pause(args);

		return false;
	}

	return true;
}

void display_help()
{
	printf(
		"\n"
		"usage: image2brick (-v | -h) [-X] image_file\n"
		"  options:\n"
		"    -v    Specifies a vertical orientation.\n"
		"    -h    Specifies a horizontal orientation.\n"
		"    -X    Makes the program operate as a command-line interface\n"
		"          that takes no keyboard input and closes immediately\n"
		"          upon completion.\n"
		"\n"
	);
}

void pause(struct Args *args)
{
	if (!args->cli_mode)
	{
		printf("\nPress enter key to continue...\n\n");
		fflush(stdout);
		getchar();	
	}
}

bool parse_colorset()
{
	FILE *file = fopen("./colorSet.txt", "r");

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

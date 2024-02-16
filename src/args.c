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

#include "args.h"

#include <stdio.h>
#include <ctype.h>

#include "defs.h"

bool parse_args(int argc, char **argv, struct Args *args)
{
	bool set_horizontal = false;
	bool set_vertical = false;
	bool set_out_file = false;
	bool set_image_file = false;

	for (int i = 1; i < argc; i++)
	{
		char *arg = argv[i];

		if (!strcmp(arg, "-h"))
		{
			args->display_help = true;
		}
		else if (!strcmp(arg, "-V"))
		{
			if (set_horizontal)
			{
				printf(ERR_PREFIX "Orientation already set with '-H'\n");
				return false;
			}

			args->is_vertical = true;
			args->orientation_set = true;
			set_vertical = true;
		}
		else if (!strcmp(arg, "-H"))
		{
			if (set_vertical)
			{
				printf(ERR_PREFIX "Orientation already set with '-V'\n");
				return false;
			}

			args->is_vertical = false;
			args->orientation_set = true;
			set_horizontal = true;
		}
		else if (!strcmp(arg, "-c"))
		{
			if (i >= argc - 1)
			{
				printf(ERR_PREFIX "Missing file name after '-c'\n");
				return false;
			}

			args->colorset_file_path = argv[i + 1];
			i++;
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
				if (arg[1] == 'V' || arg[1] == 'H' || arg[1] == 'C')
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

	if (!args->display_help)
	{
		if (!set_image_file || args->image_file_path == NULL)
		{
			printf(ERR_PREFIX "No image file specified.\n\n");
			display_help();
			pause(args);

			return false;
		}

		if (!set_vertical && !set_horizontal && args->cli_mode)
		{
			printf(ERR_PREFIX "No orientation specified.\n\n");
			display_help();
			pause(args);

			return false;
		}
	}

	return true;
}

void display_help()
{
	printf(
		"usage: image2brick image_file [-h] (-V | -H) [-c colorset_file] [-X]\n"
		"  options:\n"
		"    -h    Displays help.\n"
		"    -V    Specifies a vertical orientation.\n"
		"    -H    Specifies a horizontal orientation.\n"
		"    -c    Specifies the colorset file to use.\n"
		"    -X    Makes the program operate as a command-line interface\n"
		"          that takes no keyboard input and closes immediately\n"
		"          upon completion or failure.\n"
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

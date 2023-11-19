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

#include <stdio.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include "defs.h"
#include "args.h"
#include "color.h"
#include "file_io.h"
#include "file_path.h"

int main(int argc, char **argv)
{
	struct Args args =
	{
		.is_vertical = false,
		.cli_mode = false,
		.orientation_set = false,
		.image_file_path = NULL,
		.colorset_file_path = NULL
	};

	if (!parse_args(argc, argv, &args))
	{
		return 1;
	}

	char image_file_base[PATH_MAX];

	if (!file_base(args.image_file_path, image_file_base))
	{
		printf(ERR_PREFIX "Failed to parse file path base\n");
		pause(&args);
		return 1;
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

	if (args.colorset_file_path == NULL || strlen(args.colorset_file_path) <= 0)
	{
		args.colorset_file_path = "./colorSet.txt";
	}

	if (!parse_colorset(&args))
	{
		stbi_image_free(image.data);
		printf(ERR_PREFIX "Could not open colorset file '%s'\n", args.colorset_file_path);
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

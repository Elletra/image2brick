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

#include "file_path.h"

#include <stdio.h>
#include <string.h>

// ------------------------------------------------

/* `name` should be a file name retrieved with `file_name()` first! */
static char *file_name_ext(char *name);

// ------------------------------------------------

char *file_name(char *path)
{
	if (path == NULL)
	{
		return NULL;
	}

	char *name = path;

	for (char *p = path; *p; p++)
	{
		if (*p == '/' || *p == '\\' || *p == ':')
		{
			name = p + sizeof(char);
		}
	}

	return name;
}

bool file_base(char *path, char buffer[PATH_MAX])
{
	if (path == NULL || buffer == NULL)
	{
		return false;
	}

	char *name = file_name(path);
	char *ext = file_name_ext(name);

	if (name == NULL || ext == NULL)
	{
		return false;
	}

	size_t i = 0;

	for (char *p = name; p != ext && i < PATH_MAX - 1; p++, i++)
	{
		buffer[i] = *p;
	}

	buffer[i] = '\0';

	return true;
}

char *file_ext(char *name)
{
	return file_name_ext(file_name(name));
}

// ------------------------------------------------

/* `name` should be a file name retrieved with `file_name()` first! */
static char *file_name_ext(char *name)
{
	if (name == NULL)
	{
		return NULL;
	}

	char *ext = NULL;
	char *p = name;

	while (*p++)
	{
		if (*p == '.' || (*p == '\0' && ext == NULL))
		{
			ext = p;
		}
	}

	return ext;
}

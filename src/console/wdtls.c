// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Part of wadtools
 * 
 * Entry point for the console application
 *
 * Copyright (c) 2023
 * Authors:
 *	spacebub <spacebubs@proton.me>
 */
#include <stdio.h>
#include <string.h>
#include "common/wad.h"

static int fill_args(list_t* output, char** args, int start, int argc);
static void combine_wads(list_t* paths);
static void list_map_names(list_t* paths);
static void list_lump_names(list_t* paths);

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		printf("At least one argument must be provided!\n");
		return 1;
	}

	for (int i = 0; i < argc;)
	{
		if (strcmp(argv[i], "--list-maps") == 0)
		{
			list_t* args = list_new(2);
			int count = fill_args(args, argv, i, argc);

			if (count < 1)
			{
				printf("Invalid arguments for --list-maps!");
				list_free(args);
				return 1;
			}
			
			list_map_names(args);

			list_free(args);
			i += count;
			continue;
		}

		if (strcmp(argv[i], "--list-lumps") == 0)
		{
			list_t* args = list_new(2);
			int count = fill_args(args, argv, i, argc);

			if (count < 1)
			{
				printf("Invalid arguments for --list-lumps!");
				list_free(args);
				return 1;
			}

			list_lump_names(args);

			list_free(args);
			i += count;
			continue;
		}
		
		if (strcmp(argv[i], "--combine-wads") == 0)
		{
			list_t* args = list_new(2);
			int count = fill_args(args, argv, i, argc);
			
			if (count < 1)
			{
				printf("Invalid arguments for --combine-wads!");
				list_free(args);
				return 1;
			}
			
			combine_wads(args);
			
			list_free(args);
			i += count;
			continue;
		}
		
		i++;
	}

	return 0;
}

static int fill_args(list_t* output, char** args, int start, int argc)
{
	int i = start + 1;
	
	while(i < argc && args[i][0] != '-')
	{
		list_add(output, args[i], (int)strlen(args[i]));
		i++;
	}
	
	return i - start - 1;
}

static void list_map_names(list_t* paths)
{
	list_t* mapnames;
	
	for (int i = 0; i < paths->length; ++i)
	{
		mapnames = wad_map_names_path(paths->values[i]);
		
		if (!mapnames)
		{
			printf("Invalid path: %s\n", (char*)paths->values[i]);
			continue;
		}
		
		for (int j = 0; j < mapnames->length; ++j)
		{
			printf("%s\n", (char*)mapnames->values[j]);
		}
		list_free(mapnames);
	}
}

static void list_lump_names(list_t* paths)
{
	list_t* lumpnames;

	for (int i = 0; i < paths->length; ++i)
	{
		lumpnames = wad_lump_names_path(paths->values[i]);

		if (!lumpnames)
		{
			printf("Invalid path: %s\n", (char*)paths->values[i]);
			continue;
		}

		for (int j = 0; j < lumpnames->length; ++j)
		{
			printf("%s\n", (char*)lumpnames->values[j]);
		}
		list_free(lumpnames);
	}
}

static void combine_wads(list_t* paths)
{
	wad_t* wad = wad_combine((const char**)paths->values, paths->length);
	wad_write(wad, "./combined.wad");
	wad_free(wad);
}

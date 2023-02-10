// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Part of wadtools
 * 
 * Provides basic reading and writing of WAD files
 *
 * Copyright (c) 2023 spacebub
 * Authors:
 *	spacebub <spacebubs@proton.me>
 */
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "wad.h"

// The first lump name after the map lump
#define MAP_IDENTIFIER "THINGS"
// The max length of a lump name according to specification
#define LUMP_NAME_LENGTH 8

typedef struct
{
	size_t lumpdatalength;
	lumpbundle_t* lumpdata;
	int numlumps;
} combination_metadata_t;

static combination_metadata_t* get_metadata(wad_t** wads, int length);
static void free_metadata(combination_metadata_t* metadata);
static void set_header(wad_t* receiving, combination_metadata_t* metadata, const char wadtype[4]);
static void set_data(wad_t* receiving, combination_metadata_t* metadata);
static void set_lumps(wad_t* receiving, combination_metadata_t* metadata);

wad_t*
wad_open(const char* path)
{
	FILE* fp = fopen(path, "r");

	if (!fp)
	{
		return NULL;
	}

	// Read the single WAD header into the corresponding struct
	wadheader_t header;
	fread(&header, sizeof(wadheader_t), 1, fp);

	// Read the lump headers into an array of the corresponding structs
	wadlump_t* lumps = malloc(header.numlumps * sizeof(wadlump_t));
	fseek(fp, header.lumpoffset, 0);
	fread(lumps, sizeof(wadlump_t), header.numlumps, fp);

	wad_t* wad = malloc(sizeof(wad_t));
	wad->header = header;

	lumpbundle_t* lumpbundles = malloc(sizeof(lumpbundle_t) * header.numlumps);
	for (int i = 0; i < header.numlumps; ++i)
	{
		lumpbundle_t lumpbundle;

		char* buffer = malloc(sizeof(char) * lumps[i].length);

		// Jump to the data sector defined by the lump header and read its data into
		// the above preallocated buffer
		fseek(fp, lumps[i].offset, 0);
		fread(buffer, sizeof(char), lumps[i].length, fp);

		lumpbundle.lump = lumps[i];
		lumpbundle.buffer = buffer;

		lumpbundles[i] = lumpbundle;
	}

	wad->lumpbundles = lumpbundles;

	free(lumps);
	fclose(fp);
	return wad;
}

void
wad_free(wad_t* wad)
{
	if (!wad)
	{
		return;
	}
	
	for (int i = 0; i < wad->header.numlumps; ++i)
	{
		free(wad->lumpbundles[i].buffer);
	}

	free(wad->lumpbundles);
	free(wad);
}

int
wad_write(wad_t* wad, const char* path)
{
	if (!wad)
	{
		return 1;
	}
	
	FILE* fp = fopen(path, "w");

	if (!fp)
	{
		return 1;
	}

	// Write the single WAD header to a file at the specified path
	fwrite(&wad->header, sizeof(wadheader_t), 1, fp);

	for (int i = 0; i < wad->header.numlumps; ++i)
	{
		// Move the file pointer to the specified by the header lump offset
		// and write the lump header to file. Since the lumps are written
		// one by one, the lump offset is used as a baseline and each
		// subsequent lump adds its size to the offset
		fseek(fp, wad->header.lumpoffset + i * (long)sizeof(wadlump_t), 0);
		fwrite(&wad->lumpbundles[i].lump, sizeof(wadlump_t), 1, fp);

		// Move the file pointer to the specified by the lump data offset
		// and write all the data associated with the lump to the file
		fseek(fp, wad->lumpbundles[i].lump.offset, 0);
		fwrite(wad->lumpbundles[i].buffer, wad->lumpbundles[i].lump.length, 1, fp);
	}

	fclose(fp);

	return 0;
}

list_t*
wad_map_names(wad_t* wad)
{
	if (!wad)
	{
		return NULL;
	}

	list_t* strlist = list_new(2);

	// Generally the WAD structure follows a simple layout,
	// and we can assume that it will hold for most WADs.
	// In most cases map lumps follow the pattern:
	// MAPNAME
	// THINGS
	// ...
	// so we take the first lump name preceding THINGS
	char* previous = NULL;
	for (int i = 0; i < wad->header.numlumps; ++i)
	{
		if (strcmp(wad->lumpbundles[i].lump.name, MAP_IDENTIFIER) == 0 && previous)
		{
			list_add(strlist, previous, LUMP_NAME_LENGTH);
		}

		previous = wad->lumpbundles[i].lump.name;
	}

	return strlist;
}

list_t*
wad_map_names_path(const char* path)
{
	list_t* strlist = list_new(2);

	FILE* fp = fopen(path, "r");

	if (!fp)
	{
		return NULL;
	}

	wadheader_t header;
	fread(&header, sizeof(wadheader_t), 1, fp);

	wadlump_t* lumps = malloc(header.numlumps * sizeof(wadlump_t));
	fseek(fp, header.lumpoffset, 0);
	fread(lumps, sizeof(wadlump_t), header.numlumps, fp);

	// Generally the WAD structure follows a simple layout,
	// and we can assume that it will hold for most WADs.
	// In most cases map lumps follow the pattern:
	// MAPNAME
	// THINGS
	// ...
	// so we take the first lump name preceding THINGS
	char* previous = NULL;
	for (int i = 0; i < header.numlumps; ++i)
	{
		if (strcmp(lumps[i].name, MAP_IDENTIFIER) == 0 && previous)
		{
			list_add(strlist, previous, LUMP_NAME_LENGTH);
		}

		previous = lumps[i].name;
	}

	free(lumps);
	fclose(fp);
	return strlist;
}

list_t*
wad_lump_names(wad_t* wad)
{
	if (!wad)
	{
		return NULL;
	}

	list_t* strlist = list_new(2);

	for (int i = 0; i < wad->header.numlumps; ++i)
	{
		list_add(strlist, wad->lumpbundles[i].lump.name, LUMP_NAME_LENGTH);
	}

	return strlist;
}

list_t*
wad_lump_names_path(const char* path)
{
	list_t* strlist = list_new(2);

	FILE* fp = fopen(path, "r");

	if (!fp)
	{
		return NULL;
	}

	wadheader_t header;
	fread(&header, sizeof(wadheader_t), 1, fp);

	wadlump_t* lumps = malloc(header.numlumps * sizeof(wadlump_t));
	fseek(fp, header.lumpoffset, 0);
	fread(lumps, sizeof(wadlump_t), header.numlumps, fp);

	for (int i = 0; i < header.numlumps; ++i)
	{
		list_add(strlist, lumps[i].name, LUMP_NAME_LENGTH);
	}

	free(lumps);
	fclose(fp);
	return strlist;
}

wad_t*
wad_combine(const char** paths, int length)
{
	wad_t* combinedwad = malloc(sizeof(wad_t));
	wad_t** wad_list = malloc(sizeof(wad_t*) * length);
	
	for (int i = 0; i < length; ++i)
	{
		if (!(wad_list[i] = wad_open(paths[i])))
		{
			for (int j = 0; j < i; ++j)
			{
				wad_free(wad_list[j]);
			}
			free(wad_list);
			free(combinedwad);
			
			return NULL;
		}
	}

	combination_metadata_t* metadata = get_metadata(wad_list, length);
	set_header(combinedwad, metadata, "IWAD");
	set_lumps(combinedwad, metadata);
	set_data(combinedwad, metadata);
	free_metadata(metadata);

	for (int i = 0; i < length; ++i)
	{
		wad_free(wad_list[i]);
	}
	free(wad_list);

	return combinedwad;
}

static combination_metadata_t*
get_metadata(wad_t** wads, int length)
{
	combination_metadata_t* metadata = malloc(sizeof(combination_metadata_t));
	metadata->lumpdatalength = 0;
	metadata->numlumps = 0;

	for (int i = 0; i < length; ++i)
	{
		metadata->numlumps += wads[i]->header.numlumps;
	}

	if (!(metadata->lumpdata = malloc(sizeof(lumpbundle_t) * metadata->numlumps)))
	{
		return NULL;
	};

	int bufflength;
	for (int iwad = 0, ilump = 0; iwad < length; ++iwad)
	{
		for (int j = 0; j < wads[iwad]->header.numlumps; ++j)
		{
			bufflength = wads[iwad]->lumpbundles[j].lump.length;

			metadata->lumpdata[ilump].lump.length = bufflength;
			metadata->lumpdata[ilump].lump = wads[iwad]->lumpbundles[j].lump;

			metadata->lumpdata[ilump].buffer = malloc(sizeof(char) * bufflength);
			memcpy(metadata->lumpdata[ilump].buffer, wads[iwad]->lumpbundles[j].buffer, bufflength);

			metadata->lumpdatalength += bufflength;
			ilump++;
		}
	}

	return metadata;
}

static void
set_header(wad_t* receiving, combination_metadata_t* metadata, const char wadtype[4])
{
	wadheader_t header;
	header.lumpoffset = (int)(metadata->lumpdatalength * sizeof(char) + sizeof(wadheader_t));
	header.numlumps = metadata->numlumps;
	memcpy(header.type, wadtype, 4);

	receiving->header = header;
}

static void
set_data(wad_t* receiving, combination_metadata_t* metadata)
{
	int currentoffset = sizeof(wadheader_t);

	for (int i = 0; i < metadata->numlumps; ++i)
	{
		receiving->lumpbundles[i].lump.offset = currentoffset;
		receiving->lumpbundles[i].buffer = metadata->lumpdata[i].buffer;
		currentoffset += (int)metadata->lumpdata[i].lump.length;
	}
}

static void
set_lumps(wad_t* receiving, combination_metadata_t* metadata)
{
	lumpbundle_t* lumpbundles = malloc(sizeof(lumpbundle_t) * metadata->numlumps);

	for (int i = 0; i < metadata->numlumps; ++i)
	{
		lumpbundles[i].lump = metadata->lumpdata[i].lump;
	}

	receiving->lumpbundles = lumpbundles;
}

static void
free_metadata(combination_metadata_t* metadata)
{
	free(metadata->lumpdata);
	free(metadata);
}

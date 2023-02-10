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
#ifndef _WAD_H_
#define _WAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "list.h"

// The following struct layouts should not change

// WAD header layout as defined by the original Doom source code
typedef struct
{
	char type[4];
	int numlumps;
	int lumpoffset;
} wadheader_t;

// WAD lump layout as defined by the original Doom source code
typedef struct
{
	int offset;
	int length;
	char name[8];
} wadlump_t;

// Easier to use container for lumps, bundling together the lump
// info with lump data in the form of a char buffer.
typedef struct
{
	wadlump_t lump;
	char* buffer;
} lumpbundle_t;

// Base type used by the wad implementation.
// Should ideally not be manually created or
// disposed of, instead use wad_open for creation
// and wad_free for deletion.
typedef struct
{
	wadheader_t header;
	lumpbundle_t* lumpbundles;
	const char* filename;
} wad_t;

// Reads WAD file at given path and returns a convenient to use
// wad_t with the header, the lump headers and the lump data
// all in one.
wad_t*
wad_open(const char* path);
// Returns a list_t with all the map names of a given wad_t
list_t*
wad_map_names(wad_t* wad);
// Returns a list_t with all the map names defined in the
// wad lumps of the WAD at the specified location.
list_t*
wad_map_names_path(const char* path);
// Returns a list_t with all the lump names of a given wad_t.
list_t*
wad_lump_names(wad_t* wad);
// Returns a list_t with all the lump names of the WAD
// at the specified location.
list_t*
wad_lump_names_path(const char* path);
// Writes given wad_t to a WAD file at given path.
int
wad_write(wad_t* wad, const char* path);
// Frees data associated with the wad_t generated by wad_open.
void
wad_free(wad_t* wad);
// Reads a number of WADs from the provided paths and
// combines them into a single wad, making sure to
// correctly reorder the lump data and update the
// header with the new lump positions.
wad_t*
wad_combine(const char** paths, int length);

#ifdef __cplusplus
}
#endif

#endif //_WAD_H_
